#include "my_net_utils.h"

void format(int count,const char character) {

	for (int i=0 ; i<count ; i++) {
		putchar(character);
	}
	putchar('\n');
}

void usage(char **argv) {
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

#ifdef TEST_SERVER
#include <pthread.h>
#include <netinet/in.h>
#define BACKLOG          10       /* Listen connections */

void start_server(char **argv) {
	
	/* Socket File Descriptors
	   Error Validation     */
	int sock_fd, status, client_fd;

	/* Structure to hold information for the server */
	struct addrinfo hints, *server;

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

	/* Information to send and rec */
	Methods_t requests = { "Get Ok","Put Ok","List Ok","Read Ok"};

	/* Initial message to be sent to client after connection */
	const char *welcome_message = "Hello! Welcome to RPi FTP Server\n"
								  "Use the following methods:\n"
								  "Get file_name.txt (download file)\n"
								  "Put file_name.txt (upload file)\n"
								  "List files on mounted directory\n"
								  "Read file_name.txt (see content in file)\n";
	
	/* Variables need to recieve and response to the client(s) 
	   Buffer size for recieve and response is 1024 bytes */
	ssize_t bytes;
	char recieve[BUFFER];
	char response[BUFFER];

	/* Get process id of the server running */
	pid_t process_id = getpid();
	printf("Created new process: %d\n",process_id);

	/* Copying command line arguments into: 
	   IPv4: 192.168.0.241 or
	   Local: 127.0.0.1
	   Port: 8000                        */
	if ( inet_pton(AF_INET,argv[1],ip) == 1 )
	{
		strcpy(ip,argv[1]);	
		strcpy(port,argv[2]);	
	}
	else {
		error_msg("The IPv4 address argv[1] is not a proper IPv4 format");
		usage(argv);
		printf("Process killed: %d\n",process_id);
		exit(1);
	}

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


	/* Resolve server information */
	printf("Resolving server information...\n");
	status = getaddrinfo(ip,port,&hints,&server);
	if ( status < 0 ) {
		error_msg("Could not resolve server information...");
		printf("Process killed: %d\n",process_id);
		exit(1);
	}

	pass_msg();
	
	/* Creates a socket descriptor to listen on */
	printf("Creating socket...\n");
	sock_fd = socket(server->ai_family,server->ai_socktype,server->ai_protocol);
	if ( sock_fd < 0 ) {
		error_msg("Could not create a socket");
		printf("Process killed: %d\n",process_id);
		freeaddrinfo(server);
		exit(1);
	}

	printf("Created Socket FD: %d\n",sock_fd);
	pass_msg();

	/* Binds the socket with port and address */
	printf("Binding socket to port and address...\n");
	status = bind(sock_fd,server->ai_addr,server->ai_addrlen);
	if ( status < 0 ) {
		error_msg("Could not bind socket to address and port");
		printf("Process killed: %d\n",process_id);
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
		printf("Process killed: %d\n",process_id);
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
		printf("Process killed: %d\n",process_id);
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
		printf("Process killed: %d\n",process_id);
		close(client_fd);
		close(sock_fd);
		exit(1);
	}

	/* Once accepted print out IPv4:Port that was accepted on server side */
	printf("Created Client FD: %d\n",client_fd);
	printf("Accepted connection from %s:%s\n",host_ip,host_port);

	bytes = send(client_fd,welcome_message,strlen(welcome_message),0);
	if ( bytes == -1 ) {
		error_msg("Could not send bytes to client");	
		close(client_fd);
		close(sock_fd);
		exit(1);
	}

	while(1) {
		
		/* Ensure the message buffers are 0 out before (re)sending data */
		memset(recieve,0,sizeof(recieve));
		memset(response,0,sizeof(response));

		bytes = recv(client_fd,recieve,sizeof(recieve),0);
		if ( bytes == -1 ) {
			error_msg("Could not recieve bytes from client");
			close(client_fd);
			close(sock_fd);
			exit(1);
		}

		printf("Request: %s",recieve);

		if ( strncmp(recieve,"Get",3) == 0 ) {
			strncpy(response,requests.get,sizeof(response)-1);

			bytes = send(client_fd,response,strnlen(response,BUFFER),0);
			if ( bytes == -1 ) {
				error_msg("Could not send get response to client");
				close(client_fd);
				close(sock_fd);
				pass_msg();
				exit(1);
			}
			printf("Server response complete...\n");
			printf("Disconnecting from client...\n");
			close(client_fd);
			break;
		}
		else {
			error_msg("Invalid input disconnecting from client...\n");
			close(client_fd);
			close(sock_fd);
			exit(1);
		}
	}

	printf("Process killed: %d\n",process_id);
	close(sock_fd);
}
#endif /* End TEST_SERVER */

#ifdef TEST_CLIENT
void start_client(char **argv) {
	
	/* Socket File Descriptors
	   Error Validation     */
	int sock_fd, status;

	/* Structure to resolving the server information */
	struct addrinfo hints, *server;

	/* Command line arguments from user
	   IPv4 max character = 15 + '\0' = 16
	   Port max character = 5 + '\0' = 6  */
	char ip[IP_MAX_LEN];
	char port[PORT_MAX_LEN];

	/* Bytes to recieve or send of a response 
	   Buffer size of 1024 characters */
	ssize_t bytes;
	char response[BUFFER];
	char recieve[BUFFER];

	/* Gives the process id of the FTP server */
	pid_t process_id = getpid();
	printf("Create new process: %d\n",process_id);

	if ( inet_pton(AF_INET,argv[1],ip) == 1 )
	{
		strcpy(ip,argv[1]);	
		strcpy(port,argv[2]);	
	}
	else {
		error_msg("The given arguments are out of max character range");
		printf("Process killed: %d\n",process_id);
		usage(argv);
		exit(1);
	}

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

	/* Resolve server information */
	printf("Resolving server information...\n");
	status = getaddrinfo(ip,port,&hints,&server);
	if ( status < 0 ) {
		error_msg("Could not resolve server information...");
		printf("Process killed: %d\n",process_id);
		exit(1);
	}

	pass_msg();
	
	/* Creates a socket descriptor to listen on */
	printf("Creating socket...\n");
	sock_fd = socket(server->ai_family,server->ai_socktype,server->ai_protocol);
	if ( sock_fd < 0 ) {
		error_msg("Could not create a socket");
		printf("Process killed: %d\n",process_id);
		freeaddrinfo(server);
		exit(1);
	}

	printf("Created Socket FD: %d\n",sock_fd);
	pass_msg();
	
	printf("Attempting to connect to server...\n");
	status = connect(sock_fd,server->ai_addr,server->ai_addrlen);
	if ( status < 0 ) {
		error_msg("Failed to connect server.");
		printf("Process killed: %d\n",process_id);
		close(sock_fd);
		freeaddrinfo(server);
		exit(1);
	}

	freeaddrinfo(server);
	printf("Connected to %s:%s\n",argv[1],argv[2]);

	/* Begin Data transfering */
	while(1) {

		/* Zero out the buffers to avoid recieve old data */
		memset(recieve,0,sizeof(recieve));
		memset(response,0,sizeof(response));

		bytes = recv(sock_fd,recieve,sizeof(recieve),0);
		if ( bytes == -1 ) {
			error_msg("Could not recieve bytes from server");
			close(sock_fd);
			exit(1);
		}
		printf("%s",recieve);

		if (strncmp(recieve,"Get",3) == 0) {
			printf("Transfer complete\n");
			printf("Closing connection...\n");
			break;
		}

		printf("$ ");
		fgets(response,BUFFER-1,stdin);
		bytes = send(sock_fd,response,strnlen(response,BUFFER),0);
		if ( bytes == -1 ) {
			error_msg("Could not recieve bytes from server");
			close(sock_fd);
			exit(1);
		}
	}

	printf("Process killed: %d\n",process_id);
	close(sock_fd);
}
#endif /* End TEST_CLIENT */
