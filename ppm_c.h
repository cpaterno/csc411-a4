#ifndef __PPM_C__
#define __PPM_C__

#include <stdio.h>
#include "array.h"

// Module representing a compressed ppm
// This module mimics the style of pnm.h and ppm_d.h
// It exports 3 functions and one data structure

// Represents a compressed image, similar to how Pnm_ppm represents an uncompressed image
// Width and height represent the width and height of the original uncompressed image
// Words is a 1D array of codewords
// exported as pointer to follow pnm interface's format
typedef struct Pnm_comp {
    unsigned width, height;
    Array_T words;
} *Pnm_comp;

// read a compressed image from a file
extern Pnm_comp Pnm_comp_read(FILE *fp);

// write a compressed image to a file
extern void Pnm_comp_write(FILE *fp, Pnm_comp pnm);

// free a compressed image, note it is assumed Pnm_comp is made from heap allocated memory
extern void Pnm_comp_free(Pnm_comp *ppmp);

#endif 
