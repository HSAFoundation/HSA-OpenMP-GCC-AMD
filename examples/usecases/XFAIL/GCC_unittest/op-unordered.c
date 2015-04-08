void __attribute__ ((hsa))
foo (int *r, double a, double b)
{
  *r = __builtin_isunordered ((a), (b)) || ((a) == (b))
    || __builtin_islessgreater ((a), (b));
}
