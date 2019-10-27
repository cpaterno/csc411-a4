#ifndef __PPM_D_
#define __PPM_D__

#include "pnm.h"

// Module representing the decompressed ppm
// This module builds off of pnm.h and exports 4 functions and one data structure

// double version of Pnm_rgb used for float representation of the image's pixels
// exported as pointer to follow pnm interface's format
// if used for cv space, y = red, pb = green, pr = blue
typedef struct Pnm_rgb_f {
    double red, green, blue;
} *Pnm_rgb_f; 

// count the number of 2 by 2 blocks in an even dimensioned image
extern unsigned Pnm_num_blocks(const Pnm_ppm ppm);

// return a new ppm which has even dimensions
Pnm_ppm Pnm_trim(const Pnm_ppm ppm);

// return a new ppm which has pixels with floating point representation
Pnm_ppm Pnm_floatrep(const Pnm_ppm ppm);

// return a new ppm which has pixels with int representation
Pnm_ppm Pnm_intrep(const Pnm_ppm ppm);

#endif
