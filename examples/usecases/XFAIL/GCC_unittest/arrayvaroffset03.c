struct S
{
  struct A
  {
    struct B
    {
      struct C
      {
	int d[10];
      } c[10];
    } b[10];
  } a [10];
};


void __attribute__ ((hsa))
foo (int *a, int *b, struct S *s, int i, int j, int k, int l)
{
  s->a[i].b[j].c[k].d[l] = *b;
  *a = s->a[i].b[j].c[k].d[l];
}
