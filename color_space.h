#ifndef __COLOR_SPACE__
#define __COLOR_SPACE__


extern double luma(double r, double g, double b); // y

extern double diff_blue(double r, double g, double b); // pb

extern double diff_red(double r, double g, double b); // pr

extern double red(double y, double pb, double pr); // red

extern double green(double y, double pb, double pr); // green

extern double blue(double y, double pb, double pr); // blue


#endif
