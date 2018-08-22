#ifndef __VB_MEM_H
#define __VB_MEM_H
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_MEM.H
 Opis: Plik naglowkowy VB_MEM.C.
------------------------------------------------
*/

//===========================================

#define MEM_BUFMAX 8192

//===========================================

int InitMem (int,int);
int CloseMem ();
void *Mem_SAlloc (int);
int Mem_DAlloc (void **,int);
int Mem_DPtr (void **,void*);
int Mem_FindAddr (void **);
int Mem_DFree (void **);

#endif
