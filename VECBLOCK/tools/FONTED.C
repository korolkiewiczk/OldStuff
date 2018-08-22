#include "stdio.h"
#include "conio.h"

#define fontwidth 5
#define fontheight 5
#define fontmaxchars 255
#define fontname "font.bin"
#define fontcharsname "fontchrs.txt"

#define K_UP                    72
#define K_DOWN                  80
#define K_LEFT                  75
#define K_RIGHT                 77

char ctab[fontmaxchars];

unsigned char fontbits[fontmaxchars][fontheight][fontwidth];

unsigned char cchar,cx=0,cy=0;
int fontnchars=0;

int main()
{
 FILE *f;
 int i,j,c;
 f=fopen (fontname,"rb");
 memset(fontbits,0,fontnchars*fontheight*fontwidth);
 if (f)
 {
  fread (&fontnchars,1,1,f);
  fread (ctab,1,fontnchars,f);
  fread (&cchar,1,1,f);       //w
  fread (&cchar,1,1,f);       //h
  fread (fontbits,1,fontwidth*fontheight*fontnchars,f);
 }
 fclose(f);
 fontnchars=0;
 f=fopen (fontcharsname,"rb");
 if (!f) return 0;
 while (!feof(f)) fread(&ctab[fontnchars++],1,1,f);
 fontnchars--;
 fclose(f);
 cchar=0;
 while (1)
 {
  clrscr();
  gotoxy(1,1);
  for (j=0;j<fontheight;j++)
  {
   for (i=0;i<fontwidth*2;i++)
   {
    if (fontbits[cchar][j][i/2]) putch('Û'); else putch(' ');
   }
   putch(13);
   putch(10);
  }
  gotoxy(1,fontheight+2);
  printf("Aktualny znak:%c(index:%d/%d)",ctab[cchar],cchar+1,fontnchars);
  gotoxy(cx*2+1,cy+1);
  c=getch();
  if (c==27) break;
  if (c==0)
  {
   c=getch();
   if (c==K_UP) if(cy>0) cy--;
   if (c==K_DOWN) if (cy<fontheight-1) cy++;
   if (c==K_LEFT) if(cx>0) cx--;
   if (c==K_RIGHT) if (cx<fontwidth-1) cx++;
  }
  if (c==13) fontbits[cchar][cy][cx]=1-fontbits[cchar][cy][cx];
  if (c=='+') if (cchar<fontnchars-1) cchar++;
  if (c=='-') if (cchar>0) cchar--;
 }
 f=fopen (fontname,"wb");
 fwrite (&fontnchars,1,1,f);
 fwrite (ctab,1,fontnchars,f);
 cchar=fontwidth;
 fwrite (&cchar,1,1,f);
 cchar=fontheight;
 fwrite (&cchar,1,1,f);
 fwrite (fontbits,1,fontwidth*fontheight*fontnchars,f);
 fclose(f);
}
