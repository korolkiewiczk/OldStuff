#ifndef __VGA_H
#define __VGA_H
//vga.h dla VECBLOC z vga.inc

enum VGAMODES
{
 VGATEXT                =       0x3,
 VGA320x200             =       0x13
};

//ustawia tryb vga
extern void SetVGAMode(int);
//ustawia dany kolor
extern void SetColor(int,int,int,int);
//ustawia palete kolorow
extern void SetPalette(void*);
//czeka na powrot pionowy
extern void Wait4VSync();

#endif
