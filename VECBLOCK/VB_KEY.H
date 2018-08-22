#ifndef __VB_KEY_H
#define __VB_KEY_H
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_KEY.H
 Opis: Obsluga klawiatury - plik naglowkowy
------------------------------------------------
*/

//===========================================

#include "asm\kb.h"

//===========================================

#define K_UP                    72
#define K_DOWN                  80
#define K_LEFT                  75
#define K_RIGHT                 77
#define K_INS                   82
#define K_DEL                   83
#define K_HOME                  71
#define K_END                   79
#define K_PGUP                  73
#define K_PGDOWN                81
#define K_ESC                   1
#define K_SPACE                 57
#define K_LSHF                  42
#define K_RSHF                  54
#define K_CTRL                  29
#define K_ALT                   56
#define K_TAB                   15
#define K_ENTER                 28
#define K_BKSPACE               14
#define K_F1                    59
#define K_F2                    60
#define K_F3                    61
#define K_F4                    62
#define K_F5                    63
#define K_F6                    64
#define K_F7                    65
#define K_F8                    66
#define K_F9                    67
#define K_F10                   68
#define K_F11                   87
#define K_F12                   88
#define K_SLASH                 53
#define K_BSLASH                43
#define K_TLD                   41

#define KEY_K2CN                54

//===========================================

int InitKeySys();
void CloseKeySys();
inline int Key_CGet(int);
int Key_Key2Char(int);
inline void Key_Bind (int,int);
void Key_Defaults();
void Key_Restore();

#endif
