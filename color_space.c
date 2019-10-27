#include "color_space.h"

// y
double luma(double r, double g, double b) {
    return 0.299 * r + 0.587 * g + 0.114 * b;
}

// pb
double diff_blue(double r, double g, double b) {
    return -0.168736 * r - 0.331264 * g + 0.5 * b;
}

// pr
double diff_red(double r, double g, double b) {
    return 0.5 * r - 0.418688 * g - 0.081312 * b;
}

// red
double red(double y, double pb, double pr) {
    return 1.0 * y + 0.0 * pb + 1.402 * pr;
}

// green
double green(double y, double pb, double pr) {
    return 1.0 * y - 0.344136 * pb - 0.714136 * pr;
}

// blue
double blue(double y, double pb, double pr) {
    return 1.0 * y + 1.772 * pb + 0.0 * pr;
}
