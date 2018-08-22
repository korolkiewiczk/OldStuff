//game.cpp

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <gl/gl.h>
#include <math.h>

#include "winopengl.h"
#include "spaceship.h"
#include "game.h"

CTex CGame::GlobalTex;

CSpaceShip CGame::playership;
CSpaceShip CGame::invadship[MAXINVAD];
float CGame::frametime;
float CGame::prevt=0;
CPart CGame::stars[MAXSTARS];
int CGame::width,CGame::height;
float CGame::scale;
int CGame::points=0;
int CGame::maxpoints=0;

int CGame::mode=0;

float CGame::bonusx;
float CGame::bonusy=100000000.f;
CPic CGame::bonuspic;

list <CPart*> CGame::boomparts;

void CGame::genstar(int no,int n)
{
 float pwr=frand()*0.8+0.2;
 float vx=0;
 float vy=(0.9+frand()*0.1)*pwr*STARVEL;
 stars[no].Init(rand()%width,!n ? rand()%width:0,vx,vy,18,8,22,12,pwr,&GlobalTex);
}

void CGame::updatestars(int a,int b)
{
 for (int i=a; i<b; i++)
 {
  stars[i].Draw(stars[i].GetPower()*(scale*0.3+0.7));
  stars[i].Update();
  float x,y;
  stars[i].GetPos(x,y);
  if (y>height) genstar(i,1);
 }
}

void CGame::Init(int w,int h)
{
 srand(time(0));
 if (!GlobalTex.Load("itex.bmp")) throw "itex.bmp not found";
 
 if (w>h)
 scale=(float)w/1280.f;
 else
 scale=(float)h/1280.f;
 
 width=w;
 height=h;
 
 InitPlayer();
 
 for (int i=0; i<MAXINVAD; i++)
 {
  InitInvad(i);
 }
 
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-(float)w/2,(float)w/2,(float)h/2,-(float)h/2,1,-1);
 glTranslatef(-(float)w/2,-(float)h/2,0);
 glMatrixMode(GL_MODELVIEW);
 //glScalef(0.5,0.5,0.5);
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

 
 for (int i=0; i<MAXSTARS; i++) genstar(i);
}

int CGame::MainLoop()
{
 
 glClear(GL_COLOR_BUFFER_BIT);
 
 
 if (KeyPressed(1)) return 0;
 
 updatestars(0,MAXSTARS); 
 
 //rysuj gracza
 int cmd=0;
 if (KeyPressed(72)) cmd|=CSpaceShip::Up;
 if (KeyPressed(80)) cmd|=CSpaceShip::Down;
 if (KeyPressed(75)) cmd|=CSpaceShip::Left;
 if (KeyPressed(77)) cmd|=CSpaceShip::Right;
 if (mode==1)
 {
  if (KeyPressed(57)) {InitPlayer();}
 }
 if (KeyPressed(57)) cmd|=CSpaceShip::Shot;
 playership.Command(cmd);
 playership.Update();
 playership.Draw();
 
 //rysuj przeciwnikow
 for (int i=0; i<MAXINVAD; i++)
 {
  playership.CheckColWith(invadship[i]);
  if (!invadship[i].IsAlive())
  {
   float x,y;
   invadship[i].GetPos(x,y);
   NewBoom(x,y,0,STARVEL/2,STARVEL/3,GENPARTS);
   InitInvad(i);
   points+=100;
   continue;
  }
  invadship[i].CheckColWith(playership);
  int c=rand()%8;
  if (GetTime()-invadship[i].timer>INVADTIME)
  {
   switch(c)
   {
    case 0: cmd=CSpaceShip::Up; break;
    case 1: cmd=CSpaceShip::Up|CSpaceShip::Right; break;
    case 2: cmd=CSpaceShip::Right; break;
    case 3: cmd=CSpaceShip::Down|CSpaceShip::Right; break;
    case 4: cmd=CSpaceShip::Down; break;
    case 5: cmd=CSpaceShip::Down|CSpaceShip::Left; break;
    case 6: cmd=CSpaceShip::Left; break;
    case 7: cmd=CSpaceShip::Up|CSpaceShip::Left; break;
   }
   invadship[i].timer=GetTime();
   if ((!(rand()%4))&&(mode==0))
   cmd|=CSpaceShip::Shot;
   invadship[i].dir=cmd;
  }
  invadship[i].Command(invadship[i].dir);
  invadship[i].Update();
  invadship[i].Draw();
 }
 
 if (mode==0)
 {
  DrawPlayerBar();
  if (!playership.IsAlive())
  {
   float x,y;
   playership.GetPos(x,y);
   NewBoom(x,y,0,STARVEL/5,STARVEL/7,GENPARTS*4);
   mode=1;
   if (points>maxpoints)
   maxpoints=points;
   /*char bf[128],l[20];
   itoa(points,l,10);
   strcpy(bf,"You got ");
   strcat(bf,l);
   strcat(bf," points.\nRestart\n");
   if (MessageBox(OpenGL_hwnd,bf,"Game over",MB_YESNO)==IDYES) 
   {
    mode=0;
    InitPlayer();
   }*/
  }
 }
 
 UpdateBoomParts();
 CheckBonus();

 OutI(20,20,2,points);
 
 if (maxpoints)
 OutI(width-80,20,2,maxpoints);
 
 if (mode==0)
 {
  static float pt=0;
  if (GetTime()-pt>0.3)
  {
   points++;
   pt=GetTime();
  }
 }
 
 if (prevt!=0)
 frametime=GetTime()-prevt;
 prevt=GetTime();
 
 return 1;
}

void CGame::Close()
{
}

float CGame::GetTime()
{
 long long f,t;
 QueryPerformanceFrequency((PLARGE_INTEGER)&f);
 QueryPerformanceCounter((PLARGE_INTEGER)&t);
 return (float)t/(float)f;
}

void CGame::InitPlayer()
{
 playership.Init(width/BOUNDDIV,height/BOUNDDIV,width-width/BOUNDDIV,height-height/BOUNDDIV,
 width/2,height-70,PLAYERPOWER,
 PLAYERACC,scale,-PLAYERSHOTVEL,PLAYERSHOTTIME);
 playership.SetTex(72,74,125,120,&GlobalTex);
 playership.SetShotTex(5,8,7,19,&GlobalTex);
 points=0;
 frametime=0;
 prevt=0;
 mode=0;
}

void CGame::InitInvad(int i)
{
 invadship[i].Init(width/BOUNDDIV,height/BOUNDDIV,width-width/BOUNDDIV,height/3*2,
  rand()%width,-(rand()%INVADDEFY),1,INVADACC,-scale,INVADSHOTVEL,INVADSHOTTIME);
 if (rand()%2)
  invadship[i].SetTex(2,65,46,121,&GlobalTex);
 else
  invadship[i].SetTex(78,1,118,63,&GlobalTex);
 invadship[i].SetShotTex(8,8,10,19,&GlobalTex);
}

void CGame::NewBoom(float x,float y,float velx,float vely,float maxvel,int n)
{
 float d=2.f*3.1415f/(float)n,a=0;
 for (int i=0; i<n; i++)
 {
  CPart *p=new CPart;
  float pwr=frand()*0.5+0.5;
  float vx=cos(a)*frand()*maxvel+velx;
  float vy=sin(a)*frand()*maxvel+vely;
  p->Init(x,y,vx,vy,18,8,22,12,pwr,&GlobalTex);
  p->SetColor(frand()*0.4+0.6,frand()*0.5+0.5,frand()*0.4,frand()*0.3+0.7);
  boomparts.push_back(p);
  a+=d;
 }
}

void CGame::UpdateBoomParts()
{
 list<CPart*>::iterator it;
 for (it=boomparts.begin(); it!=boomparts.end(); ++it)
 {
  (*it)->Draw(((*it)->GetPower()*0.2+0.8)*(scale*0.3+0.7));
  (*it)->Update();
  float x,y;
  (*it)->GetPos(x,y);
  if ((x<0)||(y<0)||(x>=width)||(y>=height))
  {
   delete *it;
   list<CPart*>::iterator pomit=it;
   ++pomit;
   boomparts.erase(it);
   it=pomit;
  }
 }
}

void CGame::DrawPlayerBar()
{
 float vtab[2*2] = {0,height-1,(float)playership.GetPower()/PLAYERPOWER*width,height-1};
 //float vtab[2*2] = {0,height-45,width-1,height-56};
 glEnableClientState(GL_VERTEX_ARRAY);
 glVertexPointer(2,GL_FLOAT,0,&vtab);
 glColor4f(255,0,0,128);
 glLineWidth(2);
 
 glDrawArrays(GL_LINES,0,2);
 
 glDisableClientState(GL_VERTEX_ARRAY);
}

void CGame::CheckBonus()
{
 if (bonusy>height)
 {
  bonusy=frand()*BONUSMINY;
  bonusx=frand()*width;
  bonuspic.Init(12,30,30,48,&GlobalTex);
 }
 glColor4f(255,255,255,255);
 bonuspic.Draw(bonusx,bonusy,scale);
 bonusy+=BONUSVEL*GetFrameTime();
 
 TRect r=playership.ShipRect();
 if ((bonusx>=r.x1)&&(bonusx<=r.x2)&&(bonusy>=r.y1)&&(bonusy<=r.y2))
 {
  playership.SetPower(PLAYERPOWER);
  bonusy=100000000.f;
 }
}

void CGame::OutI(float x,float y,float s,int l)
{
 CPic pic;
 int j=0,k=l;
 while (k)
 {
  k/=10;
  j++;
 }
 int fx=22;
 int fy=2;
 glColor4f(255,255,255,255);
 for (int i=0; i<j; i++)
 {
  int m=l%10;
  l/=10;
  pic.Init(fx+m*4,fy,fx+m*4+4,fy+5,&GlobalTex);
  pic.Draw(x+(j-i)*4*s,y,s);
 }
}
