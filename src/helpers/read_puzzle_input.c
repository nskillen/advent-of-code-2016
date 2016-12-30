#include "read_puzzle_input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *DELIMS = "\n";

linked_list* read_puzzle_input(const char *filename) {
	linked_list *list = create_list(NULL, NULL);

	FILE *fp = NULL;
	int flen = 0;
	char *contents = NULL;
	char *tok = NULL;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Unable to open data file: %s\n", filename);
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	flen = ftell(fp) + 1;
	fseek(fp, 0, SEEK_SET);

	contents = (char*)calloc(flen, sizeof(char));
	if (contents == NULL) {
		fprintf(stderr, "Unable to allocate %d bytes to read from file %s\n", flen, filename);
		fclose(fp);
		exit(1);
	}

	fread(contents, sizeof(char), flen, fp);
	fclose(fp);

	tok = strtok(contents, DELIMS);

	do { list_insert(list, tok, 1+strlen(tok)); } while ((tok = strtok(NULL, DELIMS)) != NULL);

	free(contents);

	return list;
}
