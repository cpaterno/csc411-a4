#include "assert.h"
#include "mem.h"
#include "a2plain.h"
#include "ppm_d.h"
#include "ppm_c.h"
#include "dct.h"
#include "color_space.h"
#include "codeword.h"

/*******************************************COMPRESS*****************************************/

Pnm_ppm Pnm_cvrep(const Pnm_ppm ppm) {
    assert(ppm);
    // make float rep of image
    Pnm_ppm out = Pnm_floatrep(ppm);
    // update float values so they represent y, pb, pr
    Pnm_rgb_f pixel = NULL;
    float y, pb, pr;
    y = pb = pr = 0.0f;
    for (unsigned i = 0; i < out->height; ++i) {
        for (unsigned j = 0; j < out->width; ++j) {
	        pixel = (Pnm_rgb_f)out->methods->at(out->pixels, j, i);
	        y = luma(pixel->red, pixel->green, pixel->blue);
	        pb = diff_blue(pixel->red, pixel->green, pixel->blue);
	        pr = diff_red(pixel->red, pixel->green, pixel->blue);
	        pixel->red = y;
	        pixel->green = pb;
	        pixel->blue = pr;
	    }
    }
    return out;
}

void block_update(Pnm_rgb_f *block, unsigned len,
                  const Pnm_ppm img, unsigned c, unsigned r) {
    assert(block && len == 4);
    assert(img);
    assert(c + 1 < img->width && r + 1 < img->height);
    block[0] = (Pnm_rgb_f)img->methods->at(img->pixels, c, r);
    block[1] = (Pnm_rgb_f)img->methods->at(img->pixels, c + 1, r);
    block[2] = (Pnm_rgb_f)img->methods->at(img->pixels, c, r + 1);
    block[3] = (Pnm_rgb_f)img->methods->at(img->pixels, c + 1, r + 1);
}

void block_values(Pnm_rgb_f *block, unsigned len, float *a, float *b,
                  float *c, float *d, float *pb, float *pr) {
    assert(block && len == 4);
    assert(a && b && c && d && pb && pr);
    float y[len];
    for (unsigned i = 0; i < len; ++i) {
        y[i] = block[i]->red;
        *pb += block[i]->green;
        *pr += block[i]->blue;
    }
    *pb /= len;
    *pr /= len;
    *a = coef_a(y[0], y[1], y[2], y[3]);
    *b = coef_b(y[0], y[1], y[2], y[3]);
    *c = coef_c(y[0], y[1], y[2], y[3]);
    *d = coef_d(y[0], y[1], y[2], y[3]);
}

void blocks_to_words(const Pnm_ppm ppm, Array_T words) {
    assert(ppm && words);
    assert(ppm->height % 2 == 0 && ppm->width % 2 == 0);
    unsigned index = 0;
    unsigned width = ppm->width;
    unsigned height = ppm->height;
    float a, b, c, d, avg_pb, avg_pr;
    a = b = c = d = avg_pb = avg_pr = 0.0f;
    unsigned blocksize = 4;
    Pnm_rgb_f block[blocksize];
    codeword *elem = NULL;
    codeword word = 0;
    for (unsigned i = 0; i < height; i += 2) {
        for (unsigned j = 0; j < width; j += 2) {
            block_update(block, blocksize, ppm, j, i);
            block_values(block, blocksize, &a, &b, &c, &d, &avg_pb, &avg_pr);
            word = pack_word(a, b, c, d, avg_pb, avg_pr);
            elem = (codeword *)Array_get(words, index++);
            *elem = word;
        }
    }
}

// reads PPM and writes a compressed image
void compress(FILE *input) {
    assert(input);
    // init image
    A2Methods_T methods = array2_methods_plain;
    assert(methods);
    Pnm_ppm img_orig = Pnm_ppmread(input, methods);
    // make new trimmed image
    Pnm_ppm img_trim = Pnm_trim(img_orig);
    Pnm_ppmfree(&img_orig);
    // convert to cv space
    Pnm_ppm img_cv = Pnm_cvrep(img_trim);
    Pnm_ppmfree(&img_trim);
    unsigned num_blocks = Pnm_num_blocks(img_cv);
    Array_T words = Array_new(num_blocks, sizeof(codeword));
    blocks_to_words(img_cv, words);
    Pnm_comp img_comp = Pnm_comp_new(img_cv->width, img_cv->height, words);
    Pnm_ppmfree(&img_cv);
    Pnm_comp_write(stdout, img_comp);
    Pnm_comp_free(&img_comp);
}

/*******************************************DECOMPRESS***************************************/

float clamp_rgb(float c) {
    if (c < 0.0f) {
        c = 0.0f;
    }
    if (c > 1.0f) {
        c = 1.0f;
    }
    return c;
}

Pnm_ppm Pnm_rgbrep(const Pnm_ppm ppm) {
    assert(ppm);
    // assumed input image is a float rep in cv space
    Pnm_ppm temp;
    NEW(temp);
    temp->width = ppm->width;
    temp->height = ppm->height;
    temp->denominator = ppm->denominator;
    temp->methods = ppm->methods;
    temp->pixels = temp->methods->new(temp->width, temp->height,
		                      sizeof(struct Pnm_rgb_f));
    // update float values so they represent r, g, b
    Pnm_rgb_f old_pixel = NULL;
    Pnm_rgb_f new_pixel = NULL;
    float r, g, b;
    r = g = b = 0.0f;
    for (unsigned i = 0; i < temp->height; ++i) {
        for (unsigned j = 0; j < temp->width; ++j) {
	        old_pixel = (Pnm_rgb_f)ppm->methods->at(ppm->pixels, j, i);
            r = red(old_pixel->red, old_pixel->green, old_pixel->blue);
            g = green(old_pixel->red, old_pixel->green, old_pixel->blue);
            b = blue(old_pixel->red, old_pixel->green, old_pixel->blue);
	        new_pixel = (Pnm_rgb_f)temp->methods->at(temp->pixels, j, i);
	        new_pixel->red = clamp_rgb(r);
	        new_pixel->green = clamp_rgb(g);
	        new_pixel->blue = clamp_rgb(b);
        }
    }
    Pnm_ppm out = Pnm_intrep(temp);
    Pnm_ppmfree(&temp);
    return out;
}

void unblock_values(codeword word, float *a, float *b, float *c,
                    float *d, float *pb, float *pr) {
    assert(a && b && c && d && pb && pr);
    *a = unpack_a(word);
    *b = unpack_b(word);
    *c = unpack_c(word);
    *d = unpack_d(word);
    *pb = unpack_pb(word);
    *pr = unpack_pr(word);
}

void update_float_pixel(Pnm_ppm img, unsigned c, unsigned r, 
                  float y, float pb, float pr) {
    assert(img);
    Pnm_rgb_f pixel = NULL;
    pixel = (Pnm_rgb_f)img->methods->at(img->pixels, c, r);
    pixel->red = y;
    pixel->green = pb;
    pixel->blue = pr;
}

void words_to_blocks(Pnm_ppm img, const Array_T words) {
    assert(img && words);
    unsigned len = Array_length(words);
    codeword *elem = NULL;
    float a, b, c, d, pb, pr;
    a = b = c = d = pb = pr = 0.0f;
    float y[4];
    unsigned col = 0;
    unsigned row = 0;
    unsigned width = img->methods->width(img->pixels);
    for (unsigned i = 0; i < len; ++i) {
        if (col >= width) {
            row += 2;
            col = 0;
        }
        elem = (codeword *)Array_get(words, i);
        unblock_values(*elem, &a, &b, &c, &d, &pb, &pr);
        y[0] = luma_tl(a, b, c, d);
        y[1] = luma_tr(a, b, c, d);
        y[2] = luma_bl(a, b, c, d);
        y[3] = luma_br(a, b, c, d);
        update_float_pixel(img, col, row, y[0], pb, pr);
        update_float_pixel(img, col + 1, row, y[1], pb, pr);
        update_float_pixel(img, col, row + 1, y[2], pb, pr);
        update_float_pixel(img, col + 1, row + 1, y[3], pb, pr);
        col += 2;
    }
}

void bitprint(uint64_t n) {
    for (uint64_t i = (uint64_t)1 << 63; i > 0; i /= 2) {
        (n & i) ? printf("1") : printf("0");
    }
    printf("\n");
}

// reads a compressed image and writes PPM
void decompress(FILE *input) {
    assert(input);
    Pnm_comp img_comp = Pnm_comp_read(input);
    /*for (int i = 0; i < Array_length(img_comp->words); ++i) {
        bitprint(*(codeword *)Array_get(img_comp->words, i));
    }*/
    A2Methods_T methods = array2_methods_plain;
    assert(methods);
    Pnm_ppm img_decomp;
    NEW(img_decomp);
    img_decomp->width = img_comp->width;
    img_decomp->height = img_comp->height;
    img_decomp->denominator = 255;
    img_decomp->methods = methods;
    img_decomp->pixels = methods->new(img_decomp->width, img_decomp->height,
                                      sizeof(struct Pnm_rgb_f));
    words_to_blocks(img_decomp, img_comp->words);
    Pnm_comp_free(&img_comp);
    Pnm_ppm img_rgb = Pnm_rgbrep(img_decomp);
    Pnm_ppmfree(&img_decomp);
    Pnm_ppmwrite(stdout, img_rgb);
    Pnm_ppmfree(&img_rgb);
}
