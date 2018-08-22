#ifndef __XSTUB32_H
#define __XSTUB32_H
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: ASM\XSTUB32.H
 Opis: Plik naglowkowy funkcji i zmiennych zawartych w XSTUB32.ASM.
       Funkcja _xstub32init inicjuje ponizsze zmienne
------------------------------------------------
*/

//selektor o adresie poczatkowym 0 dla danych
extern unsigned short stub32zerosel;
//nazwa procesora
extern char cpu_name[13];
//wersja procesora
extern int cpu_ver;
//"etykieta" procesora
extern int cpu_brand;
//obslugiwane przez procesor
extern int cpu_feat;
extern int cpu_efeat;

//inicjuje modul
extern void _xstub32init();
//pobiera wartosc z pamieci o danym adresie
extern int __peek(void*);

#endif
