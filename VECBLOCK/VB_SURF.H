#ifndef __VB_SURF_H
#define __VB_SURF_H
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_SURF.C
 Opis: Obsluga powierzchni.
------------------------------------------------
*/

//==========================================

//Struktora przechowujaca powierzchnie
typedef struct _TSurf
{
 int swidth;            //Szerokosc powierzchni
 int sheight;           //Wysokosc powierzchni
 unsigned char *sbits;  //Wskaznik do bajtow powierzchni
} TSurf;

//Struktora definiujaca prostokat
typedef struct _TRect
{
 int rx;        //Pozycja X prostokata
 int ry;        //Pozycja Y prostokata
 int rw;        //Szerokosc prostokata
 int rh;        //Wysokosc prostokata
} TRect;

//Struktura definiujaca czcionke
typedef struct _TFnt
{
 unsigned char ctab[256];       //tablica znakow(['znak']->pozycja znaku)
 int width,height;              //szerokosc i wysokosc czcionki
 int space;                     //odstep pomiedzy kolejnymi znakami
 int charsize;                  //dlugosc znaku w bajtach=width*height
 unsigned char *fnttab;         //tablica znakow(0/1)
} TFnt;

//==========================================

#include "asm\surface8.h"

//==========================================

int Surf_Alloc (TSurf *);
int Surf_Free (TSurf *);
int Surf_Create(TSurf **,int,int);
int Surf_Dispose(TSurf **);
int Surf_LoadFromBMP (TCF *,TSurf **,const char *);
int Surf_Print8X8 (int,int,int,void *,TSurf *,char *);
int Surf_Printf8X8 (int,int,int,void *,TSurf *,char *,...);
TFnt *Surf_LoadFnt(int,int,int,TCF *,const char *);
int Surf_Print(int,int,int,TFnt *,TSurf *,char *);
int Surf_Printf(int,int,int,TFnt *,TSurf *,char *,...);
inline int Surf_GetFntW(TFnt *);
inline int Surf_GetFntW(TFnt *);

#endif
