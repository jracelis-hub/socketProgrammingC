#include "my_net_utils.h"

int main(int argc, char **argv) {

	int port = atoi(argv[2]);
	if ( argc != 3 ) {
		usage(argv);
		return -1;
	}

	if ( port < 1024 ) {
		fprintf(stderr,"Error: Given Port %s has to be above 1023\n",argv[2]);
		usage(argv);
		return -1;
	}
	else if ( port > 65535 ) {
		fprintf(stderr,"Error: Given Port %s has to be below 65536\n",argv[2]);
		usage(argv);
		return -1;
	}

	#ifdef TEST_SERVER
		start_server(argv);
	#elif TEST_CLIENT
		start_client(argv);
	#else
		printf("When compling include -DTEST_SERVER to test server\n");
		printf("When compling include -DTEST_CLIENT to test client\n");
		return -1;
	#endif

	return 0;
}
