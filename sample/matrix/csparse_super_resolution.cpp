#!/usr/bin/env c-script

#pragma c-script:opt -I/usr/include/suitesparse
#pragma c-script:libs -lcsparse
#pragma c-script:libs -lcxsparse

#include <cs.h>

#define set(A,xd,yd,v)  cs_entry (A,((x)+(y)*w),((xd)+(yd)*w), (v)) ;

int
limit(double v){
  return
        v<0 ? 0 :
	v> 255 ? 255 : v;
}




cs*
mkcoeff(const BMP& in){
  cs* AR;
  cs* A;
  int w=in.w;
  int h=in.h;
  int n=in.h*in.w;
  A=cs_spalloc(n,n,n*5,1,1);

  for(int y=0;y<in.h;y++)
    for(int x=0;x<in.w;x++){
      if(y==0||x==0||x==in.w-1||y==in.h-1){
	set(A, x, y, 1.0);
      }else{
	set(A, x, y-1, 1.0/6);
	set(A, x-1, y, 1.0/6);
	set(A, x,   y, 2.0/6);
	set(A, x+1, y, 1.0/6);
	set(A, x, y+1, 1.0/6);
      }
    }
  AR = cs_compress (A) ;             /* Eye = speye (m) */
  cs_spfree(A);

  return AR;
}

cs*
mkeyec(const BMP& in,double c){
  cs* AR;
  cs* A;
  int w=in.w;
  int h=in.h;
  int n=in.h*in.w;
  A=cs_spalloc(n,n,n*5,1,1);

  for(int y=0;y<in.h;y++)
    for(int x=0;x<in.w;x++)
      set(A, x, y, c);
  AR = cs_compress (A) ;             /* Eye = speye (m) */
  cs_spfree(A);

  return AR;
}


int
main(int argc,char** argv){
  BMP in2(argv[1]);
  BMP in(128,128);

  // bmp_for(in)
  //   in(x,y)=in2(x+500,y+200);
  bmp_for(in)
    in(x,y)=in2(x,y);
  in.write("in.png");


  BMP out=in;
  int w=in.w;
  int h=in.h;
  int n=in.h*in.w;

  

  for(int y=1;y<in.h-1;y++)
    for(int x=1;x<in.w-1;x++)
      for(int z=0;z<3;z++)
	out(x,y,z)=
	  (in(x,y-1,z)+
	   in(x-1,y,z)+in(x,y,z)*2+in(x+1,y,z)+
	   in(x,y+1,z))/6;

  out.write("low.png");


  //min  |ax-b|+c|x|
  //min   xtatax-2xtatb+btb+cxtx
  //min   xt(ata+cE)x-2xtatb+btb
  //    (ata+cE)x=atb
  double* img= (double*)malloc(sizeof(double)*n);
  double* rhs= (double*)malloc(sizeof(double)*n);

  cs* A      = mkcoeff(out);
  cs* AT     = cs_transpose (A, 1) ;          /* AT = A' */
  cs* ATA    = cs_multiply (AT, A) ;
  double c   = 0.01;
  cs* EyeC   = mkeyec(out,c);
  cs* ATA_EyeC = cs_add(ATA,EyeC,1.0,1.0);

  cs_spfree(ATA);
  cs_spfree(EyeC);
  
  for(int k=0;k<3;k++){

    for(int i=0;i<n;i++)
      img[i]=out(i%in.w,i/in.w,k);
  
    for(int i=0;i<n;i++)
      rhs[i]=0;

    cs_gaxpy(AT,img,rhs);
  
    cs_cholsol (0, ATA_EyeC,rhs);
    for(int i=0;i<n;i++)
      out(i%in.w,i/in.w,k)=limit(rhs[i]);
  }
  out.write("restore.png");
  return 0;
}

