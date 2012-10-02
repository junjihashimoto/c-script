#!/usr/bin/env c-script

#pragma c-script:libs -lfftw3

#include <fftw3.h>
 
 
int
main(){
  double (*in )[8];
  double (*out)[8];
  fftw_plan p       = NULL;
  int i,j,idx;
 
  in  = (double(*)[8])fftw_malloc( 8*8*sizeof(double));
  out = (double(*)[8])fftw_malloc( 8*8*sizeof(double));
  
  for(int y=0;y<8;y++)
    for(int x=0;x<8;x++)
      in[y][x]=0;
  
 
  p = fftw_plan_r2r_2d( 8,8, in, out,FFTW_REDFT10 , FFTW_ESTIMATE );
 
  fftw_execute(p);

  for(int i=0;i<4;i++)
    printf("out %d %f\n",i,out[i]/8);

  // p = fftw_plan_r2r_1d( 4, out, in,FFTW_REDFT01 , FFTW_ESTIMATE );
 
  // fftw_execute(p);

  // for(int i=0;i<4;i++)
  //   printf("out %d %f\n",i,in[i]);

 
  if( p   ) fftw_destroy_plan(p);
  if( in  ) fftw_free(in);
  if( out ) fftw_free(out);
 
  return true;
}
