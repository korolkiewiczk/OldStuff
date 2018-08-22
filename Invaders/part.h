//part.h
#ifndef PART_H
#define PART_H

#include "rect.h"
#include "pic.h"
#include "tex.h"

class CPart
{
private:
 float x,y,vx,vy;
 CPic pic;
 float power;
 float colorvec[4];
public:
 void Init(float,float,float,float,int,int,int,int,float,CTex *);
 void SetColor(float,float,float,float);
 void Draw(float);
 void Update();
 void GetPos(float &px,float &py) {px=x; py=y;}
 float GetPower() {return power;}
};

#endif
