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
		
		handleClient(clientSocket);	
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
	char* getString;
	char* parsedPath;
	int n;
	char* answer;
	char test[] = "ok";
	
	

	if ((n = read(clientSocket, buffer, sizeof(buffer) - 1)) < 0) {
		perror("read()");
		exit(errno);
	}

	buffer[n] = '\0';
	getString = buffer;
	
	printf("received :\n%s\n", getString);
	
	parsedPath = parseQuery(getString);
	printf("parsedPath : %s\n", parsedPath);
	
	if (parsedPath == NULL) {
		printf("Error while parsing query %s\n", getString);
		answer = getHTTP_BAD_REQUEST();
		
		if (sendString(clientSocket, answer) < 0)
			printf("Error while sending answer to client");
		
		return;
	}
	
	answer = getHTTP_OK();
	if (sendString(clientSocket, answer) < 0)
		printf("Error while sending answer to client");	
	
	shutdown(clientSocket, 2);
	close(clientSocket);
}

int sendString(Socket s, const char* toWrite) {
	/*
	if (write(socket, toWrite, sizeof(toWrite)) == -1) {
		perror("write"); 
		return -1;
	}
	*/	
	return 0;
}

char* parseQuery(char* query) {
	char* path;
	char** lines;
	char** GETLine;
	int lineSize, getLineSize;
	
	lines = tokenize(query, HTTP_HEADER_LINE_DELIM, &lineSize);
	
	if (lineSize <= 0)
		return NULL;
	printf("line size ok\n");
	GETLine = tokenize(lines[0], HTTP_ARGS_DELIM, &getLineSize);
	
	if (getLineSize != 3)
		return NULL;	
	printf("get size ok\n");
	
	if (strlen(GETLine[0]) != HTTP_GET_SIZE && strncmp(GETLine[0], HTTP_GET, HTTP_GET_SIZE) != 0)
		return NULL;
	printf("GET ok\n");
	if (strlen(GETLine[2]) != HTTP_VERSION_SIZE && strncmp(GETLine[2], HTTP_VERSION, HTTP_VERSION_SIZE) != 0)
		return NULL;
	printf("VERSION ok\n");
	
	path = malloc(sizeof(GETLine[1]));
	
	copyString(GETLine[1], path);
	
	free(lines);
	free(GETLine);
	
	printf("path : %s\n", path);
	return path; 
}

char* createAnswer(char* code, char* message) {
	char* result;
	//+2 for spaces	
	result = malloc(sizeof(HTTP_VERSION) + sizeof(code) + sizeof(message) + (2 * sizeof(char)));
	
	concatString(result, HTTP_VERSION);
	concatString(result, " ");
	concatString(result, code);
	concatString(result, " ");
	concatString(result, message);
	
	
	
	return result;
}



















