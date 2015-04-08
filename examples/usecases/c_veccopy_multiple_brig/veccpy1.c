void veccpy1(int *a, int *b, int n) {

   int j;
   #pragma omp parallel for
   for( j = 0; j<n/2;j++) {
      b[j]= a[j];
   }
}


