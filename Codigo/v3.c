#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>
#include <immintrin.h>
#include <string.h>
#include <math.h>

#include "counter.h"

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
    if(argc != 3) {
        printf("Usage: %s <n> <c>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int c = atoi(argv[2]);

    srand(1);
    //srand(n);

    float **a = _mm_malloc(n * sizeof(float *), 64);
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
        norm2 = 0.0f;
        for (int i = 0; i < n; i++) {
            // Vectorizado con AVX2: sumamos a[i][j] * x[j] en chunks de 8 floats
            __m256 vs = _mm256_setzero_ps();
            int j = 0;

            // Parte antes de la diagonal (j = 0 .. i-1)
            int L1 = i;
            for (j = 0; j + 8 <= L1; j += 8) {
                __m256 ai = _mm256_loadu_ps(&a[i][j]);
                __m256 xv = _mm256_loadu_ps(&x[j]);
                vs = _mm256_fmadd_ps(ai, xv, vs);
            }
            float sigma = 0.0f;
            // Acumular parcial vectorial
            float tmp[8];
            _mm256_storeu_ps(tmp, vs);
            for (int k = 0; k < 8; k++) sigma += tmp[k];
            // Restantes antes de i
            for (int jj = j; jj < L1; jj++)
                sigma += a[i][jj] * x[jj];

            // Parte después de la diagonal (j = i+1 .. n-1)
            int L2 = n;
            j = i + 1;
            for (; j + 8 <= L2; j += 8) {
                __m256 ai = _mm256_loadu_ps(&a[i][j]);
                __m256 xv = _mm256_loadu_ps(&x[j]);
                vs = _mm256_setzero_ps();               // reutilizamos vs para esta parte
                vs = _mm256_fmadd_ps(ai, xv, vs);
                _mm256_storeu_ps(tmp, vs);
                for (int k = 0; k < 8; k++) sigma += tmp[k];
            }
            // Restantes después de i
            for (int jj = j; jj < L2; jj++)
                sigma += a[i][jj] * x[jj];

            // Jacobi update
            float xi_new = (b[i] - sigma) / a[i][i];
            norm2 += (xi_new - x[i]) * (xi_new - x[i]);
            x_new[i] = xi_new;
        }

        // Copiamos x_new en x
        memcpy(x, x_new, n * sizeof(float));

        if (sqrtf(norm2) < tol) {
            double cycles = get_counter();
            csvPrint(iter, norm2, cycles);
            return 0;
        }
    }
    unsigned long long cycles = get_counter();
    csvPrint(-1, norm2, cycles);
    return 0;
}
