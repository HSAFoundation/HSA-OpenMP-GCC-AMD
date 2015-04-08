#include <omp.h>
#include <stdio.h>
#include <string.h>
#define N 256

void printList(int *src, int *dst, int size) {
  int idx = 0;
  for (; idx < size; idx++) {
    printf ("Src[%d] : %d Dst[%d] : %d \n", idx, src[idx], idx, dst[idx]);
  }
}

void veccpy(int *a, int *b, int n) {

   int j;
   #pragma omp parallel for
   for( j = 0; j<n;j++) {
      b[j] = a[j];
   }
}

int main() {
  int a[N], b[N], i;

  for (i = 0; i< N; i++) {
    a[i] = i;
    b[i] = 0;
  }
  
  veccpy(a, b, N);

  if (memcmp(a, b, N * sizeof (int)) != 0) {
    printList((int *)a, (int *)b, N);
    printf ("Vector copy C case - Failed \n");
  } else {
    printf ("Vector copy C case - Passed \n");
  }
  return 0;
} 
