#define STDPRINT
#include "vbengine.h"

int a,w,f,g,h,i,j,k;
float b;
double c,d,e;

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
 VR_AddName("d",VR_DOUBLE,&d);
 VR_AddName("e",VR_DOUBLE,&e);
 VR_AddName("f",VR_INT,&f);
 VR_AddName("g",VR_INT,&g);
 VR_AddName("h",VR_INT,&h);
 VR_AddName("i",VR_INT,&i);
 VR_AddName("j",VR_INT,&j);
 VR_AddName("k",VR_INT,&k);
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
 w=Cfg_Exec("a.cfg");
 if (w==0)
 {
  printf ("a=%d\nb=%f\nc=%f\nd=%f\ne=%f\nf=%d\ng=%d\nh=%d\ni=%d\nj=%d\nk=%d\n",a,b,c,d,e,f,g,h,i,j,k);
 }
 else printf ("%d",w);
 CloseMem();
}
