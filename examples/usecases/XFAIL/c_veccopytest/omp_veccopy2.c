#include <omp.h>
#include <stdio.h>
#include <string.h>
#define N 256
void func(int *a, int *b, int n) {

   int j;
   #pragma omp parallel for
   for( j = 0; j<n;j++) {
      b[j] = a[j];
   }
}

