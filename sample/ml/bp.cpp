#!/usr/bin/env c-script

double
sigm(double v){
  return 1.0/(1.0+exp(-v));
}
const Matrix<double>&
h(const Matrix<double>& X){
  Matrix<double> a(X.nr,X.nc);
  mat_for(a)
    a(r,c)=sigm(X(r,c));
  return a;
}
const Matrix<double>&
hd(const Matrix<double>& X){
  Matrix<double> a(X.nr,X.nc);
  mat_for(a)
    a(r,c)=sigm(X(r,c))*(1.0-sigm(X(r,c)));
  return a;
}

template<int N>
struct BP{
  Matrix<double> X[N+1];
  Matrix<double> S[N];
  Matrix<double> ES[N];
  Matrix<double> W[N]; 
  Matrix<double> EW[N]; 
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
      X[i+1]=h(S);
    }
    return X[N];
  }
  void
  backward(const Matrix<double>& D){
    //E/Wn1ij=(Xn1i-Di)*hd(Sn1i)*Xnj
    //E/Wnij=sumj(E/Sn1j*Wn1ji)*hd(Sni)*X3j
    //E/Sn1i =(Xn1i-Dn1i)*hd(Sn1i);
    //E/Sni =sumj(E/Sn1j*Wn1ji)*hd(Sni);
    for(int n=N-1;n>=0;n--){
      if(n==N-1){
	ES[n]=x(X[n+1]-D,hd(S[n]));
	EW[n]=ES[n]*t(X[n]);
      }else{
	ES[n]=x(t(W[n])*ES[n+1],hd(S[n]));
	EW[n]=ES[n]*t(X[n]);
      }
    }
    for(int n=N-1;n>=0;n--){
      W[n]+=EW[n]*d;
    }
  }
  void
  update(const Matrix<double>& Y,
	 const Matrix<double>& D
	 ){
    forward(Y);
    backward(D);
  }
};

int
main(){
  BP<1>  bp;
  
  return 0;
}
