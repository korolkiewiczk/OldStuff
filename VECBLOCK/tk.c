#include "vbengine.h"

TTime t;

int __(int _) {return _;}

int main()
{
 _xstub32init();
 int c;
 InitTime();
 printf ("%D",__cpu__tsc__freq);
 InitKeySys();
 Time_InitStruct(&t,TIME_FLAGS_SEC_TIME);
 Key_Bind(K_UP,K_DOWN);
 while (1)
 {
  if (Key_IsP(1)==1) break;
  if (Key_CGet(K_ENTER)==1) Key_Defaults();
  if (Key_CGet(K_BKSPACE)==1) Key_Restore();
  for (c=0; c<128; c++) if (Key_CGet(c)==1) printf ("%02d %c %s\n",c,Key_Key2Char(c),key_names[c-1]);
 }
 __(0x21436587);
 printf ("%f",Time_Get(&t));
 CloseKeySys();
}
