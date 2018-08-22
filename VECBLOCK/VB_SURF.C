#ifndef __VB_SURF_C
#define __VB_SURF_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_SURF.C
 Opis: Obsluga powierzchni.
------------------------------------------------
*/

//===========================================

#include "vb_surf.h"

//===========================================


/*
########################################
 Surf_Alloc
 Alokuje powierzchnie
 Jesli ok 1 : 0
########################################
*/

int Surf_Alloc (TSurf *surf)
{
 return Mem_DAlloc((void *)&surf->sbits,surf->swidth*surf->sheight);
}

/*
########################################
 Surf_Free
 Zwalnia powierzchnie
 Jesli ok 1 : 0
########################################
*/

int Surf_Free (TSurf *surf)
{
 return Mem_DFree((void *)&surf->sbits);
}

/*
########################################
 Surf_Create
 Tworzy powierzchnie
 Jesli ok 1 : 0
########################################
*/

int Surf_Create(TSurf **surf,int w,int h)
{
 if (Mem_DAlloc((void *)surf,sizeof(TSurf))==0) return 0;
 (*surf)->swidth=w;
 (*surf)->sheight=h;
 if (Surf_Alloc(*surf)==0) return 0;
 return 1;
}

/*
########################################
 Surf_Dispose
 Usuwa powierzchnie z pamieci
 Jesli ok 1 : 0
########################################
*/

int Surf_Dispose(TSurf **surf)
{
 if (Surf_Free(*surf)==0) return 0;
 if (Mem_DFree((void*)surf)==0) return 0;
 return 1;
}

/*
########################################
 Surf_LoadFromBMP
 Laduje powierzchnie z pliku bmp 8-bitowego
 Jesli ok 1 : 0
########################################
*/

int Surf_LoadFromBMP (TCF *cf,TSurf **surf,const char *fn)
{
 char bmsgn[2];
 int imgstart,w,h;
 unsigned short bpx;
 int extrabits,extrabitstab;
 int i;
 void *wsk;
 if (CF_Curr(cf,fn)==0) return 0;
 CF_CRead(cf,&bmsgn,0,2);
 if ((bmsgn[0]!='B')&&(bmsgn[1]!='M')) return 0;
 CF_CSeek(cf,0xa);
 CF_CRead(cf,&imgstart,0,4);
 CF_CSeek(cf,0x1c);
 CF_CRead(cf,&bpx,0,2);
 if (bpx!=8) return 0;
 CF_CSeek(cf,0x12);
 CF_CRead(cf,&w,0,4);
 CF_CRead(cf,&h,0,4);
 if (*surf==0)           //powierzchnia nie zainicjowana
 {
  if (Surf_Create(surf,w,h)==0) return 0;
 }
 else                   //zainicjowana
 {
  (*surf)->swidth=w;
  (*surf)->sheight=h;
  if (Surf_Alloc(*surf)==0) return 0;
 }
 CF_CSeek(cf,imgstart);
 extrabits=4-((*surf)->swidth%4);
 wsk=(*surf)->sbits+((*surf)->swidth*((*surf)->sheight-1));
 for (i=0; i<(*surf)->sheight; i++)
 {
  CF_CRead(cf,wsk,0,(*surf)->swidth);
  if (extrabits!=4) CF_CRead(cf,&extrabitstab,0,extrabits);
  wsk-=(*surf)->swidth;
 }
 return 1;
}

/*
########################################
 Surf_Print8X8
 Pisze ciag ASCIIZ na powierzchni z uzyciem znakow 8X8 podanych w param. fnt
 Jesli c in [0..255] to numer koloru/ adres do powierzchni o rozmiarach 8X8.
 Jesli napis znajduje sie na powierzchni to 1 : 0
########################################
*/

int Surf_Print8X8 (int x,int y,int c,void *fnt,TSurf *surf,char *txt)
{
 char buf[1024];
 char ch;
 int i=0,j=0,w=0;
 if (txt[0]==0) return 0;
 while (1)
 {
  ch=txt[i++];
  if (ch==0)
  {
   buf[j]=0;
   break;
  }
  if ((ch==10)||(ch==13))       //enter
  {
   buf[j]=0;
   if ((txt[i]==10)||(txt[i]==13)) i++;
   if (c>255)
   w=w|DrawString8X8S(x,y,(TSurf*)c,fnt,buf,j,surf);
   else
   w=w|DrawString8X8(x,y,c,fnt,buf,j,surf);
   j=0;
   y+=8;
   if (y>surf->sheight) return w;
  }
  else
  if (ch=='\t')
  {
   buf[j++]=' ';
   buf[j++]=' ';
   buf[j++]=' ';
   buf[j++]=' ';
  }
  else
  buf[j++]=ch;
 }
 if (buf[0]!=0)
 if (c>255)
 w=w|DrawString8X8S(x,y,(TSurf*)c,fnt,buf,j,surf);
 else
 w=w|DrawString8X8(x,y,c,fnt,buf,j,surf);
 return w;
}

/*
########################################
 Surf_Printf8X8
 Pisze formatowany ciag ASCIIZ na powierzchni z uzyciem znakow 8X8
 podanych w param. fnt.
 Jesli c in [0..255] to numer koloru/ adres do powierzchni o rozmiarach 8X8.
 Jesli napis znajduje sie na powierzchni to 1 : 0
########################################
*/

int Surf_Printf8X8 (int x,int y,int c,void *fnt,TSurf *surf,char *txt,...)
{
 char buf[1024];
 va_list va;
 va_start(va,txt);
 vsprintf(buf,txt,va);
 va_end(va);
 return Surf_Print8X8(x,y,c,fnt,surf,buf);
}


/*
########################################
 Surf_LoadFnt
 Wczytuje czionke z pliku gdzie:
 w-szerokosc
 h-wysokosc czionki
 s-odstepy pomiedzy poszczegolnymi znakami
 Jesli ok to adres do nowej czionki : 0
########################################
*/

TFnt *Surf_LoadFnt(int w,int h,int s,TCF *cf,const char *fn)
{
 TFnt *fnt;
 int nchars;
 fnt=Mem_SAlloc(sizeof(TFnt));
 if (CF_Curr(cf,fn)==0) return 0;
 CF_CRead(cf,&nchars,0,1);
 CF_CRead(cf,&fnt->ctab,0,256);
 fnt->width=w;
 fnt->height=h;
 fnt->space=s;
 fnt->charsize=w*h;
 fnt->fnttab=Mem_SAlloc(fnt->charsize*nchars);
 CF_CRead(cf,fnt->fnttab,0,fnt->charsize*nchars);
 return fnt;
}

/*
########################################
 Surf_Print
 Pisze dany ciag ASCIZ w pozycji (x,y), o kolorze(0..255)
 lub powierchni (>255) c, przy czionce fnt(jesli 0 to 8X8), na powierzchni surf.
########################################
*/

int Surf_Print(int x,int y,int c,TFnt *fnt,TSurf *surf,char *txt)
{
 if (fnt==0) return Surf_Print8X8(x,y,c,&biosfont,surf,txt);
 char buf[1024];
 char ch;
 int i=0,j=0,w=0;
 if (txt[0]==0) return 0;
 while (1)
 {
  ch=txt[i++];
  if (ch==0)
  {
   buf[j]=0;
   break;
  }
  if ((ch==10)||(ch==13))       //enter
  {
   buf[j]=0;
   if ((txt[i]==10)||(txt[i]==13)) i++;
   if (c>255)
   w=w|DrawStringFntS(x,y,c,fnt,surf,j,buf);
   else
   w=w|DrawStringFnt(x,y,c,fnt,surf,j,buf);
   j=0;
   y+=8;
   if (y>surf->sheight) return w;
  }
  else
  if (ch=='\t')
  {
   buf[j++]=' ';
   buf[j++]=' ';
   buf[j++]=' ';
   buf[j++]=' ';
  }
  else
  buf[j++]=ch;
 }
 if (buf[0]!=0)
 if (c>255)
 w=w|DrawStringFntS(x,y,c,fnt,surf,j,buf);
 else
 w=w|DrawStringFnt(x,y,c,fnt,surf,j,buf);
 return w;
}


/*
########################################
 Surf_Printf
 Pisze formatwany dany ciag ASCIZ w pozycji (x,y), o kolorze(0..255)
 lub powierchni (>255) c, przy czionce fnt(jesli 0 to 8X8), na powierzchni surf.
########################################
*/

int Surf_Printf(int x,int y,int c,TFnt *fnt,TSurf *surf,char *txt,...)
{
 char buf[1024];
 va_list va;
 va_start(va,txt);
 vsprintf(buf,txt,va);
 va_end(va);
 return Surf_Print(x,y,c,fnt,surf,buf);
}

/*
########################################
 Surf_GetFntW
 Podaje szerokosc danej czcionki
########################################
*/

inline int Surf_GetFntW(TFnt *fnt)
{
 if (fnt) return fnt->width+fnt->space;
 else return 8;
}

/*
########################################
 Surf_GetFntH
 Podaje szerokosc danej czcionki
########################################
*/

inline int Surf_GetFntH(TFnt *fnt)
{
 if (fnt) return fnt->height+fnt->space;
 else return 8;
}

/*
########################################
 Surf_Blt(TRect*,TRect*,TSurf*,TSurf*)
 Rysuje powierzchnie na innej
 Jesli ok to adres do bitow powierzchni docelowej
########################################
*/

#define Surf_Blt DrawSurface8

/*
########################################
 Surf_SBlt(TRect*,TRect*,TSurf*,TSurf*)
 Rysuje powierzchnie na innej w skali
 Jesli ok to adres do bitow powierzchni docelowej
########################################
*/

#define Surf_SBlt DrawSurface8S

/*
########################################
 Surf_CBlt(TRect*,TRect*,TSurf*,TSurf*)
 Rysuje przezroczysta powierzchnie na innej
 Jesli ok to adres do bitow powierzchni docelowej
########################################
*/

#define Surf_CBlt DrawSurface8C

/*
########################################
 Surf_CSBlt(TRect*,TRect*,TSurf*,TSurf*)
 Rysuje przezroczysta powierzchnie na innej w skali
 Jesli ok to adres do bitow powierzchni docelowej
########################################
*/

#define Surf_CSBlt DrawSurface8CS


/*
########################################
 Surf_FBlt(TRect*,TRect*,TSurf*,TSurf*,int)
 Rysuje powierzchnie na innej z cieniowniem
 Jesli ok to adres do bitow powierzchni docelowej
########################################
*/

#define Surf_FBlt DrawSurface8F

/*
########################################
 Surf_FSBlt(TRect*,TRect*,TSurf*,TSurf*,int)
 Rysuje powierzchnie na innej w skali z cieniowaniem
 Jesli ok to adres do bitow powierzchni docelowej
########################################
*/

#define Surf_FSBlt DrawSurface8FS

/*
########################################
 Surf_TBlt(TRect*,TRect*,TSurf*,TSurf*,int,int)
 Rysuje powierzchnie na innej z cieniowniem i przezroczystoscia
 Jesli ok to adres do bitow powierzchni docelowej
########################################
*/

#define Surf_TBlt DrawSurface8T

/*
########################################
 Surf_TSBlt(TRect*,TRect*,TSurf*,TSurf*,int,int)
 Rysuje powierzchnie na innej w skali z cieniowaniem i przezroczystoscia
 Jesli ok to adres do bitow powierzchni docelowej
########################################
*/

#define Surf_TSBlt DrawSurface8TS

#endif
