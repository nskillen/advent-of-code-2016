#include "md5.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "debug.h"

const size_t CHUNK_BITS = 512;
const size_t CHUNK_BYTES = CHUNK_BITS / 8;

#define F(B,C,D) (B & C) | ((~B) & D)
#define G(B,C,D) (B & D) | (C & (~D))
#define H(B,C,D) (B ^ C ^ D)
#define I(B,C,D) (C ^ (B | (~D)))

//typedef unsigned char      uint8_t;
//typedef unsigned short     uint16_t;
//typedef unsigned long      uint32_t;
//typedef unsigned long long uint64_t;

uint32_t s[64] = {
  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
};

uint32_t K[64] = {
  0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
  0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
  0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
  0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
  0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
  0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
  0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
  0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
  0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
  0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
  0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
  0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
  0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
  0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
  0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

static uint32_t left_rotate(uint32_t x, uint32_t c) {
  return (x << c) | (x >> (32 - c));
}

void md5(const char *message, char *hash) {
  if (message == NULL || hash == NULL) { return; }

  uint64_t a0 = 0x67452301;
  uint64_t b0 = 0xefcdab89;
  uint64_t c0 = 0x98badcfe;
  uint64_t d0 = 0x10325476;

  size_t msg_len = strlen(message);
  size_t chunks = (msg_len / CHUNK_BYTES) + (msg_len % CHUNK_BYTES == 0 ? 0 : 1);

  size_t padding_bytes = CHUNK_BYTES - (msg_len % CHUNK_BYTES);
  if (padding_bytes < 9) { padding_bytes += CHUNK_BYTES; chunks += 1; }

  char *padded_message = (char*)calloc(msg_len + padding_bytes, sizeof(char));
  strcpy(padded_message, message);
  memset(padded_message + msg_len, 0x80, 1);
  *((uint64_t*)(padded_message + (msg_len + padding_bytes) - 8)) = msg_len;

  size_t chunk_num = 0;
  char chunk[CHUNK_BYTES];
  while (chunk_num < chunks) {
    memcpy(chunk, padded_message + (chunk_num * CHUNK_BYTES), CHUNK_BYTES);

    uint32_t M[16];
    for (size_t i = 0; i < 16; i++) {
      M[i] = (uint32_t)(chunk + (i * sizeof(uint32_t)));
    }

    uint32_t A = a0;
    uint32_t B = b0;
    uint32_t C = c0;
    uint32_t D = d0;

    uint32_t F, g, dTemp;

    for (size_t i = 0; i < CHUNK_BYTES; i++) {
      if (i < 0x10) {
        F = 0;
        g = i;
      } else if (i < 0x20) {
        F = (D & B) | (~D & C);
        g = (5 * i + 1) % 16;
      } else if (i < 0x30) {
        F = B ^ C ^ D;
        g = (3 * i + 5) % 16;
      } else {
        F = C ^ (B | ~D);
        g = (7 * i) % 16;
      }
      
      dTemp = D;
      D = C;
      C = B;
      B = B + left_rotate((A + F + K[i] + M[g]), s[i]);
      A = dTemp;
    }

    a0 += A;
    b0 += B;
    c0 += C;
    d0 += D;

    a0 %= 0x100000000;
    b0 %= 0x100000000;
    c0 %= 0x100000000;
    d0 %= 0x100000000;

    chunk_num += 1;
  }

  sprintf(hash, "%0llX%0llX%0llX%0llX", a0, b0, c0, d0);

  if (DEBUG) { printf("calculated md5: %s\n", hash); }

  free(padded_message);
}
