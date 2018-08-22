/*
  X86 EMULATOR by Kamil Korolkiewicz 2007
  Plik: defx86.h
  Opis: Definicje typow danych i stalych dla x86
*/

//======================================

//definicje stalych

#define REG_COUNT 5                     //ilosc rodzajow rejestrow

//rejestry x86 - wartosci liczbowe odpowiadajace rejestrom
#define AL 0
#define BL 3
#define CL 1
#define DL 2
#define AH 4
#define BH 7
#define CH 5
#define DH 6

#define EAX 0
#define EBX 3
#define ECX 1
#define EDX 2
#define ESI 6
#define EDI 7
#define EBP 5
#define ESP 4

#define SEGREGS 1

#define ES 0
#define CS 1
#define SS 2
#define DS 3
#define FS 4
#define GS 5

//flagi (opis: patrz desc.txt)
//#define F_ 0x00000000

//opcje glowne
#define F_D            0x00000001
#define F_DP           0x00000002
#define F_MP           0x00000004
#define F_CP           0x00000008
#define F_REL          0x00000010
//prefiksy
#define F_ES           0x00000020
#define F_CS           0x00000040
#define F_SS           0x00000080
#define F_DS           0x00000100
#define F_FS           0x00000200
#define F_GS           0x00000400
#define F_LOCK         0x00000800
#define F_REPNE        0x00001000
#define F_REP          0x00002000
//opcje
#define F_DIR          0x00004000
#define F_PAR          0x00008000
#define F_IMM          0x00010000
#define F_FAR          0x00020000
#define F_PTR          0x00040000
#define F_DIMM         0x00080000
#define F_MIMM         0x00100000
#define F_IMMB         0x00200000
#define F_PAR1D        0x00400000
#define F_PAR2D        0x00800000
#define F_PAR2B        (F_PAR1D+F_PAR2D)
#define F_FORMS        0x01000000
#define F_IMM1         0x02000000
#define F_XCL          0x04000000
#define F_SW           0x08000000
#define F_TW           (F_SW+F_SD)
#define F_TWC          F_SW
#define F_W            0x10000000
#define F_DW           (F_W+F_SD)
#define F_DWC          F_W
#define F_QW           0x20000000
#define F_DQW          (F_QW+F_SD)
#define F_DQWC         F_QW
#define F_SD           0x40000000

//maska dla prefiksow
#define PREFIX_MASK    (F_CS|F_DS|F_ES|F_SS|F_FS|F_GS|F_LOCK|F_REPNE|F_REP)
#define ADSIZE_MASK    (F_SW|F_W|F_QW)
#define SEG_MASK       (F_CS|F_DS|F_ES|F_SS|F_FS|F_GS)

//pozycje w rejestrze eflags
//S Indicates a Status Flag
//C Indicates a Control Flag
//X Indicates a System Flag
#define FR_CF          0x00000001                //S Carry Flag (CF)
#define FR_PF          0x00000004                //S Parity Flag (PF)
#define FR_AF          0x00000010                //S Auxiliary Carry Flag (AF)
#define FR_ZF          0x00000040                //S Zero Flag (ZF)
#define FR_SF          0x00000080                //S Sign Flag (SF)
#define FR_TF          0x00000100                //X Trap Flag (TF)
#define FR_IF          0x00000200                //X Interrupt Enable Flag (IF)
#define FR_DF          0x00000400                //X Direction Flag (DF)
#define FR_OF          0x00000800                //X Overflow Flag (OF)
#define FR_IOPL1       0x00001000                //X I/O Privilege Level (IOPL)
#define FR_IOPL2       0x00002000                //---||---
#define FR_NT          0x00004000                //X Nested Task (NT)
#define FR_RF          0x00010000                //X Resume Flag (RF)
#define FR_VM          0x00020000                //X Virtual-8086 Mode (VM)
#define FR_AC          0x00040000                //X Alignment Check (AC)
#define FR_VIF         0x00080000                //X Virtual Interrupt Flag (VIF)
#define FR_VIP         0x00100000                //X Virtual Interrupt Pending (VIP)
#define FR_ID          0x00200000                //X ID Flag (ID)

//dla adtab - specjalne rodzaje adresowania (dotyczy disasemblera)
#define AD_IMMBP       ((char *)-1)              //immed lub bp
#define AD_SIB         ((char *)-2)              //adresowanie sib

//typy operandow
#define OP_REG         0                         //operand to rejestr
#define OP_MEM         1                         //operand to pamiec

//typy adresowania pamieci
#define MEM_R          0                         //adresowanie bez przesuniecia
#define MEM_RIMM8      1                         //adresowanie z przesunieciem 8
#define MEM_RIMM1632   2                         //adresowanie z przesunieciem 16/32

//wartosc poczatkowa CS i IP
#define CS_IN_START 0xf000
#define EIP_IN_START 0xf000

//maksymalna liczba urzadzen predefiniowanych I/O
#define DEV_MAX 10

//bledy inicjacji vm
#define INITERR_NOAVAILMEM -1                    //za malo pamieci


//parametry dla kompilacji
//#define FAST_FPU                                 //szybsze emulowanie instrukcji fpu
                                                 //kosztem zmiejszenia dokladnosci


//======================================

//definicje typow zmiennych
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned long long qword;
typedef char sbyte;
typedef short sword;
typedef int sdword;
typedef long long sqword;
typedef float fdword;
typedef double fqword;
typedef long double ftword;
typedef void* ptr;

//======================================

//definicje struktur

//struktura opisuje kod "opcode"
typedef struct _op_t
{
 char *name;             //nazwa instrukcji
 dword flags;            //informacje o instrukcji
 dword params;           //parametry
 dword funcindex;        //numer funckji wykonujacej instrukcje
 struct _op_t* next;     //kolejny kod rozkazu  
} op_t;

//wirtualna maszyna - opis
typedef struct _VM_t
{
 dword memsize;           //rozmiar pamieci wirtualnego PC
 dword devicecontrolfreq; //czestotliwosc sprawdzania urzadzen zgodna z tsc
 dword sysretfreq;        //czestotliwosc oddania watku systemowi (*devicefreq)
 dword memtypetabshf;     //rozmiar bloku informacji o pamieci uzyskiwany przy >>memtypetabshf
} VM_t;

//struktora przechowuje przedzialy (obsluga: misc.c)
typedef struct _range_t
{
 dword a,b;
 struct _range_t *next;
} range_t;

//definicja urzadzenia I/O
typedef struct _device_t
{
 ptr env;                 //srodowisko urzadzenia-obszar pamieci wewn. urzadzenia
 range_t *iorange;        //przedzial i/o urzadzenia
 ptr iofunctab;           //tablica funkcji obslugi wejscia/wyjscia (in/out)
 range_t *memrange;       //przedzial pamieci systemowej urzadzenia
 ptr memfunctab;          //tablica funkcji obslugi pamieci dla urzadzenia (pam)
 ptr memblockfunctab;     //tablica funkcji operacji na blokach pamieci
} device_t;

//======================================

//zmienne extern

//x86core.c
extern ptr vmmemaddr;
extern dword vmmemsize;
extern byte *vmmemtypetab;
extern dword vmmemtypetabshf;
extern byte *iotab;
extern dword flags;
extern dword params;
extern dword funcindex;
extern dword param[3];          //poszczegole parametry (operandy) rozkazu
extern dword adsize;
dword inline _call(ptr);
dword inline _call1p(ptr,dword);
dword inline _call2p(ptr,dword,dword);

//x86memory.c
byte inline mem_code_b();
word inline mem_code_w();
dword inline mem_code_d();

//dodatek
#define NV 0x78563412
void _null(int a)
{
 printf("%d",a);
}
