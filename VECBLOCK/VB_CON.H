#ifndef __VB_CON_H
#define __VB_CON_H
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_CON.H
 Opis: Definicje konsoli.
------------------------------------------------
*/

//==========================================

#define CON_BUFLEN      (64*8)
#define CON_STRLEN      64
#define CON_STDLINES    21
#define CON_EDITBUFLEN  15
#define CON_ENTERKEY    10
#define CON_BKSPACEKEY  8
#define CON_DELKEY      127
#define CON_X           2
#define CON_Y           4
#define CON_CURSORCHAR  "_"
#define CON_CHARWXH     8
#define CON_LIBNFUNC    2

//==========================================

//prototypy funkcji
void InitConsole (TFnt *,int,TCF *,const char *,time_f,time_f);
void Con_Print(const char *);
void Con_Printf(const char *, ...);
void Con_EnterCmd();
int Con_GoUp();
int Con_GoDown();
int Con_WriteChar (unsigned char);
int Con_MoveCursor(int s);
void Con_PrevCommand();
void Con_NextCommand();
void Con_Draw();
void Con_GetKeyEvents();
void Con_OpenClose();
void Con_SetnLines(int);
void Con_SetConTime(float,float);

#endif
