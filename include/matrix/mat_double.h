#include "mat.h"

extern "C"{
  
  void dswap_(int* N,double* SX,int* INCX,double* SY,int* INCY );
  void dcopy_(const int* N,const double* DX,const int* INCX,double* DY,const int* INCY);
  void dscal_(int* N,double* DA,double* DX,int* INCX);
  int  idamax_(int* N,double* DX,int* INCX);
  void dgetrf_(int*  M,int* N,double* A,int* LDA,int* IPIV,int* INFO);
  void dgetri_(int*  N,double* A,int* LDA,int* IPIV,double* work,int* lwork,int* INFO);
  void dgetrs_(const char* TRANS,int* N,int* NRHS,
	       double* A,int* LDA,int* IPIV,
	       double* B,int* LDB,int* INFO);

  double ddot_(const int* N,double* DX,int* INCX,double* DY,int* INCY);
  void   daxpy_(const int* N,double* DA,double* DX,int* INCX,double* DY,int* INCY);
  double dnrm2_(const int* N,const double* X,const int* INCX);
  void   dgemm_ (const char* TRANSA,const char* TRANSB,
		 const int* M,const int* N,const int* K,
		 double* ALPHA,
		 const double* A,const int* LDA,
		 const double* B,const int* LDB,
		 double* BETA,double* C,int* LDC );

  void dgeev_(const char* JOBVL,const char* JOBVR,
	      int* N,double* A,int* LDA,
	      double* WR,double* WI,
	      double* VL,int* LDVL,
	      double* VR,int* LDVR,
	      double* WORK,int* LWORK,int* INFO );

  void dggev_(const char* JOBVL,const char* JOBVR,
	      int* N,double* A,int* LDA,double* B,int* LDB,double* ALPHAR,double* ALPHAI,
	      double* BETA,
	      double* VL,int* LDVL,
	      double* VR,int* LDVR,
	      double* WORK,int* LWORK,
	      int* INFO );
  void dgels_(const char* TRANS,int*  M,int* N,int* NRHS,
	      double* A,int* LDA,double* B,int* LDB,
	      double* WORK,int* LWORK,int* INFO);

  void dsyev_(const char* JOBZ,const char* UPLO,int* N,
	      double* A,int* LDA,
	      double* W,double* WORK,int* LWORK,int* INFO );

}

template<>
Matrix<double>& 
Matrix<double>::operator += (const Matrix<double>& b){
  double alpha=1.0;
  int n=nrow()*ncol();
  int s=1;
  //b=a+alpha*b
  daxpy_(&n,
	 &alpha,
	 b.dat,
	 &s,
	 dat,
	 &s);

  return *this;
}

template<>
Matrix<double>& 
Matrix<double>::operator -= (const Matrix<double>& b){
  double alpha=-1.0;
  int n=nrow()*ncol();
  int s=1;
  //b=a+alpha*b
  daxpy_(&n,
  	 &alpha,
  	 b.dat,
  	 &s,
  	 dat,
  	 &s);
  dscal_(&n,
	 &alpha,
	 dat,
	 &s);
  return *this;
}


template<>
Matrix<double>& 
Matrix<double>::operator *= (double b){
  double alpha=b;
  int n=nrow()*ncol();
  int s=1;
  //a=ax
  dscal_(&n,
	 &alpha,
	 dat,
	 &s);
  return *this;
}

template<>
Matrix<double>& 
Matrix<double>::operator /= (double b){
  double alpha=1.0/b;
  int n=nrow()*ncol();
  int s=1;
  //a=ax
  dscal_(&n,
	 &alpha,
	 dat,
	 &s);
  return *this;
}

template<>
Matrix<double> operator - (const Matrix<double>& a){
  Matrix<double> c=a;
  double alpha=-1.0;
  int n=a.nrow()*a.ncol();
  int s=1;
  //a=ax
  dscal_(&n,
	 &alpha,
	 c.dat,
	 &s);
  return c;
}

template<>
Matrix<double> operator * (const Matrix<double>& a,const Matrix<double>& b){
  Matrix<double> c(a.nrow(),b.ncol());
  double alpha=1.0;
  double beta=0;
  
  assert(b.nrow()==a.ncol());
  char tran='N';
  dgemm_(&tran,
	 &tran,
	 &a.nr,&b.nc,&a.nc,
	 &alpha,
	 a.dat,&a.nr,
	 b.dat,&b.nr,
	 &beta,
	 c.dat,&c.nr
	 );

  return c;
}


template<>
double
nrm2(const Matrix<double>& a){
  int s=1;
  int n=a.nc*a.nr;
  return dnrm2_(&n,a.dat,&s);
}

template<>
double
dot(const Matrix<double>& a,const Matrix<double>& b){
  assert(a.ncol()==1&&b.ncol()==1);
  int s=1;
  return ddot_(&a.nr,
	       a.dat,
	       &s,
	       b.dat,
	       &s);
}

template<>
void
eig_sym    (const Matrix<double>& input,
	    Matrix<double>& eigen_value,
	    Matrix<double>& eigen_vec){
  eigen_vec=input;
  assert(eigen_vec.nr==eigen_vec.nc);
  eigen_value.init(eigen_vec.nrow(),1);
  int lwork=eigen_vec.nr*3-1;
  double* work=(double*)malloc(sizeof(double)*lwork);
  int info;
  
  dsyev_("V",
	 "L",
	 &eigen_vec.nr,
	 eigen_vec.dat,
	 &eigen_vec.nr,
	 eigen_value.dat,
	 work,
	 &lwork,
	 &info
	 );

  free(work);
}

template<>
Matrix<double>
solve_gels (const Matrix<double>& a,
	    const Matrix<double>& B){// AX=B -> return X
  Matrix<double> A=a;
  Matrix<double> X=B;
  Matrix<double> R(A.nc,B.nc);
  char trans='N';
  int mn=max(A.nr,A.nc);
  int lwork=mn+max(mn,X.nc);
  double* work=(double*)malloc(sizeof(double)*lwork);
  int info;
  dgels_(&trans,
	 &A.nr,
	 &A.nc,
	 &X.nc,
	 A.dat,
	 &A.nr,
	 X.dat,
	 &X.nr,
	 work,
	 &lwork,
	 &info);
  free(work);
  mat_for(R)
    R(r,c)=X(r,c);
  return R;
}

template<>
int
pca(const Matrix<double>& a,
    Matrix<double>& eigen_value,
    Matrix<double>& eigen_vec){
  Matrix<double> av=ave_row(a);
  Matrix<double> b=a;
  for(int y=0;y<a.nc;y++){
    for(int x=0;x<a.nr;x++)
      b(x,y)=b(x,y)-av(0,y);
  }
  
  // Matrix<double> rr=t(b)*b;
  // mat_for(rr)
  //   rr(r,c)=rr(r,c)/a.nr;
  Matrix<double> rr(b.nc,b.nc);
  
  double alpha=1.0/a.nr;
  double beta=0;

  dgemm_("T",
	 "N",
	 &b.nc,&b.nc,&b.nr,
	 &alpha,
	 b.dat,&b.nr,
	 b.dat,&b.nr,
	 &beta,
	 rr.dat,&rr.nr
	 );
  
  eig_sym(rr,eigen_value,eigen_vec);

  int step=1;
  return idamax_(&eigen_value.nr,eigen_value.dat,&step)-1;
}
