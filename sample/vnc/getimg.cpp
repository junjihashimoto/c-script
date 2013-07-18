#!/usr/bin/env c-script

#pragma c-script:use vnc

#include <unistd.h>
int
main(int argc,char** argv){
  VNC_Client vnc;
  if(argc<5){
    printf ("Usage: ./getimg hostname port(5900+vncport) password img-name\n");
    return 1;
  }
  
  vnc.init(argv[1],atoi(argv[2]),argv[3]);
  vnc.wait_get_display();
  {
    Lock lock(vnc.img_mutex);
    vnc.img.write(argv[4]);
  }
  vnc.close();
  return 0;
}
