#include "codeword.h"
#include <stdio.h>

int main() {
    double b = -0.25;
    double c = -0.5;
    double d = 0.5;
    printf("b %f c %f d %f\n", b, c, d);
    int b_code = code_bcd(b);
    int c_code = code_bcd(c);
    int d_code = code_bcd(d);
    printf("b %d, c %d, d %d\n", b_code, c_code, d_code);
    return 0;
}
