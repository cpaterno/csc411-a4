# csc411-a4
Integer, Logical Operations and Image Compression  
By Jack Edgar and Chris Paterno

## Modules
- [ ] Bitpack
  - [ ] Raise `Except_T Bitpack_Overflow` when appropriate
  - [ ] `bool Bitpack_fitsu(uint64_t n, unsigned width)`
  - [ ] `bool Bitpack_fitss( int64_t n, unsigned width)`
  - [ ] `uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)`
  - [ ] `int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)`
  - [ ] `uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)`
  - [ ] `uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value)`

- [ ] Compress
  - [ ] Compress Algorithm
    - [ ] Read and store decompressed ppm and allocate space for compressed ppm
    - [ ] Trim width and height so they are even
    - [ ] Change rgb to float representation
    - [ ] Convert every pixel’s color space from RGB to Component Video
    - [ ] For every 2 by 2 block of an image, pack it into a 32b word
      - [ ] Average a block’s Pb and Pr
      - [ ] Convert average Pb and average Pr into their respective 4b code words
      - [ ] Use a discrete cosine transform to convert the brightnesses (Ys) to a, b, c, d
      - [ ] Convert a to a 9b unsigned. Convert b, c, d into 5b signed scaled integers
      - [ ] Pack results of steps 5.2 and 5.4 into code word
    - [ ] Pack results a, b, c, d into codeword
    - [ ] Write compressed image to stdout
  - [ ] Decompress Algorithm
    - [ ] Read and store compressed ppm and allocate space for uncompressed ppm
    - [ ] Unpack every codeword in compressed image
      - [ ] Convert 4b code words into average Pb and average Pr
      - [ ] Convert a, b, c, d back intro floating point numbers
      - [ ] Undo discrete cosine transform to convert a, b, c, d into the brightnesses (Ys)
      - [ ] Convert every pixel’s color space from Component Video to RGB
     - [ ] Write out decompressed image to stdout

- [ ] Codeword
  - [ ] codeword macros
  - [ ] `unsigned code_a(double a)`
  - [ ] `int code_bcd(double d)`

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

- [x] Decompressed PPM
  - [x] `Pnm_rgb_f` struct
    - [x] double red
    - [x] double green
    - [x] double blue
   - [x] `unsigned num_blocks(Pnm_ppm ppm)`
   - [x] `Pnm_ppm trim(const Pnm_ppm ppm)`
   - [x] `Pnm_ppm floatrep(const Pnm_ppm ppm)`
   - [x] `Pnm_ppm intrep(const Pnm_ppm ppm)`

- [ ] Compressed PPM
  - [ ] `Pnm_comp` struct
    - [ ] unsigned width
    - [ ] unsigned height
    - [ ] `unsigned num_words`
    - [ ] `Array_T words`
   - [ ] `void Ppm_comp_read(FILE *fp)`
   - [ ] `void Ppm_comp_write(FILE *fp)`
   - [ ] `void Ppm_comp_free(Ppm_comp *ppmp)`
  
## Programs
- [ ] Image
  - [ ] Implement compress (image hands off to this module)

- [x] PPM Test
  - [x] Read decompressed ppm
  - [x] Trim image
  - [x] Convert to floating point numbers
  - [x] Convert back to integers
  - [x] Write out image
  - [ ] Advanced steps (done before write out)
    - [ ] Convert from RGB to YPBPR
    - [ ] Convert from YPBPR to RGB

- [x] PPM Diff
  - [x] Read in 1-2 arguments (representing ppms, missing arg is stdin)
  - [x] Read in first ppm to I, and second ppm to I'
  - [x] Check if width and height diffs are at most 1
    - [x] If not, print error message to stderr and print 1.0 to stdout, cleanup and abort
  - [x] For every pixel, using the smaller image's dimensions
    - [x] Compute root mean square difference (E)
  - [x] Print E to stdout with four digits of precision
