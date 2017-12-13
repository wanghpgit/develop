#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h> 
#include <sys/ioctl.h>
#include <sys/epoll.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <errno.h>

const int MAX_CONNECTION_COUNT = 100;
const int MAX_EPOLL_COUNT = 256;
const int EPOLL_TIMEOUT = -1;
const int MAX_MSG_LEN = 1024;

int main() { 
	int server_sockfd, client_sockfd, epfd; 
	socklen_t server_len, client_len; 
	struct sockaddr_in server_address; 
	struct sockaddr_in client_address; 
	int result; 
	struct epoll_event ev, evs[MAX_CONNECTION_COUNT];
	
	epfd = epoll_create(MAX_EPOLL_COUNT);
	if (-1 == epfd) {
			printf("epoll_create failed! errno: %d\n", errno);
			return 1;
	}
	
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);//建立服务器端socket 
	if (-1 == server_sockfd) {
		printf("socket failed! errno: %d\n", errno);
		close(epfd);	
		return 2;	
	}
		
	ev.data.fd = server_sockfd;
	ev.events = EPOLLIN | EPOLLET;
	result = epoll_ctl(epfd, EPOLL_CTL_ADD, server_sockfd, &ev);
	if (-1 == result) {
		printf("server epoll_ctl failed! errno: %d\n", errno);
		close(epfd);
		close(server_sockfd);
		return 3;	
	}
	
	server_address.sin_family = AF_INET; 
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
	server_address.sin_port = htons(9734); 
	server_len = sizeof(server_address); 
	result = bind(server_sockfd, (struct sockaddr *)&server_address, server_len); 
	if (-1 == result) {
		printf("bind failed! errno: %d\n", errno);
		close(epfd);
		close(server_sockfd);
		return 3;	
	}
	
	result = listen(server_sockfd, MAX_CONNECTION_COUNT); 
	if (-1 == result) {
		printf("listen failed! errno: %d\n", errno);
		close(epfd);
		close(server_sockfd);
		return 3;	
	}
	
	while(true) {               
		int i; 
		int nread;
		int event_count;
		printf("server waiting\n"); 

		event_count = epoll_wait(epfd, evs, MAX_CONNECTION_COUNT, EPOLL_TIMEOUT); 
		for(i = 0; i < event_count; ++i) {//扫描所有事件
			
			if ( evs[i].data.fd == server_sockfd) {//客户端请求链接
				client_len = sizeof(client_address); 
				client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len); 
				if (-1 == client_sockfd) {
					printf("\taccept failed! errno: %d\n", errno);
					continue;
				}
				
				char * ip_str = inet_ntoa(client_address.sin_addr);
				printf("\taccept a connection from: %s\n",ip_str);
				
				ev.data.fd = client_sockfd;
				ev.events = EPOLLIN | EPOLLET;
				result = epoll_ctl(epfd, EPOLL_CTL_ADD, client_sockfd, &ev);
				if (-1 == result) {
					printf("\tclient epoll_ctl failed! errno: %d\n", errno);
				}	
			} else if ( evs[i].events & EPOLLIN ) {//收到已连接客户端数据
				printf("\tEPOLLIN %d\n", evs[i].data.fd);
				if ((client_sockfd = evs[i].data.fd) < 0) {
					printf("\tclient_sockfd < 0\n");
					continue;
				}
				
				char buff[MAX_MSG_LEN+1] = {0}; 
				int len = read(client_sockfd, buff, MAX_MSG_LEN);
				if (0 > len) {
					if (ECONNRESET == errno) {
						close(client_sockfd);
						evs[i].data.fd = -1;						
					} else {
						printf("\tread error! errno:%d\n",errno);
					}
				} else if (0 == len) {
						printf("\tnot msg\n");
						close(client_sockfd);
						evs[i].data.fd = -1;
				} else {
					printf("\tfrom client: %s\n", buff);
					ev.data.fd=client_sockfd;
					ev.events = EPOLLOUT | EPOLLET;
					result = epoll_ctl(epfd, EPOLL_CTL_MOD, client_sockfd, &ev);
					if (-1 == result) {
						printf("\tclient epoll_ctl EPOLL_CTL_MOD failed! errno: %d\n", errno);
					}	
					
				}				
			} else if (evs[i].events & EPOLLOUT) {
				printf("\tEPOLLOUT %d\n", evs[i].data.fd);
				client_sockfd = evs[i].data.fd;
				const char * str_res = "1234567890";
				write(client_sockfd, str_res, strlen(str_res));
				ev.data.fd = client_sockfd;
				ev.events = EPOLLIN | EPOLLET;
				result = epoll_ctl(epfd, EPOLL_CTL_MOD, client_sockfd, &ev);
				if (-1 == result) {
					printf("\tclient epoll_ctl failed! errno: %d\n", errno);
				}	
			} else {
				if (evs[i].events & EPOLLERR ) {
					printf("\t%d 'EPOLLERR'\n", evs[i].data.fd);
				} else if (evs[i].events & EPOLLPRI) {
					printf("\t%d 'EPOLLPRI'\n", evs[i].data.fd);
				} else if (evs[i].events & EPOLLHUP) {
					printf("\t%d 'EPOLLHUP'\n", evs[i].data.fd);
				} else if (evs[i].events & EPOLLONESHOT) {
					printf("\t%d 'EPOLLONESHOT'\n", evs[i].data.fd);
				} else {
					printf("\t%d 'unkonw'\n", evs[i].data.fd);
				}				
			}			
		} 
	} 
} 
