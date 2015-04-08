void __attribute__ ((hsa))
foo (int *r, int a, int b, int c)
{
  switch (c)
    {
    case 1:
      *r = a + b;
      break;
    case 2:
      *r = 2 * a + b;
      break;
    default:
      *r = 8;
    }
}
