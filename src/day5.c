/* --- Day 5: How About a Nice Game of Chess? ---
 * 
 * You are faced with a security door designed by Easter Bunny engineers that seem to have acquired most of their security knowledge by watching hacking movies.
 * 
 * The eight-character password for the door is generated one character at a time by finding the MD5 hash of some Door ID (your puzzle input) and an increasing integer index (starting with 0).
 * 
 * A hash indicates the next character in the password if its hexadecimal representation starts with five zeroes. If it does, the sixth character in the hash is the next character of the password.
 * 
 * For example, if the Door ID is abc:
 * 
 * The first index which produces a hash that starts with five zeroes is 3231929, which we find by hashing abc3231929; the sixth character of the hash, and thus the first character of the password, is 1.
 * 5017308 produces the next interesting hash, which starts with 000008f82..., so the second character of the password is 8.
 * The third time a hash starts with five zeroes is for abc5278568, discovering the character f.
 * In this example, after continuing this search a total of eight times, the password is 18f47a30.
 * 
 * Given the actual Door ID, what is the password?
 */

#include "helpers/DEBUG.h"
#include "helpers/linked_list.h"
#include "helpers/read_puzzle_input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/md5.h>

char password[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int password_set[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

void process_line(void*, size_t);
void process_line_p2(void*, size_t data_len);

int main(int argc, char **argv) {
	linked_list *input_lines = NULL;
	if (argc > 1) {
		input_lines = read_puzzle_input(argv[1]);
	} else {
		input_lines = read_puzzle_input("day5.txt");
	}

  list_each(input_lines, process_line);
  printf("The door password is: %s\n", password);

  memset(password, 0, 9);
  list_each(input_lines, process_line_p2);
  printf("The second door password is %s\n", password);
}

void process_line(void *data, size_t data_len) {
  char *line = (char*)calloc(data_len + 1, sizeof(char));
  strcpy(line, data);

  size_t pass = 0;
  size_t pass_len = 0;

  char message[256];
  unsigned char digest[17];
  char hex_digest[33];
  MD5_CTX ctx;

  while (pass_len < 8) {
    memset(message, 0, 256);
    sprintf(message, "%s%zu", line, pass++);

    MD5_Init(&ctx);
    MD5_Update(&ctx, message, strlen(message));
    MD5_Final(digest, &ctx);

    memset(hex_digest, 0, 33);
    for (size_t i = 0; i < 16; i++) { sprintf(&hex_digest[i*2], "%02X", (unsigned int)digest[i]); }


    char c = hex_digest[5];
    hex_digest[5] = 0;
    if (strcmp("00000", hex_digest) == 0) {
      if (DEBUG) { printf("Hex digest of %s with %zu: %s, char is %c\n", line, pass, hex_digest, c); }
      password[pass_len++] = c;
    }
  }
  free(line);
}

void process_line_p2(void *data, size_t data_len) {
  char *line = (char*)calloc(data_len + 1, sizeof(char));
  strcpy(line, data);

  size_t pass = 0;
  size_t pass_len = 0;

  char message[256];
  unsigned char digest[17];
  char hex_digest[33];
  MD5_CTX ctx;

  while (pass_len < 8) {
    memset(message, 0, 256);
    sprintf(message, "%s%zu", line, pass++);

    MD5_Init(&ctx);
    MD5_Update(&ctx, message, strlen(message));
    MD5_Final(digest, &ctx);

    memset(hex_digest, 0, 33);
    for (size_t i = 0; i < 16; i++) { sprintf(&hex_digest[i*2], "%02X", (unsigned int)digest[i]); }


    size_t p = hex_digest[5] - '0';
    size_t pass_chars = 0;
    char v = hex_digest[6];
    if (p > 7) { continue; }

    if (password_set[p]) { continue; }

    hex_digest[5] = 0;
    if (strcmp("00000", hex_digest) == 0) {
      if (DEBUG) { printf("Hex digest of %s with %zu: %s, char in position %zu is %c\n", line, pass, hex_digest, p, v); }
      password[p] = v;
      password_set[p] = 1;
      pass_chars += 1;
    }
    if (pass_chars == 8) { break; }
  }
  free(line);
}
