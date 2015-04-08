typedef _Complex int C;

void __attribute__ ((hsa))
foo (C *a, C b)
{
  *a = b;
}
