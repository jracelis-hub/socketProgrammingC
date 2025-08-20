#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

void run_server_in(char *input[]);
void *handle_client(void *arg);
void error_msg(const char *message);
void usage(char *input[]);
void format(int count, const char character);

int main(int argc, char *argv[]) {
	
	/* This will take a command line argument:
       Localhost: 127.0.0.1
	   IPv4 Address: 192.168.0.x  */
	if ( argc == 3 ) {
		run_server_in(argv);
	}
	else {
		fprintf(stderr,"%s takes two args: IPv4, Port!\n",argv[0]);
		usage(argv);
		return -1;
	}
}

void run_server_in(char *input[]) {

	int status, sock_fd, client_fd;
	struct addrinfo hints, *server;
	const int backlog = 10;

	/* Can take only IPv4 address: 192.168.xxx.xxx + '\0'
	   Can take port size: xxxxx + '\0' */
	char ip_addr[16]; 
	char port[6];	

	/* Make sure that the hints struct is empty */
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;

	/* When creating a server don't use any reserved ports
       See /etc/protocols                               */
	if ( atoi(input[2]) < 1024  ){
		error_msg("Ports below 1024 are reserved. Please use port above 1024.");
		usage(input);
		exit(1);
	}

	strncpy(ip_addr,input[1],sizeof(ip_addr)-1);
	ip_addr[sizeof(ip_addr)-1] = '\0';

	strncpy(port,input[2],sizeof(port)-1);
	port[sizeof(port)-1] = '\0';

	status = getaddrinfo(ip_addr,port,&hints,&server);
	if ( status < 0 ) {
		error_msg("Could not resolve server name");
		exit(1);
	}

	sock_fd = socket(server->ai_family,server->ai_socktype,server->ai_protocol);
	if ( sock_fd < 0 ) {
		error_msg("Could not create socket descriptor");
		freeaddrinfo(server);
		exit(1);
	}
	
	status = bind(sock_fd,server->ai_addr,server->ai_addrlen);
	if ( status < 0 ) {
		error_msg("Could not bind socket to address and port");
		freeaddrinfo(server);
		close(sock_fd);
		exit(1);
	}

	status = listen(sock_fd, backlog);
	if ( status < 0 ) {
		error_msg("Could not listen to any connections");
		freeaddrinfo(server);
		close(sock_fd);
		exit(1);
	}

	/* This pointer struct is no longer needed */
	freeaddrinfo(server);

	printf("Listening for any incoming connections...\n");

	while(1) {
		
		/* Client fd to accept the connection and 
		   cthread to be used for the thread of the connection */
		int client_fd, *cthread_fd;
		struct sockaddr_in client_in;
		socklen_t client_len;

		/* Used to hold the clients IP address */
		char host_ip[NI_MAXHOST];
		socklen_t host_ip_len;
		
		client_len = sizeof(client_in);
		client_fd = accept(sock_fd,(struct sockaddr*)&client_in,&client_len);
		if ( client_fd < 0 ) {
			error_msg("Could not accept the connection");
		}

		/* To get connected host IPv4 address */
		host_ip_len = sizeof(host_ip);
		status = getnameinfo((struct sockaddr*)&client_in,client_len,
		                      host_ip,host_ip_len, 
							  NULL,0,                /* Use to get port but dont need */
							  NI_NUMERICHOST);
		if ( status < 0 ) {
			error_msg("Could not get host IPv4 address");
			continue;
		}

		printf("Connection to %s...\n",host_ip);
		
		cthread_fd = calloc(1,sizeof(int));

		*cthread_fd = client_fd;
		pthread_t client_thread;
		status = pthread_create(&client_thread,NULL,handle_client,cthread_fd);
		if ( status < 0 ) {
			error_msg("The server could not create a thread for the client");
			free(cthread_fd);
			continue;
		}
		
		status = pthread_detach(client_thread);
		if ( status < 0 ) {
			error_msg("Client thread failed to detach");
		}
	}

	close(sock_fd);
}

void *handle_client(void *arg) {

	int *pclient_fd = (int *)arg;

	if ( pclient_fd == NULL ) {

		return NULL;
	}

	ssize_t bytes;
	const int buffer = 1024;
	char recieve[buffer];
	char reply[buffer];

	/* Send initial message to client */
	const char *message = "Welcome To Pi Chat Box!\n";
	bytes = send(*pclient_fd,message,strlen(message),0);
	if ( bytes < 0 ) {
		error_msg("Unable to send starting message");
		exit(1);
	}

	while(1) {

		/* Ensure that the string buffers are zeroed out */
		memset(recieve,0,buffer);
		memset(reply,0,buffer);

		bytes = recv(*pclient_fd,recieve,buffer,0);
		if ( bytes < 0 ) {
			error_msg("Did not recieve any bytes from client");
			break;
		}

		if ( strcmp(recieve,"quit\n") == 0 ||
		     strcmp(recieve,"exit\n") == 0 ||
			 strcmp(recieve,"shutdown\n") == 0) 
		{
			printf("Client left the chat...\n");
			break;
		}
		recieve[bytes] = '\0';
		printf("Client: %s\n",recieve);
		
		printf("Me: ");
		fgets(reply,buffer,stdin);
		bytes = send(*pclient_fd,reply,strlen(reply),0);
		if ( bytes < 0 ) {
			error_msg("Could not send any bytes to client");
			break;
		}
	}

	if ( pclient_fd != NULL ) {
		free(pclient_fd);
	}
		
	return NULL;
}

void error_msg(const char *message) {
	fprintf(stderr,"%s\n",message);
}

void usage(char *input[]) {
	format(42,'-');
	printf("Usage  : %s [IPv4] [Port]\n",input[0]);
	printf("Example: %s 192.168.0.xxx 8080\n",input[0]);
	printf("Notes  : Make sure port is > 1023\n");
}

void format(int count, const char character) {
	for (int i=0; i<count; i++) {
		putchar(character);
	}
	putchar('\n');
}
