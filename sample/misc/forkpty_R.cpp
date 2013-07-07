#!/usr/bin/env c-script

#pragma c-script:use cexpect

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
    printf("Press-enter for exit:\n");
    getchar();
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
