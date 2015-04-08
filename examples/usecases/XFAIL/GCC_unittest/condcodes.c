#define TST(foo,func) void __attribute__ ((hsa))	\
foo(double x, double y, int *r) \
{ \
  if (func(x, y)) \
    *r = true; \
  else \
    *r = false;\
} \

TST(foo1,__builtin_isunordered)
TST(foo2,__builtin_isless)
TST(foo3,__builtin_islessequal)
TST(foo4,__builtin_isgreater)
TST(foo5,__builtin_isgreaterequal)
TST(foo6,__builtin_islessgreater)
