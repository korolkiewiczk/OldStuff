/*
  BIBLIOTEKA STANOWI PRZYKLAD
  NA JEJ PODSTAWIE MOZNA TWORZYC KOLEJNE FUNKCJE DODAJAC JE DO STRUKTURY DLL_LIB
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "clib.h"
#include "env.h"

ENV expr_env;

/*
  FUNKCJE
*/

float sample(float a)
{
 return expr_env.CSys_GetTime();
}

/*
  BIBLIOTEKA FUNKCJI
*/

ELEMENT dll_lib[]=
{
 {"sample",(void*)sample,VAL_FLOAT,1,VAL_FLOAT,0}
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
