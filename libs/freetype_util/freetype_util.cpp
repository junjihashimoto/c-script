#include "freetype_util.h"

struct UTF8String{
  unsigned char* str;
  unsigned char  *pos;
  UTF8String(const char* str):str((unsigned char*)str){
    pos=(unsigned char*)this->str;
  }
  /*
    00-7F						7bit
    0						
    7F						
    C2-DF	80-BF					11bit
    C2	80					
    DF	BF					
    E0-EF	80-BF	80-BF				16bit
    E0	A0	80				
    EF	BF	BF				
    F0-F7	80-BF	80-BF	80-BF			21bit
    F0	90	80	80			
    F7	BF	BF	BF			
    F8-FB	80-BF	80-BF	80-BF	80-BF		26bit
    F8	88	80	80	80		
    FB	BF	BF	BF	BF		
    FC-FD	80-BF	80-BF	80-BF	80-BF	80-BF	31bit
    FC	84	80	80	80	80	
    FD	BF	BF	BF	BF	BF	
  */
  
  int length(){
    int len=0;
    int s=0;
    for(unsigned char* pos=str;*pos!=0;pos++){
      if(*pos<=0x7f){
	len++;
      }else if(0xc2<=*pos&&*pos<=0xdf){
	len++;
      }else if(0xe0<=*pos&&*pos<=0xef){
	len++;
      }else if(0xf0<=*pos&&*pos<=0xf7){
	len++;
      }else if(0xf8<=*pos&&*pos<=0xfb){
	len++;
      }else if(0xfc<=*pos&&*pos<=0xfd){
	len++;
      }
    }
    return len>strlen((const char*)str) ? -1 : len;
  }
  int get(){
    int val=0;
    if(*pos==0){
      return 0;
    }else if(*pos<=0x7f){
      val=*pos;
      pos++;
      return val;
    }else if(0xc2<=*pos&&*pos<=0xdf){
      val=0x1f&(*pos);
      pos++;
      if(0x80<=*pos&&*pos<=0xbf){
	val=(val<<6)|(0x3f&*pos);
	pos++;
	return val;
      }else{
	return 0;
      }
      
    }else if(0xe0<=*pos&&*pos<=0xef){
      val=0xf&(*pos);
      pos++;
      for(int i=0;i<2;i++){
	if(0x80<=*pos&&*pos<=0xbf){
	  val=(val<<6)|(0x3f&*pos);
	  pos++;
	}else{
	  return 0;
	}
      }
      return val;
    }else if(0xf0<=*pos&&*pos<=0xf7){
      val=0x7&(*pos);
      pos++;
      for(int i=0;i<3;i++){
	if(0x80<=*pos&&*pos<=0xbf){
	  val=(val<<6)|(0x3f&*pos);
	  pos++;
	}else{
	  return 0;
	}
      }
      return val;
    }else if(0xf8<=*pos&&*pos<=0xfb){
      val=0x3&(*pos);
      pos++;
      for(int i=0;i<4;i++){
	if(0x80<=*pos&&*pos<=0xbf){
	  val=(val<<6)|(0x3f&*pos);
	  pos++;
	}else{
	  return 0;
	}
      }
      return val;
    }else if(0xfc<=*pos&&*pos<=0xfd){
      val=0x1&(*pos);
      pos++;
      for(int i=0;i<5;i++){
	if(0x80<=*pos&&*pos<=0xbf){
	  val=(val<<6)|(0x3f&*pos);
	  pos++;
	}else{
	  return 0;
	}
      }
      return val;
    }
  }
  
};



Freetype::Freetype(){
  library=NULL;
  face   =NULL;
}
void
Freetype::init(){
  if(library!=NULL){
    FT_Done_FreeType(*library);
    free(library);
    library=NULL;
  }
    
  library=(FT_Library*)malloc(sizeof(FT_Library));
  assert(library!=NULL);
  err = FT_Init_FreeType(library);
  assert(!err);
}
void
Freetype::close(){
  if(face!=NULL){
    FT_Done_Face(*face);
    free(face);
    face=NULL;
  }
  if(library!=NULL){
    FT_Done_FreeType(*library);
    free(library);
    library=NULL;
  }
}
void
Freetype::set_font(const char* font_file){
  if(face!=NULL){
    FT_Done_Face(*face);
    free(face);
    face=NULL;
  }
    
  face=(FT_Face*)malloc(sizeof(FT_Face));
  assert(face!=NULL);
  err = FT_New_Face(*library,font_file, 0, face);
  assert(!err);
}
void
Freetype::set_size(int w,int h){
  this->w=w;
  this->h=h;
  err = FT_Set_Pixel_Sizes(*face, w, h);
  assert(!err);
}
BMPb&
Freetype::get(int charcode){
  err = FT_Load_Char(*face, charcode, 0);
  assert(!err);
  
  err = FT_Render_Glyph((*face)->glyph, FT_RENDER_MODE_MONO);
  assert(!err);

  FT_Bitmap *bm = &((*face)->glyph->bitmap);
  int row, col, bit, c;
    
  img.init(bm->pitch*8,bm->rows);
  bmp_for(img){
    int c=((bm->buffer[bm->pitch * y + (x/8)])>>(7-(x%8)))&1 ? 255: 0;
    img(x,y,0)=c;
    img(x,y,1)=c;
    img(x,y,2)=c;
  }
  return img;
}

void
Freetype::_get(int charcode,int offset_x,int offset_y,int baseline){
  err = FT_Load_Char(*face, charcode, 0);
  assert(!err);
  
  err = FT_Render_Glyph((*face)->glyph, FT_RENDER_MODE_MONO);
  assert(!err);

  FT_Bitmap *bm = &((*face)->glyph->bitmap);
  int row, col, bit, c;

  offset_x+=(*face)->glyph->bitmap_left;
  offset_y+=baseline-(*face)->glyph->bitmap_top;


  for(int y=0;y<bm->rows;y++)
    for(int x=0;x<bm->pitch*8;x++){
      int c=((bm->buffer[bm->pitch * y + (x/8)])>>(7-(x%8)))&1 ? 255: 0;
      img(offset_x+x,offset_y+y,0)=c;
      img(offset_x+x,offset_y+y,1)=c;
      img(offset_x+x,offset_y+y,2)=c;
    }
}

BMPb&
Freetype::getstring(const char* str){
  UTF8String s(str);
  int length=s.length();
  int posx=0;
  int posy=0;
  int baseline=
    ((*face)->height + (*face)->descender) *
    (*face)->size->metrics.y_ppem / (*face)->units_per_EM;
  int height=((*face)->max_advance_height) *
    (*face)->size->metrics.y_ppem / (*face)->units_per_EM;
  int width=((*face)->max_advance_width) *
    (*face)->size->metrics.x_ppem / (*face)->units_per_EM;
  img.init(width*strlen(str),height);
  for(int i=0;i<length;i++){
    // int offsetx=posx+(*face)->glyph->bitmap_left;
    // int offsety=posy+baseline-(*face)->glyph->bitmap_top;
    _get(s.get(),posx,posy,baseline);
    //posx+=((*face)->glyph->metrics.horiAdvance)>>6;
    posx+=((*face)->glyph->advance.x>>6);
  }
  return img;
    
}
