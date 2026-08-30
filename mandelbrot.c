#include <stdio.h>
#include "mandelbrot.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    int *buffer;
    int largura;
    int altura;
    int max_iter;
    int y_inicio;
    int y_fim;   // exclusivo: essa thread calcula de y_inicio ate y_fim-1
} ThreadArgs;

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

void mandelbrot_openmp(int *buffer, int largura, int altura, int max_iter) {
    #pragma omp parallel for
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            double cx = XMIN + x * (XMAX - XMIN) / largura;
            double cy = YMIN + y * (YMAX - YMIN) / altura;
            buffer[y * largura + x] = calcula_pixel(cx, cy, max_iter);
        }
    }
}

void *worker_pthreads1(void *arg) {
    ThreadArgs *a = (ThreadArgs *)arg;

    for (int y = a->y_inicio; y < a->y_fim; y++) {
        for (int x = 0; x < a->largura; x++) {
            double cx = XMIN + x * (XMAX - XMIN) / a->largura;
            double cy = YMIN + y * (YMAX - YMIN) / a->altura;
            a->buffer[y * a->largura + x] = calcula_pixel(cx, cy, a->max_iter);
        }
    }

    return NULL;
}

void mandelbrot_pthreads1(int *buffer, int largura, int altura, int max_iter, int num_threads) {
    pthread_t threads[num_threads];
    ThreadArgs args[num_threads];

    int linhas_por_thread = altura / num_threads;

    for (int i = 0; i < num_threads; i++) {
        args[i].buffer = buffer;
        args[i].largura = largura;
        args[i].altura = altura;
        args[i].max_iter = max_iter;
        args[i].y_inicio = i * linhas_por_thread;
        args[i].y_fim = (i == num_threads - 1) ? altura : (i + 1) * linhas_por_thread;
        pthread_create(&threads[i], NULL, worker_pthreads1, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
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

    int *buffer = malloc(largura * altura * sizeof(int));
    if (buffer == NULL) {
        fprintf(stderr, "Erro ao alocar buffer\n");
        return 1;
    }

    mandelbrot_pthreads1(buffer, largura, altura, max_iter, num_threads);
    escreve_pgm("mandelbrot_hac2_pthreads1.pgm", buffer, largura, altura, max_iter);

    free(buffer);
    return 0;
}