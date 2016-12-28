#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "common.h"
#include "utils.h"

#define DEFAULT_LOG_PATH "/tmp/http3502305.log"

pthread_mutex_t logLock;

int appendLog(char* fname, char* content);
int writeRequestLog(char* clientIP,char* date, char* serverPID,
		char* threadID, char* request, char* returnCode, char* resultSize);

int initLog();
