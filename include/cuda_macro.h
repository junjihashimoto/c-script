
#define malloc_d(ptr,size)						\
  CUDA_SAFE_CALL( cudaMalloc( (void**) &ptr, sizeof(ptr[0])*(size)));
#define free_d(ptr) \
  CUDA_SAFE_CALL( cudaFree  ( ptr))
#define cpy_h2d(from,to,size) \
  CUDA_SAFE_CALL( cudaMemcpy( (to), (from),  (size), cudaMemcpyHostToDevice) ) 
#define cpy_d2h(from,to,size) \
  CUDA_SAFE_CALL( cudaMemcpy( (to), (from),  (size), cudaMemcpyDeviceToHost) )
#define cpy_d2d(from,to,size) \
  CUDA_SAFE_CALL( cudaMemcpy( (to), (from),  (size), cudaMemcpyDeviceToDevice) )
#define call_h2d(grid,threads,func,...) {    \
  func<<< grid,threads>>>(__VA_ARGS__);  \
  CUDA_SAFE_CALL( cudaThreadSynchronize() ); \
  CUT_CHECK_ERROR("Kernel execution failed"); \
  }

const int thread_size=128;
#define SETIDX const int x=threadIdx.x+blockIdx.x*thread_size

// #define foreach(func,mem,...) {			\
//   mems_h2d(); \
//   func<<< 1,mem.s>>>(mem, ## __VA_ARGS__ );	  \
//   CUDA_SAFE_CALL( cudaThreadSynchronize() ); \
//   CUT_CHECK_ERROR("Kernel execution failed"); \
//   mems_d2h(); \
//   }

template<class T>
struct Ptr{
  T* ptr;
  Ptr(T& t){
    ptr=&t;
  }
};

#define foreach(func,mem,...) {			\
  Ptr<MemRoot> memroot[]={mem, ## __VA_ARGS__};					\
  for(int i=0;i<sizeof(memroot)/sizeof(Ptr<MemRoot>);i++) memroot[i].ptr->h2d(); \
  func<<< mem.inter_s/thread_size,thread_size>>>(mem, ## __VA_ARGS__ );	  \
  CUDA_SAFE_CALL( cudaThreadSynchronize() ); \
  CUT_CHECK_ERROR("Kernel execution failed"); \
  for(int i=0;i<sizeof(memroot)/sizeof(Ptr<MemRoot>);i++) memroot[i].ptr->d2h(); \
  }

struct MemRoot{
  virtual void h2d()=0;
  virtual void d2h()=0;
};


template<class T>
struct Mem : MemRoot{
  T*  d;
  T*  h;
  int s;
  int inter_s;
  int rflag;
  int wflag;
  
  Mem(int size,char* rw="rw"){
    s=size;
    inter_s=((size+thread_size-1)/thread_size)*thread_size;
    h=(T*)malloc(sizeof(T)*s);
    malloc_d(d,inter_s);
    rflag=index(rw,'r')!=NULL;
    wflag=index(rw,'w')!=NULL;
  }
  void h2d(){
    if(rflag)
      cpy_h2d(h,d,s*sizeof(T));
  }
  
  void d2h(){
    if(wflag)
      cpy_d2h(d,h,s*sizeof(T));
  }
  T& operator [](int x){
    return h[x];
  }
  operator T* (){
    return d;
  }
  
  ~Mem(){
    free(h);
    free_d(d);
  }
  
};

