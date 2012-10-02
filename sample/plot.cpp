#!/usr/bin/env c-script

double a(double x){
  return sin(x*2);
}


int
main(){
  plot2d((F1)sin,a,0.0,M_PI*2.0);
  return 0;
}
