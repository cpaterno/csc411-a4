#ifndef __UARRAY_2__
#define __UARRAY_2__

#define T UArray2_T
typedef struct T *T;

// UArray2 "constructor" and "destructor"
extern T UArray2_new(int width, int height, int size);
extern void UArray2_free(T *array2);

// get pointer to element in UArray2
extern void *UArray2_at(T array2, int x, int y);

// get each element's size
extern int UArray2_size(T array2);
// get Uarray2's width (number of columns)
extern int UArray2_width(T array2);
// get UArray2's height (number of rows)
extern int UArray2_height(T array2);

// Apply a function to each element in UArray2 by row major order 
extern void UArray2_map_row_major(T array2,
	   void apply(int x, int y, T array2, void *elem, void *cl), void *cl);	
// Apply a function to each element in UArray2 by column major order
extern void UArray2_map_col_major(T array2,
	   void apply(int x, int y, T array2, void *elem, void *cl), void *cl);	
#undef T
#endif
