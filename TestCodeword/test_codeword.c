#include <stdio.h>
#include "codeword.h"

int main() {
   float a = 1.0f;
   float b = -0.25f;
   float c = 0.3f;
   float d = -0.5f;
   float pb = -0.01f; 
   float pr = -0.1f;
   printf("a %f, b %f, c %f, d %f, pb %f, pr %f\n",
           a, b, c, d, pb, pr);
   codeword word = pack_word(a, b, c, d, pb, pr);
   float new_a = unpack_a(word);
   printf("a: %f\n", new_a); 
   float new_b = unpack_b(word);
   printf("b: %f\n", new_b); 
   float new_c = unpack_c(word);
   printf("c: %f\n", new_c); 
   float new_d = unpack_d(word);
   printf("d: %f\n", new_d); 
   float new_pb = unpack_pb(word);
   printf("pb: %f\n", new_pb); 
   float new_pr = unpack_pr(word);
   printf("pr: %f\n", new_pr); 
   return 0;
}
