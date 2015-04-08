void __attribute__ ((hsa))
foo (unsigned int *r, unsigned int x)
{
  *r= (x << 1) | (x >> 31);
}
