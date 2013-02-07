#include <bmp.h>

#include <ft2build.h>
#include FT_FREETYPE_H


struct Freetype{
  int err;
  FT_Library* library;
  FT_Face*    face;
  int w;
  int h;
  Freetype();
  void init();
  void close();
  void set_font(const char* font_file);
  void set_size(int w,int h);
  BMP get(int charcode);
  BMP getstring(const char* str);
};

