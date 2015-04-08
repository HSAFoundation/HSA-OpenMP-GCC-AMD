/* -O1 required to test the intended problem.  */

void __attribute__ ((hsa))
foo (char *r, int a)
{
  char *p;

  if (a < 5)
    p  = "small stuff";
  else
    p = "big stuff";

  *r = p[3];
}
