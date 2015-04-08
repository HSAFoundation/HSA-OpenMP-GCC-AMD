/* Shows different problems depending on optimization level. */

typedef _Complex int C;

void __attribute__ ((hsa))
foo (C *a, int i)
{
  *a = i + 8I;
}

