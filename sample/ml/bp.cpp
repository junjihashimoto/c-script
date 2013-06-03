#!/usr/bin/env c-script

struct BP{
  Matrix<double> X;
  Matrix<double> D;
  Matrix<double> S;
  Matrix<double> W;
  Matrix<double> dW;
  Matrix<double> Y;
  Matrix<double> Hd;
  double d;
  void forward(){
    S=W*Y;
    mat_for(Hd)
      X(r,c)=sigm(S(r,c));
  }
  void update(){
    mat_for(Hd)
      Hd(r,c)=sigm(S(r,c))*(1-sigm(S(r,c)));
    dW=-2*ary_mul(ary_mul((D-X),Hd),Y);
  }
  void update(Matrix<double>& hW,Matrix<double>& hY){
    mat_for(Hd)
      Hd(r,c)=sigm(S(r,c))*(1-sigm(S(r,c)));
    dW=-2*ary_mul(ary_mul(t(hW*hY),Hd),Y);
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
