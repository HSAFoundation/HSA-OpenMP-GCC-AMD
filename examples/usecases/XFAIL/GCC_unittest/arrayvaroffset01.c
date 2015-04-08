struct S
{
  int a [10][10][10][10];
};


void __attribute__ ((hsa))
foo (int *a, struct S *s, int i, int j, int k, int l)
{
  *a = s->a[i][j][k][l];
}
