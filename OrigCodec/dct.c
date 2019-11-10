#include "dct.h"

// y1
float luma_tl(float a, float b, float c, float d) {
    return a - b - c + d;
}

// y2
float luma_tr(float a, float b, float c, float d) {
    return a - b + c - d;
}

// y3
float luma_bl(float a, float b, float c, float d){
    return a + b - c - d;
}

// y4
float luma_br(float a, float b, float c, float d) {
    return a + b + c + d;
}

// a
float coef_a(float y1, float y2, float y3, float y4) {
    return (y4 + y3 + y2 + y1) / 4.0f;
}

// b
float coef_b(float y1, float y2, float y3, float y4) {
    return (y4 + y3 - y2 - y1) / 4.0f;
}

// c
float coef_c(float y1, float y2, float y3, float y4) {
    return (y4 - y3 + y2 - y1) / 4.0f;
}

// d
float coef_d(float y1, float y2, float y3, float y4) {
    return (y4 - y3 - y2 + y1) / 4.0f;
}
