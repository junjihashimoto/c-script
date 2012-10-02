#!/usr/bin/env c-script

double a(double x,double y){
  return sin(x)*cos(y);
}


int
main(){
  splot(a,0.0,M_PI*2.0,0.0,M_PI*2.0);
  return 0;
}
