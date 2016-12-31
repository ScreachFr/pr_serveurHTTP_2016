#include <stdlib.h>
#include <stdio.h>
#include "../include/logger.h"
#include "../include/server.h"

int main(int argc, char** argv) {
	if (argc < 2){
		printf("too few args\n");
		return EXIT_FAILURE;
	}
    printf("Launching HTTP server...\n");

	int port = atoi(argv[1]);

    Socket s;


	s = initServerSocket(port);

	connectionHandler(s);


    return EXIT_SUCCESS;
}
