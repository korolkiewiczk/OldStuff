#ifndef __HMEM_H
#define __HMEM_H
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: ASM\HMEM.H
 Opis: Plik naglowkowy funkcji i zmiennych zawartych w HMEM.INC dla C
       Funkcje umozliwiaja zainicjowanie pamieci heap z uzyciem DPMI
------------------------------------------------
*/

//liniowy adres poczatku pamieci heap
extern void *HEAP_LIN_ADDR;

//inicjuje pamiec heap (uzywa funkcji DPMI)
extern int InitHeapMem(int);
//zwalnia pamiec heap (uzywa funkcji DPMI)
extern int FreeHeapMem();

#endif
