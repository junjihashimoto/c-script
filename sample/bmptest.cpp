#!/usr/bin/env c-script


int
main(){
  BMP bmp("testimg.bmp");
  bmp_for3(bmp){
    if(z==0)
      bmp(x,y,z)=255-bmp(x,y,z);
    else
      bmp(x,y,z)=0;
  }
  bmp.write("a.bmp");
  return 0;
};
