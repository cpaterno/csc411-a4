#include <stdlib.h>
#include "assert.h"
#include "mem.h"
#include "array.h"
#include "uarray2.h"

#define T UArray2_T

struct T {
    int height;
    int width;
    int size;
    Array_T array;
};

// UArray2 "constructor" and "destructor"
T UArray2_new(int width, int height, int size) {
    assert(width > 0 && height > 0 && size > 0);
    T array2;
    NEW(array2);
    array2->height = height;
    array2->width = width;
    array2->size = size;
    array2->array = Array_new(width * height, size);
    return array2;
}

void UArray2_free(T *array2) {
    assert(array2 && *array2);
    Array_free(&(*array2)->array);
    FREE(*array2);
}

// get pointer to element in UArray2
void *UArray2_at(T array2, int x, int y) {
    assert(array2);
    assert(x >= 0 && x < array2->width);
    assert(y >= 0 && y < array2->height);
    return Array_get(array2->array, y * array2->width + x);
}

// get each element's size
int UArray2_size(T array2) {
    assert(array2);
    return array2->size;
}

// get Uarray2's width (number of columns)
int UArray2_width(T array2) {
    assert(array2);
    return array2->width;
}

// get UArray2's height (number of rows)
int UArray2_height(T array2) {
    assert(array2);
    return array2->height;
}

// Apply a function to each element in UArray2 by row major order 
void UArray2_map_row_major(T array2, 
    void apply(int x, int y, T array2, void *elem, void *cl), void *cl) {
    assert(array2 && apply);
    void *element = NULL;
    for (int i = 0; i < array2->height; ++i) {
        for (int j = 0; j < array2->width; ++j) {
	    element = UArray2_at(array2, j, i);
            apply(j, i, array2, element, cl);
	}
    }    
}

// Apply a function to each element in UArray2 by column major order
void UArray2_map_col_major(T array2,
    void apply(int x, int y, T array2, void *elem, void *cl), void *cl) {
    assert(array2 && apply);
    void *element = NULL;
    for (int j = 0; j < array2->width; ++j) {
        for (int i = 0; i < array2->height; ++i) {
	    element = UArray2_at(array2, j, i);
	    apply(j, i, array2, element, cl);
	}
    }    
}
