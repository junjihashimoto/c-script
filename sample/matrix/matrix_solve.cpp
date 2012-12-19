

template<class T>
int
test(){

  Matrix<T> d(2,2);
  Matrix<T> d2(2,2);

  d<<
    1,2,
    3,4;

  d2<<
    3,4,
    6,7;


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
  //  test<float>();
  //  test<int>();
  return 0;
}
