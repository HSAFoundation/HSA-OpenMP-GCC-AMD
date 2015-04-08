#include "launchKernel.h"
#include "okra.h"
#include "string.h"
#include <dlfcn.h>
#include "utils.h"
/* #include "hsail_c.h" */

typedef okra_status_t (*okra_get_context_func_t)(okra_context_t**);
typedef okra_status_t (*okra_kernel_create_func_t)( okra_context_t* ,const char *, const char *,okra_kernel_t **);
typedef okra_status_t (*okra_push_pointer_func_t)(okra_kernel_t* , void* );
typedef okra_status_t (*okra_execute_kernel_func_t)(okra_context_t* ,okra_kernel_t* , okra_range_t* );
typedef okra_status_t (*okra_clear_args_func_t)(okra_kernel_t* );

okra_get_context_func_t      _okra_get_context;
okra_kernel_create_func_t   _okra_kernel_create;
okra_push_pointer_func_t    _okra_push_pointer;
okra_execute_kernel_func_t    _okra_execute_kernel;
okra_clear_args_func_t       _okra_clear_args;

static void* okraLib = NULL;
static okra_context_t* context = NULL;
static okra_kernel_t* kernel = NULL;
static const char* lastRunKernel = NULL;


/* Entry point from gcc to Okra layer */
__attribute__((constructor))
static char* okra_init() {
  okra_status_t status;
  if (okraLib == NULL) {
     okraLib = dlopen("libokra_x86_64.so", RTLD_LAZY);
     if (okraLib == NULL) {
        printf( "...unable to load libokra_x86_64.so\n");
        return NULL;
     }
     _okra_get_context         = (okra_get_context_func_t)dlsym(okraLib, "okra_get_context");
     _okra_kernel_create       = (okra_kernel_create_func_t)dlsym(okraLib, "okra_create_kernel");
     _okra_push_pointer        = (okra_push_pointer_func_t)dlsym(okraLib, "okra_push_pointer");
     _okra_execute_kernel      = (okra_execute_kernel_func_t)dlsym(okraLib, "okra_execute_kernel");
     _okra_clear_args          = (okra_clear_args_func_t)dlsym(okraLib, "okra_clear_args");
   }
   if (context == NULL) { 
     status = _okra_get_context(&context);
     if (status != OKRA_SUCCESS) {
        printf( "...unable to create context\n");
        return NULL;
     }
  }
}

void *
__hsa_launch_kernel (__hsa_kernel_desc * _kd, __hsa_launch_attrs *attrs, __hsa_kernelarg *args)
{
  okra_status_t status;
  okra_range_t range;
  size_t size = 0;
  unsigned int i;
  const char* fileName = _kd->filename;
  if (_kd->filename[0] == 0)
    fileName = "hsakernel.hsail"; 

/*  
  brig_container_t handle = brig_container_create_empty();
  brig_container_load_from_file(handle, "./hsakernel.brig");
  printf ("error in load from file %s",  brig_container_get_error_text(handle));
  //brig_container_disassemble_to_file(handle, "./temp.hsail");
  brig_container_save_to_file(handle, "./temp.hsail");
  printf ("error in disass file %s",  brig_container_get_error_text(handle));
*/

  if (kernel == NULL || strcmp (lastRunKernel, _kd->name) != 0 ) {
     char* hsailStr= buildStringFromSourceFile(fileName);
     status = _okra_kernel_create(context, hsailStr, _kd->name, &kernel);
     if (status != OKRA_SUCCESS) {
        printf( "...unable to create Kernel \n");
       return NULL;
     }
     lastRunKernel = _kd->name;
  }
     
  _okra_clear_args(kernel);


  for (i = 0; i < _kd->nargs; i++)
    {
      void *cur_args = args[i].addr;
      _okra_push_pointer(kernel, cur_args);
    }

/* Set launc dimensions */
  range.dimension = 1;
  range.global_size[0] = 256;
  range.group_size[0] = 16;

  status = _okra_execute_kernel(context, kernel, &range);
  if (status != OKRA_SUCCESS) {
     printf( "Failed to launch kernel \n");
     return NULL;
  }
  return kernel; 
}

