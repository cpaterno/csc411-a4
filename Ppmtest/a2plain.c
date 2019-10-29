#include <stdlib.h>

#include <a2plain.h>
#include "uarray2.h"

// define a private version of each function in A2Methods_T that we implement

// private abbreviation
typedef A2Methods_Array2 A2;

static A2 new(int width, int height, int size) {
    return UArray2_new(width, height, size); 
}

static A2 new_with_blocksize(int width, int height, int size, int blocksize) {
    (void) blocksize;
    return UArray2_new(width, height, size);
}

static void a2free(A2 *array2p) {
    UArray2_free((UArray2_T *)array2p);
}

static int width(A2 array2) {
    return UArray2_width(array2);
}

static int height(A2 array2) {
    return UArray2_height(array2);
}   

static int size(A2 array2) {
    return UArray2_size(array2);
}

static int blocksize(A2 array2) {
    (void)array2;
    return 1;
}

static A2Methods_Object *at(A2 array2, int i, int j) {
    return UArray2_at(array2, i, j);
}

typedef void applyfun(int i, int j, UArray2_T array2, void *elem, void *cl);

static void map_row_major(A2 array2, A2Methods_applyfun apply, void *cl) {
    UArray2_map_row_major(array2, (applyfun *)apply, cl);
}

static void map_col_major(A2 array2, A2Methods_applyfun apply, void *cl) {
    UArray2_map_col_major(array2, (applyfun *)apply, cl);
}

// now create the private struct containing pointers to the functions

static struct A2Methods_T array2_methods_plain_struct = {
    new,
    new_with_blocksize,
    a2free,
    width,
    height,
    size, 
    blocksize,
    at,
    map_row_major,
    map_col_major,
    NULL,
    map_row_major
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T array2_methods_plain = &array2_methods_plain_struct;
