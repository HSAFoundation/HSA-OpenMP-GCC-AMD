/* At this moment this is really only a compile-time test but eventually should
   be a run time too. */

#include <stdio.h>

struct S
{
  int a[32];
};

void __attribute__ ((hsa))
foo (struct S *a, struct S *b)
{
  *a = *b;
}

int
main (int argc, char **argv)
{
  struct S src, dst;
  int i;

  for (i = 0; i < 32; i++)
    src.a[i] = i;
  foo (&dst, &src);
  for (i = 0; i < 32; i++)
    if (src.a[i] != dst.a[i])
      {
	fprintf (stderr, "Mismatch at %i\n", i);
	__builtin_abort ();
      }
  return 0;
}
