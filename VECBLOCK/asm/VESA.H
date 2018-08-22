#ifndef __VESA_H
#define __VESA_H
//vesa.h dla VECBLOCK z vesa.inc

//ustawia tryb vesa
extern void SetVESAMode(int,int);
//informacje o trybie
extern int GetModeInfo(int,void*);
//informacje o vesa
extern int GetVESAInfo(void*);

#endif
