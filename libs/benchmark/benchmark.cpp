#include <cstdio>
#include "benchmark.h"

Benchmark benchmark;

double Benchmark::get_time(){
  struct timeval t;
  struct timezone tzp;
  gettimeofday(&t, &tzp);
  return t.tv_sec + t.tv_usec*1e-6;
}
void Benchmark::reset(){
  delta=0;
  num=0;
  pre_time=get_time();
}
void Benchmark::begin(){
  pre_time=get_time();
}
void Benchmark::end(){
  delta+=get_time()-pre_time;
  num++;
}
void Benchmark::print(){
  printf("%e\n",delta/(double)num);
}
