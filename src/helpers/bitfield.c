#include "bitfield.h"

#include <stdlib.h>

bitfield* create_bitfield(int size) {
	bitfield *b = NULL;
	b = (bitfield*)malloc(sizeof(bitfield));
	if (b == NULL) { return b; }

	b->size = size;

	int num_bytes = size / sizeof(char);
	if (size % sizeof(char) > 0) {
		num_bytes += 1; // integer division rounds down, so if there is a remainder we need to add one.
	}

	b->size = num_bytes;

	b->bits = NULL;
	b->bits = (unsigned char*)malloc(num_bytes * sizeof(unsigned char));
	if (b->bits == NULL) {
		free(b);
		return NULL;
	}

	for (int i = 0; i < num_bytes; i++) {
		b->bits[i] = 0;
	}

	return b;
}

void destroy_bitfield(bitfield *b) {
	free(b->bits);
	free(b);
}

int bitfield_check_bit(bitfield *b, int bit) {
	int byte = bit / sizeof(unsigned char);
	int offset = bit % sizeof(unsigned char);
	
	if (byte >= b->size || byte < 0) { return -1; }

	return (b->bits[byte] >> offset) & 0x01;
}

void bitfield_set_bit(bitfield *b, int bit) {
	int byte = bit / sizeof(unsigned char);
	int offset = bit % sizeof(unsigned char);

	if (byte >= b->size || byte < 0) { return; }

	b->bits[byte] |= (0x01 << offset);
}

void bitfield_clear_bit(bitfield *b, int bit) {
	int byte = bit / sizeof(unsigned char);
	int offset = bit % sizeof(unsigned char);

	if (byte >= b->size || byte < 0) { return; }

	b->bits[byte] &= ~(0x01 << offset);
}

