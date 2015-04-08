void __attribute__ ((hsa))
bar (int *r, int a)
{
  *r = a;
}

void __attribute__ ((hsa))
foo (int *r, int a, int b)
{
  bar (r, a);
  *r += b;
}

