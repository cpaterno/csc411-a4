#include <stdio.h>
#include "bitpack.h"
#include "assert.h"

void bitprint(uint64_t n, unsigned width) {
    for (int i = (uint64_t)i << (width - 1); i > 0; i /= 2) {
        (n & i) ? printf("1 ") : printf("0 ");
    }
    printf("\n");
}

int main() {

   return 0;
}
