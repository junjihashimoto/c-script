#include <matrix/mat.h>
template<> Matrix<double>&  Matrix<double>::operator += (const Matrix<double>& b);
template<> Matrix<double>&  Matrix<double>::operator -= (const Matrix<double>& b);
template<> Matrix<double>&  Matrix<double>::operator *= (double b);
template<> Matrix<double>&  Matrix<double>::operator /= (double b);
template<> Matrix<double> operator - (Matrix<double> a);
template<> Matrix<double> operator * (const Matrix<double>& a,const Matrix<double>& b);
template<> double nrm2(const Matrix<double>& a);
template<> double dot(const Matrix<double>& a,const Matrix<double>& b);
template<> void eig_sym (const Matrix<double>& input,
			 Matrix<double>& eigen_value,
			 Matrix<double>& eigen_vec);
template<> Matrix<double> solve_gels (const Matrix<double>& a,
				      const Matrix<double>& B);// AX=B -> return X
template<> int pca(const Matrix<double>& a,
		   Matrix<double>& eigen_value,
		   Matrix<double>& eigen_vec);
