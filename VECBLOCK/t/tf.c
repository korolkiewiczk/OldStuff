#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "asm\mcon.h"
#include "vb_con.c"
#include "vb_mem.c"
#include "vb_expr.c"
#include "asm\_13h.h"

TFuncVar x={0,0,0,0};
float hwidth=20;

void hline(int x,int y1,int y2,char c)
{
 int s,i;
 if ((y1>199)||(y2<0)) return;
 if (y1<0)y1=0;
 if (y2>199)y2=199;
 if (y1>y2) {i=y2; y2=y1; y1=i;}
 i=(y1<<6)+(y1<<8)+x;
 while (y1<=y2)
 {
  byte0a0000h(i,c);
  i+=320;
  y1++;
 }
}

int main()
{
 char onp[1024];
 _xstub32init();
 InitMem(1024*1024,0);            //1MB
 InitExpr ();
 Expr_IncludeLib (stdf,STDF_MAX);
 Expr_AddFV("x",&x);
 InitConsole();
 float wyn;
 float y,oy;
 int iy,ioy;
 int i;
 unsigned char c;
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
   x.value=0;
   wyn=Expr_Comp(con_str);
   if (Expr_CheckErr(con_str)!=EXPR_OK) Con_Print (con_str);
   else
   {
    Expr_ONP (onp,con_str);
    Con_Printf ("onp=%s",onp);
    Con_Printf ("%s=%f",con_str,wyn);
    set13h();
    x.value=(hwidth/320.0f)*(-161);
    oy=Expr_Comp (con_str);
    for (i=0; i<200; i++)
    {
     if (((i-100)%(200/(int)hwidth))==0)
     byte0a0000h(160+(i*320),15);
     else
     byte0a0000h(160+(i*320),1);
    }
    for (i=0; i<320; i++)
    {
     if (((i-160)%(320/(int)hwidth))==0)
     byte0a0000h(i+100*320,15);
     else
     byte0a0000h(i+100*320,1);
     x.value=(hwidth/320.0f)*(i-160);
     y=Expr_Comp (con_str);
     iy=100.0f-y*(200.0f/hwidth);
     ioy=100.0f-oy*(200.0f/hwidth);
     hline (i,ioy,iy,4);
     oy=y;
    }
    MReadKey();
    set3h();
   }
   Con_EnterCmd();
  }
  MCls();
  Con_Draw();
 }
 CloseMem();
}
