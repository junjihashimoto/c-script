#!/usr/bin/env c-script

#pragma c-script:libs -lfftw3

#include <fftw3.h>

int qt[8][8]=
  {{1, 11,10,16, 24, 40, 51, 61},
   {12,12,14,19, 26, 58, 60, 55},
   {14,13,16,24, 40, 57, 69, 56},
   {14,17,22,29, 51, 87, 80, 62},
   {18,22,37,56, 68,109,103, 77},
   {24,35,55,64, 81,104,113, 92},
   {49,64,78,87,103,121,120,101},
   {72,92,95,98,112,100,103, 99}};

double quality=0.01;

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
  BMP fin (argv[1]);
  BMP fout;fout.init(fin.w,fin.h);
  
  double  (*in  )[8];
  double  (*out )[8];
  double  (*out2)[8];
  fftw_plan p ;
  int i,j,idx;
 
  in  = (double (*)[8])fftw_malloc( sizeof(double) * 8*8);
  out = (double (*)[8])fftw_malloc( sizeof(double) * 8*8);
  out2= (double (*)[8])fftw_malloc( sizeof(double) * 8*8);

 
  for(int c=0;c<3;c++){
    for(int by=0;by<fin.h/8;by++)
      for(int bx=0;bx<fin.w/8;bx++){
	for(int dy=0;dy<8;dy++)
	  for(int dx=0;dx<8;dx++){
	    int x=bx*8+dx;
	    int y=by*8+dy;
	    in[   dy][   dx]=fin(x,y)[c];
	  }


	p = fftw_plan_r2r_2d( 8,8,
			      (double*)in, (double*)out,
			      FFTW_REDFT10,FFTW_REDFT10,
			      FFTW_ESTIMATE );
      
	fftw_execute(p);


	for(int dy=0;dy<8;dy++)
	  for(int dx=0;dx<8;dx++){
	    out[   dy][   dx]/=8*8*4;
	  }
      
	for(int dy=0;dy<8;dy++)
	  for(int dx=0;dx<8;dx++){
	    //	    out[   dy][   dx]=round(out[   dy][   dx]/(qt[dy][dx]*quality));
	    out[   dy][   dx]=round(out[   dy][   dx]/2);
	  }

	for(int dy=0;dy<8;dy++)
	  for(int dx=0;dx<8;dx++){
	    //	    out[   dy][   dx]*=qt[dy][dx]*quality;
	    out[   dy][   dx]*=2;
	  }

      
	// for(int dy=0;dy<8;dy++)
	//   for(int dx=0;dx<8;dx++){
	//     printf("%04d\n",(int)out[   dy][   dx]);
	//   }
      
	p = fftw_plan_r2r_2d( 8,8,
			      (double*)out, (double*)out2,
			      FFTW_REDFT01,FFTW_REDFT01,
			      FFTW_ESTIMATE );
	fftw_execute(p);

      


	for(int dy=0;dy<8;dy++)
	  for(int dx=0;dx<8;dx++){
	    int x=bx*8+dx;
	    int y=by*8+dy;
	    fout(x,y)[c]=lmt(out2[   dy][   dx]);
	  }



      }
  }

  fout.write(argv[2]);
 
  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);
  fftw_free(out2);
 
  return true;
}
