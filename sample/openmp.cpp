#!/usr/bin/env c-script

#pragma c-script:opt -fopenmp

#include <stdio.h>
#include <omp.h>

int
main(){
  #pragma omp parallel
  printf("Hello, OpenMP!\n");
  return 0;
}

