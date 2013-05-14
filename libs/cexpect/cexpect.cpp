#include "cexpect.h"

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
  
