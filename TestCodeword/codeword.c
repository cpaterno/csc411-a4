#include <math.h>
#include <stdbool.h>
#include "assert.h"
#include "bitpack.h"
#include "arith411.h"
#include "codeword.h"

#define WIDTH_A 9
#define WIDTH_B 5
#define WIDTH_IDX 4

#define LSB_A 23
#define LSB_B 18
#define LSB_C 13
#define LSB_D 8
#define LSB_PB 4
#define LSB_PR 0

#define UMAX_9b 511

#define INTERVAL_MIN_I -15
#define INTERVAL_MAX_I 15
#define INTERVAL_MIN_F -0.3f
#define INTERVAL_MAX_F 0.3f
#define INTERVAL_RNG 31

#include <stdio.h> // testing only

static bool IS_INIT = false;
static float INTERVAL_F[INTERVAL_RNG];
static float INTERVAL_I[INTERVAL_RNG];

// fill set with evenly spaced values
// basically implementing mathlab's linspace
static void linspace(float s[], float x1, float x2, int n) {
    assert(n > 1);
    assert(x2 > x1);
    float step = (x2 - x1) / (n - 1);
    for (int i = 0; i < n; ++i) {
        s[i] = x1 + (i * step);
    }
}

// simple function to init_sets if they are not created already
static void init_sets() {
    if (!IS_INIT) {
        linspace(INTERVAL_I, INTERVAL_MIN_I, INTERVAL_MAX_I, INTERVAL_RNG);
	linspace(INTERVAL_F, INTERVAL_MIN_F, INTERVAL_MAX_F, INTERVAL_RNG);
	IS_INIT = true;
    }
}

/*----------------------------------------ENCODE--------------------------------------------*/

// encode b, c, or d from f 
static int encode_bcd(float f) {
    // init value sets if they are not already created
    init_sets(); 
    int code = 0;
    // handle edges and the last element of the set
    if (f < INTERVAL_F[0]) {
        code = (int)INTERVAL_I[0];
    } else if (f >= INTERVAL_F[INTERVAL_RNG - 1]) {
        code = INTERVAL_I[INTERVAL_RNG - 1];
    // value is in the range of the set
    } else {
        float dist_l = 0.0f;
	float dist_r = 0.0f;
	// epsilon will be a 10th of the difference
	// between values in the set
	float epsilon = (INTERVAL_MAX_F - INTERVAL_MIN_F) 
		         / (INTERVAL_RNG - 1) / 10.0f;
	// go to almost last element
	for (int i = 0; i < INTERVAL_RNG - 1; ++i) {
	    // "equal" case
	    if (fabs(f - INTERVAL_F[i]) < epsilon) {
	        code = (int)INTERVAL_I[i];
		break;
	    } else if (f > INTERVAL_F[i]
		       && f < INTERVAL_F[i + 1]) {
	        dist_l = fabs(f - INTERVAL_F[i]);
		dist_r = fabs(f - INTERVAL_F[i + 1]);
		// bias towards greater number in even case
		if (dist_l > dist_r) {
		    code = (int)INTERVAL_I[i];
		} else {
		    code = (int)INTERVAL_I[i + 1];
		}
		break;
	    }	    
	}
    }
    return code;
}

// encode a
static unsigned encode_a(float a) {
    return (unsigned)round(a * UMAX_9b);
}

// pack 4 coded values into a codeword
codeword pack_word(float a, float b, float c, 
		   float d, float pb, float pr) {
    printf("a %f, b %f, c %f, d %f, pb %f, pr %f\n",
	   a, b, c, d, pb, pr);	    
    unsigned code_a = encode_a(a);
    int code_b = encode_bcd(b);
    int code_c = encode_bcd(c);
    int code_d = encode_bcd(d);
    int idx_pb = Arith_index_of_chroma(pb);
    int idx_pr = Arith_index_of_chroma(pr);
    printf("a %d, b %d, c %d, d %d, pb %d, pr %d\n",
	   code_a, code_b, code_c, code_d, idx_pb, idx_pr);	    
    codeword word = 0;
    (void)code_a; // codeword word = Bitpack_newu(word, WIDTH_A, LSB_A);
    (void)code_b; // codeword word = Bitpack_news(word, WIDTH_B, LSB_B);
    (void)code_c; // codeword word = Bitpack_news(word, WIDTH_B, LSB_C);
    (void)code_d; // codeword word = Bitpack_news(word, WIDTH_B, LSB_D);
    (void)idx_pb; // codeword word = Bitpack_news(word, WIDTH_IDX, LSB_PB);
    (void)idx_pr; // codeword word = Bitpack_news(word, WIDTH_IDX, LSB_PR);
    return word;
}

/*----------------------------------------DECODE--------------------------------------------*/

// decode a
static float decode_a(unsigned a) {
    return (float)a * UMAX_9b;
}

// unpack a from a codeword
float unpack_a(codeword word) {
    uint64_t code_a = word; // Bitpack_getu(word, WIDTH_A, LSB_A);
    float a = decode_a(code_a); 
    return a;
}

static float decode_bcd(int code) {
    // init value sets if they are not already created
    init_sets(); 
    float f = 0.0f;
    // epsilon will be a 10th of the difference
    // between values in the set
    float epsilon = (INTERVAL_MAX_I - INTERVAL_MIN_I) 
		     / (INTERVAL_RNG - 1) / 10.0f;
    // go through "int" interval and grab closest value
    for (int i = 0; i < INTERVAL_RNG; ++i) {
	// "equal" case
	if (fabs((float)code - INTERVAL_I[i]) < epsilon) {
	    f = INTERVAL_F[i];
	    break;
	} 
    }
    return f;
}

// unpack b from a codeword
float unpack_b(codeword word) {
    uint64_t code_b = word; // Bitpack_gets(word, WIDTH_B, LSB_B);
    float b = decode_bcd(code_b); 
    return b;
}

// unpack c from a codeword
float unpack_c(codeword word) {
    uint64_t code_c = word; // Bitpack_gets(word, WIDTH_C, LSB_C);
    float c = decode_bcd(code_c); 
    return c;
}

// unpack d from a codeword
float unpack_d(codeword word) {
    uint64_t code_d = word; // Bitpack_gets(word, WIDTH_D, LSB_D);
    float d = decode_bcd(code_d); 
    return d;
}

// unpack pb from a codeword
float unpack_pb(codeword word) {
    uint64_t idx_pb = word; // Bitpack_gets(word, WIDTH_IDX, LSB_PB);
    float pb = Arith_chroma_of_index(idx_pb); 
    return pb;
}

// unpack pr from a codeword
float unpack_pr(codeword word) {
    uint64_t idx_pr = word; // Bitpack_gets(word, WIDTH_IDX, LSB_PR);
    float pr = Arith_chroma_of_index(idx_pr); 
    return pr;
}
