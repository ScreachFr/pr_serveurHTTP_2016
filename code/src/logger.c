#include "../include/logger.h"

int appendLog(char* fname, char* content) {
	pthread_mutex_lock(&logLock);
	int fd;
	
	fd = open(fname, O_WRONLY|O_CREAT|O_APPEND, 0600);	
	
	if (fd < 0) {
		pthread_mutex_unlock(&logLock);
		perror("Error while opening file ");
		return -1;
	}
	
	
	if (write(fd, content, strlen(content)) < 0) {
		pthread_mutex_unlock(&logLock);
		perror("Error while writing in the file ");
		return -1;
	}
	
	pthread_mutex_unlock(&logLock);
	return 0;
	
}

int writeRequestLog(char* clientIP,char* date, char* serverPID,
		char* threadID, char* request, char* returnCode, char* resultSize) {
	char* toWrite;
	int size = (strlen(clientIP) + strlen(date) + strlen(serverPID)
		+ strlen(threadID) + strlen(request) + strlen(returnCode) 
		+ strlen(resultSize) + 8) * sizeof(char);
	int i;
	toWrite = malloc(size);
	clearArray(toWrite, size);
	
	
	
	strcat(toWrite, clientIP);
	strcat(toWrite, " ");
	strcat(toWrite, date);
	strcat(toWrite, " ");
	strcat(toWrite, serverPID);
	strcat(toWrite, " ");
	strcat(toWrite, threadID);
	strcat(toWrite, " ");
	strcat(toWrite, request);
	strcat(toWrite, " ");
	strcat(toWrite, returnCode);
	strcat(toWrite, " ");
	strcat(toWrite, resultSize);

	for ( i = 0; i < strlen(toWrite); i++) {
		if (toWrite[i] == '\n')
			toWrite[i] = ' ';
	}

	strcat(toWrite, "\n");
	
	return appendLog(DEFAULT_LOG_PATH, toWrite);
}


int initLog() {
	if (pthread_mutex_init(&logLock, NULL) != 0) {
        printf("mutex init failed\n");
        return 1;
    }
    
	return 0;
}











