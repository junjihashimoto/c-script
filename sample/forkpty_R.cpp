#!/usr/bin/env c-script

#pragma c-script:libs -lutil

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pty.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct expect{
  int fd;
  pid_t pid;
  int init(const char* path,char *const argv[]);
  void run();
  void close();
  void (*read_callback)(expect* exp,const char* str);
  int  write_str(const char* str);
};
extern void callback(expect* exp,const char* str);

int
expect::init(const char* path,char *const argv[]){
  struct winsize ws;
  ws.ws_col    = 80;
  ws.ws_row    = 60;
  ws.ws_xpixel = 0;
  ws.ws_ypixel = 0;

  switch ((pid = forkpty(&fd, NULL, NULL, &ws))) {
  case -1:
    return -1; /* error */
  case 0:
    setenv("TERM", "vt100", 1);
    execv(path,argv);
  default:
    break;
  }
}

void
expect::close(){
  ::close(fd);
  kill(pid, SIGKILL);
}

void
expect::run(){
  fd_set set;
  int len;
  char buff[1025];

  while (1) {
    FD_ZERO(&set);
    FD_SET(fd, &set);
    
    if (select(FD_SETSIZE, &set, NULL, NULL, NULL) < 1) {
      break;
    }
    if (FD_ISSET(fd, &set)) {
      if ((len = read(fd, buff, sizeof(buff))) == -1) {
	break;
      }
      buff[len]=0;
      read_callback(this,buff);
    }
  }
}

int
expect::write_str(const char* str){
  return write(fd,str,strlen(str));
}
  

int st=0;

void
callback(expect* exp,const char* str){
  if(st==0&&strstr(str,">")){
    exp->write_str("x <- 1:10\n"
		   "y <- 1:10\n"
		   "plot(x,y)\n"
		   "plot(x,y,xlim=c(10,1))\n");
    st=1;
  }else if(st==1&&strstr(str,">")){
    sleep(3);
    exp->write_str("q()\n");
    st=2;
  }else if(st==2 && strstr(str,"Save workspace image? ")){
    exp->write_str("n\n");
  }
}

int
main(int argc,char* const argv[]){
  expect ex;
  ex.read_callback=callback;
  ex.init("/usr/bin/R",argv);
  ex.run();
  ex.close();
  
  return 0;
}
