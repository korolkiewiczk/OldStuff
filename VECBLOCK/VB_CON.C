#ifndef __VB_CON_C
#define __VB_CON_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_CON.C
 Opis: Obsluga konsoli.
------------------------------------------------
*/

//==========================================

#include "vb_con.h"

//==========================================

char con_buf[CON_BUFLEN][CON_STRLEN];           //tresc konsoli
char con_editbuf[CON_EDITBUFLEN][CON_STRLEN];   //wpisywane komendy
char con_str[CON_STRLEN];                       //pole edycji

int con_wsk;                    //wskaznik konsoli
int con_edwsk;                  //wskaznik wpisywanych komend
int con_lines;                  //liczba lini
int con_lastline;               //ostatnia wyswietlana linia (od dolu)
int con_edline;                 //pozycja wpisywanych komend
int con_cursorpos;              //pozycja kursora pola edycji
int con_edshf;                  //przesuniecie w polu edycji
int con_strlen;                 //dlugosc wpisywanej komendy
int con_ison;                   //czy konsola wlaczona
float con_apos;                 //pozycja animacji konsoli
int con_closing;                //czy konsola jest zamykana
TFnt *con_font;                 //czcionka konsoli
int con_fontc;                  //kolor/powierzchnia czionki konsoli
TSurf *con_bsurf;               //tylna powierzchnia konsoli
int con_cursoron;               //czy widoczny kursor
time_f con_cursortime;          //czas po jakim pojawia sie kursor
time_f con_cursortimer;         //czas kursora
time_f con_openclosetm;         //czas zamykania/otwierania konsoli
int con_maxapos;                //koniec konsoli
int con_printexprv;             //czy pisac wynik dzialania
int con_key;                    //nacisniety klawisz

TFVLib con_lib[CON_LIBNFUNC]=
{
 {"con_setnlines",{&Con_SetnLines,1,0,0}},
 {"con_setcontime",{&Con_SetConTime,2,1|2|4,0}},
};

//==========================================

/*
########################################
 InitConsole
 Inicjuje konsole
 font-adres do uzytej w konsoli czcionki
 fontc-kolor czcionki
 cf-zrodlowy plik cf
 sname-nazwa pliku z powierzchnia
 octm-czas zamykania/otwierania konsoli (w sek.)
 ctime-szybkosc migania kursora (w sek.)
########################################
*/

void InitConsole (TFnt *font,int fontc,TCF *cf,const char *sname,time_f octm,time_f ctime)
{
 if ((cf!=NULL)&&(sname!=NULL))
 Surf_LoadFromBMP(cf,&con_bsurf,sname);
 else con_bsurf=NULL;
 //
 con_font=font;
 con_fontc=fontc;
 //
 con_buf[0][0]=0;
 con_editbuf[0][0]=0;
 con_str[0]=0;
 //
 con_lines=CON_STDLINES;
 //
 con_openclosetm=octm;
 con_cursortime=ctime;
 con_cursortimer=0;
 //
 con_wsk=con_edwsk=con_lastline=
 con_strlen=con_cursorpos=con_edshf=
 con_edline=con_closing=con_printexprv=0;
 //
 con_maxapos=con_lines*CON_CHARWXH+CON_CHARWXH+CON_Y+CON_Y;
 con_apos=0;
 con_ison=1;
 //
 Expr_IncludeLib(con_lib,CON_LIBNFUNC);
}

/*
########################################
 Con_Print
 Dodaje tekst na konsole
########################################
*/

void Con_Print(const char *txt)
{
 char buf[1024];
 strcpy (buf,txt);
 if (strlen(buf)>=CON_STRLEN)
 {
  buf[CON_STRLEN-1]=0;
  buf[CON_STRLEN-2]='.';
  buf[CON_STRLEN-3]='.';
  buf[CON_STRLEN-4]='.'; //text...
 }
 strcpy (con_buf[(con_wsk++)%CON_BUFLEN],buf);
}

/*
########################################
 Con_Printf
 Formatuje i dodaje tekst na konsole
########################################
*/

void Con_Printf(const char *txt, ...)
{
 va_list args;
 char buf[1024];
 va_start (args,txt);
 vsprintf (buf,txt,args);
 va_end (args);
 if (strlen(buf)>=CON_STRLEN)
 {
  buf[CON_STRLEN-1]=0;
  buf[CON_STRLEN-2]='.';
  buf[CON_STRLEN-3]='.';
  buf[CON_STRLEN-4]='.'; //text...
 }
 strcpy (con_buf[(con_wsk++)%CON_BUFLEN],buf);
}

/*
########################################
 Con_EnterCmd
 Wywoluje wpisywana komende
######################################## 
*/

void Con_EnterCmd()
{
 if (con_edwsk==0) strcpy (con_editbuf[(con_edwsk++)%CON_EDITBUFLEN],con_str);
 else
 if (strcmp (con_str,con_editbuf[(con_edwsk-1)%CON_EDITBUFLEN])!=0)
 strcpy (con_editbuf[(con_edwsk++)%CON_EDITBUFLEN],con_str);
 float f=Expr_Comp(con_str);
 char err[1024];
 if (Expr_CheckErr(err)==1)
 {
  if (con_printexprv) Con_Printf ("%f",f);
 }
 else Con_Print (err);
 con_edline=con_edwsk;
 con_str[0]=0;
 con_lastline=con_cursorpos=con_strlen=con_edshf=0;
}

/*
#######################################
 Con_GoUp
 Przesuwa obraz konsoli w dol (idziemy w gore)
 Jesli przesuniecie nie nastapilo to 0 : 1
#######################################
*/

int Con_GoUp()
{
 int buflen=(con_wsk<CON_BUFLEN) ? con_wsk : CON_BUFLEN;
 if (con_lastline+con_lines>=buflen) return 0;
 con_lastline++;
 return 1;
}

/*
#######################################
 Con_GoDown
 Przesuwa obraz konsoli w gore (idziemy w dol)
 Jesli przesuniecie nie nastapilo to 0 : 1
#######################################
*/

int Con_GoDown()
{
 if (con_lastline==0) return 0;
 con_lastline--;
 return 1;
}

/*
#######################################
 Con_WriteChar
 Przetwarza znak i jesli jest odpowiedni to zapisuje go w pole edycji
 Obsugiane znaki:od 32 do 126 oraz [enter], [backspace] i [delete]
 Jesli znak byl odpowiedni to 1, jesli nie to -1,
 jesli [enter] jesli zdanie bylo niepuste to 2 : 3,
 jesli zdanie bylo o maksymalnej dlugosci to 0
#######################################
*/

int Con_WriteChar (unsigned char c)
{
 int i;
 if ((c==0)||(c>127)) return -1;
 if (c==CON_ENTERKEY)   //enter
 {
  if (con_str[0]==0) return 3;
  Con_EnterCmd();
  return 2;
 }
 if (c==CON_BKSPACEKEY)                 //backspace
 {
  if (con_cursorpos<=0) return -1;
  for (i=con_cursorpos-1; i<con_strlen; i++) con_str[i]=con_str[i+1];
  if (con_edshf>0) con_edshf--;
  con_cursorpos--;
  con_strlen--;
 }
 else
 if (c==CON_DELKEY)             //delete
 {
  if (con_cursorpos==con_strlen) return -1;
  for (i=con_cursorpos; i<con_strlen; i++) con_str[i]=con_str[i+1];
  con_strlen--;
 }
 else
 if ((c>=32)&&(c<127))
 {
  if (con_strlen==CON_STRLEN-1) return 0;
  if (con_cursorpos==con_strlen)        //koniec zdania
  {
   con_str[con_cursorpos++]=c;
   con_str[con_cursorpos]=0;
   con_strlen++;
  }
  else
  {
   for (i=con_strlen; i>=con_cursorpos; i--) con_str[i+1]=con_str[i];
   con_str[con_cursorpos++]=c;
   con_strlen++;
  }
 }
 else return -1;
 if ((con_cursorpos-con_edshf)+3>vid_buf.swidth/CON_CHARWXH)
    con_edshf+=(con_cursorpos-con_edshf)+3-vid_buf.swidth/CON_CHARWXH;
 else
 if ((con_cursorpos-con_edshf)<0)
    con_edshf=con_cursorpos;
 return 1;
}

/*
########################################
 Con_MoveCursor
 Przesuwa kursor
 Zwraca nowa pozycje kursora
########################################
*/

int Con_MoveCursor(int s)
{
 con_cursorpos+=s;
 if (con_cursorpos<0) con_cursorpos=0;
 else
 if (con_cursorpos>con_strlen) con_cursorpos=con_strlen;
 if ((con_cursorpos-con_edshf)+3>vid_buf.swidth/CON_CHARWXH)
    con_edshf+=(con_cursorpos-con_edshf)+3-vid_buf.swidth/CON_CHARWXH;
 else
 if ((con_cursorpos-con_edshf)<0)
    con_edshf=con_cursorpos;
 return con_cursorpos;
}

/*
########################################
 Con_PrevCommand
 Zwraca w lini edycji wczesniej wpisywana komende
########################################
*/

void Con_PrevCommand()
{
 if (con_edwsk==0) return;
 int buflen=(con_edwsk<CON_EDITBUFLEN) ? con_edwsk : CON_EDITBUFLEN;
 if (con_edwsk-con_edline>=buflen) con_edline=con_edwsk;
 strcpy (con_str,con_editbuf [(--con_edline)%buflen]);
 con_strlen=strlen(con_str);
 con_cursorpos=con_strlen;
 if (con_cursorpos+3>vid_buf.swidth/CON_CHARWXH)
    con_edshf=con_cursorpos+3-vid_buf.swidth/CON_CHARWXH;
 else
 con_edshf=0;
}

/*
########################################
 Con_NextCommand
 Zwraca w lini edycji nastepna wpisywana komende
########################################
*/

void Con_NextCommand()
{
 if (con_edwsk==0) return;
 int buflen=(con_edwsk<CON_EDITBUFLEN) ? con_edwsk : CON_EDITBUFLEN;
 if (con_edline>=con_edwsk-1) con_edline=con_edwsk-buflen-1;
 strcpy (con_str,con_editbuf [(++con_edline)%buflen]);
 con_strlen=strlen(con_str);
 con_cursorpos=con_strlen;
 if (con_cursorpos+3>vid_buf.swidth/CON_CHARWXH)
    con_edshf=con_cursorpos+3-vid_buf.swidth/CON_CHARWXH;
 else
 con_edshf=0;    
}

/*
########################################
 Con_Draw
 Rysuje konsole oraz odpowiada za animacje konsoli
########################################
*/

void Con_Draw()
{
 if ((con_ison==0)&&(con_apos==con_maxapos)) return;
 int buflen=(con_wsk<CON_BUFLEN) ? con_wsk : CON_BUFLEN;
 int last=con_wsk-con_lastline;
 int first,y,i;
 //animacja konsoli, kursora
 if (con_closing)
 {
  if (con_apos<con_maxapos)
  {
   con_apos+=(con_maxapos/con_openclosetm)*localtime;
   if (con_apos>con_maxapos) con_apos=con_maxapos;
  }
 }
 else
 {
  if (con_apos>0)
  {
   con_apos-=(con_maxapos/con_openclosetm)*localtime;
   if (con_apos<0) con_apos=0;
  }
 }
 con_cursortimer+=localtime;
 if (con_cursortimer>con_cursortime)
 {
  con_cursortimer=0;
  con_cursoron=1-con_cursoron;
 }
 //rysowanie konsoli
 if (con_lines>=buflen)
 {
  first=last-buflen;
  y=con_lines-buflen;
 }
 else
 {
  first=last-con_lines;
  y=0;
 }
 if (con_bsurf!=0)
 {
  TRect crect={0,0,con_bsurf->swidth,con_bsurf->sheight};
  TRect srect={0,con_maxapos-vid_buf.sheight-con_apos,vid_buf.swidth,vid_buf.sheight};
  Surf_SBlt(&crect,&srect,con_bsurf,&vid_buf);
 }
 for (i=first; i<last; i++)
 {
  Surf_Print(CON_X,CON_Y+y*CON_CHARWXH-con_apos,con_fontc,con_font,&vid_buf,con_buf[i%CON_BUFLEN]);
  y++;
 }
 Surf_Printf(CON_X,CON_Y+y*CON_CHARWXH-con_apos,con_fontc,con_font,&vid_buf,">%s",con_str+con_edshf);
 if (con_cursoron) Surf_Print(CON_X+(con_cursorpos-con_edshf)*CON_CHARWXH+CON_CHARWXH,CON_Y+y*CON_CHARWXH-con_apos+1,con_fontc,con_font,&vid_buf,CON_CURSORCHAR);
 //Surf_Printf8X8(CON_X,CON_Y+(y<<3)+10,con_fontc,con_font,&vid_buf,"%d",con_bsurf->sheight);//"%f,%f",localtime,globaltime);
 //gotoxy (1,y);
 //for (i=first; i<last; i++) MPrint (0,y++,con_buf[i%CON_BUFLEN]); //printf ("%s\n",con_buf[i%CON_BUFLEN]);
 //MPrint (0,y,con_str); //printf ("%s\n",con_str);
 //MGotoXY (con_cursorpos,y);
 //gotoxy (con_cursorpos+1,wherey()-1);
}

/*
########################################
 Con_GetKeyEvents
 Obsluga klawiszy w konsoli
########################################
*/

void Con_GetKeyEvents()
{
 int k;
 int c;
 con_key=0;
 for (k=0;k<128;k++) if (Key_CGet(k)==1)
 {
  con_key=k;
  switch (k)
  {
   case K_UP:           Con_PrevCommand(); break;
   case K_DOWN:         Con_NextCommand(); break;
   case K_LEFT:         Con_MoveCursor(-1); break;
   case K_RIGHT:        Con_MoveCursor(1); break;
   case K_HOME:         Con_MoveCursor(-1000000); break;
   case K_END:          Con_MoveCursor(1000000); break;
   case K_PGUP:         Con_GoUp(); break;
   case K_PGDOWN:       Con_GoDown(); break;
   case K_TLD:          Con_OpenClose(); break;
   case K_ESC:          {
                         con_str[0]=con_lastline=con_cursorpos=con_strlen=con_edshf=0;
                         con_edline=con_edwsk;
                        } break;
   case K_F12:          vid_prtscr=1;
   default:                                  //znak
   {
    c=Key_Key2Char(k);
    if (c!=-1) Con_WriteChar(c);
   }
  } //switch
 } //for (...)
}

/*
########################################
 Con_OpenClose
 Zamyka lub otwiera konsole
########################################
*/

void Con_OpenClose()
{
 con_ison=1-con_ison;
 if (con_ison) con_closing=0;
 else con_closing=1;  
}


//funkcje dostepowe konsoli

/*
########################################
 Con_SetnLines
 Ustawia liczbe lini konsoli
########################################
*/

void Con_SetnLines(int n)
{
 float pom=con_maxapos;
 con_lines=n;
 con_maxapos=con_lines*CON_CHARWXH+CON_CHARWXH+CON_Y+CON_CHARWXH;
 if (con_apos!=0) con_apos=con_apos/pom*con_maxapos;
}

/*
########################################
 Con_SetConTime
 Ustawia czas zamykania/otwierania konsoli i czas migania kursora
########################################
*/

void Con_SetConTime(float opt,float ct)
{
 con_openclosetm=opt;
 con_cursortime=ct;
}

#endif
