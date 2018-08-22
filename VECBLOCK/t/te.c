#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "asm\mcon.h"
#include "vb_con.c"
#include "vb_mem.c"
#include "vb_expr.c"

/*char w1[]="5*(2+3)/4-1";
char w2[]="1*2*3*4-10/5";
char w3[]="(2*(2)())+1";*/
char txt[1024];

int main()
{
 char onp[1024];
 _xstub32init();
 InitMem(1024*1024,0);            //1MB
 InitConsole();
 InitExpr ();
 Expr_IncludeLib (stdf,STDF_MAX);
 float wyn;
 char c;
 unsigned short p;
 while (1)
 {
  p=MReadKey();
  c=p;
  p=p>>8;
  switch (p)
  {
   case 72: Con_PrevCommand(); break;
   case 80: Con_NextCommand(); break;
   case 77: Con_MoveCursor(1); break;
   case 75: Con_MoveCursor(-1); break;
   case 73: Con_GoUp(); break;
   case 81: Con_GoDown(); break;
  }
  if (p==83) c=CON_DELKEY;
  if (c==27) break;
  int w=Con_WriteChar (c);
  if (w==3) Con_Printf ("con_wsk=%i",con_wsk);
  if (w==2)
  {
   Con_PrevCommand();
   wyn=Expr_Comp(con_str);
   if (Expr_CheckErr(txt)!=EXPR_OK) Con_Print (txt);
   else
   {
    Expr_ONP (onp,con_str);
    Con_Printf ("onp=%s",onp);
    Con_Printf ("%s=%f",con_str,wyn);
   }
   Con_EnterCmd();
  }
  MCls();
  Con_Draw();
 }
 /*float w;
 while (1)
 {
  scanf ("%s",txt);
  if (txt[0]=='@') break;
  Expr_ONP (onp,txt);
  w=Expr_Comp (txt);
  if (Expr_CheckErr(txt)!=EXPR_OK)
  {
   printf ("Blad: %s\n",txt);
  }
  else
  printf ("onp:%s\n%s=%f\n",onp,txt,w);
 }
 /*
 if (Expr_ONP (onp,w1)==0) return 0;
 printf ("%s\n%s\n%f\n",w1,onp,Expr_Comp (w1));
 if (Expr_ONP (onp,w2)==0) return 0;
 printf ("%s\n%s\n%f\n",w2,onp,Expr_Comp (w2));
 if (Expr_ONP (onp,w3)==0) return 0;
 printf ("%s\n%s\n%f\n",w2,onp,Expr_Comp (w3));*/
 CloseMem();
}
