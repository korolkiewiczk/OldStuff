#include "vbengine.h"

int abc=10;
int br=0;
int fnt;
TSurf *altsurf;
TSurf *fntsurf;
TCF cf;

void exitp()
{
 Vid_BackTXT();
 CloseKeySys();
 printf ("%d",cf.f);
}

void pp()
{
 if (con_key!=0)
 {
  Con_Printf("%d %c %s",con_key,Key_Key2Char(con_key),key_names[con_key-1]);
  {Surf_Printf(100,0,Vid_FindRGB(23,255,255),0,&vid_buf,"%d,%d,%d",menu_font_alt->ctab['a'],menu_font_alt->ctab['b'],menu_font_alt->ctab[1]);}
 }
}

int main()
{
 _xstub32init();
 InitMem(65536*10,65536*40);
 CF_Load (&cf,"p.cf");
 if (Menu_Load(&cf,"../menu.txt",160,"bm1.bmp","bm2.bmp",30,"../ok.bmp","../no.bmp")==0) exit(0);
 Vid_LoadPalFromBMP(&cf,"bm1.bmp");
 /*TMenuItem *mi;
 int i;
 for (i=0;i<menu_nsections;i++)
 {
  printf ("%s,%x,%d,%d\n",menu_stab[i].sname,menu_stab[i].itaddr,menu_stab[i].spos,menu_stab[i].nspos);
  mi=menu_stab[i].itaddr;
  while (mi!=NULL)
  {
   printf ("%s\n",mi->txt);
   mi=mi->next;
  }
 }*/
 atexit(exitp);
 InitVid();
 Vid_SetMode(0);
 InitKeySys();
 InitExpr();
 InitVR();
 InitTime();
 InitConsole(0,Vid_FindRGB(0,0,0),&cf,"conbmp.bmp",1.0,0.1);
 InitLoopSys(0,pp);
 VR_New("abc",VR_INT,&abc);
 VR_New("gt",VR_DOUBLE,&globaltime);
 VR_New("br",VR_INT,&br);
 Menu_VidApply();
 if (!Surf_Create((void*)&altsurf,5,5)) return;
 if (!Surf_Create((void*)&fntsurf,8,8)) return;
 int i,j;
 for (i=0;i<5;i++)
  for (j=0;j<5;j++)
   *(altsurf->sbits+i*5+j)=Vid_FindRGB(i*10+196,i*10+196,i*10+196);
 for (i=0;i<8;i++)
  for (j=0;j<8;j++)
   *(fntsurf->sbits+i*8+j)=Vid_FindRGB(i*8+180,i*8+180,i*8+180);
 menu_fontcolor_h=Vid_FindRGB(255,255,255);
 //menu_fontcolor_std=Vid_FindRGB(200,200,200);
 Mem_DPtr((void*)&menu_fontcolor_std,fntsurf);
 menu_fontcolor_hidden=Vid_FindRGB(128,128,128);
 Mem_DPtr((void*)&menu_fontcolor_alt,altsurf);
 //menu_fontcolor_alt=Vid_FindRGB(128,128,128);
 menu_font_alt=Surf_LoadFnt (5,5,1,&cf,"fnt(5x5).bin");
 fnt=(int)menu_font_alt->ctab;
 VR_New("fnt",VR_INT,&fnt);
 CF_Close(&cf);
 vid_wait=0;
 loop_isdrawfps=1;
 loop_drawfpscol=Vid_FindRGB(255,200,200);
 Loop_Entry();
}
