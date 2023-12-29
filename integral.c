#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const int LIM_TRAP = 9000000;

double f(double x) {
    double y;
    y = 1.0 / (sin(x) + 2.0) + 1.0 / (sin(x) * cos(x) + 2.0);
    return y;
}

void Leer_datos(double *a, double *b, double *c_m) {
    double x, y, z;
    printf("\nIntroduzca a, b (limites) y c_m (cota máxima de error): \n");
    scanf("%lf %lf %lf", &x, &y, c_m);
    (*a) = x;
    (*b) = y;
    (*c_m) = z;
}

double Integrar(double a, double b, int n, double w) {
    double resultado = (f(a) + f(b)) / 2.0;

    #pragma omp parallel for reduction(+: resultado)
    for (int i = 1; i < n; i++) {
        double x = a + i * w;
        resultado += f(x);
    }

    resultado *= w;
    return resultado;
}

int main() {
    double a, b, w, resultado, resultado_2, diferencia, c_m;
    int n = 2;

    double tex;
    struct timespec t0, t1;

    Leer_datos(&a, &b, &c_m);
    w = (b - a) / n;

    diferencia = 1;

    clock_gettime(CLOCK_REALTIME, &t0);
    while (diferencia > c_m && n < LIM_TRAP) {
        resultado = Integrar(a, b, n, w);
        n *= 2;

        if (n > 9000000) {
            printf("\nLa cantidad de trapecios ha llegado a su límite (9000000): %i\n", n);
        } else {
            w = (b - a) / n;
            resultado_2 = Integrar(a, b, n, w);
            diferencia = resultado_2 - resultado;

            if (diferencia < 0) {
                diferencia *= (-1);
            }
        }
    }

    resultado = Integrar(a, b, n, w);

    clock_gettime(CLOCK_REALTIME, &t1);
    tex = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / (double)1e9;

    printf("\nValor de la integral: %.12f\n", resultado_2);
    printf("Tiempo de ejecucion (serie) = %1.3f ms \n\n", tex * 1000);

    return 0;
}
