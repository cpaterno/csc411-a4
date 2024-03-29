#include <math.h>
#include "assert.h"
#include "bitpack.h"
#include "arith411.h"
#include "codeword.h"

#define WIDTH_A 6
#define WIDTH_B 6
#define WIDTH_C WIDTH_B
#define WIDTH_D WIDTH_B
#define WIDTH_IDX 4

#define LSB_A 26
#define LSB_B 20
#define LSB_C 14
#define LSB_D 8
#define LSB_PB 4
#define LSB_PR 0

#define MAX_A 63

#define MAX_B_I 31
#define MAX_B_F 0.3f
#define MAX_C_I MAX_B_I
#define MAX_C_F MAX_B_F
#define MAX_D_I MAX_B_I
#define MAX_D_F MAX_B_F

/*----------------------------------------ENCODE--------------------------------------------*/

// encode bcd
static int encode_bcd(float f, int max_int, float max_float) {
    // clamp f to be in the range of [-max_float, +max_float]
    if (f > max_float) {
        return max_float;
    }
    if (f < -max_float) {
        return -max_float;
    }
    // multiply f by scale factor
    return round(f * ((float)max_int / max_float));  
}

// encode a
static unsigned encode_a(float a) {
    return (unsigned)round(a * MAX_A);
}

// pack 4 coded values into a codeword
codeword pack_word(float a, float b, float c, 
		           float d, float pb, float pr) {
    // encode values
    unsigned code_a = encode_a(a);
    int code_b = encode_bcd(b, MAX_B_I, MAX_B_F);
    int code_c = encode_bcd(c, MAX_C_I, MAX_C_F);
    int code_d = encode_bcd(d, MAX_D_I, MAX_D_F);
    unsigned idx_pb = Arith_index_of_chroma(pb);
    unsigned idx_pr = Arith_index_of_chroma(pr);
    codeword word = 0;
    // pack each value   
    word = Bitpack_newu(word, WIDTH_A, LSB_A, code_a);
    word = Bitpack_news(word, WIDTH_B, LSB_B, code_b);
    word = Bitpack_news(word, WIDTH_C, LSB_C, code_c);
    word = Bitpack_news(word, WIDTH_D, LSB_D, code_d);
    word = Bitpack_newu(word, WIDTH_IDX, LSB_PB, idx_pb);
    word = Bitpack_newu(word, WIDTH_IDX, LSB_PR, idx_pr);
    return word;
}

/*----------------------------------------DECODE--------------------------------------------*/

// decode a
static float decode_a(unsigned a) {
    return (float)a / MAX_A;
}

// unpack a from a codeword
float unpack_a(codeword word) {
    uint64_t code_a = Bitpack_getu(word, WIDTH_A, LSB_A);
    float a = decode_a(code_a); 
    return a;
}

// decode bcd
static float decode_bcd(int code, int max_int, float max_float) {
    // multiply code by inverse scale factor
    return code * (max_float / (float)max_int);  
}

// unpack b from a codeword
float unpack_b(codeword word) {
    int64_t code_b = Bitpack_gets(word, WIDTH_B, LSB_B);
    float b = decode_bcd(code_b, MAX_B_I, MAX_B_F);
    return b;
}

// unpack c from a codeword
float unpack_c(codeword word) {
    int64_t code_c = Bitpack_gets(word, WIDTH_C, LSB_C);
    float c = decode_bcd(code_c, MAX_C_I, MAX_C_F); 
    return c;
}

// unpack d from a codeword
float unpack_d(codeword word) {
    int64_t code_d = Bitpack_gets(word, WIDTH_D, LSB_D);
    float d = decode_bcd(code_d, MAX_D_I, MAX_D_F); 
    return d;
}

// unpack pb from a codeword
float unpack_pb(codeword word) {
    uint64_t idx_pb = Bitpack_getu(word, WIDTH_IDX, LSB_PB);
    float pb = Arith_chroma_of_index(idx_pb); 
    return pb;
}

// unpack pr from a codeword
float unpack_pr(codeword word) {
    uint64_t idx_pr = Bitpack_getu(word, WIDTH_IDX, LSB_PR);
    float pr = Arith_chroma_of_index(idx_pr);
    return pr;
}
