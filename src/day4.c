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

#include "helpers/DEBUG.h"
#include "helpers/linked_list.h"
#include "helpers/read_puzzle_input.h"
#include "helpers/sorting.h"
#include "helpers/vector.h"

void process_line(void*, size_t data_len);
void process_line_p2(void*, size_t data_len);

int sector_id_sum = 0;
static const char *pattern = "^([a-z-]+)-([0-9]{3})\\[([a-z]{5})\\]$";
static pcre *rx = NULL;
static vector *rooms = NULL;

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

  printf("Decrypting rooms...\n");

  list_each(input_lines, process_line_p2);

	pcre_free(rx);

	return 0;
}

int comp(void *a, void *b) {
	char *_a = (char*)a;
	char *_b = (char*)b;

  if (DEBUG) { printf("_a: 0x%X\n_b: 0x%X\n", (unsigned int)_a, (unsigned int)_b); }
  if (DEBUG) { printf("Comparing ('%c',%d) to ('%c',%d)\n", _a[0] + 'a', _a[1], _b[0] + 'a', _b[1]); }

	if (_a[1] > _b[1]) { return -1; }
	if (_a[1] < _b[1]) { return 1; }
	if (_a[0] < _b[0]) { return -1; }
	if (_a[0] > _b[0]) { return 1; }
	return 0;
}

typedef struct {
  char *name;
  char *encrypted_name;
  int sector_id;
  char checksum[5];
} room;

int is_room_real(room r) {
  if (DEBUG) { printf("Is-room-real: r{ encrypted_name: \"%s\", sector_id: %d, checksum: \"%s\" }\n", r.encrypted_name, r.sector_id, r.checksum); }
	char letters[26];
	memset(letters, 0, 26 * sizeof(char));

	for (size_t i = 0; i < strlen(r.encrypted_name); i++) {
    if (r.encrypted_name[i] < 'a' || r.encrypted_name[i] > 'z') { continue; }
		letters[r.encrypted_name[i] - 'a'] += 1;
	}

	vector *tmp = vector_create(2 * sizeof(char), 26, 1, comp);
	for (size_t i = 0; i < 26; i++) {
		char thing[2] = { i, letters[i] };
		vector_push(tmp, thing);
	}
  if (DEBUG) { printf ("before sort\n"); }
	v_insertion_sort(tmp);
  if (DEBUG) { printf ("after sort\n"); }

  for (size_t i = 0; i < 5; i++) {
    char* t = (char*)vector_get(tmp, i);
    if (t[0] + 'a' != r.checksum[i]) {
      free(t);
      return 0;
    }
    free(t);
  }

  return 1;
}

void decrypt_room(room *r) {
  r->name = (char*)calloc(strlen(r->encrypted_name)+1, sizeof(char));
  for (size_t i = 0; i < strlen(r->encrypted_name); i++) {
    char c = r->encrypted_name[i];
    if (c >= 'a' && c <= 'z') {
      c -= 'a';
      c = (c + (r->sector_id % 26)) % 26;
      r->name[i] = c + 'a';
    } else { r->name[i] = c; }
  }
}

void line_to_room(char *line, room *r) {
  if (DEBUG) { printf("Line-to-room: %s\n", line); }
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
	r->encrypted_name = calloc(crypted_len + 1, sizeof(char));
  if (r->encrypted_name == NULL) {
    fprintf(stderr, "ERROR, unable to allocate %d bytes of memory for the encrypted name\nwith room string %s\n", crypted_len, line);
    exit(1);
  }
	memcpy(r->encrypted_name, line + offsets[2], crypted_len);

  line[offsets[6] - 1] = 0;
	r->sector_id = atoi(line + offsets[4]);
  line[offsets[6] - 1] = '[';

	memcpy(r->checksum, line + offsets[6], 5);
  decrypt_room(r);
}

void process_line(void *data, size_t data_len) {
	char *line = calloc(data_len, sizeof(char));
	strcpy(line, data);

  if (DEBUG) { printf("process-line: %s\n", line); }

  if (rooms == NULL) { rooms = vector_create(sizeof(room), 20, 1, NULL); }

  room r;
  line_to_room(line, &r);

  if (is_room_real(r)) {
    sector_id_sum += r.sector_id;
    vector_push(rooms, &r);
  }
}

void print_room(room r) {
  char *en = r.encrypted_name;
  char *n = r.name;
  int sid = r.sector_id;
  char *cs = calloc(6, sizeof(char));
  memcpy(cs, r.checksum, 5);
  printf("Room %s:\n\tEncrypted name: %s\n\tSector ID: %d\n\tChecksum: %s\n", n, en, sid, cs);
  free(cs);

}

void process_line_p2(void *data, size_t data_len) {
  char *line = (char*)calloc(data_len + 1, sizeof(char));
  strcpy(line, data);

  if (DEBUG) { printf("process-data-p2: %s\n", line); }

  for (size_t i = 0; i < rooms->element_count; i++) {
    print_room(*((room*)vector_get(rooms, i)));
  }
}

