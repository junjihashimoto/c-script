#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>


#define  WIDTH  glutGet(GLUT_WINDOW_WIDTH)
#define  HEIGHT glutGet(GLUT_WINDOW_HEIGHT)

struct glut_util{
  glut_util();
  void start(int width=640,int height=480);
  void (*draw   )();
  void (*keydown)(unsigned char key, int x, int y);
  void (*mouse)(int button , int state , int x , int y) ;
  void (*motion)(int x , int y);
  void (*passive_motion)(int x , int y);
  void (*display)();
};

struct GLUT2D_SETUP{
  GLUT2D_SETUP();
  ~GLUT2D_SETUP();
};


void line(int x0,int y0,int x1,int y1);
void square(int x0,int y0,int x1,int y1,int fill=0);
void Printf(int x,int y,const char* format, ...);

void white();
void blue();
void red();
void green();
void black();
void yellow();
