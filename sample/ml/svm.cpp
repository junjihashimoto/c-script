#!/usr/bin/env c-script

struct SVM{
Matrix<double> X;
Matrix<double> Y;
Matrix<double> XX;
Matrix<double> L;
Matrix<double> dL;
Matrix<double> W;
Matrix<double> U;
Matrix<double> one;
double         d;
  void init(){
    XX=ary_mul(X*t(X),Y*t(Y));
    mat_for(one)
      one(r,c)=1;

  }
  void update(){
    dL=(one-XX*L)*d;
    L+=dL;
  }
  double cost(){
    return (t(L)*one+(t(one)*ary_mul(XX,L*t(L))*one)*(-0.5))(0,0);
  }
};

int
main(){
  SVM svm;
  svm.d=0.02;
  svm.XX.init(7,7);
  svm.X.init(7,2);
  svm.Y.init(7,1);
  svm.L.init(7,1);
  svm.one.init(7,1);
  
  svm.X <<
    0,0,
    0,0.5,
    0,1,
    0.5,0,
    1,0,
    1,0.5,
    1,1;
  svm.Y <<
    -1,
    -1,
    -1,
    1,
    1,
    1,
    1;

  svm.init();

  svm.L.zero();
  svm.L.write(stdout,"%f");
  svm.dL.write(stdout,"%f");
  printf("cost:%f\n",svm.cost());
  for(int i=0;i<10000000;i++){
    svm.update();
    printf("cost:%f\r",svm.cost());
  }
  // svm.L.write(stdout,"%f");
  // svm.dL.write(stdout,"%f");
  
  return 0;
}

