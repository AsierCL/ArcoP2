#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>
#include <immintrin.h>
#include <pmmintrin.h>
#include <string.h>
#include <math.h>


#include "./counter.h"


void rellenarMatriz(float **a, float *b, float *x, int n) {
    // Inicialización de la matriz y de los vectores
    for (int i = 0; i < n; i++) {
        float suma_fila = 0.0f;
        // Inicializa cada elemento de la fila con (float)rand()/RAND_MAX
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
    if(argc != 3) {
        printf("Usage: %s <n> <c>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int c = atoi(argv[2]);
    srand(n);

    float **a = _mm_malloc(n * sizeof(float *), 64);
    for (int i = 0; i < n; i++) {
        a[i] = _mm_malloc(n * sizeof(float), 64);
    }
    float *b = _mm_malloc(n * sizeof(float), 64);
    float *x = _mm_malloc(n * sizeof(float), 64);
    float tol = 10e-8;
    int max_iter = 20000;
    float *x_new = _mm_malloc(n * sizeof(float), 64);
    
    float norm2 = 0;

    rellenarMatriz(a, b, x, n);
    //imprimirMatriz(a, n);

    start_counter();
    for (int iter = 0; iter < max_iter; iter++) {
        norm2 = 0;
        for (int i = 0; i < n; i++) {
            float sigma = 0;
            for (int j = 0; j < n; j++) {
                if(i!=j){
                    sigma += a[i][j] * x[j];
                }
            }
            x_new[i] = (b[i] - sigma) / a[i][i];
            norm2 += pow(x_new[i] - x[i], 2);
        }

        //x = x_new;
        memcpy(x, x_new, n * sizeof(float));
        if(sqrt(norm2) < tol) {
            double cycles = get_counter();
            csvPrint(iter, norm2, cycles);;
            return 0;
        }
    }
    unsigned long long int cycles = get_counter();
    csvPrint(-1, norm2, cycles);
    return 0;
}
