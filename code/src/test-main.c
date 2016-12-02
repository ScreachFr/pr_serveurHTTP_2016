#include <stdlib.h>
#include <stdio.h>
#include "../include/logger.h"
#include "../include/server.h"

int main() {
	Socket s;
	
	
	s = initServerSocket(90);
	
	printf("s : %d\n", s);
	
	return EXIT_SUCCESS;
}


