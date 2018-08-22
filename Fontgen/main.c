#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include "winopengl.h"

#define SCRW 200
#define SCRH 240

CHOOSEFONT cf;
LOGFONT lf;
HFONT hfont=0;

void setfont()
{
 memset(&cf,0,sizeof(cf));
 cf.lStructSize=sizeof(cf);
 cf.Flags=CF_SCREENFONTS;
 cf.lpLogFont=&lf;
 if (!ChooseFont(&cf))
 {
  SelectObject(OpenGL_hdc,GetStockObject(SYSTEM_FONT)); 
  wglUseFontBitmaps(OpenGL_hdc,0,255,1);
 }
 else
 {
  if (hfont) DeleteObject(hfont);
  hfont=CreateFontIndirect(&lf);
  SelectObject(OpenGL_hdc,hfont); 
  wglUseFontBitmaps(OpenGL_hdc,0,255,1); 
 }
}

void resetpos()
{
 glRasterPos2f(-0.8,0);
}

#define BUF(bf,a,b) *(unsigned int*)((int)(bf)+((a)+(b)*SCRW)*4)
#define BUF8W(bf,a,b,w) *(unsigned char*)((int)(bf)+((a)+(b)*(w)))

unsigned char font[256*16];
int mincharwidth=0,mincharheight=0;
int charwidth,charheight,Aheight;

void createchar(unsigned char c,void *b,int align,int h)
{
 unsigned int *bwsk=b;
 int i,j;
 int cx1=SCRW,cy1=SCRH,cx2=0,cy2=0;
 for (i=0; i<SCRH; i++)
 {
  for (j=0; j<SCRW; j++)
  {
   if (*bwsk++)
   {
    if (j<cx1) cx1=j;
    if (i<cy1) cy1=i;
    if (j>cx2) cx2=j;
    if (i>cx2) cy2=i;
   }
  }
 }
 if ((cx1>=SCRW)||(cy1>=SCRH)) return;
 
 charwidth=cx2-cx1+1;
 charheight=cy2-cy1+1;
 
 if (charwidth<mincharwidth)
 {
  int center=(cx1+cx2)/2;
  cx1=center-mincharwidth/2;
  cx2=center+mincharwidth/2;
  charwidth=cx2-cx1+1;
 }
 
 if (charheight<mincharheight)
 {
  if (align==0)
  {
   //int center=(cy1+cy2)/2;
   //cy1=center-mincharheight/2;
   cy2=cy1+Aheight;
   cy1-=mincharheight-Aheight;
   //cy2=center+mincharheight/2;
  }
  else
  if (align==-1)
  {
   cy2+=mincharheight-charheight;
  }
  else
  if (align==2)
  {
   cy1-=mincharheight-charheight;
  }
  else
  {
   int center=(cy1+cy2)/2;
   cy1=center-Aheight/2;
   cy2=center+Aheight/2;
  }
  charheight=cy2-cy1+1;
 }
 
 unsigned char *pombuf,*pwsk;
 pombuf=(unsigned char *)malloc(charwidth*charheight);
 pwsk=pombuf;
 for (i=cy2; i>=cy1; i--)
 {
  for (j=cx1; j<=cx2; j++)
  {
   *pwsk=BUF(b,j,i)>0;
   pwsk++;
  }
 }
 unsigned char buf[16][8];
 float dx=(float)charwidth/8.f;
 float dy=(float)charheight/(float)h;
 
 for (i=0; i<h; i++)
 {
  unsigned char cb=0;
  for (j=0; j<8; j++)
  {
   float y=(float)cy1+(float)i*dy;
   float x2=(float)cx1+(float)(j+1)*dx;
   float y2=(float)cy1+(float)(i+1)*dy;
   int cn=0,nc=0;
   while (y<y2)
   {
    float x=(float)cx1+(float)j*dx;
    while (x<x2)
    {
     cn+=BUF8W(pombuf,(int)x-cx1,(int)y-cy1,cx2-cx1+1);
     x+=1;
     nc++;
    }
    y+=1;
   }
   int w=(cn>nc/2);
   if (w) cb|=(1<<(7-j));
  }
  font[c*h+i]=cb;
 }
 
 free((void*)pombuf);
}

unsigned char *buf;
char alignup[]="^\"'`";
char aligncenter[]="=+-*";
char aligndown[]="gjpqy,_";
//abcdefghijklmnopqrstuvwxyz

int main()
{
 
 OpenGL_Init(SCRW,SCRH,32,0);
 buf=(unsigned char *)malloc(SCRW*SCRH*4);
 setfont();
 int fonth;
 if (IDYES==MessageBox(0,"Do you want to generate 8X16 font?",
 "FontGen",MB_YESNO|MB_ICONQUESTION))
 fonth=16;
 else
 fonth=8;
 
 glListBase(1);
 
 char s[256];
 int i;
 for (i=0; i<255; i++) s[i]=i;
 s[255]=0;
 
 resetpos();
 glClear(GL_COLOR_BUFFER_BIT);
 glCallLists(1,GL_UNSIGNED_BYTE,&s['A']);
 glReadPixels(0,0,SCRW,SCRH,GL_RGBA,GL_UNSIGNED_BYTE,buf);
 createchar(i,buf,0,fonth);
 mincharwidth=charwidth;
 Aheight=charheight;
 resetpos();
 glClear(GL_COLOR_BUFFER_BIT);
 glCallLists(1,GL_UNSIGNED_BYTE,&s['j']);
 glReadPixels(0,0,SCRW,SCRH,GL_RGBA,GL_UNSIGNED_BYTE,buf);
 createchar(i,buf,0,fonth);
 mincharheight=charheight;
 
 for (i=0; i<255; i++)
 {
  resetpos();
  glClear(GL_COLOR_BUFFER_BIT);
  glCallLists(1,GL_UNSIGNED_BYTE,&s[i]);
  glReadPixels(0,0,SCRW,SCRH,GL_RGBA,GL_UNSIGNED_BYTE,buf);
  OpenGL_Present();
  int j,k=0;
  for (j=0; j<sizeof(alignup)-1; j++) if (i==alignup[j]) k=2;
  if (k==0)
  for (j=0; j<sizeof(aligndown)-1; j++) if (i==aligndown[j]) k=-1;
  if (k==0)
  for (j=0; j<sizeof(aligncenter)-1; j++) if (i==aligncenter[j]) k=1;
  createchar(i,buf,k,fonth);
 }

 OpenGL_Close();
 FILE *f;
 /*f=fopen("f.txt","wt");
 fputs("unsigned char biosfont[256*8]=\n{\n",f);
 for (i=0; i<255; i++)
 {
  int j;
  fputs(" ",f);
  for (j=0; j<8; j++) 
  {fprintf(f,"0x%02X",font8x8[i*8+j]); if (j<7) fputs(",",f);}
  if (i<254)
  fputs(",\n",f);
 }
 fputs("\n};\n",f);*/
 char bf[256];
 if (fonth==8)
 {
  sprintf(bf,"%s8X8.bin",lf.lfFaceName);
  f=fopen(bf,"wb");
  fwrite(font,1,256*8,f);
  fclose(f);
 }
 else
 {
  sprintf(bf,"%s8X16.bin",lf.lfFaceName);
  f=fopen(bf,"wb");
  fwrite(font,1,256*16,f);
  fclose(f);
 }
}
