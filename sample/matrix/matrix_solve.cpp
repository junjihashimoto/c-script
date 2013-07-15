#!/usr/bin/env c-script

#pragma c-script:eval_begin
$default_header="#define DEBUG_MATRIX\n".$default_header;
#pragma c-script:eval_end

template<class T>
int
test(){

  Matrix<T> d(2,2);
  Matrix<T> d2(2,2);
  Matrix<T> d3;

  d<<
    1,2,
    3,4;

  d2<<
    3,4,
    6,7;

  printf("=\n");
  d3=d;
  printf("+\n");
  d3=d+d2;
  printf("=\n");
  Matrix<T> d5=d;
  printf("+=\n");
  Matrix<T> d4=d+d2;

  printf("x\n");
  Matrix<T> d6=x(d,d2);
  printf("++\n");
  Matrix<T> d7=d+d2+d3;

  return 0;


  inv(d).write();
  (inv(d)*d).write();


  Matrix<T> f(2,1);
  f<<
    1,
    1;

  f.write();
  solve_gels(d,f).write();
  solve_gels(d2,f).write();

  return 0;
}

int
main(){
  test<double>();
  test<float>();
  test<int>();
  //  test<float>();
  //  test<int>();
  return 0;
}
