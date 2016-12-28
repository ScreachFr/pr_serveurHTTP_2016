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
#include "utils.h"

#define BUFFER_SIZE 1024
#define PATH_BUFFER_SIZE 256

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define SIMU_CONNECTIONS 5

#define HTTP_HEADER_LINE_DELIM "\n"
#define HTTP_ARGS_DELIM " "
#define HTTP_GET "GET"
#define HTTP_GET_SIZE 3
#define HTTP_VERSION "HTTP/1.1"
#define HTTP_VERSION_SIZE 8

#define HTTP_OK "200"
#define HTTP_OK_MSG "OK"

#define HTTP_BAD_REQUEST "400"
#define HTTP_BAD_REQUEST_MSG "Bad request"
#define HTTP_NOT_FOUND "404"
#define HTTP_NOT_FOUND_MSG "Not found"
#define HTTP_FORBIDDEN "403"
#define HTTP_FORBIDDEN_MSG "Forbidden"

#define getHTTP_OK() createAnswer(HTTP_OK, HTTP_OK_MSG)
#define getHTTP_BAD_REQUEST() createAnswer(HTTP_BAD_REQUEST, HTTP_BAD_REQUEST_MSG)
#define getHTTP_NOT_FOUND() createAnswer(HTTP_NOT_FOUND, HTTP_NOT_FOUND_MSG)

#define HTTP_ARG_CONTENT_TYPE "Content-Type"
#define HTTP_ARG_TEXT_HTML "text/html"

#define PUBLIC_HTML "/public_html/"
#define DEFAULT_FILE_NAME "index.html"


typedef int Socket;
typedef struct sockaddr Sockaddr;
typedef struct sockaddr_in Sockaddr_in;

struct HandleClientArgs {
	Socket clientSocket;
	int id;
};

Socket initServerSocket(int port);
int connectionHandler(Socket socket);
void handleClient(Socket clientSocket, int threadID);
void * thread_handleClient(void * args);
char* parseQuery(char* query);
char* createAnswer(char* code, char* message);
char* addArgToAnswer(char* answer, char* argName, char* argValue);
char* addFileToAnswer(char* answer, char* fileContent);
char* getFile(char* path, int* errcode);
int sendString(Socket s, char* toWrite);
















