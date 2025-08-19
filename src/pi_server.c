#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

void usage(char *input[]);
void format(int count, const char character);

int main(int argc, char *argv[]) {
	
	/* This will take a command line argument:
       Localhost: 127.0.0.1
	   IPv4 Address: 192.168.0.x  */
	if ( argc == 3 ) {
		
		int sock_fd, client_fd;
		struct addrinfo hints, *server;

		const int b_addr = 32; 
		const int b_port = 16;
		char ip_addr[b_addr];
		char port[b_port];

		struct sockaddr_in client_in;
		socklen_t client_len;
		char host_ip[NI_MAXHOST];
		socklen_t host_len;

		const int buffer = 1024;
		char recieve[buffer];
		char reply[buffer];
		ssize_t bytes;
		const char *message = "Hello From Pi!\nWelcome To 192.168.0.241\n";

		/* Sets up what type of socket
           TCP
		   IPv4 */
		memset(&hints,0,sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = 0;

		/* Grabs command line arguments and places 
		   it into the following variables */
		strcpy(ip_addr,argv[1]);
		strcpy(port,argv[2]);

		if ( getaddrinfo(ip_addr,port,&hints,&server ) < 0 ) {
			fprintf(stderr,"Could not get address information\n");
			exit(1);
		}

		sock_fd = socket(server->ai_family,server->ai_socktype,server->ai_protocol);
		if ( sock_fd < 0 ) {
			fprintf(stderr,"Could not create socket\n");
			freeaddrinfo(server);
			exit(1);
		}

		if ( bind(sock_fd,server->ai_addr,server->ai_addrlen) < 0 ) {
			fprintf(stderr,"Could not bind socket to port and address\n");
			close(sock_fd);
			freeaddrinfo(server);
			exit(1);
		}

		freeaddrinfo(server);
		
		printf("Listening for connections...\n");
		if ( listen(sock_fd,10) < 0 ) {
			fprintf(stderr,"Could not listen to any connections\n");
			close(sock_fd);
			exit(1);
		}

		/* Prints out proces ID */
		printf("Process ID: %d\n",getpid());

		client_len = sizeof(client_in);
		client_fd = accept(sock_fd,(struct sockaddr*)&client_in,&client_len);
		if ( client_fd < 0 ) {
			fprintf(stderr,"Could not bind socket to port and address\n");
			close(sock_fd);
			exit(1);
		}

		close(sock_fd);

		host_len = sizeof(host_ip);
		getnameinfo((struct sockaddr*)&client_in,client_len,host_ip,host_len,NULL,0,NI_NUMERICHOST);
		printf("Host connect %s\n",host_ip);

		bytes = send(client_fd,message,strlen(message),0);
		if ( bytes < 0 ) {
			fprintf(stderr,"Could not send reply\n");
			close(client_fd);
			exit(1);
		}

		while(1) {

			memset(recieve,0,buffer);
			memset(reply,0,buffer);

			bytes = recv(client_fd,recieve,buffer,0);
			if ( bytes < 0 ) {
				fprintf(stderr,"Could not recieve response\n");
				close(client_fd);
				exit(1);
			}

			printf("%s: %s",host_ip,recieve);
			if ( strcmp(recieve,"quit\n") == 0 ) {
				printf("Client has left the chat...\n");
				break;
			}

			printf("Server: ");
			fgets(reply,buffer,stdin);
			bytes = send(client_fd,reply,strnlen(reply,buffer),0);
			if ( bytes < 0 ) {
				fprintf(stderr,"Could not send reply\n");
				close(client_fd);
				exit(1);
			}
			
		}
		close(client_fd);
		return 0;
	}
	else {
		fprintf(stderr,"%s takes two args: IPv4, Port!\n",argv[0]);
		usage(argv);
		
		return -1;
	}
}

void usage(char *input[]) {
	format(42,'-');
	printf("Usage  : %s [IPv4] [Port]\n",input[0]);
	printf("Example: %s 192.168.0.xxx 8080\n",input[0]);
}

void format(int count, const char character) {
	for (int i=0; i<count; i++) {
		putchar(character);
	}
	putchar('\n');
}
