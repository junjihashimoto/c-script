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
    int pxm1   =px0-1;
    int pym1   =py0-1;
    int px1   =px0+1;
    int py1   =py0+1;
    int px2   =px0+2;
    int py2   =py0+2;

    if(pxm1<0)
      pxm1=0;
    
    if(pym1<0)
      pym1=0;
    
    if(px1>=in.w)
      px1=in.w-1;
    
    if(py1>=in.h)
      py1=in.h-1;
    
    if(px2>=in.w)
      px2=in.w-1;
    
    if(py2>=in.h)
      py2=in.h-1;
    
    
    double ppx=x/scale-px0;
    double ppy=y/scale-py0;
    
#define fd0(x) (1.0        -2.0*(x)*(x)+(x)*(x)*(x))
#define fd1(x) (4.0-8.0*(x)+5.0*(x)*(x)-(x)*(x)*(x))
    double a=
      +fd1(1.0+ppx)*fd1(1.0+ppy)
      +fd0(ppx    )*fd1(1.0+ppy)
      +fd0(1.0-ppx)*fd1(1.0+ppy)
      +fd1(2.0-ppx)*fd1(1.0+ppy)
       
      +fd1(1.0+ppx)*fd0(    ppy)
      +fd0(ppx    )*fd0(    ppy)
      +fd0(1.0-ppx)*fd0(    ppy)
      +fd1(2.0-ppx)*fd0(    ppy)
       
      +fd1(1.0+ppx)*fd0(1.0-ppy)
      +fd0(ppx    )*fd0(1.0-ppy)
      +fd0(1.0-ppx)*fd0(1.0-ppy)
      +fd1(2.0-ppx)*fd0(1.0-ppy)
       
      +fd1(1.0+ppx)*fd1(2.0-ppy)
      +fd0(ppx    )*fd1(2.0-ppy)
      +fd0(1.0-ppx)*fd1(2.0-ppy)
      +fd1(2.0-ppx)*fd1(2.0-ppy);
    
    
    for(int c=0;c<3;c++)
      out(x,y)[c]=lmt((0.0
		       +in(pxm1,pym1)[c]*fd1(1.0+ppx)*fd1(1.0+ppy)
		       +in(px0 ,pym1)[c]*fd0(ppx    )*fd1(1.0+ppy)
		       +in(px1 ,pym1)[c]*fd0(1.0-ppx)*fd1(1.0+ppy)
		       +in(px2 ,pym1)[c]*fd1(2.0-ppx)*fd1(1.0+ppy)
		      
		       +in(pxm1,py0 )[c]*fd1(1.0+ppx)*fd0(    ppy)
		       +in(px0 ,py0 )[c]*fd0(ppx    )*fd0(    ppy)
		       +in(px1 ,py0 )[c]*fd0(1.0-ppx)*fd0(    ppy)
		       +in(px2 ,py0 )[c]*fd1(2.0-ppx)*fd0(    ppy)
		      
		       +in(pxm1,py1 )[c]*fd1(1.0+ppx)*fd0(1.0-ppy)
		       +in(px0 ,py1 )[c]*fd0(ppx    )*fd0(1.0-ppy)
		       +in(px1 ,py1 )[c]*fd0(1.0-ppx)*fd0(1.0-ppy)
		       +in(px2 ,py1 )[c]*fd1(2.0-ppx)*fd0(1.0-ppy)
		      
		       +in(pxm1,py2 )[c]*fd1(1.0+ppx)*fd1(2.0-ppy)
		       +in(px0 ,py2 )[c]*fd0(ppx    )*fd1(2.0-ppy)
		       +in(px1 ,py2 )[c]*fd0(1.0-ppx)*fd1(2.0-ppy)
		       +in(px2 ,py2 )[c]*fd1(2.0-ppx)*fd1(2.0-ppy)
		       )/a
		      );
  }

  out.write(fout);
  return 0;
}
