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
    int64_t word = 0xC7;
    bitprint(word);
    int64_t new_word = Bitpack_newu(word, 3, 2, 3);
    bitprint(new_word);
    printf("%lud\n", new_word);
    int64_t extracted = Bitpack_getu(new_word, 3, 2);
    bitprint(extracted);
    printf("%lud\n", extracted);
    int64_t new_word2 = Bitpack_news(word, 3, 2, 3);
    int64_t extracted2 = Bitpack_gets(new_word2, 3, 2);
    bitprint(new_word2);
    printf("%lud\n", new_word2);
    bitprint(extracted2);
    printf("%lud\n", extracted2);
    assert(extracted == extracted2);
    assert(new_word == new_word2);
    return 0;
}
