# csc411-a4
Integer, Logical Operations and Image Compression  
By Jack Edgar and Chris Paterno

## Modules
- [ ] Bitpack
- [ ] Compress
- [ ] Color Space
  - [ ] `double luma(double r, double g, double b)` // y 
  - [ ] `double diff_blue(double r, double g, double b)` // pb
  - [ ] `double diff_red(double r, double g, double b)` // pr
  - [ ] `double red(double y, double pb, double pr)` // red
  - [ ] `double green(double y, double pb, double pr)` // green
  - [ ] `double blue(double y, double pb, double pr)` // blue
- [ ] Discrete Cosine Transform
  - [ ] `double luma_tl(double a, double b, double c, double d)` // y1
  - [ ] `double luma_tr(double a, double b, double c, double d)` // y2
  - [ ] `double luma_bl(double a, double b, double c, double d)` // y3
  - [ ] `double luma_br(double a, double b, double c, double d)` // y4
  - [ ] `double coef_a(double y1, double y2, double y3, double y4)` // a
  - [ ] `double coef_b(double y1, double y2, double y3, double y4)` // b
  - [ ] `double coef_c(double y1, double y2, double y3, double y4)` // c
  - [ ] `double coef_d(double y1, double y2, double y3, double y4)` // d
- [ ] Decompressed PPM
  - [ ] 
- [ ] Compressed PPM
  - [ ] codeword macro
  - [ ] Pnm_comp struct
    - [ ] unsigned width
    - [ ] unsigned height
    - [ ] unsigned num_words
    - [ ] Array_T words
   - [ ] `void Ppm_comp_read(FILE *fp)`
   - [ ] `void Ppm_comp_write(FILE *fp)`
   - [ ] `void Ppm_comp_free(Ppm_comp *ppmp)`
   - [ ] 
  
## Programs
- [ ] Image
- [ ] PPM Test
- [ ] PPM Diff
