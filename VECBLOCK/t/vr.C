#include "vbengine.h"

int a;
float b;
double c;

int main()
{
 char onp[1024],err[1024];
 _xstub32init();
 InitMem(1024*1024,1024*1024);
 InitVR();
 InitExpr();
 VR_AddName("a",VR_INT,&a);
 VR_AddName("b",VR_FLOAT,&b);
 VR_AddName("c",VR_DOUBLE,&c);
 VR_Set_i("a",10);
 VR_Set_f("b",10.6);
 VR_Set_d("c",0.5e2);
 printf ("%d,%f,%f\n",a,b,c);
 printf ("%d,%f,%f\n",VR_Get_i("a"),VR_Get_f("b"),VR_Get_d("c"));
 #define str "$a$b+2"
 Expr_ONP(onp,str);
 if (Expr_CheckErr(err)==EXPR_OK)
 printf ("%s\n%f\n",onp,Expr_Comp(str));
 else printf (err);
 CloseMem();
}
