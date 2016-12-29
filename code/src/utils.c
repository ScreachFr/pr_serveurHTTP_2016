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

void clearArray(char* array, int size) {
	int i;

	for (i = 0; i < size; i++)
		array[i] = NULL;
}

/**
 * XXX This should use mime types.
 */
int getFileType(char* path) {
	char** tokenResult;
	int size = 0;
	char* ext;
	char* safePath = strdup(path);

	tokenResult = tokenize(safePath, ".", &size);

	if (tokenResult == NULL)
		return TYPE_UNKOWN;

	ext = tokenResult[size-1];

	if (strcmp(ext, EXT_HTML) == 0) {
		return TYPE_HTML;
	} else if (strcmp(ext, EXT_RUNNABLE) == 0) {
		return TYPE_RUNNABLE;
	} else {
		return TYPE_UNKOWN;
	}
}
