#ifndef __DCT__
#define __DCT__

// This module exports 8 functions which represents the discrete cosine transform
// The first four functions undo a discrete cosine transform on 
// the four coefficients 
// The last four functions do a discrete cosine transform on the 4 brightnesses

// Return y1 value
extern float luma_tl(float a, float b, float c, float d);

// Return y2 value
extern float luma_tr(float a, float b, float c, float d);

// Retyurn y3 value
extern float luma_bl(float a, float b, float c, float d);

// Return y4 value
extern float luma_br(float a, float b, float c, float d);

// Return a value
extern float coef_a(float y1, float y2, float y3, float y4);

// Return b value
extern float coef_b(float y1, float y2, float y3, float y4);

// Return c value
extern float coef_c(float y1, float y2, float y3, float y4);

// Return d value
extern float coef_d(float y1, float y2, float y3, float y4);

#endif
