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
	/*
	char testString[] = "GET / HTTP/1.1\nHost: 127.0.0.1\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\n\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0";
	
	char* path;
	
	path = parseQuery(testString);	
	
	printf("Parsed path : %s\n", path);
	*/
	return EXIT_SUCCESS;
}


