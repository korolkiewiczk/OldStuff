//part.cpp
#include "gl/gl.h"

#include "part.h"
#include "game.h"

void CPart::Init(float px,float py,float pvx,float pvy,int x1,int y1,int x2,int y2,
float pwr,CTex *tex)
{
 x=px;
 y=py;
 vx=pvx;
 vy=pvy;
 pic.Init(x1,y1,x2,y2,tex);
 power=pwr;
 colorvec[0]=colorvec[1]=colorvec[2]=colorvec[3]=1.f;
}

void CPart::SetColor(float r,float g,float b,float a)
{
 colorvec[0]=r;
 colorvec[1]=g;
 colorvec[2]=b;
 colorvec[3]=a;
}

void CPart::Draw(float s)
{
 glColor4f(colorvec[0]*power,colorvec[1]*power,colorvec[2]*power,colorvec[3]);
 pic.Draw(x,y,s);
}

void CPart::Update()
{
 x+=vx*CGame::GetFrameTime();
 y+=vy*CGame::GetFrameTime();
}
