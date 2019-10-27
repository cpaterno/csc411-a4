#ifndef __COLOR_SPACE__
#define __COLOR_SPACE__

// This module exports 6 functions representing a color space converter
// The first three functions compute the component video space given  r g b
// The last three functions compute the rgb space given y pb pr

// return y
extern double luma(double r, double g, double b);

// return pb
extern double diff_blue(double r, double g, double b);

// return pr
extern double diff_red(double r, double g, double b);

// return red
extern double red(double y, double pb, double pr);

// return green
extern double green(double y, double pb, double pr);

// return blue
extern double blue(double y, double pb, double pr);

#endif
