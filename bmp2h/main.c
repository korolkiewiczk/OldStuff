#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
 FILE *f;
 char sign[2];
 unsigned short bpp;
 int imgstart;
 unsigned int BGR;
 unsigned int *wsk,*pwsk;
 unsigned int *pal=0;
 char *fn;
 int width,height;
 int i,j;
 
 if (argc<2)
 {
  printf("BMP2H by Kamil Korolkiewicz 12.2008\nBMP2H [bitmap file name]");
  return 0;
 }
 fn=argv[1];
 
 f=fopen(fn,"rb");
 if (f==0) {printf("File not found!"); return 0;}
 fread(&sign,1,2,f);
 if ((sign[0]!='B')&&(sign[1]!='M')) {printf("Invalid file!"); fclose(f); return 0;}
 fseek(f,0xa,SEEK_SET);
 fread(&imgstart,1,4,f);
 fseek(f,0x1c,SEEK_SET);
 fread(&bpp,1,2,f);
 fseek(f,0x12,SEEK_SET);
 fread(&width,1,4,f);
 fread(&height,1,4,f);
 if (bpp==8) 
 {
  pal=(unsigned int*)malloc(4*256);
  fseek(f,54,SEEK_SET);
  unsigned char *cwsk;
  cwsk=(unsigned char*)pal;
  for (i=0; i<256; i++)
  {
   fread(&BGR,1,4,f);
   *cwsk=(BGR>>16)&255;
   *(cwsk+1)=(BGR>>8)&255;
   *(cwsk+2)=(BGR)&255;
   *(cwsk+3)=255;
   cwsk+=4;
  }
 }
 else 
 if (bpp!=24) {fclose(f); return 0;}
 
 unsigned int *bf;
 bf=(unsigned int*)malloc(width*(height+1)*sizeof(unsigned int));
 
 fseek(f,imgstart,SEEK_SET);
 wsk=bf+(width*(height-1));
 int extrabits;
 if (bpp==24)
 {
  extrabits=4-((width*3)%4);
  for (i=0; i<height; i++)
  {
   pwsk=wsk;
   for (j=0; j<width; j++)
   {
    fread(&BGR,1,3,f);
    *(unsigned char*)((int)&BGR+3)=255;
    *wsk=BGR;
    wsk++;
   }
   if (extrabits!=4) fread(&BGR,1,extrabits,f);
   wsk=pwsk-width;
  }
 }
 else
 //if (bpp==8)
 {
  extrabits=4-((width)%4);
  for (i=0; i<height; i++)
  {
   pwsk=wsk;
   for (j=0; j<width; j++)
   {
    fread(&BGR,1,1,f);
    *wsk=pal[BGR&255];
    wsk++;
   }
   if (extrabits!=4) fread(&BGR,1,extrabits,f);
   wsk=pwsk-width;
  }
 }
 fclose(f);
 
 printf("//From: %s\n//Width: %d\n//Height: %d\n",fn,width,height);
 i=strlen(fn)-1;
 while ((i>=0)&&(fn[i]!='.')) i--;
 fn[i]=0;
 i--;
 while (i>=0) {fn[i]=toupper(fn[i]); i--;}
 printf("#define %s_WIDTH %d\n#define %s_HEIGHT %d\n",fn,width,fn,height);
 printf("unsigned int %s[%s_HEIGHT*%s_WIDTH]=\n{\n",fn,fn,fn);
 
 wsk=bf;
 for (i=0; i<height; i++)
 {
  for (j=0; j<width; j++)
  {
   if ((j==width-1)&&(i==height-1))
   printf(" 0x%X",*wsk);
   else
   printf(" 0x%X,",*wsk);
   wsk++;
  }
  printf("\n");
 }
 printf("};\n");
 
 return 1;
}
