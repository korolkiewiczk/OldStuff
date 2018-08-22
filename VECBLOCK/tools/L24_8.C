#include <dir.h>
#include <stdio.h>

#define PBMPDESCNAME "bmdesc.txt"
#define KAT "../"
#define BMPPACKNAME "pbm.bmp"

typedef struct
{
 char fname[64];
 int w,h;
} tfdesc;

tfdesc fdesc[1024];
unsigned char head[0x36];
int nfiles=0;
void *buf;

void pack()
{
 struct ffblk fb;
 FILE *f,*fbm24,*fp;
 int fw,w,i,j,maxh=0,aw=0,e;
 void *wsk=&head,*bufwsk,*pbufwsk;
 remove(BMPPACKNAME);
 fw=findfirst ("*.bmp",&fb,0);
 while (fw==0)
 {
  strcpy(fdesc[nfiles].fname,fb.ff_name);
  f=fopen (fb.ff_name,"rb");
  fread(&head,0x36,1,f);
  fdesc[nfiles].w=*(int*)(wsk+0x12);
  fdesc[nfiles].h=*(int*)(wsk+0x16);
  if (fdesc[nfiles].h>maxh) maxh=fdesc[nfiles].h;
  aw+=fdesc[nfiles].w;
  fclose(f);
  fw=findnext (&fb);
  nfiles++;
 }
 aw=(4+((aw-1)/4)*4);
 buf=(void*)malloc(aw*maxh*3);
 memset (buf,0,aw*maxh*3);
 bufwsk=buf;
 printf ("w=%d,h=%d\n",aw,maxh);
 f=fopen (PBMPDESCNAME,"wt");
 fp=fopen (BMPPACKNAME,"wb");
 *(int*)(wsk+0x12)=aw;
 *(int*)(wsk+0x16)=maxh;
 *(int*)(wsk+0x2)=0x36+aw*maxh*3;
 *(int*)(wsk+0xa)=0x36;
 *(int*)(wsk+0x22)=aw*maxh*3;
 head[0x1c]=24;
 *(int*)(wsk+0x2e)=0;
 *(int*)(wsk+0x32)=0;
 fwrite (&head,0x36,1,fp);
 fprintf (f,"%d\n",nfiles);
 for (i=0; i<nfiles; i++)
 {
  fprintf (f,"%s %d %d\n",fdesc[i].fname,fdesc[i].w,fdesc[i].h);
  fbm24=fopen (fdesc[i].fname,"rb");
  fseek(fbm24,0x36,SEEK_SET);
  e=4-(fdesc[i].w%4);
  pbufwsk=bufwsk;
  for (j=0;j<fdesc[i].h;j++)
  {
   fread (bufwsk,1,fdesc[i].w*3,fbm24);
   bufwsk+=aw*3;
   if (e!=4) fread(&w,1,e,fbm24);
  }
  bufwsk=pbufwsk+fdesc[i].w*3;
  fclose (fbm24);
 }
 bufwsk=buf;
 for (i=0; i<aw*maxh*3;i++)
 {
  *(unsigned char*)(bufwsk++)&=0xfc;
 }
 fwrite (buf,1,aw*maxh*3,fp);
 fclose (fp);
 fclose(f);
}

void unpack()
{
 FILE *f,*fp,*fbm8;
 unsigned char pal[1024];
 char fn[1024];
 int i,j,w,h,e,_0=0;
 void *wsk=&head,*bufwsk,*pbufwsk;
 f=fopen (PBMPDESCNAME,"rt");
 fp=fopen (BMPPACKNAME,"rb");
 fread (&head,0x36,1,fp);
 w=*(int*)(wsk+0x12);
 h=*(int*)(wsk+0x12);
 buf=(void*)malloc(w*h);
 bufwsk=buf;
 *(int*)(wsk+0x2e)=0;
 *(int*)(wsk+0x32)=0;
 fread (&pal,1024,1,fp);
 fseek (fp,*(int*)(wsk+0xa),SEEK_SET);
 fread (buf,1,w*h,fp);
 fclose(fp);
 fscanf(f,"%d",&nfiles);
 printf("%d\n",nfiles);
 for (i=0; i<nfiles; i++)
 {
  fscanf (f,"%s %d %d",fdesc[i].fname,&fdesc[i].w,&fdesc[i].h);
  strcpy(fn,KAT);
  strcat (fn,fdesc[i].fname);
  printf ("%s %d %d\n",fn,fdesc[i].w,fdesc[i].h);
  fbm8=fopen (fn,"wb");
  *(int*)(wsk+0x12)=fdesc[i].w;
  *(int*)(wsk+0x16)=fdesc[i].h;
  *(int*)(wsk+0x2)=1078+((4+((fdesc[i].w-1)/4)*4)*fdesc[i].h);
  *(int*)(wsk+0xa)=1078;
  *(int*)(wsk+0x22)=((4+((fdesc[i].w-1)/4)*4)*fdesc[i].h);
  e=4-(fdesc[i].w%4);
  fwrite (&head,0x36,1,fbm8);
  fwrite (&pal,1024,1,fbm8);
  pbufwsk=bufwsk;
  for (j=0;j<fdesc[i].h;j++)
  {
   fwrite (bufwsk,1,fdesc[i].w,fbm8);
   bufwsk+=w;
   if (e!=4) fwrite(&_0,1,e,fbm8);
  }
  bufwsk=pbufwsk+fdesc[i].w;
  fclose (fbm8);
 }
}

int main(int argc,char *argv[])
{
 if (argc==1) pack();
 else
 if (strcmp("-u",argv[1])==0) unpack();
}
