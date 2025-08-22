#ifndef NET_UTILITY_H  
#define NET_UTILITY_H  

/* Standard C libraries for 
   I/O 
   Strings
   Memory Allocation
   Closing File Descriptors        
   Threads                    */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* Network API libraries */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

/* Macros */
#define BUFFER         1024
#define IP_MAX_LEN       16	      /* xxx.xxx.xxx.xxx + \0 */
#define PORT_MAX_LEN      6	      /* xxxxx + \0 */
#define IP_MIN_LEN        8       /* 10.0.0.0 */

/* Functions Prototypes */
void format(int count,const char character);
void usage(char **argv);
void error_msg(const char *message);
void pass_msg(void);

/* Server Methods */
typedef struct {
	const char *get;	/* To download file from server to local */
	const char *put;	/* To upload file from local to server */
	const char *list;	/* To list files on the server directory */
	const char *see;	/* To see a file from server into local stdout */
	const char *help;	/* To get help of how to use API */
} Methods_t;

#if defined(SERVER)
	void start_server(char **argv);
#endif

#if defined(CLIENT)
	void start_client(char **argv);
#endif

#endif /* NET_UTILITY_H */
