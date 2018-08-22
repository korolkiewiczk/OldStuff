#ifndef __VB_MENU_H
#define __VB_MENU_H
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_MENU.H
 Opis: Obsluga menu - plik naglowkowy
------------------------------------------------
*/

//==========================================

#define MENU_SNAMEMAXLEN 32
#define MENU_NPARAMS 9

#define MENUPAR_NAME            0
#define MENUPAR_TYPE            1
#define MENUPAR_ACTION          2
#define MENUPAR_ALT             3
#define MENUPAR_VREF            4
#define MENUPAR_BAR             5
#define MENUPAR_VAR             6
#define MENUPAR_LIST            7
#define MENUPAR_HIDDEN          8

#define MENU_ITYPE_NI           0               //normal item
#define MENU_ITYPE_BAR          1               //"suwak"
#define MENU_ITYPE_ONOFF        2               //pole on/off
#define MENU_ITYPE_VAR          3               //jakas liczba
#define MENU_ITYPE_LIST         4               //lista wyboru

#define MENUCMD_UP              0
#define MENUCMD_DOWN            1
#define MENUCMD_RIGHT           2
#define MENUCMD_LEFT            3
#define MENUCMD_ENTER           4
#define MENUCMD_ESC             5


//==========================================

typedef struct _TMenuSection
{
 char sname[MENU_SNAMEMAXLEN];          //nazwa sekcji
 int spos;                              //pozycja w sekcji
 int nspos;                             //ilosc pozycji pozycja
 void *itaddr;                          //adres do pol menu
} TMenuSection;

typedef struct _TAlt
{
 char *line;
 struct _TAlt *next;
} TAlt;

typedef struct _TMenuItem
{
 char *txt;             //nazwa pola
 int type;              //typ
 int value;             //domyslna wartosc (dla typu:lista)
 int maxvalue;          //maksymalna wartosc value
 int ishidden;          //czy pole dostepne
 char *action;          //ciag dotyczacy wykonywanej akcji
 int *tstruct;          //pole wlasciwe dla typu
 char *vref;            //nazwa referowanej zmiennej VR
 TAlt *alt;             //opis
 //kolejna komorka
 struct _TMenuItem *next;
} TMenuItem;

typedef struct _TSBarVar        //takie same dla bar i var
{
 int min,max,delta;
} TSBarVar;

typedef struct _TSList          //pole specjalne dla listy
{
 char *name;
 char *proc;
 struct _TSList *next;
} TSList;

//==========================================

int Menu_Load (TCF *,char *,int,char *,char *,int,char *,char *);
int Menu_TXT2Par (char *);
int Menu_Par (int,char *,TMenuItem *);
int Menu_Section2int (char *);
inline int Menu_CurrS();
inline int Menu_CurrPos();
void Menu_Cmd (int);
void Menu_Draw();
void Menu_DrawAlt();
void Menu_GetKeyEvents();
void Menu_VidApply();
int Menu_LoadBar(TCF *,char *,char *,int);

#endif
