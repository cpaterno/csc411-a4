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
    uint64_t max_pow = leftshift(1, width - 1);
    return n <= (max_pow | (max_pow - 1));
}

bool Bitpack_fitss(int64_t n, unsigned width) {
    uint64_t max_pow = leftshift(1, width - 1);
    return n + max_pow <= (max_pow | (max_pow - 1));
}

// helper function which contains the asserts used in the following 4 functions 
static inline void check(unsigned width, unsigned lsb) {
    assert(width <= WSIZE);
    assert(width + lsb <= WSIZE);
}

// extract bits starting from lsb, with bitlength width, from word 
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
    check(width, lsb);
    uint64_t max_pow = leftshift(1, width - 1);
    uint64_t mask = max_pow | (max_pow - 1);
    mask = leftshift(mask, lsb);
    uint64_t new_word = mask & word;
    return rightshift(new_word, lsb);
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) {
    return Bitpack_getu(word, width, lsb);
}

// insert value at lsb, with bitlength width, to word
uint64_t Bitpack_newu(uint64_t word, unsigned width, 
		      unsigned lsb, uint64_t value) {
    check(width, lsb);
    if (!Bitpack_fitsu(value, width)) {
	RAISE(Bitpack_Overflow);    
    }
    uint64_t clear_back = lsb + width;
    uint64_t clear_front = WSIZE - lsb;
    uint64_t temp = rightshift(word, clear_back);
    uint64_t left_bits = leftshift(temp, clear_back);
    temp = leftshift(word, clear_front);
    uint64_t right_bits = rightshift(temp, clear_front);
    uint64_t shift_in = leftshift(value, lsb);
    return left_bits | shift_in | right_bits;
}

uint64_t Bitpack_news(uint64_t word, unsigned width, 
		      unsigned lsb, int64_t value) {
    check(width, lsb);
    if (!Bitpack_fitss(value, width)) {
	RAISE(Bitpack_Overflow);    
    }
    uint64_t clear_back = lsb + width;
    uint64_t clear_front = WSIZE - lsb;
    uint64_t temp = rightshift(word, clear_back);
    uint64_t left_bits = leftshift(temp, clear_back);
    temp = leftshift(word, clear_front);
    uint64_t right_bits = rightshift(temp, clear_front);
    uint64_t shift_in = leftshift(value, lsb);
    return left_bits | shift_in | right_bits;
}

