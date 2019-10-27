#ifndef __DCT__
#define __DCT__

// This module exports 8 functions which represents the discrete cosine transform
// The first four functions undo a discrete cosine transform on 
// the four coefficients 
// The last four functions do a discrete cosine transform on the 4 brightnesses

// Return y1 value
extern double luma_tl(double a, double b, double c, double d);

// Return y2 value
extern double luma_tr(double a, double b, double c, double d);

// Retyurn y3 value
extern double luma_bl(double a, double b, double c, double d);

// Return y4 value
extern double luma_br(double a, double b, double c, double d);

// Return a value
extern double coef_a(double y1, double y2, double y3, double y4);

// Return b value
extern double coef_b(double y1, double y2, double y3, double y4);

// Return c value
extern double coef_c(double y1, double y2, double y3, double y4);

// Return d value
extern double coef_d(double y1, double y2, double y3, double y4);

#endif
