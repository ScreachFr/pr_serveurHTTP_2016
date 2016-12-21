#include "../include/utils.h"

void copyString(char* src, char* dest) {
	
	while(*src != '\0') {
		*dest = *src;
		dest++;
		src++;
	}
	
	*dest = '\0';
}

char** tokenize(char* toTokenize, char* delimiters, int * resultSize) {
	char** result;
	char* crtToken;
	int i = 0;
	
	crtToken = strtok(toTokenize, delimiters);
	//Nothing to tokenize.
	if (crtToken == NULL)
		return NULL;
	
	result = malloc(sizeof(char**) * 254);//XXX Temporary fix
	result[i] = malloc(sizeof(crtToken));
	copyString(crtToken, result[i]);

	while ((crtToken = strtok(NULL, delimiters)) != NULL) {
		i++;
		//result = realloc(result, sizeof(char**) * i);
		result[i] = malloc(sizeof(crtToken));
		copyString(crtToken, result[i]);
	}
	
	
	*resultSize = i+1;
	
	free(crtToken);

	return result;
}

void concatString(char* dest, char* toConcat) {
	dest += strlen(dest);
	
	while(*toConcat != '\0') {
		*dest = *toConcat;
		dest++;
		toConcat++;
	}
	
	*dest = '\0';
	
}
