#include "../include/server.h"

/**
 * Initialize a server socket.
 */
Socket initServerSocket(int port) {
	Sockaddr_in sin;
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
 * Dispatch connexion request to thread.
 * socket is the will be the listening socket.
 */
int connectionHandler(Socket socket) {
	Sockaddr_in csin;
	Socket clientSocket;
	int sinSize = sizeof(csin);
	int threadID = 0;
	struct HandleClientArgs * args;
	pthread_t * thread;

	//Initialize logs.
	if (initLog() < 0){
		printf("Log initialization failed\n");
		return EXIT_FAILURE;
	} else {
		printf("Log initialization successful\n");
	}

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

		//Launching new thread.
		if (pthread_create(thread, NULL, thread_handleClient, args)) {
			perror("Error while creating thread");
			return EXIT_FAILURE;
		}

		//handleClient(clientSocket);
	}

	return EXIT_SUCCESS;
}

/*
 * This function simply converts args and launchs the client handling.
 */
void * thread_handleClient(void * args) {
	struct HandleClientArgs * arguments = (struct HandleClientArgs *) args;
	Socket clientSocket = arguments->clientSocket;
	Sockaddr_in clientInfo = arguments->clientInfo;
	int id = arguments->id;
	handleClient(clientSocket, clientInfo, id);

	pthread_exit(NULL);
	return NULL;
}

/*
 * Read the input stream, process the incoming request and send an answer.
 */
void handleClient(Socket clientSocket, Sockaddr_in clientInfo, int threadID) {
	char buffer[BUFFER_SIZE];
	char* getString;
	char* parsedPath;
	char* finalPath;
	int n;
	char* answer;
	char* requestedFile;
	int errcode = 0;
    char request[REQUEST_BUFFER_SIZE];
    struct in_addr clientAddress = clientInfo.sin_addr;
    LogInfo * lInfo;
    time_t now = time(NULL);
    int fileType;
	//Initialize log information
    lInfo = newLogInfo(asctime(gmtime(&now)), NULL, threadID, NULL, getpid(),
    			clientAddress, 0);

	printf("Incoming connection from %s\n", inet_ntoa(lInfo->clientAddress));

	//Read in stream
	if ((n = read(clientSocket, buffer, sizeof(buffer) - 1)) < 0) {
		perror("read()");
		exit(errno);
	}

	buffer[n] = '\0';
	getString = (char*)malloc(strlen(buffer) * sizeof(char));
	strcpy(getString, &buffer);

	//Parse request
	parsedPath = parseQuery(getString, &request);

	if (request != NULL)
		lInfo->request = strdup(request);

	//Wrong path
	if (parsedPath == NULL) {
		answer = getHTTP_BAD_REQUEST();
		answerError(clientSocket, answer, HTTP_BAD_REQUEST, lInfo);
		shutdown(clientSocket, 2);
		close(clientSocket);

		return;
	}

	//Parse the requested path
	finalPath = getPath(parsedPath, &errcode);

	//Wrong path
	if (finalPath == NULL) {
		lInfo->resultSize = 0;

		if (errcode == 2) {
			answer = getHTTP_NOT_FOUND();
			answerError(clientSocket, answer, HTTP_NOT_FOUND, lInfo);
		} else {
			answer = getHTTP_BAD_REQUEST();
			answerError(clientSocket, answer, HTTP_BAD_REQUEST, lInfo);
		}

		shutdown(clientSocket, 2);
		close(clientSocket);
		printf("done\n");

		return;
	}

	//What kink of file is the user requesting ?
	fileType = getFileType(finalPath);

	switch (fileType) {
		case TYPE_HTML :
			answerHTML(clientSocket, finalPath, lInfo);
			break;
		case TYPE_RUNNABLE :
			answerRunnable(clientSocket, finalPath, lInfo);
		break;
		case TYPE_UNKOWN :
			answer = getHTTP_BAD_REQUEST();
			answerError(clientSocket, answer, HTTP_BAD_REQUEST, lInfo);
		break;
	}

}

/*
 * Write a string on a socket. In case of success, this function will
 * log the message.
 * This function must only be used to send a final message. Indeed this will
 * send "\n\n" to end the current request answer.
 */
int sendString(Socket s, char* toWrite, LogInfo * info) {
	if (send(s, toWrite, strlen(toWrite) * sizeof(char), 0) < 0) {
		perror("send");
		return -1;
	}
	if (send(s, "\n\n", sizeof(char) * 2, 0) < 0) {
		perror("send");
		return -1;
	}

	writeRequestLog(info);

	return 0;
}

/*
 * This function checks queries validity.
 * It will returns the requested path and the request in 'char* request' in case
 * of success. Otherwise this will return NULL.
 */
char* parseQuery(char* query, char* request) {
	char* path;
	char** lines;
	char** GETLine;
	int lineSize = 0;
	int getLineSize = 0;

	//Tokenize query with "\n".
	lines = tokenize(query, HTTP_HEADER_LINE_DELIM, &lineSize);
	if (lineSize <= 0)
		return NULL;

	//Tokenize first line with " ".
	GETLine = tokenize(lines[0], HTTP_ARGS_DELIM, &getLineSize);
	if (getLineSize != 3)
		return NULL;
	//Check if the query is formed like the following : "GET <PATH> HTTP/1.1".
	if (strlen(GETLine[0]) != HTTP_GET_SIZE && strncmp(GETLine[0], HTTP_GET, HTTP_GET_SIZE) != 0)
		return NULL;
	if (strlen(GETLine[2]) != HTTP_VERSION_SIZE && strncmp(GETLine[2], HTTP_VERSION, HTTP_VERSION_SIZE) != 0)
		return NULL;

	path = malloc(strlen(GETLine[1]) * sizeof(char));

	strcpy(path, GETLine[1]);
	strcpy(request, lines[0]);

	free(lines);
	free(GETLine);

	return path;
}

char* createAnswer(char* code, char* message) {
	char* result;
	int size = (strlen(HTTP_VERSION) + strlen(code) + strlen(message) + 3) * sizeof(char);
	result = (char *)malloc(size);

	strcat(result, HTTP_VERSION);
	strcat(result, " ");
	strcat(result, code);
	strcat(result, " ");
	strcat(result, message);

	return result;
}

char* addArgToAnswer(char* answer, char* argName, char* argValue) {
	answer = realloc(answer, (strlen(answer) + strlen(argName) + strlen(argValue) + 4) * sizeof(char));

	strcat(answer, "\n");
	strcat(answer, argName);
	strcat(answer, ": ");
	strcat(answer, argValue);

	return answer;
}

char* addFileToAnswer(char* answer, char* fileContent) {
	answer = realloc(answer, (strlen(answer) + 3 + strlen(fileContent)) * sizeof(char));
	strcat(answer, "\n\n");
	strcat(answer, fileContent);

	return answer;
}





char* getFile (char* path , int* errcode){

    char* result;
    int fd;
    char buffer[SIZE_BUFFER];
  
    size_t size;
  
    
    //file exists ?
    if (access(path, F_OK) < 0) {
        printf("Requested file does not exist. 1 \n");
        *errcode = 2;
        return NULL;
    }
    

    if ( (fd=open(path, O_RDONLY, S_IRUSR))< 0) {
        perror("Error while opening file ");
        *errcode = 1;
        return NULL;
    }
    
    while(size==read(fd, buffer, SIZE_BUFFER)>=0);
    
    result=(char*) malloc(sizeof(char)*strlen(buffer));
    strcpy(result, buffer);
    
    return  result ;
}




/*
char* getFile(char* path, int* errcode) {
	char* result;
	int fd;
	struct stat statbuf;
	int size;
	int n;

	//file exists ?
	if (access(path, F_OK) < 0) {
		printf("Requested file does not exist. 1 \n");
		*errcode = 2;
		return NULL;
	}

	fd = open(path, O_RDONLY, S_IRUSR);

	if (fd < 0) {
		perror("Error while opening file ");
		*errcode = 1;
		return NULL;
	}

	if(stat(path, &statbuf) == -1){
		perror("stat");
		*errcode = 1;
		return NULL;
	}

	size = statbuf.st_size;
	result = malloc(size + sizeof(char));

	if ((n = read(fd, result, size)) < 0) {
		perror("read()");
		*errcode = 1;
		return NULL;
	}
	//TODO check this.
	result[(size/sizeof(char)) + 1] = '\0';

	return result;
}
*/
/**
 * errcode :
 *		1 : 400
 *		2 : 404
 */
char* getPath(char* requestPath, int* errcode) {
	char finalPath[PATH_BUFFER_SIZE];
	struct stat statbuf;

	strcpy(finalPath, getenv("HOME"));
	strcat(finalPath, PUBLIC_HTML);
	strcat(finalPath, requestPath);

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

	return strdup(finalPath);
}


void answerError(Socket s, char* toWrite, char* errcode, LogInfo * info) {
	char* fileContent;
	char* path;
	int err = 0;
	info->returnCode = errcode;
	info->resultSize = 0;

	if (strcmp(errcode, HTTP_NOT_FOUND) == 0) {
		path = getPath(HTTP_NOT_FOUND_FILE, &err);
	} else if (strcmp(errcode, HTTP_BAD_REQUEST) == 0) {
		path = getPath(HTTP_BAD_REQUEST_FILE, &err);
	} else {
		path = getPath(HTTP_BAD_REQUEST_FILE, &err);
	}

	if (path != NULL) {
		fileContent = getFile(path, &err);
		toWrite = addArgToAnswer(toWrite, HTTP_ARG_CONTENT_TYPE, HTTP_ARG_TEXT_HTML);
		toWrite = addFileToAnswer(toWrite, fileContent);
	}
	if (sendString(s, toWrite, info) < 0)
			printf("Error while sending answer to client");
}

int answerHTML(Socket socket, char* path, LogInfo * info) {
	char* fileContent;
	char* answer;
	int errcode = 0;

	fileContent = getFile(path, &errcode);
	if (fileContent == NULL) {
		printf("Can't open requested file\n");
		if (errcode == 2) {
			answer = getHTTP_NOT_FOUND();
			answerError(socket, answer, HTTP_NOT_FOUND, info);
		} else {
			answer = getHTTP_BAD_REQUEST();
			answerError(socket, answer, HTTP_BAD_REQUEST, info);
		}

		shutdown(socket, 2);
		close(socket);
		printf("done\n");

		return -1;

	} else {
		answer = getHTTP_OK();
		answer = addArgToAnswer(answer, HTTP_ARG_CONTENT_TYPE, HTTP_ARG_TEXT_HTML);
		answer = addFileToAnswer(answer, fileContent);
		info->returnCode = HTTP_OK;
		info->resultSize = strlen(fileContent);


		if (sendString(socket, answer, info) < 0)
			printf("Error while sending answer to client");

		shutdown(socket, 2);
		close(socket);
	}

	return 0;
}



int answerRunnable(Socket socket, char* path, LogInfo * info) {
	pid_t forkResult;
	int returnValue;
	char* resultFileName;
	char* answer;


	sigset_t set;
	pid_t pid;

	printf("SIGCHLD is %i\n", SIGCHLD);

	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigprocmask(SIG_BLOCK, &set, NULL);

	if ((forkResult = fork()) < 0) {
		perror("fork");
		return -1;
	}

	if (forkResult != 0) { //Parent
		resultFileName = getRunnableResultFileName(forkResult);
		int retCode;

		sigset_t set;
        pid_t pid;

        printf("SIGCHLD is %i\n", SIGCHLD);

		sigset_t set2;
		siginfo_t siginfo;
		struct timespec timeout = {RUNNABLE_TIMEOUT, 0};
		int signal;

		sigemptyset(&set2);
		sigaddset(&set2, SIGCHLD);

		//TODO use something more reliable with a timeout. sigtimedwait ?
		if ((retCode = sigtimedwait(&set2, &siginfo, &timeout)) < 0) {
			//Timed out
			if (retCode == EAGAIN) {
				printf("Child timed out.\n");
				kill(pid, SIGKILL);
			} else if (retCode == SIGCHLD){
				printf("Runnable execution successful.\n");
			} else {
				perror("sigtimedwait");
				return -1;
			}
		}

		returnValue = readRunnableResult(resultFileName);
		printf("%d\n", returnValue);
		if (returnValue == 0) {
			answer = getHTTP_OK();
			info->returnCode = HTTP_OK;
			info->resultSize = 0;

			if (sendString(socket, answer, info) < 0)
				printf("Error while sending answer to client");

			shutdown(socket, 2);
			close(socket);
		} else {
			answer = getHTTP_SERVER_ERROR();
			answerError(socket, answer, HTTP_SERVER_ERROR, info);

			shutdown(socket, 2);
			close(socket);
		}


	} else { //Child
		char toWrite[6];
		resultFileName = getRunnableResultFileName(getpid());
		returnValue = execRunnable(path);
		sprintf(toWrite, "%d", returnValue);
		appendLog(resultFileName, &toWrite);
		printf("Child's ending.\n");
	}




	return 0;
}

int execRunnable(char* path) {
	int result;

	printf("Executing %s\n", path);

	if ((result = system(path)) < 0) {
		perror("system");
		return -1;
	}

	return result;
}

char* getRunnableResultFileName(pid_t pid) {
	char* result[23];

	sprintf(result, "%s%d", DEFAULT_RUNNABLE_RETURN_PATH, pid);

	return strdup(result);
}

int readRunnableResult(char* fname) {
	int returnCode = 0;
	char* fileContent;
	if ((fileContent = getFile(fname, &returnCode)) == NULL)
		return returnCode;

	//The tmp file is not usefull anymore.
	if (unlink(fname) < 0) {
		perror("unlink");
	}

	return atoi(fileContent);
}
