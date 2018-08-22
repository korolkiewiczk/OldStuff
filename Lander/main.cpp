#include <windows.h>
#include <DX8/d3d8.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "wingraph.h"
#include "biosfont.h"

//======================================

#define SCRWIDTH 640
#define SCRHEIGHT 480
#define BPP 16

#define SHIPLENGTH 6.0
#define WINGSLENGTH 3.0
#define SHIPCOLOR RGBCol(220,220,210)
#define MAXFUEL 3500
#define ALPHADELTA 6
#define MAXLANDVEL 0.5

#define LAND_MINPOINTS 20
#define LAND_MAXPOINTS 30
#define LAND_MINHEIGHT 5
#define LAND_MAXHEIGHT 200
#define LAND_MAXBUMP ((LAND_MAXHEIGHT-LAND_MINHEIGHT)/3)

#define RAD(a) ((a)*3.1415/180)

#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESC 1
#define KEY_SPACE 57

#define K(a) if (KeyPressed(a))

#define T 450.0
#define G (10.0/T)
#define F (20.0/T)
#define M (1)
#define FUELM (0.0001)

#define MAXPARTS 3900
#define MAXPARTPOWER 40

#define MAXSTARS 100


#if (BPP==16)
#define SetBuf(x,y,c) SetBuf16(x,y,c)
#define RGBCol(r,g,b) RGB16(r,g,b)
#else
#define SetBuf(x,y,c) SetBuf32(x,y,c)
#define RGBCol(r,g,b) RGB32(r,g,b)
#endif

//======================================

unsigned int landpoints[SCRWIDTH][2];                   //[][0]-height,[][0]-alpha
int landbuf[SCRWIDTH][LAND_MAXHEIGHT];

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
 if ((x1<0)||(x2<0)||(y1<0)||(y2<0)||(x1>SCRWIDTH)||(x2>SCRWIDTH)||(y1>SCRHEIGHT)||(y2>SCRHEIGHT)) return;
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
  point2x=point1x+(SCRWIDTH/points)+rand()%4;
  if (point2x>=SCRWIDTH) point2x=SCRWIDTH;
  
  //rysuj
  float deltay=(float)(point1-point2)/(float)(point1x-point2x);
  float y=point1;
  int alpha=(int)(atan(-deltay)*180/3.1415);
  int j,k;
  for (j=point1x; j<point2x; j++)
  {
   int sy=(int)(SCRHEIGHT-y);
   landpoints[j][0]=sy;
   landpoints[j][1]=alpha;
   for (k=sy; k<SCRHEIGHT; k++)
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
  if (((rand()%(MAXSTARS/10))==0)&&(i<MAXSTARS-5))
  {
   int x=rand()%(SCRWIDTH-2)+1;
   int y=rand()%(SCRHEIGHT-2)+1;
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
   stars[i].x=rand()%SCRWIDTH;
   stars[i].y=rand()%SCRHEIGHT;
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
 for (int i=0; i<SCRWIDTH; i++)
 {
  #if (BPP==16)
  int k=landpoints[i][0]*pitch+(i<<1);
  #else
  int k=landpoints[i][0]*pitch+(i<<2);
  #endif
  int *l;
  l=landbuf[i];
  for (int j=landpoints[i][0]; j<SCRHEIGHT; j++) //SetBuf(i,j,landbuf[i][j-landpoints[i][0]]);
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
 if ((x<0)||(x>=SCRWIDTH)) return (y<SCRHEIGHT)-1;
 if (landpoints[x][0]<=y+SHIPLENGTH/2*cos(RAD(alpha)))
 {
  if ((abs(alpha-landpoints[x][1])<=2*ALPHADELTA)&&(vel<=MAXLANDVEL)) return 1;
  else return -1;
 }
 else
 return 0;
}

void AddPart(float x,float y,float velx,float vely,int color)
{
 if ((x<0)||(x>=SCRWIDTH)||(y<0)||(y>=SCRHEIGHT)) return;
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
  if ((parttab[j].x<0)||(parttab[j].x>=SCRWIDTH)||(parttab[j].y<0)||(parttab[j].y>=SCRHEIGHT)) 
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

int main()
{
 if (!InitWinGraph()) return 0;
 if (!SetVidMode(SCRWIDTH,SCRHEIGHT,BPP,WINGRAPH_VSYNC)) {CloseApp(); return 0;}
 srand(time(0));
restart:
 GenStars();
 partl=lastpart=0;
 fuel=MAXFUEL;
 ClearBuf(0);
 gamestate=0;
 x=prevx=SCRWIDTH/2;
 y=prevy=SCRHEIGHT/4;
 vx=vy=alpha=0;
 GenLand();
 Cursor(0);
 while (gamestate==0)
 {
  if (!WinControl()) break;
  char bf[128];
  float a=F/(M+fuel*FUELM);
  sprintf(bf,"X=%d Y=%d \1=%d G\1=%d VX=%4.4f VY=%4.4f A=%4.4f Fuel:%d H=%d ",(int)x,SCRHEIGHT-(int)y,(int)alpha,((unsigned int)x<SCRWIDTH)? landpoints[(int)x][1]:0,vx,vy,a,fuel,((unsigned int)x<SCRWIDTH)? landpoints[(int)x][0]-(int)y:SCRHEIGHT-(int)y);
  DrawText(0,0,RGBCol(255,255,250),bf);
  if (fuel>0)
  K(KEY_SPACE)
  {
   vx+=a*sin(RAD(alpha));
   vy-=a*cos(RAD(alpha));
   AddTrustParts(x,y,vx,vy,alpha);
   fuel--;
  }
  K(KEY_RIGHT) alpha+=ALPHADELTA;
  K(KEY_LEFT) alpha-=ALPHADELTA;
  K(KEY_ESC) break;
  
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
  
  PresentBuf();
 }
 GetKey(KEY_ESC);
 char infos[128];
 if (gamestate==0) strcpy(infos,"Game ended");
 else
 if (gamestate==1) sprintf(infos,"You win!!! Your landing vel. was %4.2f",sqrt(vx*vx+vy*vy));
 else
 {
  strcpy(infos,"You lose");
  DrawSpaceShip(prevx,prevy,0,prevalpha);
  Boom(prevx,prevy,sqrt(vx*vx+vy*vy));
 }
 char tbf[128];
 sprintf(tbf,"%s. Press [esc] to exit or [1] to restart",infos);
 DrawText(SCRWIDTH/2-strlen(tbf)*4,SCRHEIGHT/2,RGBCol(200,150,100),tbf);
 PresentBuf();
 while (1)
 {
  if (!WinControl()) break;
  K(KEY_ESC) break;
  K(2) goto restart;
  DrawParts();
  DrawStars();
  DrawLand();
  if (gamestate!=-1) DrawSpaceShip(x,y,SHIPCOLOR,alpha);
  PresentBuf();
 }
 
 Cursor(1);
 CloseApp();
 return 0;
}
