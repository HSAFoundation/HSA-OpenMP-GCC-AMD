void __attribute__ ((hsa))
foo (int *r, int a)
{
  int i;
  if (a < 5)
    i = 8;
  else if (i < 8)
    i = 10;
  else if (i < 12)
    i = 12;
  else if (i < 14)
    i = 14;
  else if (i < 18)
    i = 16;
  else if (i < 28)
    i = 18;
  else if (i < 38)
    i = 22;
  else
    i  = 0;

  *r = i + 5;
}
