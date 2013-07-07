#!/usr/bin/env c-script

double a(double x){
  return sin(x*2);
}

double m(double x){
  //  return [[echo 'load(cform)$ cform(integrate(x,x))$' | maxima --very-quiet]]

  [[
    for((i=0;i<10;i++));do
     printf "x+=%d;\n" $i
    done
    ]]
  return [[echo 'load(cform)$ cform(integrate(x,x))$' | maxima --very-quiet]]
}


int
main(){
  //  plot2d((F1)sin,a,0.0,M_PI*2.0);
  plot2d((F1)sin,a,m,0.0,M_PI*2.0);
  return 0;
}
