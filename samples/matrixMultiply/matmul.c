#include <stdio.h>
#define  size  8
int i,j,k;

int main()
{
    float a[size][size];
    float b[size][size];
    float c[size][size];

    // Initialize the buffers.
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            a[i][j] = (float)i * 7 + j * 9;
            b[i][j] = (float)i* 20 - j * 15;
            c[i][j] = 0.0f;
        }
    }

    // Do the matrix multiplication C <- C + A x B
    #pragma omp target
    #pragma omp parallel for default(none) private(i,j,k) shared(a,b,c)
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            for (k = 0; k < size; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    // Print the matrix.
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            printf ("\n Value of c[%d][%d]:%f\n",i,j,c[i][j]);
        }
    }
    return 0;
}

