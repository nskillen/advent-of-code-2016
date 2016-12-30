/* --- Day 4: Security Through Obscurity ---
 * 
 * Finally, you come across an information kiosk with a list of rooms. Of course, the list is encrypted and full of decoy data, but the instructions to decode the list are barely hidden nearby. Better remove the decoy data first.
 * 
 * Each room consists of an encrypted name (lowercase letters separated by dashes) followed by a dash, a sector ID, and a checksum in square brackets.
 * 
 * A room is real (not a decoy) if the checksum is the five most common letters in the encrypted name, in order, with ties broken by alphabetization. For example:
 * 
 * aaaaa-bbb-z-y-x-123[abxyz] is a real room because the most common letters are a (5), b (3), and then a tie between x, y, and z, which are listed alphabetically.
 * a-b-c-d-e-f-g-h-987[abcde] is a real room because although the letters are all tied (1 of each), the first five are listed alphabetically.
 * not-a-real-room-404[oarel] is a real room.
 * totally-real-room-200[decoy] is not.
 * Of the real rooms from the list above, the sum of their sector IDs is 1514.
 * 
 * What is the sum of the sector IDs of the real rooms?
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pcre.h>

#include "helpers/linked_list.h"
#include "helpers/read_puzzle_input.h"
#include "helpers/sorting.h"

void process_line(void*, int data_len);
void process_line_p2(void*, int data_len);

int sector_id_sum = 0;
static const char *pattern = "^([a-z-]+)-([0-9]{3})\\[([a-z]{5})\\]$";
static pcre *rx = NULL;

int main(int argc, char **argv) {
	linked_list *input_lines = NULL;
	if (argc > 1) {
		input_lines = read_puzzle_input(argv[1]);
	} else {
		input_lines = read_puzzle_input("day4.txt");
	}

	const char *pcre_error_str;
	int pcre_error_offset;
	rx = pcre_compile(pattern, 0, &pcre_error_str, &pcre_error_offset, NULL);
	if (rx == NULL) {
		fprintf(stderr, "ERROR compiling rx '%s': %s\n", pattern, pcre_error_str);
		exit(1);
	}

	list_each(input_lines, process_line);

	printf("The sum of the sector IDs for the real rooms is %d!\n", sector_id_sum);

	pcre_free(rx);

	return 0;
}

int list_comp(void *a, void *b) {
	char *_a = (char*)a;
	char *_b = (char*)b;

	if (_a[1] > _b[1]) { return -1; }
	if (_a[1] < _b[1]) { return 1; }
	if (_a[0] < _b[0]) { return -1; }
	if (_a[0] > _b[0]) { return 1; }
	return 0;
}

void process_line(void *data, int data_len) {
	char *line = calloc(data_len, sizeof(char));
	strcpy(line, data);

	char letters[26];
	memset(letters, 0, 26 * sizeof(char));

	char checksum[6];
	int sector_id;
	char* encrypted_name;

	memset(checksum, 0, 6 * sizeof(char));
	sector_id = 0;
	encrypted_name = calloc(strlen(line) - 10 + 1, sizeof(char));

	int offsets[30];
	int result = pcre_exec(rx, NULL, line, strlen(line), 0, 0, offsets, 30);
	if (result < 0) {
		switch(result) {
			case PCRE_ERROR_NOMATCH      : printf("String '%s' did not match the pattern '%s'\n", line, pattern); break;
			case PCRE_ERROR_NULL         : printf("Something was null\n");                                        break;
			case PCRE_ERROR_BADOPTION    : printf("A bad option was passed\n");                                   break;
			case PCRE_ERROR_BADMAGIC     : printf("Magic number bad (compiled re corrupt?)\n");                   break;
			case PCRE_ERROR_UNKNOWN_NODE : printf("Something kooky in the compiled re\n");                        break;
			case PCRE_ERROR_NOMEMORY     : printf("Ran out of memory\n");                                         break;
			default                      : printf("Unknown error\n");                                             break;
		}
		exit(1);
	}

	int crypted_len = offsets[3] - offsets[2];
	encrypted_name = malloc(crypted_len * sizeof(char));
	memcpy(encrypted_name, line + offsets[2], crypted_len);
	encrypted_name[crypted_len] = 0;

	memcpy(letters, line + offsets[4], 3);
	sector_id = atoi(letters);

	memcpy(checksum, line + offsets[6], 5);

	for (size_t i = 0; i < strlen(encrypted_name); i++) {
		letters[encrypted_name[i] - 'a'] += 1;
	}

	linked_list *tmp = create_list(NULL, list_comp);
	for (size_t i = 0; i < 26; i++) {
		char thing[2] = { i, letters[i] };
		list_insert(tmp, thing, sizeof(char[2]));
	}
	selection_sort(tmp);
}
