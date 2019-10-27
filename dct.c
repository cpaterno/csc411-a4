#include "dct.h"

// y1
double luma_tl(double a, double b, double c, double d) {
    return a - b - c + d;
}

// y2
double luma_tr(double a, double b, double c, double d) {
    return a - b + c - d;
}

// y3
double luma_bl(double a, double b, double c, double d){
    return a + b - c - d;
}

// y4
double luma_br(double a, double b, double c, double d) {
    return a + b + c + d;
}

// a
double coef_a(double y1, double y2, double y3, double y4) {
    return (y4 + y3 + y2 + y1) / 4.0;
}

// b
double coef_b(double y1, double y2, double y3, double y4) {
    return (y4 + y3 - y2 - y1) / 4.0;
}

// c
double coef_c(double y1, double y2, double y3, double y4) {
    return (y4 - y3 + y2 - y1) / 4.0;
}

// d
double coef_d(double y1, double y2, double y3, double y4) {
    return (y4 - y3 - y2 + y1) / 4.0;
}
