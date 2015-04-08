typedef _Complex int C;

void __attribute__ ((hsa))
foo (int *a, C *b)
{
  *a = __real__ *b;
}

