//tex.cpp

#include <gl/gl.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tex.h"

//laduje obraz z pliku do bufora imgbuf i zwraca jego szerokosc i wysokosc
int LoadImage(char *fn,unsigned int *&imgbuf,int &width,int &height)
{
 FILE *f;
 char sign[2];
 unsigned short bpp;
 int imgstart;
 unsigned int BGR;
 unsigned int *wsk,*pwsk;
 int w,h;
 
 width=0;
 height=0;
 imgbuf=0;
 
 f=fopen(fn,"rb");
 if (f==0) return 0;
 fread(&sign,1,2,f);
 if ((sign[0]!='B')&&(sign[1]!='M')) {fclose(f); return 0;}
 fseek(f,0xa,SEEK_SET);
 fread(&imgstart,1,4,f);
 fseek(f,0x1c,SEEK_SET);
 fread(&bpp,1,2,f);
 fseek(f,0x12,SEEK_SET);
 fread(&w,1,4,f);
 fread(&h,1,4,f);
 
 if (bpp!=24) {fclose(f); return 0;}
 
 width=w;
 height=h;
 unsigned int *bf;
 bf=new unsigned int [width*(height+1)];
 
 fseek(f,imgstart,SEEK_SET);
 wsk=bf+(width*(height-1));
 int extrabits;
 if (bpp==24)
 {
  extrabits=4-((width*3)%4);
  for (int i=0; i<height; i++)
  {
   pwsk=wsk;
   for (int j=0; j<width; j++)
   {
    fread(&BGR,1,3,f);
    *(unsigned char*)((int)&BGR+3)=255;
    *(unsigned char*)((int)wsk)=*(unsigned char*)((int)&BGR+2);
    *(unsigned char*)((int)wsk+1)=*(unsigned char*)((int)&BGR+1);
    *(unsigned char*)((int)wsk+2)=*(unsigned char*)((int)&BGR);
    *(unsigned char*)((int)wsk+3)=*(unsigned char*)((int)&BGR+3);
    wsk++;
   }
   if (extrabits!=4) fread(&BGR,1,extrabits,f);
   wsk=pwsk-width;
  }
 }
 fclose(f);
 
 imgbuf=bf;
 return 1;
}

int CTex::Load(char *fn)
{
 unsigned int *wsk,*pwsk;
 unsigned int *bf; //bufor pomocniczy
 
 int height;
 if (!LoadImage(fn,bf,size,height)) return 0;
 if (size!=height) return 0;

 //znajdz rozmiary tekstury
 int s=1;
 while ((s<size)&&(shift<MAXMIPMAPS))
 {
  s<<=1;
  shift++;
 }
  
 //alokuj pamiec dla bufora i mipmap
 int j=s;
 for (int i=0; i<=shift; i++)
 {
  if (b[i])
  delete [] b[i];
  b[i]=new unsigned int [j*j];
  j>>=1;
 }
 

 memcpy(b[0],bf,size*size*sizeof(unsigned int));
 
 delete [] bf;

 
 //buduj mipmapy
 SetAlpha(0);
 BuildMipMaps();
 
 return 1;
}

void CTex::BuildMipMaps()
{
 unsigned int *wsk,*pwsk;
 int j;
 j=size>>1;
 for (int i=1; i<=shift; i++)
 {
  wsk=b[i];
  for (int y=0; y<j; y++)
  {
   pwsk=b[i-1]+(j<<2)*y;
   for (int x=0; x<j; x++)
   {
    int c1=*pwsk;
    int c2=*(pwsk+1);
    int c3=*(pwsk+(j<<1));
    int c4=*(pwsk+(j<<1)+1);
    int sr=(((c1>>16)&255)+((c2>>16)&255)+((c3>>16)&255)+((c4>>16)&255))/4;
    int sg=(((c1>>8)&255)+((c2>>8)&255)+((c3>>8)&255)+((c4>>8)&255))/4;
    int sb=(((c1)&255)+((c2)&255)+((c3)&255)+((c4)&255))/4;
    int sa=(((c1>>24)&255)+((c2>>24)&255)+((c3>>24)&255)+((c4>>24)&255))/4;
    *wsk=(sa<<24)+(sr<<16)+(sg<<8)+sb;
    wsk++;
    pwsk+=2;
   }
  }
  j>>=1;
 }
}

void CTex::SetAlpha(unsigned int c)
{
 int i,j;
 c|=0xff000000;
 for (i=0; i<=shift; i++)
 {
  unsigned int *wsk=(unsigned int*)b[i];
  for (j=0; j<((size>>i)*(size>>i)); j++)
  {
   if (*wsk==c) *(unsigned char*)((int)wsk+3)=0;
   wsk++;
  }
 }
}

void CTex::Bind()
{
 if (bind==0)
 {
  glGenTextures(1,(GLuint*)&bind);
  glBindTexture(GL_TEXTURE_2D,bind);
  for (int i=0; i<=shift; i++)
  {
   glTexImage2D(GL_TEXTURE_2D,i,GL_RGBA,size>>i,size>>i,0,
   GL_RGBA,GL_UNSIGNED_BYTE,b[i]);
  }
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
 }
 else
 {
  glBindTexture(GL_TEXTURE_2D,bind);
 }
}

CTex::~CTex()
{
 if (b[0]>0)
 {
  for (int i=0; i<=shift; i++)
  {
   delete [] b[i];
  }
 }
}
