#include <stdio.h>
#include "mandelbrot.h"
#include <stdlib.h>
#include <string.h>

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

void escreve_pgm(const char *nome_arquivo, int *buffer, int largura, int altura, int max_iter) {
    FILE *f = fopen(nome_arquivo, "w");
    if (f == NULL) {
        fprintf(stderr, "Erro ao abrir %s para escrita\n", nome_arquivo);
        return;
    }

    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            int iter = buffer[y * largura + x];
            int intensidade = (iter * 255) / max_iter;
            if (x > 0) {
                fprintf(f, " ");
            }
            fprintf(f, "%d", intensidade);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}

void mandelbrot_serial(int *buffer, int largura, int altura, int max_iter) {
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            double cx = XMIN + x * (XMAX - XMIN) / largura;
            double cy = YMIN + y * (YMAX - YMIN) / altura;
            buffer[y * largura + x] = calcula_pixel(cx, cy, max_iter);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s largura altura max_iteracoes num_threads\n", argv[0]);
        return 1;
    }

    int largura = atoi(argv[1]);
    int altura = atoi(argv[2]);
    int max_iter = atoi(argv[3]);
    int num_threads = atoi(argv[4]);
    (void)num_threads;

    int *buffer = malloc(largura * altura * sizeof(int));
    if (buffer == NULL) {
        fprintf(stderr, "Erro ao alocar buffer\n");
        return 1;
    }

    mandelbrot_serial(buffer, largura, altura, max_iter);
    escreve_pgm("mandelbrot_hac2_serial.pgm", buffer, largura, altura, max_iter);

    free(buffer);
    return 0;
}