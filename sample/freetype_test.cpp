#!/usr/bin/env c-script
#pragma c-script:use freetype_util

int
main(int argc,char** argv){
  Freetype font;
  font.init();
  font.set_font( "/usr/share/fonts/truetype/fonts-japanese-gothic.ttf");
  int size=atoi(argv[1]);
  font.set_size(size,size);
  font.get('A').write("a.png");
  font.getstring(argv[2]).write("abc.png");
  font.close();
  return 0;
}
