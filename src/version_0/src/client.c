#if defined(CLIENT)
#include "net_utility.h"
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
		else if (strncmp(recieve,"Invalid",7) == 0) {
			error_msg("Invalid request from server... Closing connection");
			close(sock_fd);
			exit(1);
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
