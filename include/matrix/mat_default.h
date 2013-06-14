#include "mat.h"

template<class T>
Matrix<T>::Matrix(){
  dat=NULL;
  nr=0;
  nc=0;
}

template<class T>
Matrix<T>::Matrix(int nr){
  init(nr,1);
}

template<class T>
Matrix<T>::Matrix(int nr,int nc){
  init(nr,nc);
}

template<class T>
Matrix<T>::Matrix(int nr,int nc,T* d){
  init(nr,nc);
  memcpy(dat,d,sizeof(T)*nc*nr);
}

template<class T>
Matrix<T>::Matrix(int nr,int nc,const Matrix& m,int r_offset,int c_offset){
  init(nr,nc);
  for(int c=0;c<nc;c++)
    for(int r=0;r<nr;r++)
      dat[nr*c+r]=m(r+r_offset,c+c_offset);
}


template<class T>
Matrix<T>::Matrix(const Matrix& m){
  init(m.nr,m.nc);
  memcpy(dat,m.dat,sizeof(T)*nc*nr);
}

template<class T>
Matrix<T>::~Matrix(){
  if(dat!=NULL)
    free(dat);
  dat=NULL;
}


template<class T>
void
Matrix<T>::init(int nr,int nc){
  this->nr=nr;
  this->nc=nc;
  dat=(T*)malloc(nr*nc*sizeof(T));
}


template<class T>
const Matrix<T>
Matrix<T>::row(int r){
  return Matrix<T>(1,nc,*this,r,0);
}

template<class T>
const Matrix<T>
Matrix<T>::col(int c){
  return Matrix<T>(nr,1,dat+nr*c);
}


template<class T>
void
Matrix<T>::set(const T* ary){
  memcpy(dat,ary,sizeof(T)*nc*nr);
}

template<class T>
void
Matrix<T>::set_row(int row,const T* ary){
  for(int c=0;c<nc;c++)
    dat[nr*c+row]=ary[c];
}

template<class T>
void
Matrix<T>::set_col(int col,const T* ary){
  memcpy(dat+col*nr,ary,sizeof(T)*col);
}

template<class T>
void
Matrix<T>::zero(){
  memset(dat,0,nr*nc*sizeof(T));
}

template<class T>
void
Matrix<T>::one(){
  mat_for((*this))
    (*this)(r,c)=1;
}


template<class T>
void
Matrix<T>::swap(Matrix<T>& mat){
  T* d=this->dat;
  this->dat=mat.dat;
  mat.dat=d;

  int  a=this->nr;
  this->nr=mat.nr;
  mat.nr=a;
    
  a=this->nc;
  this->nc=mat.nc;
  mat.nc=a;
}

template<class T>
void
Matrix<T>::write(FILE* out,const char* format)const{
  for(int r=0;r<nrow();r++){
    for(int c=0;c<ncol();c++){
      if(format==NULL)
	fprintf(out,"%3d",(int)((*this)(r,c)));
      else
	fprintf(out,format,(*this)(r,c));
      fprintf(out,"%c",(c==ncol()-1 ? ' ' : ','));
    }
    fprintf(out,"\n");
  }
  fprintf(out,"\n");
}

template<class T>
void
Matrix<T>::read (FILE* in,const char* format){
  char buf[2048];
  for(int r=0;r<nr;r++){
    if(fgets(buf,sizeof(buf),in)==NULL)
      break;
    char *p=buf;
    char *n;
    int d;
    T   dd;
    int c=0;
    while(1){
      //skip space or '"'
      while(*p==' '||*p=='"')
	p++;
      n=p;
      //seach ',' or 0
      while(*p!=','&&*p!=0)
	p++;

      if(format==NULL)
	sscanf(n,"%d",&d);
      else
	sscanf(n,format,&dd);
      (*this)(r,c)=dd;

      if(*p==0)
	break;
      else if(*p==',')
	p++;
      c++;
    }
  }
}

template<class T>
int
Matrix<T>::nrow() const{
  return nr;
}

template<class T>
int
Matrix<T>::ncol() const{
  return nc;
}

template<class T>
int
Matrix<T>::size() const{
  return nc*nr;
}


template<class T>
T&
Matrix<T>::operator()(int row,int col){
  return dat[row+col*nr];
}

template<class T>
const T&
Matrix<T>::operator()(int row,int col)const{
  return dat[row+col*nr];
}


template<class T>
T&
Matrix<T>::operator[](int i){
  return dat[i];
}

template<class T>
const T&
Matrix<T>::operator[](int i)const{
  return dat[i];
}

template<class T>
Matrix<T>&
Matrix<T>::operator=(const Matrix& m){
  if(dat!=NULL){
    free(dat);
    dat=NULL;
  }
    
  nr=m.nr;
  nc=m.nc;
  dat=(T*)malloc(nr*nc*sizeof(T));

  memcpy(dat,m.dat,sizeof(T)*nc*nr);
  return *this;
}
  
template<class T>
Pair<T>
Matrix<T>::operator<<(T v){
  dat[0]=v;
  return Pair<T>(this,1);
}


template<class T>
Matrix<T>& 
Matrix<T>::operator += (const Matrix<T>& b){
  for(int k=0;k<this->nc;k++)
    for(int i=0;i<this->nr;i++)
      (*this)(i,k)+=b(i,k);
  return *this;
}

template<class T>
Matrix<T>& 
Matrix<T>::operator += (T b){
  for(int k=0;k<this->nc;k++)
    for(int i=0;i<this->nr;i++)
      (*this)(i,k)+=b;
  return *this;
}


template<class T>
Matrix<T>& 
Matrix<T>::operator -= (const Matrix<T>& b){
  for(int k=0;k<this->nc;k++)
    for(int i=0;i<this->nr;i++)
      (*this)(i,k)-=b(i,k);
  return *this;
}


template<class T>
Matrix<T>& 
Matrix<T>::operator -= (T b){
  for(int k=0;k<this->nc;k++)
    for(int i=0;i<this->nr;i++)
      (*this)(i,k)-=b;
  return *this;
}

template<class T>
Matrix<T>& 
Matrix<T>::operator *= (const Matrix<T>& b){
  Matrix<T> a=(*this);
  (*this)=a*b;
  return *this;
}

template<class T>
Matrix<T>& 
Matrix<T>::operator *= (T b){
  for(int k=0;k<this->nc;k++)
    for(int i=0;i<this->nr;i++)
      (*this)(i,k)*=b;
  return *this;
}




template<class T>
Matrix<T>& 
Matrix<T>::operator /= (T b){
  for(int k=0;k<this->nc;k++)
    for(int i=0;i<this->nr;i++)
      (*this)(i,k)/=b;
  return *this;
}


template<class T>
Matrix<T>& 
Matrix<T>::operator /= (const Matrix<T>& b){
  not_implemented();
}

template<class T>
Matrix<T>
Matrix<T>::x(const Matrix<T>& b)const{
  return ary_mul(*this,b);
}

template<class T>
Matrix<T>
operator + (const Matrix<T>& a,const Matrix<T>& b){
  Matrix<T> c=a;
  return (c+=b);
}

template<class T>
Matrix<T> operator + (const Matrix<T>& a,T b){
  Matrix<T> c=a;
  return (c+=b);
}

template<class T>
Matrix<T> operator - (const Matrix<T>& a,const Matrix<T>& b){
  Matrix<T> c=a;
  return (c-=b);
}

template<class T>
Matrix<T> operator - (const Matrix<T>& a,T b){
  Matrix<T> c=a;
  return (c-=b);
}


template<class T>
Matrix<T> operator - (const Matrix<T>& a){
  Matrix<T> c=a;
  for(int j=0;j<c.nc;j++)
    for(int i=0;i<c.nr;i++)
      c(i,j)=-c(i,j);
  return c;
}



template<class T>
Matrix<T> operator * (const Matrix<T>& a,const Matrix<T>& b){
  Matrix<T> c(a.nrow(),b.ncol());
  c.zero();
  for(int j=0;j<c.nc;j++)
    for(int k=0;k<a.nc;k++)
      for(int i=0;i<c.nr;i++)
	c(i,j)+=a(i,k)*b(k,j);
  return c;
}



template<class T>
Matrix<T> operator * (const Matrix<T>& a,T b){
  Matrix<T> c=a;
  return (c*=b);
}

template<class T>
Matrix<T> operator / (const Matrix<T>& a,T b){
  Matrix<T> c=a;
  return (c/=b);
}

template<class T>
Matrix<T> operator / (const Matrix<T>& a,const Matrix<T>& b){
  not_implemented();
}

template<class T>
Matrix<T>
eye        (int nr,int nc){
  int n=MIN(nr,nc);
  Matrix<T> a(nr,nc);
  a.zero();
  for(int i=0;i<n;i++)
    a(i,i)=1;
  return a;
}

template<class T>
Matrix<T>
zero       (int nr,int nc){
  Matrix<T> a(nr,nc);
  a.zero();
  return a;
}


template<class T>
Matrix<T>
one       (int nr,int nc){
  Matrix<T> a(nr,nc);
  mat_for(a)
    a(r,c)=1;
  return a;
}


template<class T>
Matrix<T> ary_mul    (const Matrix<T>& a,const Matrix<T>& b){
  Matrix<T> c=a;
  for(int k=0;k<a.nc;k++)
    for(int i=0;i<a.nr;i++)
      c(i,k)*=b(i,k);
  return c;
}

template<class T>
Matrix<T> x(const Matrix<T>& a,const Matrix<T>& b){
  return ary_mul(a,b);
}

template<class T>
Matrix<T> ary_div    (const Matrix<T>& a,const Matrix<T>& b){
  Matrix<T> c=a;
  for(int k=0;k<a.nc;k++)
    for(int i=0;i<a.nr;i++)
      c(i,k)/=b(i,k);
  return c;
}

template<class T>
bool      operator ==(const Matrix<T>& a,const Matrix<T>& b){
  if(a.nrow()==b.nrow()&&
     a.ncol()==b.ncol()){
    for(int c=0;c<a.ncol();c++)
      for(int r=0;r<a.nrow();r++)
	if(a(r,c)!=b(r,c))
	  return false;
    return true;
  }else{
    return false;
  }
}

template<class T>
bool      operator !=(const Matrix<T>& a,const Matrix<T>& b){
  return !(a==b);
}

template<class T>
Matrix<T> operator ^ (const Matrix<T>& a,int v){
  not_implemented();
}

template<class T>
Matrix<T>
inv        (const Matrix<T>& a){
  Matrix<T> e=eye<T>(a.nr,a.nc);
  return solve_gels(a,e);
}


template<class T>
T
nrm2(const Matrix<T>& a){
  not_implemented();
}



template<class T>
Matrix<T>
t(const Matrix<T>& a){// return At
  Matrix<T> cc(a.ncol(),a.nrow());
  mat_for(cc)
    cc(r,c)=a(c,r);
  return cc;
}

template<class T>
T
dot(const Matrix<T>& a,const Matrix<T>& b){
  T v=0;
  mat_for(a)
    v+=a(c,r)*b(c,r);
  return v;
}


template<class T>
T
ave(const Matrix<T>& a){
  T t=0;
  mat_for(a)
    t+=a(r,c);
  return t/(a.nr*a.nc);
}


template<class T>
Matrix<T>
ave_row    (const Matrix<T>& a){
  Matrix<T> r(1,a.nc);
  for(int y=0;y<a.nc;y++){
    r(0,y)=0;
    for(int x=0;x<a.nr;x++)
      r(0,y)+=a(x,y);
    r(0,y)/=(a.nr);
  }
  return r;
}

template<class T>
Matrix<T>
ave_col    (const Matrix<T>& a){
  Matrix<T> r(a.nr,1);
  for(int x=0;x<a.nr;x++)
    r(x,0)=0;
  for(int y=0;y<a.nc;y++){
    for(int x=0;x<a.nr;x++)
      r(x,0)+=a(x,y);
  }
  for(int x=0;x<a.nr;x++)
    r(x,0)/=(a.nc);
  return r;
}

template<class T> T
var        (const Matrix<T>& a){
  T average=ave(a);
  T t=0;
  mat_for(a)
    t+=(a(r,c)-average)*(a(r,c)-average);
  return t/(a.nr*a.nc);
}

template<class T>
Matrix<T>
var_row    (const Matrix<T>& a){
  Matrix<T> av=ave_row(a);
  Matrix<T> r(1,a.nc);
  for(int y=0;y<a.nc;y++){
    r(0,y)=0;
    for(int x=0;x<a.nr;x++)
      r(0,y)+=(a(x,y)-av(0,y))*(a(x,y)-av(0,y));
    r(0,y)=r(0,y)/(a.nr);
  }
  return r;
}

template<class T>
Matrix<T>
var_col    (const Matrix<T>& a){
  Matrix<T> av=ave_col(a);
  Matrix<T> r(a.nr,1);
  for(int x=0;x<a.nr;x++)
    r(x,0)=0;
  for(int y=0;y<a.nc;y++)
    for(int x=0;x<a.nr;x++)
      r(x,y)+=(a(x,y)-av(x,0))*(a(x,y)-av(x,0));
  for(int x=0;x<a.nr;x++)
    r(x,0)=r(x,0)/(a.nc);
  return r;
}

template<class T> T
sum        (const Matrix<T>& a){
  T t=0;
  mat_for(a)
    t+=a(r,c);
  return t;
}


template<class T>
void
eig_sym    (const Matrix<T>& input,
	    Matrix<T>& eigen_value,
	    Matrix<T>& eigen_vec){
  not_implemented();
}

template<class T>
Matrix<T>
solve_gels (const Matrix<T>& a,
	    const Matrix<T>& B){// AX=B -> return X
  not_implemented();
}

template<class T>
Matrix<T>
solve_yacobi(const Matrix<T>& a,
	     const Matrix<T>& B,
	     const Matrix<T>& init,
	     int num){
  Matrix<T> A=t(a)*a;
  Matrix<T> Y=t(a)*B;
  Matrix<T> LU(A.nr,A.nc);
  Matrix<T> D(A.nr,B.nc);
  Matrix<T> X(A.nc,B.nc);
  mat_for(X)
    X(r,c)=init(r,c);

  mat_for(A)
    if(r==c){
      for(int i=0;i<D.nc;i++)
	D(r,i)=A(r,c);
      LU(r,c)=0;
    }else{
      LU(r,c)=A(r,c);
    }

  for(int i=0;i<num;i++){
    X=ary_div((Y-LU*X),D);
  }

  return X;
}

template<class T>
int
pca(const Matrix<T>& a,
    Matrix<T>& eigen_value,
    Matrix<T>& eigen_vec){
  not_implemented();
  return 0;
}

template<class T,class Func>
Matrix<T>
MAP(const Matrix<T>& X,Func v){
  Matrix<T> a(X.nr,X.nc);
  mat_for(a)
    a(r,c)=v(X(r,c));
  return a;
}

template<class T,class Func>
Matrix<T>
FOLDR(const Matrix<T>& X,Func v,T initv){
  T t=initv;
  mat_for(X)
    t+=X(r,c);
  return t;
}
