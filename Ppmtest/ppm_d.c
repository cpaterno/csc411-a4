#include <string.h>
#include "assert.h"
#include "mem.h"
#include "ppm_d.h"

// count the number of 2 by 2 blocks in an even dimensioned image
unsigned Pnm_num_blocks(const Pnm_ppm ppm) {
    assert(ppm);
    assert(ppm->width % 2 == 0
           && ppm->height % 2 == 0);
    return (ppm->width * ppm->height) / 4;
}

// return a new ppm which has even dimensions
Pnm_ppm Pnm_trim(const Pnm_ppm ppm) {
    assert(ppm);
    // initialize out with ppm's defaults 
    Pnm_ppm out;
    NEW(out);
    out->width = ppm->width;
    out->height = ppm->height;
    out->denominator = ppm->denominator;
    out->methods = ppm->methods;
    if (ppm->width % 2 != 0) {
    	out->width -= 1;
    }
    if (ppm->height % 2 != 0) {
    	out->height -= 1;
    }
    unsigned size = ppm->methods->size(ppm->pixels);
    out->pixels = out->methods->new(out->width, out->height, size);
    // go through each pixel in ppm, in the range of new width and new height,
    // and copy its value over to out
    void *old_pixel = NULL;
    void *new_pixel = NULL;
    for (unsigned i = 0; i < out->height; ++i) {
    	for (unsigned j = 0; j < out->width; ++j) {
	    old_pixel = ppm->methods->at(ppm->pixels, j, i);
	    new_pixel = out->methods->at(out->pixels, j, i);
	    memcpy(new_pixel, old_pixel, size);
	}
    }
    return out;
}

// helper function which converts an int rgb to float
static float color_to_float(unsigned color, unsigned denom) {
    assert(denom != 0);
    return (float)color / denom;
}

// return a new ppm which has pixels with floating point representation
Pnm_ppm Pnm_floatrep(const Pnm_ppm ppm) {
    assert(ppm);
    // initialize out with ppm's defaults 
    Pnm_ppm out;
    NEW(out);
    out->width = ppm->width;
    out->height = ppm->height;
    out->denominator = ppm->denominator;
    out->methods = ppm->methods;
    out->pixels = out->methods->new(out->width, out->height, sizeof(struct Pnm_rgb_f));
    // go through each pixel in ppm convert its rgb values to float
    // and then copy it over to out 
    Pnm_rgb old_pixel = NULL;
    Pnm_rgb_f new_pixel = NULL;
    for (unsigned i = 0; i < out->height; ++i) {
    	for (unsigned j = 0; j < out->width; ++j) {
	    old_pixel = (Pnm_rgb)ppm->methods->at(ppm->pixels, j, i);
	    new_pixel = (Pnm_rgb_f)out->methods->at(out->pixels, j, i);
	    struct Pnm_rgb_f temp = {
	        color_to_float(old_pixel->red, out->denominator),
	        color_to_float(old_pixel->green, out->denominator),
	        color_to_float(old_pixel->blue, out->denominator)
	    };
	    *new_pixel = temp;
	}
    }
    return out;
}

// helper function which converts an int rgb to float
static unsigned color_to_int(float color, unsigned denom) {
    return color * denom;
}

// return a new ppm which has pixels with int representation
Pnm_ppm Pnm_intrep(const Pnm_ppm ppm) {
    assert(ppm);
    // initialize out with ppm's defaults 
    Pnm_ppm out;
    NEW(out);
    out->width = ppm->width;
    out->height = ppm->height;
    out->denominator = ppm->denominator;
    out->methods = ppm->methods;
    out->pixels = out->methods->new(out->width, out->height, sizeof(struct Pnm_rgb));
    // go through each pixel in ppm convert its rgb values to unsigned
    // and then copy it over to out 
    Pnm_rgb_f old_pixel = NULL;
    Pnm_rgb new_pixel = NULL;
    for (unsigned i = 0; i < out->height; ++i) {
    	for (unsigned j = 0; j < out->width; ++j) {
	    old_pixel = (Pnm_rgb_f)ppm->methods->at(ppm->pixels, j, i);
	    new_pixel = (Pnm_rgb)out->methods->at(out->pixels, j, i);
	    struct Pnm_rgb temp = {
	        color_to_int(old_pixel->red, out->denominator),
	        color_to_int(old_pixel->green, out->denominator),
	        color_to_int(old_pixel->blue, out->denominator)
	    };
	    *new_pixel = temp;
	}
    }
    return out;
}
