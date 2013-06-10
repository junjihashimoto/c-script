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
  Matrix<double> W[N]; 
  double d;
  void init(){
    
  }
  //X[i+1]=sigm(S[i]=W[i]*X[i])
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
  }

};

int
main(){
  BP<1>  bp;
  
  return 0;
}
