#!/usr/bin/env c-script


__global__
void
test(int *a,int *b){
  b[threadIdx.x]=a[threadIdx.x]*2;
} 

__global__
void
test2(int *a){
  a[threadIdx.x]*=2;
}

int
main(int argc,char** argv){
  //CUT_DEVICE_INIT(argc,argv);
  
  Mem<int> mem(12,"r");
  Mem<int> mem2(12);
  int a[1024];
  int b[1024];
  
  int i;
  for(i=0;i<mem.s;i++)
    mem[i]=i;

  for(i=0;i<sizeof(a)/sizeof(int);i++)
    a[i]=i;
  memcpy(b,a,sizeof(a));

  foreach(test,mem,mem2);
  foreach(test2,mem2);
  
  for(i=0;i<mem2.s;i++)
    printf("%d\n",mem2[i]);
  
  //  CUT_EXIT(argc,argv);
  return 0;
}

