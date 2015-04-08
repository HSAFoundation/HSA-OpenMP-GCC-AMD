/* -O3 required */

struct st {char a, b, c, d; };

void __attribute__ ((hsa))
zloop (struct st *s, int *p, int *q)
{
  int i;

  for (i = 0;  i < 100;  i++)
    {
      p[i] = s[i].c;
      q[i] = s[i].b;
    }
}
