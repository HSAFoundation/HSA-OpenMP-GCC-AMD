/* -O3 required */

void __attribute__ ((hsa))
f1()
{
  long bit=0, exponent;
  exponent = -exponent;
  for (bit = 1; exponent; bit <<= 1)
      if (exponent & bit)
              exponent ^= bit;
}
