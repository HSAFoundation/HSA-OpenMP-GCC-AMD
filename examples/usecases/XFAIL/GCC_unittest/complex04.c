_Complex float b;

void __attribute__ ((hsa))
foo (void)
{
  _Complex float a = __FLT_MAX__;
  b = __FLT_MAX__ + a;
}
