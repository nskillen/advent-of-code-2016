/*
 * Advent of Code - Day 2 problem
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers/linked_list.h"
#include "helpers/read_puzzle_input.h"

int *code_digits;
int position = 5;
int position_p2[2] = { 0, 2 };

int can_move_to[5][5] = {
	{ 0, 0, 1, 0, 0 },
	{ 0, 1, 1, 1, 0 },
	{ 1, 1, 1, 1, 1 },
	{ 0, 1, 1, 1, 0 },
	{ 0, 0, 1, 0, 0 }
};

char place_values[5][5] = {
	{ 0,  0,  1,  0, 0 },
	{ 0,  2,  3,  4, 0 },
	{ 5,  6,  7,  8, 9 },
	{ 0, 10, 11, 12, 0 },
	{ 0,  0, 13,  0, 0 }
};

void process_input(size_t list_index, void *data, size_t data_len);
void process_input_part_2(size_t list_index, void *data, size_t data_len);

int main(int argc, char **argv) {
	linked_list *input_lines = NULL;
	if (argc > 1) {
		input_lines = read_puzzle_input(argv[1]);
	} else {
		input_lines = read_puzzle_input("day2.txt");
	}

	int code_length = input_lines->length;
	code_digits = (int*)calloc(code_length, sizeof(int));

	list_each_with_index(input_lines, process_input);

	printf("The bathroom code is: ");
	for (int i = 0; i < code_length; i++) {
		printf("%d", code_digits[i]);
	}
	printf("\n");

	memset(code_digits, 0, code_length * sizeof(int));
	list_each_with_index(input_lines, process_input_part_2);

	printf("Oh no! The keypad is different!\n");
	printf("The new code is: ");
	for (int i = 0; i < code_length; i++) {
		printf("%X,", code_digits[i]);
	}
	printf("\n");

	return 0;
}

void process_input(size_t list_index, void *data, size_t data_len) {
	char *line = (char*)calloc(data_len + 1, sizeof(char));
  memcpy(line, data, data_len);

	for (size_t i = 0; i < strlen(line); i++) {
		switch (line[i]) {
			case 'U':
				if (position > 3) { position -= 3; }
				break;
			case 'D':
				if (position < 7) { position += 3; }
				break;
			case 'L':
				if (position % 3 != 1) { position -= 1; }
				break;
			case 'R':
				if (position % 3 != 0) { position += 1; }
				break;
			default:
				fprintf(stderr, "ERROR: unexpected character '%c' in string!\n", line[i]);
				exit(1);
		}
	}

	code_digits[list_index] = position;

  free(line);

}

void process_input_part_2(size_t list_index, void *data, size_t data_len) {
	char *line = (char*)calloc(data_len + 1, sizeof(char));
  memcpy(line, data, data_len);

	int *x = &position_p2[0];
	int *y = &position_p2[1];

	for (size_t i = 0; i < strlen(line); i++) {
		switch (line[i]) {
			case 'U':
				if (*y > 0 && can_move_to[(*y)-1][*x]) { *y -= 1; }
				break;
			case 'D':
				if (*y < 4 && can_move_to[(*y)+1][*x]) { *y += 1; }
				break;
			case 'L':
				if (*x > 0 && can_move_to[*y][(*x)-1]) { *x -= 1; }
				break;
			case 'R':
				if (*x < 4 && can_move_to[*y][(*x)+1]) { *x += 1; }
				break;
			default:
				fprintf(stderr, "ERROR: unexpected character '%c' in string!\n", line[i]);
				exit(1);
		}
	}

	code_digits[list_index] = place_values[*y][*x];

  free(line);
}

