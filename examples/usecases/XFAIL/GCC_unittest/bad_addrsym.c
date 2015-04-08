int bar(int a)
{
  return 0;
}

void __attribute__ ((hsa))
foo (int (**a)(int))
{
  *a = &bar;
}
