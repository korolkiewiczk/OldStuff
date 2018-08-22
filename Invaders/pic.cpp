#include <gl/gl.h>
#include "game.h"

void CPic::Init(int x1,int y1,int x2,int y2,CTex *t)
{
 texrect.x1=x1;
 texrect.y1=y1;
 texrect.x2=x2;
 texrect.y2=y2;
 tex=t;
}

void CPic::Draw(float x,float y,float s)
{
 float w=texrect.Width()*s*0.5f;
 float h=texrect.Height()*s*0.5f;
 float vtab[6*2]=
 {
  x-w,
  y-h,
  
  x+w,
  y-h,
  
  x+w,
  y+h,
  
  x-w,
  y-h,
  
  x+w,
  y+h,
  
  x-w,
  y+h
 };    

 float ttab[6*2]=
 {
  tex->Coord(texrect.x1),
  tex->Coord(texrect.y1),
  
  tex->Coord(texrect.x2),
  tex->Coord(texrect.y1),
  
  tex->Coord(texrect.x2),
  tex->Coord(texrect.y2),
  
  tex->Coord(texrect.x1),
  tex->Coord(texrect.y1),
  
  tex->Coord(texrect.x2),
  tex->Coord(texrect.y2),
  
  tex->Coord(texrect.x1),
  tex->Coord(texrect.y2)
 };
 
 tex->Bind();
 
 glEnableClientState(GL_VERTEX_ARRAY);
 glVertexPointer(2,GL_FLOAT,0,&vtab);
 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 glTexCoordPointer(2,GL_FLOAT,0,ttab);
 glEnable(GL_TEXTURE_2D);
 
 glDrawArrays(GL_TRIANGLES,0,6);
 
 glDisable(GL_TEXTURE_2D);
 glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 glDisableClientState(GL_VERTEX_ARRAY);
}
