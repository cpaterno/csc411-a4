#include <math.h>
#include <string.h>
#include "assert.h"
#include "pnm.h"
#include "a2plain.h"

double rmsd(const Pnm_ppm, const Pnm_ppm);

int main(int argc, char *argv[]) {
    assert(argc == 3);
    FILE *images[2];
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-") == 0) {
            images[i - 1] = stdin;
        } else {
            images[i - 1] = fopen(argv[i], "rb");
	    assert(images[i - 1]);
        }
    }
    assert((images[0] != stdin && images[1] != stdin)
            || (images[0] == stdin && images[1] != stdin)
            || (images[0] != stdin && images[1] == stdin));

    A2Methods_T methods = array2_methods_plain;
    assert(methods);
    Pnm_ppm i = Pnm_ppmread(images[0], methods);
    Pnm_ppm i_prime = Pnm_ppmread(images[1], methods);
    
    double e = rmsd(i, i_prime);
    printf("%.4f\n", e);
    
    Pnm_ppmfree(&i);
    Pnm_ppmfree(&i_prime);
    if (images[0] != stdin) {
        fclose(images[0]);
    }
    if (images[1] != stdin) {
        fclose(images[1]);
    }
}

double rmsd(const Pnm_ppm i, const Pnm_ppm i_prime) {
    double e = 1.0;
    int w = 0;
    int h = 0;
    int diff_w = i->width - i_prime->width;
    if (diff_w < 0) {
        w = i->width;
    } else {
        w = i_prime->width;
    }
    int diff_h = i->height - i_prime->height;
    if (diff_h < 0) {
        h = i->height;
    } else {
        h = i_prime->height;
    }
    if (fabs(diff_w) > 1 || fabs(diff_h) > 1) {
        fprintf(stderr, "Difference in dimensions is too large\n");
    } else {
        int numer, denom, diff_r, diff_g, diff_b;
        denom = 3 * w * h;
        numer = diff_r = diff_g = diff_b = 0;
        Pnm_rgb i_pixel;
        Pnm_rgb i_prime_pixel;
        for (int k= 0; k < h; ++k) {
            for (int j = 0; j < w; ++j) {
                i_pixel = (Pnm_rgb)i->methods->at(i->pixels, j, k);
                i_prime_pixel = (Pnm_rgb)i_prime->methods->at(i_prime->pixels,
                                                               j, k);
                diff_r = i_pixel->red - i_prime_pixel->red;
                diff_g = i_pixel->green - i_prime_pixel->green;
                diff_b = i_pixel->blue - i_prime_pixel->blue;
                numer += diff_r * diff_r + diff_g * diff_g + diff_b * diff_b;
            }
        }
        e = sqrt(numer / denom);
    }
    return e;
}
