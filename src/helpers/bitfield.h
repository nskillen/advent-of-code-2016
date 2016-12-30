#ifndef __BITFIELD_H__
#define __BITFIELD_H__

typedef struct {
	int size;
	unsigned char *bits;
} bitfield;

bitfield* create_bitfield(int size);
void destroy_bitfield(bitfield *b);

int bitfield_check_bit(bitfield *b, int bit);

void bitfield_set_bit(bitfield *b, int bit);
void bitfield_clear_bit(bitfield *b, int bit);

#endif//__BITFIELD_H__
