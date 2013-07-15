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
  double         b;
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
    return sum(L)-0.5*sum(x(x(XX,YY),LL));
  }
  void update(double d,double C){
    LL=L*t(L);
    L+=(ones-x(XX,YY)*L-YY*L*C)*d;
    mat_for(L){
      if(L(r,c)<0)
	L(r,c)=0;
    }
    w=t(X)*x(L,Y);
    b=sum(x(X*w,L))/sum(L);
  }
};

int
main(){
  SVM svm;
  svm.init(9,2);
  svm.set();
  for(int i=0;i<1000;i++)
    svm.update(0.1,1);
  for(int i=0;i<10000;i++)
    svm.update(0.01,10);
  for(int i=0;i<10000;i++)
    svm.update(0.00001,100);
  for(int i=0;i<10000;i++)
    svm.update(0.000001,1000);
  
  printf("L:\n");
  svm.L.write(stdout,"%2.2f");
  printf("w:\n");
  svm.w.write(stdout,"%2.2f");
  printf("X*w-b:\n");
  (svm.X*svm.w-svm.b).write(stdout,"%2.2f");
  printf("wx:\n");
  printf("b:%f\n",svm.b);
  printf("LY:%f\n",dot(svm.L,svm.Y));
  printf("cost:%f\n",svm.cost());

  
  return 0;
}
