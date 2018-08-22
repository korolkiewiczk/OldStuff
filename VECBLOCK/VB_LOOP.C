#ifndef __VB_LOOP_C
#define __VB_LOOP_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_LOOP.C
 Opis: Glowna petla
------------------------------------------------
*/

//===========================================

#include "vb_loop.h"

//===========================================

TTime loop_time;
//dodatkowe procedury dla uzytkownika
void (*loop_firstextraproc)()=0;
void (*loop_lastextraproc)()=0;
//zmienne dotyczace rysowania klatek na sek.
int loop_isdrawfps=0;
int loop_drawfpscol=0;

//===========================================

/*
########################################
 InitLoopSys
 Inicjuje system petli
########################################
*/

void InitLoopSys(void (*loopprc1)(),void (*loopprc2)())
{
 loop_firstextraproc=loopprc1;
 loop_lastextraproc=loopprc2;
 Time_InitStruct(&loop_time,TIME_FLAGS_SEC_TIME);       //w sek.
 localtime=1;
 globaltime=0;
}

/*
########################################
 Loop_Entry
 Glowna petla nieskonczona
########################################
*/

void Loop_Entry()
{
 while (1)
 {
  memset(vid_buf.sbits,0,vid_buf.swidth*vid_buf.sheight);
  if (loop_firstextraproc) loop_firstextraproc();
  if (con_ison)         //tryb konsoli
  {
   Con_GetKeyEvents();
  }
  else                  //tryb programu
  {
   //Con_GetKeyEvents();
   //Con_Draw();
   if (menu_ison)
   {
    //Menu_Draw();
    Menu_GetKeyEvents();
   }
   //...
  }
  Menu_Draw();
  Con_Draw();
  if (loop_isdrawfps) Surf_Printf8X8(0,0,loop_drawfpscol,&biosfont,&vid_buf,"%f",1/localtime);
  if (loop_lastextraproc) loop_lastextraproc();
  Vid_Swap();
  localtime=Time_Get(&loop_time);
  globaltime+=localtime;
 }
}

#endif
