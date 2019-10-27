#include "assert.h"
#include "mem.h"
#include "ppm_d.h"
#include "a2plain.h"
#include "color_space.h"

Pnm_ppm Pnm_cvrep(const Pnm_ppm);
Pnm_ppm Pnm_rgbrep(const Pnm_ppm);

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
   // done with file so close it
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
   // convert to cv space
   Pnm_ppm cv = Pnm_cvrep(int_img);
   Pnm_ppmfree(&int_img);
   // convert back to rgb space (intrep)
   Pnm_ppm rgb = Pnm_rgbrep(cv);
   Pnm_ppmfree(&cv);
   // write out result
   Pnm_ppmwrite(stdout, rgb);
   Pnm_ppmfree(&rgb);
   return 0;
}

Pnm_ppm Pnm_cvrep(const Pnm_ppm ppm) {
    assert(ppm);
    // make float rep of image
    Pnm_ppm out = Pnm_floatrep(ppm);
    // update float values so they represent y, pb, pr
    Pnm_rgb_f pixel = NULL;
    double y, pb, pr;
    y = pb = pr = 0.0;
    for (unsigned i = 0; i < out->height; ++i) {
        for (unsigned j = 0; j < out->width; ++j) {
	    pixel = (Pnm_rgb_f)out->methods->at(out->pixels, j, i);
	    y = luma(pixel->red, pixel->green, pixel->blue);
	    pb = diff_blue(pixel->red, pixel->green, pixel->blue);
	    pr = diff_red(pixel->red, pixel->green, pixel->blue);
	    pixel->red = y;
	    pixel->green = pb;
	    pixel->blue = pr;
	}
    }
    return out;
}

Pnm_ppm Pnm_rgbrep(const Pnm_ppm ppm) {
    assert(ppm);
    // assumed input image is a float rep in cv space
    Pnm_ppm temp;
    NEW(temp);
    temp->width = ppm->width;
    temp->height = ppm->height;
    temp->denominator = ppm->denominator;
    temp->methods = ppm->methods;
    temp->pixels = temp->methods->new(temp->width, temp->height,
		                      sizeof(struct Pnm_rgb_f));
    // update float values so they represent r, g, b
    Pnm_rgb_f old_pixel = NULL;
    Pnm_rgb_f new_pixel = NULL;
    double r, g, b;
    r = g = b = 0;
    for (unsigned i = 0; i < temp->height; ++i) {
        for (unsigned j = 0; j < temp->width; ++j) {
	    old_pixel = (Pnm_rgb_f)ppm->methods->at(ppm->pixels, j, i);
            r = red(old_pixel->red, old_pixel->green, old_pixel->blue);
            g = green(old_pixel->red, old_pixel->green, old_pixel->blue);
            b = blue(old_pixel->red, old_pixel->green, old_pixel->blue);
	    new_pixel = (Pnm_rgb_f)temp->methods->at(ppm->pixels, j, i);
	    new_pixel->red = r;
	    new_pixel->green = g;
	    new_pixel->blue = b;
        }
    }
    Pnm_ppm out = Pnm_intrep(temp);
    Pnm_ppmfree(&temp);
    return out;
}
