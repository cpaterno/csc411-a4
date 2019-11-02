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
    // figure out number of codewords, but jumping to the end of the file
    // then substracting the current position - 1 from position of eof character
    long cur_pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long eof_pos = ftell(fp);
    long num_words = (eof_pos - cur_pos) / sizeof(codeword);
    Array_T words = Array_new(num_words, sizeof(codeword));
    // move back to where we left off
    fseek(fp, cur_pos, SEEK_SET);
    // read codeword into element as 1 byte chunks
    codeword *element = NULL;
    for (int i = 0; i < Array_length(words); ++i) {
        element = (codeword *)Array_get(words, i);
	assert(sizeof(*element) == Array_size(words));
	read = fread((void *)element, sizeof(char), sizeof(*element), fp);
	assert(read == sizeof(*element));
    }
    // read EOF character
    getc(fp);
    // assert that everything was read
    assert(feof(fp));
    return Pnm_comp_new(width, height, words);
}

// write a compressed image to a file
void Pnm_comp_write(FILE *fp, const Pnm_comp pnm) {
    assert(fp && pnm);
    // write header
    fprintf(fp, "Compressed image format 2\n%u %u\n", pnm->width, pnm->height);
    codeword *element = NULL;
    // write out codeword in 1 byte chunks
    for (int i = 0; i < Array_length(pnm->words); ++i) {
	element = (codeword *)Array_get(pnm->words, i);
	assert(sizeof(*element) == Array_size(pnm->words));
	fwrite((const void *)element, sizeof(char), sizeof(*element), fp);
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
