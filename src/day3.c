/*
 * Advent of Code - Day 3 problem
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers/linked_list.h"
#include "helpers/read_puzzle_input.h"

int impossible_count = 0;

void process_line(void*, int data_len);
void process_line_p2(void*, int data_len);

int main(int argc, char **argv) {
	linked_list *input_lines = NULL;
	if (argc > 1) {
		input_lines = read_puzzle_input(argv[1]);
	} else {
		input_lines = read_puzzle_input("day3.txt");
	}
	
	list_each(input_lines, process_line);

	printf("There are %d possible triangles!\n", input_lines->length - impossible_count);

	impossible_count = 0;
	list_each(input_lines, process_line_p2);

	printf("Oops, the triangles are written vertically!\n");
	printf("There are %d possible triangles!\n", input_lines->length - impossible_count);

	return 0;
}

void swap(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int impossible(int sides[3]) {
	if ((sides[0] + sides[1] <= sides[2]) ||
		(sides[0] + sides[2] <= sides[1]) ||
		(sides[1] + sides[2] <= sides[0])) {
		return 1;
	}
	return 0;
}

int impossible_print(int sides[3]) {
	printf("Checking: (%d,%d,%d)\n", sides[0],sides[1],sides[2]);
	return impossible(sides);
}

void process_line(void *data, int data_len) {
	char *line = calloc(data_len, sizeof(char));
	strcpy(line, data);

	int sides[3];

	sides[0] = atoi(strtok(line, " "));
	sides[1] = atoi(strtok(NULL, " "));
	sides[2] = atoi(strtok(NULL, " "));

	if (impossible(sides)) { impossible_count += 1; }

	free(line);
}

void process_line_p2(void *data, int data_len) {
	char *line = calloc(data_len, sizeof(char));
	strcpy(line, data);

	static int sides[3][3] = {
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 }
	};

	int rsides[3];

	rsides[0] = atoi(strtok(line, " "));
	rsides[1] = atoi(strtok(NULL, " "));
	rsides[2] = atoi(strtok(NULL, " "));

	if (sides[0][0] == -1) {
		sides[0][0] = rsides[0];
		sides[1][0] = rsides[1];
		sides[2][0] = rsides[2];
	} else if (sides[0][1] == -1) {
		sides[0][1] = rsides[0];
		sides[1][1] = rsides[1];
		sides[2][1] = rsides[2];
	} else {
		sides[0][2] = rsides[0];
		sides[1][2] = rsides[1];
		sides[2][2] = rsides[2];

		if (impossible(sides[0])) { impossible_count += 1; }
		if (impossible(sides[1])) { impossible_count += 1; }
		if (impossible(sides[2])) { impossible_count += 1; }

		sides[0][0] = -1;
		sides[0][1] = -1;
		sides[0][2] = -1;
	}

	free(line);
}
