/***************************************
    Arquitectura de Computadores
    OpenMP laborategia

    reparto_it.c  --  PARA PARALELIZAR
*****************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <omp.h>
#include <time.h>
#define N 100
#define LOTE 6

void fun (int x){
  usleep (100*(x%15));
}

int main () {
  int  i, j, A[N], B[N];
  int  tid = -1;
  double tex;
  struct timespec t0, t1;


  clock_gettime(CLOCK_REALTIME, &t0);
  // Bucle 1 para paralelizar: reparto estatico consecutivo
  #pragma omp parallel for private(i, tid) shared(A) schedule(static, 1)
  for (i=0; i<N; i++) 
  {  
    tid = omp_get_thread_num();
    fun (i);
    A[i] = tid;
  }

  // Bucle 2 para paralelizar: reparto dinamico, chunk scheduling
  // lote (chuck) de 6 iteraciones (LOTE)
  #pragma omp parallel for private(i, tid) shared(B) schedule(dynamic, LOTE)
  for (i=0; i<N; i++) 
  {  
    tid = omp_get_thread_num();
    fun (i);
    B[i] = tid;
  }

  // inprimir vectores como matrices de 10 x 10
  printf ("\n\n Vector A\n");
  for (i=0; i<10; i++) {
    for (j=0; j<10; j++) printf ("%3d", A[10*i+j]);
    printf ("\n");
  }

  printf ("\n\n Vector B\n");
  for (i=0; i<10; i++) {
    for (j=0; j<10; j++) printf ("%3d", B[10*i+j]);
    printf ("\n");
  }
  clock_gettime(CLOCK_REALTIME, &t1);
  tex = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / (double)1e9;
  printf("Tiempo de ejecucion (serie) = %1.3f ms \n\n", tex * 1000);


  return (0);
}
