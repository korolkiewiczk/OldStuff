//main.cpp

/*
  S60Graph by Kamil Korolkiewicz 2008
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "biosfont.h"

//======================================

#define SHIPLENGTH 6.0
#define WINGSLENGTH 3.0
#define SHIPCOLOR RGBCol(220,220,210)
#define MAXFUEL 3500
#define ALPHADELTA 3
#define MAXLANDVEL 0.5

#define LAND_MINPOINTS 12
#define LAND_MAXPOINTS 25
#define LAND_MINHEIGHT 5
#define LAND_MAXHEIGHT 180
#define LAND_MAXWIDTH 640
#define LAND_MAXBUMP ((LAND_MAXHEIGHT-LAND_MINHEIGHT)/3)

#define RAD(a) ((a)*3.1415/180)

#define K(a) if (KeyPressed(a))

#define T 450.0
#define G (10.0/T)
#define F (20.0/T)
#define M (1)
#define FUELM (0.0001)

#define MAXPARTS 4900
#define MAXPARTPOWER 50

#define MAXSTARS 250

#define BPP 32

#if (BPP==16)
#define SetBuf(x,y,c) SetBuf16(x,y,c)
#define RGBCol(r,g,b) RGB16(r,g,b)
#else
#define SetBuf(x,y,c) SetBuf32(x,y,c)
#define RGBCol(r,g,b) RGB32(r,g,b)
#endif

//======================================

unsigned int landpoints[LAND_MAXWIDTH][2];                   //[][0]-height,[][0]-alpha
int landbuf[LAND_MAXWIDTH][LAND_MAXHEIGHT];

typedef struct _Star
{
 int x,y,color;
} Star;

Star stars[MAXSTARS];

typedef struct _Part
{
 float x,y,vx,vy;
 int color,power;
} Part;
Part parttab[MAXPARTS];
int partl=0,lastpart=0;

//======================================

void DrawText(int x,int y,int color,char *s)
{
 int xs=0;
 while (*s)
 {
  char c=*s;
  if (c=='\n') 
  {
   y+=8;
   xs=0;
   s++;
   continue;
  }
  for (int j=0; j<8; j++)
  {
   unsigned char line=biosfont[c*8+j];
   for (int i=0; i<8; i++)
   {
    if (((line>>(7-i))&0x1)==1) SetBuf(x+i+xs,y+j,color); else SetBuf(x+i+xs,y+j,0);
   }
  }
  s++;
  xs+=8;
 }
}

void Line(int x1,int y1,int x2,int y2,int color)
{
 if ((x1<0)||(x2<0)||(y1<0)||(y2<0)||(x1>GetScrWidth())||(x2>GetScrWidth())||(y1>GetScrHeight())||(y2>GetScrHeight())) return;
 float delta=fabs((float)(y1-y2)/(float)(x1-x2));
 if (delta<=1)
 {
  float y;
  if (y1-y2>0) delta=-delta;
  if (x1>x2) {int pom=x2; x2=x1; x1=pom; y=y2; delta=-delta;}
  else y=y1;
  for (int i=x1; i<=x2; i++)
  {
   SetBuf(i,(int)y,color);
   y+=delta;
  }
 }
 else
 {
  float x;
  if (x1-x2>0) delta=-delta;
  if (y1>y2) {int pom=y2; y2=y1; y1=pom; delta=-delta; x=x2;}
  else x=x1;
  delta=1/delta;
  for (int i=y1; i<=y2; i++)
  {
   SetBuf((int)x,i,color);
   x+=delta;
  }
 }
}

void DrawSpaceShip(float x,float y,int color,float alpha)
{
 float kx,ky;
 kx=SHIPLENGTH/2*sin(RAD(alpha));
 ky=SHIPLENGTH/2*cos(RAD(alpha));
 //gora
 Line ((int)x,(int)y,(int)(kx+x),(int)(-ky+y),color);
 //dol
 Line ((int)x,(int)y,(int)(-kx+x),(int)(ky+y),color);
 //boki
 Line ((int)(-kx+x),(int)(ky+y),(int)(-kx+x-WINGSLENGTH*sin(RAD(alpha+45))),(int)(ky+y+WINGSLENGTH*cos(RAD(alpha+45))),color);
 Line ((int)(-kx+x),(int)(ky+y),(int)(-kx+x-WINGSLENGTH*sin(RAD(alpha+45-90))),(int)(ky+y+WINGSLENGTH*cos(RAD(alpha+45-90))),color);
}

void GenLand()
{
 int points=rand()%(LAND_MAXPOINTS-LAND_MINPOINTS+1)+LAND_MINPOINTS;
 int point1,point2,point1x,point2x;
 point1=rand()%(LAND_MAXHEIGHT-LAND_MINHEIGHT+1)+LAND_MINHEIGHT;
 point1x=0;
 for (int i=0; i<points; i++)
 {
  do
  {
   point2=rand()%(LAND_MAXHEIGHT-LAND_MINHEIGHT+1)+LAND_MINHEIGHT;
  } while (abs(point2-point1)>LAND_MAXBUMP);
  point2x=point1x+(GetScrWidth()/points)+rand()%4;
  if (point2x>=GetScrWidth()) point2x=GetScrWidth();
  
  //rysuj
  float deltay=(float)(point1-point2)/(float)(point1x-point2x);
  float y=point1;
  int alpha=(int)(atan(-deltay)*180/3.1415);
  int j,k;
  for (j=point1x; j<point2x; j++)
  {
   int sy=(int)(GetScrHeight()-y);
   landpoints[j][0]=sy;
   landpoints[j][1]=alpha;
   for (k=sy; k<GetScrHeight(); k++)
   landbuf[j][k-sy]=RGBCol((240+rand()%10),(120+rand()%15),0);
   y+=deltay;
  }
  
  point1=point2;
  point1x=point2x;
 }
}

void GenStars()
{
 for (int i=0; i<MAXSTARS; i++) 
 {
  if (((rand()%(MAXSTARS/15))==0)&&(i<MAXSTARS-5))
  {
   int x=rand()%(GetScrWidth()-2)+1;
   int y=rand()%(GetScrHeight()-2)+1;
   int c=rand()%50+205;
   c=RGBCol(c,c,c);
   stars[i].x=x;
   stars[i].y=y;
   stars[i].color=c;
   i++;
   stars[i].x=x-1;
   stars[i].y=y;
   stars[i].color=c;
   i++;
   stars[i].x=x+1;
   stars[i].y=y;
   stars[i].color=c;
   i++;
   stars[i].x=x;
   stars[i].y=y-1;
   stars[i].color=c;
   i++;
   stars[i].x=x;
   stars[i].y=y+1;
   stars[i].color=c;
  }
  else
  {
   stars[i].x=rand()%GetScrWidth();
   stars[i].y=rand()%GetScrHeight();
   int c=rand()%200+56;
   stars[i].color=RGBCol(c,c,c);
  }
 }
}

void DrawStars()
{
 for (int i=0; i<MAXSTARS; i++) SetBuf(stars[i].x,stars[i].y,stars[i].color);
}

void DrawLand()
{
 for (int i=0; i<GetScrWidth(); i++)
 {
  #if (BPP==16)
  int k=landpoints[i][0]*pitch+(i<<1);
  #else
  int k=landpoints[i][0]*pitch+(i<<2);
  #endif
  int *l;
  l=landbuf[i];
  for (int j=landpoints[i][0]; j<GetScrHeight(); j++) //SetBuf(i,j,landbuf[i][j-landpoints[i][0]]);
  {
   #if (BPP==16)
   *(unsigned short*)((int)buf+k)=*l;
   #else
   *(unsigned int*)((int)buf+k)=*l;
   #endif
   k+=pitch;
   l++;
  }
 }
}

//0-no, 1-ok, -1-crash
int CheckCollision(int x,int y,int alpha,float vel)
{
 if ((x<0)||(x>=GetScrWidth())) return (y<GetScrHeight())-1;
 if (landpoints[x][0]<=y+SHIPLENGTH/2*cos(RAD(alpha)))
 {
  if ((abs(alpha-landpoints[x][1])<=4*ALPHADELTA)&&(vel<=MAXLANDVEL)) return 1;
  else return -1;
 }
 else
 return 0;
}

void AddPart(float x,float y,float velx,float vely,int color)
{
 if ((x<0)||(x>=GetScrWidth())||(y<0)||(y>=GetScrHeight())) return;
 int j=partl%MAXPARTS;
 parttab[j].x=(int)x;
 parttab[j].y=(int)y;
 parttab[j].vx=velx;
 parttab[j].vy=vely;
 parttab[j].color=color;
 parttab[j].power=MAXPARTPOWER;
 partl++;
}

void AddTrustParts(float x,float y,float vx,float vy,float alpha)
{
 float px=x-SHIPLENGTH/2*sin(RAD(alpha));
 float py=y+SHIPLENGTH/2*cos(RAD(alpha));
 for (int i=0; i<4; i++) 
 {
  float vvx=-sin(RAD(alpha+rand()%(ALPHADELTA*2)-ALPHADELTA))-vx;
  float vvy=cos(RAD(alpha+rand()%(ALPHADELTA*2)-ALPHADELTA))+vy;
  AddPart(px,py,vvx,vvy,RGBCol(240+rand()%10,rand()%256,50));
 }
}

void Boom(float x,float y,float v)
{
 for (int i=0; i<360; i++)
 {
  float vx=sin(RAD(i))*v/2;
  float vy=cos(RAD(i))*v/2;
  int c=128+rand()%128;
  for (int j=0; j<10; j++)
  AddPart(x+rand()%6,y+rand()%6,vx,vy,RGBCol(c,c,c*((rand()%5)!=0)));
 }
}

void DrawParts()
{
 for (int i=lastpart; i<partl; i++)
 {
  int j=i%MAXPARTS;
  SetBuf((int)parttab[j].x,(int)parttab[j].y,0);
  parttab[j].power--;
  if (parttab[j].power<=0) {lastpart++; continue;}
  parttab[j].x+=parttab[j].vx;
  parttab[j].y+=parttab[j].vy;
  parttab[j].vy+=G;
  if ((parttab[j].x<0)||(parttab[j].x>=GetScrWidth())||(parttab[j].y<0)||(parttab[j].y>=GetScrHeight())) 
  {
   if (i!=lastpart) parttab[j]=parttab[lastpart%MAXPARTS];
   lastpart++;
   continue;
  }
  int x=(int)parttab[j].x;
  int y=(int)parttab[j].y;
#if (BPP==16)
  SetBuf(x,y,
  RGBCol((int)(((parttab[j].color>>11)<<3)*((float)parttab[j].power/MAXPARTPOWER)),    //r
  (int)((((parttab[j].color>>5)&63)<<2)*((float)parttab[j].power/MAXPARTPOWER)),      //g
  (int)(((parttab[j].color&31)<<3)*((float)parttab[j].power/MAXPARTPOWER))));         //b
#else
  SetBuf(x,y,
  RGBCol((int)((parttab[j].color>>16)*((float)parttab[j].power/MAXPARTPOWER)),    //r
  (int)(((parttab[j].color>>8)&255)*((float)parttab[j].power/MAXPARTPOWER)),      //g
  (int)((parttab[j].color&255)*((float)parttab[j].power/MAXPARTPOWER))));         //b
#endif
 }
}

float x,y,vx,vy,alpha,prevx,prevy,prevalpha,gamestate;
int fuel;

void ClearBuf(int c)
{
 memset(buf,c,pitch*GetScrHeight());
}

float t0;
//if returns 0 program exits else goes to MainLoop()
int AppInit()
{
 srand(time(0));
 t0=GetTime();
 return 1;
}

int mode=0;
int frames,fps;
//if returns 0 program goes to AtExit()
int AppMainLoop()
{
 static int scrw=0;
 if (GetScrWidth()!=scrw)
 {
  scrw=GetScrWidth();
  mode=0;
 }
 frames++;
 fps=(int)((float)frames/(GetTime()-t0));
 if (mode==0)
 {
  //restart:
  GenStars();
  partl=lastpart=0;
  fuel=MAXFUEL;
  ClearBuf(0);
  gamestate=0;
  x=prevx=GetScrWidth()/2;
  y=prevy=GetScrHeight()/4;
  vx=vy=alpha=0;
  GenLand();
  mode=1;
 }
 if (mode==1)
 {
  char bf[128];
  float a=F/(M+fuel*FUELM);
  int xx=(int)x;
  sprintf(bf,"X=%d Y=%d \1=%d G\1=%d \nVX=%4.4f VY=%4.4f A=%4.4f \nFuel:%d H=%d \nframes=%d fps=%d ",
  (int)x,GetScrHeight()-(int)y,
  (int)alpha,((unsigned int)xx<GetScrWidth())? landpoints[xx][1]:0,
  vx,vy,a,fuel,
  ((unsigned int)xx<GetScrWidth())? landpoints[xx][0]-(int)y:GetScrHeight()-(int)y,
  frames,fps);
  DrawText(0,0,RGBCol(255,255,250),bf);
  if (fuel>0)
  if(KeyPressed(16)||KeyPressed(167)||KeyPressed(50)||KeyPressed(53)
  ||KeyPressed(56)||KeyPressed(48))
  {
   vx+=a*sin(RAD(alpha));
   vy-=a*cos(RAD(alpha));
   AddTrustParts(x,y,vx,vy,alpha);
   fuel--;
  }
  if (KeyPressed(14)||KeyPressed(49)||KeyPressed(52)
  ||KeyPressed(55)||KeyPressed(42)) alpha-=ALPHADELTA;
  if (KeyPressed(15)||KeyPressed(51)||KeyPressed(54)
  ||KeyPressed(57)||KeyPressed(127)) alpha+=ALPHADELTA;
  
  K(164) mode=2;
  K(165) mode=2;
  
  vy+=G;
  
  x+=vx;
  y+=vy;
  
  DrawStars();
  
  DrawParts();
  
  DrawLand();
  
  DrawSpaceShip(prevx,prevy,0,prevalpha);
  DrawSpaceShip(x,y,SHIPCOLOR,alpha);
  
  prevx=x;
  prevy=y;
  prevalpha=alpha;
  
  gamestate=CheckCollision((int)x,(int)y,((int)alpha)%360,sqrt(vx*vx+vy*vy));
  if (gamestate!=0) mode=2;
 }
 if (mode==2)
 {
  GetKey(165);
  char infos[128];
  if (gamestate==0) strcpy(infos,"Game ended");
  else
  if (gamestate==1) sprintf(infos,"You win!!!\nYour landing vel.=%4.2f",sqrt(vx*vx+vy*vy));
  else
  {
   strcpy(infos,"You lose");
   DrawSpaceShip(prevx,prevy,0,prevalpha);
   Boom(prevx,prevy,sqrt(vx*vx+vy*vy));
  }
  char tbf[128];
  sprintf(tbf,"%s.\nPress [0] to exit\nor [1] to restart",infos);
  DrawText(0,32,RGBCol(200,150,100),tbf);
  mode=3;
 }
 if (mode==3)
 {
  K(48) return 0;
  if (GetKey(49)) mode=0;
  DrawParts();
  DrawStars();
  DrawLand();
  if (gamestate!=-1) DrawSpaceShip(x,y,SHIPCOLOR,alpha);
 }
 return 1;
}

void AppExit()
{
}
