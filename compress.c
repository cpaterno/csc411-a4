#include "assert.h"
#include "mem.h"
#include "a2plain.h"
#include "ppm_d.h"
#include "ppm_c.h"
#include "dct.h"
#include "color_space.h"
#include "codeword.h"

#define BLOCKSIZE 4
#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOT_LEFT 2
#define BOT_RIGHT 3

/****************************************COMPRESS*********************************/

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
                  const Pnm_ppm img, unsigned r, unsigned c) {
    assert(block && len == BLOCKSIZE);
    assert(img);
    assert(r + 1 < img->width && c + 1 < img->height);
    block[TOP_LEFT] = (Pnm_rgb_f)img->methods->at(img->pixels, c, r);
    block[TOP_RIGHT] = (Pnm_rgb_f)img->methods->at(img->pixels, c + 1, r);
    block[BOT_LEFT] = (Pnm_rgb_f)img->methods->at(img->pixels, c, r + 1);
    block[BOT_RIGHT] = (Pnm_rgb_f)img->methods->at(img->pixels, c + 1, r + 1);
}

void block_values(Pnm_rgb_f *block, unsigned len, float *a, float *b,
                  float *c, float *d, float *pb, float *pr) {
    assert(block && len == BLOCKSIZE);
    assert(a && b && c && d && pb && pr);
    float y[BLOCKSIZE];
    for (int i = 0; i < BLOCKSIZE; ++i) {
        y[i] = block[i]->red;
        *pb += block[i]->green;
        *pr += block[i]->blue;
    }
    *pb /= BLOCKSIZE;
    *pr /= BLOCKSIZE;
    *a = coef_a(y[0], y[1], y[2], y[3]);
    *b = coef_b(y[0], y[1], y[2], y[3]);
    *c = coef_c(y[0], y[1], y[2], y[3]);
    *d = coef_d(y[0], y[1], y[2], y[3]);
}

void blocks_to_words(const Pnm_ppm ppm, Array_T words) {
    assert(ppm && words);
    assert(ppm->height % 2 == 0 && ppm->width % 2 == 0);
    unsigned index = 0;
    unsigned half_width = ppm->width / 2;
    unsigned half_height = ppm->height / 2;
    float a, b, c, d, avg_pb, avg_pr;
    a = b = c = d = avg_pb = avg_pr = 0.0f;
    Pnm_rgb_f block[4];
    codeword *elem = NULL;
    codeword word = 0;
    for (unsigned i = 0; i < half_height; i += BLOCKSIZE / 2) {
        for (unsigned j = 0; j < half_width; j += BLOCKSIZE / 2) {
            block_update(block, BLOCKSIZE, ppm, i, j);
            block_values(block, BLOCKSIZE, &a, &b, &c, &d, &avg_pb, &avg_pr);
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

/**************************************DECOMPRESS*********************************/

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
    r = g = b = 0;
    for (unsigned i = 0; i < temp->height; ++i) {
        for (unsigned j = 0; j < temp->width; ++j) {
	        old_pixel = (Pnm_rgb_f)ppm->methods->at(ppm->pixels, j, i);
            r = red(old_pixel->red, old_pixel->green, old_pixel->blue);
            g = green(old_pixel->red, old_pixel->green, old_pixel->blue);
            b = blue(old_pixel->red, old_pixel->green, old_pixel->blue);
	        new_pixel = (Pnm_rgb_f)temp->methods->at(temp->pixels, j, i);
	        new_pixel->red = r;
	        new_pixel->green = g;
	        new_pixel->blue = b;
        }
    }
    Pnm_ppm out = Pnm_intrep(temp);
    Pnm_ppmfree(&temp);
    return out;
}

void words_to_blocks(Pnm_ppm img, const Array_T words) {
    assert(img && words);
    unsigned len = Array_length(words);
    codeword *elem = NULL;
    codeword word = 0;
    float a, b, c, d, pb, pr;
    a = b = c = d = pb = pr = 0.0f;
    float y[BLOCKSIZE];
    unsigned col = 0;
    unsigned row = 0;
    Pnm_rgb_f pixel = NULL;
    for (unsigned i = 0; i < len; ++i) {
        elem = (codeword *)Array_get(words, i);
        word = *elem;
        a = unpack_a(word);
        b = unpack_b(word);
        c = unpack_c(word);
        d = unpack_d(word);
        pb = unpack_pb(word);
        pr = unpack_pr(word);
        y[0] = luma_tl(a, b, c, d);
        y[1] = luma_tr(a, b, c, d);
        y[2] = luma_bl(a, b, c, d);
        y[3] = luma_br(a, b, c, d);
        pixel = (Pnm_rgb_f)img->methods->at(img->pixels, col, row);
        pixel->red = y[0];
        pixel->green = pb;
        pixel->blue = pr;
        pixel = (Pnm_rgb_f)img->methods->at(img->pixels, col + 1, row);
        pixel->red = y[1];
        pixel->green = pb;
        pixel->blue = pr;
        pixel = (Pnm_rgb_f)img->methods->at(img->pixels, col, row + 1);
        pixel->red = y[2];
        pixel->green = pb;
        pixel->blue = pr;
        pixel = (Pnm_rgb_f)img->methods->at(img->pixels, col + 1, row + 1);
        pixel->red = y[3];
        pixel->green = pb;
        pixel->blue = pr;
        col += BLOCKSIZE / 2;
        row += BLOCKSIZE / 2;
    }
}

// reads a compressed image and writes PPM
void decompress(FILE *input) {
    assert(input);
    Pnm_comp img_comp = Pnm_comp_read(input);
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
    // skip step 3
    Pnm_comp_free(&img_comp);
    Pnm_ppm img_rgb = Pnm_rgbrep(img_decomp);
    Pnm_ppmfree(&img_decomp);
    Pnm_ppmwrite(stdout, img_rgb);
    Pnm_ppmfree(&img_rgb);
}
