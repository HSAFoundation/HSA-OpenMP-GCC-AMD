void veccpy2(int *a, int *b, int n) {

   int j;
   #pragma omp parallel for
   for( j = n/2; j<n;j++) {
      b[j]= a[j];
   }
}


