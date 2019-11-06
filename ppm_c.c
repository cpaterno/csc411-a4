#include <string.h>
#include "mem.h"
#include "assert.h"
#include "codeword.h"
#include "ppm_c.h"

// create a new Pnm_comp on the heap
Pnm_comp Pnm_comp_new(unsigned width, unsigned height, Array_T words) {
    assert(width > 0 && height > 0);
    assert(words);
    // create and init the struct
    Pnm_comp img;
    NEW(img);
    img->width = width;
    img->height = height;
    img->words = words;
    return img;
}

// read a compressed image from a file
Pnm_comp Pnm_comp_read(FILE *fp) {
    assert(fp);
    // read header
    unsigned height, width;
    int read = fscanf(fp, "Compressed image format 2\n%u %u", &width, &height);
    assert(read == 2);
    int c = getc(fp);
    assert(c == '\n');
    Array_T words = Array_new(0, sizeof(codeword));
    int size = Array_size(words);
    int len = Array_length(words);
    unsigned char buf[size];
    codeword *element = NULL;
    int i = 0;
    int j = 0;
    while (!feof(fp)) {
        buf[j++] = getc(fp);
        if (j == size) {
            j = 0;
            if (i == len) {
                if (i != 0) {
                    len *= 2;
                } else {
                    len = 1;
                }
                Array_resize(words, len);
            }
            element = (codeword *)Array_get(words, i++);
            assert(sizeof(*element) == size);
            memcpy(element, buf, size);
        }
    }
    Array_resize(words, i);
    return Pnm_comp_new(width, height, words);
}

// write a compressed image to a file
void Pnm_comp_write(FILE *fp, const Pnm_comp pnm) {
    assert(fp && pnm);
    // write header
    fprintf(fp, "Compressed image format 2\n%u %u\n", pnm->width, pnm->height);
    codeword *element = NULL;
    int size = Array_size(pnm->words);
    unsigned char buf[size];
    // write out codeword in 1 byte chunks
    for (int i = 0; i < Array_length(pnm->words); ++i) {
	    element = (codeword *)Array_get(pnm->words, i);
	    assert(sizeof(*element) == Array_size(pnm->words));
        memcpy(buf, element, size);
        for (int j = 0; j < size; ++j) {
            putc(buf[j], fp);
        }
    }
}

// free a compressed image, note it is assumed Pnm_comp is made from heap allocated memory
void Pnm_comp_free(Pnm_comp *ppmp) {
    assert(ppmp && *ppmp);
    // free the words array
    Array_free(&(*ppmp)->words);
    // free the Pnm_comp
    FREE(*ppmp);
}
