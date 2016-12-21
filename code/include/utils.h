#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

void copyString(char* src, char* dest);
char** tokenize(char* toTokenize, char* delimiters, int * resultSize);
void concatString(char* dest, char* toConcat);
