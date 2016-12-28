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
	Sockaddr_in csin;
	Socket clientSocket;
	int sinSize = sizeof(csin);
	int threadID = 0;
	struct HandleClientArgs * args;
	pthread_t * thread;
	
	//Simultaneous connection limit. 	
	if (listen(socket, SIMU_CONNECTIONS) == SOCKET_ERROR) {
		perror("listen()");
		exit(errno);
	}

	
	while (TRUE) {
		if ((clientSocket = accept(socket, (Sockaddr *)&csin, &sinSize)) < -1) {
			perror("accept()");
			exit(3);
		}
		printf("client connection\n");
		
		//New thread setup.
		args = malloc(sizeof(struct HandleClientArgs));
		thread = malloc(sizeof(pthread_t));
		args->clientSocket = clientSocket;
		args->clientInfo = csin;
		args->id = threadID++;
		
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
	Sockaddr_in clientInfo = arguments->clientInfo;
	int id = arguments->id;
	handleClient(clientSocket, clientInfo, id);
	
	pthread_exit(NULL);
	return NULL;
}

void handleClient(Socket clientSocket, Sockaddr_in clientInfo, int threadID) {
	char buffer[BUFFER_SIZE];
	char* getString;
	char* parsedPath;
	int n;
	char* answer = "";
	char* requestedFile;
	int errcode = 0;
    char request[REQUEST_BUFFER_SIZE];
    struct in_addr clientAddress = clientInfo.sin_addr;
    
	printf("Thread #%d launched.\n", threadID);

	if ((n = read(clientSocket, buffer, sizeof(buffer) - 1)) < 0) {
		perror("read()");
		exit(errno);
	}
	
	buffer[n] = '\0';
	getString = buffer;
	
	printf("received :\n%s\n", getString);
	
	parsedPath = parseQuery(getString, &request);
	printf("parsedPath : %s\n", parsedPath);
	
	if (parsedPath == NULL) {
		printf("Error while parsing query %s\n", getString);
		answer = getHTTP_BAD_REQUEST();
		
		if (sendString(clientSocket, answer, HTTP_BAD_REQUEST, threadID, 0, request, clientAddress) < 0)
			printf("Error while sending answer to client");
		
		
		shutdown(clientSocket, 2);
		close(clientSocket);
		printf("done\n");
		
		return;
	}
	
	requestedFile = getFile(parsedPath, &errcode);
	
	
	if (requestedFile == NULL) {
		printf("Can't open requested file\n");
		if (errcode == -1) {
			answer = getHTTP_BAD_REQUEST();
			if (sendString(clientSocket, answer, HTTP_BAD_REQUEST, threadID, 0, request, clientAddress) < 0)
				printf("Error while sending answer to client");	
		} else{
			answer = getHTTP_NOT_FOUND();
			if (sendString(clientSocket, answer, HTTP_NOT_FOUND, threadID, 0, request, clientAddress) < 0)
				printf("Error while sending answer to client");			
		}
	} else {
		answer = getHTTP_OK();
		answer = addArgToAnswer(answer, HTTP_ARG_CONTENT_TYPE, HTTP_ARG_TEXT_HTML);	
		answer = addFileToAnswer(answer, requestedFile);
		
		if (sendString(clientSocket, answer, HTTP_OK, threadID, strlen(requestedFile), request, clientAddress) < 0)
			printf("Error while sending answer to client");	
	}
	
	printf("generated answer : %s\n", answer);
	
	
	shutdown(clientSocket, 2);
	close(clientSocket);
	printf("done\n");
}

int sendString(Socket s, char* toWrite, char* returnCode, int threadID, 
		int resultSize, char* request, struct in_addr clientAddress) {

	time_t result = time(NULL);
    char* time = asctime(gmtime(&result));
	char pid[5];
    char threadID_str[15];	
	char resultSize_str[15];
	char* ip;
	
	sprintf(pid, "%d", getpid());
    sprintf(threadID_str, "%d", threadID);
    sprintf(resultSize_str, "%d", resultSize);
	ip = inet_ntoa(clientAddress);
	
	printf("answer : %s\n", toWrite);
	if (send(s, toWrite, strlen(toWrite) * sizeof(char), 0) < 0) {
		perror("send");
		return -1;
	}
	
	writeRequestLog(ip, time, &pid, &threadID_str, request, returnCode, &resultSize_str);
	
	
	return 0;
}

char* parseQuery(char* query, char* request) {
	char* path;
	char** lines;
	char** GETLine;
	int lineSize, getLineSize;
	
	lines = tokenize(query, HTTP_HEADER_LINE_DELIM, &lineSize);
	
	if (lineSize <= 0)
		return NULL;
	GETLine = tokenize(lines[0], HTTP_ARGS_DELIM, &getLineSize);
	
	if (getLineSize != 3)
		return NULL;	
	
	if (strlen(GETLine[0]) != HTTP_GET_SIZE && strncmp(GETLine[0], HTTP_GET, HTTP_GET_SIZE) != 0)
		return NULL;
	if (strlen(GETLine[2]) != HTTP_VERSION_SIZE && strncmp(GETLine[2], HTTP_VERSION, HTTP_VERSION_SIZE) != 0)
		return NULL;
	
	path = malloc(strlen(GETLine[1]) * sizeof(char));
	
	copyString(GETLine[1], path);
	copyString(lines[0], request);
	//free(lines);
	//free(GETLine);
	
	printf("path : %s\n", path);
	return path; 
}

char* createAnswer(char* code, char* message) {
	char* result;
	int size = (strlen(HTTP_VERSION) + strlen(code) + strlen(message) + 3) * sizeof(char);
	result = malloc(size);
	//clearArray(result, size);
	strcat(result, HTTP_VERSION);
	strcat(result, " ");
	strcat(result, code);
	strcat(result, " ");
	strcat(result, message);
	
	return result;
}

char* addArgToAnswer(char* answer, char* argName, char* argValue) {
	answer = realloc(answer, (strlen(answer) + strlen(argName) + strlen(argValue) + 3) * sizeof(char));
	strcat(answer, "\n");
	strcat(answer, argName);
	strcat(answer, ": ");
	strcat(answer, argValue);
	
	return answer;
}

char* addFileToAnswer(char* answer, char* fileContent) {
	answer = realloc(answer, (strlen(answer) + 2 + strlen(fileContent)) * sizeof(char));
	strcat(answer, "\n");
	strcat(answer, fileContent);

	return answer;
}

char* getFile(char* path, int* errcode) {
	char finalPath[PATH_BUFFER_SIZE];
	char* result;
	int fd;
	struct stat statbuf;
	int size;
	int n;
	
	
	//create public html path
	clearArray(finalPath, PATH_BUFFER_SIZE);
	strcat(finalPath, getenv("HOME"));
	strcat(finalPath, PUBLIC_HTML);
	strcat(finalPath, path);
	
	//file exists ?	
	if (access(finalPath, F_OK) < 0) {
		printf("Requested file does not exist. 1 \n");
		*errcode = 2;
		return NULL;
	}	
	
	if(stat(finalPath, &statbuf) == -1){
		perror("stat");
		*errcode = 1;
		return NULL;
	}
	
	//is finalPath a directory ?
	if (S_ISDIR(statbuf.st_mode)) { 
		strcat(finalPath, DEFAULT_FILE_NAME);
		
		if (access(finalPath, F_OK) < 0) {
			printf("Requested file does not exist. 1\n");
			*errcode = 2;
			return NULL;
		}	
	} 
	
	fd = open(finalPath, O_RDONLY, S_IRUSR);	

	if (fd < 0) {
		perror("Error while opening file ");
		*errcode = 1;
		return NULL;
	}
	
	if(stat(finalPath, &statbuf) == -1){
		perror("stat");
		*errcode = 1;
		return NULL;
	}
	size = statbuf.st_size + 1;
	result = malloc(size);
	
	if ((n = read(fd, result, size-1)) < 0) {
		perror("read()");
		*errcode = 1;
		return NULL;
	}
	
	result[size] = '\0';
	
	return result;
}















