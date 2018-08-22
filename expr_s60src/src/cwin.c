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

void CWin::KeyFunc(int k)
{
 //nic nie rob
}

void CWin::Process()
{
 //nic nie rob
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
 CSys::memset32(buf+(x1+y*width),col,x2-x1+1);
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
  CSys::memset32(b,col,d);
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
   pal[j*8+i]=CSys::FadeColor(color,cf>>FIXED_SHIFT);
   cf+=d2;
   if (cf>(255<<FIXED_SHIFT)) cf=(255<<FIXED_SHIFT);
  }
  f+=d;
 }
 while (*s)
 {
  unsigned char c=*s;
  if (c<=CSys::CMax) 
  {
   color=CSys::Color[c-1];
   s++;
   continue;
  }
  if (fontheight<16)
  DrawChar8X8(x,y,pal,*s);
  else
  DrawChar8X16(x,y,pal,*s);
  s++;
  x+=fontwidth;
 }
 delete pal;
}

void CWin::DrawTextHighlighted(int x,int y,unsigned int color,int fade,char *s)
{
 unsigned int *pal[CSys::CMax+1];
 int ch;
 if (fontheight<16) ch=8; else ch=16;
 for (int k=0; k<=CSys::CMax; k++)
 {
  pal[k]=new unsigned int[8*ch];
  int d=((TEXT_FADE2-TEXT_FADE1)<<FIXED_SHIFT)/ch;
  int d2=(TEXT_FADEL<<FIXED_SHIFT)/ch;
  int f=TEXT_FADE1<<FIXED_SHIFT;
  int c;
  if (k<CSys::CMax)
  c=CSys::FadeColor(CSys::Color[k],fade);
  else
  c=CSys::FadeColor(color,fade);
  for (int j=0; j<ch; j++)
  {
   int cf=f;
   for (int i=0; i<8; i++)
   {
    pal[k][j*8+i]=CSys::FadeColor(c,cf>>FIXED_SHIFT);
    cf+=d2;
    if (cf>(255<<FIXED_SHIFT)) cf=(255<<FIXED_SHIFT);
   }
   f+=d;
  }
 }
 int palno=CSys::CMax;
 int fcol=0;
 int num=0;
 int txt=0;
 int bsl=0;
 while (*s)
 {
  unsigned char c=*s;
  if (fcol==0)
  {
   if (c=='\\') bsl=1-bsl;
   else
   if ((c=='"')||(c=='\'')) 
   {
    if (bsl==0)
    txt=1-txt;
    bsl=0;
    palno=CSys::CTxt;
   }
   else
   if (txt==0)
   {
   if (c=='$') num=1;
   else
   if ((c>='0')&&(c<='9')) palno=CSys::CNum;
   else
   if (((c>='A')&&(c<='Z'))||((c>='a')&&(c<='z'))) palno=CSys::CMax;
   else
   {
    palno=CSys::COp;
    num=0;
   }
   if (num) palno=CSys::CNum;
   }
   else bsl=0;
  }
  if (c<=CSys::CMax) 
  {
   palno=c-1;
   s++;
   fcol=1;
   continue;
  }
  if (fontheight<16)
  DrawChar8X8(x,y,pal[palno],c);
  else
  DrawChar8X16(x,y,pal[palno],c);
  s++;
  x+=fontwidth;
 }
 for (int k=CSys::CMax; k>=0; k--)
 {
  delete pal[k];
 }
}

void CWin::Clear()
{
 CSys::memset32(buf,CSys::Color[CSys::CPattern],width*height);
}

void CWin::DrawText3X5(int x,int y,unsigned int color,char *s)
{
 if (y+5>height) return;
 if (y<0) return;
 int cont=0;
 unsigned int *wsk;
 wsk=(unsigned int*)(y*width+x+buf);
 while (*s)
 {
  if (x>=width-4) return;
  if (x<0) {x+=4; wsk+=4; s++; continue;}
  char c=*s;
  if ((c<=' ')||(c>'}')) {x+=4; wsk+=4; s++; continue;}
  if ((c=='&')||(c=='|')) {cont=1-cont; if (cont==0) {s++; continue;}}
  else cont=0;
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

//rysowanie cieniowanego trojkata z 32-bitowym buforem z
void CWin::GTriangle_z(int xa,int ya,int xb,int yb,int xc,int yc,
                     unsigned int ca,unsigned int cb,unsigned int cc,
                     unsigned int *pal,unsigned int z,unsigned int *zbuf)
{
 const int left=0,top=0,right=width-1,bottom=height-1;
 if (((xa<left)&&(xb<left)&&(xc<left))||((ya<top)&&(yb<top)&&(yc<top))||
     ((xa>right)&&(xb>right)&&(xc>right))||((ya>bottom)&&(yb>bottom)&&(yc>bottom)))
     return;
 
 //y1<=y2<=y3
 int x1,y1,x2,y2,x3,y3,c1,c2,c3;
 //c1=c2=c3=ca;
 
 int part=0; //czesc rysowania, domyslnie obie
 int disp1=0,disp2=0; //przesuniecie y
 int _y1,_y2,_y3;     //przyciete y1,y2,y3
 
 
 //sortuj wedlug rosnacych wsp. y
 if (ya<=yb)
 {
  if (yb<=yc) {x1=xa; y1=ya; x2=xb; y2=yb; x3=xc; y3=yc;
               c1=ca;        c2=cb;        c3=cc;} //ya<=yb<=yc
  else
  if (ya<=yc) {x1=xa; y1=ya; x2=xc; y2=yc; x3=xb; y3=yb;
               c1=ca;        c2=cc;        c3=cb;} //ya<=yc<yb
  else 
              {x1=xc; y1=yc; x2=xa; y2=ya; x3=xb; y3=yb;
               c1=cc;        c2=ca;        c3=cb;} //yc<ya<=yb
 }
 else
 //yb<ya
 {
  if (ya<=yc) {x1=xb; y1=yb; x2=xa; y2=ya; x3=xc; y3=yc;
               c1=cb;        c2=ca;        c3=cc;} //yb<ya<=yc
  else
  if (yb<=yc) {x1=xb; y1=yb; x2=xc; y2=yc; x3=xa; y3=ya;
               c1=cb;        c2=cc;        c3=ca;} //yb<=yc<ya
  else 
              {x1=xc; y1=yc; x2=xb; y2=yb; x3=xa; y3=ya;
               c1=cc;        c2=cb;        c3=ca;} //yc<yb<ya
 }
 
 if (y1==y2)
 {
  if (x1>x2)
  {
   int pom=x2;
   x2=x1;
   x1=pom;
   pom=c2;
   c2=c1;
   c1=pom;
  }
  part=2;
 }
 
 if (y2==y3)
 {
  if (x2>x3)
  {
   int pom=x3;
   x3=x2;
   x2=pom;
   pom=c3;
   c3=c2;
   c2=pom;
  }
  part|=1;
 }
 
 if (part==3) return; //zerowa wysokosc (y1==y2==y3)
 
 if ((y1<top)&&(y2>=top))
 {
  disp1=top-y1;
  _y1=top;
  _y2=y2;
 }
 else
 if ((y1<top)&&(y2<top))
 {
  part=2;
  disp1=top-y1;
  disp2=top-y2;
  _y1=top;
  _y2=top;
 }
 else
 {
  _y1=y1;
  _y2=y2;
 }

 //dol
 if ((y1<=bottom)&&(y2>bottom))
 {
  part=1;
  _y2=bottom;
  _y3=y3;
 }
 else
 if ((y1<=bottom)&&(y2<=bottom)&&(y3>bottom))
 {
  _y3=bottom;
 }
 else
 {
  _y3=y3;
 }
 
 //rysowanie
 int i,y;
 int d12,d13,d23,xA,xB;
 int dc12,dc13,dc23,cA,cB;
 unsigned int *wsk,*zwsk;
 
 if (part==2) goto part2;
 //1 czesc - od y1 do y2
 
 d12=((x2-x1)<<FIXED_SHIFT)/(y2-y1);
 d13=((x3-x1)<<FIXED_SHIFT)/(y3-y1);
 dc12=((c2-c1)<<FIXED_SHIFT)/(y2-y1);
 dc13=((c3-c1)<<FIXED_SHIFT)/(y3-y1);
 if (d12>d13) 
 {
  int pom=d12; 
  d12=d13; 
  d13=pom;
  pom=dc12; 
  dc12=dc13; 
  dc13=pom;
 }
 xA=(x1<<FIXED_SHIFT)+disp1*d12;
 xB=(x1<<FIXED_SHIFT)+disp1*d13;
 cA=(c1<<FIXED_SHIFT)+disp1*dc12;
 cB=(c1<<FIXED_SHIFT)+disp1*dc13;
 y=_y1;
 while(y<=_y2)
 {
  int _xA=(xA>>FIXED_SHIFT);
  int _xB=(xB>>FIXED_SHIFT);
  int disp;
  if (_xA>right) goto nodrawloop1;
  if (_xB<left)  goto nodrawloop1;
  disp=0;
  if (_xA<left)  {disp=left-_xA; _xA=left;}
  if (_xB>right) _xB=right;
  i=_xB-_xA+1;
  wsk=(unsigned int*)((int)buf+((_xA+y*width)*sizeof(int)));
  zwsk=(unsigned int*)((int)zbuf+((_xA+y*width)*sizeof(int)));
  
  int dc=((cB-cA)<<FIXED_SHIFT)/(xB-xA+(1<<FIXED_SHIFT));
  int c=cA+disp*dc;

#ifdef X861
 #include "asm\CWin_GTriangle_z.c"
#else
  while (i--)
  {
   if (z<*zwsk)
   {
    *wsk=pal[c>>FIXED_SHIFT];
    *zwsk=z;
   }
   wsk++;
   zwsk++;
   c+=dc;
  }
#endif
nodrawloop1:
  xA+=d12;
  xB+=d13;
  cA+=dc12;
  cB+=dc13;
  y++;
 }
 
 if (part==1) goto part_end;
part2:
 //2 czesc od y2 do y3
 
 d23=((x3-x2)<<FIXED_SHIFT)/(y3-y2);
 d13=((x3-x1)<<FIXED_SHIFT)/(y3-y1);
 dc23=((c3-c2)<<FIXED_SHIFT)/(y3-y2);
 dc13=((c3-c1)<<FIXED_SHIFT)/(y3-y1);
 xA=(x2<<FIXED_SHIFT)+disp2*d23;
 xB=(x1<<FIXED_SHIFT)+(_y2-y1)*d13;
 cA=(c2<<FIXED_SHIFT)+disp2*dc23;
 cB=(c1<<FIXED_SHIFT)+(_y2-y1)*dc13;
 if (xA>xB)
 {
  int pom=d23;
  d23=d13; 
  d13=pom;
  pom=xB;
  xB=xA;
  xA=pom;
  pom=dc23; 
  dc23=dc13; 
  dc13=pom;
  pom=cB;
  cB=cA;
  cA=pom;
 }
 y=_y2;
 while(y<=_y3)
 {
  if (xB<xA) xB=xA;
  int _xA=(xA>>FIXED_SHIFT);
  int _xB=(xB>>FIXED_SHIFT);
  int disp;
  if (_xA>right) goto nodrawloop2;
  if (_xB<left)  goto nodrawloop2;
  disp=0;
  if (_xA<left)  {disp=left-_xA; _xA=left;}
  if (_xB>right) _xB=right;
  i=_xB-_xA+1;
  wsk=(unsigned int*)((int)buf+((_xA+y*width)*sizeof(int)));
  zwsk=(unsigned int*)((int)zbuf+((_xA+y*width)*sizeof(int)));
  
  int dc=((cB-cA)<<FIXED_SHIFT)/(xB-xA+(1<<FIXED_SHIFT));
  int c=cA+disp*dc;
  
#ifdef X861
 #include "asm\CWin_GTriangle_z.c"
#else
  while (i--)
  {
   if (z<*zwsk)
   {
    *wsk=pal[c>>FIXED_SHIFT];
    *zwsk=z;
   }
   wsk++;
   zwsk++;
   c+=dc;
  }
#endif
nodrawloop2:
  xA+=d23;
  xB+=d13;
  cA+=dc23;
  cB+=dc13;
  y++;
 }
part_end:;
}

//rysowanie teksturowanego trojkata z 32-bitowym buforem z
void CWin::TTriangle_z(int xa,int ya,int xb,int yb,int xc,int yc,
                     float fua,float fva,float fub,float fvb,float fuc,float fvc,
                     CTex *tex,
                     unsigned int ca,unsigned int cb,unsigned int cc,
                     unsigned int z,unsigned int *zbuf)
{
 const int left=0,top=0,right=width-1,bottom=height-1;
 if (((xa<left)&&(xb<left)&&(xc<left))||((ya<top)&&(yb<top)&&(yc<top))||
     ((xa>right)&&(xb>right)&&(xc>right))||((ya>bottom)&&(yb>bottom)&&(yc>bottom)))
     return;
     
 //y1<=y2<=y3
 int x1,y1,x2,y2,x3,y3,c1,c2,c3;
 //c1=c2=c3=ca;
 float fu1,fv1,fu2,fv2,fu3,fv3;
 
 int part=0; //czesc rysowania, domyslnie obie
 int disp1=0,disp2=0; //przesuniecie y
 int _y1,_y2,_y3;     //przyciete y1,y2,y3
 
 //sortuj wedlug rosnacych wsp. y
 if (ya<=yb)
 {
  if (yb<=yc) {x1=xa; y1=ya; x2=xb; y2=yb; x3=xc; y3=yc;
               c1=ca;        c2=cb;        c3=cc;
               fu1=fua; fv1=fva; fu2=fub; fv2=fvb; fu3=fuc; fv3=fvc;} //ya<=yb<=yc
  else
  if (ya<=yc) {x1=xa; y1=ya; x2=xc; y2=yc; x3=xb; y3=yb;
               c1=ca;        c2=cc;        c3=cb;
               fu1=fua; fv1=fva; fu2=fuc; fv2=fvc; fu3=fub; fv3=fvb;} //ya<=yc<yb
  else 
              {x1=xc; y1=yc; x2=xa; y2=ya; x3=xb; y3=yb;
               c1=cc;        c2=ca;        c3=cb;
               fu1=fuc; fv1=fvc; fu2=fua; fv2=fva; fu3=fub; fv3=fvb;} //yc<ya<=yb
 }
 else
 //yb<ya
 {
  if (ya<=yc) {x1=xb; y1=yb; x2=xa; y2=ya; x3=xc; y3=yc;
               c1=cb;        c2=ca;        c3=cc;
               fu1=fub; fv1=fvb; fu2=fua; fv2=fva; fu3=fuc; fv3=fvc;} //yb<ya<=yc
  else
  if (yb<=yc) {x1=xb; y1=yb; x2=xc; y2=yc; x3=xa; y3=ya;
               c1=cb;        c2=cc;        c3=ca;
               fu1=fub; fv1=fvb; fu2=fuc; fv2=fvc; fu3=fua; fv3=fva;} //yb<=yc<ya
  else 
              {x1=xc; y1=yc; x2=xb; y2=yb; x3=xa; y3=ya;
               c1=cc;        c2=cb;        c3=ca;
               fu1=fuc; fv1=fvc; fu2=fub; fv2=fvb; fu3=fua; fv3=fva;} //yc<yb<ya
 }
 
 if (y1==y2)
 {
  if (x1>x2)
  {
   int pom=x2;
   x2=x1;
   x1=pom;
   pom=c2;
   c2=c1;
   c1=pom;
   float fpom;
   fpom=fu2;
   fu2=fu1;
   fu1=fpom;
   fpom=fv2;
   fv2=fv1;
   fv1=fpom;
  }
  part=2;
 }
 
 if (y2==y3)
 {
  if (x2>x3)
  {
   int pom=x3;
   x3=x2;
   x2=pom;
   pom=c3;
   c3=c2;
   c2=pom;
   float fpom;
   fpom=fu3;
   fu3=fu2;
   fu2=fpom;
   fpom=fv3;
   fv3=fv2;
   fv2=fpom;
  }
  part|=1;
 }
 
 if (part==3) return; //zerowa wysokosc (y1==y2==y3)
 
 if ((y1<top)&&(y2>=top))
 {
  disp1=top-y1;
  _y1=top;
  _y2=y2;
 }
 else
 if ((y1<top)&&(y2<top))
 {
  part=2;
  disp1=top-y1;
  disp2=top-y2;
  _y1=top;
  _y2=top;
 }
 else
 {
  _y1=y1;
  _y2=y2;
 }

 //dol
 if ((y1<=bottom)&&(y2>bottom))
 {
  part=1;
  _y2=bottom;
  _y3=y3;
 }
 else
 if ((y1<=bottom)&&(y2<=bottom)&&(y3>bottom))
 {
  _y3=bottom;
 }
 else
 {
  _y3=y3;
 }
 
 //rysowanie
 int i,y;
 int d12,d13,d23,xA,xB;
 int dc12,dc13,dc23,cA,cB;
 int du12,dv12,du13,dv13,du23,dv23,uA,vA,uB,vB;
 unsigned int *wsk,*zwsk;
 
 //okresl poziom mipmapy
 int lev=0;
 float fduy=fabs((fu3-fu1)*(float)tex->GetSize(0)/(float)(y3-y1));
 float fdvy=fabs((fv3-fv1)*(float)tex->GetSize(0)/(float)(y3-y1));
 float fdux,fdvx;
 if ((xa<=xb)&&(xb<=xc)) //xa<xb<xc
 {
  fdux=fabs((fuc-fua)*(float)tex->GetSize(0)/(float)(xc-xa));
  fdvx=fabs((fvc-fva)*(float)tex->GetSize(0)/(float)(xc-xa));
 }
 else
 if ((xa<=xc)&&(xc<=xb)) //xa<=xc<=xb
 {
  fdux=fabs((fub-fua)*(float)tex->GetSize(0)/(float)(xb-xa));
  fdvx=fabs((fvb-fva)*(float)tex->GetSize(0)/(float)(xb-xa));
 }
 else
 if ((xb<=xa)&&(xa<=xc)) //xb<=xa<=xc
 {
  fdux=fabs((fuc-fub)*(float)tex->GetSize(0)/(float)(xc-xb));
  fdvx=fabs((fvc-fvb)*(float)tex->GetSize(0)/(float)(xc-xb));
 }
 else
 if ((xb<=xc)&&(xc<=xa)) //xb<=xc<=xa
 {
  fdux=fabs((fua-fub)*(float)tex->GetSize(0)/(float)(xa-xb));
  fdvx=fabs((fva-fvb)*(float)tex->GetSize(0)/(float)(xa-xb));
 }
 else
 if ((xc<=xa)&&(xa<=xb)) //xc<=xa<=xb
 {
  fdux=fabs((fub-fuc)*(float)tex->GetSize(0)/(float)(xb-xc));
  fdvx=fabs((fvb-fvc)*(float)tex->GetSize(0)/(float)(xb-xc));
 }
 else
 if ((xc<=xb)&&(xb<=xa)) //xc<=xb<=xa
 {
  fdux=fabs((fua-fuc)*(float)tex->GetSize(0)/(float)(xa-xc));
  fdvx=fabs((fva-fvc)*(float)tex->GetSize(0)/(float)(xa-xc));
 }
 
 float fdy=(fduy>fdvy)? fduy:fdvy;
 float fdx=(fdux>fdvx)? fdux:fdvx;
 float fd=(fdy>fdx)? fdy:fdx;
 if (fd>1)
 {
  int l=0;
  while ((fd>1)&&(l<tex->GetMaxLev()))
  {
   fd*=0.5f;
   l++;
  }
  lev=l;
 }
 tex->SetPeekLev(lev);
 
 if (part==2) goto part2;
 //1 czesc - od y1 do y2
  
 d12=((x2-x1)<<FIXED_SHIFT)/(y2-y1);
 d13=((x3-x1)<<FIXED_SHIFT)/(y3-y1);
 dc12=((c2-c1)<<FIXED_SHIFT)/(y2-y1);
 dc13=((c3-c1)<<FIXED_SHIFT)/(y3-y1);
 du12=ifloor((fu2-fu1)*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))/(y2-y1);
 dv12=ifloor((fv2-fv1)*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))/(y2-y1);
 du13=ifloor((fu3-fu1)*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))/(y3-y1);
 dv13=ifloor((fv3-fv1)*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))/(y3-y1);
 if (d12>d13)
 {
  int pom=d12; 
  d12=d13; 
  d13=pom;
  pom=dc12; 
  dc12=dc13; 
  dc13=pom;
  pom=du12; 
  du12=du13; 
  du13=pom;
  pom=dv12; 
  dv12=dv13; 
  dv13=pom;
 }
 xA=(x1<<FIXED_SHIFT)+disp1*d12;
 xB=(x1<<FIXED_SHIFT)+disp1*d13;
 cA=(c1<<FIXED_SHIFT)+disp1*dc12;
 cB=(c1<<FIXED_SHIFT)+disp1*dc13;
 uA=((ifloor(fu1*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))))+disp1*du12;
 vA=((ifloor(fv1*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))))+disp1*dv12;
 uB=((ifloor(fu1*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))))+disp1*du13;
 vB=((ifloor(fv1*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))))+disp1*dv13;
 y=_y1;
 while(y<=_y2)
 {
  int _xA=(xA>>FIXED_SHIFT);
  int _xB=(xB>>FIXED_SHIFT);
  int disp;
  if (_xA>right) goto nodrawloop1;
  if (_xB<left)  goto nodrawloop1;
  disp=0;
  if (_xA<left)  {disp=left-_xA; _xA=left;}
  if (_xB>right) _xB=right;
  i=_xB-_xA+1;
  wsk=(unsigned int*)((int)buf+((_xA+y*width)*sizeof(int)));
  zwsk=(unsigned int*)((int)zbuf+((_xA+y*width)*sizeof(int)));
  
  int du=((uB-uA)<<FIXED_SHIFT)/(xB-xA+(1<<FIXED_SHIFT));
  int dv=((vB-vA)<<FIXED_SHIFT)/(xB-xA+(1<<FIXED_SHIFT));
  int u=uA+disp*du;
  int v=vA+disp*dv;
  int dc=((cB-cA)<<FIXED_SHIFT)/(xB-xA+(1<<FIXED_SHIFT));
  int c=cA+disp*dc;
#ifdef X861
 #include "asm\CWin_TTriangle_z.c"
#else
  while (i--)
  {
   if (z<*zwsk)
   {
    *wsk=CSys::FadeColor(tex->Peek(u>>FIXED_SHIFT,v>>FIXED_SHIFT),c>>FIXED_SHIFT);
    *zwsk=z;
   }
   wsk++;
   zwsk++;
   u+=du;
   v+=dv;
   c+=dc;
  }
#endif
nodrawloop1:
  xA+=d12;
  xB+=d13;
  cA+=dc12;
  cB+=dc13;
  uA+=du12;
  vA+=dv12;
  uB+=du13;
  vB+=dv13;
  y++;
 }
 
 if (part==1) goto part_end;
part2:
 //2 czesc od y2 do y3
 
 d23=((x3-x2)<<FIXED_SHIFT)/(y3-y2);
 d13=((x3-x1)<<FIXED_SHIFT)/(y3-y1);
 dc23=((c3-c2)<<FIXED_SHIFT)/(y3-y2);
 dc13=((c3-c1)<<FIXED_SHIFT)/(y3-y1);
 du23=ifloor((fu3-fu2)*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))/(y3-y2);
 dv23=ifloor((fv3-fv2)*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))/(y3-y2);
 du13=ifloor((fu3-fu1)*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))/(y3-y1);
 dv13=ifloor((fv3-fv1)*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))/(y3-y1);
 xA=(x2<<FIXED_SHIFT)+disp2*d23;
 xB=(x1<<FIXED_SHIFT)+(_y2-y1)*d13;
 cA=(c2<<FIXED_SHIFT)+disp2*dc23;
 cB=(c1<<FIXED_SHIFT)+(_y2-y1)*dc13;
 uA=((ifloor(fu2*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))))+disp2*du23;
 vA=((ifloor(fv2*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))))+disp2*dv23;
 uB=((ifloor(fu1*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))))+(_y2-y1)*du13;
 vB=((ifloor(fv1*(float)tex->GetSize(lev)*(1<<FIXED_SHIFT))))+(_y2-y1)*dv13;
 if (xA>xB)
 {
  int pom=d23;
  d23=d13; 
  d13=pom;
  pom=xB;
  xB=xA;
  xA=pom;
  pom=dc23; 
  dc23=dc13; 
  dc13=pom;
  pom=cB;
  cB=cA;
  cA=pom;
  pom=du23; 
  du23=du13; 
  du13=pom;
  pom=dv23; 
  dv23=dv13; 
  dv13=pom;
  pom=uB;
  uB=uA;
  uA=pom;
  pom=vB;
  vB=vA;
  vA=pom;
 }
 y=_y2;
 while(y<=_y3)
 {
  if (xB<xA) xB=xA;
  int _xA=(xA>>FIXED_SHIFT);
  int _xB=(xB>>FIXED_SHIFT);
  int disp;
  if (_xA>right) goto nodrawloop2;
  if (_xB<left)  goto nodrawloop2;
  disp=0;
  if (_xA<left)  {disp=left-_xA; _xA=left;}
  if (_xB>right) _xB=right;
  i=_xB-_xA+1;
  wsk=(unsigned int*)((int)buf+((_xA+y*width)*sizeof(int)));
  zwsk=(unsigned int*)((int)zbuf+((_xA+y*width)*sizeof(int)));
  
  int du=((uB-uA)<<FIXED_SHIFT)/(xB-xA+(1<<FIXED_SHIFT));
  int dv=((vB-vA)<<FIXED_SHIFT)/(xB-xA+(1<<FIXED_SHIFT));
  int u=uA+disp*du;
  int v=vA+disp*dv;
  int dc=((cB-cA)<<FIXED_SHIFT)/(xB-xA+(1<<FIXED_SHIFT));
  int c=cA+disp*dc;
#ifdef X862
 #include "asm\CWin_TTriangle_z.c"
#else
  while (i--)
  {
   if (z<*zwsk)
   {
    *wsk=CSys::FadeColor(tex->Peek(u>>FIXED_SHIFT,v>>FIXED_SHIFT),c>>FIXED_SHIFT);
    *zwsk=z;
   }
   wsk++;
   zwsk++;
   u+=du;
   v+=dv;
   c+=dc;
  }
#endif
nodrawloop2:
  xA+=d23;
  xB+=d13;
  cA+=dc23;
  cB+=dc13;
  uA+=du23;
  vA+=dv23;
  uB+=du13;
  vB+=dv13;
  y++;
 }
part_end:;
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
 sprintf(fn,"IMG%0.0f.bmp",CSys::GetTime()*1000);
 f=CSys::FOpen(fn,"wb");
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
