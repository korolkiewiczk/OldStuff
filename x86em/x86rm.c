/*
  X86 EMULATOR by Kamil Korolkiewicz 2007
  Plik:x86rm.c
  Opis: Zapis/odczyt do rejestrow/pamieci - funkcje dostepowe
        Funkcje pozwalaja na dostep do wirtualnych rejestrow/pamieci,
        przekazuja wartosc do zmiennych ktore beda wykorzystane w funckjach
        symulujacych instrukcje procesora
*/

#include "x86addr.c"
#include "x86memory.c"

//======================================

//zmienne dostepowe

dword ddval;       //32bit
qword dqval;       //64bit
#ifdef FAST_FPU
fqword fval;       //wartosc zmienno-przecinkowa
#else
ftword fval;
#endif

//======================================

//parametr
dword rmpar;

//======================================

//funckje dostepowe rejestrow

/*
  NazwaTypuRejestru_operacja_rozmiar
  operacja: get-pobierz/set-zapisz
  rozmiar: b-8bit,w-16bit,d-32bit,q-64bit,t-80bit
*/


//CPUREGS

void reg_get_b()
{
 ddval=(byte)(cpuregs[rmpar&3]>>((rmpar<<2)>>3));
}

void reg_get_w()
{
 ddval=(word)cpuregs[rmpar];
}

void reg_get_d()
{
 ddval=cpuregs[rmpar];
}


void reg_set_b()
{
 *(byte*)(cpuregs+sizeof(dword)*(rmpar&3)+((rmpar<<2)>>1))=(byte)ddval;
}

void reg_set_w()
{
 *(word*)(cpuregs+sizeof(dword)*rmpar)=(word)ddval;
}

void reg_set_d()
{
 cpuregs[rmpar]=ddval;
}


//CPUSREGS

void sreg_get_w()
{
 ddval=cpusregs[rmpar];
}

void sreg_set_w()
{
 cpusregs[rmpar]=ddval;
}


//FPUREGS

void fpu_get_d()
{
 fval=fpuregs[rmpar];
}

void fpu_get_q()
{
 fval=fpuregs[rmpar];
}

void fpu_get_t()
{
 fval=fpuregs[rmpar];
}


void fpu_set_d()
{
 fpuregs[rmpar]=fval;
}

void fpu_set_q()
{
 fpuregs[rmpar]=fval;
}

void fpu_set_t()
{
 fpuregs[rmpar]=fval;
}


//CRREGS

void cr_get_b()
{
 ddval=(byte)(crregs[rmpar&3]>>((rmpar<<2)>>3));
}

void cr_get_w()
{
 ddval=(word)crregs[rmpar];
}

void cr_get_d()
{
 ddval=crregs[rmpar];
}


void cr_set_b()
{
 *(byte*)(crregs+sizeof(dword)*(rmpar&3)+((rmpar<<2)>>1))=(byte)ddval;
}

void cr_set_w()
{
 *(word*)(crregs+sizeof(dword)*rmpar)=(word)ddval;
}

void cr_set_d()
{
 crregs[rmpar]=ddval;
}


//DRREGS

void dr_get_b()
{
 ddval=(byte)(drregs[rmpar&3]>>((rmpar<<2)>>3));
}

void dr_get_w()
{
 ddval=(word)drregs[rmpar];
}

void dr_get_d()
{
 ddval=drregs[rmpar];
}


void dr_set_b()
{
 *(byte*)(drregs+sizeof(dword)*(rmpar&3)+((rmpar<<2)>>1))=(byte)ddval;
}

void dr_set_w()
{
 *(word*)(drregs+sizeof(dword)*rmpar)=(word)ddval;
}

void dr_set_d()
{
 drregs[rmpar]=ddval;
}


//======================================

//funckje dostepowe pamieci

//bez immed

void mem_get_b0()
{
 dword register addr=_call(addrcalltab[adsize][0][rmpar]);
 ddval=mem_read_b(addr);
}

void mem_get_w0()
{
 dword register addr=_call(addrcalltab[adsize][0][rmpar]);
 ddval=mem_read_w(addr);
}

void mem_get_d0()
{
 dword register addr=_call(addrcalltab[adsize][0][rmpar]);
 ddval=mem_read_d(addr);
}


void mem_set_b0()
{
 dword register addr=_call(addrcalltab[adsize][0][rmpar]);
 mem_write_b(addr,ddval);
}

void mem_set_w0()
{
 dword register addr=_call(addrcalltab[adsize][0][rmpar]);
 mem_write_w(addr,ddval);
}

void mem_set_d0()
{
 dword register addr=_call(addrcalltab[adsize][0][rmpar]);
 mem_write_d(addr,ddval);
}

//immed8

void mem_get_b1()
{
 dword register addr=_call(addrcalltab[adsize][1][rmpar]);
 ddval=mem_read_b(addr);
}

void mem_get_w1()
{
 dword register addr=_call(addrcalltab[adsize][1][rmpar]);
 ddval=mem_read_w(addr);
}

void mem_get_d1()
{
 dword register addr=_call(addrcalltab[adsize][1][rmpar]);
 ddval=mem_read_d(addr);
}


void mem_set_b1()
{
 dword register addr=_call(addrcalltab[adsize][1][rmpar]);
 mem_write_b(addr,ddval);
}

void mem_set_w1()
{
 dword register addr=_call(addrcalltab[adsize][1][rmpar]);
 mem_write_w(addr,ddval);
}

void mem_set_d1()
{
 dword register addr=_call(addrcalltab[adsize][1][rmpar]);
 mem_write_d(addr,ddval);
}

//immed16/32

void mem_get_b2()
{
 dword register addr=_call(addrcalltab[adsize][2][rmpar]);
 ddval=mem_read_b(addr);
}

void mem_get_w2()
{
 dword register addr=_call(addrcalltab[adsize][2][rmpar]);
 ddval=mem_read_w(addr);
}

void mem_get_d2()
{
 dword register addr=_call(addrcalltab[adsize][2][rmpar]);
 ddval=mem_read_d(addr);
}


void mem_set_b2()
{
 dword register addr=_call(addrcalltab[adsize][2][rmpar]);
 mem_write_b(addr,ddval);
}

void mem_set_w2()
{
 dword register addr=_call(addrcalltab[adsize][2][rmpar]);
 mem_write_w(addr,ddval);
}

void mem_set_d2()
{
 dword register addr=_call(addrcalltab[adsize][2][rmpar]);
 mem_write_d(addr,ddval);
}


//======================================

//tablica wywolan rm
//[x][][]-kolejny tryb, [][x][]-rozmiar: byte,word,dword,sword,qword,tword,dqword,zarezerwowane
//[][][x]-get/set

ptr rmcalltab[32][8][2]=
{
 //cpuregs
 {
  {reg_get_b,reg_set_b},
  {reg_get_w,reg_set_w},
  {reg_get_d,reg_set_d},
  {0,0},{0,0},{0,0},{0,0},{0,0}
 },
 
 //cpusregs
 {
  {reg_get_w,reg_set_w},
  {reg_get_w,reg_set_w},
  {reg_get_w,reg_set_w},
  {0,0},{0,0},{0,0},{0,0},{0,0}
 },
 
 //fpuregs
 {
  {0,0},{0,0},
  {fpu_get_d,fpu_set_d},
  {0,0},
  {fpu_get_q,fpu_set_q},
  {fpu_get_t,fpu_set_t},
  {0,0},{0,0}
 },
 
 //crregs
 {
  {cr_get_b,cr_set_b},
  {cr_get_w,cr_set_w},
  {cr_get_d,cr_set_d},
  {0,0},{0,0},{0,0},{0,0},{0,0}
 },
 
 //drregs
 {
  {dr_get_b,dr_set_b},
  {dr_get_w,dr_set_w},
  {dr_get_d,dr_set_d},
  {0,0},{0,0},{0,0},{0,0},{0,0}
 },
 
 //zarezerwowane 5-15
 {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
 {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
 {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
 {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
 {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
 {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
 {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
 {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
 {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
 {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
 {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
 
 //pamiec bez immed
 {
  {mem_get_b0,mem_set_b0},
  {mem_get_w0,mem_set_w0},
  {mem_get_d0,mem_set_d0},
  {0,0},{0,0},{0,0},{0,0},{0,0}
 },
 
 //pamiec immed8
 {
  {mem_get_b1,mem_set_b1},
  {mem_get_w1,mem_set_w1},
  {mem_get_d1,mem_set_d1},
  {0,0},{0,0},{0,0},{0,0},{0,0}
 },
 
 //pamiec immed16/32
 {
  {mem_get_b2,mem_set_b2},
  {mem_get_w2,mem_set_w2},
  {mem_get_d2,mem_set_d2},
  {0,0},{0,0},{0,0},{0,0},{0,0}
 }
};

//makra do pobierania i zapisywania danych

//a-parametr,b-rozmiar

#define rm_get(a,b) rmpar=(a&0xffff)>>8; _call(rmcalltab[(a)&0xff][b][0]);
#define rm_set(a,b) rmpar=(a&0xffff)>>8; _call(rmcalltab[(a)&0xff][b][1]);

//definicje dodatkowe

#define par1 param[0]
#define par2 param[1]
#define rm_byte   0
#define rm_word   1
#define rm_dword  2
#define rm_sword  3
#define rm_qword  4
#define rm_tword  5
#define rm_dqword 6

