#!/usr/bin/env c-script

#pragma c-script:use vnc

#include "vnclib.h"
#include <unistd.h>
int
main(int argc,char** argv){
  VNC_Client vnc;
  if(argc<4){
    printf ("Usage: ./mousemove hostname port(5900+vncport) password x y\n");
    return 1;
  }
  
  vnc.init(argv[1],atoi(argv[2]),argv[3]);
  int x=atoi(argv[4]);
  int y=atoi(argv[5]);
  vnc.set_point(x,y,0);
  vnc.close();
  return 0;
}
