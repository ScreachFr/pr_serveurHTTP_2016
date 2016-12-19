#include "../include/server.h"

/**
 * Initialize a server socket.
 */
Socket initServerSocket(int port) {
	Sockaddr_in sin = {0};
	Socket s;
	
	
	s = socket(AF_INET, SOCK_STREAM, 0);
	
	if (s == INVALID_SOCKET) {
		perror("socket()");
		exit(errno);
	}
	
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	
	if (bind(s, (Sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR) {
		perror("bind()");
		exit(errno);
	}

	
	return s;
}

/**
 * 
 */
int connectionHandler(Socket socket) {
	Sockaddr_in csin = { 0 };
	Socket clientSocket;
	int sinSize = sizeof(csin);
	
	struct HandleClientArgs * args;
	pthread_t * thread;
	
	//Simultaneous connection limit. 	
	if (listen(socket, SIMU_CONNECTIONS) == SOCKET_ERROR) {
		perror("listen()");
		exit(errno);
	}

	
	while (TRUE) {
		if ((clientSocket = accept(socket, (Sockaddr *)&csin, &sinSize))< -1) {
			perror("accept()");
			exit(3);
		}
		printf("client connection\n");
		
		//New thread setup.
		args = malloc(sizeof(struct HandleClientArgs));
		thread = malloc(sizeof(pthread_t));
		args->clientSocket = clientSocket;
		
		if (pthread_create(thread, NULL, thread_handleClient, args)) {
			perror("Error while creating thread");
			return EXIT_FAILURE;
		}
		
		//handleClient(clientSocket);	
	}
	
	return EXIT_SUCCESS;
}

void * thread_handleClient(void * args) {
	struct HandleClientArgs * arguments = (struct HandleClientArgs *) args;
	Socket clientSocket = arguments->clientSocket;
	
	handleClient(clientSocket);
	
	pthread_exit(NULL);
	return NULL;
}

void handleClient(Socket clientSocket) {
	char buffer[BUFFER_SIZE];
	char* getString, parsedPath;
	int n;
	
	if ((n = read(clientSocket, buffer, sizeof(buffer) - 1)) < 0) {
		perror("read()");
		exit(errno);
	}

	buffer[n] = '\0';
	getString = buffer;
	
	printf("received :\n%s\n", getString);
	
	parsedPath = "";
	
	if ((n = parseQuery(getString, parsedPath)) < 0) {
		printf("Error while parsing query (%d) \n", n);
		return;
	}
	
		
}

//TODO check if "HTTP/1.1" is in query
//TODO check if the path is empty
int parseQuery(char* query, char* path) {
	char* result;
	int i;
	
	if (strlen(query) < 4)
		return -1;
	
	if (strncmp(query, "GET ", 4) != 0)
		return -2;
	
	i = 4;
	result = malloc(sizeof(char));
	
	while (query[i] != ' ' && query[i] != '\n' && query[i] != '\0') {
		result = realloc(result, sizeof(result) + sizeof(char));
		result[i-4] = query[i];
		i++;
	}
	
	result[i-3] = '\0';
	
	printf("Parsed path : %s\n", result);
	
	path = result;
	
	return 0;
}

char* createAnswer(int code, char* message) {
	//TODO
	
	return NULL;
}





















