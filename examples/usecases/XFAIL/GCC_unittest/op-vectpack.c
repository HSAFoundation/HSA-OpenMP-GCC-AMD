/* -O3 required */
/* Event though this first triggers an unimplemented vector load sorry, if we
   did not stop after that it would also bump into an unimplemented gimple RHS
   operations vec_pack_trunc_expr, vec_unpack_lo_expr and
   vec_unpack_hi_expr. */


void __attribute__ ((hsa))
foo (int *isr) {
  int i;

  for (i=0; i<256; i++)
    isr[i]=(((short)((i*4) & 0xFFFF)) | (0xf000 & 0xFFFF) << 16);
}
