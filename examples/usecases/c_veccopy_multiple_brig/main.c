#include <omp.h>
#include <stdio.h>
#include <string.h>
#define N 256
extern void veccpy1(int *a, int *b, int n);
extern void veccpy2(int *a, int *b, int n);

void printList(int *src, int *dst, int size) {
  int idx = 0;
  for (; idx < size; idx++) {
    printf ("Src[%d] : %d Dst[%d] : %d \n", idx, src[idx], idx, dst[idx]);
  }
}

int main() {
  int a[N], b[N], i;

  for (i = 0; i< N; i++) {
    a[i] = i;
  }
  
  veccpy1(a, b, N);
  veccpy2(a, b, N);

  if (memcmp(a, b, N * sizeof (int)) != 0) {
    printList((int *)a, (int *)b, N);
    printf ("Vector Copy (Brig from multiple files) - Failed\n");
  } else {
    printf ("Vector Copy (Brig from multiple files) - Passed \n");
  }
  return 0;
} 
