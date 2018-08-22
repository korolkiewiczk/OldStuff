//cwin.c

#include "biosfont.h"
#include "biosfont8x16.h"
#include "font3x5.h"


CWin::CWin(int w,int h,void *b)
{
 width=w;
 height=h;
 buf=(unsigned int*)b;
 fontwidth=8;
 fontheight=8;
}

void CWin::Change(int w,int h,void *b)
{
 width=w;
 height=h;
 buf=(unsigned int*)b;
}

void CWin::Change(void *b)
{
 buf=(unsigned int*)b;
}

void CWin::ChangeFontSize(int w,int h)
{
 fontwidth=w;
 fontheight=h;
}

void CWin::ChangeActiveState(int state)
{
 //nic nie rob
}

CWin::~CWin()
{
}

void CWin::PutPixel(int x,int y,int c)
{
 if ((x<0)||(y<0)||(x>=width)||(y>=height)) return;
 *(unsigned int*)((int)buf+((y*width+x)*sizeof(int)))=c;
}

void CWin::Line(int x1,int y1,int x2,int y2,int col)
{
 //przytnij linie
 if ((((x1<0)&&(x2<0))||((y1<0)&&(y2<0)))||
 (((x1>=width)&&(x2>=width))||((y1>=height)&&(y2>=height)))) return;
 
 if (x1==x2)
 {
  /*if (y1<0) y1=0;
  if (y2<0) y2=0;
  if (y1>=height) y1=height-1;
  if (y2>=height) y2=height-1;*/
  VLine(x1,y1,y2,col);
  return;
 }
 else
 if (y1==y2)
 {
  /*if (x1<0) x1=0;
  if (x2<0) x2=0;
  if (x1>=width) x1=width-1;
  if (x2>=width) x2=width-1;*/
  HLine(x1,y1,x2,col);
  return;
 }
 
 if ((x1<0)||(x2<0)||(y1<0)||(y2<0)||
 (x1>=width)||(x2>=width)||(y1>=height)||(y2>=height))
 { 
  /*float a,b;
  a=(float)(y2-y1)/(float)(x1-x2);
  b=-(float)y1-a*(float)x1;
  int left,right,top,bottom;
  left=(int)round(b);
  right=(int)round(a*(width-1)+b);
  top=(int)round(-b/a);
  bottom=(int)round((-(height-1)-b)/a);*/
  int a,b;
  a=((y2-y1)<<FIXED_SHIFT)/(x1-x2);
  b=-(y1<<FIXED_SHIFT)-a*x1;
  int left,right,top,bottom;
  left=b>>FIXED_SHIFT;
  right=(a*(width-1)+b)>>FIXED_SHIFT;
  if (a==0) a=1;
  top=(-b/a);
  bottom=((-((height-1)<<FIXED_SHIFT)-b)/a);
  int ok=0;
  
  if ((left<=0)&&(left>=-height+1))
  {
   if (x1<0)
   {
    x1=0;
    y1=-left;
   }
   else
   if (x2<0)
   {
    x2=0;
    y2=-left;
   }
   ok=1;
  }
  if ((right<=0)&&(right>=-height+1))
  {
   if (x1>=width)
   {
    x1=width-1;
    y1=-right;
   }
   else
   if (x2>=width)
   {
    x2=width-1;
    y2=-right;
   }
   ok=1;
  }
  if ((top>=0)&&(top<width))
  {
   if (y1<0)
   {
    x1=top;
    y1=0;
   }
   else
   if (y2<0)
   {
    x2=top;
    y2=0;
   }
   ok=1;
  }
  if ((bottom>=0)&&(bottom<width))
  {
   if (y1>=height)
   {
    x1=bottom;
    y1=height-1;
   }
   else
   if (y2>=height)
   {
    x2=bottom;
    y2=height-1;
   }
   ok=1;
  }
  if (ok==0) return;
  if ((x1<0)||(x2<0)||(y1<0)||(y2<0)||
  (x1>=width)||(x2>=width)||(y1>=height)||(y2>=height)) return;
 }
 
 int sx=x2-x1,sy=y2-y1;
 int dx1,dy1,dx2,dy2;
 if (sx>0) dx1=1; else if (sx<0) dx1=-1; else dx1=0;
 if (sy>0) dy1=1; else if (sy<0) dy1=-1; else dy1=0;
 int a=abs(sx),b=abs(sy);
 if (a>b)
 {
  dx2=dx1; dy2=0;
 }
 else
 {
  dx2=0; dy2=dy1;
  int pom=a; a=b; b=pom;
 }
 int s=a/2;
 unsigned int *wsk,dwsk1=dx1+dy1*width,dwsk2=dx2+dy2*width;
 wsk=(unsigned int*)((int)buf+((y1*width+x1)*sizeof(int)));
#ifdef X86
 #include "asm\CWin_Line.c"
#else
 for (int i=0; i<=a; i++)
 {
  *wsk=col;
  s+=b;
  if (s>=a)
  {
   s-=a;
   wsk+=dwsk1;
  }
  else
  {
   wsk+=dwsk2;
  }
 }
#endif
}

void CWin::HLine(int x1,int y,int x2,int col)
{
 if ((y<0)||(y>=height)) return;
 if (x1>x2) {int pom=x1; x1=x2; x2=pom;}
 if (x1<0) x1=0;
 if (x2>=width) x2=width-1;
 if (x2-x1+1<=0) return;
 _memset32(buf+(x1+y*width),col,x2-x1+1);
}

void CWin::VLine(int x,int y1,int y2,int col)
{
 if ((x<0)||(x>=width)) return;
 if (y1>y2) {int pom=y1; y1=y2; y2=pom;}
 if (y1<0) y1=0;
 if (y2>=height) y2=height-1;
 int i=y2-y1+1;
 if (i<=0) return;
 unsigned int *bf;
 bf=(unsigned int*)(x+y1*width+buf);
#ifdef X86
 #include "asm\CWin_VLine.c"
#else
 while (i--)
 {
  *bf=col;
  bf+=width;
 }
#endif
}

void CWin::Bar(int x1,int y1,int x2,int y2,int col)
{
 if (y1>y2) {int pom=y1; y1=y2; y2=pom;}
 if (y1<0) y1=0;
 if (y2>=height) y2=height-1;
 if (x1>x2) {int pom=x1; x1=x2; x2=pom;}
 if (x1<0) x1=0;
 if (x2>=width) x2=width-1;
 if (x2-x1+1<=0) return;
 int i=y2-y1+1;
 if (i<=0) return;
 int d=x2-x1+1;
 unsigned int *b;
 b=(unsigned int*)(x1+y1*width+buf);
 while (i--)
 {
  _memset32(b,col,d);
  b+=width;
 }
}

void CWin::DrawChar8X8(int x,int y,unsigned int *pal,unsigned char c)
{
 if (y<=-8) return;
 if (y>=height) return;
 if (x<=-8) return;
 if (x>=width) return;
 int w=8,h=8,sw=0,sh=0;
 if (x<0) {sw=-x; x=0;}
 if (x>width-8) w-=x-(width-8);
 if (y<0) {sh=-y; y=0;}
 if (y>height-8) h-=y-(height-8);
 int d=w-sw;
 unsigned int *wsk;
 wsk=(unsigned int*)(y*width+x+buf);
 for (int j=sh; j<h; j++)
 {
  unsigned char line=biosfont[c*8+j];
  for (int i=sw; i<w; i++)
  {
   if (((line>>(7-i))&0x1)==1) *wsk=pal[j*8+i];
   wsk++;
  }
  wsk+=width-d;
 }
}

void CWin::DrawChar8X16(int x,int y,unsigned int *pal,unsigned char c)
{
 if (y<=-16) return;
 if (y>=height) return;
 if (x<=-8) return;
 if (x>=width) return;
 int w=8,h=16,sw=0,sh=0;
 if (x<0) {sw=-x; x=0;}
 if (x>width-8) w-=x-(width-8);
 if (y<0) {sh=-y; y=0;}
 if (y>height-16) h-=y-(height-16);
 int d=w-sw;
 unsigned int *wsk;
 wsk=(unsigned int*)(y*width+x+buf);
 for (int j=sh; j<h; j++)
 {
  unsigned char line=biosfont8x16[c*16+j];
  for (int i=sw; i<w; i++)
  {
   if (((line>>(7-i))&0x1)==1) *wsk=pal[j*8+i];
   wsk++;
  }
  wsk+=width-d;
 }
}

unsigned int inline CWin::FadeColor(unsigned int c,unsigned char fade)
{
 /*unsigned int r=(((c>>16)&255)*fade)>>8;
 unsigned int g=(((c>>8)&255)*fade)>>8;
 unsigned int b=((c&255)*fade)>>8;
 return RGB32(r,g,b);*/
 //return RGB32((((c>>16)&255)*fade)>>8,(((c>>8)&255)*fade)>>8,((c&255)*fade)>>8);
 return (((((c>>16)&255)*fade)<<8)&0xff0000)|((((c>>8)&255)*fade)&0xff00)|
 ((((c&255)*fade)>>8)&0xff);
}

void CWin::DrawText(int x,int y,unsigned int color,char *s)
{
 unsigned int *pal;
 int ch;
 if (fontheight<16) ch=8; else ch=16;
 pal=new unsigned int[8*ch];
 int d=((TEXT_FADE2-TEXT_FADE1)<<FIXED_SHIFT)/ch;
 int d2=(TEXT_FADEL<<FIXED_SHIFT)/ch;
 int f=TEXT_FADE1<<FIXED_SHIFT;
 for (int j=0; j<ch; j++)
 {
  int cf=f;
  for (int i=0; i<8; i++)
  {
   pal[j*8+i]=FadeColor(color,cf>>FIXED_SHIFT);
   cf+=d2;
   if (cf>(255<<FIXED_SHIFT)) cf=(255<<FIXED_SHIFT);
  }
  f+=d;
 }
 while (*s)
 {
  unsigned char c=*s;
  /*if (c<=CMax) 
  {
   color=Color[c-1];
   s++;
   continue;
  }*/
  if (fontheight<16)
  DrawChar8X8(x,y,pal,*s);
  else
  DrawChar8X16(x,y,pal,*s);
  s++;
  x+=fontwidth;
 }
 delete pal;
}

void CWin::Clear()
{
 _memset32(buf,BACKGROUNDCOLOR,width*height);
}

void CWin::DrawText3X5(int x,int y,unsigned int color,char *s)
{
 if (y+5>height) return;
 if (y<0) return;
 unsigned int *wsk;
 wsk=(unsigned int*)(y*width+x+buf);
 while (*s)
 {
  if (x>=width-3) return;
  if (x<0) {x+=4; wsk+=4; s++; continue;}
  char c=*s;
  if ((c<=' ')||(c>'}')) {x+=4; wsk+=4; s++; continue;}
  if ((c>='a')&&(c<='z')) c-='a'-'A';
  if ((c>='{')&&(c<='}')) c-='z'-'a'+1;
  c-=' '+1;
  for (int j=0; j<5; j++)
  {
   for (int i=0; i<3; i++)
   {
    if (font3X5[c][j][i]==1) *wsk=color;
    wsk++;
   }
   wsk+=width-3;
  }
  wsk-=width*5-4;
  s++;
  x+=4;
 }
}

void CWin::PrintScreen(int neg)
{
 FILE *f;
 char bufBM[]="BM";
 int s=width*height*3;
 int s1=s+54;
 int zero=0,_54=54,_40=40,_1=1,bpp=24;
 int i,j;
 unsigned int c;
 char fn[256];
 time_t tim;
 time(&tim);
 tm *t=localtime(&tim);
 char *str=asctime(t);
 char *ss=str;
 while (*ss) {if (*ss==':') *ss=' '; if (*ss<=32) *ss=' '; ss++;}
 sprintf(fn,"IMG%s.bmp",str);
 f=fopen(fn,"wb");
 if (!f) return;

 fwrite (&bufBM,2,1,f);                         //2
 fwrite (&s1,4,1,f);                            //6
 fwrite (&zero,4,1,f);                          //10
 fwrite (&_54,4,1,f);                           //14
 fwrite (&_40,4,1,f);                           //18
 fwrite (&width,4,1,f);                         //22
 fwrite (&height,4,1,f);                        //26
 fwrite (&_1,2,1,f);                            //28
 fwrite (&bpp,2,1,f);                           //30
 fwrite (&zero,4,1,f);                          //34
 fwrite (&s,4,1,f);                             //38
 fwrite (&zero,4,1,f);                          //42
 fwrite (&zero,4,1,f);                          //46
 fwrite (&zero,4,1,f);                          //50
 fwrite (&zero,4,1,f);                          //54
 unsigned int *wsk;
 wsk=(unsigned int*)((int)buf+width*(height-1)*sizeof(int));
 for (i=0; i<height; i++)
 {
  for (j=0; j<width; j++)
  {
   c=wsk[j];
   if (neg)
   {
    _MEM(&c,0,unsigned char)=255-_MEM(&c,0,unsigned char);
    _MEM(&c,1,unsigned char)=255-_MEM(&c,1,unsigned char);
    _MEM(&c,2,unsigned char)=255-_MEM(&c,2,unsigned char);
   }
   fwrite(&c,3,1,f);
  }
  wsk-=width;
 }
 fclose(f);
}
