#ifndef __CODEWORD__
#define __CODEWORD__

// Module exports 7 functions and a macro, and represents all manipulations that
// can be done to a codeword
// The macro defines the codeword which is used by other modules

#include <stdint.h>

#define codeword uint32_t 

// pack 4 coded values into a codeword
extern codeword pack_word(float a, float b, float c, 
		          float d, float pb, float pr);

// unpack a from a codeword
extern float unpack_a(codeword word);
// unpack b from a codeword
extern float unpack_b(codeword word);
// unpack c from a codeword
extern float unpack_c(codeword word);
// unpack d from a codeword
extern float unpack_d(codeword word);

// unpack pb from a codeword
extern float unpack_pb(codeword word);
// unpack pr from a codeword
extern float unpack_pr(codeword word);
#endif
