/* -O3 required */

void __attribute__ ((hsa))
bug (unsigned short value, unsigned short *buffer, unsigned short *bufend, unsigned *r)
{
  unsigned short *tmp;

  for (tmp = buffer; tmp < bufend; tmp++)
    value -= *tmp;

  *r = value;
}
