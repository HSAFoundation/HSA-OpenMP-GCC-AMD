void __attribute__ ((hsa))
foo (double *r, double a, double b)
{
  *r = fmodf (a, b);
}
