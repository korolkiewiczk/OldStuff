#include <windows.h>
#include <DX8/d3d8.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "wingraph.h"
#define SCRWIDTH 640
#define SCRHEIGHT 480
#define BPP 32
#include "engine.h"

#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_UP 72
#define KEY_DOWN 80

#define DV1 0.1
#define DV2 0.04

TMap map;

int mode=0;
int mapno=1;
int clip=0;

int main()
{
 if (!InitWinGraph()) return 0;
 if (!SetVidMode(SCRWIDTH,SCRHEIGHT,BPP,WINGRAPH_VSYNC)) {CloseApp(); return 0;}
 Float danglel=1,dangler=1;
 Float dvf=0,dvb=0;
 while (1)
 {
  if (mode==0)
  {
   char s[64];
   sprintf(s,"map%d.txt",mapno);
   Map_Load(s,&map,2);
   mode=1;
   continue;
  }
  if (!WinControl()) break;

  /*for (int i=48; i<=58; i++)
  {
   if (GetKey(i))
   {
    mapno=i-48;
    mode=0;
    break;
   }
  }*/
  for (int i=2; i<=12; i++)
  {
   if (GetKey(i))
   {
    mapno=i-2+1;
    mode=0;
    break;
   }
  }
  if (GetKey(81)) map.viewangle=0;
  if (GetKey(82)) clip=1-clip;
  if (KeyPressed(KEY_LEFT)) {map.viewangle-=MIN(danglel,3); danglel*=1.05;} else danglel=1;
  if (KeyPressed(KEY_RIGHT)) {map.viewangle+=MIN(dangler,3); dangler*=1.05;} else dangler=1;
  //if (map.viewangle<0) map.viewangle+=360;
  if (KeyPressed(KEY_UP))
  {
   dvf+=DV1;
   if (dvf>1) dvf=1;
  } 
  else 
  {
   dvf-=DV2;
   if (dvf<0) dvf=0;
  }
  if (KeyPressed(KEY_DOWN))
  {
   dvb-=DV1;
   if (dvb<-1) dvb=-1;
  } 
  else 
  {
   dvb+=DV2;
   if (dvb>0) dvb=0;
  }
  if (dvf) MoveViewPoint(&map,dvf,clip);
  if (dvb) MoveViewPoint(&map,dvb,clip);
  Render_Project_Buf_Line(0);
  Create_Project_Buf(&map);
  Render_Project_Buf_Line(RGB32(255,255,255));
  PresentBuf();
 }
 CloseApp();
 return 0;
}
