//pic.h
#ifndef PIC_H
#define PIC_H

#include "rect.h"
#include "tex.h"

class CPic
{
private:
 TRect texrect;
 CTex *tex;
public:
 void Init(int,int,int,int,CTex*);
 void Draw(float,float,float);
 int Width() {return texrect.Width();}
 int Height() {return texrect.Height();}
 TRect &Rect() {return texrect;}
};

#endif
