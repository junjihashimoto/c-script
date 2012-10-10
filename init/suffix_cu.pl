$cc="nvcc";
$gdb="cuda-gdb";
$opt.=" -I/usr/local/cuda/sdk/C/common/inc";
#$opt.=" --compiler-options -fno-inline -Xcompiler -D__builtin_stdarg_start=__builtin_va_start";
#$opt.=" --compiler-bindir /usr/local/cuda/gcc.bin";
$libs.=" -L/usr/local/cuda/sdk/C/lib -lcutil_x86_64";
$default_header=qq(
#include <cutil.h>
#include <cuda_macro.h>
);

