void __attribute__ ((hsa))
foo (int *r)
{
  *r = 5;
  asm volatile ("" : : : "memory");
}
