#include "assert.h"
#include "ppm_c.h"

int main(int argc, char *argv[]) {
    // handle command line args and open file 
    assert(argc <= 2);
    FILE *instream = NULL;
    if (argc == 1) {
        instream = stdin;
    } else {
        instream = fopen(argv[1], "rb");
        assert(instream);
    }
    // make new comp img
    Pnm_comp img = Pnm_comp_read(instream);
    Pnm_comp_write(stdout, img);
    Pnm_comp_free(&img);    
    return 0;
}
