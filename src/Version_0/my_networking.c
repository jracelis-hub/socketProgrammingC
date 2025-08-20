#include "my_networking.h"

void format(int count,const char character) {

	for (int i=0 ; i<count ; i++) {
		putchar(character);
	}
	putchar('\n');
}

void usage(char *argv[]) {
	format(40,'-');
	printf("  Usage: %s [IPv4] [Port].\n",argv[0]);
	printf("Example: %s 192.168.0.xxx 8000.\n",argv[0]);
	printf("   Note: Port has to be > 1023. See /etc/protocols.\n");

	format(40,'-');
}

void error_msg(const char *message) {
	fprintf(stderr,"%s\n",message);
}

void pass_msg(void) {
	int count = 35;	
	for (int i=0; i < count; i++) {
		putchar('-');
		if ( i == count-1 ) {
			putchar('>');
			puts(" Done");
		}
	} 
}


void start_server(char *argv[]) {
	
	/* Socket File Descriptors
	   Error Validation     */
	int sock_fd, status, client_fd;

	/* Command line arguments from user
	   IPv4 max character = 15 + '\0' = 16
	   Port max character = 5 + '\0' = 6  */
	char ip[IP_MAX_LEN];
	char port[PORT_MAX_LEN];

	/* Structure to hold client information 
	   ONLY HOLDS IPv4!                   */
	struct sockaddr_in client_in;
	socklen_t client_len;
	char host_ip[NI_MAXHOST];
	socklen_t host_ip_len;
	char host_port[NI_MAXSERV];
	socklen_t host_port_len;

	/* Structure to hold information for the server */
	struct addrinfo hints, *server;
	
	/* Important to zero out any information in the hints structure             
	   Server Type:
	   IPv4
	   TCP                               
	   Using user input for IPv4 Server addr                     */
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = 0;

	/* Copying command line arguments into: 
	   IPv4: 192.168.0.241 or
	   Local: 127.0.0.1
	   Port: 8000                        */
	if ( strlen(argv[1]) < IP_MAX_LEN && 
	     strlen(argv[2]) < PORT_MAX_LEN &&
		 strlen(argv[1]) > IP_MIN_LEN) 
	{
		strcpy(ip,argv[1]);	
		strcpy(port,argv[2]);	
	}
	else {
		error_msg("The given arguments are out of max character range");
		usage(argv);
		exit(1);
	}

	/* Resolve server information */
	printf("Resolving server information...\n");
	status = getaddrinfo(ip,port,&hints,&server);
	if ( status < 0 ) {
		error_msg("Could not resolve server information...");
		exit(1);
	}

	pass_msg();
	
	/* Creates a socket descriptor to listen on */
	printf("Creating socket...\n");
	sock_fd = socket(server->ai_family,server->ai_socktype,server->ai_protocol);
	if ( sock_fd < 0 ) {
		error_msg("Could not create a socket");
		freeaddrinfo(server);
		exit(1);
	}

	pass_msg();

	/* Binds the socket with port and address */
	printf("Binding socket to port and address...\n");
	status = bind(sock_fd,server->ai_addr,server->ai_addrlen);
	if ( status < 0 ) {
		error_msg("Could not bind socket to address and port");
		close(sock_fd);
		freeaddrinfo(server);
		exit(1);
	}

	pass_msg();

	/* Socket so now ready to listen to any incoming requests 
	   Max backlog connections is 10                       */
	printf("Setting up socket for listening...\n");
	status = listen(sock_fd,BACKLOG);
	if ( status < 0 ) {
		error_msg("Could not set up socket for listening.");
		close(sock_fd);
		freeaddrinfo(server);
		exit(1);
	}

	/* This structure is no longer needed */
	freeaddrinfo(server);
	pass_msg();

	/* The socket is now waiting for any incoming connections
	   if accepted a new socket descriptor is created      */
	printf("Waiting to accept connections...\n");
	client_len = sizeof(client_in);
	client_fd = accept(sock_fd,(struct sockaddr*)&client_in,&client_len);
	if ( client_fd < 0 ) {
		error_msg("Could not accept the connection.");
		close(sock_fd);
		exit(1);
	}

	/* Getting clients information */
	host_ip_len = sizeof(host_ip);
	host_port_len = sizeof(host_port);
	status = getnameinfo((struct sockaddr*)&client_in,client_len,
	                     host_ip, host_ip_len,
						 host_port, host_port_len,
						 NI_NUMERICHOST | NI_NUMERICSERV);
	if ( status < 0 ) {
		error_msg("Could not get host IPv4 address or Port");
		close(client_fd);
		close(sock_fd);
		exit(1);
	}

	/* Once accepted print out IPv4:Port that was accepted on server side */
	printf("Accepted connection from %s:%s\n",host_ip,host_port);

	pass_msg();
	close(client_fd);
	close(sock_fd);
}


