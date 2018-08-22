/*
  X86 EMULATOR by Kamil Korolkiewicz 2007
  Plik:main.c
  Opis:Glowny modul programu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "defx86.h"
#include "asm\asmhead.h"
#include "misc.c"
#include "x86func.h"
#include "x86.h"
#include "x86device.c"
#include "x86rm.c"
#include "x86call.c"
#include "x86core.c"

/*byte tabins[]=
{
 0x66,0xb8,0xff,0xff,0x2d,0x89,0x17,0x66,0x9a,0x11,0x22,0x33,0x44,0x55,0x66,
 0xc8,0x12,0x34,0x11,0xec,0xed,0xee,0xef,0x66,0xee,0x66,0xef,0xAB,
 0x66,0xAB,0x6C,0x9C,0x66,0x9C,0x98,0x66,0x98,0x99,0x66,0x99,0xe3,0x12,0x66,0xe3,0x12,
 0x0f,0x80,0xe0,0xff,0xf,0xa0,0x66,0xd3,0x0,0xd0,0x0,0xf,0xa4,0x0,0x0f,0xf,0xac,0x00,0x23,
 0xf,0xaf,0x23,0xf,0x0,0x0,0x66,0x0f,0x1,0x00,0xd8,0x00,0xd8,0xc0,0xd8,0xd2,0x8b,0x7,
 0x66,0x67,0xf,0xa5,0x08,0x66,0x67,0x83,0x30,0x01,0x62,0,0xd9,0x2,0xd9,0xc3,0xd9,0xc9,
 0xd9,0xee,0xd9,0xd0,0xd9,0xfe,0xdb,0x28,0xdc,0x34,0xdf,0xe0,0xdf,0x30,0xdf,0x38,
 0x0f,0xb6,0x00,0x0f,0xb7,0x23,0x0f,0xb6,0xc3,0x66,0x0f,0xb7,0xc3,0x0f,0x92,0x00,
 0x0f,0x40,0xc2,0x0f,0xc0,0x23,0x0f,0xc1,0xc2,0xf,0x20,0x00
};*/
#define N 1024*1024
byte tabins[N];

int main(int argc,char *argv[])
{
 //wordsize=1;
 InitTimer();
 printf ("%f\n",(double)cpu_freq);
 FILE *f;
 f=fopen(argv[1],"r");
 fseek(f,0,SEEK_END);
 int n=ftell(f);
 fseek(f,0,SEEK_SET);
 if (n>N) n=N;
 fread(tabins,1,n,f);
 fclose(f);
 double t=GetTime();
 Disasm(tabins,n,1);
 t=GetTime()-t;
 printf ("%f\n",t);
 t=GetTime();
 sleep (1000/2);
 t=GetTime()-t;
 printf ("%f",t);
 /*int a,b;
 a=_rdtsc(0);
 Disasm(tabins,sizeof(tabins),1);
 b=_rdtsc(0);
 printf ("%d",b-a);*/
 mem_write_b(0,0);
 return 0;
}
