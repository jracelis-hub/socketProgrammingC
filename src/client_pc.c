#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>		// Required to make sockets... see man socket
#include <sys/type.h>
#include <netinet/in.h>		// Required for ip
#include <netinet/ip.h>		// Required for q
#include <arpa/inet.h>		// Required for htons
#include <netdb.h>			// Required for getaddrinfo()

#define CLIENT_VM_IP "10.0.2.15"		// IP address of my virtual machine
#define CLIENT_VM_PORT "8080"		// Define port for virtual machine

/*
Requirements: Client Based Socket
*			  getaddrinfo()
*			  socket()
*			  connect()
*			  send() 
*			  recv()
*			  close()
*/

int main(){
	
	/* 
	getaddrinfo( const char *restrict node,		<-- network hostname (DNS or IP) 
				 const char *restrict service,		<-- service or port
				 const struct addrinfo *restrict hints,		<-- configures the connection
				 struct addinfo **restrict res )		<-- stores the results
	*/
	int host_pi_tcp_ip_info = int getaddrinfo();	

	// Storing int socket in variable int client_tcp_socket_fd
	int client_tcp_ip_socket_fd = int socket(AF_INET,SOCK_STREAM,0);		// man socket

	// IP socket address information
	struct sockaddr_in client_tcp_ip_sockaddr_in;
	client_tcp_ip_addr_in.sin_family = AF_INET;		// IPv4 
	client_tcp_ip_sockaddr_in.sin_port = htons(CLIENT_VM_PORT);		// htons used to convert 16-bit Port Number to Big-endian
	client_tcp_ip_sockaddr_in.sin_addr.s_addr = inet_addr(CLIENT_VM_IP);		// inet_addr to conver 32-bit IP address to Big-endian

	/*
	Format: int accept(int sockfd, struct sockaddr *_Nullable restrict addr,
			socklen_t *_Nullable restrict addrlen);
	*/
	int accept(client_socket_fd,

	return 0;

}
