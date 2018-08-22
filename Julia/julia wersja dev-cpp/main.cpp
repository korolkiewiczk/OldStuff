#include <windows.h>
#include <DX8/d3d8.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "wingraph.h"
#include "biosfont.h"

int SCRWIDTH=640;
int SCRHEIGHT=480;
#define BPP 32
#define R 2.0
#define DELTAS 0.01
/*#define CMIN 80
#define CMAX 255
#define DELTAC 25
#define SINMUL 1
#define DELTACRE 1.5
#define DELTACIM 1
#define MINXRE -2.5
#define MINXIM -2.1
#define MAXXRE 2.5
#define MAXXIM 2.1*/
int CMIN=80;
int CMAX=255;
int DELTAC=25;
float SINMUL=1;
float DELTACRE=1.5;
float DELTACIM=1;
float MINXRE=-2.5;
float MINXIM=-2.1;
float MAXXRE=2.5;
float MAXXIM=2.1;
float POSX=0;
float POSY=0;

float tabsin[360];
int cvals[256];
int frame=0;
int method=0;


double GetTime()
{
 long long f,t;
 QueryPerformanceFrequency((PLARGE_INTEGER)&f);
 QueryPerformanceCounter((PLARGE_INTEGER)&t);
 return (double)t/(double)f;
}

void gensin()
{
 int i;
 for (i=0; i<360; i++) tabsin[i]=SINMUL*sin((float)i/180*3.1415);
}

void genpal(int rr,int gg,int bb)
{
 int r=(int)(255*sin((float)rr/180*3.1415));
 int g=(int)(255*sin((float)gg/180*3.1415));
 int b=(int)(255*sin((float)bb/180*3.1415));
 for (int i=0; i<256; i++) cvals[i]=RGB32(i+r,i+g,i+b);//((i<<16)+(i<<8)+i)|v;
}

void genpalrnd()
{
 srand((int)(GetTime()*1000));
 for (int i=0; i<256; i++) cvals[i]=((i<<16)+(i<<8)+i)|(rand()*rand());
}

void julia(float x_re,float x_im,float c_re,float c_im,float d_re,float d_im)
{
 int x,y,c;
 float x_re2;
 float x_re0=x_re,a_x_re,a_x_im=x_im;
 float mulc=(float)(CMAX-CMIN)/DELTAC;
 unsigned int *b;
 b=(unsigned int *)buf;
 for (y=0; y<SCRHEIGHT; y++)
 {
  a_x_re=x_re0;
  for (x=0; x<SCRWIDTH; x++)
  {
   x_re=a_x_re;
   x_im=a_x_im;
   c=DELTAC;
   while ((c--)&&(x_re*x_re+x_im*x_im<R*R))
   {
    x_re2=x_re*x_re-x_im*x_im+c_re;
    x_im=2*x_re*x_im+c_im;
    x_re=x_re2;
   }
   c=(int)((float)c*mulc)+CMIN;
   *b=cvals[c&255];
   b++;
   a_x_re+=d_re;
  }
  a_x_im+=d_im;
 }
}

void juliaint(float fx_re,float fx_im,float fc_re,float fc_im,float fd_re,float fd_im)
{
 #define SHVAL 13
 #define MULVAL (1<<SHVAL)
 int x,y=SCRHEIGHT,c;
 int x_re,x_im,c_re,c_im,d_re,d_im,x_re2,x_re0,a_x_re,a_x_im;
 int mulc=(int)((float)(CMAX-CMIN)/DELTAC*MULVAL);
 x_re=(int)(fx_re*MULVAL);
 x_im=(int)(fx_im*MULVAL);
 c_re=(int)(fc_re*MULVAL);
 c_im=(int)(fc_im*MULVAL);
 d_re=(int)(fd_re*MULVAL);
 d_im=(int)(fd_im*MULVAL);
 x_re0=x_re;
 a_x_im=x_im;
 unsigned int *b;
 b=(unsigned int *)buf;
 while (y--)
 {
  a_x_re=x_re0;
  x=SCRWIDTH;
  while (x--)
  {
   x_re=a_x_re;
   x_im=a_x_im;
   c=DELTAC;
   while ((c--)&&(x_re*x_re+x_im*x_im<(int)(R*R*MULVAL*MULVAL)))
   {
    x_re2=((x_re*x_re-x_im*x_im)>>SHVAL)+c_re;
    x_im=((2*x_re*x_im)>>SHVAL)+c_im;
    x_re=x_re2;
   }
   c=(c*mulc>>SHVAL)+CMIN;
   *b=cvals[c&255];
   b++;
   a_x_re+=d_re;
  }
  a_x_im+=d_im;
 }
}

extern "C"
{
 void _juliaintasm(int,int,int,int,int,int,void*,int,int,int);
}
void juliaintasm(float fx_re,float fx_im,float fc_re,float fc_im,float fd_re,float fd_im)
{
 int x_re,x_im,c_re,c_im,d_re,d_im;
 x_re=(int)(fx_re*MULVAL);
 x_im=(int)(fx_im*MULVAL);
 c_re=(int)(fc_re*MULVAL);
 c_im=(int)(fc_im*MULVAL);
 d_re=(int)(fd_re*MULVAL);
 d_im=(int)(fd_im*MULVAL);
 _juliaintasm(x_re,x_im,c_re,c_im,d_re,d_im,buf,(int)((float)(CMAX-CMIN)/DELTAC*MULVAL),SCRWIDTH,SCRHEIGHT);
}
#define MAXTHREADS 4
float tfx_re,tfx_im[MAXTHREADS],tfc_re,tfc_im,tfd_re,tfd_im;
int sh;
void *bufr[MAXTHREADS];
void *bf0;
int numofthreads=1;
DWORD WINAPI _juliathreaded(LPVOID i)
{
 float x_re,x_im,c_re,c_im,d_re,d_im;
 x_re=tfx_re;
 x_im=tfx_im[(int)i];
 c_re=tfc_re;
 c_im=tfc_im;
 d_re=tfd_re;
 d_im=tfd_im;
 buf=bufr[(int)i];
 if (method==0)
 julia(x_re,x_im,c_re,c_im,d_re,d_im);
 else
 if (method==1)
 juliaint(x_re,x_im,c_re,c_im,d_re,d_im);
 else
 juliaintasm(x_re,x_im,c_re,c_im,d_re,d_im);
 ExitThread(0);
 return 0;
}

void juliathreaded(float fx_re,float fx_im,float fc_re,float fc_im,float fd_re,float fd_im)
{
 DWORD d;
 HANDLE h[MAXTHREADS];
 void *b;
 b=buf;
 bf0=buf;
 sh=SCRHEIGHT;
 tfx_re=fx_re;
 tfc_re=fc_re;
 tfc_im=fc_im;
 tfd_re=fd_re;
 tfd_im=fd_im;
 SCRHEIGHT=SCRHEIGHT/numofthreads;
 for (int i=0; i<numofthreads; i++)
 {
  tfx_im[i]=fx_im; 
  bufr[i]=b;
  h[i]=CreateThread(0,0x1000,_juliathreaded,0,0,(LPDWORD)&i);
  fx_im+=((float)SCRHEIGHT*fd_im);
  b=(void*)((int)bf0+4*(SCRWIDTH*(SCRHEIGHT*(i+1))));
 }
 WaitForMultipleObjects(numofthreads,(HANDLE*)&h,FALSE,INFINITE);
 SCRHEIGHT=sh;
 buf=bf0;
 for (int i=0; i<numofthreads; i++) CloseHandle(h[i]);
}

int textblank=255;
void DrawText(int x,int y,int color,char *s)
{
 if (textblank==0) return;
 int xs=0;
 while (*s)
 {
  char c=*s;
  if (c=='\n') {y+=8; xs=0; s++; continue;}
  for (int j=0; j<8; j++)
  {
   unsigned char line=biosfont[c*8+j];
   for (int i=0; i<8; i++)
   {
    if (((line>>(7-i))&0x1)==1) 
    {
     int xx=x+i+xs;
     int yy=y+j;
     int cl=*(unsigned int*)((int)buf+(yy)*pitch+((xx)<<2));
     int r=(cl>>16)&255;
     int g=(cl>>8)&255;
     int b=cl&255;
     r=(((color>>16)&255)*textblank+r*(255-textblank))>>8;
     g=(((color>>8)&255)*textblank+g*(255-textblank))>>8;
     b=(((color)&255)*textblank+b*(255-textblank))>>8;
     //cl=((color&255)*textblank+cl*(255-textblank))>>8;
     SetBuf32(x+i+xs,y+j,RGB32(r,g,b));
    }
   }
  }
  s++;
  xs+=8;
 }
}

int stop=0;
char *methodnames[3]={"Dokladna(FPU)","Szybka(CPU-INT32)","Bardzo szybka(ASM:CPU-INT32)"};
int vcol=0;
float maxfps=0,minfps=1000000;
#define NVIDMODES 4
int vidmodes[NVIDMODES][2]={{640,480},{320,240},{800,600},{1024,768}};

int main()
{
 if (!InitWinGraph()) return 0;
 if (!SetVidMode(SCRWIDTH,SCRHEIGHT,BPP,!WINGRAPH_VSYNC)) {CloseApp(); return 0;}
 gensin();
 genpal(0,0,0);
 while (1)
 {
  double t0=GetTime();
  if (!WinControl()) break;
  for (int i=0; i<256; i++) if (KeyPressed(i))
  {
   textblank=256;
   break;
  }
  if ((!stop)&&(frame%2==0))textblank--;
  if (textblank<0) textblank=0;
  if (GetKey(1)) break;
  if (GetKey(11)) {maxfps=0; minfps=1000000;method++;} //0
  if (method>2) method=0;
  for (int i=0; i<NVIDMODES; i++)
  {
   if (GetKey(i+59))
   {
    SCRWIDTH=vidmodes[i][0];
    SCRHEIGHT=vidmodes[i][1];
    if (!SetVidMode(SCRWIDTH,SCRHEIGHT,BPP,!WINGRAPH_VSYNC))
    if (!SetVidMode(SCRWIDTH,SCRHEIGHT,BPP,WINGRAPH_VSYNC)) return 0;
    break;
   }
  }
  if (KeyPressed(29)) //ctrl
  {
   for (int i=0; i<MAXTHREADS; i++)
   {
    if (GetKey(i+2)) numofthreads=i+1;
   }
  }
  else
  if (KeyPressed(42)) //shift
  {
   if (KeyPressed(28)) genpal(vcol/(rand()%3+1),vcol/(rand()%3+1),vcol/(rand()%3+1));
   vcol++;
   if (KeyPressed(57)) genpalrnd();
   if (KeyPressed(72)) {SINMUL+=DELTAS; gensin();}
   if (KeyPressed(80)) {SINMUL-=DELTAS;gensin();}
   if (KeyPressed(2)) CMIN--;  //1
   if (KeyPressed(3)) CMAX--;  //2
   if (KeyPressed(4)) DELTAC--;  //3
   if (KeyPressed(5)) DELTACRE-=DELTAS; //4
   if (KeyPressed(6)) DELTACIM-=DELTAS; //5
   if (KeyPressed(7)) MINXRE*=(1+DELTAS); //6
   if (KeyPressed(8)) MINXIM*=(1+DELTAS); //7
   if (KeyPressed(9)) MAXXRE*=(1+DELTAS); //8
   if (KeyPressed(10)) MAXXIM*=(1+DELTAS); //9
  }
  else
  {
   if (KeyPressed(28)) genpal(vcol,vcol/2,vcol/3);
   vcol++;
   if (GetKey(57)) stop=1-stop;
   if (KeyPressed(2)) CMIN++;  //1
   if (KeyPressed(3)) CMAX++;  //2
   if (KeyPressed(4)) DELTAC++;  //3
   if (KeyPressed(5)) DELTACRE+=DELTAS; //4
   if (KeyPressed(6)) DELTACIM+=DELTAS; //5
   if (KeyPressed(7)) MINXRE/=(1+DELTAS); //6
   if (KeyPressed(8)) MINXIM/=(1+DELTAS); //7
   if (KeyPressed(9)) MAXXRE/=(1+DELTAS); //8
   if (KeyPressed(10)) MAXXIM/=(1+DELTAS); //9
   float dxy=sqrt((MAXXRE-MINXRE)*(MAXXRE-MINXRE)+(MAXXIM-MINXIM)*(MAXXIM-MINXIM));
   /*if (KeyPressed(72)) {MINXIM+=DELTAS; MAXXIM+=DELTAS;}
   if (KeyPressed(80)) {MINXIM-=DELTAS; MAXXIM-=DELTAS;}
   if (KeyPressed(75)) {MINXRE+=DELTAS; MAXXRE+=DELTAS;}
   if (KeyPressed(77)) {MINXRE-=DELTAS; MAXXRE-=DELTAS;}*/
   if (KeyPressed(72)) POSY+=DELTAS*dxy;
   if (KeyPressed(80)) POSY-=DELTAS*dxy;
   if (KeyPressed(75)) POSX+=DELTAS*dxy;
   if (KeyPressed(77)) POSX-=DELTAS*dxy;
  }
  if (DELTAC<0) DELTAC=0;
  if (DELTAC>255) DELTAC=255;
  
  if (numofthreads==1)
  {
  if (method==0)
  julia(MINXRE+POSX,MINXIM+POSY,
  tabsin[(int)((float)frame*DELTACRE)%360],tabsin[(int)((float)frame*DELTACIM)%360],
  (float)(MAXXRE-MINXRE)/(float)SCRWIDTH,(float)(MAXXIM-MINXIM)/(float)SCRHEIGHT);
  else
  if (method==1)
  juliaint(MINXRE+POSX,MINXIM+POSY,
  tabsin[(int)((float)frame*DELTACRE)%360],tabsin[(int)((float)frame*DELTACIM)%360],
  (float)(MAXXRE-MINXRE)/(float)SCRWIDTH,(float)(MAXXIM-MINXIM)/(float)SCRHEIGHT);
  else
  juliaintasm(MINXRE+POSX,MINXIM+POSY,
  tabsin[(int)((float)frame*DELTACRE)%360],tabsin[(int)((float)frame*DELTACIM)%360],
  (float)(MAXXRE-MINXRE)/(float)SCRWIDTH,(float)(MAXXIM-MINXIM)/(float)SCRHEIGHT);
  }
  else
  juliathreaded(MINXRE+POSX,MINXIM+POSY,
  tabsin[(int)((float)frame*DELTACRE)%360],tabsin[(int)((float)frame*DELTACIM)%360],
  (float)(MAXXRE-MINXRE)/(float)SCRWIDTH,(float)(MAXXIM-MINXIM)/(float)SCRHEIGHT);
  
  char bf[256];
  double fps=1/(GetTime()-t0);
  sprintf(bf,"ZBIOR JULII.\nPROGRAM: KAMIL KOROLKIEWICZ\nMetoda:%s\nLiczba watkow:%d\nFPS=%0.2f(min:%0.2f,max:%0.2f)\nCMIN=%d\nCMAX=%d\nDELTAC=%d\nSINMUL=%0.2f\nDELTACRE=%0.2f\nDELTACIM=%0.2f\nMINXRE=%0.4f\nMINXIM=%0.4f\nMAXXRE=%0.4f\nMAXXIM=%0.4f",
  methodnames[method],
  numofthreads,
  fps,minfps,maxfps,
  CMIN,
  CMAX,
  DELTAC,
  SINMUL,
  DELTACRE,
  DELTACIM,
  MINXRE,
  MINXIM,
  MAXXRE,
  MAXXIM
  );
  char cl=(255-*(unsigned char*)buf);
  DrawText(0,0,RGB32(cl,cl,cl),bf);
  
  if (fps>maxfps) maxfps=fps;
  if (fps<minfps) minfps=fps;
  
  PresentBuf();
  if (!stop) frame++;
 }
 CloseApp();
 return 0;
}
