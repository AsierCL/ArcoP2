#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>
#include <immintrin.h>
#include <pmmintrin.h>
#include <string.h>
#include <omp.h>
#include <math.h>


#include "./counter.h"


void rellenarMatriz(float **a, float *b, float *x, int n) {
    // Inicialización de la matriz y de los vectores
    for (int i = 0; i < n; i++) {
        float suma_fila = 0.0f;
        
        // Inicializa cada elemento de la fila con (float)rand()/RAND_MAX
        a[i] = _mm_malloc(n * sizeof(float), 64);
        for (int j = 0; j < n; j++) {
            a[i][j] = (float)rand() / RAND_MAX;
            suma_fila += a[i][j];
        }
        // 3. Al elemento de la diagonal se le suma la suma total de la fila
        a[i][i] += suma_fila;

        // Inicializa el vector b aleatoriamente
        b[i] = (float)rand() / RAND_MAX;
        // Inicializa el vector x con ceros
        x[i] = 0.0f;
    }
}

void csvPrint(int iter, double norm, unsigned long long int cycles) {
    printf("%d,%e,%llu\n", iter, norm, cycles);
}


int main(int argc, char const *argv[]) {
    if(argc < 2) {
        printf("Usage: %s <n> <c>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int c = atoi(argv[2]);  // Numero de hilos (opcional)

    srand(1);
    //srand(n);

    float **a = _mm_malloc(n * sizeof(float *), 64);
    float *b = _mm_malloc(n * sizeof(float), 64);
    float *x = _mm_malloc(n * sizeof(float), 64);
    float tol = 10e-8;
    int max_iter = 20000;
    float *x_new = _mm_malloc(n * sizeof(float), 64);

    float norm2 = 0.0f;
    int i, j;
    float sigma;
    
    rellenarMatriz(a, b, x, n);
    //imprimirMatriz(a, n);
    
    start_counter();
    for (int iter = 0; iter < max_iter; iter++) {
        norm2 = 0.0f;  // Compartida en la región parallel

        /*
        #pragma omp parallel private(i, j, sigma) \
                shared(a, b, x, x_new, n, tol) \
                reduction(+:norm2) \
                num_threads(c)
        */

        #pragma omp parallel private(i, j, sigma) \
                shared(a, b, x, x_new, n, tol) \
                num_threads(c)
        {
            float local_norm = 0.0f;  // Acumulador por hilo

            #pragma omp for schedule(static)
            for (i = 0; i < n; i++) {
                sigma = 0;
                for (j = 0; j < n; j++) {
                    if(i != j){
                        sigma += a[i][j] * x[j];
                    }
                }
                x_new[i] = (b[i] - sigma) / a[i][i];
                local_norm += (x_new[i] - x[i]) * (x_new[i] - x[i]);
            }
            
            #pragma omp atomic
            norm2 += local_norm;  // Suma hilo‑segura sin reduction
        }
                
        memcpy(x, x_new, n * sizeof(float));
        if(sqrt(norm2) < tol) {
            double cycles = get_counter();
            csvPrint(iter, norm2, cycles);
            exit(0);
        }
    }

    unsigned long long int cycles = get_counter();
    csvPrint(-1, norm2, cycles);
    return 0;
}
