#include <stdio.h>

#define fontname "font.bin"
#define outfontname "fnt(%dx%d).bin"

int nchars,fntw,fnth;
unsigned char i2ctab[256];
unsigned char ftab[65536];
//unsigned char cftab[8192];

int main()
{
 FILE *f;
 int i,j,sb;
 unsigned char c;
 char fnbuf[32];
 f=fopen(fontname,"rb");
 if (!f) return 0;
 fread (&nchars,1,1,f);
 memset(i2ctab,255,256);
 for (i=0;i<nchars;i++)
 {
  fread (&c,1,1,f);
  i2ctab[c]=i;
  i2ctab[tolower(c)]=i;
 }
 fread (&fntw,1,1,f);
 fread (&fnth,1,1,f);
 fread (ftab,1,fntw*fnth*nchars,f);
 fclose(f);
 /*j=-1;
 for (i=0;i<fntw*fnth*nchars;i++)
 {
  if ((i%8)==0){j++; sb=0; cftab[j]=0;}
  cftab[j]|=(ftab[i]==1)<<sb;
  sb++;
 }*/
 sprintf(fnbuf,outfontname,fntw,fnth);
 f=fopen(fnbuf,"wb");
 fwrite (&nchars,1,1,f);
 fwrite (i2ctab,1,256,f);
 fwrite (ftab,1,fntw*fnth*nchars,f);
 fclose(f);
}
