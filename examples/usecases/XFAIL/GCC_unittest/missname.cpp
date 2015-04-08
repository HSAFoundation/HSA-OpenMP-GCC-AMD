/* Note that this is a C++ testcase.  */

void __attribute__ ((hsa))
foo (int *a, int *b, int *)
{
  *a = *b;
}
