//expr.h

#define ONPSTACKBUFLEN 16
#define NUMOFOP 19
#define ONP_NUM 0
#define ONP_NAME 1
#define ONP_NAMEREF 2
#define ONP_INUM 3
#define EXPR_SET_BUFLEN 512
#define MAXONPBUFLEN EXPR_SET_BUFLEN
#define CHAR_NUM 0
#define CHAR_LETTER 1
#define CHAR_LBRACKET 2
#define CHAR_RBRACKET 3
#define CHAR_COMMA 4
#define CHAR_QUOT 5
#define CHAR_LSBRACKET 6
#define CHAR_RSBRACKET 7
#define CHAR_OTHER 8
#define CHAR_HEXNUM 9
#define CHAR_REF 10
#define STRDATALEN 256
#define RETSTRLEN 64
#define MAXSTRINGS 16

typedef struct _OP
{
 char *opname,*fname;
 void *ref;
 unsigned int p;
} OP;

class CExpr
{
 public:
 enum errors
 {
  NoErr=0,
  NullStr=1,
  UndefinedName=2,
  SyntaxError=3,
  BufOverflow=4,
  TooManyParams=5,
  TooFewParams=6,
  UnreconOp=7
 };
 
 //Jesli 1 to wyrazenie jest wykonywane wiele razy, jesli -1 to co najwyzej kilka razy
 //jesli 0 to tylko raz
 static int MultiExec;
 enum
 {
  MultiExec_Once=0,
  MultiExec_Multi=1,
  MultiExec_Begin=-1,
  MultiExec_End=-2
 };
 
 private:

 CLib *library;
 static CLib *clib;
 static CExpr *cexpr;
 CExpr *pcexpr;
 char exprstr[EXPR_SET_BUFLEN];
 char strdata[STRDATALEN];
 char retstr[RETSTRLEN];
 int hashtab[MAXSTRINGS];
 int strcount;
 
#ifdef DC
 #define MAXCODELEN 4096
 #define CODE_SETB(p,b) *(unsigned char*)((p)++)=((unsigned char)b)
 #define CODE_SETW(p,w) *(unsigned short*)(p)=((unsigned short)w); p+=2
 #define CODE_SETWB(p,wb) *(unsigned int*)(p)=((unsigned int)wb); p+=3
 #define CODE_SETD(p,d) *(unsigned int*)(p)=((unsigned int)d); p+=4
 unsigned char *do_code;
#endif
 
 int opindex(char *s);
 int chartype(char c);
 unsigned int htoi(char *s);

#ifndef DC 
 //wywolywanie funkcji
 friend void CExpr_f0_i(CExpr*);
 friend void CExpr_f1_i(CExpr*);
 friend void CExpr_f2_i(CExpr*);
 friend void CExpr_f3_i(CExpr*);
 friend void CExpr_f4_i(CExpr*);
 friend void CExpr_f5_i(CExpr*);
 friend void CExpr_f6_i(CExpr*);
 friend void CExpr_f7_i(CExpr*);
 friend void CExpr_f8_i(CExpr*);
 friend void CExpr_f9_i(CExpr*);
 friend void CExpr_f10_i(CExpr*);
 friend void CExpr_f11_i(CExpr*);
 friend void CExpr_f12_i(CExpr*);
 friend void CExpr_f13_i(CExpr*);
 friend void CExpr_f14_i(CExpr*);
 friend void CExpr_f15_i(CExpr*);
 
 friend void CExpr_f0_f(CExpr*);
 friend void CExpr_f1_f(CExpr*);
 friend void CExpr_f2_f(CExpr*);
 friend void CExpr_f3_f(CExpr*);
 friend void CExpr_f4_f(CExpr*);
 friend void CExpr_f5_f(CExpr*);
 friend void CExpr_f6_f(CExpr*);
 friend void CExpr_f7_f(CExpr*);
 friend void CExpr_f8_f(CExpr*);
 friend void CExpr_f9_f(CExpr*);
 friend void CExpr_f10_f(CExpr*);
 friend void CExpr_f11_f(CExpr*);
 friend void CExpr_f12_f(CExpr*);
 friend void CExpr_f13_f(CExpr*);
 friend void CExpr_f14_f(CExpr*);
 friend void CExpr_f15_f(CExpr*);
 unsigned int (*faddr_i)(...);
 float (*faddr_f)(...);
#endif
 /*typedef unsigned int dyntab[2];
 dyntab *onp; *///[][0]-typ, [][1]-wartosc
 unsigned int onp[MAXONPBUFLEN][2];
 int onpl;
 unsigned int onpstack[ONPSTACKBUFLEN][2];  //[][0]-typ parametru, [][1]-wartosc
 int onpsl;
 int tag;
 
 public:
 CExpr() {}
 ~CExpr();
 CExpr(CLib *l);
 void operator=(CExpr *e);
 int Set(char *expr);
#ifdef DC
 float (*Do)();
#else
 float Do();
#endif
 static CLib *GetLib() {return clib;}
 static CExpr *GetExpr() {return cexpr;}
 char *RetStr() {return retstr;}
 int StrHash(int h) {return hashtab[h];}
 int StrCount() {return strcount;}
 int Tag() {return tag&65535;}
 char *ExprStr() {return exprstr;}
};
