/*
  Biblioteka wspomagajaca wyswietlanie bitmap
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
  FUNKCJE
*/

typedef struct _BMP
{
 int w,h;
 void *b;
} BMP;

BMP bmp;

#define BMP_POS(x,y,m) ((int)bmp.b+(ifloor(x)+(ifloor(y))*bmp.w)*(m))

float ColorFunc_AV(unsigned char r,unsigned char g,unsigned char b)
{
 return (r+g+b)*(1.f/255.f/3.f);
}

float ColorFunc_R(unsigned char r,unsigned char g,unsigned char b)
{
 return r*(1.f/255.f);
}

float ColorFunc_G(unsigned char r,unsigned char g,unsigned char b)
{
 return g*(1.f/255.f);
}

float ColorFunc_B(unsigned char r,unsigned char g,unsigned char b)
{
 return b*(1.f/255.f);
}

float ColorFunc_OR(unsigned char r,unsigned char g,unsigned char b)
{
 return (r|g|b)*(1.f/255.f);
}

//Podstawowa funkcja ladujaca plik bmp. Drugi parametr okresla funkcje przetwarzajaca punkt
int CommonLoadBMP(char *fn,float (*cf)(unsigned char,unsigned char,unsigned char))
{
 if (*expr_env.MultiExec) return 0;
 FILE *f;
 char sign[2];
 unsigned short bpp;
 int imgstart,i,j,s;
 unsigned char BGR[4];
 unsigned int width,height;
 float *wsk;
 
 f=expr_env.CSys_FOpen(fn,"rb");
 if (f==0) return 0;
 fread(&sign,1,2,f);
 if ((sign[0]!='B')&&(sign[1]!='M')) {fclose(f); return 0;}
 fseek(f,0xa,SEEK_SET);
 fread(&imgstart,1,4,f);
 fseek(f,0x1c,SEEK_SET);
 fread(&bpp,1,2,f);
 if (bpp!=24) {fclose(f); return 0;}
 fseek(f,0x12,SEEK_SET);
 fread(&width,1,4,f);
 fread(&height,1,4,f);
 
 if (bmp.b==0)
 bmp.b=(void*)new float [width*height];
 else
 {
  delete (float*)bmp.b;
  bmp.b=(void*)new float [width*height];
 }
 
 fseek(f,imgstart,SEEK_SET);
 wsk=(float*)bmp.b;
 int extrabits=4-((width*3)%4);
 for (i=0; i<height; i++)
 {
  float *pwsk;
  pwsk=wsk;
  for (j=0; j<width; j++)
  {
   fread(&BGR,1,3,f);
   *wsk=cf(BGR[2],BGR[1],BGR[0]);
   wsk++;
  }
  if (extrabits!=4) fread(&BGR,1,extrabits,f);
  wsk=(pwsk+width);
 }
 bmp.w=width;
 bmp.h=height;
 fclose(f);
 return 1;
}

//laduje plik bmp. Standardowe ladowanie z usrednianiem kolorow
int LoadBMP(char *fn)
{
 CommonLoadBMP(fn,&ColorFunc_AV);
}

//funkcje ladujace dany kanal kolorow
int LoadBMP_R(char *fn)
{
 CommonLoadBMP(fn,&ColorFunc_R);
}

int LoadBMP_G(char *fn)
{
 CommonLoadBMP(fn,&ColorFunc_G);
}

int LoadBMP_B(char *fn)
{
 CommonLoadBMP(fn,&ColorFunc_B);
}

int LoadBMP_OR(char *fn)
{
 CommonLoadBMP(fn,&ColorFunc_OR);
}

float PeekBMP(float x,float y)
{
 if ((x<0)||(y<0)||(x>=bmp.w)||(y>=bmp.h)) return 0;
 return *(float*)BMP_POS(x,y,sizeof(float));
}

float PeekBMP_Smooth(float x,float y)
{
 if ((x<0.5f)||(y<0.5f)||(x>=bmp.w-0.5f)||(y>=bmp.h-0.5f)) return 0;
 x-=.5f;
 y-=.5f;
 float *d=(float*)BMP_POS(x,y,sizeof(float));
 float x1=x-floor(x),y1=y-floor(y);
 return (*d*(1-x1)+*(d+1)*x1)*(1-y1)+(*(d+bmp.w+1)*x1+*(d+bmp.w)*(1-x1))*y1;
}

/*float PeekBMP_Smooth_Rotate(float x,float y,float a,float w,float h)
{
 float xr,yr;
 xr=x*cos(a)-y*sin(a)+w;
 yr=x*sin(a)+y*cos(a)+h;
 if ((xr<0.5f)||(yr<0.5f)||(xr>=bmp.w-0.5f)||(yr>=bmp.h-0.5f)) return 0;
 xr-=.5f;
 yr-=.5f;
 float *d=(float*)BMP_POS(xr,yr,sizeof(float));
 float x1=1-xr+floor(xr),y1=1-y+floor(yr);
 return (*d*x1+*(d+1)*(1-x1))*y1+(*(d+bmp.w+1)*(1-x1)+*(d+bmp.w)*x1)*(1-y1);
}*/

/*
  BIBLIOTEKA FUNKCJI
*/

ELEMENT bmp_lib[]=
{
 {"BMP_LOAD",(void*)LoadBMP,VAL_INT,1,VAL_STR,0},
 {"BMP_LOAD_R",(void*)LoadBMP_R,VAL_INT,1,VAL_STR,0},
 {"BMP_LOAD_G",(void*)LoadBMP_G,VAL_INT,1,VAL_STR,0},
 {"BMP_LOAD_B",(void*)LoadBMP_B,VAL_INT,1,VAL_STR,0},
 {"BMP_LOAD_OR",(void*)LoadBMP_OR,VAL_INT,1,VAL_STR,0},
 {"BMP_PEEK",(void*)PeekBMP,VAL_FLOAT,2,0,0},
 {"BMP_PEEK_SMOOTH",(void*)PeekBMP_Smooth,VAL_FLOAT,2,0,0},
 //{"PEEKBMP_SMOOTH_ROTATE",(void*)PeekBMP_Smooth_Rotate,VAL_FLOAT,5,0,0},
 {"BMP",&bmp.b,VAL_PTR,VAR,0,0},
 {"BMP_WIDTH",&bmp.w,VAL_INT,VAR,0,0},
 {"BMP_HEIGHT",&bmp.h,VAL_INT,VAR,0,0}
};
