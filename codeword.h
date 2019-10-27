#ifndef __CODEWORD__
#define __CODEWORD__

// Module exports 2 functions and a macro
// The macro defines the codeword which is used by 2 other modules

#include <stdint.h>

#define codeword uint32_t

// encode a into a 9b unsigned scaled int
extern unsigned code_a(double a);
// encode d into a 5b scaled int
extern int code_bcd(double d);

#endif
