/*
  BIBLIOTEKA STANOWI PRZYKLAD
  NA JEJ PODSTAWIE MOZNA TWORZYC KOLEJNE FUNKCJE DODAJAC JE DO STRUKTURY DLL_LIB
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <e32def.h>

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
 {"template_sample",(void*)sample,VAL_FLOAT,1,VAL_FLOAT,0}
};


/*
  FUNKCJA REFERENCYJNA
*/


EXPORT_C int RefFunc(void **lib,ENV *env)
{
 memcpy(&expr_env,(void*)env,sizeof(ENV));
 *lib=dll_lib;
 return sizeof(dll_lib)/sizeof(ELEMENT);
}
