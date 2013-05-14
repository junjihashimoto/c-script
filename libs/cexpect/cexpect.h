#ifndef CEXPECT
#define CEXPECT

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pty.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct expect;

typedef void (*cexpect_read_callback)(expect* exp,const char* str);

struct expect{
  int   fd;
  pid_t pid;
  int   init(const char* path,char *const argv[]);
  void  run();
  void  close();
  cexpect_read_callback read_callback;
  int   write_str(const char* str);
};

#endif
