#include <stdio.h>
#include <dir.h>
#include <math.h>

#define MAXC 64
#define CSHF 3
#define NCOL 254
#define CSTART 1
#define BM8DIR "../"

char flist[512][64];
int nfiles;

typedef struct
{
 int n;
 unsigned char r,c;
} rgbrec_t;

typedef struct
{
 int n;
 unsigned char r,g,b;
} crec_t;

rgbrec_t rgbrec[MAXC][MAXC][MAXC];
crec_t tabs[MAXC*MAXC*MAXC];
unsigned char pal[1024];

int main()
{
 struct ffblk fb;
 memset(&rgbrec,0,sizeof (rgbrec));
 //lista plikow bmp
 printf ("Czytam nazwy plikow bmp w katalogu...\n");
 int w=findfirst ("*.bmp",&fb,0);
 while(w==0)
 {
  strcpy(flist[nfiles++],fb.ff_name);
  w=findnext(&fb);
 }
 //1
 printf ("Odczytuje z plikow bmp do rgbrec...\n");
 int i;
 FILE *f;
 int x,y,e,j,k;
 for (i=0; i<nfiles; i++)
 {
  f=fopen (flist[i],"rb");
  fseek(f,0x12,SEEK_SET);
  fread (&x,4,1,f);
  fread (&y,4,1,f);
  e=4-(x%4);
  fseek(f,0x36,SEEK_SET);
  for (j=0; j<y; j++)
  {
   for (k=0; k<x; k++)
   {
    unsigned char r,g,b;
    fread (&b,1,1,f);
    fread (&g,1,1,f);
    fread (&r,1,1,f);
    rgbrec[r>>CSHF][g>>CSHF][b>>CSHF].n++;
    //rgbrec[r>>CSHF][g>>CSHF][b>>CSHF].r=0;
   }
   if (e!=4) fread(&w,1,e,f);
  }
  fclose(f);
 }
 //2
 printf ("Tworze tablice tabs...");
 int tabsl=0;
 memset(&tabs,0,sizeof(crec_t));
 rgbrec[0][0][0].r=1;
 int r,g,b,ar=0,ag=0,ab=0,ir=0,ig=0,ib=0,minc=1024,z;
 int sg,kg,sb,kb;
 j=0;
 for (r=0; r<MAXC; r++)
 {
  for (g=0; g<MAXC; g++)
  {
   for (b=0; b<MAXC; b++)
   {
    if (rgbrec[r][g][b].n!=0) printf ("%d:%d,%d,%d,%d\n",j++,rgbrec[r][g][b].n,r,g,b);
   }
  }
 }
 printf ("\n");
 while (1)
 {
 nextc:
 z=0;
 for (r=0; r<MAXC; r++)
 {
  sg=((ag-minc)<0)? 0:ag-minc;
  kg=((ag+minc)>MAXC)? MAXC:ag+minc;
  for (g=sg; g<kg; g++)
  {
   sb=((ab-minc)<0)? 0:ab-minc;
   kb=((ab+minc)>MAXC)? MAXC:ab+minc;
   for (b=sb; b<kb; b++)
   {
    if ((rgbrec[r][g][b].n!=0)&&(rgbrec[r][g][b].r==0))
    {
     if ((abs(r-ar)+abs(g-ag)+abs(b-ab))<minc)
     {
      minc=abs(r-ar)+abs(g-ag)+abs(b-ab);
      ir=r;
      ig=g;
      ib=b;
      z=1;
      if (b>ab) break;
     }
    }
   }//b
  }//g
 }//r
 endcl:
 if (z==0) break;
 ar=ir;
 ag=ig;
 ab=ib;
 tabs[tabsl].r=ir;
 tabs[tabsl].g=ig;
 tabs[tabsl].b=ib;
 tabs[tabsl].n=rgbrec[ir][ig][ib].n;
 rgbrec[ir][ig][ib].r=1;
 printf ("%d,%d,%d,%d,%d\n",tabsl,ir,ig,ib,tabs[tabsl].n);
 minc=1024;
 tabsl++;
 z=0;
 }
 printf ("tabsl=%d\n",tabsl);
 //3
 printf ("Tworze palete kolorow...\n");
 float nc,dc,fcol=0,pcol,bcol;
 unsigned int cr,cg,cb;
 nc=(float)tabsl/NCOL;
 if (nc<1)
 {
  for (i=CSTART;i<CSTART+NCOL;i++)
  {
   pal[i*4]=tabs[i].b<<CSHF;
   pal[i*4+1]=tabs[i].g<<CSHF;
   pal[i*4+2]=tabs[i].r<<CSHF;
   pal[i*4+3]=0;
   rgbrec[tabs[i].r][tabs[i].g][tabs[i].b].c=i;
  }
  goto bmpsave;
 }
 i=0;
 //pcol=0;
 dc=nc/floor(nc);
 //for (i=CSTART;i<CSTART+NCOL;i++)               //dla kazdego koloru palety
 while (i<NCOL)
 {
  cr=cg=cb=w=0;
  //for (j=0; j<nc;j++)
  pcol=0;
  k=0;
  while ((pcol<nc)&&(fcol<tabsl))
  {
   cr+=tabs[(int)fcol].n*tabs[(int)fcol].r;
   cg+=tabs[(int)fcol].n*tabs[(int)fcol].g;
   cb+=tabs[(int)fcol].n*tabs[(int)fcol].b;
   w+=tabs[(int)fcol].n;
   rgbrec[tabs[(int)fcol].r][tabs[(int)fcol].g][tabs[(int)fcol].b].c=i;
   bcol=fcol;
   pcol+=dc;
   fcol+=dc;
   if (((int)fcol-(int)bcol)>1) {fcol--,pcol--;}
   printf ("%.2f,%.2f.%.2f,%.2f,%d\n",fcol,pcol,dc,nc,(int)fcol);
   k++;
  }
  if (w==0) break;
  //pcol=fmod(pcol,1.f);-(int)((float)k*dc);
  //fcol=floor(fcol);
  cr/=w;
  cb/=w;
  cg/=w;
  pal[i*4]=cb<<CSHF;
  pal[i*4+1]=cg<<CSHF;
  pal[i*4+2]=cr<<CSHF;
  pal[i*4+3]=0;
  i++;
  printf ("%d:%d,%d,%d\n",i,cr,cg,cb);
 }
 //kolory graniczne
 bmpsave:
 pal[0]=0;
 pal[1]=0;
 pal[2]=0;
 pal[3]=0;
 pal[1020]=255;
 pal[1021]=255;
 pal[1022]=255;
 pal[1023]=0;
 //4
 printf ("Tworze pliki 8-bitowe...\n");
 FILE *f8;
 char fn[256];
 unsigned char head[0x36];
 void *wsk=head;
 for (i=0; i<nfiles; i++)
 {
  //int x,y,e;
  strcpy(fn,BM8DIR);
  strcat(fn,flist[i]);
  f=fopen (flist[i],"rb");
  f8=fopen (fn,"wb");
  fread(&head,0x36,1,f);
  x=*(int*)(wsk+0x12);
  y=*(int*)(wsk+0x16);
  head[0x1c]=8;                 //bpp
  (int)head[0xa]=1078;          //do bmp
  *(int*)(wsk+0x2)=1078+((4+((x-1)/4)*4)*y);
  *(int*)(wsk+0xa)=1078;
  *(int*)(wsk+0x22)=((4+((x-1)/4)*4)*y);
  *(int*)(wsk+0x2e)=0;
  *(int*)(wsk+0x32)=0;
  printf ("%s:%s\t%d,%d,fs=%d,is=%d\n",flist[i],fn,x,y,*(int*)(wsk+0x2),*(int*)(wsk+0x22));
  fwrite (&head,0x36,1,f8);
  fwrite (&pal,1,1024,f8);
  e=4-(x%4);
  for (j=0;j<y;j++)
  {
   for (k=0;k<x;k++)
   {
    fread (&b,1,1,f);
    fread (&g,1,1,f);
    fread (&r,1,1,f);
    w=rgbrec[r>>CSHF][g>>CSHF][b>>CSHF].c;
    fwrite (&w,1,1,f8);
   }
   if (e!=4) fread(&w,1,e,f);
   if (e!=4) fwrite(&w,1,e,f8);
  }
  fclose (f8);
  fclose (f);
 }
 printf ("ok\n");
}
