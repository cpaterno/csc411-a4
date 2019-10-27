#include "assert.h"
#include "ppm_d.h"
#include "a2plain.h"

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
   // init image
   A2Methods_T methods = array2_methods_plain;
   assert(methods);
   Pnm_ppm image = Pnm_ppmread(instream, methods);
   if (instream != stdin) {
       fclose(instream);
   }
   // make new trimmed image
   Pnm_ppm trimmed = Pnm_trim(image);
   Pnm_ppmfree(&image);
   // convert to float rep
   Pnm_ppm float_img = Pnm_floatrep(trimmed);
   Pnm_ppmfree(&trimmed);
   // convert to int rep
   Pnm_ppm int_img = Pnm_intrep(float_img);
   Pnm_ppmfree(&float_img);
   // write out result
   Pnm_ppmwrite(stdout, int_img);
   Pnm_ppmfree(&int_img);
   return 0;
}
