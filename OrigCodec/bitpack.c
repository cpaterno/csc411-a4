#include "assert.h"
#include "bitpack.h"

#define WSIZE 64
// Hanson exception to raise when a value can't be inserted into a word
Except_T Bitpack_Overflow = {"Value doesn't fit in width\n"};

// private bitwise shift functions, to handle undefined behavior in hardware
static inline uint64_t leftshift(uint64_t n, unsigned shift) {
    assert(shift <= WSIZE);
    // do nothing
    if (shift == 0) {
        return n;
    }
    // clear word
    if (shift == WSIZE) {
        return 0;
    }
    return n << shift;
}

static inline uint64_t urightshift(uint64_t n, unsigned shift) {
    assert(shift <= WSIZE);
    // do nothing
    if (shift == 0) {
        return n;
    }
    // clear word
    if (shift == WSIZE) {
        return 0;
    }
    return n >> shift;
}

static inline int64_t srightshift(uint64_t n, unsigned shift) {
    assert(shift <= WSIZE);
    // do nothing
    if (shift == 0) {
        return n;
    }
    // return word full of sign bits
    if (shift == WSIZE) {
        return -1;
    }
    return (int64_t)n >> shift;
}

// check if an unsigned value, n, fits into a word
bool Bitpack_fitsu(uint64_t n, unsigned width) {
    // a number can not fit into a word of 0 width
    if (width == 0) {
        return false;
    }
    uint64_t max_pow = leftshift(1, width - 1);
    // range [0, UINTW_MAX], where w is the width of the number in bits
    return n <= (max_pow | (max_pow - 1));
}

// check if a signed value, n, fits into a word
bool Bitpack_fitss(int64_t n, unsigned width) {
    // a number can not fit into a word of 0 width
    if (width == 0) {
        return false;
    }
    int64_t min = srightshift(INT64_MIN, WSIZE - width);
    // range [INTW_MIN, INTW_MAX], where w is the width of the number in bits
    return n >= min && n <= ~min;
}

// helper function which contains the asserts used in the following functions 
static inline void check(unsigned width, unsigned lsb) {
    assert(width <= WSIZE);
    assert(width + lsb <= WSIZE);
}

// extract a field starting from lsb, with length width, from word 
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
    check(width, lsb);
    // slide field to rightmost side of bit vector
    uint64_t start = urightshift(word, lsb);
    // slide field to the left most side of the bit vector 
    // clearing the back bits
    uint64_t new_word = leftshift(start, WSIZE - width);
    // slide field to the right most side of the bit vector 
    // clearing the front
    return urightshift(new_word, WSIZE - width);
}

// extract a field starting from lsb, with length width, from word
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) {
    check(width, lsb);
    // slide field to rightmost side of bit vector
    uint64_t start = urightshift(word, lsb);
    // slide field to the left most side of the bit vector 
    // clearing the back bits
    int64_t new_word = leftshift(start, WSIZE - width);
    // add back leading sign bits if negative
    // slide field to the right most side of the bit vector 
    // clearing the front
    if (new_word & INT64_MIN) {
        // add back leading sign bits if negative
        new_word = srightshift(new_word, WSIZE - width);    
    } else {
        new_word = urightshift(new_word, WSIZE - width);
    }
    return new_word;
}

// core part of new algorithm used by both signed and unsigned new functions
static inline uint64_t new_core(uint64_t word, unsigned width,
		                        unsigned lsb, uint64_t value) {
    // get left bits
    uint64_t clear_back = lsb + width;
    uint64_t left_bits = urightshift(word, clear_back);
    left_bits = leftshift(left_bits, clear_back);
    
    // get right bits
    uint64_t clear_front = WSIZE - lsb;    
    uint64_t right_bits = leftshift(word, clear_front);
    right_bits = urightshift(right_bits, clear_front);
    
    // shift in middle value
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
    // shave off leading sign bits (1s)
    value = leftshift(value, WSIZE - width);
    value = urightshift(value, WSIZE - width);
    return new_core(word, width, lsb, value);
}
