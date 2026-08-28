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

int main(void) {
    int buffer[16] = {1, 2, 2, 2, 1, 3, 6, 5, 50, 50, 50, 50, 1, 3, 6, 5};
    escreve_pgm("teste.pgm", buffer, 4, 4, 50);
    return 0;
}