#!/usr/bin/env c-script

#pragma c-script:libs  -llapack-3 -latlas -lcblas
extern "C"{
#include <cblas.h>
#include <clapack.h>
}


struct Matrix{
  double *dat;
  int nr;
  int nc;
  Matrix(int nr,int nc){
    this->nr=nr;
    this->nc=nc;
    dat=new double[nr*nc];
  }
  Matrix(Matrix& m){
    nr=m.nr;
    nc=m.nc;
    dat=new double[nr*nc];
    memcpy(dat,m.dat,sizeof(double)*nc*nr);
  }
  ~Matrix(){
    delete []dat;
    dat=NULL;
  }
  int nrow(){
    return nr;
  }
  int ncol(){
    return nc;
  }
  double* operator[](int row){
    return dat+row*nc;
  }
};

template<int NR,int NC>
struct CMatrix{
  double dat[NR][NC];
  int nrow(){
    return NR;
  }
  int ncol(){
    return NC;
  }
  double* operator[](int row){
    return dat[row];
  }
};


/*
C := alpha*op( A )*op( B ) + beta*C,
void cblas_dgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
                 const enum CBLAS_TRANSPOSE TransB, const int M, const int N,
                 const int K, const double alpha, const double *A,
                 const int lda, const double *B, const int ldb,
                 const double beta, double *C, const int ldc);
*/
template<class Ma,class Mb,class Mc>
void
mdgemm(Ma& a,Mb& b,Mc& c,double alpha=1.0,double beta=0.0){
  cblas_dgemm(CblasRowMajor,
	      CblasNoTrans,
	      CblasNoTrans,
	      a.nrow(),b.ncol(),a.ncol(),
	      alpha,
	      (double*)a.dat,a.ncol(),
	      (double*)b.dat,b.ncol(),
	      beta,
	      (double*)c.dat,c.ncol()
	      );
}

template<class Ma,class Mb>
void
mdaxpy(Ma& a,Mb& b,double alpha=1.0){
  cblas_daxpy(a.nrow()*a.ncol(),
	      alpha,
	      (double*)a.dat,
	      1,
	      (double*)b.dat,
	      1);
}

template<class Ma,class Mb>
void
mdgetrs(Ma& a,Mb& b){
  int n=min(a.ncol(),a.nrow());
  int* ipiv=new int[n];
  clapack_dgetrf(CblasRowMajor,
		 a.nrow(),a.ncol(),
		 (double*)a.dat,a.ncol(),
		 ipiv);
  clapack_dgetrs(CblasRowMajor,
		 CblasNoTrans,
		 n,
		 b.ncol(),
		 (double*)a.dat,a.ncol(),
		 ipiv,
		 (double*)b.dat,a.ncol());
  delete[] ipiv;
}

#define mmul mdgemm
#define madd mdaxpy


template<class M>
void
mprint(M& a){
  for(int r=0;r<a.nrow();r++){
    for(int c=0;c<a.ncol();c++)
      printf("%3d,",(int)(a[r][c]));
    printf("\n");
  }
  printf("\n");
}
 
void
mset(Matrix& a,double* dat){
  for(int i=0;i<a.nr*a.nc;i++)
    a.dat[i]=dat[i];
}

 
int
main(){
  // Matrix b(2,2);
  // Matrix c(2,2);
  // CMatrix<2,2> a={{{1,2},
  // 		   {3,2}}};
  // double bb[]={3,1,
  // 	       2,2};
  // mset(b,bb);
  // mmul(a,b,c);
  // mprint(a);
  // mprint(b);
  // mprint(c);
  
  // madd(a,b);
  // mprint(a);
  // mprint(b);


  CMatrix<2,2> d={{{1,2},
		   {0,2}}};
  
  CMatrix<2,1> e={{{1},
		   {2}}};
  
  mprint(d);
  mprint(e);
  mdgetrs(d,e);
  mprint(d);
  mprint(e);
  
  return true;
}
