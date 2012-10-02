#!/usr/bin/env c-script

int
main(int argc,char** argv){
  BMP bmp(argv[1]);

  int hist[16];
  for(int i=0;i<16;i++)
    hist[i]=0;

  bmp_for2(bmp){
    int v=(bmp(x,y)[0]+2*bmp(x,y)[1]+bmp(x,y)[2])/4;
    hist[v/16]++;
  }
  histgram(hist,16);
  return 0;
}
