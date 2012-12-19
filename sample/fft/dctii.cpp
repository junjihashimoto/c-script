#!/usr/bin/env c-script

#pragma c-script:libs -lfftw3

#include <fftw3.h>

#define N 8

void 
dct(double in[N][N],double out[N][N]){
  for(int y=0;y<N;y++)
    for(int x=0;x<N;x++)
      out[y][x]=0;

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
  for(int y=0;y<N;y++)
    for(int x=0;x<N;x++)
      out[y][x]=0;

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

int qt[8][8]=
  {{16,11,10,16, 24, 40, 51, 61},
   {12,12,14,19, 26, 58, 60, 55},
   {14,13,16,24, 40, 57, 69, 56},
   {14,17,22,29, 51, 87, 80, 62},
   {18,22,37,56, 68,109,103, 77},
   {24,35,55,64, 81,104,113, 92},
   {49,64,78,87,103,121,120,101},
   {72,92,95,98,112,100,103, 99}};
 
double quality=0.75;
 
int
main(){
  fftw_plan p ;
  double out[8][8];
  double out2[8][8];
  double in[8][8]={
    {126, 138, 135, 118, 118, 126, 126, 130},
    {150, 168, 161, 122, 105, 109, 100, 118},
    {150, 150, 126, 150, 142, 126, 126, 117},
    {150, 161, 168, 130, 134, 150, 138, 130},
    {130, 118, 134, 142, 157, 142, 117, 126},
    {115, 117, 108, 117, 101,  99, 117, 126},
    {122, 130, 130, 138, 117, 108, 108, 138},
    {142, 118, 134, 117, 109,  91, 126, 109}
  };
  
  double in2[8][8]={
    {255,   0, 255,   0, 255,   0, 255,   0},
    {  0, 255,   0, 255,   0, 255,   0, 255},
    {255,   0, 255,   0, 255,   0, 255,   0},
    {  0, 255,   0, 255,   0, 255,   0, 255},
    {255,   0, 255,   0, 255,   0, 255,   0},
    {  0, 255,   0, 255,   0, 255,   0, 255},
    {255,   0, 255,   0, 255,   0, 255,   0},
    {  0, 255,   0, 255,   0, 255,   0, 255}
  };
  
 
  p = fftw_plan_r2r_2d(8,8,(double*)in,(double*)out2,FFTW_REDFT10,FFTW_REDFT10,FFTW_ESTIMATE );
  fftw_execute(p);
 
  // fftw_execute(p);
  for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
      in[y][x]=256*(double)rand()/(double)RAND_MAX;
      printf("%4d,",(int)(in[y][x]));
    }
    printf("\n");
  }
  printf("\n");
  
  dct(in,out);
  for(int y=0;y<8;y++){
    for(int x=0;x<8;x++)
      printf("%4d,",(int)(out[y][x]/(qt[y][x]*quality)));
    printf("\n");
  }
  printf("\n");
  
  dct(in,out);
  for(int y=0;y<8;y++){
    for(int x=0;x<8;x++)
      printf("%f,",(out2[y][x]/out[y][x]));
    printf("\n");
  }
  printf("\n");
  
  // idct(out,out2);
  
  // // for(int y=0;y<8;y++){
  // //   for(int x=0;x<8;x++)
  // //     out[y][y]*=1/8;
  // // 	// (x==0?sqrt(0.5):1.0)*
  // // 	// (y==0?sqrt(0.5):1.0)
  // // 	// /2.0;
  // // }

  // for(int y=0;y<8;y++){
  //   for(int x=0;x<8;x++)
  //     printf("%4d,",(int)(out2[y][x]));
  //   printf("\n");
  // }
  
  fftw_destroy_plan(p);

  return true;
}
