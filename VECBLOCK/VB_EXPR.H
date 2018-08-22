#ifndef __VB_EXPR_H
#define __VB_EXPR_H
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_EXPR.H
 Opis: Plik naglowkowy pliku VB_EXPR.C
------------------------------------------------
*/

//===========================================

#define FVTAB_MAXCHARS  37      //znaki 'A'..'Z', '0'..'9' i '_' dla nazw
#define FV_MAXPARAMS    31
#define OP_MAXOPERANDS  7
#define EXPR_BUFLEN     256     //maksymalna dlugosc wyrazenia
#define EXPR_STRBUFLEN  512     //maksymalna dlugosc wprowadzanych ciagow
#define EXPR_STACKSIZE  128     //rozmiar stosu dla ONP
#define EXPR_INT        0       //dana calkowita
#define EXPR_FLOAT      1       //dana zmiennopozycyjna

//===========================================

#include "asm\prc.h"

//===========================================

enum EXPR_ERRORS
{
 EXPR_OK=1,
 EXPRERR_INVOP=-1,
 EXPRERR_UNDEFNAME=-2,
 EXPRERR_TMPARAMS=-3,
 EXPRERR_TFPARAMS=-4,
 EXPRERR_SYNTAX=-5,
 EXPRERR_UNDEF=0
};

//operatory dzialan
typedef struct _TOp
{
 unsigned char c;       //znak ASCII znaku operatora
 unsigned char npars;   //ilosc parametrow operatora
 unsigned char pr;      //priorytet operandu
 void *funcaddr;        //adres funkcji obslugi
} TOp;

//zmienne lub funkcje
typedef struct _TFuncVar
{
 void *funcaddr;       //adres funkcji obslugi
 unsigned char nparams;//ilosc parametrow funkcji
 unsigned int params;  //typy parametrow funkcji, bit 0 - typ zwracany przez funkcje
 float value;          //wartosc liczbowa dla zmiennej
} TFuncVar;

//przechowywane nazwy zmiennych i funkcji w strukturze typu USS
typedef struct _TFVTab
{
 struct _TFVTab *namechar[FVTAB_MAXCHARS];      //znaki
 struct _TFuncVar *funcvar;                     //jesli NULL to nie koniec slowa
} TFVTab;

//bibliteka standardowych funkcji
typedef struct _TFVLib
{
 char *name;
 TFuncVar fv;
} TFVLib;

//===========================================

int InitExpr ();
int Expr_FVA2I (char);
int Expr_AddFV (char *,TFuncVar *);
TFuncVar *Expr_FindFV (char *);
int Expr_IncludeLib (TFVLib *,int);
int Expr_FindOp (char);
int Expr_CheckErr (char *);
int Expr_ONP (char *,const char *);
int Expr_V2F (void *,void *,int);
float Expr_Comp (const char *);

#endif
