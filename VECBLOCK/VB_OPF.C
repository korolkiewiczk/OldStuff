#ifndef __VB_OPF_C
#define __VB_OPF_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_OPF.C
 Opis: Funkcje dla operandow
------------------------------------------------
*/

//===========================================

//znak '+'
float Op_Add (float a,float b) {return a+b;}
//znak '-'
float Op_Sub (float a,float b) {return b-a;}
//znak '*'
float Op_Mul (float a,float b) {return a*b;}
//znak '/'
float Op_Div (float a,float b) {return b/a;}
//znak '^'
float Op_Pow (float a,float b) {return pow(b,a);}
//znak '!'
float Op_Sil (float a)
{
 int i,n=a,w=1;
 for (i=1;i<=n;i++) w*=i;
 return w;
}
//znak '%'
float Op_Mod (float a,float b) {return fmod(b,a);}

#endif
