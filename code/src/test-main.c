#include <stdlib.h>
#include <stdio.h>
#include "../include/logger.h"
#include "../include/server.h"

int main(int argc, char** argv) {
	if (argc < 2){
		printf("too few args\n");
		return EXIT_FAILURE;
	}
	
	int port = atoi(argv[1]);
	Socket s;
	
	
	s = initServerSocket(port);
	
	printf("s : %d\n", s);
	
	connectionHandler(s);
	/*
	char testString[] = "GET / HTTP/1.1\nHost: 127.0.0.1\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\n\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0";
	
	char* path;
	
	path = parseQuery(testString);	
	
	printf("Parsed path : %s\n", path);
	*/
	/*
	char* result;
	
	result = getFile("/");

	printf("result : %s\n", result);
	*/
	/*
	char* test;
	
	test = realloc(NULL, sizeof("test"));
	copyString("test", test);
	printf("%s\n", test);
	
	test = realloc(test, sizeof("test") + sizeof("test2"));
	concatString(test, "test2");
	printf("%s\n", test);
	*/
	/*
	char* fileContent = getFile("/");	
	char* answer = createAnswer("200", "test");
	
	answer = addArgToAnswer(answer, "test", "lol");
	answer = addArgToAnswer(answer, "test2", "lol");
	answer = addArgToAnswer(answer, "test3", "lol");
	answer = addArgToAnswer(answer, "test4", "lol");
	answer = addArgToAnswer(answer, "test5", "lol");
	answer = addArgToAnswer(answer, "test6", "lol");
	answer = addFileToAnswer(answer, fileContent);
	
	printf("%s\n", answer);
	*/
	/*
	time_t result = time(NULL);
    char* time = asctime(gmtime(&result));
    char pid[5];
    char threadID[15];
    char* request = "GET / HTTP/1.1";
    char resultSize[15];
    sprintf(pid, "%d", getpid());
    sprintf(threadID, "%d", 25);
    sprintf(resultSize, "%d", 256);
    
	writeRequestLog("localhost", time, &pid, &threadID, request, HTTP_OK, &resultSize);
	*/
	return EXIT_SUCCESS;
}


