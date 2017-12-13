#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>                                                                    
#include <arpa/inet.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>

int main() 
{ 
	int client_sockfd; 
	int len; 
	struct sockaddr_in address;
	int result; 
    
	client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(9734); 
	len = sizeof(address); 
	result = connect(client_sockfd, (struct sockaddr *)&address, len); 
	if(result == -1) 
	{ 
		perror("oops: client2 "); 
		exit(1); 
	}

	char msg[] = "client msg A";
	char buff[100] = {0};
	for (char ch = 'A'; ch<'Z'; ++ch) {
    		msg[sizeof(msg)-2] = ch;    	 
		write(client_sockfd, msg, strlen(msg));
		printf("char to server: %s\n", msg); 
		read(client_sockfd, buff, sizeof(buff)-1); 
    		printf("char from server: %s\n", buff); 
	}

	close(client_sockfd); 
	exit(0); 
}                      
