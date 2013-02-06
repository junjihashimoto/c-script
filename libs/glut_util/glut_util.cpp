#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <cassert>

#include "glut_util.h"

static void
idle(){
  glutPostRedisplay();
}


static void
Init(){
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static void
keydown_(unsigned char key, int x, int y){
  if(key=='q')
    glutLeaveMainLoop();
}

static void
mouse_(int button , int state , int x , int y) {
  int shift=glutGetModifiers() & GLUT_ACTIVE_SHIFT;
  int ctrl =glutGetModifiers() & GLUT_ACTIVE_CTRL;
  int alt  =glutGetModifiers() & GLUT_ACTIVE_ALT;
}


glut_util::glut_util(){
  draw=NULL;
  keydown=NULL;
  mouse=NULL;
  motion=NULL;
  passive_motion=NULL;
  display=NULL;
}

void
glut_util::start(int width,int height){
  char *temp[2]={"show_img",NULL};
  int num=1;
  glutInit(&num,temp);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA );
  glutInitWindowSize(width,height);
  glutCreateWindow(temp[0]);
  if(this->keydown)
    glutKeyboardFunc(this->keydown);
  else
    glutKeyboardFunc(keydown_);
  if(this->mouse)
    glutMouseFunc(this->mouse);
  else
    glutMouseFunc(mouse_);
  
  if(motion)
    glutMotionFunc(motion);
  if(passive_motion)
    glutPassiveMotionFunc(passive_motion);

  assert(display!=NULL);
  glutDisplayFunc(display);
  Init();
  
  glutIdleFunc(idle);
  //  glutReshapeFunc(reshape);

  glutMainLoop();
}
  

GLUT2D_SETUP::GLUT2D_SETUP(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, WIDTH, HEIGHT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.0, 1.0);
}

GLUT2D_SETUP::~GLUT2D_SETUP(){
  glutSwapBuffers();
}

void
line(int x0,int y0,int x1,int y1){
  glBegin(GL_LINES);
  glVertex2i(x0, y0);
  glVertex2i(x1, y1);
  glEnd();
}

void
square(int x0,int y0,int x1,int y1,int fill){
  if(fill){
    glBegin(GL_QUADS);
    glVertex2i(x0,y0);
    glVertex2i(x1,y0);
    glVertex2i(x1,y1);
    glVertex2i(x0,y1);
    glEnd();
  }else{
    glBegin(GL_LINE_LOOP);
    glVertex2i(x0,y0);
    glVertex2i(x1,y0);
    glVertex2i(x1,y1);
    glVertex2i(x0,y1);
    glEnd();
  }
}



void
Printf(int x,int y,const char* format, ...){
  static char strf_buf[1024];
  va_list ap;
  va_start(ap, format);
  vsnprintf(strf_buf,sizeof(strf_buf), format, ap);
  va_end(ap);

  glPushMatrix();
  glRasterPos2f( x , y );
  glutBitmapString( GLUT_BITMAP_HELVETICA_18 , (const unsigned char*)strf_buf );
  glPopMatrix();
}

void
white(){
  glColor3ub(255,255,255);
}
void
blue(){
  glColor3ub(0,0,255);
}
void
red(){
  glColor3ub(255,0,0);
}
void
green(){
  glColor3ub(0,255,0);
}
void
black(){
  glColor3ub(0,0,0);
}
void
yellow(){
  glColor3ub(255,255,0);
}
