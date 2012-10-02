#!/usr/bin/env c-script
int
main(){
  chdir("/tmp");
  systemf("ls");
  
  return 0;
}
