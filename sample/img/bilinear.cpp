#!/usr/bin/env c-script

#define MAX(a,b) ((a)>=(b) ? (a) : (b))
#define MIN(a,b) ((a)< (b) ? (a) : (b))
#define MAX3(a)  MAX(MAX(a[0],a[1]),a[2])
#define MIN3(a)  MIN(MIN(a[0],a[1]),a[2])
#define LMT(a)  ((a)>255 ? 255 : (a));


#define systemf(...) do{ \
 char systemf_buffer[1024]; \
 snprintf(systemf_buffer,sizeof(systemf_buffer),__VA_ARGS__); \
 system(systemf_buffer); \
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
    int px1   =px0+1;
    int py1   =py0+1;

    if(px1>=in.w)
      px1=in.w-1;
    
    if(py1>=in.h)
      py1=in.h-1;
    
    
    double ppx=x/scale-px0;
    double ppy=y/scale-py0;
    
    for(int c=0;c<3;c++)
      out(x,y)[c]=lmt(
		      (in(px0,py0)[c] *(1.0-ppx)*(1.0-ppy)
		       +in(px1,py0)[c]*ppx*(1.0-ppy)
		       +in(px0,py1)[c]*(1.0-ppx)*ppy
		       +in(px1,py1)[c]*ppx*ppy
		      )
		      );
  }

  out.write(fout);
  return 0;
}
