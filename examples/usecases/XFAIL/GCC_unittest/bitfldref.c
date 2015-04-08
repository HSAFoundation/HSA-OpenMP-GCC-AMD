struct S
{
  unsigned i1 : 5;
  unsigned i2 : 15;
};

void __attribute__ ((hsa))
foo (unsigned *a, struct S *b)
{
  *a = b->i2;
}

