#!/usr/bin/env c-script


#define NX 64
#define NY 64

void 
mprint(double in[NY][NX]){
  for(int y=0;y<NY;y++){
    for(int x=0;x<NX;x++)
      printf("%4d,",(int)in[y][x]);
    printf("\n");
  }
  printf("\n");
   }

void 
mprint(int in[NY][NX]){
  for(int y=0;y<NY;y++){
    for(int x=0;x<NX;x++)
      printf("%4d,",(int)in[y][x]);
    printf("\n");
  }
  printf("\n");
}

#define mforeach(in,FUNC)			\
  for(int y=0;y<NY;y++)				\
    for(int x=0;x<NX;x++){			\
      (FUNC(in[y][x]));				\
    }

#define mmap(in,FUNC)				\
  for(int y=0;y<NY;y++)				\
    for(int x=0;x<NX;x++){			\
      in[y][x]=(FUNC(in[y][x]));		\
    }


void 
mzero(double in[NY][NX]){
  for(int y=0;y<NY;y++)
    for(int x=0;x<NX;x++)
      in[y][x]=0;
}

void 
mzero(int in[NY][NX]){
  for(int y=0;y<NY;y++)
    for(int x=0;x<NX;x++)
      in[y][x]=0;
}

void 
mone(int in[NY][NX]){
  for(int y=0;y<NY;y++)
    for(int x=0;x<NX;x++)
      in[y][x]=1;
}

void 
mmul(double in[NY][NX],double a){
  for(int y=0;y<NY;y++)
    for(int x=0;x<NX;x++)
      in[y][x]*=a;
}

double
mmax(double in[NY][NX]){
  double a=0;
  for(int y=0;y<NY;y++)
    for(int x=0;x<NX;x++){
      if(y==0&&x==0)
	continue;
      double b=fabs(in[y][x]);
      if(b>a)
	a=b;
    }
  return a;
}


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
lmt2(int a){
  if(a>127)
    return 127;
  else if(a<-128)
    return -128;
  else
    return a;
}

typedef int (*type_lmt)(int a);

type_lmt lmt3[3]={lmt,lmt2,lmt2};


int w_jx[NX][NX];
int w_jy[NY][NY];

void 
init_walsh(){
  // memset(m_walsh,0,sizeof(m_walsh));
  // memset(m_iwalsh,0,sizeof(m_iwalsh));
  memset(w_jx,0,sizeof(w_jx));
  memset(w_jy,0,sizeof(w_jy));

#define W_B(k,i) (((k)&(1<<(i))) ?  1 : 0)
#define W_1(v)   ((v)%2==0?1:-1)

  for(int k=0;k<NX;k++){
    //http://fourier.eng.hmc.edu/e161/lectures/wht/node4.html
    for(int m=0;m<NX;m++){
      int j=0;
      for(int i=0;i<3;i++){
	j+=(W_B(k,i)+W_B(k,i+1))*W_B(m,3-i-1);
      }
      w_jx[k][m]=W_1(j);
    }
  }
  
  for(int k=0;k<NY;k++){
    //http://fourier.eng.hmc.edu/e161/lectures/wht/node4.html
    for(int m=0;m<NY;m++){
      int j=0;
      for(int i=0;i<3;i++){
	j+=(W_B(k,i)+W_B(k,i+1))*W_B(m,3-i-1);
      }
      w_jy[k][m]=W_1(j);
    }
  }
  
}



void 
walsh(double in[NY][NX],double out[NY][NX]){
  double tmp[NY][NX];
  mzero(out);
  mzero(tmp);

  for(int y=0;y<NY;y++)
    for(int x=0;x<NX;x++)
      for(int iy=0;iy<NY;iy++)
	tmp[y][x]+=(double)w_jx[y][iy]*in[iy][x];
  
  for(int y=0;y<NY;y++)
    for(int x=0;x<NX;x++)
      for(int iy=0;iy<NX;iy++)
	out[y][x]+=tmp[y][iy]*(double)w_jy[iy][x]/8.0;

}

double p2[]={1,1};
double p4[]={0.6830127,1.1830127,0.3169873,-0.1830127};
double p6[]={0.47046721,1.14111692,0.650365,-0.19093442,-0.12083221,0.0498175};
double p8[]={0.32580343,1.01094572,0.8922014,-0.03957503,-0.26450717,0.0436163,
	     0.0465036,-0.01498699};
double p10[]={0.22641898,0.85394354,1.02432694,0.19576696,-0.34265671,-0.04560113,
	      0.10970265,-0.0088268,-0.01779187,0.004717428};
double p12[]={0.15774243,0.69950381,1.06226376,0.44583132,-0.3199866,-0.18351806,
	      0.13788809,0.03892321,-0.04466375,0.000783251,0.006756062,-0.001523534};
double p14[]={0.11009943,0.56079128,1.03114849,0.66437248,-0.20351382,-0.31683501,
	      0.1008467,1.14E-01,-0.05378245,-0.02343994,0.01774979,0.000607515,
	      -0.002547905,0.000500227};
double p16[]={0.07695562,0.44246725,0.95548615,0.82781653,-0.02238574,-0.40165863,
	      0.000668194,0.18207636,-0.0245639,-0.06235021,0.01977216,0.01236884,
	      -0.006887719,-0.000554005,0.00095523,-0.000166137};
double p18[]={0.05385035,0.3448343,0.85534906,0.92954571,0.18836955,-0.41475176,
	      -0.13695355,0.21006834,0.043452675,-0.09564726,0.000354893,0.03162417,
	      -0.00667962,-0.006054961,0.002612967,0.000325815,-3.56E-04,5.56E-05};
double p20[]={0.03771716,0.26612218,0.74557507,0.97362811,3.98E-01,-3.53E-01,
	      -0.27710988,0.18012745,0.13160299,-0.10096657,-0.04165925,0.04696981,
	      0.005100437,-0.015179,0.001973325,0.002817687,-9.70E-04,-1.65E-04,
	      1.32E-04,-1.88E-05};

double q2[2];
double q4[4];
double q6[6];
double q8[8];
double q10[10];
double q12[12];
double q14[14];
double q16[16];
double q18[18];
double q20[20];


void
init_dwt(){
  
#define INIT_DWT(N) \
  for(int i=0;i<N;i++) \
    q##N[i]=( i%2==0 ? 1.0 : -1.0) * p##N[N-i-1];

  INIT_DWT(2);
  INIT_DWT(4);
  INIT_DWT(6);
  INIT_DWT(8);
  INIT_DWT(10);
  INIT_DWT(12);
  INIT_DWT(14);
  INIT_DWT(16);
  INIT_DWT(18);
  INIT_DWT(20);
}


void 
haar(double in[NY][NX],double out[NY][NX]){
  mzero(out);

  for(int n=NX;n>1;n/=2){
    for(int y=0;y<NY;y++)
      for(int x=0;x<n/2;x++){
	out[y][x]    =(in[y][x*2]+in[y][x*2+1])/2;
	out[y][x+n/2]= in[y][x*2]-in[y][x*2+1];
      }

  
    for(int y=0;y<NY;y++)
      for(int x=0;x<n;x++)
	in[y][x]=out[y][x];
  }

  for(int n=NY;n>1;n/=2){
    for(int x=0;x<NX;x++)
      for(int y=0;y<n/2;y++){
	out[y    ][x]=(in[y*2][x]+in[y*2+1][x])/2;
	out[y+n/2][x]= in[y*2][x]-in[y*2+1][x];
      }
  
    for(int x=0;x<NX;x++)
      for(int y=0;y<n;y++)
	in[y][x]=out[y][x];
  }

}

void 
ihaar(double in[NY][NX],double out[NY][NX]){
  mzero(out);

  for(int n=2;n<=NY;n*=2){
    for(int x=0;x<NX;x++)
      for(int y=0;y<n/2;y++){
	out[y*2  ][x]=(in[y][x]*2+in[y+n/2][x])/2;
	out[y*2+1][x]=(in[y][x]*2-in[y+n/2][x])/2;
      }
    for(int x=0;x<NX;x++)
      for(int y=0;y<n;y++)
	in[y][x]=out[y][x];
  }


  for(int n=2;n<=NX;n*=2){
    for(int y=0;y<NY;y++)
      for(int x=0;x<n/2;x++){
	out[y][x*2  ]=(in[y][x]*2+in[y][x+n/2])/2;
	out[y][x*2+1]=(in[y][x]*2-in[y][x+n/2])/2;
      }
    for(int y=0;y<NY;y++)
      for(int x=0;x<n;x++)
	in[y][x]=out[y][x];
  }


}

void 
dwt(double in[NY][NX],double out[NY][NX]){
  mzero(out);
  
  for(int y=0;y<NY;y++)
    for(int x=0;x<NX;x++){
      in[y][x]*=2.0;
      in[y][x]+=1.0;
    }

  for(int n=NX;n>1;n/=2){
    for(int y=0;y<NY;y++)
      for(int x=0;x<n/2;x++){
	out[y][x]    =0;
	out[y][x+n/2]=0;
	for(int i=0;i<4;i++){
	  int idx=(2*x+i)%n;
	  out[y][x]    +=p4[i]*in[y][idx]/2.0;
	  out[y][x+n/2]+=q4[i]*in[y][idx];
	}
      }

    for(int y=0;y<NY;y++)
      for(int x=0;x<n;x++)
	in[y][x]=out[y][x];
  }

  for(int n=NY;n>1;n/=2){
    for(int x=0;x<NX;x++)
      for(int y=0;y<n/2;y++){
	out[y    ][x]=0;
	out[y+n/2][x]=0;
	for(int i=0;i<4;i++){
	  int idx=(2*y+i)%n;
	  out[y    ][x]+=p4[i]*in[idx][x]/2.0;
	  out[y+n/2][x]+=q4[i]*in[idx][x];
	}
      }

    
    for(int y=0;y<n;y++)
      for(int x=0;x<NX;x++)
	in[y][x]=out[y][x];
  }


}

void 
idwt(double in[NY][NX],double out[NY][NX]){
  mzero(out);

  for(int n=2;n<=NY;n*=2){
    for(int x=0;x<NX;x++)
      for(int y=0;y<n/2;y++){
	out[y*2  ][x]=0;
	out[y*2+1][x]=0;
	for(int i=0;i<4/2;i++){
	  int idx=(y-i+(n/2)*128)%(n/2);
	  out[y*2  ][x]+=(p4[i*2  ]*in[idx][x]+q4[i*2  ]*in[idx+n/2][x]/2.0);
	  out[y*2+1][x]+=(p4[i*2+1]*in[idx][x]+q4[i*2+1]*in[idx+n/2][x]/2.0);
	}
      }

    for(int x=0;x<NX;x++)
      for(int y=0;y<n;y++)
	in[y][x]=out[y][x];
  }


  for(int n=2;n<=NX;n*=2){
    for(int y=0;y<NY;y++)
      for(int x=0;x<n/2;x++){
	out[y][x*2  ]=0;
	out[y][x*2+1]=0;
	for(int i=0;i<4/2;i++){
	  int idx=(x-i+(n/2)*128)%(n/2);
	  out[y][x*2  ]+=(p4[i*2  ]*in[y][idx]+q4[i*2  ]*in[y][idx+n/2]/2.0);
	  out[y][x*2+1]+=(p4[i*2+1]*in[y][idx]+q4[i*2+1]*in[y][idx+n/2]/2.0);
	}
      }
    for(int y=0;y<NY;y++)
      for(int x=0;x<n;x++)
	in[y][x]=out[y][x];
  }

  for(int y=0;y<NY;y++)
    for(int x=0;x<NX;x++)
      out[y][x]/=2.0;
}



void 
dct(double in[NY][NX],double out[NY][NX]){
  mzero(out);

  for(int y=0;y<NY;y++)
    for(int x=0;x<NX;x++)
      for(int iy=0;iy<NY;iy++)
	for(int ix=0;ix<NX;ix++)
	  out[y][x]+=
	    (x==0?1/(sqrt(2)):1)*
	    (y==0?1/(sqrt(2)):1)*
	    cos((2*ix+1)*x*M_PI/(2*NX))*
	    cos((2*iy+1)*y*M_PI/(2*NY))*
	    in[iy][ix]*
	    (2.0/sqrt(NX*NY));
}

void 
idct(double in[NY][NX],double out[NY][NX]){
  mzero(out);

  for(int y=0;y<NY;y++)
    for(int x=0;x<NX;x++)
      for(int iy=0;iy<NY;iy++)
	for(int ix=0;ix<NX;ix++)
	  out[y][x]+=
	    (ix==0?1/(sqrt(2)):1)*
	    (iy==0?1/(sqrt(2)):1)*
	    cos((2*x+1)*ix*M_PI/(2*NX))*
	    cos((2*y+1)*iy*M_PI/(2*NY))*
	    in[iy][ix]*
	    (2.0/sqrt(NX*NY));
}


// void
// tRGB2YUV(RGB& in,RGB& out){
//   out[0]=((int)in[0]+2*(int)in[1]+(int)in[2])/4;
//   out[1]=((int)in[2]-(int)in[1])/2;
//   out[2]=((int)in[0]-(int)in[1])/2;
// }

// void
// tYUV2RGB(RGB& in,RGB& out){
//   out[1]=(int)in[0]-(((int)in[1]*2+(int)in[2]*2)/4);
//   out[0]=(int)out[1]+(int)in[2]*2;
//   out[2]=(int)out[1]+(int)in[1]*2;
//   for(int c=0;c<3;c++)
//     out[c]=lmt(out[c]);
// }


void
RGB2YUV(RGB& in,RGB& out){
  out[0]=(double)(0.29891 *in[0]+0.58661*in[1]+0.11448*in[2]);
  out[1]=(double)(-0.16874*in[0]-0.33126*in[1]+0.5*in[2]);
  out[2]=(double)(0.5*in[0]-0.41869*in[1]-0.08131*in[2]);
}

void
YUV2RGB(RGB& in,RGB& out){
  out[0]=(double)(1.0*in[0]+1.402*in[2]);
  out[1]=(double)(1.0*in[0]-0.34413*in[1]-0.71414*in[2]);
  out[2]=(double)(1.0*in[0]+1.772*in[1]);
  for(int c=0;c<3;c++)
    out[c]=lmt(out[c]);
}




// int qt2[3][8][8]=
//   {
//     {{16,11,10,16, 24, 40, 51, 61},
//      {12,12,14,19, 26, 58, 60, 55},
//      {14,13,16,24, 40, 57, 69, 56},
//      {14,17,22,29, 51, 87, 80, 62},
//      {18,22,37,56, 68,109,103, 77},
//      {24,35,55,64, 81,104,113, 92},
//      {49,64,78,87,103,121,120,101},
//      {72,92,95,98,112,100,103, 99}},
//     {{17, 18, 24, 47, 99, 99, 99, 99},
//      {18, 21, 26, 66, 99, 99, 99, 99},
//      {24, 26, 56, 99, 99, 99, 99, 99},
//      {47, 66, 99, 99, 99, 99, 99, 99},
//      {99, 99, 99, 99, 99, 99, 99, 99},
//      {99, 99, 99, 99, 99, 99, 99, 99},
//      {99, 99, 99, 99, 99, 99, 99, 99},
//      {99, 99, 99, 99, 99, 99, 99, 99}},
//     {{17, 18, 24, 47, 99, 99, 99, 99},
//      {18, 21, 26, 66, 99, 99, 99, 99},
//      {24, 26, 56, 99, 99, 99, 99, 99},
//      {47, 66, 99, 99, 99, 99, 99, 99},
//      {99, 99, 99, 99, 99, 99, 99, 99},
//      {99, 99, 99, 99, 99, 99, 99, 99},
//      {99, 99, 99, 99, 99, 99, 99, 99},
//      {99, 99, 99, 99, 99, 99, 99, 99}}
//   };

// int qt[3][8][8]=
//   {
//     {{1,2,8,8, 32,32,32,32},
//      {2,4,8,8, 32,32,32,32},
//      {8,8,16,16, 32,32,32,32},
//      {8,8,16,16, 32,32,32,32},
//      {32,32,32,32, 64,64,64,64},     
//      {32,32,32,32, 64,64,64,64},     
//      {32,32,32,32, 64,64,64,64},     
//      {32,32,32,32, 64,64,64,64}},
//     {{1,2,8,8, 32,32,32,32},
//      {2,4,8,8, 32,32,32,32},
//      {8,8,16,16, 32,32,32,32},
//      {8,8,16,16, 32,32,32,32},
//      {32,32,32,32, 64,64,64,64},     
//      {32,32,32,32, 64,64,64,64},     
//      {32,32,32,32, 64,64,64,64},     
//      {32,32,32,32, 64,64,64,64}},
//     {{1,2,8,8, 32,32,32,32},
//      {2,4,8,8, 32,32,32,32},
//      {8,8,16,16, 32,32,32,32},
//      {8,8,16,16, 32,32,32,32},
//      {32,32,32,32, 64,64,64,64},     
//      {32,32,32,32, 64,64,64,64},     
//      {32,32,32,32, 64,64,64,64},     
//      {32,32,32,32, 64,64,64,64}}
//   };



int qt[NY][NX];

double quality=0.5;

void
init_qt(){
  mzero(qt);
  int nx=NX;
  int ny=NY;
  while(1){
    for(int y=0;y<ny;y++){
	for(int x=0;x<nx;x++){
	    if(y<ny/2&&x<nx/2)
	      qt[y][x]+=2;
	    else if(y<ny/2||x<nx/2)
	      qt[y][x]+=1;
	}
    }
    if(nx>1) nx/=2;
    if(ny>1) ny/=2;
    if(nx==1&&ny==1)
      break;
  }
  int dc=qt[0][0];

  for(int y=0;y<NY;y++)
    for(int x=0;x<NX;x++)
      qt[y][x]=1<<(dc-qt[y][x]);
}


struct zigzag{
  int zx,zy;
  int px;
  zigzag(){
    zx=0;
    zy=0;
    px=0;

  }
  void next(){
    while(1){
      if(zy==NY-1&&zx==NX-1)
	return;
      if(px==0){
	if(zy==0){
	  px=1;
	  zx++;
	}else{
	  zy--;
	  zx++;
	}
      }else{
	if(zx==0){
	  px=0;
	  zy++;
	}else{
	  zx--;
	  zy++;
	}
      }
      if(zy<NY&&zx<NX)
	return;
    }
  }
  
};

struct RunBit{
  int zero;// >=0:runlength, -1:EOB
  int code;
};

struct Code{
  int len;
  int code;
};

void
mkRunBit(double in[NY][NX],vector<RunBit>& vec){
  zigzag a;
  int zero=0;
  RunBit r;
  for(int i=0;i<64;i++){
    r.zero=zero;
    r.code=in[a.zy][a.zx];
    if((int)(in[a.zy][a.zx])==0)
      zero++;
    else{
      vec.push_back(r);
      zero=0;
    }
    a.next();
  }
  if(zero!=0){
    r.zero=-1;
    r.code=0;
    vec.push_back(r);
  }
}




void
type2(vector<RunBit>& rvec,vector<Code>& cvec){
  Code c;
  for(int i=0;i<rvec.size();i++){
    RunBit r=rvec[i];
    if(i==0){
      c.code=r.code;
      c.len=11;
      cvec.push_back(c);
      continue;
    }
  RESTART:
    if(r.zero==-1){
      c.code=0x1f;
      c.len =5;
    }else if(r.zero>=8){
      c.code=0x1e;
      c.len =5;
      cvec.push_back(c);
      r.zero-=8;
      goto RESTART;
    }else if(r.zero!=0){
      c.code=0;
      c.len =r.zero;
      cvec.push_back(c);
      r.zero=0;
      goto RESTART;
    }else if(abs(r.code)<=1){
      c.code=(2<<1);
      if(r.code<0)
	c.code|=1;
      c.len =3;
    }else if(abs(r.code)<=3){
      c.code=(0xc<<2);
      if(r.code<0)
	c.code|=(1<<1)|(abs(r.code)-2);
      else
	c.code|=(abs(r.code)-2);
      c.len =6;
    }else if(abs(r.code)<=7){
      c.code=(0xd<<3);
      if(r.code<0)
	c.code|=(1<<2)|(abs(r.code)-4);
      else
	c.code|=(abs(r.code)-4);
      c.len =7;
    }else if(abs(r.code)<=15){
      c.code=(0x38<<6);
      if(r.code<0)
	c.code|=(1<<3)|(abs(r.code)-7);
      else
	c.code|=(abs(r.code)-7);
      c.len =10;
    }else if(abs(r.code)<=31){
      c.code=(0x39<<6);
      if(r.code<0)
	c.code|=(1<<4)|(abs(r.code)-15);
      else
	c.code|=(abs(r.code)-15);
      c.len =11;
    }else{
      c.code=(0x1d<<11);
      if(r.code<0)
	c.code|=(1<<10)|abs(r.code);
      else
	c.code|=abs(r.code);
      c.len =16;
    }
    cvec.push_back(c);
  }
}
	   


void
type2_1(vector<RunBit>& rvec,vector<Code>& cvec){
  Code c;
  for(int i=0;i<rvec.size();i++){
    RunBit r=rvec[i];
    if(i==0){
      c.code=r.code;
      c.len=11;
      cvec.push_back(c);
      continue;
    }
  RESTART:
    if(r.zero==-1){
      c.code=0x7f;
      c.len =7;
      // }else if(r.zero>=8){
      //   c.code=0x1e;
      //   c.len =5;
      //   cvec.push_back(c);
      //   r.zero-=8;
      //   goto RESTART;
    }else if(r.zero!=0){
      c.code=0;
      c.len =r.zero;
      cvec.push_back(c);
      r.zero=0;
      goto RESTART;
    }else if(r.code==1){
      c.code=2;
      c.len =2;
    }else if(r.code==-1){
      c.code=6;
      c.len =3;
    }else if(abs(r.code)<=3){
      c.code=(0xc<<2);
      if(r.code<0)
	c.code|=(1<<1)|(abs(r.code)-2);
      else
	c.code|=(abs(r.code)-2);
      c.len =7;
    }else if(abs(r.code)<=7){
      c.code=(0xd<<3);
      if(r.code<0)
	c.code|=(1<<2)|(abs(r.code)-4);
      else
	c.code|=(abs(r.code)-4);
      c.len =8;
    }else if(abs(r.code)<=15){
      c.code=(0x38<<6);
      if(r.code<0)
	c.code|=(1<<3)|(abs(r.code)-7);
      else
	c.code|=(abs(r.code)-7);
      c.len =10;
    }else if(abs(r.code)<=31){
      c.code=(0x39<<6);
      if(r.code<0)
	c.code|=(1<<4)|(abs(r.code)-15);
      else
	c.code|=(abs(r.code)-15);
      c.len =12;
    }else{
      c.code=(0x1d<<11);
      if(r.code<0)
	c.code|=(1<<10)|abs(r.code);
      else
	c.code|=abs(r.code);
      c.len =16;
    }
    cvec.push_back(c);
  }
}
	   


void
type2_2(vector<RunBit>& rvec,vector<Code>& cvec){
  Code c;
  for(int i=0;i<rvec.size();i++){
    RunBit r=rvec[i];
    if(i==0){
      c.code=r.code;
      c.len=11;
      cvec.push_back(c);
      continue;
    }
  RESTART:
    if(r.zero==-1){
      c.code=0x3f;
      c.len =6;
    }else if(r.zero>=8){
      c.code=0x3e;
      c.len =6;
      cvec.push_back(c);
      r.zero-=8;
      goto RESTART;
    }else if(r.zero!=0){
      c.code=0;
      c.len =r.zero;
      cvec.push_back(c);
      r.zero=0;
      goto RESTART;
    }else if(abs(r.code)<=1){
      c.code=(0xc<<2);
      if(r.code<0)
	c.code|=(1<<1)|(abs(r.code)-1);
      else
	c.code|=(abs(r.code)-1);
      c.len =4;
    }else if(abs(r.code)<=3){
      c.code=(0xc<<2);
      if(r.code<0)
	c.code|=(1<<1)|(abs(r.code)-2);
      else
	c.code|=(abs(r.code)-2);
      c.len =5;
    }else if(abs(r.code)<=7){
      c.code=(0xd<<3);
      if(r.code<0)
	c.code|=(1<<2)|(abs(r.code)-4);
      else
	c.code|=(abs(r.code)-4);
      c.len =6;
    }else if(abs(r.code)<=15){
      c.code=(0x38<<6);
      if(r.code<0)
	c.code|=(1<<3)|(abs(r.code)-7);
      else
	c.code|=(abs(r.code)-7);
      c.len =9;
    }else if(abs(r.code)<=31){
      c.code=(0x39<<6);
      if(r.code<0)
	c.code|=(1<<4)|(abs(r.code)-15);
      else
	c.code|=(abs(r.code)-15);
      c.len =9;
    }else{
      c.code=(0x1d<<11);
      if(r.code<0)
	c.code|=(1<<10)|abs(r.code);
      else
	c.code|=abs(r.code);
      c.len =16;
    }
    cvec.push_back(c);
  }
}
	   

void
type2_3(vector<RunBit>& rvec,vector<Code>& cvec){
  Code c;
  for(int i=0;i<rvec.size();i++){
    RunBit r=rvec[i];
    if(i==0){
      c.code=r.code;
      c.len=11;
      cvec.push_back(c);
      continue;
    }
  RESTART:
    if(r.zero==-1){
      c.code=0x1f;
      c.len =5;
    }else if(r.zero>=8){
      c.code=0x1e;
      c.len =5;
      cvec.push_back(c);
      r.zero-=8;
      goto RESTART;
    }else if(r.zero!=0){
      c.code=0;
      c.len =r.zero;
      cvec.push_back(c);
      r.zero=0;
      goto RESTART;
    }else if(abs(r.code)<=1){
      c.len =4;
    }else if(abs(r.code)<=3){
      c.len =6;
    }else if(abs(r.code)<=7){
      c.len =7;
    }else if(abs(r.code)<=15){
      c.len =8;
    }else if(abs(r.code)<=31){
      c.len =9;
    }else if(abs(r.code)<=63){
      c.len =11;
    }else{
      c.code=(0x1d<<11);
      if(r.code<0)
	c.code|=(1<<10)|abs(r.code);
      else
	c.code|=abs(r.code);
      c.len =16;
    }
    cvec.push_back(c);
  }
}
	   


void
o2mkCode(vector<RunBit>& rvec,vector<Code>& cvec){
  Code c;
  for(int i=0;i<rvec.size();i++){
    RunBit r=rvec[i];
    if(i==0){
      c.code=r.code;
      c.len=11;
      cvec.push_back(c);
      continue;
    }
  RESTART:
    if(r.zero==-1){
      c.code=0x1f;
      c.len =5;
    }else if(r.zero>=8){
      c.code=0x1e;
      c.len =5;
      cvec.push_back(c);
      r.zero-=8;
      goto RESTART;
    }else if(r.zero!=0){
      c.code=0;
      c.len =r.zero;
      cvec.push_back(c);
      r.zero=0;
      goto RESTART;
    }else if(abs(r.code)<=1){
      c.code=(2<<1);
      if(r.code<0)
	c.code|=1;
      c.len =3;
    }else if(abs(r.code)<=3){
      c.code=(0xc<<2);
      if(r.code<0)
	c.code|=(1<<1)|(abs(r.code)-2);
      else
	c.code|=(abs(r.code)-2);
      c.len =6;
    }else if(abs(r.code)<=7){
      c.code=(0xd<<3);
      if(r.code<0)
	c.code|=(1<<2)|(abs(r.code)-4);
      else
	c.code|=(abs(r.code)-4);
      c.len =7;
    }else if(abs(r.code)<=15){
      c.code=(0x38<<4);
      if(r.code<0)
	c.code|=(1<<3)|(abs(r.code)-7);
      else
	c.code|=(abs(r.code)-7);
      c.len =10;
    }else if(abs(r.code)<=31){
      c.len =11;
    }else if(abs(r.code)<=63){
      c.len =14;
    }else if(abs(r.code)<=127){
      c.len =15;
    }else if(abs(r.code)<=255){
      c.len =17;
    }else if(abs(r.code)<=511){
      c.len =19;
    }else{
      c.code=(0x1d<<11);
      if(r.code<0)
	c.code|=(1<<10)|abs(r.code);
      else
	c.code|=abs(r.code);
      c.len =20;
    }
    cvec.push_back(c);
  }
}
	   
void
omkCode(vector<RunBit>& rvec,vector<Code>& cvec){
  Code c;
  for(int i=0;i<rvec.size();i++){
    RunBit r=rvec[i];
    if(i==0){
      c.code=r.code;
      c.len=11;
      cvec.push_back(c);
      continue;
    }
  RESTART:
    if(r.zero==-1){
      c.code=0x1f;
      c.len =5;
    }else if(r.zero>=8){
      c.code=0x1e;
      c.len =5;
      cvec.push_back(c);
      r.zero-=8;
      goto RESTART;
    }else if(r.zero!=0){
      c.code=0;
      c.len =r.zero*2;
      cvec.push_back(c);
      r.zero=0;
      goto RESTART;
    }else if(abs(r.code)<=1){
      c.code=(2<<1);
      if(r.code<0)
	c.code|=1;
      c.len =2;
    }else if(abs(r.code)<=3){
      c.code=(0xc<<2);
      if(r.code<0)
	c.code|=(1<<1)|(abs(r.code)-2);
      else
	c.code|=(abs(r.code)-2);
      c.len =6;
    }else if(abs(r.code)<=7){
      c.code=(0xd<<3);
      if(r.code<0)
	c.code|=(1<<2)|(abs(r.code)-4);
      else
	c.code|=(abs(r.code)-4);
      c.len =7;
    }else if(abs(r.code)<=15){
      c.code=(0x38<<4);
      if(r.code<0)
	c.code|=(1<<3)|(abs(r.code)-7);
      else
	c.code|=(abs(r.code)-7);
      c.len =10;
    }else if(abs(r.code)<=31){
      c.len =11;
    }else if(abs(r.code)<=63){
      c.len =14;
    }else if(abs(r.code)<=127){
      c.len =15;
    }else if(abs(r.code)<=255){
      c.len =17;
    }else if(abs(r.code)<=511){
      c.len =19;
    }else{
      c.code=(0x1d<<11);
      if(r.code<0)
	c.code|=(1<<10)|abs(r.code);
      else
	c.code|=abs(r.code);
      c.len =20;
    }
    cvec.push_back(c);
  }
}
	   

#define mkCode type2_3


int
main(int argc,char** argv){
  quality=atof(argv[1]);
  BMP fin2 (argv[2]);
  BMP fin ;fin.init(fin2.w,fin2.h);
  BMP fout2;fout2.init(fin2.w,fin2.h);
  BMP fout;fout.init(fin2.w,fin2.h);
  
  double in[NY][NX];
  double out[NY][NX];
  double out2[NY][NX];
  int i,j,idx;

  init_qt();

  if(quality==0.0)
    mone(qt);
    
    

  init_walsh();
  init_dwt();

  bmp_for(fin)
    RGB2YUV(fin2(x,y),fin(x,y));


  int    num_dat[14];
  double prob_dat[14];
  int    sum_dat[14];
  double bit_dat[14];
  
  memset(num_dat,0,sizeof(num_dat));


  int num_totallen=0;
  for(int c=0;c<3;c++){
    for(int by=0;by<fin.h/NY;by++)
      for(int bx=0;bx<fin.w/NX;bx++){
	int num_codelen=0;
	for(int dy=0;dy<NY;dy++)
	  for(int dx=0;dx<NX;dx++){
	    int x=bx*NX+dx;
	    int y=by*NY+dy;
	    in[   dy][   dx]=fin(x,y)[c];
	    // in[   dy][   dx]=255;
	  }

	//dct(in,out);
	//walsh(in,out);

	// mprint(in);
	//dwt(in,out);
	haar(in,out);
	// mprint(out);
	// idwt(out,in);
	// mprint(in);
	// return 0;
	// ihaar(out,in);
	// mprint(in);
	// return 0;
	
	
	// if(c!=0)
	mmap(out,(1.0/(qt[y][x]*quality < 1.0 ? 1.0 : qt[y][x]*quality))*);
	mmap(out,round);


	//	mprint(out);
	vector<RunBit> rvec;
	vector<Code>   cvec;
	mkRunBit(out,rvec);
	mkCode(rvec,cvec);
	
	for(int dy=0;dy<NY;dy++)
	  for(int dx=0;dx<NX;dx++){
	    num_dat[(int)(log(fabs(out[dy][dx])+1.0)/log(2))]++;
	  }
		
	// for(int i=0;i<rvec.size();i++)
	//   printf("%d %d\n",rvec[i].zero,rvec[i].code);

	// printf("\n");
	
	// for(int i=0;i<cvec.size();i++)
	//   printf("%d %x\n",cvec[i].len,cvec[i].code);
	// return 0;

	for(int i=0;i<cvec.size();i++)
	  num_codelen+=cvec[i].len;
	double comprate=(double)(8*8*8)/(double)(num_codelen);

	// if(comprate<3){
	//   mprint(out);

		
	//   for(int i=0;i<rvec.size();i++)
	//     printf("%d %d\n",rvec[i].zero,rvec[i].code);
	//   printf("\n");
	
	//   for(int i=0;i<cvec.size();i++)
	//     printf("%d %x\n",cvec[i].len,cvec[i].code);
	//   printf("\n");

	//   printf("comprate %f",comprate);


	//   return 0;
	// }

	num_totallen+=num_codelen;
    
	
	//	if(c!=0)
	mmap(out,(qt[y][x]*quality < 1.0 ? 1.0 : qt[y][x]*quality)*);
	//idct(out,out2);
	//walsh(out,out2);
	//idwt(out,out2);
	ihaar(out,out2);

	for(int dy=0;dy<NY;dy++)
	  for(int dx=0;dx<NX;dx++){
	    int x=bx*NX+dx;
	    int y=by*NY+dy;
	    fout(x,y)[c]=lmt3[c](out2[   dy][   dx]);
	  }



      }
  }
  bmp_for(fin)
    YUV2RGB(fout(x,y),fout2(x,y));

  for(int y=0;y<14;y++)
    prob_dat[y]=(double)(num_dat[y])/(double)(fin.w*fin.h*3);

  for(int y=0;y<14;y++)
    sum_dat[y]=1<<y;
  
  for(int y=0;y<14;y++)
    if(sum_dat[y]!=0&&prob_dat[y]!=0.0)
      bit_dat[y]=-log(prob_dat[y]/sum_dat[y])/log(2);

  double ave_bits=0;
  
  for(int y=0;y<14;y++)
    ave_bits+=prob_dat[y]*bit_dat[y];
  

  
  for(int y=0;y<14;y++){
    printf("%2d,%5d,%9d,%8.3f,%8.3f,%8.3f\n",
	   y,
	   sum_dat[y],
	   num_dat[y],
	   prob_dat[y],
	   bit_dat[y],
	   prob_dat[y]*bit_dat[y]
	   );
  }
  printf("ideal comprate:%f\n",8.0/ave_bits);


  
  double tcomprate=(double)(fin.w*fin.h*24)/(double)(num_totallen);
  printf("tcomprate %f\n",tcomprate);


  fout2.write(argv[3]);
 
  return true;
}
