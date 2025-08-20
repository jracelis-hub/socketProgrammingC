#ifndef                 MY_NETWORKING_H
#define                 MY_NETWORKING_H
/*	   Standard C libraries for 
	   I/O 
	   Strings
	   Memory Allocation
	   Closing File Descriptors        
	   Threads                       */
#include                      <stdio.h>
#include                     <string.h>
#include                     <stdlib.h>
#include                     <unistd.h>
#include                    <pthread.h>

/*     Network API libraries         */
#include                  <sys/types.h>
#include                 <sys/socket.h>
#include                      <netdb.h>
#include                 <netinet/in.h>

/*     My Macros                     */
#define BUFFER                     1024
#define IP_MAX_LEN                   16
#define PORT_MAX_LEN                  6
#define IP_MIN_LEN                    8 /* 10.0.0.0 + '\0' */
#define BACKLOG                      10
/*     User Created Functions        */
void format(int count,const char character);
void usage(char *argv[]);
void start_server(char *argv[]);
void error_msg(const char *message);
void pass_msg(void);

/*     Server Methods                */
struct methods {
	const char *Get;	/* To get file from server to local */
	const char *Put;	/* To put file from local to server */
	const char *List;	/* To list current files on the server directory */
	const char *Read;	/* To read a file from server into local stdout */
};

#endif
