#include <stdio.h>
#include "mandelbrot.h"

int calcula_pixel(double cx, double cy, int max_iter) {
    double zx = 0.0, zy = 0.0;
    int iter = 0;

    while ((zx * zx + zy * zy <= 4.0) && (iter < max_iter)) {
        double tmp = zx * zx - zy * zy + cx;
        zy = 2 * zx * zy + cy;
        zx = tmp;
        iter++;
    }

    return iter;
}

int main(void) {
    printf("%d\n", calcula_pixel(-1.25, -0.75, 50));
    return 0;
}