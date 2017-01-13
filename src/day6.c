/* --- Day 6: Signals and Noise ---
 * 
 * Something is jamming your communications with Santa. Fortunately, your signal is only partially jammed, and protocol in situations like this is to switch to a simple repetition code to get the message through.
 * 
 * In this model, the same message is sent repeatedly. You've recorded the repeating message signal (your puzzle input), but the data seems quite corrupted - almost too badly to recover. Almost.
 * 
 * All you need to do is figure out which character is most frequent for each position.
 */

#include "helpers/read_puzzle_input.h"
#include "helpers/sorting.h"
#include "helpers/vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t MESSAGE_LENGTH = 8;

char message[MESSAGE_LENGTH + 1];
vector *letters_in_col[MESSAGE_LENGTH];

int comp(void*, void*);
void process_line(void *data, size_t data_len);

int main(int argc, char** argv) {
  linked_list *input_lines = NULL;
  if (argc > 1) {
    input_lines = read_puzzle_input(argv[1]);
  } else {
    input_lines = read_puzzle_input("day6.txt");
  }

  for (size_t i = 0; i < MESSAGE_LENGTH; i++) {
    letters_in_col[i] = vector_create(2 * sizeof(char), 26, 1, comp);
  }

  list_each(input_lines, process_line);

  for (size_t i = 0; i < MESSAGE_LENGTH; i++) {
    v_selection_sort(letters_in_col[i]);
    message[i] = ((char*)vector_get(letters_in_col[i], 0))[0];
  }


  printf("The error-corrected message is: %s\n", message);
}

int comp(void *a, void *b) {
  char *_a = (char*)a;
  char *_b = (char*)b;

  if      (_a[1] < _b[1]) { return -1; }
  else if (_a[1] > _b[1]) { return  1; }
  else                    { return  0; }
}

void process_line(void *data, size_t data_len) {
  char *line = calloc(data_len + 1, sizeof(char));
  memcpy(line, data, data_len);

  for (size_t i = 0; i < MESSAGE_LENGTH; i++) {
    size_t idx = line[i] - 'a';
    char *elem = (char*)vector_get(letters_in_col[i], idx);
    elem[0] = line[i];
    elem[1]++;
  }
}
