#!/usr/bin/env c-script

#pragma c-script:libs -lfftw3

#include <fftw3.h>

#define N 8

void 
mprint(double in[N][N]){
  for(int y=0;y<8;y++){
    for(int x=0;x<8;x++)
      printf("%4d,",(int)in[y][x]);
    printf("\n");
  }
  printf("\n");
}

#define mforeach(in,FUNC) \
  for(int y=0;y<8;y++) \
    for(int x=0;x<8;x++){ \
      (FUNC(in[y][x])); \
    }

#define mmap(in,FUNC) \
  for(int y=0;y<8;y++) \
    for(int x=0;x<8;x++){ \
      in[y][x]=(FUNC(in[y][x])); \
    }


void 
mzero(double in[N][N]){
  for(int y=0;y<N;y++)
    for(int x=0;x<N;x++)
      in[y][x]=0;
}

void 
mmul(double in[N][N],double a){
  for(int y=0;y<N;y++)
    for(int x=0;x<N;x++)
      in[y][x]*=a;
}

double
mmax(double in[N][N]){
  double a=0;
  for(int y=0;y<N;y++)
    for(int x=0;x<N;x++){
      if(y==0&&x==0)
	continue;
      double b=fabs(in[y][x]);
      if(b>a)
	a=b;
    }
  return a;
}


void 
dct(double in[N][N],double out[N][N]){
  mzero(out);

  for(int y=0;y<N;y++)
    for(int x=0;x<N;x++)
      for(int iy=0;iy<N;iy++)
	for(int ix=0;ix<N;ix++)
	  out[y][x]+=
	    (x==0?1/(sqrt(2)):1)*
	    (y==0?1/(sqrt(2)):1)*
	    cos((2*ix+1)*x*M_PI/(2*N))*
	    cos((2*iy+1)*y*M_PI/(2*N))*
	    in[iy][ix]*
	    (2.0/N);
}

void 
idct(double in[N][N],double out[N][N]){
  mzero(out);

  for(int y=0;y<N;y++)
    for(int x=0;x<N;x++)
      for(int iy=0;iy<N;iy++)
	for(int ix=0;ix<N;ix++)
	  out[y][x]+=
	    (ix==0?1/(sqrt(2)):1)*
	    (iy==0?1/(sqrt(2)):1)*
	    cos((2*x+1)*ix*M_PI/(2*N))*
	    cos((2*y+1)*iy*M_PI/(2*N))*
	    in[iy][ix]*
	    (2.0/N);
}

 
 
int
main(){
  double out[8][8];
  double out2[8][8];
  double out3[8][8];
  double in[8][8];
  double in2[8][8];

  
  for(int y=0;y<N;y++)
    for(int x=0;x<N;x++){
      mzero(in);
      in[y][x]=1;
      idct(in,out);
  
      for(int iy=0;iy<N;iy++)
	for(int ix=0;ix<N;ix++){
	  if(out[iy][ix]>=0)
	    in2[iy][ix]=255;
	  else
	    in2[iy][ix]=0;
	}

      dct(in2,out2);

      if(x==0&&y==0)
	out3[y][x]=out2[0][0];
      else
	out3[y][x]=mmax(out2);
    }

  // mmap(out3,(1.0/4)*);
  mmap(out3,round);
  mprint(out3);
  

  return true;
}
