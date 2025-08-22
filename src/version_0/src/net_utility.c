#include "net_utility.h"

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
