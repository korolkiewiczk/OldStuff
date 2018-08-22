//spaceship.cpp

#include <gl/gl.h>
#include <math.h>

#include "spaceship.h"
#include "game.h"

void CShot::Update(float s)
{
 y+=vel*CGame::GetFrameTime();
 pic->Draw(x,y,s);
}

void CSpaceShip::Init(int bx1,int by1,int bx2,int by2,int x,int y,int pwr,
float acc,float s,float svel,float stime)
{
 boundx1=bx1;
 boundy1=by1;
 boundx2=bx2;
 boundy2=by2;
 this->x=x;
 this->y=y;
 this->acc=acc;
 vx=0;
 vy=0;
 timer=0;
 power=pwr;
 scale=s;
 shotvel=svel;
 shottime=stime;
 shottimer=CGame::GetTime()+CGame::frand();
}

void CSpaceShip::SetTex(int x1,int y1,int x2,int y2,CTex *tex)
{
 pic.Init(x1,y1,x2,y2,tex);
}

void CSpaceShip::SetShotTex(int x1,int y1,int x2,int y2,CTex *tex)
{
 shotpic.Init(x1,y1,x2,y2,tex);
}

void CSpaceShip::Command(int cmd)
{
 if (cmd&Up)
 {
  vy-=acc*CGame::GetFrameTime();
 }

 if (cmd&Down)
 {
  vy+=acc*CGame::GetFrameTime();
 }

 if (cmd&Left)
 {
  vx-=acc*CGame::GetFrameTime();
 }

 if (cmd&Right)
 {
  vx+=acc*CGame::GetFrameTime();
 }
 
 if (!(cmd&Up)&&!(cmd&Down) || ((!(cmd&Down))&&(cmd&Up)&&(vy>0))
  || ((!(cmd&Up))&&(cmd&Down)&&(vy<0)))
 vy*=VELATTEN;
 
 if (!(cmd&Left)&&!(cmd&Right) || ((!(cmd&Right))&&(cmd&Left)&&(vx>0))
  || ((!(cmd&Left))&&(cmd&Right)&&(vx<0)))
 vx*=VELATTEN;
 
 if (cmd&Shot)
 {
  if (CGame::GetTime()-shottimer>shottime)
  {
   CShot *s=new CShot(x,y,shotvel,&shotpic);
   shot.push_back(s);
   shottimer=CGame::GetTime();
  }
 }
}

void CSpaceShip::Draw()
{
 if (power<=0) return;
 glColor4f(255,255,255,255);
 pic.Draw(x,y,scale);
 
 list <CShot*>::iterator it;
 for (it=shot.begin(); it!=shot.end(); ++it)
 {
  (*it)->Update(fabs(scale)*0.25+0.75);
  float y=(*it)->GetY();
  if ((y<0)||(y>=CGame::GetScrHeight()))
  {
   delete *it;
   list <CShot*>::iterator pomit=it;
   ++pomit;
   shot.erase(it);
   it=pomit;
  }
 }
}

void CSpaceShip::Update()
{
 float w=pic.Width()/2*fabs(scale);
 float h=pic.Height()/2*fabs(scale);
 if ((vx<0)&&(x-w>boundx1))
 {
  x+=vx*CGame::GetFrameTime();
 }
 if ((vy<0)&&(y-h>boundy1))
 {
  y+=vy*CGame::GetFrameTime();
 }
 if ((vx>0)&&(x+w<boundx2))
 {
  x+=vx*CGame::GetFrameTime();
 }
 if ((vy>0)&&(y+h<boundy2))
 {
  y+=vy*CGame::GetFrameTime();
 }
}

void CSpaceShip::CheckColWith(CSpaceShip &s)
{
 list <CShot*>::iterator it;
 for (it=shot.begin(); it!=shot.end(); ++it)
 {
  float x=(*it)->GetX();
  float y=(*it)->GetY();
  
  TRect r=s.ShipRect();
  
  if ((r.x1<=x)&&(r.x2>=x)&&(r.y1<=y)&&(r.y2>=y))
  {
   s.DecPower();

   delete *it;
   list <CShot*>::iterator pomit=it;
   ++pomit;
   shot.erase(it);
   it=pomit;
  }
 }
}

TRect CSpaceShip::ShipRect()
{
 int w=(int)(pic.Rect().Width()*fabs(scale));
 int h=(int)(pic.Rect().Height()*fabs(scale));
 return TRect((int)x-w/2,(int)y-h/2,(int)x+w/2,(int)y+h/2);
}
