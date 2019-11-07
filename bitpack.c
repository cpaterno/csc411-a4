#include "assert.h"
#include "bitpack.h"

#define WSIZE 64
Except_T Bitpack_Overflow = {"Value doesn't fit in width\n"};

// private bitwise shift functions, to handle undefined behavior in hardware
static inline uint64_t leftshift(uint64_t n, unsigned shift) {
    assert(shift <= WSIZE);
    if (shift == 0) {
        return n;
    }
    if (shift == WSIZE) {
        return 0;
    }
    return n << shift;
}

static inline uint64_t urightshift(uint64_t n, unsigned shift) {
    assert(shift <= WSIZE);
    if (shift == 0) {
        return n;
    }
    if (shift == WSIZE) {
        return 0;
    }
    return n >> shift;
}

static inline int64_t srightshift(uint64_t n, unsigned shift) {
    assert(shift <= WSIZE);
    if (shift == 0) {
        return n;
    }
    if (shift == WSIZE) {
        return -1;
    }
    return (int64_t)n >> shift;
}

// check if a n fits into a word
bool Bitpack_fitsu(uint64_t n, unsigned width) {
    // a number can not fit into a word of 0 width
    if (width == 0) {
        return false;
    }
    uint64_t max_pow = leftshift(1, width - 1);
    return n <= (max_pow | (max_pow - 1));
}

bool Bitpack_fitss(int64_t n, unsigned width) {
    // a number can not fit into a word of 0 width
    if (width == 0) {
        return false;
    }
    int64_t min = srightshift(INT64_MIN, WSIZE - width);
    return n >= min && n <= ~min;
}

// helper function which contains the asserts used in the following 4 functions 
static inline void check(unsigned width, unsigned lsb) {
    assert(width <= WSIZE);
    assert(width + lsb <= WSIZE);
}

// extract a field starting from lsb, with length width, from word 
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
    check(width, lsb);
    uint64_t start = urightshift(word, lsb);
    uint64_t new_word = leftshift(start, WSIZE - width);
    return urightshift(new_word, WSIZE - width);
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) {
    check(width, lsb);
    uint64_t start = urightshift(word, lsb);
    int64_t new_word = leftshift(start, WSIZE - width);
    if (new_word & INT64_MIN) {
        new_word = srightshift(new_word, WSIZE - width);    
    } else {
        new_word = urightshift(new_word, WSIZE - width);
    }
    return new_word;
}

// core part of new algorithm used by both signed and unsigned new functions
static inline uint64_t new_core(uint64_t word, unsigned width,
		                        unsigned lsb, uint64_t value) {
    uint64_t clear_back = lsb + width;
    uint64_t left_bits = urightshift(word, clear_back);
    left_bits = leftshift(left_bits, clear_back);
    
    uint64_t clear_front = WSIZE - lsb;    
    uint64_t right_bits = leftshift(word, clear_front);
    right_bits = urightshift(right_bits, clear_front);
    
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
    return new_core(word, width, lsb, value);
}
