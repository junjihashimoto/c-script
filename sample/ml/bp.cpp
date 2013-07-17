#!/usr/bin/env c-script
// #pragma c-script:eval_begin
// $default_header="#define DEBUG_MATRIX\n".$default_header;
// #pragma c-script:eval_end
template<class T>
T
sigm(T v){
  return 1.0/(1.0+exp(-v));
}

template<>
float
sigm(float v){
  return 1.0f/(1.0f+expf(-v));
}

template<class T>
T
sigmd(T v){
  return sigm(v)*(1.0-sigm(v));
}

template<class T>
Matrix<T>
h(Matrix<T> X){
  Matrix<T> a=MAP(X,sigm<T>);
  return a;
}

template<class T>
Matrix<T>
hd(Matrix<T> X){
  Matrix<T> a=MAP(X,sigmd<T>);
  return a;
}

template<class T>
struct BP{
  int N;
  Matrix<T>* X;//[N+1];
  Matrix<T>* S;//[N];
  Matrix<T>* ES;//[N];
  Matrix<T>* W;//[N]; 
  Matrix<T>* EW;//[N];
  vector<int>     hier;
  T d;
  
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
    X	=new Matrix<T>[N+1];
    S	=new Matrix<T>[N];
    ES	=new Matrix<T>[N];
    W	=new Matrix<T>[N];
    EW	=new Matrix<T>[N];
    
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
  
  const Matrix<T>&
  forward(const Matrix<T>& Y){
    X[0]=Y;
    for(int i=0;i<N;i++){
      S[i]=W[i]*X[i];
      X[i+1]=h(S[i]);
    }
    return X[N];
  }
  void
  backward(const Matrix<T>& D){
    //E=sumi(Xn1i-Di)^2
    //E/Wn1ij=(Xn1i-Di)*hd(Sn1i)*Xnj
    //E/Wnij=sumj(E/Sn1j*Wn1ji)*hd(Sni)*X3j
    //E/Sn1i =(Xn1i-Dn1i)*hd(Sn1i);
    //E/Sni =sumj(E/Sn1j*Wn1ji)*hd(Sni);
    // printf("start\n");
    for(int n=N-1;n>=0;n--){
      if(n==N-1)
        ES[n]=x(X[n+1]-D,hd(S[n]));
      else
        ES[n]=x(t(t(ES[n+1])*W[n+1]),hd(S[n]));
      //      EW[n]=ES[n]*t(X[n]);
    }
    for(int n=N-1;n>=0;n--){
      //      W[n]+=EW[n]*d;
      W[n]+=ES[n]*t(X[n])*d;
    }
    // printf("end\n");
    //    exit(0);
  }
  void
  update(const Matrix<T>& IN,
	 const Matrix<T>& EXP
	 ){
    forward(IN);
    backward(EXP);
  }
};


template<class T>
int
test(){
  BP<T> bp;
  bp<<
    2,//input
    256*256,//inernal
    16*16,//inernal
    4*4,//inernal
    2*2,//inernal
    1;//output
  bp.init();
  for(int i=0;i<bp.hier.size();i++)
    printf("%d\n",bp.hier[i]);
  for(int i=0;i<10;i++){
    Matrix<T> in(2);
    Matrix<T> out(1);
    in[0]=(T)(rand()%2);
    in[1]=(T)(rand()%2);
    out[0]=(T)((int)in[0]^(int)in[1]);
    bp.update(in,out);
    printf("%d\n",i);
  }

  
  return 0;
}
int
main(){
  test<float>();
  test<double>();
  return 0;
}
