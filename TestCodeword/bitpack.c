#include "assert.h"
#include "bitpack.h"

#define WSIZE 64
Except_T Bitpack_Overflow = {"Value doesn't fit in width\n"};

// private bitwise shift functions, to handle undefined behavior in hardware
static inline uint64_t leftshift(uint64_t n, unsigned shift) {
    assert(shift < WSIZE);
    return n << shift;
}

static inline uint64_t rightshift(uint64_t n, unsigned shift) {
    assert(shift < WSIZE);
    return n >> shift;
}

// check if a n fits into a word
bool Bitpack_fitsu(uint64_t n, unsigned width) {
    // a number can not fit into a word of 0 width
    if (!width) {
        return false;
    }
    uint64_t max_pow = leftshift(1, width - 1);
    return n <= (max_pow | (max_pow - 1));
}

bool Bitpack_fitss(int64_t n, unsigned width) {
    // a number can not fit into a word of 0 width
    if (!width) {
        return false;
    }
    uint64_t max_pow = leftshift(1, width - 1);
    return n + max_pow <= (max_pow | (max_pow - 1));
}

// helper function which contains the asserts used in the following 4 functions 
static inline void check(unsigned width, unsigned lsb) {
    assert(width <= WSIZE);
    assert(width + lsb <= WSIZE);
}

// extract a field starting from lsb, with length width, from word 
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
    check(width, lsb);
    // can't extract a 0 width field from a word
    if (!width) {
        return 0;
    }
    uint64_t max_pow = leftshift(1, width - 1);
    uint64_t mask = max_pow | (max_pow - 1);
    mask = leftshift(mask, lsb);
    uint64_t new_word = mask & word;
    return rightshift(new_word, lsb);
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) {
    uint64_t new_word = Bitpack_getu(word, width, lsb);
    // if new_word should have sign bit
    // width guaranteed not to be 0 because of Bitpack_getu
    if (rightshift(new_word, width - 1)) {
	uint64_t neg_bits = leftshift(UINT64_MAX, width);
	new_word |= neg_bits;
    }
    return new_word;
}

// core part of new algorithm used by both signed and unsigned new functions
static inline uint64_t new_core(uint64_t word, unsigned width,
		                unsigned lsb, uint64_t value) {
    uint64_t temp = 0;
    uint64_t left_bits = 0;
    uint64_t clear_back = lsb + width;
    if (clear_back != WSIZE) {
    	temp = rightshift(word, clear_back);
    	left_bits = leftshift(temp, clear_back);
    }
    uint64_t right_bits = 0;
    uint64_t clear_front = WSIZE - lsb;
    if (clear_front != WSIZE) {
    	temp = leftshift(word, clear_front);
    	right_bits = rightshift(temp, clear_front);
    }
    uint64_t shift_in = leftshift(value, lsb);
    return left_bits | shift_in | right_bits;
}

// insert a field starting from lsb, with length width, to word 
uint64_t Bitpack_newu(uint64_t word, unsigned width, 
		      unsigned lsb, uint64_t value) {
    check(width, lsb);
    if (!Bitpack_fitsu(value, width)) {
	RAISE(Bitpack_Overflow);    
    }
    return new_core(word, width, lsb, value);
}

// insert a signed value at lsb, with bitlength width, to word
uint64_t Bitpack_news(uint64_t word, unsigned width, 
		      unsigned lsb, int64_t value) {
    check(width, lsb);
    if (!Bitpack_fitss(value, width)) {
	RAISE(Bitpack_Overflow);    
    }
    // shave off leading 1s, 
    // width guaranteed not to be 0 because of Bitpack_fitss
    value = leftshift(value, WSIZE - width);
    value = rightshift(value, WSIZE - width);
    return new_core(word, width, lsb, value);
}

