#include "../include/utils.h"

char** tokenize(char* toTokenize, char* delimiters, int * resultSize) {
	char** result;
	char* crtToken;
	int i = 0;
	char* toTokenize_safe = strdup(toTokenize);

	// printf("toTokenize_safe : \n%s\n", toTokenize_safe);

	// First tokenization
	crtToken = strtok(toTokenize_safe, delimiters);
	//Nothing to tokenize.
	if (crtToken == NULL)
		return NULL;

	//Copying first result to final result
	result = (char**) malloc(sizeof(char*));
	result[i] = (char*) malloc(strlen(crtToken) * sizeof(char));
	strcpy(result[i], strdup(crtToken));

	while ((crtToken = strtok(NULL, delimiters)) != NULL) {
		i++;
		result = (char**) realloc(result, (i+1) * sizeof(char*));
		result[i] = (char*)malloc((strlen(crtToken)+1) * sizeof(char));
		strcpy(result[i], strdup(crtToken));
	}

	*resultSize = i+1;

	free(crtToken);
	return result;
}

void clearArray(char* array, int size) {
	int i;

	for (i = 0; i < size; i++)
		array[i] = 0;
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





