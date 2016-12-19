#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> /* gethostbyname */
#include <pthread.h> 
#include <string.h>

#include "logger.h"
#include "common.h"

#define BUFFER_SIZE 1024

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define SIMU_CONNECTIONS 5

typedef int Socket;
typedef struct sockaddr Sockaddr;
typedef struct sockaddr_in Sockaddr_in;

struct HandleClientArgs {
	Socket clientSocket;
};

Socket initServerSocket(int port);
int connectionHandler(Socket socket);
void handleClient(Socket clientSocket);
void * thread_handleClient(void * args);


