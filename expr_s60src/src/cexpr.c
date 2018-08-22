//expr.c

#include "hash.h"

int CExpr::MultiExec=CExpr::MultiExec_Once;

//funkcje wewnetrzne

float CExpr_op_add(float a,float b)
{
 return a+b;
}

float CExpr_op_sub(float a,float b)
{
 return a-b;
}

float CExpr_op_mul(float a,float b)
{
 return a*b;
}

float CExpr_op_div(float a,float b)
{
 return a/b;
}

float CExpr_op_mod(float a,float b)
{
 return fmod(a,b);
}

float CExpr_op_pow(float a,float b)
{
 return pow(a,b);
}

float CExpr_op_equ_eps=0.01;
float CExpr_op_equ(float a,float b)
{
 float v=fabs(a-b);
 if (v<CExpr_op_equ_eps)
 {
  return 1-v/CExpr_op_equ_eps;
 }
 else
 return 0;
}

float CExpr_op_equp(float a,float b)
{
 return a==b;
}

float CExpr_op_nequ(float a,float b)
{
 float v=fabs(a-b);
 if (v<CExpr_op_equ_eps)
 {
  return v/CExpr_op_equ_eps;
 }
 else
 return 1;
}


float CExpr_op_gt(float a,float b)
{
 return a>b;
}

float CExpr_op_lw(float a,float b)
{
 return a<b;
}

float CExpr_op_gte(float a,float b)
{
 float v=fabs(a-b);
 if ((a<b)&&(v<CExpr_op_equ_eps))
 {
  return 1-v/CExpr_op_equ_eps;
 }
 if (a>=b) return 1;
 else
 return 0;
}

float CExpr_op_lwe(float a,float b)
{
 float v=fabs(a-b);
 if ((a>b)&&(v<CExpr_op_equ_eps))
 {
  return 1-v/CExpr_op_equ_eps;
 }
 if (a<=b) return 1;
 else
 return 0;
}

float CExpr_op_land(float a,float b)
{
 return ((a!=0)&&(b!=0));
}

float CExpr_op_lor(float a,float b)
{
 return !((a==0)&&(b==0));
}

float CExpr_op_lxor(float a,float b)
{
 return ((a==0)&&(b!=0))||((a!=0)&&(b==0));
}

float CExpr_op_limp(float a,float b)
{
 if ((a!=0)&&(b==0)) return 0;
 else
 return 1;
}

float CExpr_op_lequ(float a,float b)
{
 return ((a==0)&&(b==0))||((a!=0)&&(b!=0));
}

float CExpr_op_set(float *a,float b)
{
 if (CSys::IsValidWPtr((void*)a))
 *a=b;
 return b;
}

float CExpr_op_chs(float a)
{
 return -a;
}

unsigned int CExpr_op_ptr(void *a,int b)
{
 if (CSys::IsValidRPtr((void*)((int)a+b)))
 return *(unsigned int*)((int)a+b);
 else return 0;
}

float CExpr_nullfunc()
{
 return 0;
}

#ifndef DC

//int

void CExpr_f0_i(CExpr *e)
{
 e->onpstack[++e->onpsl][1]=(*e->faddr_i)();
}

void CExpr_f1_i(CExpr *e)
{
 e->onpstack[e->onpsl][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl][1]
 );
}

void CExpr_f2_i(CExpr *e)
{
 e->onpstack[e->onpsl-1][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=1;
}
void CExpr_f3_i(CExpr *e)
{
 e->onpstack[e->onpsl-2][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=2;
}

void CExpr_f4_i(CExpr *e)
{
 e->onpstack[e->onpsl-3][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=3;
}

void CExpr_f5_i(CExpr *e)
{
 e->onpstack[e->onpsl-4][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=4;
}

void CExpr_f6_i(CExpr *e)
{
 e->onpstack[e->onpsl-5][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=5;
}

void CExpr_f7_i(CExpr *e)
{
 e->onpstack[e->onpsl-6][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=6;
}

void CExpr_f8_i(CExpr *e)
{
 e->onpstack[e->onpsl-7][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=7;
}

void CExpr_f9_i(CExpr *e)
{
 e->onpstack[e->onpsl-8][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=8;
}

void CExpr_f10_i(CExpr *e)
{
 e->onpstack[e->onpsl-9][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-9][1],
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=9;
}

void CExpr_f11_i(CExpr *e)
{
 e->onpstack[e->onpsl-10][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-10][1],
  e->onpstack[e->onpsl-9][1],
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=10;
}

void CExpr_f12_i(CExpr *e)
{
 e->onpstack[e->onpsl-11][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-11][1],
  e->onpstack[e->onpsl-10][1],
  e->onpstack[e->onpsl-9][1],
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=11;
}

void CExpr_f13_i(CExpr *e)
{
 e->onpstack[e->onpsl-12][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-12][1],
  e->onpstack[e->onpsl-11][1],
  e->onpstack[e->onpsl-10][1],
  e->onpstack[e->onpsl-9][1],
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=12;
}

void CExpr_f14_i(CExpr *e)
{
 e->onpstack[e->onpsl-13][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-13][1],
  e->onpstack[e->onpsl-12][1],
  e->onpstack[e->onpsl-11][1],
  e->onpstack[e->onpsl-10][1],
  e->onpstack[e->onpsl-9][1],
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=13;
}

void CExpr_f15_i(CExpr *e)
{
 e->onpstack[e->onpsl-14][1]=(*e->faddr_i)
 (
  e->onpstack[e->onpsl-14][1],
  e->onpstack[e->onpsl-13][1],
  e->onpstack[e->onpsl-12][1],
  e->onpstack[e->onpsl-11][1],
  e->onpstack[e->onpsl-10][1],
  e->onpstack[e->onpsl-9][1],
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=14;
}

//float

void CExpr_f0_f(CExpr *e)
{
 *(float*)((int)e->onpstack+(++e->onpsl)*8+4)=(*e->faddr_f)();
}

void CExpr_f1_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl][1]
 );
}

void CExpr_f2_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-1*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=1;
}
void CExpr_f3_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-2*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=2;
}

void CExpr_f4_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-3*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=3;
}

void CExpr_f5_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-4*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=4;
}

void CExpr_f6_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-5*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=5;
}

void CExpr_f7_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-6*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=6;
}

void CExpr_f8_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-7*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=7;
}

void CExpr_f9_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-8*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=8;
}

void CExpr_f10_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-9*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-9][1],
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=9;
}

void CExpr_f11_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-10*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-10][1],
  e->onpstack[e->onpsl-9][1],
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=10;
}

void CExpr_f12_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-11*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-11][1],
  e->onpstack[e->onpsl-10][1],
  e->onpstack[e->onpsl-9][1],
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=11;
}

void CExpr_f13_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-12*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-12][1],
  e->onpstack[e->onpsl-11][1],
  e->onpstack[e->onpsl-10][1],
  e->onpstack[e->onpsl-9][1],
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=12;
}

void CExpr_f14_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-13*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-13][1],
  e->onpstack[e->onpsl-12][1],
  e->onpstack[e->onpsl-11][1],
  e->onpstack[e->onpsl-10][1],
  e->onpstack[e->onpsl-9][1],
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=13;
}

void CExpr_f15_f(CExpr *e)
{
 *(float*)((int)e->onpstack+e->onpsl*8+4-14*8)=(*e->faddr_f)
 (
  e->onpstack[e->onpsl-14][1],
  e->onpstack[e->onpsl-13][1],
  e->onpstack[e->onpsl-12][1],
  e->onpstack[e->onpsl-11][1],
  e->onpstack[e->onpsl-10][1],
  e->onpstack[e->onpsl-9][1],
  e->onpstack[e->onpsl-8][1],
  e->onpstack[e->onpsl-7][1],
  e->onpstack[e->onpsl-6][1],
  e->onpstack[e->onpsl-5][1],
  e->onpstack[e->onpsl-4][1],
  e->onpstack[e->onpsl-3][1],
  e->onpstack[e->onpsl-2][1],
  e->onpstack[e->onpsl-1][1],
  e->onpstack[e->onpsl][1]
 );
 e->onpsl-=14;
}

//[][0]-float [][1]-int
void (*calltab[][2])(CExpr *)=
{
 {CExpr_f0_f,CExpr_f0_i},
 {CExpr_f1_f,CExpr_f1_i},
 {CExpr_f2_f,CExpr_f2_i},
 {CExpr_f3_f,CExpr_f3_i},
 {CExpr_f4_f,CExpr_f4_i},
 {CExpr_f5_f,CExpr_f5_i},
 {CExpr_f6_f,CExpr_f6_i},
 {CExpr_f7_f,CExpr_f7_i},
 {CExpr_f8_f,CExpr_f8_i},
 {CExpr_f9_f,CExpr_f9_i},
 {CExpr_f10_f,CExpr_f10_i},
 {CExpr_f11_f,CExpr_f11_i},
 {CExpr_f12_f,CExpr_f12_i},
 {CExpr_f13_f,CExpr_f13_i},
 {CExpr_f14_f,CExpr_f14_i},
 {CExpr_f15_f,CExpr_f15_i},
};

#endif

OP CExpr_operands[NUMOFOP]=
{
 {"+","ADD",(void*)CExpr_op_add,100},
 {"-","SUB",(void*)CExpr_op_sub,100},
 {"*","MUL",(void*)CExpr_op_mul,200},
 {"/","DIV",(void*)CExpr_op_div,200},
 {"^","POW",(void*)CExpr_op_pow,400},
 {"%","MOD",(void*)CExpr_op_mod,200},
 {"=","EQU",(void*)CExpr_op_equ,50},
 {"==","EQUP",(void*)CExpr_op_equp,50},
 {"!=","NEQU",(void*)CExpr_op_nequ,50},
 {">","GT",(void*)CExpr_op_gt,50},
 {"<","LW",(void*)CExpr_op_lw,50},
 {">=","GTE",(void*)CExpr_op_gte,50},
 {"<=","LWE",(void*)CExpr_op_lwe,50},
 {"&&","LAND",(void*)CExpr_op_land,50},
 {"||","LOR",(void*)CExpr_op_lor,50},
 {"^^","LXOR",(void*)CExpr_op_lxor,50},
 {"=>","LIMP",(void*)CExpr_op_limp,50},
 {"<=>","LEQU",(void*)CExpr_op_lequ,50},
 {":=","SET",(void*)CExpr_op_set,50}
};

char *CExpr_ErrorNames[]=
{
 "OK",
 "Null string",
 "Undefined name",
 "Syntax error",
 "Buffer overflow",
 "Too many parameters",
 "Too few parameters",
 "Unrecognized operand"
};

CLib *CExpr::clib=0;
CExpr *CExpr::cexpr=0;

CExpr::CExpr(CLib *l)
{
 ELEMENT e;
 onpl=0;
 exprstr[0]=0;
 strdata[0]=0;
 retstr[0]=0;
 library=l;
#ifdef DC
 //do_code=(unsigned char *)CSys::AllocMem(MAXCODELEN);
 do_code=0;
 Do=(float(*)())&CExpr_nullfunc;
#endif
 //onp=(dyntab*)CSys::AllocMem(MAXONPBUFLEN);
 if (CExpr::clib==l) return;
 CExpr::clib=l;
 
 int i;
 e.partypes=0; //tylko float
 e.tag=0;
 e.parcount=2;
 e.rtype=VAL_FLOAT;
 for (i=0; i<NUMOFOP-1; i++)
 {
  e.name=CExpr_operands[i].fname;
  e.fptr=CExpr_operands[i].ref;
  library->AddElement(&e);
 }
 
 //set
 e.partypes=VAL_PTR;
 e.name=CExpr_operands[i].fname;
 e.fptr=CExpr_operands[i].ref;
 library->AddElement(&e);

 //chs
 e.partypes=0;
 e.name="CHS";
 e.fptr=(void*)CExpr_op_chs;
 e.parcount=1;
 library->AddElement(&e);
 
 //ptr
 e.partypes=VAL_PTR+VAL_INT*4;
 e.rtype=VAL_PTR;
 e.name="PTR";
 e.fptr=(void*)CExpr_op_ptr;
 e.parcount=2;
 library->AddElement(&e);
 
 //_eps
 e.parcount=VAR;
 e.rtype=VAL_FLOAT;
 e.name="_eps";
 e.fptr=&CExpr_op_equ_eps;
 library->AddElement(&e);
}

CExpr::~CExpr()
{
#ifdef DC
 CSys::FreeMem(do_code);
 do_code=0;
 Do=(float(*)())&CExpr_nullfunc;
#endif
}

int CExpr::opindex(char *s)
{
 for (int i=0; i<NUMOFOP; i++)
 {
  int j=0;
  while ((CExpr_operands[i].opname[j]==s[j])&&(s[j]!=0)) j++;
  if (s[j]==0) return i;
 }
 return -1;
}

int CExpr::chartype(char c)
{
 if ((c>='0')&&(c<='9')||(c=='.')) return CHAR_NUM;
 if (((c>='A')&&(c<='Z'))||((c>='a')&&(c<='z'))||(c=='_')) return CHAR_LETTER;
 if (c=='(') return CHAR_LBRACKET;
 if (c==')') return CHAR_RBRACKET;
 if (c==',') return CHAR_COMMA;
 if ((c=='"')||(c=='\'')) return CHAR_QUOT;
 if (c=='[') return CHAR_LSBRACKET;
 if (c==']') return CHAR_RSBRACKET;
 if (c=='$') return CHAR_HEXNUM;
 if (c=='@') return CHAR_REF;
 if (c==0) return -1;
 return CHAR_OTHER;
}

unsigned int CExpr::htoi(char *s)
{
 int sl=strlen(s);
 int w=0;
 int m=1,i;
 char c;
 for (i=sl-1; i>=0; i--)
 {
  c=s[i];
  if ((c>='0')&&(c<='9')) w+=(c-'0')*m;
  else
  if ((c>='A')&&(c<='F')) w+=(c-'A'+10)*m;
  else
  if ((c>='a')&&(c<='f')) w+=(c-'a'+10)*m;
  m<<=4;
 }
 return w;
}

void CExpr::operator=(CExpr *e)
{
#ifdef DC
 //unsigned char *dc=do_code;
#endif
 memcpy(this,e,sizeof(CExpr));
#ifdef DC
 /*if (dc==0)
 {
  do_code=(unsigned char *)CSys::AllocMem(MAXCODELEN);
 }
 else
 {
  do_code=dc;
 }*/
 //do_code=(unsigned char *)CSys::AllocMem(MAXCODELEN);
 do_code=0;
 Do=(float(*)())&CExpr_nullfunc;
#endif
 if (e->exprstr[0]!=0) Set(e->exprstr);
}

//zwraca 0 jesli ok | >1 - kod bledu
int CExpr::Set(char *expr)
{
#define CExpr_SetErr(e) {strcpy(retstr,CExpr_ErrorNames[e]); onpl=0; return e;}
 if (*expr==0) CExpr_SetErr(NullStr)
 char bf[EXPR_SET_BUFLEN];
 char exprbuf[EXPR_SET_BUFLEN];
 unsigned int stack[EXPR_SET_BUFLEN][2]; //[][0]-wartosc,[][1]-priorytet
 int sl=-1;
 float pom=0;
 
 int pctype=-1,ctype=-1;
 int stype=0;
 
 int funcstack[EXPR_SET_BUFLEN][5];
 int funcsl=-1;
 const int freq=0,npar=1,parl=2,partype=3,bracketv=4;
 
 int stron=0,strl=0,strl0=0,strbson=0;
 char strchar=0;
 
 int vstart=2; //okresla czy piewszy znak po nawiasie ( lub poczatek
 
 int vcount=0; //licznik ilosci wartosci na stosie
 int bcount=0; //licznik nawiasow
 int sbcount=0; //licznik nawiasow kwadratowych
 int sref=0;    //czy stosowac referencje
 int i=0;
 char c;
 strcount=0;
 exprstr[0]=0;
 
 int isextracode=0; //dla trybu do_code
 
 //usun spacje
 while(*expr)
 {
  c=*(expr++);
  if (stron==0)
  {
   if ((c=='"')||(c=='\'')) {stron=1; strchar=c;}
  }
  else
  if (stron==1)
  {
   if ((strbson==0)&&(c=='\\')) strbson=1;
   else
   {
    if ((c==strchar)&&(strbson==0)) stron=0;
    strbson=0;
   }
  }
  if ((c!=' ')||(stron==1))
  exprbuf[i++]=c;
  if (i==EXPR_SET_BUFLEN) CExpr_SetErr(BufOverflow)
 }
 if (stron==1) CExpr_SetErr(SyntaxError)
 exprbuf[i]=0;
 expr=exprbuf;
 
 i=0;
 onpl=0;
 while (1)
 {
  c=*expr;
  if (stron)
  //jesli ciag znakow
  {
   if ((strbson==0)&&(c=='\\'))
   {
    strbson=1;
   }
   else
   if ((c==strchar)&&(strbson==0))
   {
    strdata[strl++]=0;
    if (stron==2)
    {
     onp[onpl][0]=ONP_NUM;
     *(float*)((int)onp+onpl*8+4)=(float)strlen((char*)(strl0+(int)strdata));
     onpl++;
    }
    else
    {
     if (strcount==MAXSTRINGS) CExpr_SetErr(BufOverflow)
     onp[onpl][0]=ONP_INUM;
     onp[onpl++][1]=strl0+(int)strdata;
     hashtab[strcount++]=str2hash((char*)(strl0+(int)strdata));
    }
    stron=0;
    i=0;
    strl0=strl;
   }
   else
   {
    if (strbson==1)
    {
     if (c=='n') c='\n';
     else
     if (c=='t') c='\t';
     else
     if (c=='0')
     {
      i=0;
      while (1)
      {
       c=*(expr++);
       int ct=chartype(c);
       if (ct!=CHAR_NUM) break;
       bf[i++]=c;
      }
      expr-=2;
      bf[i]=0;
      c=atoi(bf);
     }
    }
    strdata[strl++]=c;
    if (strl>=STRDATALEN-1) CExpr_SetErr(BufOverflow)
    if (c==0) c=1;
    strbson=0;
   }
  }
  else
  {
   ctype=chartype(c);
   if (funcsl>=0)
   {
    if ((funcstack[funcsl][freq]==-1)&&(pctype!=CHAR_LBRACKET)) CExpr_SetErr(SyntaxError)
    if ((funcstack[funcsl][freq]==2)&&(pctype!=CHAR_RBRACKET)) CExpr_SetErr(TooManyParams)
   }
   if (sref==1)
   {
    if (pctype==CHAR_LETTER) sref=2;
    else CExpr_SetErr(SyntaxError)
   }
   if ((ctype!=pctype)&&(!((pctype==CHAR_LETTER)&&(ctype==CHAR_NUM)))
   &&(!((pctype==CHAR_HEXNUM)&&(ctype==CHAR_NUM)))
   &&(!((stype==CHAR_HEXNUM)&&(ctype==CHAR_LETTER)))||
   ((pctype==CHAR_LBRACKET)&&(ctype==CHAR_LBRACKET))||
   ((pctype==CHAR_RBRACKET)&&(ctype==CHAR_RBRACKET))||
   ((pctype==CHAR_LSBRACKET)&&(ctype==CHAR_LSBRACKET))||
   ((pctype==CHAR_RSBRACKET)&&(ctype==CHAR_RSBRACKET))||
   ((pctype==CHAR_QUOT)&&(ctype==CHAR_QUOT)))
   {
    if (i>0)
    {
     bf[i]=0;
     switch (stype)
     {
      case CHAR_NUM:
       pom=atof(bf);
       break;
      case CHAR_LETTER:
       NAME *n;
       n=library->Find(bf);
       if (n==0) CExpr_SetErr(UndefinedName)
       if ((n->parattr&0x80000000)==0)
       {
        if (sref) CExpr_SetErr(SyntaxError)
        stack[++sl][0]=(unsigned int)n;
        stack[sl][1]=10000;
        funcstack[++funcsl][freq]=-1; //wymus uzycie nawiasu
        funcstack[funcsl][npar]=n->parattr&255;
        vcount-=funcstack[funcsl][npar]-1;
        funcstack[funcsl][partype]=n->partypes;
        funcstack[funcsl][parl]=0;
        funcstack[funcsl][bracketv]=bcount;
        for (unsigned int j=0; j<n->parattr&255; j++)
        {
         if (GET_PAR(n->partypes,j)==VAL_STR) isextracode=1;
        }
        if (n->tag&(TAG_EXTRACODE)) isextracode=1;
       }
       else
       {
        vcount++;
        if (sref)
        {
         onp[onpl][0]=ONP_NAMEREF;
         sref=0;
        }
        else
        onp[onpl][0]=ONP_NAME;
        onp[onpl++][1]=(unsigned int)n;
       }
       break;
      case CHAR_HEXNUM:
       pom=(float)htoi(bf);
       break;
      case CHAR_OTHER:
       int oi;
       oi=opindex(bf);
       if (oi==-1) CExpr_SetErr(UnreconOp)
       if ((vstart==1)&&(oi==1))
       {
        n=library->Find("CHS");
        if (n==0) CExpr_SetErr(UndefinedName)
        stack[++sl][0]=(unsigned int)n;
        stack[sl][1]=10000;
       }
       else
       {
        n=library->Find(CExpr_operands[oi].fname);
        if (n==0) CExpr_SetErr(UndefinedName)
        vcount-=1;
       
        //zdejmij operandy o wyzszym/rownym priorytecie
        while ((sl>=0)&&(stack[sl][1]>=CExpr_operands[oi].p))
        {
         onp[onpl][0]=ONP_NAME;
         onp[onpl++][1]=stack[sl--][0];
         if (onpl>=MAXONPBUFLEN) CExpr_SetErr(BufOverflow)
        }
        stack[++sl][0]=(unsigned int)n;
        stack[sl][1]=CExpr_operands[oi].p;
       }
       break;
      case CHAR_LSBRACKET:
       sbcount++;
       if (onpl>=1)
       {
        //if (onp[onpl-1][0]==ONP_NAME) onp[onpl-1][0]=ONP_NAMEADDR;
       } else CExpr_SetErr(SyntaxError)
      case CHAR_LBRACKET:
       if (funcsl>=0)
       {
        if (funcstack[funcsl][npar]>0)
        funcstack[funcsl][freq]=1;
        else
        funcstack[funcsl][freq]=2;
       }
       vstart=2;
       stack[++sl][1]=0;
       bcount++;
       break;
      case CHAR_RBRACKET:
      case CHAR_RSBRACKET:
       bcount--;
       if (bcount<0) CExpr_SetErr(SyntaxError)
       //zdejmij ze stosu wszystkie wartosci az do namiasu (
       while ((sl>=0)&&(stack[sl][1]>0))
       {
        onp[onpl][0]=ONP_NAME;
        onp[onpl++][1]=stack[sl--][0];
        if (onpl>=MAXONPBUFLEN) CExpr_SetErr(BufOverflow)
       }
       sl--;
       if (funcsl>=0)
       if (funcstack[funcsl][bracketv]==bcount)
       {
        if ((funcstack[funcsl][freq]!=2)&&(funcstack[funcsl][parl]!=funcstack[funcsl][npar]-1))
        CExpr_SetErr(TooFewParams)
        onp[onpl][0]=ONP_NAME;
        onp[onpl++][1]=stack[sl--][0];
        if (onpl>=MAXONPBUFLEN) CExpr_SetErr(BufOverflow)
        funcsl--;
       }
       if (stype==CHAR_RSBRACKET)
       {
        if (sbcount<=0) CExpr_SetErr(SyntaxError)
        sbcount--;
        n=library->Find("PTR");
        if (n==0) CExpr_SetErr(UndefinedName)
        onp[onpl][0]=ONP_NAME;
        onp[onpl++][1]=(int)n;
        vcount--;
       }
       else
       if (ctype==CHAR_LSBRACKET) CExpr_SetErr(SyntaxError)
       break;
      case CHAR_COMMA:
       if (funcsl==-1) CExpr_SetErr(SyntaxError)
       while ((sl>=0)&&(stack[sl][1]>0))
       {
        onp[onpl][0]=ONP_NAME;
        onp[onpl++][1]=stack[sl--][0];
        if (onpl>=MAXONPBUFLEN) CExpr_SetErr(BufOverflow)
       }
       sl--;
       vstart=2;
       stack[++sl][1]=0;
       funcstack[funcsl][parl]++;
       if (funcstack[funcsl][parl]>=funcstack[funcsl][npar])
       CExpr_SetErr(TooManyParams)
       break;
      case CHAR_QUOT:
       if (funcsl==-1) CExpr_SetErr(SyntaxError)
       vcount++;
       if (GET_PAR(funcstack[funcsl][partype],funcstack[funcsl][parl])!=VAL_STR)
       stron=2;
       else
       stron=1;
       strchar=*(expr-1);
       strbson=0;
       continue;
      case CHAR_REF:
       if (ctype!=CHAR_LETTER) CExpr_SetErr(SyntaxError)
       sref=1;
       break;
     }
     
     if ((stype==CHAR_NUM)||(stype==CHAR_HEXNUM))
     {
      vcount++;
      onp[onpl][0]=ONP_NUM;
      *(float*)((int)onp+onpl*8+4)=pom;
      onpl++;
     }
     if (
     ((stype==CHAR_NUM)||(stype==CHAR_HEXNUM))&&
     ((ctype==CHAR_LETTER)||(ctype==CHAR_HEXNUM)||(ctype==CHAR_LBRACKET))||
     ((stype==CHAR_RBRACKET)&&(ctype==CHAR_LBRACKET))
     )
     {
      int oi;
      oi=opindex("*");
      if (oi==-1) CExpr_SetErr(UnreconOp)
      NAME *n;
      n=library->Find(CExpr_operands[oi].fname);
      if (n==0) CExpr_SetErr(UndefinedName)
      vcount-=1;
       
      //zdejmij operandy o wyzszym/rownym priorytecie
      while ((sl>=0)&&(stack[sl][1]>=CExpr_operands[oi].p))
      {
       onp[onpl][0]=ONP_NAME;
       onp[onpl++][1]=stack[sl--][0];
       if (onpl>=MAXONPBUFLEN) CExpr_SetErr(BufOverflow)
      }
      stack[++sl][0]=(unsigned int)n;
      stack[sl][1]=CExpr_operands[oi].p;
     }
    }
    
    i=0;
    stype=ctype;
   }
   
   bf[i++]=c;
   if (i==EXPR_SET_BUFLEN) CExpr_SetErr(BufOverflow)
   pctype=ctype;
   if (vstart==2) vstart=1;
   else
   vstart=0;
  }
  if (onpl>=MAXONPBUFLEN) CExpr_SetErr(BufOverflow)
  if (c==0) break;
  expr++;
 }
 if (funcsl>=0)
 if (funcstack[funcsl][freq]!=0) CExpr_SetErr(SyntaxError)
 if ((bcount>0)||(sbcount>0)) CExpr_SetErr(SyntaxError)
 if (vcount!=1) CExpr_SetErr(SyntaxError)
 while (sl>=0)
 {
  onp[onpl][0]=ONP_NAME;
  onp[onpl++][1]=stack[sl--][0];
  if (onpl>=MAXONPBUFLEN) CExpr_SetErr(BufOverflow)
 }
 strcpy(retstr,CExpr_ErrorNames[NoErr]);
 strcpy(exprstr,exprbuf);
#ifdef DC
 //kompiluj na kod maszynowy dane wyrazenie
 if (do_code==0) do_code=(unsigned char *)CSys::AllocMem(MAXCODELEN);
 unsigned char *codeptr=do_code;
 struct {NAME *n; unsigned int pc;} sf[MAXONPBUFLEN];
 int cf=-1;
 int espl=0;
 int callf;
 onpsl=-1;
 NAME *n;
 if (isextracode)
 {
  CODE_SETB(codeptr,0xA1);                               //mov eax,CExpr::cexpr
  CODE_SETD(codeptr,&CExpr::cexpr);
  CODE_SETB(codeptr,0xA3);                               //mov pcexpr,eax
  CODE_SETD(codeptr,&pcexpr);
  CODE_SETW(codeptr,0x05C7);                             //mov CExpr::cexpr,this
  CODE_SETD(codeptr,&CExpr::cexpr);
  CODE_SETD(codeptr,this);
  CODE_SETW(codeptr,0x05C7);                             //mov strcount,0
  CODE_SETD(codeptr,&strcount);
  CODE_SETD(codeptr,0);
 }
 //6*4+2*2+2=30
 
 for (int i=onpl-1; i>=0; i--)
 {
  callf=0;
  switch (onp[i][0])
  {
   case ONP_NUM:
    onpstack[++onpsl][0]=VAL_FLOAT;
    //push v
    espl++;
    CODE_SETWB(codeptr,0x2444C7);                        //mov d,[esp-$espl*4],onp[i][1]
    CODE_SETB(codeptr,-espl*4);
    CODE_SETD(codeptr,onp[i][1]);
    callf=1;
    break;
   case ONP_INUM:
    onpstack[++onpsl][0]=VAL_STR+10;
    //push v
    espl++;
    CODE_SETWB(codeptr,0x2444C7);                        //mov d,[esp-$espl*4],onp[i][1]
    CODE_SETB(codeptr,-espl*4);
    CODE_SETD(codeptr,onp[i][1]);
    callf=1;
    break;
   case ONP_NAMEREF:
    n=(NAME*)onp[i][1];
    onpstack[++onpsl][0]=VAL_PTR;
    espl++;
    CODE_SETWB(codeptr,0x2444C7);                        //mov d,[esp-$espl*4],n->fptr
    CODE_SETB(codeptr,-espl*4);
    CODE_SETD(codeptr,n->fptr);
    callf=1;
    break;
   case ONP_NAME:
    n=(NAME*)onp[i][1];
    int npa=n->parattr;
    if (npa&0x80000000)
    //zmienna
    {
     onpsl++;
     onpstack[onpsl][0]=(npa>>8)&255;
     //push v
     espl++;
     CODE_SETB(codeptr,0xA1);                            //mov eax,[n->fptr]
     CODE_SETD(codeptr,n->fptr);
     CODE_SETWB(codeptr,0x244489);                       //mov [esp-$espl*4],eax
     CODE_SETB(codeptr,-espl*4);
     callf=1;
    }
    else
    //funkcja
    {
     if ((n->parattr&255)==0)
     //funkcja bezargumentowa
     {
      if (espl>0)
      {
       CODE_SETW(codeptr,0xEC83);                        //sub esp,$espl*4
       CODE_SETB(codeptr,espl*4);
      }
      espl=1;
      if (isextracode)
      {
       CODE_SETW(codeptr,0x05C7);                        //mov tag,n->tag
       CODE_SETD(codeptr,&tag);
       CODE_SETD(codeptr,n->tag);
      }
      CODE_SETB(codeptr,0xE8);                           //call n->fptr
      CODE_SETD(codeptr,(int)n->fptr-(int)codeptr-4);
      int rt=(n->parattr>>8)&3;
      onpsl++;
      if (rt==VAL_FLOAT)
      {
       CODE_SETWB(codeptr,0x245CD9);                     //fstp d,[esp-espl*4]
       CODE_SETB(codeptr,-espl*4);
       onpstack[onpsl][0]=VAL_FLOAT;
      }
      else
      {
       CODE_SETWB(codeptr,0x244489);                     //mov [esp-$espl*4],eax
       CODE_SETB(codeptr,-espl*4);
       if (rt==VAL_INT)
       {
        CODE_SETWB(codeptr,0x2444DB);                    //fild d,[esp-$espl*4]
        CODE_SETB(codeptr,-espl*4);
        CODE_SETWB(codeptr,0x245CD9);                    //fstp d,[esp-espl*4]
        CODE_SETB(codeptr,-espl*4);
        onpstack[onpsl][0]=VAL_FLOAT;
       }
       else
       {
        onpstack[onpsl][0]=VAL_INT;
       }
      }
      callf=1;
     }
     else
     //funkcja wieloargumentowa
     {
      sf[++cf].n=n;
      sf[cf].pc=0;
     }
    }
    break;
  }
  if ((cf>=0)&&(callf))
  {
   sf[cf].pc++;
   while (sf[cf].pc==(sf[cf].n->parattr&255))
   {
    int pc=sf[cf].n->parattr&255;
    int stradd=0;
    for (int j=0; j<pc; j++)
    {
     int pt=GET_PAR(sf[cf].n->partypes,j);
     int st=onpstack[onpsl-j][0];
     if (pt!=st)
     {
      if (st==VAL_STR+10) stradd++;
      if ((pt==VAL_FLOAT)&&(st!=VAL_FLOAT))
      {
       CODE_SETWB(codeptr,0x2444DB);                     //fild d,[esp-($espl-$j)*4]
       CODE_SETB(codeptr,-(espl-j)*4);
       CODE_SETWB(codeptr,0x245CD9);                     //fstp d,[esp-($espl-$j)*4]
       CODE_SETB(codeptr,-(espl-j)*4);
      }
      else
      if ((pt!=VAL_FLOAT)&&(st==VAL_FLOAT))
      {
       CODE_SETWB(codeptr,0x2444D9);                     //fld d,[esp-($espl-$j)*4]
       CODE_SETB(codeptr,-(espl-j)*4);
       CODE_SETWB(codeptr,0x245CDB);                     //fistp d,[esp-($espl-$j)*4]
       CODE_SETB(codeptr,-(espl-j)*4);
      }
     }
    }
    if (espl>0)
    {
     CODE_SETW(codeptr,0xEC83);                          //sub esp,$espl*4
     CODE_SETB(codeptr,espl*4);
    }
    if (isextracode)
    {
     CODE_SETW(codeptr,0x05C7);                          //mov tag,n->tag
     CODE_SETD(codeptr,&tag);
     CODE_SETD(codeptr,sf[cf].n->tag);
    }
    CODE_SETB(codeptr,0xE8);                             //call sf[cf].n->fptr
    CODE_SETD(codeptr,(int)sf[cf].n->fptr-(int)codeptr-4);
    if (pc!=1)
    {
     CODE_SETW(codeptr,0xC483);                          //add esp,($pc-1)*4
     CODE_SETB(codeptr,(pc-1)*4);
    }
    if (stradd)
    {
     CODE_SETW(codeptr,0x0581);                          //add strcount,stradd
     CODE_SETD(codeptr,&strcount);
     CODE_SETD(codeptr,stradd);
    }
    int rt=(sf[cf].n->parattr>>8)&3;
    onpsl-=pc-1;
    if (rt==VAL_FLOAT)
    {
     CODE_SETWB(codeptr,0x241CD9);                       //fstp d,[esp]
     onpstack[onpsl][0]=VAL_FLOAT;
    }
    else
    {
     CODE_SETWB(codeptr,0x240489);                       //mov [esp],eax
     if (rt==VAL_INT)
     {
      CODE_SETWB(codeptr,0x2404DB);                      //fild d,[esp]
      CODE_SETWB(codeptr,0x241CD9);                      //fstp d,[esp]
      onpstack[onpsl][0]=VAL_FLOAT;
     }
     else
     {
      onpstack[onpsl][0]=VAL_INT;
     }
    }
    espl=0;
    cf--;
    if (cf>=0)
    sf[cf].pc++;
    else break;
   }
  }
 }
 if (onpstack[onpsl][0]!=VAL_FLOAT)
 {
  CODE_SETWB(codeptr,0x2444DB);                          //fild d,[esp-$espl*4]
  CODE_SETB(codeptr,-espl*4);
  CODE_SETWB(codeptr,0x245CD9);                          //fstp d,[esp-$espl*4]
  CODE_SETB(codeptr,-espl*4);
 }
 if ((*(codeptr-1)==0x24)&&(*(codeptr-3)==0xD9)) codeptr-=3;
 else
 if ((*(codeptr-2)==0x24)&&(*(codeptr-4)==0xD9)) codeptr-=4;
 else
 {
  if (espl>0)
  {
   CODE_SETWB(codeptr,0x2444D9);                         //fld d,[esp-$espl*4]
   CODE_SETB(codeptr,-espl*4);
  }
  else
  {
   CODE_SETWB(codeptr,0x2404D9);                         //fld d,[esp]
  }
 }
 if (isextracode)
 {
  CODE_SETB(codeptr,0xA1);                               //mov eax,pcexpr
  CODE_SETD(codeptr,&pcexpr);
  CODE_SETB(codeptr,0xA3);                               //mov CExpr::cexpr,eax
  CODE_SETD(codeptr,&CExpr::cexpr);
 }
 if (espl==0)
 {
  if ((*(codeptr-1)==0x04)&&(*(codeptr-2)==0xC4)&&(*(codeptr-3)==0x83)) 
  {
   codeptr-=3;
   CODE_SETWB(codeptr,0x08C483);                         //add esp,8
  }
  else
  {
   CODE_SETWB(codeptr,0x04C483);                         //add esp,4
  }
 }
 CODE_SETB(codeptr,0xC3);                                //ret
 
 Do=(float(*)())do_code;
 
 /*FILE *f;
 f=fopen("a.x","ab");
 for (int i=0; i<codeptr-do_code; i++) fputc(do_code[i],f);
 fclose(f);*/
#endif
 return NoErr;
}
#ifndef DC
float CExpr::Do()
{
 pcexpr=CExpr::cexpr;
 CExpr::cexpr=this;
 onpsl=-1;
 NAME *n;
 strcount=0;
 for (int i=0; i<onpl; i++)
 {
  switch (onp[i][0])
  {
   case ONP_NUM:
    onpstack[++onpsl][0]=VAL_FLOAT;
    onpstack[onpsl][1]=onp[i][1];
    break;
   case ONP_NAME:
    n=(NAME*)onp[i][1];
    int npa=n->parattr;
    if (npa&0x80000000)
    //zmienna
    {
     onpsl++;
     switch ((npa>>8)&255)
     {
      case VAL_FLOAT:
       onpstack[onpsl][0]=VAL_FLOAT;
       *(float*)((int)onpstack+onpsl*8+4)=*(float*)(n->fptr);
       break;
      case VAL_INT:
       onpstack[onpsl][0]=VAL_FLOAT;
       *(float*)((int)onpstack+onpsl*8+4)=(float)*(unsigned int*)(n->fptr);
       break;
      case VAL_STR: //VAL_STR=VAL_PTR
       onpstack[onpsl][0]=VAL_STR;
       onpstack[onpsl][1]=*(unsigned int*)(n->fptr);
       break;
      default: return 0;
     }
    }
    else
    //funkcja
    {
     int pc=npa&255;
     int rt=(npa>>8)&3;
     int stradd=0;
     int si=onpsl-pc+1;
     for (int j=0; j<pc; j++)
     {
      int pt=GET_PAR(n->partypes,j);
      int st=onpstack[si][0];
      if (pt!=st)
      {
       if (st==VAL_STR+10) stradd++;
       if ((pt==VAL_FLOAT)&&(st!=VAL_FLOAT))
       {
        onpstack[si][0]=VAL_FLOAT;
        *(float*)((int)onpstack+si*8+4)=(float)onpstack[si][1];
       }
       else
       if ((pt!=VAL_FLOAT)&&(st==VAL_FLOAT))
       {
        onpstack[si][0]=VAL_INT;
        onpstack[si][1]=(unsigned int)*(float*)((int)onpstack+si*8+4);
       }
      }
      si++;
     }
     tag=n->tag;
     if (rt==VAL_FLOAT)
     {
      memcpy(&faddr_f,&n->fptr,sizeof(int));
      calltab[pc][0](this);
      onpstack[onpsl][0]=VAL_FLOAT;
     }
     else
     {
      memcpy(&faddr_i,&n->fptr,sizeof(int));
      calltab[pc][1](this);
      if (rt==VAL_INT)
      {
       onpstack[onpsl][0]=VAL_FLOAT;
       *(float*)((int)onpstack+onpsl*8+4)=(float)((int)onpstack[onpsl][1]);
      }
      else
      {
       onpstack[onpsl][0]=VAL_INT;
      }
     }
     strcount+=stradd;
    }
    break;
   case ONP_INUM:
    onpstack[++onpsl][0]=VAL_PTR+10;
    onpstack[onpsl][1]=onp[i][1];
    break;
   case ONP_NAMEREF:
    n=(NAME*)onp[i][1];
    onpstack[++onpsl][0]=VAL_PTR;
    onpstack[onpsl][1]=(unsigned int)(n->fptr);
    break;
  }
 }
 CExpr::cexpr=pcexpr;
 //printf("%d %x\n",onpsl,onpstack[0][1]);
 return *(float*)((int)onpstack+4);
}
#endif
