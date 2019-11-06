#include <stdio.h>
#include "bitpack.h"
#include "assert.h"

void bitprint(uint64_t n) {
    for (uint64_t i = (uint64_t)1 << 63; i > 0; i /= 2) {
        (n & i) ? printf("1") : printf("0");
    }
    printf("\n");
}

int main() {
    uint64_t uword = 0xDC;
    uint64_t ures = Bitpack_getu(uword, 3, 4);
    bitprint(uword);
    printf("%lu\n", uword);
    bitprint(ures);
    printf("%lu\n", ures);

    int64_t sword = 0xB8;
    int64_t sres = Bitpack_gets(sword, 4, 2);
    bitprint(sword);
    printf("%ld\n", sword);
    bitprint(sres);
    printf("%ld\n", sres);

    uint64_t uval = 0x2AA;
    int64_t sval = 0x2AA;
    bitprint(uval);
    printf("%lu\n", uval); 
    bitprint(sval);
    printf("%ld\n", sval);
    uint64_t unword = Bitpack_newu(sres, 10, 17, uval);
    bitprint(unword);
    printf("%lu\n", unword);
    int64_t snword = Bitpack_news(sres, 10, 17, sval);
    bitprint(snword);
    printf("%ld\n", snword);
    return 0;
}
