#include <sys/types.h> 
#include <sys/socket.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <netinet/in.h> 
#include <sys/time.h> 
#include <sys/ioctl.h> 
#include <unistd.h> 

int main() 
{ 
	int server_sockfd, client_sockfd; 
	socklen_t server_len, client_len; 
	struct sockaddr_in server_address; 
	struct sockaddr_in client_address; 
	int result; 
	fd_set readfds, testfds; 
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);//������������socket 
	server_address.sin_family = AF_INET; 
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
	server_address.sin_port = htons(9734); 
	server_len = sizeof(server_address); 
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len); 
	listen(server_sockfd, 5); 
	FD_ZERO(&readfds); 
	FD_SET(server_sockfd, &readfds);//����������socket���뵽������
	while(1) 
	{
		char ch;                
		int fd; 
		int nread; 
		testfds = readfds; 
		printf("server waiting\n"); 

		/*�������������������ļ��������䶯 */
		result = select(FD_SETSIZE, &testfds, (fd_set *)0,(fd_set *)0, (struct timeval *) 0); 
		if(result < 1) 
		{ 
			perror("server5\n");
			exit(1); 
		} 

		/*ɨ�����е��ļ�������*/
		for(fd = 0; fd < FD_SETSIZE; fd++) 
		{
			/*�ҵ�����ļ�������*/
			if(FD_ISSET(fd,&testfds)) 
			{ 
		     	        /*�ж��Ƿ�Ϊ�������׽��֣������ʾΪ�ͻ��������ӡ�*/
				if(fd == server_sockfd) 
				{ 
					client_len = sizeof(client_address); 
					client_sockfd = accept(server_sockfd, 
					(struct sockaddr *)&client_address, &client_len); 
					FD_SET(client_sockfd, &readfds);//���ͻ���socket���뵽������
					printf("adding client on fd %d/n", client_sockfd); 
				} 

				/*�ͻ���socket������������ʱ*/
				else 
				{                                          
					ioctl(fd, FIONREAD, &nread);//ȡ������������nread
					
					/*�ͻ�����������ϣ��ر��׽��֣��Ӽ����������Ӧ������ */
					if(nread == 0) 
					{ 
						close(fd); 
						FD_CLR(fd, &readfds); 
						printf("removing client on fd %d\n", fd); 
					} 

					/*����ͻ���������*/
					else 
					{ 
						//printf("serving client on fd %d\n", fd); 
						read(fd, &ch, 1);
						printf("\tchar from client: %c\n", ch); 
						sleep(1); 
						ch++; 
						write(fd, &ch, 1); 
						printf("\tchar to client: %c\n", ch);
					} 
				} 
			} 
		} 
	} 
} 
