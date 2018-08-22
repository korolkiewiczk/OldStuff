#include <stdio.h>
#include <string.h>
#include "map_enum.h"
#define RGB32(r,g,b) (unsigned int)(((r)&255)<<16)|(((g)&255)<<8)|((b)&255)
#define MAPTARMACCOLOR (RGB32(0,0,0))
#define MAPGRAVELCOLOR (RGB32(128,128,128))
#define MAPGRASSCOLOR (RGB32(0,255,0))
#define MAPSANDCOLOR (RGB32(255,255,0))

typedef struct _BMP
{
 int w,h;
 unsigned int *b;
} BMP;

int LoadBMP(char *fn,BMP *b)
{
 FILE *f;
 char sign[2];
 unsigned short bpp;
 int imgstart,i,j,s;
 unsigned int BGR=0;
 unsigned int width,height;
 unsigned int *wsk;
 
 f=fopen (fn,"rb");
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
 
 if (b->b==0)
 b->b=(unsigned int *)malloc(width*height*4);
 else
 b->b=(unsigned int *)realloc(b->b,width*height*4);
 
 fseek(f,imgstart,SEEK_SET);
 wsk=(unsigned int *)((int)b->b+(width*(height-1))*4);
 int extrabits=4-((width*3)%4);
 for (i=0; i<height; i++)
 {
  unsigned int *pwsk;
  pwsk=wsk;
  for (j=0; j<width; j++)
  {
   fread(&BGR,1,3,f);
   *wsk=BGR;
   wsk++;
  }
  if (extrabits!=4) fread(&BGR,1,extrabits,f);
  wsk=(pwsk-width);
 }
 b->w=width;
 b->h=height;
 fclose(f);
 return 1;
}

BMP map;
unsigned char *bf;

int main()
{
 char fn[256];
 scanf("%s",fn);
 if (LoadBMP(fn,&map)==0) return 0;
 int w=map.w*map.h;
 bf=(unsigned char *)malloc(w);
 int i=0,l=0;
 int carposx=map.w/2,carposy=map.h/2,carangle=0;
 while (1)
 {
  int c,v,j,k;
  unsigned char r,b,g;
  k=0;
  if (i>=w) break;
  c=map.b[i];
  r=(c>>16)&255;
  g=(c>>8)&255;
  b=c&255;
  //specjalne wartosci r:>128, g==0, b-parametr
  if ((r>128)&&(g==0))
  {
   //pozycja samochodu
   if (r==255)
   {
    carposx=i%map.w;
    carposy=i/map.w;
    carangle=b*2;
   }
   bf[l++]=v*16;
   i++;
   continue;
  }
  if (c==MAPTARMACCOLOR) v=ETarmac;
  else
  if (c==MAPGRAVELCOLOR) v=EGravel;
  else
  if (c==MAPGRASSCOLOR) v=EGrass;
  else
  if (c==MAPSANDCOLOR) v=ESand;
  //else
  //printf("%d %d %d\n",r,g,b);
  while ((i<w)&&(k<15)&&(map.b[++i]==c)) k++;
  bf[l++]=v*16|k;
  //printf("%d %d %X\n",k+1,v,bf[l-1]);
 }
 i=strlen(fn)-1;
 while ((i>0)&&(fn[i]!='.')) i--;
 fn[i]=0;
 strcat(fn,".map");
 FILE *f;
 f=fopen(fn,"wb");
 fwrite(&carposx,4,1,f);
 fwrite(&carposy,4,1,f);
 fwrite(&carangle,4,1,f);
 fwrite(&map.w,4,1,f);
 fwrite(&map.h,4,1,f);
 fwrite(bf,1,l,f);
 fclose(f);
 return 0;
}
