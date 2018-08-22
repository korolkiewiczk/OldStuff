#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "biosfont.h"

#define BPP 32
#define HZ 1000
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
 for (y=0; y<GetScrHeight(); y++)
 {
  a_x_re=x_re0;
  for (x=0; x<GetScrWidth(); x++)
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
 int x,y=GetScrHeight(),c;
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
  x=GetScrWidth();
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

int textblank=255;
void DrawText(int x,int y,int color,char *s)
{
 if (textblank==0) return;
 int xs=0;
 while (*s)
 {
  char c=*s;
  if (c=='\n') {y+=8; xs=0; s++; continue;}
  if (x+8+xs+GetScrWidth()*(y+8)>=GetScrWidth()*GetScrHeight()) return;
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

//if returns 0 program exits else goes to MainLoop()
int AppInit()
{
 gensin();
 genpal(0,0,0);
 return 1;
}

int method=1;
int stop=0;
int vmode=0;
int lastfps=0;
char *methodnames[2]={"Dokladna(FPU)","Szybka(CPU-INT32)"};
int vcol=0;
int notext=0;

//if returns 0 program goes to AtExit()
int AppMainLoop()
{
  float t0=GetTime();
  
  if (!notext)
  {
   for (int i=0; i<256; i++) if (KeyPressed(i))
   {
    textblank=256;
    break;
   }
  }
  else
  textblank=0;
  if (!stop)textblank-=(60/(lastfps+1)<1)? 1:60/(lastfps+1);
  if (textblank<0) textblank=0;
  if (GetKey(165)) return 0;
  if (KeyPressed(EStdKeyLeftShift)) //shift
  {
   if (GetKey(164))
   {
    vmode=1-vmode;
    SetVidMode(s60graph_scrwidth/4,s60graph_scrheight/4,BPP,(vmode==0)? S60GRAPH_AUTOVIDMODE:0);
   }
   if (KeyPressed(167)) genpal(vcol/(rand()%3+1),vcol/(rand()%3+1),vcol/(rand()%3+1));
   vcol++;
   if (GetKey(48)) notext=1-notext; //0
   if (KeyPressed(196)) genpalrnd();
   if (KeyPressed(EStdKeyUpArrow)) {SINMUL+=DELTAS; gensin();}
   if (KeyPressed(EStdKeyDownArrow)) {SINMUL-=DELTAS;gensin();}
   if (KeyPressed(49)) CMIN--;  //1
   if (KeyPressed(50)) CMAX--;  //2
   if (KeyPressed(51)) DELTAC--;  //3
   if (KeyPressed(52)) DELTACRE-=DELTAS; //4
   if (KeyPressed(53)) DELTACIM-=DELTAS; //5
   if (KeyPressed(54)) MINXRE*=(1+DELTAS); //6
   if (KeyPressed(55)) MINXIM*=(1+DELTAS); //7
   if (KeyPressed(56)) MAXXRE*=(1+DELTAS); //8
   if (KeyPressed(57)) MAXXIM*=(1+DELTAS); //9
  }
  else
  {
   if (GetKey(164))
   {
    vmode=1-vmode;
    SetVidMode(s60graph_scrwidth/2,s60graph_scrheight/2,BPP,(vmode==0)? S60GRAPH_AUTOVIDMODE:0);
   }
   if (KeyPressed(167)) genpal(vcol,vcol/2,vcol/3);
   vcol++;
   if (GetKey(196)) stop=1-stop;
   if (GetKey(48)) method=1-method; //0
   if (KeyPressed(49)) CMIN++;  //1
   if (KeyPressed(50)) CMAX++;  //2
   if (KeyPressed(51)) DELTAC++;  //3
   if (KeyPressed(52)) DELTACRE+=DELTAS; //4
   if (KeyPressed(53)) DELTACIM+=DELTAS; //5
   if (KeyPressed(54)) MINXRE/=(1+DELTAS); //6
   if (KeyPressed(55)) MINXIM/=(1+DELTAS); //7
   if (KeyPressed(56)) MAXXRE/=(1+DELTAS); //8
   if (KeyPressed(57)) MAXXIM/=(1+DELTAS); //9
   float dxy=sqrt((MAXXRE-MINXRE)*(MAXXRE-MINXRE)+(MAXXIM-MINXIM)*(MAXXIM-MINXIM));
   /*if (KeyPressed(72)) {MINXIM+=DELTAS; MAXXIM+=DELTAS;}
   if (KeyPressed(80)) {MINXIM-=DELTAS; MAXXIM-=DELTAS;}
   if (KeyPressed(75)) {MINXRE+=DELTAS; MAXXRE+=DELTAS;}
   if (KeyPressed(77)) {MINXRE-=DELTAS; MAXXRE-=DELTAS;}*/
   if (KeyPressed(EStdKeyUpArrow)) POSY+=DELTAS*dxy;
   if (KeyPressed(EStdKeyDownArrow)) POSY-=DELTAS*dxy;
   if (KeyPressed(EStdKeyLeftArrow)) POSX+=DELTAS*dxy;
   if (KeyPressed(EStdKeyRightArrow)) POSX-=DELTAS*dxy;
  }
  if (DELTAC<0) DELTAC=0;
  if (DELTAC>255) DELTAC=255;
  if (method==0)
  julia(MINXRE+POSX,MINXIM+POSY,
  tabsin[(int)((float)frame*DELTACRE)%360],tabsin[(int)((float)frame*DELTACIM)%360],
  (float)(MAXXRE-MINXRE)/(float)GetScrWidth(),(float)(MAXXIM-MINXIM)/(float)GetScrHeight());
  else
  juliaint(MINXRE+POSX,MINXIM+POSY,
  tabsin[(int)((float)frame*DELTACRE)%360],tabsin[(int)((float)frame*DELTACIM)%360],
  (float)(MAXXRE-MINXRE)/(float)GetScrWidth(),(float)(MAXXIM-MINXIM)/(float)GetScrHeight());
  
  char bf[256];
  lastfps=(int)(1/(GetTime()-t0));
  sprintf(bf,"ZBIOR JULII.\nPROGRAM: KAMIL KOROLKIEWICZ\nMetoda:%s\nFPS=%0.2f\nCMIN=%d\nCMAX=%d\nDELTAC=%d\nSINMUL=%0.2f\nDELTACRE=%0.2f\nDELTACIM=%0.2f\nMINXRE=%0.4f\nMINXIM=%0.4f\nMAXXRE=%0.4f\nMAXXIM=%0.4f",
  methodnames[method],
  1/(GetTime()-t0),
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
    
  if (!stop) frame++;
 return 1;
}

void AppExit()
{
}
