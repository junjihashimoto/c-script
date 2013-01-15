#ifndef BMP_UTIL_H
#define BMP_UTIL_H

template<class T> double square(T a);
template<class T> double psnr(const T& a,const T& b,int depth=8);
template<class T> double maxerr(const T& a,const T& b);
template<class T> double minerr(const T& a,const T& b,int depth=8);
template<class T> double meanerr(const T& a,const T& b);
template<class T> double stddev(const T& a,const T& b);


template<class T>
double
square(T a){
  return a*a;
}

template<class T>
double
psnr(const T& a,const T& b,int depth=8){
  double t=0.0;
  bmp_for(a){
    for(int c=0;c<3;c++){
      t+=square(a(x,y)[c]-b(x,y)[c]);
    }
  }
  t/=(3.0*a.w*a.h);
  return 10.0*(log10(square((1<<depth)-1))-log10(t));
  
}

template<class T>
double
maxerr(const T& a,const T& b){
  double m=0;
  double t;
  bmp_for(a){
    for(int c=0;c<3;c++){
      t=abs(a(x,y)[c]-b(x,y)[c]);
      if(t>m)
	m=t;
    }
  }
  return m;
}

template<class T>
double
minerr(const T& a,const T& b,int depth=8){
  double m=(int)((1<<depth)-1);
  double t;
  bmp_for(a){
    for(int c=0;c<3;c++){
      t=abs(a(x,y)[c]-b(x,y)[c]);
      if(t<m)
	m=t;
    }
  }
  return m;
}

template<class T>
double
meanerr(const T& a,const T& b){
  double t=0.0;
  bmp_for(a)
    for(int c=0;c<3;c++)
      t+=abs(a(x,y)[c]-b(x,y)[c]);
  t/=3.0*a.w*a.h;
  return t;
}

template<class T>
double
stddev(const T& a,const T& b){
  double t=0.0;
  double m=gen_meanerr(a,b);
  bmp_for(a)
    for(int c=0;c<3;c++)
      t+=square(abs(a(x,y)[c]-b(x,y)[c])-m);
  t/=3.0*a.w*a.h;
  return sqrt(t);
}

#endif

