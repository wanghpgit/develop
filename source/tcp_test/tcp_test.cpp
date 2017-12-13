#include <iostream>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <csignal>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "process/process.hpp"

class tcp_server{
public:
	tcp_server() : m_epfd(0){
	}

	tcp_server(const std::string & ip, unsigned short port, size_t worker_count=DEF_WORKER_COUNT): m_epfd(0){
		set(ip, port, worker_count);
	}

	~tcp_server(){

	}

	void set(const std::string & ip, unsigned short port, size_t worker_count=DEF_WORKER_COUNT) {
		m_ip = ip;
		m_port = port;
		m_worker_count = worker_count;
	}
	
	void start() {
		if(!init()) {
			unInit();
			return;
		}
		
		if(!fork_proc()) {
			unInit();
			return;
		}
	}

	void stop() {
		killall_child_process();
	}

private:
	bool init() {	
		//struct epoll_event ev, evs[20];//最大并发连接数量
		int epfd = epoll_create(MAX_EPOLL_COUNT);
    	if (-1 == epfd) {
			std::cout << "epoll_create failed! errno:" << errno << std::endl;
			return false;
    	}
	
		int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (-1 == server_sockfd) {
			std::cout << "socket failed! errno:" << errno << std::endl;
			close(epfd);
			return false;
    	}	

		struct epoll_event ev;
		ev.data.fd = server_sockfd;
		ev.events = EPOLLIN | EPOLLET;
		int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, server_sockfd, &ev);
		if (-1 == ret) {
			std::cout << "server epoll_ctl failed! errno:" << errno << std::endl;
			close(epfd);
			close(server_sockfd);
			return false;	
		}

		struct sockaddr_in server_address;	
		server_address.sin_family = AF_INET; 
		server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
		server_address.sin_port = htons(m_port); 
		ret = bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address)); 
		if (-1 == ret) {
			std::cout << "bind failed! errno:" << errno << std::endl;
			close(epfd);
			close(server_sockfd);
			return false;	
		}
	
		ret = listen(server_sockfd, MAX_CONNECTION_COUNT); 
		if (-1 == ret) {
			std::cout << "listen failed! errno:" << errno << std::endl;
			close(epfd);
			close(server_sockfd);
			return false;	
		}
				
		m_socket = server_sockfd;
		m_epfd = epfd;
		return true;
	}

	void unInit() {
		if (0 != m_socket) {
			close(m_socket), m_socket = 0;
		}
		
		if (0 != m_epfd) {
			close(m_epfd), m_epfd = 0;
		}
	}

	bool fork_proc() {
		for (size_t i=0; i<m_worker_count; ++i) {
			pid_t fpid = fork();
			if (0 > fpid) {
				std::cout << "fork " << i << " failed!" << std::endl;
				killall_child_process();
				return false;
			} else if ( 0 == fpid) {
				prctl(PR_SET_NAME, (unsigned long)"tcp_test:worker", NULL, NULL, NULL); 
				signal( SIGINT, tcp_server::sig_cbf);		
				child_proc_worker();
			} else {
				m_vtPid.push_back(fpid);
			}
		}
		return true;
	}

	void killall_child_process() {
		if(m_vtPid.empty()) {
			if (0 != wanghp::process::get_pid_by_name("tcp_test:worker", m_vtPid)) {
				std::cout << "get child process pid failed!" << std::endl;
			}
		}

		for(std::vector<pid_t>::const_iterator cit = m_vtPid.begin();
					cit != m_vtPid.end(); ++cit){
			if (0 != wanghp::process::send_signal_by_pid(*cit, 3)) {
				std::cout << "stop pid [" << *cit << "] failed!" << std::endl;
			}
		}
	}

	/*负责外部通信的子进程 此进程只允许有一个*/
	void child_proc_communication() {

	}

	/*负责工作的子进程 可以有多个*/
	void child_proc_worker() {
		std::cout << "\npid:" << getpid() << "start" << std::endl;

		boost::interprocess::permissions per;
		per.set_unrestricted();
		boost::interprocess::named_mutex namedMutex(boost::interprocess::open_or_create, "tcp_test:worker",per);
		while(m_run) {
			struct epoll_event evs[MAX_CONNECTION_COUNT];	
			struct epoll_event ev;
			int client_sockfd = 0;
			//boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock(namedMutex);
			std::cout << "epoll_wait" << std::endl;
			int event_count = epoll_wait(m_epfd, evs, MAX_CONNECTION_COUNT, EPOLL_TIMEOUT); 
			for(int i = 0; i < event_count; ++i) {
				if ( evs[i].data.fd == m_socket) {
					struct sockaddr_in client_address;
					socklen_t client_len = sizeof(client_address); 
					client_sockfd = accept(m_socket, (struct sockaddr *)&client_address, &client_len); 
					if (-1 == client_sockfd) {
						std::cout << "accept failed! errno:" << errno << std::endl;
						continue;
					}
				
					char * ip_str = inet_ntoa(client_address.sin_addr);
					std::cout << "\taccept a connection from:" << ip_str << std::endl;
				
					ev.data.fd = client_sockfd;
					ev.events = EPOLLIN | EPOLLET;
					int ret = epoll_ctl(m_epfd, EPOLL_CTL_ADD, client_sockfd, &ev);
					if (-1 == ret) {
						std::cout << "\tclient epoll_ctl failed! errno:" << errno << std::endl;
					}	
				} else if ( evs[i].events & EPOLLIN ) {
					std::cout << "\tEPOLLIN:" << evs[i].data.fd << std::endl;;
					if ((client_sockfd = evs[i].data.fd) < 0) {
						std::cout << "\tclient_sockfd < 0" << std::endl;
						continue;
					}
					
					std::string buff(MAX_MSG_LEN+1,0); 
					int len = read(client_sockfd, &buff[0], MAX_MSG_LEN);
					if (0 > len) {
						if (ECONNRESET == errno) {
							close(client_sockfd);
							evs[i].data.fd = -1;						
						} else {
							std::cout << "\tread error! errno:" << errno << std::endl;
						}
					} else if (0 == len) {
						buff.resize(len);
						std::cout << "\tnot msg" << std::endl;
						close(client_sockfd);
						evs[i].data.fd = -1;
					} else {
						std::cout << "\tfrom client:" << buff << std::endl;
						ev.data.fd=client_sockfd;
						ev.events = EPOLLOUT | EPOLLET;
						int ret = epoll_ctl(m_epfd, EPOLL_CTL_MOD, client_sockfd, &ev);
						if (-1 == ret) {
							std::cout << "\tclient epoll_ctl EPOLL_CTL_MOD failed! errno:" << errno << std::endl;
						}	
					
					}
				} else if (evs[i].events & EPOLLOUT) {
					std::cout << "\tEPOLLOUT " << evs[i].data.fd << std::endl;
					client_sockfd = evs[i].data.fd;
					const std::string str_res = "1234567890";
					write(client_sockfd, str_res.c_str(), str_res.length());
					ev.data.fd = client_sockfd;
					ev.events = EPOLLIN | EPOLLET;
					int ret = epoll_ctl(m_epfd, EPOLL_CTL_MOD, client_sockfd, &ev);
					if (-1 == ret) {
						std::cout << "\tclient epoll_ctl failed! errno:" << errno << std::endl;
					}	
				} else {
					if (evs[i].events & EPOLLERR ) {
						std::cout << "\t" << evs[i].data.fd << " 'EPOLLERR'\n" << std::endl;
					} else if (evs[i].events & EPOLLPRI) {
						std::cout << "\t" << evs[i].data.fd << " 'EPOLLPRI'\n" << std::endl;
					} else if (evs[i].events & EPOLLHUP) {
						std::cout << "\t" << evs[i].data.fd << " 'EPOLLHUP'\n" << std::endl;
					} else if (evs[i].events & EPOLLONESHOT) {
						std::cout << "\t" << evs[i].data.fd << " 'EPOLLONESHOT'\n" << std::endl;
					} else {
						std::cout << "\t" << evs[i].data.fd << " 'unkonw'\n" << std::endl;
					}				
				}			
			}
		}
	
		std::cout << "pid:" << getpid() << " stop" << std::endl;
	}

	static void sig_cbf(int sig) {
		if ( sig == SIGINT) {
			std::cout << "ctrl C" << std::endl;
			m_run = false;
		}
	}

	std::string 				m_ip;
	unsigned short				m_port;
	size_t						m_worker_count;
	std::vector<pid_t>			m_vtPid;
	static bool					m_run;
	int							m_epfd;
	int							m_socket;
	
	static const int			MAX_CONNECTION_COUNT;
	static const int			MAX_EPOLL_COUNT;
	static const int			EPOLL_TIMEOUT;
	static const int			MAX_MSG_LEN;
	static const size_t			DEF_WORKER_COUNT;
};

bool tcp_server::m_run = true;
const int tcp_server::MAX_CONNECTION_COUNT = 100;
const int tcp_server::MAX_EPOLL_COUNT = 256;
const int tcp_server::EPOLL_TIMEOUT = 1000;
const int tcp_server::MAX_MSG_LEN = 1024;
const size_t tcp_server::DEF_WORKER_COUNT = 1;

class tcp_client{
public:
	tcp_client(){
	}

	~tcp_client(){

	}
};

void print_help() {
	std::cout << "-s start [ip] [port] [work process count]" << std::endl;
	std::cout << "-s stop" << std::endl;
	std::cout << "-c [ip] [port]" << std::endl;
}

int main(int argc, char ** argv) {
	if (argc < 3 ) {
		print_help();
		return 0;
	}
	
	if ("-s" == std::string(argv[1])) {
		if ("stop" == std::string(argv[2])) {
			std::cout << "stop server" << std::endl;
			tcp_server server;
			server.stop();
		} else if(("start" == std::string(argv[2])) && (argc >=6)) {
			unsigned short port;
			size_t worker_count;
			std::stringstream strFmt;
			strFmt << argv[4];
			strFmt >> port;
			strFmt.clear();
			strFmt.str("");
			strFmt << argv[5];
			strFmt >> worker_count;
			std::cout << "start server ip[" << argv[2] << "] port[" << port 
					  << "] worker count [" << worker_count << "]" << std::endl;	
			tcp_server server(argv[2], port, worker_count);
			server.start();	
		} else {
			print_help();
		}
	} else if (("-c" == std::string(argv[1])) && (argc >= 4)) {
		unsigned short port;
		std::stringstream strFmt;
		strFmt << argv[3];
		strFmt >> port;

		std::cout << "start client ip[" << argv[2] << "] port[" << port 
				  << "]" << std::endl;
		
	} else {
		print_help();
	}
	
	return 0;
}
