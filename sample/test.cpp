#!/usr/bin/env c-script
printf("hello world\n");
Matrix<double> mat(8,8);
Matrix<double> mat2(8,8);
mat_for(mat)
  mat(r,c)=r*mat.nc+c;
write_csv("a.csv",mat);
read_csv("a.csv",mat2);
assert(mat==mat2);
