#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h> 

int main(int argc, char *argv[]) {

	if ( argc == 3 ) {
		/* Variables for initial set up 
		   Socket File Descriptor
		   IPv4 Address
		   Port                      */
		int sock_fd;
		char ip_addr[32];
		char port[16];

		/* Communication between the server
		   Bytes storage
		   Response String
		   Recieve String 
		   Buffer Size 
		   String Length                  */
		ssize_t bytes;
		const int BUFFER = 1024;
		char response[BUFFER];
		char recieve[BUFFER];

		struct addrinfo hints, *server;

		strncpy(ip_addr,argv[1],sizeof(ip_addr)-1);
		strncpy(port,argv[2],sizeof(port)-1);

		memset(&hints,0,sizeof(struct addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = 0;
		hints.ai_flags = 0;

		if (getaddrinfo(ip_addr,port,&hints,&server) < 0) {
			fprintf(stderr,"Could not setup address info\n");
			exit(1);
		}
		
		sock_fd = socket(server->ai_family,server->ai_socktype,server->ai_protocol);
		if (sock_fd < 0) {
			fprintf(stderr,"Could not create socket.\n");
			freeaddrinfo(server);
			exit(1);
		}

		if (connect(sock_fd,server->ai_addr,server->ai_addrlen) < 0) {
			fprintf(stderr,"Could not connect to server.\n");
			close(sock_fd);
			freeaddrinfo(server);
			exit(1);
		}

		freeaddrinfo(server);

		while(1) {

			/* Ensure that the bytes are zero out before
			   sending and recieve to/from client/server */
			memset(recieve,0,BUFFER);
			memset(response,0,BUFFER);

			bytes = recv(sock_fd,recieve,BUFFER,0);
			if ( bytes < 0 ) {
				fprintf(stderr,"Could not recieve information from the server...\n");
				fprintf(stderr,"Closing connection...\n");
				close(sock_fd);
				exit(1);
			}

			printf("Server: %s",recieve);

			printf("Me: ");
			fgets(response,BUFFER,stdin);
			bytes = send(sock_fd,response,strnlen(response,BUFFER),0);
			if ( bytes < 0 ) {
				fprintf(stderr,"Could not send information to server...\n");
				fprintf(stderr,"Closing connection...\n");
				close(sock_fd);
			}

			if ( strncmp(response,"quit\n",BUFFER) == 0) {
				printf("Exiting chat...\n");
				break;
			}
		}
	close(sock_fd);
	return 0;
	}
	else {
		printf("%s needs two arguments [IPv4] [Port]\n",argv[0]);
		printf("Example: %s 192.168.0.xxx 8080\n",argv[0]);
		return -1;
	}
}
