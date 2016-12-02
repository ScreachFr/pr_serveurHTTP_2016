#include "../include/server.h"


Socket initServerSocket(int port) {
	Sockaddr_in sin = {0};
	Socket s;
	
	
	s = socket(AF_INET, SOCK_STREAM, 0);
	
	if(s == INVALID_SOCKET) {
		perror("socket()");
		exit(errno);
	}
	
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	
	if(bind (s, (Sockaddr *) &sin, sizeof sin) == SOCKET_ERROR) {
		perror("bind()");
		exit(errno);
	}

	
	return s;
}

/*
void connectionHandler(Socket socket) {
	char buffer[BUFFER_SIZE];
	
	
	
}
*/
