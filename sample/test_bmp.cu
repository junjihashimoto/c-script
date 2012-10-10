#!/usr/bin/env c-script


#include <bmp.h>

struct img{
  int w;
  int h;
  int* d;
  __device__ __host__ img(){
    w=0;
    h=0;
    d=NULL;
  }
  
  __device__ __host__
  int&        operator () (int x,int y,int z){
    if(x>=w)
      x=w-1;
    else if(x<0)
      x=0;
    if(y>=h)
      y=h-1;
    else if(y<0)
      y=0;
    
    return d[(y*w+x)*3+z];
  }
  __device__ __host__
  const int&  operator () (int x,int y,int z)const{
    if(x>=w)
      x=w-1;
    else if(x<0)
      x=0;
    if(y>=h)
      y=h-1;
    else if(y<0)
      y=0;
    return d[(y*w+x)*3+z];
  }
};

void
copy(const BMP& bm,img& im){
  im.w=bm.w;
  im.h=bm.h;
  malloc_d(im.d,im.w*im.h*3*sizeof(int));
  cpy_h2d(bm.rgb,im.d,im.w*im.h*3*sizeof(int));
}
void
copy(const img& im,BMP& bm){
  bm.init(im.w,im.h);
  cpy_d2h(im.d,bm.rgb,im.w*im.h*3*sizeof(int));
}
void
copy(const img& im,img& im2){
  im2=im;
  malloc_d(im2.d,im.w*im.h*3*sizeof(int));
  cpy_d2d(im.d,im2.d,im.w*im.h*3*sizeof(int));
}


void
init(const img& im,img& im2){
  im2=im;
  malloc_d(im2.d,im.w*im.h*3*sizeof(int));
}

#define xl(in,x)  ((x)<0 ? 0 :( ((x)>=(in).w) ? (in).w-1:(x)))
#define yl(in,y)  ((y)<0 ? 0 :( ((y)>=(in).h) ? (in).h-1:(y)))
#define val(in,x,y,z) (in.d[((yl((in),(y)))*in.w+(xl((in),(x))))*3+(z)])

__global__
void
filter(img in,img out){
  int idx=blockIdx.x * blockDim.x + threadIdx.x;
  int x=idx%in.w;
  int y=idx/in.w;
  if(idx>in.w*in.h)
    return;
  for(int k=0;k<3;k++){
    int v=
      -in(x,y-1,k)
      -in(x-1,y,k)+in(x,y,k)*5-in(x+1,y,k)+
      -in(x,y+1,k);
    if(v>255)
      v=255;
    else if(v<0)
      v=0;
    out(x,y,k)=v;
  }
      
}


int
main(int argc,char** argv){
  BMP bin(argv[1]);
  BMP bout;

  img in;
  img out;

  copy(bin,in);
  init(in,out);

  
  int th=256;

  
  filter<<<(in.w*in.h+th-1)/th,th>>>(in,out);
  // filter(in,out);
  
  copy(out,bout);
  
  bout.write(argv[2]);

  free_d(in.d);
  free_d(out.d);
  
  return 0;
}

