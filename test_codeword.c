#include "codeword.h"

int main() {
   float a = 1.0f;
   float b = -0.25f;
   float c = 0.3f;
   float d = -0.5f;
   float pb = -0.1f; 
   float pr = 0.1f;
   pack_word(a, b, c, d, pb, pr);    
   return 0;
}
