#include <stdbool.h>
#include "mem.h"
#include "assert.h"
#include "codeword.h"
#include "ppm_c.h"

#define BITS_IN_BYTE 8

// read a compressed image from a file
Pnm_comp Pnm_comp_read(FILE *fp) {
    unsigned height, width;
    int read = fscanf(fp, "Compressed image format 2\n%u %u", &width, &height);
    assert(read == 2);
    int c = getc(fp);
    assert(c == '\n');

    Array_T words = Array_new(width * height, sizeof(codeword));

    codeword current;
    codeword *element = NULL;
    bool isDone = false;
    unsigned i;
    for (i = 0; i < width * height && !isDone; ++i) {
        current = 0;
        for (long j = sizeof(codeword) - 1; j >= 0; --j) {
            c = getc(fp);
            if (c == EOF) {
                isDone = true;
                break;
            }
            current |= c << (j * BITS_IN_BYTE);
        }
        element = (codeword *)Array_get(words, i);
        *element = current;
    }
    assert(i == width * height);
    Pnm_comp img;
    NEW(img);
    img->width = width;
    img->height = height;
    img->words = words;
    return img;
}

// write a compressed image to a file
void Pnm_comp_write(FILE *fp, Pnm_comp pnm) {
    (void)fp;
    (void)pnm;
}

// free a compressed image, note it is assumed Pnm_comp is made from heap allocated memory
void Pnm_comp_free(Pnm_comp *ppmp) {
    assert(ppmp && *ppmp);
    Array_free(&(*ppmp)->words);
    FREE(*ppmp);
}
