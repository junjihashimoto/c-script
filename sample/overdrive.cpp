#!/usr/bin/env c-script

struct OD{

  //m*x''=k(target-x)-um // x'> 0
  //m*x''=k(target-x)+um // x'< 0

  double m;
  double us;
  double um;
  double k;
  double x_1;
  double x;
  double target;
  double delta_t;
  double pre_t;

  OD(double src=0,double target=100){
    m=60;
    us=0.5;
    um=5;
    k=5;
    x_1=0;
    x=src;
    this->target=target;
    delta_t=0.001;
    pre_t=0;
  }

  void update(){
    double tx_1=x_1;
    double diff=target-x;
    double u;

    if(x_1>0)
      u=-um;
    else
      u= um;
    
    if(diff>=0)
      diff=pow(diff/256,1.5)*256;
    else
      diff=-pow(-diff/256,1.5)*256;
    
    if(x_1>=0)
      x_1+=(k*diff-um*x_1*x_1+u)/m*delta_t;
    else
      x_1+=(k*diff+um*x_1*x_1+u)/m*delta_t;
    x +=tx_1*delta_t;

  }
  double operator ()(double t){
    delta_t=t-pre_t;
    pre_t=t;
    update();
    return x;
  }
};

struct OD2{
  OD od;
  double target2;
  double t;
  OD2(OD od,double target2,double t){
    this->od=od;
    this->target2=target2;
    this->t=t;
  }
  double operator ()(double t){
    if(t>this->t)
      od.target=target2;
    return od(t);
  }
};

int
main(){
  OD  od0(0,10);
  OD  od1(0,128);
  OD  od2(0,255);
  OD  od3(200,100);
  
  OD  od41(0,255);
  OD2 od4 (od41,100,15);
  
  plot2d(od0,
	 od1,
	 od2,
	 od3,
	 od4,
	 0.0,100.0);
  return 0;
}
