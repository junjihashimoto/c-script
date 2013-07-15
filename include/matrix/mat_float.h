#include <matrix/mat.h>
template<> Matrix<float>&  Matrix<float>::operator += (const Matrix<float>& b);
template<> Matrix<float>&  Matrix<float>::operator -= (const Matrix<float>& b);
template<> Matrix<float>&  Matrix<float>::operator *= (float b);
template<> Matrix<float>&  Matrix<float>::operator /= (float b);
template<> Matrix<float> operator - (Matrix<float> a);
template<> Matrix<float> operator * (const Matrix<float>& a,const Matrix<float>& b);
template<> float nrm2(const Matrix<float>& a);
template<> float dot(const Matrix<float>& a,const Matrix<float>& b);
template<> void eig_sym (const Matrix<float>& input,
			 Matrix<float>& eigen_value,
			 Matrix<float>& eigen_vec);
template<> Matrix<float> solve_gels (const Matrix<float>& a,
				      const Matrix<float>& B);// AX=B -> return X
template<> int pca(const Matrix<float>& a,
		   Matrix<float>& eigen_value,
		   Matrix<float>& eigen_vec);
