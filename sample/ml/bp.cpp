#!/usr/bin/env c-script

double
sigm(double v){
  return 1.0/(1.0+exp(-v));
}

double
sigmd(double v){
  return sigm(v)*(1.0-sigm(v));
}


Matrix<double>
h(const Matrix<double>& X){
  return MAP(X,sigm);
}

Matrix<double>
hd(const Matrix<double>& X){
  return MAP(X,sigmd);
}

struct BP{
  int N;
  Matrix<double>* X;//[N+1];
  Matrix<double>* S;//[N];
  Matrix<double>* ES;//[N];
  Matrix<double>* W;//[N]; 
  Matrix<double>* EW;//[N];
  vector<int>     hier;
  double d;
  
  BP&
  operator <<(int i){
    hier.clear();
    hier.push_back(i);
    return *this;
  }
  BP&
  operator ,(int i){
    hier.push_back(i);
    return *this;
  }
  BP(){
    X	=NULL;
    S	=NULL;
    ES	=NULL;
    W	=NULL;
    EW	=NULL;
    d   =0.01;
  }
  ~BP(){
    delete[] X;
    delete[] S;
    delete[] ES;
    delete[] W;
    delete[] EW;
  }
  void
  init(){
    N	=hier.size()-1;
    X	=new Matrix<double>[N+1];
    S	=new Matrix<double>[N];
    ES	=new Matrix<double>[N];
    W	=new Matrix<double>[N];
    EW	=new Matrix<double>[N];
    
    X[N].init(hier[N],1);
    
    for(int i=0;i<N;i++){
      X [i].init(hier[i],1);
      S [i].init(hier[i+1],1);
      ES[i].init(hier[i+1],1);
      W [i].init(hier[i+1],hier[i]);
      EW[i].init(hier[i+1],hier[i]);
    }
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
      X[i+1]=h(S[i]);
    }
    return X[N];
  }
  void
  backward(const Matrix<double>& D){
    //E=sumi(Xn1i-Di)^2
    //E/Wn1ij=(Xn1i-Di)*hd(Sn1i)*Xnj
    //E/Wnij=sumj(E/Sn1j*Wn1ji)*hd(Sni)*X3j
    //E/Sn1i =(Xn1i-Dn1i)*hd(Sn1i);
    //E/Sni =sumj(E/Sn1j*Wn1ji)*hd(Sni);
    
    for(int n=N-1;n>=0;n--){
      if(n==N-1)
	ES[n]=x(X[n+1]-D,hd(S[n]));
      else
	ES[n]=x(t(W[n+1])*ES[n+1],hd(S[n]));
      EW[n]=ES[n]*t(X[n]);
    }
    for(int n=N-1;n>=0;n--){
      W[n]+=EW[n]*d;
    }
  }
  void
  update(const Matrix<double>& IN,
	 const Matrix<double>& EXP
	 ){
    forward(IN);
    backward(EXP);
  }
};


int
main(){
  BP bp;
  bp<<
    2,//input
    10,//inernal
    9,//inernal
    8,//inernal
    7,//inernal
    1;//output
  bp.init();
  for(int i=0;i<bp.hier.size();i++)
    printf("%d\n",bp.hier[i]);
  for(int i=0;i<10000;i++){
    Matrix<double> in(2);
    Matrix<double> out(1);
    in[0]=(double)(rand()%2);
    in[1]=(double)(rand()%2);
    out[0]=(double)((int)in[0]^(int)in[1]);
    bp.update(in,out);    
  }

  
  return 0;
}
