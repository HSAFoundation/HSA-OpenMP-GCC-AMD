union {
  char *p;
  double d;
} u;

void __attribute__ ((hsa))
f (void)
{
  u.p = "";
  u.d += 1.1;
}
