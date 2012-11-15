extern "C"{
  void sswap_(int* N,float* SX,int* INCX,float* SY,int* INCY );
  void scopy_(const int* N,const float* DX,const int* INCX,float* DY,const int* INCY);
  void sscal_(int* N,float* DA,float* DX,int* INCX);
  int  isamax_(int* N,float* DX,int* INCX);
  void sgetrf_(int*  M,int* N,float* A,int* LDA,int* IPIV,int* INFO);
  void sgetri_(int*  N,float* A,int* LDA,int* IPIV,float* work,int* lwork,int* INFO);
  void sgetrs_(char* TRANS,int* N,int* NRHS,
	       float* A,int* LDA,int* IPIV,
	       float* B,int* LDB,int* INFO);

  float sdot_(const int* N,float* DX,int* INCX,float* DY,int* INCY);
  void  saxpy_(const int* N,float* DA,float* DX,int* INCX,float* DY,int* INCY);
  float snrm2_(const int* N,const float* X,const int* INCX);
  void  sgemm_(const char* TRANSA,const char* TRANSB,
	       const int* M,const int* N,const int* K,
	       float* ALPHA,
	       const float* A,const int* LDA,
	       const float* B,const int* LDB,
	       float* BETA,float* C,int* LDC );

  void sgeev_(const char* JOBVL,const char* JOBVR,
	      int* N,float* A,int* LDA,
	      float* WR,float* WI,
	      float* VL,int* LDVL,
	      float* VR,int* LDVR,
	      float* WORK,int* LWORK,int* INFO );

  void sggev_(const char* JOBVL,const char* JOBVR,
	      int* N,float* A,int* LDA,float* B,int* LDB,float* ALPHAR,float* ALPHAI,
	      float* BETA,
	      float* VL,int* LDVL,
	      float* VR,int* LDVR,
	      float* WORK,int* LWORK,
	      int* INFO );
  void sgels_(const char* TRANS,int*  M,int* N,int* NRHS,
	      float* A,int* LDA,float* B,int* LDB,
	      float* WORK,int* LWORK,int* INFO);

  void ssyev_(const char* JOBZ,const char* UPLO,int* N,
	      float* A,int* LDA,
	      float* W,float* WORK,int* LWORK,int* INFO );

}


template<>
Matrix<float>& 
Matrix<float>::operator += (const Matrix<float>& b){
  float alpha=1.0;
  int n=nrow()*ncol();
  int s=1;
  //b=a+alpha*b
  saxpy_(&n,
	 &alpha,
	 b.dat,
	 &s,
	 dat,
	 &s);

  return *this;
}

template<>
Matrix<float>& 
Matrix<float>::operator -= (const Matrix<float>& b){
  float alpha=-1.0;
  int n=nrow()*ncol();
  int s=1;
  //b=a+alpha*b
  saxpy_(&n,
  	 &alpha,
  	 b.dat,
  	 &s,
  	 dat,
  	 &s);
  sscal_(&n,
	 &alpha,
	 dat,
	 &s);
  return *this;
}



template<>
Matrix<float>& 
Matrix<float>::operator *= (float b){
  float alpha=b;
  int n=nrow()*ncol();
  int s=1;
  //a=ax
  sscal_(&n,
	 &alpha,
	 dat,
	 &s);
  return *this;
}

template<>
Matrix<float>& 
Matrix<float>::operator /= (float b){
  float alpha=1.0/b;
  int n=nrow()*ncol();
  int s=1;
  //a=ax
  sscal_(&n,
	 &alpha,
	 dat,
	 &s);
  return *this;
}

template<>
Matrix<float> operator - (const Matrix<float>& a){
  Matrix<float> c=a;
  float alpha=-1.0;
  int n=a.nrow()*a.ncol();
  int s=1;
  //a=ax
  sscal_(&n,
	 &alpha,
	 c.dat,
	 &s);
  return c;
}



template<>
Matrix<float> operator * (const Matrix<float>& a,const Matrix<float>& b){
  Matrix<float> c(a.nrow(),b.ncol());
  float alpha=1.0;
  float beta=0;
  
  assert(b.nrow()==a.ncol());
  char tran='N';
  sgemm_(&tran,
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
float
nrm2(const Matrix<float>& a){
  int s=1;
  int n=a.nc*a.nr;
  return snrm2_(&n,a.dat,&s);
}

template<>
float
dot(const Matrix<float>& a,const Matrix<float>& b){
  assert(a.ncol()==1&&b.ncol()==1);
  int s=1;
  return sdot_(&a.nr,
	       a.dat,
	       &s,
	       b.dat,
	       &s);
}

template<>
void
eig_sym    (const Matrix<float>& input,
	    Matrix<float>& eigen_value,
	    Matrix<float>& eigen_vec){
  eigen_vec=input;
  assert(eigen_vec.nr==eigen_vec.nc);
  eigen_value.init(eigen_vec.nrow(),1);
  int lwork=eigen_vec.nr*3-1;
  float* work=(float*)malloc(sizeof(float)*lwork);
  int info;
  
  ssyev_("V",
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
Matrix<float>
solve_gels (const Matrix<float>& a,
	    const Matrix<float>& B){// AX=B -> return X
  Matrix<float> A=a;
  Matrix<float> X=B;
  Matrix<float> R(A.nc,B.nc);
  char trans='N';
  int mn=max(A.nr,A.nc);
  int lwork=mn+max(mn,X.nc);
  float* work=(float*)malloc(sizeof(float)*lwork);
  int info;
  sgels_(&trans,
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
pca(const Matrix<float>& a,
    Matrix<float>& eigen_value,
    Matrix<float>& eigen_vec){
  Matrix<float> av=ave_row(a);
  Matrix<float> b=a;
  for(int y=0;y<a.nc;y++){
    for(int x=0;x<a.nr;x++)
      b(x,y)=b(x,y)-av(0,y);
  }
  
  // Matrix<float> rr=t(b)*b;
  // mat_for(rr)
  //   rr(r,c)=rr(r,c)/a.nr;
  Matrix<float> rr(b.nc,b.nc);
  
  float alpha=1.0/a.nr;
  float beta=0;

  sgemm_("T",
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
  return isamax_(&eigen_value.nr,eigen_value.dat,&step)-1;
}
