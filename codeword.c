#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "assert.h"
#include "codeword.h"

#define INTERVAL_MIN_I -15
#define INTERVAL_MAX_I 15
#define INTERVAL_MIN_F -0.3
#define INTERVAL_MAX_F 0.3
#define INTERVAL_RNG 31

#include <stdio.h>

static bool IS_INIT = false;
static int INTERVAL_F[INTERVAL_RNG];
static int INTERVAL_I[INTERVAL_RNG];

// encode a into a 9b unsigned scaled int
unsigned code_a(double a) {
    return (unsigned)round(511 * a); 
}

// fill set with evenly spaced values, integers
// basically implementing mathlab's linspace
static void init_set(int s[], int x1, int x2, int n) {
    assert(n > 1);
    assert(x2 > x1);
    int step = (x2 - x1) / (n - 1);
    for (int i = 0; i < n; ++i) {
        s[i] = x1 + (i * step);
    }
}

// TODO implement own version of pow function
static int int_of_sig_figs(double d) {
    int exp = 0;
    frexp(d, &exp);
    exp = abs(exp);
    // make sure exp is less than word size
    assert(exp < 64);
    int num_dec_digits = ((uint64_t)1 << exp);
    int multiplier = pow(10, num_dec_digits);
    return d * multiplier;
}

// encode d into a 5b scaled int
int code_bcd(double d) {
    int new_max = int_of_sig_figs(INTERVAL_MAX_F);
    if (!IS_INIT) {
        init_set(INTERVAL_I, INTERVAL_MIN_I, INTERVAL_MAX_I, INTERVAL_RNG);
        int new_min = int_of_sig_figs(INTERVAL_MIN_F);
	// guard against overflow
	assert(2 * new_max > 0);
	init_set(INTERVAL_F, new_min + new_max, 2 * new_max, INTERVAL_RNG);
        IS_INIT = true;
    }
    int code = 0;
    int transformed_d = int_of_sig_figs(d) + new_max;
    printf("d: %d fmin %d fmax %d imin %d imax %d\n", transformed_d,
           INTERVAL_F[0], INTERVAL_F[INTERVAL_RNG - 1],
	   INTERVAL_I[0], INTERVAL_I[INTERVAL_RNG - 1]);
    if (transformed_d < INTERVAL_F[0]) {
    	code = INTERVAL_I[0];
    } else if (transformed_d >= INTERVAL_F[INTERVAL_RNG - 1]) {
        code = INTERVAL_I[INTERVAL_RNG - 1];
    } else {
	int dist_l = 0;
	int dist_r = 0;
	// go to almost last element    
        for (int i = 0; i < INTERVAL_RNG - 1; ++i) {
	    if (transformed_d == INTERVAL_F[i]) {
	        code = INTERVAL_I[i];
		break;
	    } else if (transformed_d > INTERVAL_F[i] 
		       && transformed_d < INTERVAL_F[i + 1]) {
	        dist_l = abs(transformed_d - INTERVAL_F[i]);
		dist_r = abs(transformed_d - INTERVAL_F[i + 1]);
		if (dist_l > dist_r) {
		    code = INTERVAL_I[i];
		} else {
		    // bias towards greater number in even case
		    code = INTERVAL_I[i + 1];
		}
		break;
	    }
	} 
    }
    return code;
}
