#include <sys/time.h>
#include <sys/resource.h>

struct Benchmark{
  double pre_time;
  double delta;
  int    num;
  double get_time();
  void reset();
  void begin();
  void end();
  void print();
};

extern Benchmark benchmark;
