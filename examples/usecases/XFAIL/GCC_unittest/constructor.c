/* -O3 required  */

struct a
{
  struct a * x;
};

void __attribute__ ((hsa))
bar (struct a * b)
{
  int i;

  for (i = 0; i < 1000; i++)
    {
      b->x = b;
      b--;
    }
}
