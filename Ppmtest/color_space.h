#ifndef __COLOR_SPACE__
#define __COLOR_SPACE__

// This module exports 6 functions representing a color space converter
// The first three functions compute the component video space given  r g b
// The last three functions compute the rgb space given y pb pr

// return y
extern float luma(float r, float g, float b);

// return pb
extern float diff_blue(float r, float g, float b);

// return pr
extern float diff_red(float r, float g, float b);

// return red
extern float red(float y, float pb, float pr);

// return green
extern float green(float y, float pb, float pr);

// return blue
extern float blue(float y, float pb, float pr);

#endif
