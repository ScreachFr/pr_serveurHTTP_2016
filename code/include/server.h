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
#include "logger.h"


#define BUFFER_SIZE 1024

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1


typedef int Socket;
typedef struct sockaddr Sockaddr;
typedef struct sockaddr_in Sockaddr_in;

Socket initServerSocket(int port);
//void connectionHandler(Socket socket);
