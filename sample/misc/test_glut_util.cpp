#!/usr/bin/env c-script

#pragma c-script:use glut_util


int prev=0;

void
display(){
  GLUT2D_SETUP glut2d;


  glTranslatef(256+30,256+30,0);
  glScalef(1,-1,1);

  
  white();
  line(-256,0,
       256,0);
  line(0,-256,
       0,256);
  blue();
  square(-256,-256,
	 256,256,0);

  yellow();
  line(prev,0,
       prev,-30);
  green();
  for(int i=0;i<255;i++){
    int v=(i/16)*16;
    line(i,0,
	 i,v);
  }

  white();
  Printf(-256,-256,"prev:%d",prev);
  
  black();
  line(0,0,
       256,256);
  Printf(0,256,"y");
  Printf(256,0,"x");
  Printf(-5,-5,"0");

}

static void
keydown(unsigned char key, int x, int y){
  if(key=='q')
    glutLeaveMainLoop();
  else if(key=='u')
    prev++;
  else if(key=='d')
    prev--;
  else if(key=='c'){//capture image
    BMPb cap(800,600);
    BMPb out=cap;
    glReadPixels(0,0,cap.w,cap.h,GL_RGB,GL_UNSIGNED_BYTE,
		 (unsigned char*)cap.rgb);
    bmp_for(out)
      out(x,y)=cap(x,599-y);
    
    out.write("capture.png");
  }
    
}

int
main(int argc, char *argv[]){
  glut_util gl;
  gl.display=display;
  gl.keydown=keydown;
  gl.start(800,600);
  return 0;
}
