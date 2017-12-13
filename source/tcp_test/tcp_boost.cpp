#include "boost/asio.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <iostream>
#include <string>

using namespace boost::asio;

class tcp_server{
public:
	tcp_server(const std::string & ip, unsigned short port): m_addr(m_addr.from_string(ip)), 
		m_port(port) {
		
	}


	~tcp_server(){

	}

	void start(){
		try{
			std::cout << "server sart" << std::endl;
			io_service	ios;
			ip::tcp::acceptor acceptor(ios, ip::tcp::endpoint(m_addr, m_port));
			
			while(true) {
				ip::tcp::socket sock(ios);
				acceptor.accept(sock);
				char str[100] = {0};
				sock.read_some(buffer(str));
        		std::cout << "receive form client: " << sock.remote_endpoint().address() << ":" 
						  << sock.remote_endpoint().port() << std::endl;
        		sock.write_some(buffer("hello client"));
			}
		} catch (std::exception& e) {
			std::cout << "exception:" << e.what() << std::endl; 
		}
	}

private:
    ip::address		m_addr;
	unsigned short	m_port;
};


class tcp_client{
public:
	tcp_client(const std::string & ip, unsigned short port): m_addr(m_addr.from_string(ip)), 
		m_port(port), m_run(false){

	}


	~tcp_client(){

	}

	void start() {
		try {  
			while(true) {
				ip::tcp::socket sock(m_ios);  
    			ip::tcp::endpoint ep(m_addr, m_port);  
				sock.connect(ep);
    			
				char str[100] = {0};
				sock.write_some(buffer("helo server!"));
    			sock.read_some(buffer(str));  
    			std::cout << "receive from server:" << sock.remote_endpoint().address() << ":"
					 	  << sock.remote_endpoint().port() << std::endl;
				
				boost::asio::io_service io;
		        boost::asio::deadline_timer timer(io, boost::posix_time::seconds(1));
        		timer.wait();
			}
		} catch (std::exception &e) {  
    		std::cout << e.what() << std::endl;  
			m_run = false;
		} 		
	}

private:
    ip::address		m_addr;
	unsigned short	m_port;
    io_service 		m_ios;
	bool			m_run;
};

void print_help(){
	std::cout << "-s ip port" << std::endl;
	std::cout << "-c ip prot" << std::endl;
}

int main(int arvc, char ** argv) {
	if ((arvc >= 4) && (std::string(argv[1]) == "-s")) {
		std::stringstream sFmt;
		sFmt << argv[3];
		unsigned short port;
		sFmt >> port;
		tcp_server server(argv[2], port);
		server.start();
		
	} else if((arvc >= 4) && (std::string(argv[1]) == "-c")) {
		std::stringstream sFmt;
		sFmt << argv[3];
		unsigned short port;
		sFmt >> port;
		tcp_client client(argv[2], port);
		client.start();
	} else {
		print_help();
	}

	return 0;
};
