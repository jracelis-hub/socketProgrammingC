#include "my_networking.h"

int main(int argc, char **argv) {

	if ( argc == 3 && atoi(argv[2]) > 1023 && atoi(argv[2]) < 65536 ) {
		start_server(argv);
	}
	else if ( atoi(argv[2]) < 1024 ) {
		fprintf(stderr,"Error: Given Port %s has to be above 1023\n",argv[2]);
		usage(argv);
		exit(1);
	}
	else if ( atoi(argv[2]) > 65535 ) {
		fprintf(stderr,"Error: Given Port %s has to be below 65536\n",argv[2]);
		usage(argv);
		exit(1);
	}
	else {
		usage(argv);
		exit(1);
	}
	return 0;
}
