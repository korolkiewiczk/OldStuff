//interpolacja wielomianowa

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "clib.h"
#include "env.h"
#include "debug.c"

ENV expr_env;

#define MAXF 32  //maksymalna liczba funckji
#define MAXN 128 //maksymalna liczba wezlow
float newton_tab[MAXF][MAXN][3]; //wezly+ilorazy roznicowe
int newton_tab_n[MAXF];          //liczba wezlow

float newton_tab2[MAXF][MAXN][3]; //wezly+ilorazy roznicowe
int newton_tab2_n[MAXF];          //liczba wezlow

/*
  FUNKCJE
*/

//ustala wezly f(xi)=yi
int set(int no,char *s_x,char *s_y)
{
 no%=MAXF;
 //x
 int i=0;
 while (*s_x==' ') s_x++;
 while (*s_x)
 {
  sscanf(s_x,"%f",&newton_tab[no][i++][0]);
  while ((*s_x)&&(*s_x!=' ')) s_x++;
  while (*s_x==' ') s_x++;
  i%=MAXN;
 }
 
 //y
 int j=0;
 while (*s_y==' ') s_y++;
 while (*s_y)
 {
  sscanf(s_y,"%f",&newton_tab[no][j++][1]);
  while ((*s_y)&&(*s_y!=' ')) s_y++;
  while (*s_y==' ') s_y++;
  j%=MAXN;
 }
 
 if (i>j) i=j;
 newton_tab_n[no]=i;
 int n=i;
 
 //tworz ilorazy roznicowe
 for (i=0; i<n; i++) newton_tab[no][i][2]=newton_tab[no][i][1];
 
 for (j=1; j<n; j++)
 {
  for (i=n-1; i>=j; i--)
  {
   newton_tab[no][i][2]=
   (newton_tab[no][i-1][2]-newton_tab[no][i][2])/
   (newton_tab[no][i-j][0]-newton_tab[no][i][0]);
  }
 }
 return 1;
}

//ustala wezly f(xi)=yi, odwrotna kolejnosc
int set2(int no,char *s_x,char *s_y)
{
 no%=MAXF;
 //x
 int i=0;
 while (*s_x==' ') s_x++;
 while (*s_x)
 {
  sscanf(s_x,"%f",&newton_tab2[no][i++][0]);
  while ((*s_x)&&(*s_x!=' ')) s_x++;
  while (*s_x==' ') s_x++;
  i%=MAXN;
 }
 
 //y
 int j=0;
 while (*s_y==' ') s_y++;
 while (*s_y)
 {
  sscanf(s_y,"%f",&newton_tab2[no][j++][1]);
  while ((*s_y)&&(*s_y!=' ')) s_y++;
  while (*s_y==' ') s_y++;
  j%=MAXN;
 }
 
 if (i>j) i=j;
 newton_tab2_n[no]=i;
 int n=i;
 
 //tworz ilorazy roznicowe
 for (i=0; i<n; i++) newton_tab2[no][i][2]=newton_tab2[no][i][1];
 
 for (j=1; j<n; j++)
 {
  for (i=0; i<n-j; i++)
  {
   newton_tab2[no][i][2]=
   (newton_tab2[no][i][2]-newton_tab2[no][i+1][2])/
   (newton_tab2[no][i][0]-newton_tab2[no][i+j][0]);
  }
 }
 return 1;
}

float newton(int no,float x)
{
/* float s=0;
 float x0=1;
 for (int i=0; i<newton_tab_n[no]; i++)
 {
  s+=newton_tab[no][i][2]*x0;
  x0*=(x-newton_tab[no][i][0]);
 }
 return s;*/
 
 float s=newton_tab[no][newton_tab_n[no]-1][2];
 for (int i=newton_tab_n[no]-2; i>=0; i--)
 {
  s=s*(x-newton_tab[no][i][0])+newton_tab[no][i][2];
 }
 return s;
}

float newton2(int no,float x)
{
 float s=newton_tab2[no][0][2];
 for (int i=1; i<newton_tab2_n[no]; i++)
 {
  s=s*(x-newton_tab2[no][i][0])+newton_tab2[no][i][2];
 }
 return s;
}

/*
  BIBLIOTEKA FUNKCJI
*/

ELEMENT dll_lib[]=
{
 {"ip_set",(void*)set,VAL_INT,3,VAL_INT+VAL_STR*4+VAL_STR*16,0},
 {"ip_pol",(void*)newton,VAL_FLOAT,2,VAL_INT+VAL_FLOAT*4,0},
 {"ip_set2",(void*)set2,VAL_INT,3,VAL_INT+VAL_STR*4+VAL_STR*16,0},
 {"ip_pol2",(void*)newton2,VAL_FLOAT,2,VAL_INT+VAL_FLOAT*4,0}
};


/*
  FUNKCJA REFERENCYJNA
*/

extern "C"
{
 __declspec (dllexport) int RefFunc(void **lib,ENV *env)
 {
  memcpy(&expr_env,(void*)env,sizeof(ENV));
  *lib=dll_lib;
  return sizeof(dll_lib)/sizeof(ELEMENT);
 }
}
