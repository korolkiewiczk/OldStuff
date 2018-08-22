#include "vbengine.h"

void exitp()
{
}

TCF cf;
TFnt *fnt;
TSurf fntsurf={5,5,0};

int main()
{
 int x=200,y=0;
 _xstub32init();
 InitMem(65536*5,65536*5);
 CF_Load(&cf,"fnt.cf");
 InitKeySys();
 InitVid();
 Vid_SetMode(0);
 fnt=Surf_LoadFnt(5,5,1,&cf,"fnt(5x5).bin");
 Surf_Alloc(&fntsurf);
 int i;
 for (i=0; i<5*5; i++) *(fntsurf.sbits+i)=(i/5)*2+22;
 while(1)
 {
  memset(vid_buf.sbits,0,vid_buf.swidth*vid_buf.sheight);
  if (Key_IsP(1)) break;
  if (Key_IsP(K_UP)) y--;
  if (Key_IsP(K_DOWN)) y++;
  if (Key_IsP(K_RIGHT)) x++;
  if (Key_IsP(K_LEFT)) x--;
  //memset(key_keytab,0,256);
  Surf_Print(x,y,(int)&fntsurf,fnt,&vid_buf,"Abcd123456789");
  Vid_Swap();
 }
 CloseKeySys();
 Vid_BackTXT();
}
