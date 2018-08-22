#include <windows.h>
#include <DX8/d3d8.h>
#include <math.h>
#include <stdio.h>
#include "wingraph.h"
#include "text.cpp"
#include "debug.c"

#define MAXCARS 10
#define METERINPIX 10.f
#define CELLINPIX 5
#define ADELTA 90.f
#define ARDELTA 1000.f
#define GCOLOR RGB32(100,100,100)
#define GTCOLOR RGB32(240,240,240)
#define GACOLOR RGB32(200,0,0)
#define GRN 4 //ilosc kresek miedzy wartosciami
#define GSDELTA 20 //odstepy miedzy kolejnymi predkosciami
#define TIRESSIZE 0.6f //srednica kol
#define TIRESCOLOR RGB32(70,70,70)
#define MAXBMPS 16
#define TARMACCOL RGB32(85,85,100)
#define TARMACRND 20
#define TARMACBREAK 1.
#define GRAVELCOL RGB32(120,120,110)
#define GRAVELRND 35
#define GRAVELBREAK 1.5
#define GRASSCOL RGB32(0,200,0)
#define GRASSRND 30
#define GRASSBREAK 3.
#define SANDCOL RGB32(240,240,0)
#define SANDRND 10
#define SANDBREAK 20.

int SCRWIDTH=640;
int SCRHEIGHT=480;
#define BPP 32
#define VSYNC !WINGRAPH_VSYNC
#include "bmp.cpp"

float frand()
{
 return ((float)rand())/32768.0;
}


float interval,fps;

//mapa

typedef struct _MAP
{
 int w,h;
 unsigned char *b;
}MAP;

MAP map;
#include "map_enum.h"
unsigned int surfvals[ESURFMAX][2]=
{
 {TARMACCOL,TARMACRND},
 {GRAVELCOL,GRAVELRND},
 {GRASSCOL,GRASSRND},
 {SANDCOL,SANDRND}
};

BMP surfbmps[ESURFMAX][MAXBMPS];
unsigned char *bmpmask[MAXBMPS];

unsigned int v2col[ESURFMAX]={TARMACCOL,GRAVELCOL,GRASSCOL,SANDCOL};

float surfbreaks[ESURFMAX]=
{
 TARMACBREAK,GRAVELBREAK,GRASSBREAK,SANDBREAK
};

//SAMOCHOD

typedef struct _VEC
{
 int npoints;
 float points[16][2];
} VEC;

typedef struct _CAR
{
 char carname[256];
 BMP carbmp;
 VEC acctab;
 VEC gears;
 float maxwhellangle;
 float breakacc; //przyspieszenie hamowania
 float aresist; //wsp oporu powietrza
 float gresist; //wsp oporu toczenia
 float eresist; //wsp oporu silnika przy puszczonym gazie
 float maxrpm;
 float minrpm;
 float maxg;    //maksymalne przysp. dosrodkowe
} CAR;

float carposx,carposy,carangle,carwhellangle;
int scrcarposx,scrcarposy;
float carthrottle; //0..1
float carspeed; //w km/h
float carrpm;
float cardist;  //w km
int cargear;
CAR cars[MAXCARS];
int ncars;
int currcar=0;
int carsuspendthrottle;

int negmod(int x,int a)
{
 return x+a*((-x)/a+1);
}

unsigned int rcol(int col,int rnd)
{
 int r,g,b;
 r=(col>>16)&255;
 g=(col>>8)&255;
 b=col&255;
 int rd=rand()%rnd;
 r+=rd-rnd/2;
 g+=rd-rnd/2;
 b+=rd-rnd/2;
 if (r<0) r=0;
 if (r>255) r=255;
 if (g<0) g=0;
 if (g>255) g=255;
 if (b<0) b=0;
 if (b>255) b=255;
 return (r<<16)+(g<<8)+b;
}

inline void SetBufAW(unsigned int *bf,unsigned int c,unsigned char a)
{
 unsigned int r=((c>>16)&255)*a;
 unsigned int g=((c>>8)&255)*a;
 unsigned int b=((c)&255)*a;
 //unsigned int *bf=(unsigned int*)((int)buf+(y*SCRWIDTH+x)*4);
 unsigned int c0=*bf;
 *bf=(((((c0>>16)&255)*(256-a)+r)>>8)<<16)
         +(((((c0>>8)&255)*(256-a)+g)>>8)<<8)
         +((((c0)&255)*(256-a)+b)>>8);
}

void circle(int x,int y,int R,unsigned int c,unsigned char a)
{
 int y0,x1,x2;
 unsigned int r=((c>>16)&255)*a;
 unsigned int g=((c>>8)&255)*a;
 unsigned int b=((c)&255)*a;
 for (y0=-R; y0<=R; y0++)
 {
  x1=(int)-sqrt(R*R-y0*y0)+x;
  x2=(int)sqrt(R*R-y0*y0)+x;
  unsigned int *bf=(unsigned int*)((int)buf+((y0+y)*SCRWIDTH+x1)*4);
  while (x1<=x2)
  {
   unsigned int c0=*bf;
   *bf= (((((c0>>16)&255)*(256-a)+r)>>8)<<16)
         +(((((c0>>8)&255)*(256-a)+g)>>8)<<8)
         +((((c0)&255)*(256-a)+b)>>8);
   bf++;
   x1++;
  }
 }
}

void rectangle(int x1,int y1,int x2,int y2,unsigned int c,unsigned char a)
{
 unsigned int r=((c>>16)&255)*a;
 unsigned int g=((c>>8)&255)*a;
 unsigned int b=((c)&255)*a;
 int x,y;
 unsigned int *bf;
 bf=(unsigned int*)((int)buf+(y1*SCRWIDTH+x1)*4);
 int d=SCRWIDTH-(x2-x1+1);
 for (y=y1; y<=y2; y++)
 {
  for (x=x1; x<=x2; x++)
  {
   unsigned int c0=*bf;
   *bf= (((((c0>>16)&255)*(256-a)+r)>>8)<<16)
         +(((((c0>>8)&255)*(256-a)+g)>>8)<<8)
         +((((c0)&255)*(256-a)+b)>>8);
   bf++;
  }
  bf+=d;
 }
}

void Line(int x1,int y1,int x2,int y2,unsigned int col)
{
 //przytnij linie
 if ((((x1<0)&&(x2<0))||((y1<0)&&(y2<0)))||
 (((x1>=SCRWIDTH)&&(x2>=SCRWIDTH))||((y1>=SCRHEIGHT)&&(y2>=SCRHEIGHT)))) return;
 
 if ((x1<0)||(x2<0)||(y1<0)||(y2<0)||
 (x1>=SCRWIDTH)||(x2>=SCRWIDTH)||(y1>=SCRHEIGHT)||(y2>=SCRHEIGHT))
 { 
  if (x1==x2)
  {
   if (y1<0) y1=0;
   if (y2<0) y2=0;
   if (y1>=SCRHEIGHT) y1=SCRHEIGHT-1;
   if (y2>=SCRHEIGHT) y2=SCRHEIGHT-1;
  }
  else
  if (y1==y2)
  {
   if (x1<0) x1=0;
   if (x2<0) x2=0;
   if (x1>=SCRWIDTH) x1=SCRWIDTH-1;
   if (x2>=SCRWIDTH) x2=SCRWIDTH-1;
  }
  else
  {
   float a,b;
   a=(float)(y2-y1)/(float)(x1-x2);
   b=-(float)y1-a*(float)x1;
   int left,right,top,bottom;
   left=(int)round(b);
   right=(int)round(a*(SCRWIDTH-1)+b);
   top=(int)round(-b/a);
   bottom=(int)round((-(SCRHEIGHT-1)-b)/a);
   int ok=0;
   
   if ((left<=0)&&(left>=-SCRHEIGHT+1))
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
   if ((right<=0)&&(right>=-SCRHEIGHT+1))
   {
    if (x1>=SCRWIDTH)
    {
     x1=SCRWIDTH-1;
     y1=-right;
    }
    else
    if (x2>=SCRWIDTH)
    {
     x2=SCRWIDTH-1;
     y2=-right;
    }
    ok=1;
   }
   if ((top>=0)&&(top<SCRWIDTH))
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
   if ((bottom>=0)&&(bottom<SCRWIDTH))
   {
    if (y1>=SCRHEIGHT)
    {
     x1=bottom;
     y1=SCRHEIGHT-1;
    }
    else
    if (y2>=SCRHEIGHT)
    {
     x2=bottom;
     y2=SCRHEIGHT-1;
    }
    ok=1;
   }
   if (ok==0) return;
  }
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
 int i;
 for (i=0; i<=a; i++)
 {
  SetBuf32(x1,y1,col);
  s+=b;
  if (s>=a)
  {
   s-=a;
   x1+=dx1;
   y1+=dy1;
  }
  else
  {
   x1+=dx2;
   y1+=dy2;
  }
 }
}

void LoadMap(char *mapname)
{
 int i,k;
 FILE *f;
 f=fopen(mapname,"rb");
 if (f==0) exit(0);
 int w,h;
 fread(&w,4,1,f);
 fread(&h,4,1,f);
 map.w=w;
 map.h=h;
 if (map.b==0)
 map.b=(unsigned char *)malloc(w*h*2);
 else
 map.b=(unsigned char *)realloc(map.b,w*h*2);
 for (i=0; i<w*h/2; i++)
 {
  unsigned char v;
  fread(&v,1,1,f);
  map.b[2*i]=v&0xf;
  map.b[2*i+1]=v>>4;
 }
 fclose(f);
 
 unsigned int *wsk;
 for (k=0; k<MAXBMPS; k++)
 {
  if (bmpmask[k]==0) bmpmask[k]=(unsigned char*)malloc(CELLINPIX*CELLINPIX*4);
 }
 for (i=0; i<ESURFMAX; i++)
 {
  for (k=0; k<MAXBMPS; k++)
  {
   if (surfbmps[i][k].b==0) surfbmps[i][k].b=(unsigned int*)malloc(CELLINPIX*CELLINPIX*4);
   surfbmps[i][k].w=CELLINPIX;
   surfbmps[i][k].h=CELLINPIX;
   wsk=surfbmps[i][k].b;
   int j;
   j=CELLINPIX*CELLINPIX;
   while (j--)
   {
    *(wsk++)=rcol(surfvals[i][0],surfvals[i][1]);
   }
  }
 }
  
 //losowosc bitmap i wartosci
 i=map.w*map.h;
 unsigned char *mapwsk;
 mapwsk=map.b;
 while (i--) 
 {
  unsigned char v=*mapwsk;
  *mapwsk|=(rand()%MAXBMPS)<<4;
  mapwsk++;
 }
 
 //generuj maske bitmap
 int j;
 float d0,d1,d2,d3,d4,p[5],E;
 for (j=0; j<CELLINPIX; j++)
 {
  for (i=0; i<CELLINPIX; i++)
  {
   int dx,dy;
   //0
   dx=(i-CELLINPIX/2),dy=(j-CELLINPIX/2);
   d0=1-sqrt(dx*dx+dy*dy)/(float)CELLINPIX;
   //1
   dx=(i-CELLINPIX/2),dy=(j+CELLINPIX/2);
   d1=1-sqrt(dx*dx+dy*dy)/(float)CELLINPIX;
   //2
   dx=(-i+3*CELLINPIX/2),dy=(j-CELLINPIX/2);
   d2=1-sqrt(dx*dx+dy*dy)/(float)CELLINPIX;
   //3
   dx=(i-CELLINPIX/2),dy=(-j+3*CELLINPIX/2);
   d3=1-sqrt(dx*dx+dy*dy)/(float)CELLINPIX;
   //4
   dx=(i+CELLINPIX/2),dy=(j-CELLINPIX/2);
   d4=1-sqrt(dx*dx+dy*dy)/(float)CELLINPIX;
   if (d1<0) d1=0;
   if (d2<0) d2=0;
   if (d3<0) d3=0;
   if (d4<0) d4=0;
   
   E=d0+d1+d2+d3+d4;
   p[0]=d0/E; p[1]=d1/E; p[2]=d2/E; p[3]=d3/E; p[4]=d4/E;
   for (k=0; k<MAXBMPS; k++)
   {
    unsigned char *wskc;
    wskc=(unsigned char*)((int)bmpmask[k]+j*CELLINPIX+i);
    int l=CELLINPIX*CELLINPIX;
    int z;
    float r=frand();
    for (z=0; z<5; z++)
    {
     if ((r<p[z])&&(p[z]>0))
     {
      break;
     }
     r-=p[z];
     if (r<0) r=0;
    }
    *wskc=z;
   }
  }
 }
}

BMP *cell(int x,int y)
{
 unsigned char v;
 unsigned char t;
 if ((x>=0)&&(y>=0)&&(x<map.w)&&(y<map.h))
 {
  v=*(unsigned char*)((int)map.b+(x+y*map.w));
  t=v>>4;
  v&=0xf;
 }
 else
 {
  v=ESand;
  t=abs(x*y+x+y)%MAXBMPS;
 }
 return (BMP*)((int)surfbmps[v]+sizeof(BMP)*t);
}

unsigned int cellv(int x,int y)
{
 if ((x>=0)&&(y>=0)&&(x<map.w)&&(y<map.h))
 {
  return (*(unsigned char*)((int)map.b+(x+y*map.w)));
 }
 else return ((abs(x*y+x+y)%MAXBMPS)<<4)|ESand;
}

void pos2cell(float x,float y,int &mx,int &my)
{
 mx=(int)floor(x*METERINPIX/(float)CELLINPIX)+map.w/2;
 my=-(int)floor(y*METERINPIX/(float)CELLINPIX)+map.h/2;
}

void DrawMap()
{
 float fmx,fmy;
 int mx,my;
 int scrx,scry;
 int i,j;
 fmx=carposx*METERINPIX/(float)CELLINPIX-(float)scrcarposx/CELLINPIX;
 fmy=carposy*METERINPIX/(float)CELLINPIX+(float)scrcarposy/CELLINPIX;
 mx=(int)floor(fmx);
 my=(int)floor(fmy);
 fmx=fmx-mx;
 fmy=fmy-my;
 int mx0;
 mx=mx+map.w/2;
 mx0=mx;
 my=-my+map.h/2;
 int n,m;
 n=SCRWIDTH/CELLINPIX+1;
 m=SCRHEIGHT/CELLINPIX+1;
 scry=(int)((fmy-1)*CELLINPIX);
 for (j=0; j<m; j++)
 {
  mx=mx0;
  scrx=-(int)(fmx*CELLINPIX);
  for (i=0; i<n; i++)
  {
   BMP *b[5];
   b[0]=cell(mx,my);
   b[1]=cell(mx,my-1);
   b[2]=cell(mx+1,my);
   b[3]=cell(mx,my+1);
   b[4]=cell(mx-1,my);
   int t=cellv(mx,my)>>4;
   int k,deltaw,i0,l;
   unsigned int *wsk;
   int x1=scrx,y1=scry,x2=scrx+CELLINPIX,y2=scry+CELLINPIX;
   if (x1<0) x1=0;
   if (y1<0) y1=0;
   if (x2>SCRWIDTH) x2=SCRWIDTH;
   if (y2>SCRHEIGHT) y2=SCRHEIGHT;
   if ((x2<=x1)||(y2<=y1)) goto nodraw;
   unsigned int *wskbuf;
   wskbuf=(unsigned int *)((int)buf+(y1*SCRWIDTH+x1)*4);
   j=y2-y1;
   l=x1-scrx+(y1-scry)*CELLINPIX;
   i0=x2-x1;
   deltaw=SCRWIDTH-i0;
   while (j--)
   {
    k=l;
    i=i0;
    while (i--)
    {
     wsk=b[bmpmask[t][k]]->b+k;
     *(wskbuf++)=*wsk;
     k++;
    }
    wskbuf+=deltaw;
    l+=CELLINPIX;
   }
nodraw:   
   scrx+=CELLINPIX;
   mx++;
  }
  scry+=CELLINPIX;
  my++;
 }
}

double GetTime()
{
 long long f,t;
 QueryPerformanceFrequency((PLARGE_INTEGER)&f);
 QueryPerformanceCounter((PLARGE_INTEGER)&t);
 return (double)t/(double)f;
}

void InitCar()
{
 carposx=0;
 carposy=0;
 scrcarposx=SCRWIDTH/2;
 scrcarposy=SCRHEIGHT/2;
 carwhellangle=0;
 carangle=0;
 carspeed=0;
 carrpm=0;
 carthrottle=1;
 cargear=1;
 cardist=0;
}

int acc=0,turn=0,gear=0;

int HandleCar(CAR *car)
{
 if (gear==1)
 {
  if (((cargear==0)&&(carspeed==0))||((cargear>0)&&(cargear<car->gears.npoints-1))) cargear++;
 }
 else
 if (gear==-1)
 {
  if (((cargear>0)&&(carspeed==0))||(cargear>1)) cargear--;
 }
 carrpm=(carspeed/car->gears.points[cargear][0])*car->maxrpm;
 if (carsuspendthrottle==1)
 {
  if (carrpm<car->maxrpm-100) carsuspendthrottle=0;
 }
 if ((carsuspendthrottle==0)&&(acc==1)) //przyspiesz
 {
  int i;
  for (i=1; i<=car->acctab.npoints; i++)
  {
   if (carrpm<car->acctab.points[i][0]) break;
  }
  i--;
  float p=(carrpm-car->acctab.points[i][0])/(car->acctab.points[i+1][0]-car->acctab.points[i][0]);
  p=p*(car->acctab.points[i+1][1]-car->acctab.points[i][1])+car->acctab.points[i][1];
  p*=car->gears.points[cargear][1];
  carspeed+=p*interval*carthrottle;
  carrpm=(carspeed/car->gears.points[cargear][0])*car->maxrpm;
  if (carrpm>=car->maxrpm) carsuspendthrottle=1;
 }
 if ((carsuspendthrottle==1)||(acc==0)) //bez gazu
 {
  carspeed-=car->eresist*car->gears.points[cargear][1]*interval;
 }
 if (acc==-1) //hamuj
 {
  carspeed-=car->breakacc*interval;
 }
 float gresist;
 int mx,my;
 pos2cell(carposx,carposy,mx,my);
 int v=cellv(mx,my);
 gresist=car->gresist*surfbreaks[v&0xf];
 carspeed-=(car->aresist*carspeed*carspeed+gresist)*interval;
 
 if (turn==1) //w prawo
 {
  carwhellangle+=ADELTA*interval;
 }
 if (((turn==1)&&(carwhellangle<0))||((turn==-1)&&(carwhellangle>0))||(turn==0)) //bez skretu
 {
  if (carwhellangle<0)
  {
   carwhellangle+=ARDELTA*interval;
   if (carwhellangle>0) carwhellangle=0;
  }
  else
  if (carwhellangle>0)
  {
   carwhellangle-=ARDELTA*interval;
   if (carwhellangle<0) carwhellangle=0;
  }
 }
 if (turn==-1) //w lewo
 {
  carwhellangle-=ADELTA*interval;
 }
 float deltaangle=asin(carspeed/3.6*interval*sin(carwhellangle/180*3.1415)/(float)car->carbmp.h*METERINPIX);
 float sgna=(deltaangle>0)? 1:-1;
 float g=carspeed/3.6*fabs(deltaangle)/interval;
 if (g>car->maxg*10) deltaangle=sgna*car->maxg*10*interval/carspeed*3.6;
 
 if (carspeed>0)
 carwhellangle=180.f/3.1415*asin((float)car->carbmp.h/METERINPIX*sin(deltaangle)/carspeed*3.6/interval);
 if (carwhellangle>car->maxwhellangle) carwhellangle=car->maxwhellangle;
 if (carwhellangle<-car->maxwhellangle) carwhellangle=-car->maxwhellangle;

 
 deltaangle*=180.f/3.1415;
 if (cargear>0)
 carangle+=deltaangle;
 else
 carangle-=deltaangle;
 
 carspeed-=(fabs(carwhellangle)/car->maxwhellangle*10*car->gresist)*interval;
 
 if (carspeed<0) carspeed=0;
 
 if (cargear>0)
 {
  carposx+=(carspeed/3.6f*interval)*sin(carangle/180*3.1415);
  carposy+=(carspeed/3.6f*interval)*cos(carangle/180*3.1415);
 }
 else
 {
  carposx-=(carspeed/3.6f*interval)*sin(carangle/180*3.1415);
  carposy-=(carspeed/3.6f*interval)*cos(carangle/180*3.1415);
 }
 carrpm=(carspeed/car->gears.points[cargear][0])*car->maxrpm;
 if (carrpm<car->minrpm) carrpm=car->minrpm;
 cardist+=carspeed/3.6f*interval/1000;
}

void DrawCar(CAR *car)
{
 /*float x[4],y[4],x1,y1,x2,y2;
 float w=car->carbmp.w/2,h=car->carbmp.h/2-1.3*TIRESSIZE*METERINPIX;
 //kola
 x[0]=-((w)*cos(carangle/180*3.1415)-(h)*sin(carangle/180*3.1415))+scrcarposx;
 y[0]=-((w)*sin(carangle/180*3.1415)+(h)*cos(carangle/180*3.1415))+scrcarposy;
 x[1]=((w)*cos(carangle/180*3.1415)+(h)*sin(carangle/180*3.1415))+scrcarposx;
 y[1]=((w)*sin(carangle/180*3.1415)-(h)*cos(carangle/180*3.1415))+scrcarposy; 
 x[2]=((w)*cos(carangle/180*3.1415)-(h)*sin(carangle/180*3.1415))+scrcarposx;
 y[2]=((w)*sin(carangle/180*3.1415)+(h)*cos(carangle/180*3.1415))+scrcarposy;
 x[3]=-((w)*cos(carangle/180*3.1415)+(h)*sin(carangle/180*3.1415))+scrcarposx;
 y[3]=-((w)*sin(carangle/180*3.1415)-(h)*cos(carangle/180*3.1415))+scrcarposy;
 Line((int)x[0],(int)y[0],(int)x[1],(int)y[1],0);
 Line((int)x[2],(int)y[2],(int)x[3],(int)y[3],0);
 
 x1=(TIRESSIZE/2*METERINPIX)*sin((carangle+carwhellangle)/180*3.1415);
 y1=(TIRESSIZE/2*METERINPIX)*cos((carangle+carwhellangle)/180*3.1415);
 x2=(-TIRESSIZE/2*METERINPIX)*sin((carangle+carwhellangle)/180*3.1415);
 y2=(-TIRESSIZE/2*METERINPIX)*cos((carangle+carwhellangle)/180*3.1415);
 //1
 Line((int)(x1+x[0]),(int)(-y1+y[0]),(int)(x2+x[0]),(int)(-y2+y[0]),TIRESCOLOR);
 //2
 Line((int)(x1+x[1]),(int)(-y1+y[1]),(int)(x2+x[1]),(int)(-y2+y[1]),TIRESCOLOR);
 
 x1=(TIRESSIZE/2*METERINPIX)*sin((carangle)/180*3.1415);
 y1=(TIRESSIZE/2*METERINPIX)*cos((carangle)/180*3.1415);
 x2=(-TIRESSIZE/2*METERINPIX)*sin((carangle)/180*3.1415);
 y2=(-TIRESSIZE/2*METERINPIX)*cos((carangle)/180*3.1415);
 //3
 Line((int)(x1+x[2]),(int)(-y1+y[2]),(int)(x2+x[2]),(int)(-y2+y[2]),TIRESCOLOR);
 //4
 Line((int)(x1+x[3]),(int)(-y1+y[3]),(int)(x2+x[3]),(int)(-y2+y[3]),TIRESCOLOR);*/
 //karoseria
 PutBmpTurned(scrcarposx,scrcarposy,carangle,&car->carbmp);
}

void LoadCars()
{
 FILE *f;
 f=fopen("cars.txt","rt");
 if (f==0) exit(0);
 ncars=0;
 int mode=100;
 char *t;
 while (!feof(f))
 {
  char bf[256];
  t=fgets(bf,256,f);
  if (t==0) break;
  
  if (bf[0]=='#') 
  {
   ncars++;
   mode=-1;
  }
  int i,j;
  i=0;
  while (bf[i]>' ') i++;
  bf[i]=0;
  switch (mode)
  {
   case 0: //nazwa samochodu
    strcpy(cars[ncars-1].carname,bf);
    break;
   case 1: //plik bmp
    if (LoadBMP(bf,&cars[ncars-1].carbmp)==0) exit(0);
    break;
   case 2: //wektor przyspeszenia
    cars[ncars-1].acctab.npoints=atoi(bf);
    for (j=1; j<=cars[ncars-1].acctab.npoints; j++)
    {
     t=fgets(bf,256,f);
     i=0;
     while (bf[i]!=' ') i++;
     bf[i]=0;
     cars[ncars-1].acctab.points[j][0]=atof(bf);
     cars[ncars-1].acctab.points[j][1]=atof(bf+i+1);
     //DEBUG_Printf("%f %f\n",cars[ncars-1].acctab.points[j][0],cars[ncars-1].acctab.points[j][1]);
    }
    cars[ncars-1].acctab.points[0][0]=0;
    cars[ncars-1].acctab.points[0][1]=cars[ncars-1].acctab.points[1][1];
    cars[ncars-1].maxrpm=cars[ncars-1].acctab.points[cars[ncars-1].acctab.npoints][0];
    cars[ncars-1].minrpm=cars[ncars-1].acctab.points[1][0];
    break;
   case 3: //wektor biegow
    cars[ncars-1].gears.npoints=atoi(bf)+1;
    for (j=0; j<cars[ncars-1].gears.npoints; j++)
    {
     t=fgets(bf,256,f);
     i=0;
     while (bf[i]!=' ') i++;
     bf[i]=0;
     cars[ncars-1].gears.points[j][0]=atof(bf);
     cars[ncars-1].gears.points[j][1]=atof(bf+i+1);
     //DEBUG_Printf("%f %f\n",cars[ncars-1].gears.points[j][0],cars[ncars-1].gears.points[j][1]);
    }
    break;
   case 4: //maksymalny skret
    cars[ncars-1].maxwhellangle=atof(bf);
    break;
   case 5: //maksymalne hamowanie
    cars[ncars-1].breakacc=atof(bf);
    break;
   case 6: //wsp oporu powietrza
    cars[ncars-1].aresist=atof(bf);
    break;
   case 7: //wsp oporu toczenia
    cars[ncars-1].gresist=atof(bf);
    break;
   case 8: //wsp oporu silnika
    cars[ncars-1].eresist=atof(bf);
    break;
   case 9: //maksymalne przysp. dosrodkowe
    cars[ncars-1].maxg=atof(bf);
    break;
   default:
    break;
  }
  mode++;
 }
}

void DrawGauges(CAR *car)
{
 float R=(SCRHEIGHT+SCRWIDTH)/2/10;
 char bf[16];
 
 //obrotomierz
 int rposx=SCRWIDTH-(int)R-5;
 int rposy=SCRHEIGHT-(int)R-5;
 circle(rposx,rposy,(int)R,GCOLOR,200);
 int nrpm=((int)car->maxrpm/1000+1)*GRN;
 int i;
 float angle,dangle;
 angle=225.f/180.f*3.1415;
 dangle=-270.f/(float)nrpm/180.f*3.1415;
 for (i=0; i<=nrpm; i++)
 {
  int x1,y1,x2,y2,x3,y3;
  if (i%GRN==0)
  {
   x1=(int)(R*cos(angle))+rposx;
   y1=-(int)(R*sin(angle))+rposy;
   x2=(int)(0.9*R*cos(angle))+rposx;
   y2=-(int)(0.9*R*sin(angle))+rposy;
   x3=(int)(0.8*R*cos(angle))+rposx-1;
   y3=-(int)(0.8*R*sin(angle))+rposy-2;
   int cl;
   if (i*1000/GRN>car->maxrpm)
   cl=(GTCOLOR)&0xff0000;
   else
   cl=GTCOLOR;
   Line(x1,y1,x2,y2,cl);
   sprintf(bf,"%d",i/GRN);
   DrawText3X5(x3,y3,cl,bf);
  }
  else
  {
   x1=(int)(R*cos(angle))+rposx;
   y1=-(int)(R*sin(angle))+rposy;
   x2=(int)(0.97*R*cos(angle))+rposx;
   y2=-(int)(0.97*R*sin(angle))+rposy;
   int cl;
   if (i*1000/GRN>car->maxrpm)
   cl=(GTCOLOR)&0xff0000;
   else
   cl=GTCOLOR;
   Line(x1,y1,x2,y2,cl);
  }
  angle+=dangle;
 }
 angle=(225.f/180*3.1415)-270.f/180.f*3.1415*carrpm/(float)(nrpm/GRN)/1000;
 int x1=(int)(0.8*R*cos(angle))+rposx;
 int y1=-(int)(0.8*R*sin(angle))+rposy;
 Line(rposx,rposy,x1,y1,GACOLOR);
 Line(rposx-1,rposy-1,x1,y1,GACOLOR);
 Line(rposx-1,rposy,x1,y1,GACOLOR);
 Line(rposx-1,rposy+1,x1,y1,GACOLOR);
 Line(rposx,rposy-1,x1,y1,GACOLOR);
 Line(rposx,rposy+1,x1,y1,GACOLOR);
 Line(rposx+1,rposy-1,x1,y1,GACOLOR);
 Line(rposx+1,rposy,x1,y1,GACOLOR);
 Line(rposx+1,rposy+1,x1,y1,GACOLOR);
 circle(rposx,rposy,(int)(R/12),GCOLOR,240);
 if (cargear>0)
 sprintf(bf,"%d",cargear);
 else
 strcpy(bf,"R");
 DrawText3X5D(rposx-3,(int)(rposy+0.55*R),GTCOLOR,bf);
 
 //predkosciomierz
 rposx=(int)R+5;
 rposy=SCRHEIGHT-(int)R-5;
 circle(rposx,rposy,(int)R,GCOLOR,200);
 int topgearspeed=(int)car->gears.points[car->gears.npoints-1][0];
 int nsval=(topgearspeed)/GSDELTA*2+2;
 angle=225.f/180.f*3.1415;
 dangle=-270.f/(float)nsval/180.f*3.1415;
 for (i=0; i<=nsval; i++)
 {
  int x1,y1,x2,y2,x3,y3;
  if (i%2==0)
  {
   sprintf(bf,"%d",i*GSDELTA/2);
   int sl=strlen(bf);
   x1=(int)(R*cos(angle))+rposx;
   y1=-(int)(R*sin(angle))+rposy;
   x2=(int)(0.9*R*cos(angle))+rposx;
   y2=-(int)(0.9*R*sin(angle))+rposy;
   x3=(int)(0.8*R*cos(angle))+rposx-sl*2;
   y3=-(int)(0.8*R*sin(angle))+rposy-2;
   Line(x1,y1,x2,y2,GTCOLOR);
   DrawText3X5(x3,y3,GTCOLOR,bf);
  }
  else
  {
   x1=(int)(R*cos(angle))+rposx;
   y1=-(int)(R*sin(angle))+rposy;
   x2=(int)(0.95*R*cos(angle))+rposx;
   y2=-(int)(0.95*R*sin(angle))+rposy;
   Line(x1,y1,x2,y2,GTCOLOR);
  }
  angle+=dangle;
 }
 sprintf(bf,"%d",(int)carspeed);
 int sl=strlen(bf);
 DrawText3X5(rposx-sl*2,(int)(rposy-0.4*R),GTCOLOR,bf);
 sprintf(bf,"%0.2f",cardist);
 sl=strlen(bf);
 DrawText3X5(rposx-sl*2,(int)(rposy+0.7*R),GTCOLOR,bf);
 angle=(225.f/180*3.1415)-270.f/180.f*3.1415*carspeed/(float)(nsval*GSDELTA/2);
 x1=(int)(0.8*R*cos(angle))+rposx;
 y1=-(int)(0.8*R*sin(angle))+rposy;
 Line(rposx,rposy,x1,y1,GACOLOR);
 Line(rposx-1,rposy-1,x1,y1,GACOLOR);
 Line(rposx-1,rposy,x1,y1,GACOLOR);
 Line(rposx-1,rposy+1,x1,y1,GACOLOR);
 Line(rposx,rposy-1,x1,y1,GACOLOR);
 Line(rposx,rposy+1,x1,y1,GACOLOR);
 Line(rposx+1,rposy-1,x1,y1,GACOLOR);
 Line(rposx+1,rposy,x1,y1,GACOLOR);
 Line(rposx+1,rposy+1,x1,y1,GACOLOR);
 circle(rposx,rposy,(int)(R/12),GCOLOR,240);
 
 //mapa
 #define MINIMAPDELTA 8
 #define MINIMAPMUL 6
 int MAPSX=MINIMAPMUL*SCRWIDTH/CELLINPIX/MINIMAPDELTA;
 int MAPSY=MINIMAPMUL*SCRHEIGHT/CELLINPIX/MINIMAPDELTA;
 int xa,ya,xb,yb;
 xa=(SCRWIDTH-MAPSX)/2;
 ya=SCRHEIGHT-5-MAPSY;
 xb=(SCRWIDTH+MAPSX)/2;
 yb=SCRHEIGHT-5;
 //rectangle(xa,ya,xb,yb,GCOLOR,200);
 int mx,my,mx0;
 mx0=(int)floor(carposx*METERINPIX/(float)CELLINPIX-MINIMAPMUL*(float)scrcarposx/CELLINPIX);
 my=(int)floor(carposy*METERINPIX/(float)CELLINPIX+MINIMAPMUL*(float)scrcarposy/CELLINPIX);
 mx0=mx0+map.w/2;
 my=-my+map.h/2;
 unsigned int *wsk=(unsigned int*)((int)buf+(xa+ya*SCRWIDTH)*4);
 for (y1=ya; y1<=yb; y1++)
 {
  mx=mx0;
  for (x1=xa; x1<=xb; x1++)
  {
   SetBufAW(wsk,v2col[cellv(mx,my)&0xf],128);
   wsk++;
   mx+=MINIMAPDELTA;
  }
  wsk+=SCRWIDTH-(xb-xa+1);
  my+=MINIMAPDELTA;
 }
 SetBuf32((xa+xb)/2,(ya+yb)/2,RGB32(255,0,0));
}

void ChangeVidMode(int w,int h)
{
 SCRWIDTH=w;
 SCRHEIGHT=h;
 CloseApp();
 InitWinGraph();
 SetVidMode(SCRWIDTH,SCRHEIGHT,BPP,VSYNC);
 scrcarposx=SCRWIDTH/2;
 scrcarposy=SCRHEIGHT/2;
 int i;
 for (i=0; i<0xffffff; i++) ;
}

int gamestate=0;
float t0,t00;

int main()
{
 srand((int)(GetTime()*1000));
 if (!InitWinGraph()) return 0;
 if (!SetVidMode(SCRWIDTH,SCRHEIGHT,BPP,!WINGRAPH_VSYNC)) {CloseApp(); return 0;}
 fps=60;
 interval=1/60.f;
 LoadCars();
 t00=GetTime();
 Cursor(0);
 while (1)
 {
  t0=GetTime();
  if (!WinControl()) break;
    
  if (gamestate==0)
  {
   LoadMap("mapa.map");
   InitCar();
   gamestate=1;
  }
  else
  if (gamestate==1)
  { 
   if (GetKey(1)) break;
   if (GetKey(59)) ChangeVidMode(640,480);
   if (GetKey(60)) ChangeVidMode(320,240);
   if (GetKey(61)) ChangeVidMode(800,600);
   if (GetKey(62)) ChangeVidMode(1024,768);
   if (GetKey(2)) gamestate=(gamestate+1)%2;
  
   acc=0;
   if (KeyPressed(72))acc=1;
   if (KeyPressed(80))acc=-1;

   turn=0;
   if (KeyPressed(75))turn=-1;
   if (KeyPressed(77))turn=1;
   
   gear=0;
   if (GetKey(30)) gear=1; //A
   if (GetKey(44)) gear=-1; //Z
  
   HandleCar(&cars[currcar]);
   DrawMap();
  
   char bf[64];
   sprintf(bf,"%0.4f %0.2f %0.2f %0.2f",interval,fps,carwhellangle,GetTime()-t00);
   DrawText(0,0,0xffffff,bf);
   DrawCar(&cars[currcar]);
   DrawGauges(&cars[currcar]);
   PresentBuf();
   interval=GetTime()-t0;
   fps=1/interval;
  }
 }
 CloseApp();
 return 0;
}
