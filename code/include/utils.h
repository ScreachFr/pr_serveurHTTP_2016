#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

#define TYPE_UNKOWN 0
#define TYPE_HTML 1
#define TYPE_RUNNABLE 2

#define EXT_HTML "html"
#define EXT_RUNNABLE "sh"

char** tokenize(char* toTokenize, char* delimiters, int * resultSize);
void clearArray(char* array, int size);
int getFileType(char* path);
