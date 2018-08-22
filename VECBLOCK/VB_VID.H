#ifndef __VB_VID_H
#define __VB_VID_H
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_VID.H
 Opis: Obsluga trybow graficznych
------------------------------------------------
*/

//==========================================

#include "asm\vga.h"
#include "asm\vesa.h"
#include "asm\fade8.h"
#include "asm\transp8.h"

//==========================================

#define VID_MAXMODES 10

//==========================================

typedef struct _TVidMode
{
 int modeval;
 int scrwidth,scrheight;
 int lfb;
} TVidMode;

//==========================================

int InitVid();
int Vid_SetMode (int);
void Vid_Swap();
int Vid_LoadPalFromBMP (TCF *,const char *);
int Vid_SetBrightness (int);
int Vid_FindRGB(int,int,int);
int Vid_InitFadeTab(int);
int Vid_InitTranspTab(int);
int Vid_PrintScr();

#endif
