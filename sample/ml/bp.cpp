#!/usr/bin/env c-script

double
sigm(double v){
  return 1.0/(1.0+exp(-v));
}
const Matrix<double>&
sigm(const Matrix<double>& X){
  Matrix<double> a(X.nr,X.nc);
  mat_for(a)
    a(r,c)=sigm(X(r,c));
  return a;
}

template<int N>
struct BP{
  Matrix<double> X[N+1];
  Matrix<double> D[N];
  Matrix<double> S[N];
  Matrix<double> ES[N];
  Matrix<double> W[N]; 
  Matrix<double> dW[N]; 
  double d;
  void init(){
    
  }
  //X[i+1]=sigm(S[i]=W[i]*X[i])
  //E/w1ij=E/S1i*S1i/w1ij
  //E/w1ij=E/X1i*X1i/S1i*S1i/w1ij
  //E/w1ij=(X1i-D1i)*h(S1i)(1-h(S1i))*X2j
  //E/w2ij=E/S2i*S2i/w2ij
  //E/w2ij=E/X2i*X2i/S2i*S2i/w2ij
  //E/w2ij=sumj(E/S1j*S1j/X2i)*X2i/S2i*S2i/w2ij
  //E/w2ij=sumj(E/S1j*w1ji)*h(S2i)*(1-h(S2i))*X3j
  //E/S1i =(X1i-D1i)*h(S1i)(1-h(S1i));
  //E/S2i =sumj(E/S1j*w1ji)*h(S2i)*(1-h(S2i));
  
  const Matrix<double>&
  forward(const Matrix<double>& Y){
    X[0]=Y;
    for(int i=0;i<N;i++){
      S[i]=W[i]*X[i];
      X[i+1]=sigm(S);
    }
    return X[N];
  }
  void back(const Matrix<double>& D){
    for(int i=N-1;i>=0;i--){
      if(i==N-1){
	
      }else{
      }
    }
  }
};

int
main(){
  BP<1>  bp;
  
  return 0;
}
