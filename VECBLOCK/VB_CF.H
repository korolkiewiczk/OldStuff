#ifndef __VB_CF_H
#define __VB_CF_H
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_CF.H
 Opis: Obsluga plikow laczanych ("common file") - plik naglowkowy
------------------------------------------------
*/

//==========================================

#define CF_LOADED 0x12321

//==========================================

//naglowek
typedef struct _TFHead
{
 char sign[2];  //sygnaturka 'CF'
 int nf;        //ilosc plikow
} TFHead;

//lista plikow
typedef struct _TFTab
{
 char *name;    //nazwa pliku
 int pos;       //pozycja pliku
} TFTab;

//glowna struktora common file
typedef struct _TCF
{
 FILE *f;
 FILE *fo;                      //plik zewnetrzny
 struct _TFHead fhead;
 struct _TFTab *ftab;
 int currfile;
 int isloaded;
} TCF;

int CF_Load(TCF *,const char *);
int CF_Close(TCF *);
int CF_Read (TCF *,void *,int,int,const char *);
int CF_Curr(TCF *cf,const char *fn);
int CF_CRead (TCF *,void *,int,int);
int CF_CSeek (TCF *,int);

#endif
