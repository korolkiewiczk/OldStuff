typedef struct _BMP
{
 int w,h;
 unsigned int *b;
} BMP;

//laduje plik bmp
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
 b->b=(unsigned int *)malloc(width*height*sizeof(buf));
 else
 b->b=(unsigned int *)realloc(b->b,width*height*sizeof(buf));
 
 fseek(f,imgstart,SEEK_SET);
 wsk=(unsigned int *)((int)b->b+(width*(height-1))*sizeof(buf));
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

#define KCOL 0xffffff

void PutBmp(int x,int y,BMP *b)
{
 int i,j;
 int x1=x,y1=y,x2=x+b->w,y2=y+b->h;
 if (x1<0) x1=0;
 if (y1<0) y1=0;
 if (x2>SCRWIDTH) x2=SCRWIDTH;
 if (y2>SCRHEIGHT) y2=SCRHEIGHT;
 if ((x2<=x1)||(y2<=y1)) return;
 unsigned int *wskbmp,*wskbuf;
 wskbmp=b->b;
 wskbuf=(unsigned int *)((int)buf+(y1*SCRWIDTH+x1)*sizeof(buf));
 wskbmp+=x1-x;
 j=y2-y1;
 while (j--)
 {
  i=x2-x1;
  while (i--)
  {
   if (*wskbmp!=KCOL)
   *wskbuf=*wskbmp;
   wskbmp++;
   wskbuf++;
  }
  wskbmp+=b->w-(x2-x1);
  wskbuf+=SCRWIDTH-(x2-x1);
 }
}

void PutBmpTurned(float x,float y,float angle,BMP *b)
{
 int i,j=b->h*2;
 int ix,iy;
 unsigned int *wskbmp,*wskbuf;
 float x1,y1;
 x1=(b->w/2)*cos(angle/180*3.1415)-(b->h/2)*sin(angle/180*3.1415);
 y1=(b->w/2)*sin(angle/180*3.1415)+(b->h/2)*cos(angle/180*3.1415);
 x-=x1;
 y-=y1;
 wskbmp=b->b;
 float dx,dy;
 dx=cos(angle/180*3.1415)*0.5;
 dy=sin(angle/180*3.1415)*0.5;
 float ffx=x,ffy=y,fx,fy;
 int k=0,l=0;
 while (j--)
 {
  i=b->w*2;
  fx=ffx;
  fy=ffy;
  k=(l/2)*b->w*2;
  while (i--)
  {
   ix=(int)(fx+0.5);
   iy=(int)(fy+0.5);
   if ((ix>=0)&&(ix<SCRWIDTH)&&(iy>=0)&&(iy<SCRHEIGHT))
   {
    wskbuf=(unsigned int *)((int)buf+(iy*SCRWIDTH+ix)*sizeof(buf));
    if (*(unsigned int*)((int)wskbmp+(k/2)*4)!=KCOL)
    *wskbuf=*(unsigned int*)((int)wskbmp+(k/2)*4); 
   }
   k++;
   fx+=dx;
   fy+=dy;
  }
  l++;
  ffx-=dy;
  ffy+=dx;
 }
}

/*void PutBmpTurned(float x,float y,float angle,BMP *b)
{
 float x1,y1,x2,y2,x3,y3,x4,y4;
 x1=(x-b->w)*cos(angle/180*3.1415)-(y-b->h)*sin(angle/180*3.1415);
 y1=-(x-b->w)*sin(angle/180*3.1415)+(y-b->h)*cos(angle/180*3.1415);
 x2=(x+b->w)*cos(angle/180*3.1415)-(y-b->h)*sin(angle/180*3.1415);
 y2=-(x+b->w)*sin(angle/180*3.1415)+(y-b->h)*cos(angle/180*3.1415);
 x3=(x+b->w)*cos(angle/180*3.1415)-(y+b->h)*sin(angle/180*3.1415);
 y3=-(x+b->w)*sin(angle/180*3.1415)+(y+b->h)*cos(angle/180*3.1415);
 x4=(x-b->w)*cos(angle/180*3.1415)-(y+b->h)*sin(angle/180*3.1415);
 y4=-(x-b->w)*sin(angle/180*3.1415)+(y+b->h)*cos(angle/180*3.1415);
 float dx1,dy1,dx2,dy2;
 
 //1
 d=abs(y2-y1);
 if (d>0)
 {
  dx1=(x4-x1)/(y4-y1);
  dy1=-(y4-y1)/(x4-x1);
  dx2=(x2-x1)/(y2-y1);
  dy2=-(y2-y1)/(x2-x1);
  int j,d;
  for (j=0; j<
 }
}*/
