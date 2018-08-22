//clib.h

#define NLETTERS 37
#define MAXNAMELEN 64
#define VAL_FLOAT 0
#define VAL_INT 2
#define VAL_STR 3
#define VAL_PTR 3
#define VAR 0xffffffff
#define MAXTXTLEN 1024
#define GET_PAR(p,n) (((p)>>((n)<<1))&3)

//definicje tagow
#define TAG_EXTRACODE 0x10000  //dodaj do do_code dodatkowy kod sterujacy zm. stat.

typedef struct _NAME
{
 void *fptr;
 //0..7 - liczba parametrow 8..15-zwracany typ 31-typ (zmienna/funkcja)
 unsigned int parattr;
 //po 2 bity na typ danego parametru
 unsigned int partypes;
 //stala wartosc przekazywana do funkcji, informacja dodatkowa itp.
 unsigned int tag;
} NAME;

typedef struct _ND
{
 struct _ND *l[NLETTERS];
 NAME *n;
} ND;

typedef struct _ELEMENT
{
 char *name;
 void *fptr;    //adres do funkcji lub do zmiennej w pamieci
 unsigned int rtype; //zwracany typ
 unsigned int parcount; //liczba parametrow, jesli zmienna to == VAR
 unsigned int partypes; //2 bity na parametr
 unsigned int tag;      //wartosc dodatkowa
} ELEMENT;

class CLib
{
 private:
 ND root;
 public:
 int index(char c);
 char unindex(char i);
 CLib();
 CLib(void *elist,int n);
 void AddElement(ELEMENT *e);
 void AddList(void *elist,int n);
 NAME *Find(char *name);
 int NListFromTxt(char *_t,char *ret,char schar);
};
