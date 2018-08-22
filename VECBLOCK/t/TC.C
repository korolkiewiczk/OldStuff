#include <stdio.h>
#include <stdarg.h>
#include "vb_con.c"
#include "asm\xstub32.h"
#include "asm\mcon.h"
#include "vb_mem.c"

int __(int _) {return _;}
char *wsk,*wsk2,*wsk3;

int main()
{
 _xstub32init();
 if (InitMem (1024*1024)==0) return 0;
 if (Mem_Alloc ((void *)&wsk,1000)==0) return 0;
 if (Mem_Alloc ((void *)&wsk2,1000)==0) return 0;
 if (Mem_Alloc ((void *)&wsk3,100000)==0) return 0;
 strcpy (wsk3,"kamil");
 printf ("%s\n",wsk3);
 //printf ("%X,%X,%X,%X,%X,%X\n",wsk,wsk2,wsk3,mem_buf[0][1],mem_buf[1][1],mem_buf[2][1]);
 //printf ("%X,%X,%X,%d\n",mem_buf[0][0],mem_buf[1][0],mem_buf[2][0],mem_wsk);
 if (Mem_Free((void *)&wsk)==0) return 0;
 printf ("%s\n",wsk3);
 strcpy (wsk2,"kamil232342434353455434");
 //printf ("%X,%X,%X,%X,%X,%X\n",wsk,wsk2,wsk3,mem_buf[0][1],mem_buf[1][1],mem_buf[2][1]);
 if (Mem_Free((void *)&wsk3)==0) return 0;
 printf ("%s\n",wsk2);
 //printf ("%X,%X,%X,%X,%X,%X\n",wsk,wsk2,wsk3,mem_buf[0][1],mem_buf[1][1],mem_buf[2][1]);
 if (Mem_Free((void *)&wsk2)==0) return 0;
 //printf ("%X,%X,%X,%X,%X,%X\n",wsk,wsk2,wsk3,mem_buf[0][1],mem_buf[1][1],mem_buf[2][1]);
 if (CloseMem ()==0) return 0;
 
 printf ("%s\n",cpu_name);
 unsigned char c;
 unsigned short p;
 __(0x21436587);
 InitConsole();
 while (1)
 {
  p=MReadKey();
  c=p;
  p=p>>8;
  //printf ("%i,%i ",c,p);
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
  if (w==2) {Con_PrevCommand(); Con_Printf ("%i:%s",con_wsk,con_str); Con_EnterCmd();}
  MCls();
  Con_Draw();
 }
 printf ("%i",con_wsk);
}
