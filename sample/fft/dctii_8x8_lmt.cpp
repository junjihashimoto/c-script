#!/usr/bin/env c-script


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



double qt[8][8]=
  {{16,11,10,16, 24, 40, 51, 61},
   {12,12,14,19, 26, 58, 60, 55},
   {14,13,16,24, 40, 57, 69, 56},
   {14,17,22,29, 51, 87, 80, 62},
   {18,22,37,56, 68,109,103, 77},
   {24,35,55,64, 81,104,113, 92},
   {49,64,78,87,103,121,120,101},
   {72,92,95,98,112,100,103, 99}};

double blmt2[8][8]={
  {9,6,6,5,5,4,4,3},
  {6,6,5,5,5,3,3,3},
  {6,6,5,5,4,3,3,3},
  {6,5,5,4,4,3,3,3},
  {5,5,4,4,3,3,3,3},
  {5,4,3,3,3,3,2,3},
  {4,3,3,3,3,2,2,3},
  {3,3,3,3,3,3,3,3}
};

double blmt[8][8]={
  {9,6,6,5,5,4,4,0},
  {6,6,5,5,5,3,3,0},
  {6,6,5,5,4,3,3,0},
  {6,5,5,4,4,3,3,3},
  {5,5,4,4,3,3,3,3},
  {5,4,3,3,3,3,2,3},
  {4,3,3,3,3,2,2,3},
  {3,3,3,3,3,3,3,3}
};


double quality=0.5;

int
lmt(int a){
  if(a>255)
    return 255;
  else if(a<0)
    return 0;
  else
    return a;
}
	   
double
mlmt(double a,int bit){
  if(bit==0)
    return 0;
  int v=1<<(bit-1);
  int max=v-1;
  int min=-v;
  if(a >max)
    return max;
  else if(a<min)
    return min;
  else
    return a;
}
	   
 
int
main(int argc,char** argv){
  BMP fin (argv[1]);
  BMP fout;fout.init(fin.w,fin.h);
  
  double in[8][8];
  double out[8][8];
  double out2[8][8];
  int i,j,idx;
 
 
  for(int c=0;c<3;c++){
    for(int by=0;by<fin.h/8;by++)
      for(int bx=0;bx<fin.w/8;bx++){
	for(int dy=0;dy<8;dy++)
	  for(int dx=0;dx<8;dx++){
	    int x=bx*8+dx;
	    int y=by*8+dy;
	    in[   dy][   dx]=fin(x,y)[c];
	  }


	dct(in,out);
	mmap(out,(1.0/(qt[y][x]*quality))*);
	
	for(int y=0;y<8;y++)
	  for(int x=0;x<8;x++){
	    out[y][x]=mlmt(out[y][x],blmt[y][x]);
	  }
	mmap(out,round);
	mmap(out,(qt[y][x]*quality)*);
	idct(out,out2);


	for(int dy=0;dy<8;dy++)
	  for(int dx=0;dx<8;dx++){
	    int x=bx*8+dx;
	    int y=by*8+dy;
	    fout(x,y)[c]=lmt(out2[   dy][   dx]);
	  }



      }
  }

  fout.write(argv[2]);
 
  return true;
}
