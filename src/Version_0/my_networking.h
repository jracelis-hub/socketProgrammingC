#ifndef MY_NETWORKING_H
#define MY_NETWORKING_H

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
#include <pthread.h>

/* Network API libraries */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

/* Macros */
#define BUFFER         1024
#define IP_MAX_LEN       16	      /* xxx.xxx.xxx.xxx + \0 */
#define PORT_MAX_LEN      6	      /* xxxxx + \0 */
#define IP_MIN_LEN        8       /* 10.0.0.0 */
#define BACKLOG          10       /* Listen connections */

/* Functions Prototypes */
void format(int count,const char character);
void usage(char **argv);
void start_server(char **argv);
void error_msg(const char *message);
void pass_msg(void);

/* Server Methods */
typedef struct {
	const char *get;	/* To download file from server to local */
	const char *put;	/* To upload file from local to server */
	const char *list;	/* To list files on the server directory */
	const char *read;	/* To read a file from server into local stdout */
} Methods_t;

#endif /* MY_NETWORKING_H */
