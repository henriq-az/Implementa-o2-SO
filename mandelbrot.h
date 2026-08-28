#include "mandelbrot.h"

#ifndef MANDELBROT_H
#define MANDELBROT_H

#define LARGURA_MAX 4096
#define ALTURA_MAX 4096

#define XMIN -2.0
#define XMAX 1.0
#define YMIN -1.5
#define YMAX 1.5

int calcula_pixel(double cx, double cy, int max_iter);
void escreve_pgm(const char *nome_arquivo, int *buffer, int largura, int altura, int max_iter);

void mandelbrot_serial(int *buffer, int largura, int altura, int max_iter);
void mandelbrot_openmp(int *buffer, int largura, int altura, int max_iter);
void mandelbrot_pthreads1(int *buffer, int largura, int altura, int max_iter, int num_threads);
void mandelbrot_pthreads2(int *buffer, int largura, int altura, int max_iter, int num_threads);

#endif