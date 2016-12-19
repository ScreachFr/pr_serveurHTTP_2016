#include <stdlib.h>
#include <stdio.h>
#include "../include/logger.h"
#include "../include/server.h"

int main(int argc, char** argv) {
	if (argc < 2){
		printf("too few args\n");
		return EXIT_FAILURE;
	}
	
	int port = atoi(argv[1]);
	Socket s;
	
	
	s = initServerSocket(port);
	
	printf("s : %d\n", s);
	
	connectionHandler(s);
	
	return EXIT_SUCCESS;
}


