#!/usr/bin/env c-script

struct SVM{
  Matrix<double> XX;
  Matrix<double> YY;
  Matrix<double> X;
  Matrix<double> Y;
  Matrix<double> L;
  Matrix<double> LL;
  Matrix<double> dL;
  double         d;
  Matrix<double> w;
  Matrix<double> ones;
  int nr,nc;
  void init(int nr,int nc){
    this->nr=nr;
    this->nc=nc;
    X.init(nr,nc);
    Y.init(nr,1);
    L.init(nr,1);
    dL.init(nr,1);
    d=0.01;
    XX.init(nr,nr);
    YY.init(nr,nr);
    LL.init(nr,nr);
    w.init(nc,1);
    ones.init(nr,1);
    ones.one();
  }
  void set(){
    X<<
      0,0,
      0,0.5,
      0,1.0,
      0.5,0,
      0.5,0.5,
      0.5,1.0,
      1,0,
      1,0.5,
      1,1.0;
    Y<<
      -1,
      -1,
      -1,
      -1,
      -1,
      1,  
      1,  
      1,  
      1;
    L.zero();
    
    XX=X*t(X);
    YY=Y*t(Y);
  }
  double cost(){
    return sum(L)-0.5*sum(ary_mul(ary_mul(XX,YY),LL));
  }
  void update(int i,int n){
    LL=L*t(L);
    if(n/(i+1)<3)
      L+=(ones-ary_mul(XX,YY)*L-YY*L*20.0)*d;
    else
      L+=(ones-ary_mul(XX,YY)*L-YY*L*20.0)*d;
    mat_for(L){
      if(L(r,c)<0)
	L(r,c)=0;
    }
    w=t(X)*ary_mul(L,Y);

  }
};

int
main(){
  SVM svm;
  svm.init(9,2);
  svm.set();
  int n=100000;
  for(int i=0;i<100000;i++){
    svm.update(i,n);
  }
  
  printf("L:\n");
  svm.L.write(stdout,"%2.2f");
  printf("w:\n");
  svm.w.write(stdout,"%2.2f");
  printf("LY:%f\n",dot(svm.L,svm.Y));
  printf("cost:%f\n",svm.cost());

  
  return 0;
}
