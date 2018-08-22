#include <windows.h>
#include "winopengl.h"

#include "tex.h"
#include "game.h"
#include "spaceship.h"

//#define SCRWIDTH 800
//#define SCRHEIGHT 600
int SCRWIDTH,SCRHEIGHT;

int main()
{
 SCRWIDTH=GetSystemMetrics(SM_CXSCREEN);
 SCRHEIGHT=GetSystemMetrics(SM_CYSCREEN);
 //SCRWIDTH=240;
 //SCRHEIGHT=320;
 OpenGL_Init(SCRWIDTH,SCRHEIGHT,32,0);
 ShowCursor(0);
 try
 {
 CGame::Init(SCRWIDTH,SCRHEIGHT);
 while (1)
 {
  if (!OpenGL_Present()) break;
  if (!CGame::MainLoop()) break;
 }
 CGame::Close();
 }//try
 catch (char *s) {MessageBox(0,s,"Error!",0); OpenGL_Close(); return 0;}
 catch (...) {MessageBox(0,"Undefined error","Error!",0); OpenGL_Close(); return 0;}
 OpenGL_Close();
 return 1;
}
