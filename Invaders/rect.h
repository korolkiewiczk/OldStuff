//rect.h

#ifndef RECT_H
#define RECT_H

struct TRect
{
 int x1,y1,x2,y2;
 TRect(int _x1=0,int _y1=0,int _x2=0,int _y2=0)
 {
  x1=_x1;
  y1=_y1;
  x2=_x2;
  y2=_y2;
 }
 int Width() {return x2-x1;}
 int Height() {return y2-y1;}
};

#endif
