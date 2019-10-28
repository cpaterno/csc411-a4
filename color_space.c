#include "color_space.h"

// y
float luma(float r, float g, float b) {
    return 0.299f * r + 0.587f * g + 0.114f * b;
}

// pb
float diff_blue(float r, float g, float b) {
    return -0.168736f * r - 0.331264f * g + 0.5f * b;
}

// pr
float diff_red(float r, float g, float b) {
    return 0.5f * r - 0.418688f * g - 0.081312f * b;
}

// red
float red(float y, float pb, float pr) {
    return 1.0f * y + 0.0f * pb + 1.402f * pr;
}

// green
float green(float y, float pb, float pr) {
    return 1.0f * y - 0.344136f * pb - 0.714136f * pr;
}

// blue
float blue(float y, float pb, float pr) {
    return 1.0f * y + 1.772f * pb + 0.0f * pr;
}
