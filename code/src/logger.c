#include "../include/logger.h"

int appendLog(char* fname, char* content) {
	int fd;
	
	fd = open(fname, O_WRONLY|O_CREAT|O_APPEND, 0600);	
	
	if (fd < 0) {
		perror("Error while opening file ");
		return -1;
	}
	
	
	if (write(fd, content, strlen(content)) < 0) {
		perror("Error while writing in the file ");
		return -1;
	}
	
	return 0;
	
}
