#ifndef __DCT__
#define __DCT__

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
