#!/usr/bin/env c-script

#define MAX(a,b) ((a)>=(b) ? (a) : (b))
#define MIN(a,b) ((a)< (b) ? (a) : (b))
#define MAX3(a)  MAX(MAX(a[0],a[1]),a[2])
#define MIN3(a)  MIN(MIN(a[0],a[1]),a[2])
#define LMT(a)  ((a)>255 ? 255 : (a));


#define systemf(...) do{						\
     char systemf_buffer[1024];						\
     snprintf(systemf_buffer,sizeof(systemf_buffer),__VA_ARGS__);	\
     system(systemf_buffer);						\
   }while(0)


int
lmt(int a){
  if(a>255)
    return 255;
  else if(a<0)
    return 0;
  else
    return a;
}
	   

int
main(int argc,char** argv){
  double scale  =atof(argv[1]);
  char* fin     =argv[2];
  char* fout    =argv[3];
  BMP in(fin);
  BMP out;out.init(in.w*scale,in.h*scale);

  bmp_for(out){
    int px0   =x/scale;
    int py0   =y/scale;
    int pxm2  =px0-2;
    int pym2  =py0-2;
    int pxm1  =px0-1;
    int pym1  =py0-1;
    int px1   =px0+1;
    int py1   =py0+1;
    int px2   =px0+2;
    int py2   =py0+2;
    int px3   =px0+3;
    int py3   =py0+3;

#define LIMIT(x,min,max) ((x)<(min) ? (min) : (x)>(max) ? (max) : (x))
    pxm2=LIMIT(pxm2,0,in.w-1);
    pxm1=LIMIT(pxm1,0,in.w-1);
    px0 =LIMIT(px0 ,0,in.w-1);
    px1 =LIMIT(px1 ,0,in.w-1);
    px2 =LIMIT(px2 ,0,in.w-1);
    px3 =LIMIT(px3 ,0,in.w-1);

    pym2=LIMIT(pym2,0,in.h-1);
    pym1=LIMIT(pym1,0,in.h-1);
    py0 =LIMIT(py0 ,0,in.h-1);
    py1 =LIMIT(py1 ,0,in.h-1);
    py2 =LIMIT(py2 ,0,in.h-1);
    py3 =LIMIT(py3 ,0,in.h-1);
    
    
    double ppx=x/scale-px0;
    double ppy=y/scale-py0;

#define sinc(x) (fabs(x)<0.00001 ? 1.0 : sin((x)*M_PI)/((x)*M_PI))
#define fd0(x) (sinc(x)*sinc((x)/2.0))
    double a=
      +fd0(2.0+ppx)*fd0(2.0+ppy)
      +fd0(1.0+ppx)*fd0(2.0+ppy)
      +fd0(ppx    )*fd0(2.0+ppy)
      +fd0(1.0-ppx)*fd0(2.0+ppy)
      +fd0(2.0-ppx)*fd0(2.0+ppy)
      +fd0(3.0-ppx)*fd0(2.0+ppy)
       
      +fd0(2.0+ppx)*fd0(1.0+ppy)
      +fd0(1.0+ppx)*fd0(1.0+ppy)
      +fd0(ppx    )*fd0(1.0+ppy)
      +fd0(1.0-ppx)*fd0(1.0+ppy)
      +fd0(2.0-ppx)*fd0(1.0+ppy)
      +fd0(3.0-ppx)*fd0(1.0+ppy)
       
      +fd0(2.0+ppx)*fd0(ppy)
      +fd0(1.0+ppx)*fd0(ppy)
      +fd0(ppx    )*fd0(ppy)
      +fd0(1.0-ppx)*fd0(ppy)
      +fd0(2.0-ppx)*fd0(ppy)
      +fd0(3.0-ppx)*fd0(ppy)
       
      +fd0(2.0+ppx)*fd0(1.0-ppy)
      +fd0(1.0+ppx)*fd0(1.0-ppy)
      +fd0(ppx    )*fd0(1.0-ppy)
      +fd0(1.0-ppx)*fd0(1.0-ppy)
      +fd0(2.0-ppx)*fd0(1.0-ppy)
      +fd0(3.0-ppx)*fd0(1.0-ppy)
       
      +fd0(2.0+ppx)*fd0(2.0-ppy)
      +fd0(1.0+ppx)*fd0(2.0-ppy)
      +fd0(ppx    )*fd0(2.0-ppy)
      +fd0(1.0-ppx)*fd0(2.0-ppy)
      +fd0(2.0-ppx)*fd0(2.0-ppy)
      +fd0(3.0-ppx)*fd0(2.0-ppy)
       
      +fd0(2.0+ppx)*fd0(3.0-ppy)
      +fd0(1.0+ppx)*fd0(3.0-ppy)
      +fd0(ppx    )*fd0(3.0-ppy)
      +fd0(1.0-ppx)*fd0(3.0-ppy)
      +fd0(2.0-ppx)*fd0(3.0-ppy)
      +fd0(3.0-ppx)*fd0(3.0-ppy)
      ;
    
    for(int c=0;c<3;c++)
      out(x,y)[c]=lmt((0.0
		       +in(pxm2,pym2)[c]*fd0(2.0+ppx)*fd0(2.0+ppy)
		       +in(pxm1,pym2)[c]*fd0(1.0+ppx)*fd0(2.0+ppy)
		       +in(px0 ,pym2)[c]*fd0(ppx    )*fd0(2.0+ppy)
		       +in(px1 ,pym2)[c]*fd0(1.0-ppx)*fd0(2.0+ppy)
		       +in(px2 ,pym2)[c]*fd0(2.0-ppx)*fd0(2.0+ppy)
		       +in(px3 ,pym2)[c]*fd0(3.0-ppx)*fd0(2.0+ppy)
       
		       +in(pxm2,pym1)[c]*fd0(2.0+ppx)*fd0(1.0+ppy)
		       +in(pxm1,pym1)[c]*fd0(1.0+ppx)*fd0(1.0+ppy)
		       +in(px0 ,pym1)[c]*fd0(ppx    )*fd0(1.0+ppy)
		       +in(px1 ,pym1)[c]*fd0(1.0-ppx)*fd0(1.0+ppy)
		       +in(px2 ,pym1)[c]*fd0(2.0-ppx)*fd0(1.0+ppy)
		       +in(px3 ,pym1)[c]*fd0(3.0-ppx)*fd0(1.0+ppy)
       
		       +in(pxm2,py0)[c]*fd0(2.0+ppx)*fd0(ppy)
		       +in(pxm1,py0)[c]*fd0(1.0+ppx)*fd0(ppy)
		       +in(px0 ,py0)[c]*fd0(ppx    )*fd0(ppy)
		       +in(px1 ,py0)[c]*fd0(1.0-ppx)*fd0(ppy)
		       +in(px2 ,py0)[c]*fd0(2.0-ppx)*fd0(ppy)
		       +in(px3 ,py0)[c]*fd0(3.0-ppx)*fd0(ppy)
       
		       +in(pxm2,py1)[c]*fd0(2.0+ppx)*fd0(1.0-ppy)
		       +in(pxm1,py1)[c]*fd0(1.0+ppx)*fd0(1.0-ppy)
		       +in(px0 ,py1)[c]*fd0(ppx    )*fd0(1.0-ppy)
		       +in(px1 ,py1)[c]*fd0(1.0-ppx)*fd0(1.0-ppy)
		       +in(px2 ,py1)[c]*fd0(2.0-ppx)*fd0(1.0-ppy)
		       +in(px3 ,py1)[c]*fd0(3.0-ppx)*fd0(1.0-ppy)
       
		       +in(pxm2,py2)[c]*fd0(2.0+ppx)*fd0(2.0-ppy)
		       +in(pxm1,py2)[c]*fd0(1.0+ppx)*fd0(2.0-ppy)
		       +in(px0 ,py2)[c]*fd0(ppx    )*fd0(2.0-ppy)
		       +in(px1 ,py2)[c]*fd0(1.0-ppx)*fd0(2.0-ppy)
		       +in(px2 ,py2)[c]*fd0(2.0-ppx)*fd0(2.0-ppy)
		       +in(px3 ,py2)[c]*fd0(3.0-ppx)*fd0(2.0-ppy)
       
		       +in(pxm2,py3)[c]*fd0(2.0+ppx)*fd0(3.0-ppy)
		       +in(pxm1,py3)[c]*fd0(1.0+ppx)*fd0(3.0-ppy)
		       +in(px0 ,py3)[c]*fd0(ppx    )*fd0(3.0-ppy)
		       +in(px1 ,py3)[c]*fd0(1.0-ppx)*fd0(3.0-ppy)
		       +in(px2 ,py3)[c]*fd0(2.0-ppx)*fd0(3.0-ppy)
		       +in(px3 ,py3)[c]*fd0(3.0-ppx)*fd0(3.0-ppy)
		       )/a
		      );
  }

  out.write(fout);
  return 0;
}
