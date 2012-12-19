#!/usr/bin/env c-script

#pragma c-script:libs -lfftw3

#include <fftw3.h>
 
int
main(int argc,char** argv){
  BMP fin(argv[1]);
  
  double  *in ;
  double  *out;
  fftw_plan p ;
  int i,j,idx;
 
  in  = (double *)fftw_malloc( sizeof(double) * fin.w*fin.h*4);
  out = (double *)fftw_malloc( sizeof(double) * fin.w*fin.h*4);
 
  // !! row-major alignment is recommended, but here, column-major.
  bmp_for(fin){
    in[y            *fin.w*2+x            ]=(fin(x,y)[0]+fin(x,y)[1]*2+fin(x,y)[2])/4.0;
    in[y            *fin.w*2+(fin.w*2-1-x)]=in[y*fin.w*2+x];
    in[(fin.h*2-1-y)*fin.w*2+x            ]=in[y*fin.w*2+x];
    in[(fin.h*2-1-y)*fin.w*2+(fin.w*2-1-x)]=in[y*fin.w*2+x];
  }

  p = fftw_plan_r2r_2d( fin.h, fin.w,
			in, out,
			FFTW_REDFT10,FFTW_REDFT10,
			FFTW_ESTIMATE );
 
  
  fftw_execute(p);
 
  double scale = 1./(fin.w*fin.h*4);

  for(int y=0;y<fin.h*2;y++){
    for(int x=0;x<fin.w*2;x++)
      printf("%d %d %lf\n", y,x, fabs(out[y*fin.w*2+x]*scale));
    printf("\n");
  }

 
  // p = fftw_plan_r2r_2d( fin.h, fin.w,
  // 			out,in,
  // 			FFTW_REDFT01,FFTW_REDFT01,
  // 			FFTW_ESTIMATE );
  // fftw_execute(p);
 
  if( p   ) fftw_destroy_plan(p);
  if( in  ) fftw_free(in);
  if( out ) fftw_free(out);
 
  return true;
}
