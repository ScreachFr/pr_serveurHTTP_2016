#include <stdlib.h>
#include <stdio.h>
#include "../include/logger.h"
#include "../include/server.h"

int main(int argc, char** argv) {
	/*
	if (argc < 2){
		printf("too few args\n");
		return EXIT_FAILURE;
	}

	int port = atoi(argv[1]);
	Socket s;


	s = initServerSocket(port);

	printf("s : %d\n", s);
	connectionHandler(s);
	*/
	/*
	char testString[] = "GET /test/filedeouf/index.html HTTP/1.1\nHost: 127.0.0.1\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0";

	char* path;
	char request[REQUEST_BUFFER_SIZE];
	path = parseQuery(testString, &request);

	printf("Parsed path : %s\n", path);
	printf("request : %s\n", request);
	/*
	char* result;
	int errcode = 0;
	result = getFile("/home/alexandre/public_html/index.html", &errcode);

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
	int ret = 0;
	char* fileContent = getFile("/home/alexandre/public_html/index.html", &ret);
	char* answer = createAnswer("200", "test");

	answer = addArgToAnswer(answer, "test", "lol");
	answer = addArgToAnswer(answer, "test2", "lol");
	answer = addArgToAnswer(answer, "test3", "lol");
	answer = addArgToAnswer(answer, "test4", "lol");
	answer = addArgToAnswer(answer, "test5", "lol");
	answer = addArgToAnswer(answer, "test6", "lol");
	answer = addFileToAnswer(answer, fileContent);

	printf("%s\n", answer);
	/*
	int errcode = 0;
	char* path;

	path = getPath("script.sh", &errcode);

	printf("getpath -> %s, errcode -> %d\n", path, errcode);
	*/
	/*
	// char* path = strdup("/home/alexandre/public_html/script.sh");
	char* path = strdup("/bin/echo test");
	int result;
	result = answerRunnable(NULL, path, NULL);
	printf("%d\n", result);
	*/
	char* toTokenize = "a b c d e f g h i j k ldlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddlddddddddddddddddddddddddddddddddddddddddddddddddldddddddddddddddddddddddddddddddddddddddddddddddd m d g g d s s e f f f d d s d f g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d g t r d d s s ds d d d f g g r e s s d f gg f d";
	//char* toTokenize = "a b c d e";
	char** result;
	int i;
	int size = 0;
	result = tokenize(toTokenize, " ", &size);

	printf("size : %d\n", size);
	for (i = 0; i < size; i++) {
		printf("%s\n", result[i]);
	}

	return EXIT_SUCCESS;
}
