#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"
#include "utils.h"


#ifndef _LOGGER_
#define _LOGGER_

#define DEFAULT_LOG_PATH "/tmp/http3502305.log"

pthread_mutex_t logLock;

typedef struct LogsInfo {
	char* date;
	char* returnCode;
	int threadID;
	char* request;
	pid_t pid;
	struct in_addr clientAddress;
	int resultSize;
} LogInfo;

LogInfo * newLogInfo(
	char* date,
	char* returnCode,
	int threadID,
	char* request,
	pid_t pid,
	struct in_addr clientAddress,
	int resultSize);

int appendLog(char* fname, char* content);
int writeRequestLog(LogInfo * info);

int initLog();

#endif
