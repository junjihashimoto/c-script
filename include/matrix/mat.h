#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstdarg>
#include <cstring>
#include <string>
using namespace std;

#define not_implemented() {printf("Not implemented:%s\n",__func__);exit(1);}
#define MIN(x,y) ((x)<=(y) ? (x) : (y))
#define MAX(x,y) ((x)>=(y) ? (x) : (y))

#define mat_for(mat) \
  for(int c=0;c<mat.ncol();c++)\
    for(int r=0;r<mat.nrow();r++)


template<class T> struct Matrix;

template<class T>
struct Pair{
  Matrix<T>*  dat;
  int         pos;
  Pair(Matrix<T>* d,int p){
    dat=d;
    pos=p;
  }
  Pair&       operator, (T v){
    int c=pos%dat->nc;
    int r=pos/dat->nc;
    dat->dat[(dat->nr)*c+r]=v;
    pos++;
    return *this;
  }
};


template<class T>
struct Matrix{//Column Major
  T *dat;
  
  int nr;
  int nc;
  /*
   <---------> nc
  A0 4 ...   N
  |1 5 (r,c)
  |2 6 
  V3 7
  nr == 4

  */
  Matrix();
  Matrix(int nr,int nc);
  Matrix(int nr,int nc,T* d);
  Matrix(int nr,int nc,const Matrix& m,int nr_offset,int nc_offset);
  Matrix(const Matrix& m);
  ~Matrix();

  void     init(int nr,int nc);

  const Matrix  row(int r);
  const Matrix  col(int c);

  void     set(const T* ary);
  void     set_row(int row,const T* ary);
  void     set_col(int col,const T* ary);
  void     zero();
  void     swap(Matrix& mat);
  void     write(FILE* out=stdout,const char* format=NULL)const;
  void     read (FILE* in =stdin ,const char* format=NULL);
  int      nrow() const;
  int      ncol() const;

  T&       operator()(int row,int col);
  const T& operator()(int row,int col)const;
  Matrix<T>& operator=(const Matrix<T>& m);
  
  Pair<T>  operator<<(T v);
  
  Matrix<T>& operator += (const Matrix<T>& b);
  Matrix<T>& operator += (T b);
  Matrix<T>& operator -= (const Matrix<T>& b);
  Matrix<T>& operator -= (T b);
  Matrix<T>& operator *= (const Matrix<T>& b);
  Matrix<T>& operator *= (T b);
  Matrix<T>& operator /= (T b);
  Matrix<T>& operator /= (const Matrix<T>& b);
};

template<class T> Matrix<T> operator + (const Matrix<T>& a,const Matrix<T>& b);
template<class T> Matrix<T> operator + (const Matrix<T>& a,T b);
template<class T> Matrix<T> operator - (const Matrix<T>& a,const Matrix<T>& b);
template<class T> Matrix<T> operator - (const Matrix<T>& a,T b);
template<class T> Matrix<T> operator - (const Matrix<T>& a);
template<class T> Matrix<T> operator * (const Matrix<T>& a,const Matrix<T>& b);
template<class T> Matrix<T> operator * (const Matrix<T>& a,T b);
template<class T> Matrix<T> operator / (const Matrix<T>& a,T b);
template<class T> Matrix<T> operator / (const Matrix<T>& a,const Matrix<T>& b);


template<class T> Matrix<T> eye        (int nr,int nc);
template<class T> Matrix<T> zero       (int nr,int nc);
template<class T> Matrix<T> ary_mul    (const Matrix<T>& a,const Matrix<T>& b);
template<class T> Matrix<T> ary_div    (const Matrix<T>& a,const Matrix<T>& b);
template<class T> bool      operator ==(const Matrix<T>& a,const Matrix<T>& b);
template<class T> bool      operator !=(const Matrix<T>& a,const Matrix<T>& b);
template<class T> Matrix<T> operator ^ (const Matrix<T>& a,int v);
template<class T> Matrix<T> inv        (const Matrix<T>& a);
template<class T> T         nrm2       (const Matrix<T>& a);
template<class T> Matrix<T> t          (const Matrix<T>& a);// return At
template<class T> T         dot        (const Matrix<T>& a,const Matrix<T>& b);
template<class T> T         ave        (const Matrix<T>& a);
template<class T> Matrix<T> ave_row    (const Matrix<T>& a);
template<class T> Matrix<T> ave_col    (const Matrix<T>& a);
template<class T> T         var        (const Matrix<T>& a);
template<class T> Matrix<T> var_row    (const Matrix<T>& a);
template<class T> Matrix<T> var_col    (const Matrix<T>& a);

template<class T> void      eig_sym    (const Matrix<T>& input,
					const Matrix<T>& eigen_value,
					const Matrix<T>& eigen_vec);

template<class T> Matrix<T> solve_gels (const Matrix<T>& a,
					const Matrix<T>& B);// AX=B -> return X

template<class T> Matrix<T> solve_yacobi(const Matrix<T>& a,
					 const Matrix<T>& B,
					 const Matrix<T>& init,
					 int num);


template<class T> int pca(const Matrix<T>& a,
			  Matrix<T>& eigen_value,
			  Matrix<T>& eigen_vec);

#define solve(a,b) solve_gels(a,b)

#include "mat_default.h"
#include "mat_double.h"
#include "mat_float.h"

#endif
