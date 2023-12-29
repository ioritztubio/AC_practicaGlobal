#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int cuadrado(int num) {
    int x = (int)floor(sqrt((double)num));
    return x * x == num;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0) {
        printf("Tamaño inválido\n");
        return 1;
    }

    int cuadrados[N + 1]; // Declaración de una matriz estática

    double start_time = omp_get_wtime(); // Registro del tiempo inicial

    // Llenar el vector con los cuadrados de los primeros N números
    #pragma omp parallel for
    for (int i = 1; i <= N; i++) {
        cuadrados[i] = i * i;
    }

    int count = 0;

    #pragma omp parallel for reduction(+:count)
    for (int i = 1; i <= N; i++) {
        for (int j = i; j <= N; j++) {
            int suma = cuadrados[i] + cuadrados[j];
            if (cuadrado(suma)) {
                count++;
            }
        }
    }

    double end_time = omp_get_wtime(); // Registro del tiempo final
    double execution_time = (end_time - start_time) * 1000; // Cálculo del tiempo total de ejecución en milisegundos

    printf("Número total de pares que cumplen la condición: %d\n", count);
    printf("Tiempo de ejecución: %f milisegundos\n", execution_time);

    return 0;
}

