#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union __hsa_kernelarg_
{
    void* addr;             ///< pointer to a buffer
    int32_t s32value;       ///< signed 32 bit value
    uint32_t u32value;      ///< unsigned 32 bit value
    float fvalue;           ///< float value
    double dvalue;          ///< double value
    int64_t s64value;       ///< signed 64 bit value
    uint64_t u64value;      ///< unsigned 64 bit value
} __hsa_kernelarg;

typedef struct __hsa_launch_attrs_
{
  uint64_t flags;
  uint64_t grid[3];
  uint64_t group[3];
} __hsa_launch_attrs;

typedef struct __hsa_kernel_desc_
{
  const char *filename;
  const char *name;
  uint64_t nargs;
  void *kernel;
} __hsa_kernel_desc;

void * __hsa_launch_kernel (__hsa_kernel_desc *, __hsa_launch_attrs *attrs, __hsa_kernelarg *args);

#ifdef __cplusplus
}
#endif
