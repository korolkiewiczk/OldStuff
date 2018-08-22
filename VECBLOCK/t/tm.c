#include <stdlib.h>
#include "vb_mem.c"
#include "asm\xstub32.h"

int main()
{
 _xstub32init();
 int *wsk,*dwsk[4];
 if (InitMem(65536,32768)==0) return 0;
 printf ("sw=%d,dw=%d,st=%d,dt=%d\n",mem_sheapwsk,mem_dheapwsk,mem_sheaptop,mem_dheaptop);
 printf ("SAlloc\n");
 wsk=Mem_SAlloc (1234);
 printf ("sw=%d,w=%d\n",mem_sheapwsk,wsk);
 strcpy ((char *)wsk,"dfdffdsfgfgjgkfgkkffffffffffffjjjjjjjjjjjrt");
 wsk=Mem_SAlloc (125534);
 printf ("sw=%d,w=%d\n",mem_sheapwsk,wsk);
 printf ("DAlloc\n");
 Mem_DAlloc((void *)&dwsk[0],3000);
 printf ("dw=%d\n",mem_dheapwsk);
 Mem_DAlloc((void *)&dwsk[1],4000);
 printf ("dw=%d\n",mem_dheapwsk);
 Mem_DAlloc((void *)&dwsk[2],6000);
 printf ("dw=%d\n",mem_dheapwsk);
 Mem_DAlloc((void *)&dwsk[3],15000);
 strcpy ((char *)dwsk[3],"kdjfkdfdkfkdsfkdsfkgkfkgfd");
 printf ("dw=%d\n",mem_dheapwsk);
 printf ("w1=%d,w2=%d,w3=%d,w4=%d\n",dwsk[0],dwsk[1],dwsk[2],dwsk[3]);
 printf ("w1=%d,w2=%d,w3=%d,w4=%d\n",mem_buf[0][1],mem_buf[1][1],mem_buf[2][1],mem_buf[3][1]);
 printf ("&w1=%d,&w2=%d,&w3=%d,&w4=%d\n",mem_buf[0][0],mem_buf[1][0],mem_buf[2][0],mem_buf[3][0]);
 printf ("Free\n");
 Mem_DFree((void *)&dwsk[2]);
 printf ("w1=%d,w2=%d,w3=%d,w4=%d\n",dwsk[0],dwsk[1],dwsk[2],dwsk[3]);
 printf ("dw=%d\n",mem_dheapwsk);
 printf ("%s\n",dwsk[3]);
 Mem_DFree((void *)&dwsk[3]);
 printf ("w1=%d,w2=%d,w3=%d,w4=%d\n",dwsk[0],dwsk[1],dwsk[2],dwsk[3]);
 printf ("dw=%d\n",mem_dheapwsk);
 Mem_DFree((void *)&dwsk[0]);
 printf ("w1=%d,w2=%d,w3=%d,w4=%d\n",dwsk[0],dwsk[1],dwsk[2],dwsk[3]);
 printf ("dw=%d\n",mem_dheapwsk);
 Mem_DFree((void *)&dwsk[1]);
 printf ("w1=%d,w2=%d,w3=%d,w4=%d\n",dwsk[0],dwsk[1],dwsk[2],dwsk[3]);
 printf ("dw=%d\n",mem_dheapwsk);
 CloseMem();
}
