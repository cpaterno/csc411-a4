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

- [x] Codeword
  - [x] codeword macro
  - [x] private functions
  - [x] `pack_word`
  - [x] unpack functions

- [x] Color Space
  - [x] `float luma(float r, float g, float b)` // y 
  - [x] `float diff_blue(float r, float g, float b)` // pb
  - [x] `float diff_red(float r, float g, float b)` // pr
  - [x] `float red(float y, float pb, float pr)` // red
  - [x] `float green(float y, float pb, float pr)` // green
  - [x] `float blue(float y, float pb, float pr)` // blue

- [x] Discrete Cosine Transform
  - [x] `float luma_tl(float a, float b, float c, float d)` // y1
  - [x] `float luma_tr(float a, float b, float c, float d)` // y2
  - [x] `float luma_bl(float a, float b, float c, float d)` // y3
  - [x] `float luma_br(float a, float b, float c, float d)` // y4
  - [x] `float coef_a(float y1, float y2, float y3, float y4)` // a
  - [x] `float coef_b(float y1, float y2, float y3, float y4)` // b
  - [x] `float coef_c(float y1, float y2, float y3, float y4)` // c
  - [x] `float coef_d(float y1, float y2, float y3, float y4)` // d

- [x] Decompressed PPM
  - [x] `Pnm_rgb_f` struct
    - [x] float red
    - [x] float green
    - [x] float blue
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
  - [x] Advanced steps (done before write out)
    - [x] Convert from RGB to YPBPR
    - [x] Convert from YPBPR to RGB

- [x] PPM Diff
  - [x] Read in 1-2 arguments (representing ppms, missing arg is stdin)
  - [x] Read in first ppm to I, and second ppm to I'
  - [x] Check if width and height diffs are at most 1
    - [x] If not, print error message to stderr and print 1.0 to stdout, cleanup and abort
  - [x] For every pixel, using the smaller image's dimensions
    - [x] Compute root mean square difference (E)
  - [x] Print E to stdout with four digits of precision
