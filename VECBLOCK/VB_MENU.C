#ifndef __VB_MENU_C
#define __VB_MENU_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_MENU.C
 Opis: Obsluga menu.
------------------------------------------------
*/

//==========================================

#include "vb_menu.h"

//==========================================

TMenuSection *menu_stab;
unsigned char menu_nsections=0;
int menu_x;
unsigned char menu_stree[32],menu_streepos=0;
int menu_ison=1;
TFnt *menu_font_std=0;          //czionka dla pozycji standardowej
int menu_fontcolor_std;         //kolor czcionki dla pozycji standardowej
TFnt *menu_font_h=0;            //czionka dla pozycji zaznaczonej
int menu_fontcolor_h;           //kolor czcionki dla pozycji zaznaczonej
int menu_fontcolor_hidden;      //kolor czcionki dla pozycji niedostepnej
TFnt *menu_font_alt;            //czcionka dla opisu
int menu_fontcolor_alt;         //kolor czcionki dla opisu
int menu_fonth=8;  //szerokosc i wysokosc czcionki
int menu_width;                 //szerokosc menu
int menu_barwidth,menu_barpw;
TSurf *menu_barsurf=0;
TSurf *menu_barwsk=0;
TSurf *menu_ons=0;
TSurf *menu_offs=0;

char *menu_params[MENU_NPARAMS]=
{
 "NAME",
 "TYPE",
 "ACTION",
 "ALT",
 "VREF",
 "BAR",
 "VAR",
 "LIST",
 "HIDDEN"
};

//==========================================

/*
Struktora pliku z menu:
:nazwa_sekcji
<"nazwa pola" parametr="wartosc" ...>
...
:nazwa_sekcji2:
<...>
...
Parametry:
*type-typ pola:bar,list,var,onoff
*action-wykonywana akcja:
 -jesli "funkcja(...)" to wykonuj jesli enter
 -jesli "`funkcja(...)" to wykonu esli prawy/lewy
 -jesli ":nazwa sekcji" to skacz do tej sekcji (esc aby z niej wrocic)
*vref-nazwa zmiennej VR do ktorej nalezy sie odwolac. Konieczne dla bar/var.
*alt-opis
*bar/var-parametr to kolejno:minimalna wartosc,maksymalna wartosc,amplituda zmian
 np.bar="-10 10 2"
*list-lista gdzie: "'nazwa pola listy','akcja','nazwa pola list2','akca2',..."
*/

//==========================================

/*
########################################
 Menu_Load
 Laduje menu z pliku
 Parametry:
 cf-plik cf
 fn-nazwa pliku z menu
 mw-szerokosc menu
 barname-nazwa pliku z bitmapa dla suwaka
 bwskname-nazwa pliku z bitmapa dla wskaznika suwaka
 barw-szerokosc suwaka
 menuon,menuoff-nazwa pliku z bitmapa dla pozycji on/off
 Jesli ok 1 : 0
########################################
*/

int Menu_Load (TCF *cf,char *fn,int mw,char *barname,char *bwskname,int barw,char *menuon,char *menuoff)
{
 char tbuf[32768];
 char txtb[2048];
 int i,j,s,nbytes,par;
 int isstxt,isitxt,i_ptype,i_istxt;
 char c;
 TMenuSection *ms;
 TMenuItem *mi,*pom;
 nbytes=CF_Read (cf,tbuf,0,32768,fn);
 if (nbytes==-1) return 0;
 menu_width=mw; 
 //szukanie sekcji
 i=isstxt=i_istxt=0;
 while (i<nbytes)
 {
  c=tbuf[i++];
  if ((c==' ')||(c=='\t')) if (isstxt==0) continue;
  if (c=='"') i_istxt=1-i_istxt;
  if (((c==10)||(c==13))&&(isstxt==1))
  {
   //if ((txtb[0]=='M')&&(txtb[1]=='A')&&(txtb[2]=='I')&&(txtb[3]=='N'));
   //if (*(int*)txtb=='NIAM') continue;
   ms=Mem_SAlloc(sizeof(TMenuSection));
   if (ms==0) return 0;
   if (j>MENU_SNAMEMAXLEN) j=MENU_SNAMEMAXLEN;
   txtb[j]=0;
   strcpy(ms->sname,txtb);
   ms->spos=0;
   ms->nspos=0;
   ms->itaddr=0;
   if (menu_stab==0) menu_stab=ms;
   menu_nsections++;
   isstxt=0;
  }
  if (isstxt==1)
  {
   txtb[j++]=c;
  }
  if ((c==':')&&(i_istxt==0))   //poczatek sekcji
  {
   j=0;
   isstxt=1;
  }
 }
 
 //tworzenie struktory menu
 i=j=i_istxt=isitxt=isstxt=0;
 s=-1;
 ms=menu_stab;
 while (i<nbytes)
 {
  if (tbuf[i]=='"') i_istxt=1-i_istxt;
  else
  if ((tbuf[i]==':')&&(i_istxt==0)) break;
  i++;
 }
 i--;
 while (i<nbytes)
 {
  c=tbuf[i++];
  if (isitxt==0) if (c==':') isstxt=1;
  if (isstxt)
  {
   if ((c==10)||(c==13))
   {
    isstxt=0;
    s++;
   }
   continue;
  }
  if ((c==' ')||(c=='\t')) if (isitxt==0) continue;
  if (i_istxt==0)                       //jesli nie "abc...
  {
   if (c=='<')                          //poczatek pozycji
   {
    i_istxt=i_ptype=par=0;
    isitxt=1;
    pom=Mem_SAlloc(sizeof (TMenuItem));
    if (pom==0) return 0;
    if (ms[s].itaddr!=0)
    {
     mi->next=pom;
     ms[s].nspos++;
    }
    mi=pom;
    //domyslne ustawienia
    mi->type=MENU_ITYPE_NI;
    mi->value=0;
    mi->maxvalue=0;
    mi->ishidden=0;
    mi->action=0;
    mi->tstruct=0;
    mi->vref=0;
    mi->alt=0;
    mi->next=0;
    if (ms[s].itaddr==0)
    {
     ms[s].itaddr=mi;
     ms[s].nspos++;
    }
    continue;
   }
   else
   if (c=='>') {isitxt=0; continue;}    //...koniec
  }
  if (isitxt)                           //jesli <abc...
  {
   if ((c==10)||(c==13)) continue;
   if (i_istxt)                         //jesli "abc...
   {
    if (c=='"')                         //jesli koniec to szukaj znaczenia dla parametru
    {
     i_istxt=0;
     txtb[j]=0;
     j=Menu_Par(par,txtb,mi);
     //printf ("\"%s,%d\"\n",txtb,j);
     j=0;
    }
    else
    if (c=='\\')
    {
     if (tbuf[i]=='n')
     {
      i++;
      txtb[j++]=10;
     }
    }
    else
    txtb[j++]=c;
   }
   else                                 //jesli parametr
   {
    if ((c==' ')&&(j==0)) continue;
    if (c=='"') {i_istxt=1;}       //poczatek wartosci parametru
    else
    if (c=='=')                         //szukaj znaczenia parametru
    {
     txtb[j]=0;
     par=Menu_TXT2Par(txtb);
     //printf ("%s %d=",txtb,par);
     j=0;
    }
    else
    txtb[j++]=c;
   }
  }
 }
 menu_barwidth=0;
 Menu_LoadBar(cf,barname,bwskname,barw);
 if ((Surf_LoadFromBMP(cf,(void*)&menu_ons,menuon))&&(Surf_LoadFromBMP(cf,(void*)&menu_offs,menuoff))==0) {menu_ons=0;menu_offs=0;}
 return 1;
}

/*
########################################
 Menu_TXT2Par
 Podaje numer parametru
 Zwraca numer albo -1
########################################
*/

int Menu_TXT2Par (char *txt)
{
 int i;
 char *c;
 strupr(txt);
 c=strchr(txt,' ');
 if (c!=0) *c=0;
 for (i=0;i<MENU_NPARAMS;i++)
 {
  if ((strcmp(txt,menu_params[i])==0)) return i;
 }
 return -1;
}

/*
########################################
 Menu_Par
 Obsluguje dany parametr
 Jesli ok 1 : 0
########################################
*/

int Menu_Par (int par,char *txt,TMenuItem *mi)
{
 void *wsk;
 int i,j,k,l,m,n;
 char ptxt[1024];
 TSList *tsl,*pom;
 TAlt *alt=0,*apom;
 switch (par)
 {
  case MENUPAR_NAME:                             //nazwa
       wsk=Mem_SAlloc(strlen(txt)+1);
       strcpy(wsk,txt);
       mi->txt=wsk;
       return 1;
  case MENUPAR_TYPE:
       strupr(txt);
       if (strcmp(txt,"NORMAL ITEM")==0) mi->type=MENU_ITYPE_NI;
       else
       if (strcmp(txt,"BAR")==0) mi->type=MENU_ITYPE_BAR;
       else
       if (strcmp(txt,"ONOFF")==0) mi->type=MENU_ITYPE_ONOFF;
       else
       if (strcmp(txt,"VAR")==0) mi->type=MENU_ITYPE_VAR;
       else
       if (strcmp(txt,"LIST")==0) mi->type=MENU_ITYPE_LIST;
       else
       return 0;
       return 1;
  case MENUPAR_ACTION:
       i=0;
       while (txt[i]==' ') i++;
       if (txt[i]==':')
       {
        i++;
        i=Menu_Section2int(txt+i);
        if (i==-1) return 0;
        mi->action=Mem_SAlloc(2);
        mi->action[0]=':';
        mi->action[1]=i;
        return 1;
       }
       mi->action=Mem_SAlloc(strlen(txt+i)+1);
       strcpy(mi->action,txt+i);
       return 1;
  case MENUPAR_ALT:
       i=0;
       j=0;
       m=0;
       n=0;                     //jesli n==0 to nowa linia
       k=-1;
       l=menu_width/Surf_GetFntW(menu_font_alt);
       while (k!=0)
       {
        k=txt[i++];
        if ((k==' ')||(k=='.')||(k==',')||(k=='?')||(k=='!')||(k==';'))
        {
         //while (txt[i]==k) i++;
         if (j!=0)
         {
          n=0;
          ptxt[j++]=k;
         }
         continue;
        }
        if (k==0)
        {
         //tworzenie struktory
         apom=Mem_SAlloc(sizeof (TAlt));
         apom->next=0;
         if (alt!=0) alt->next=apom;
         alt=apom;
         if (mi->alt==0) mi->alt=alt;
         //tworzenie opisu
         ptxt[j]=0;
         alt->line=Mem_SAlloc(strlen(ptxt)+1);
         strcpy(alt->line,ptxt);
         //printf ("%s-",alt->line);
         return 1;
        }
        if (n==0)                       //nowa linia
        {
         if (j>l)                       //jesli linia jest za dluga
         {
          //tworzenie struktory
          apom=Mem_SAlloc(sizeof (TAlt));
          apom->next=0;
          if (alt!=0) alt->next=apom;
          alt=apom;
          if (mi->alt==0) mi->alt=alt;
          //tworzenie opisu
          if (m==0)                     //nie patrz na dlugosc
          {
           alt->line=Mem_SAlloc(j+1);
           memcpy(alt->line,ptxt,j);
           alt->line[j]=0;
           //printf ("%s|",alt->line);
           j=0;
          }
          else
          {
           alt->line=Mem_SAlloc(m+1);
           memcpy(alt->line,ptxt,m);
           alt->line[m]=0;
           //printf ("%s|",alt->line);
           j=j-m;
           memcpy(ptxt,ptxt+m,j);
           m=0;
          }
         }
         else
         {
          m=j;
         }
         n=1;
        }
        ptxt[j++]=k;
       }
       return 1;
  case MENUPAR_VREF:
       i=0;
       while (txt[i]==' ') i++;
       if (txt[i]=='$') i++;
       mi->vref=Mem_SAlloc(strlen (txt+i)+1);
       strcpy(mi->vref,txt+i);
       return 1;
  case MENUPAR_BAR:
       mi->type=MENU_ITYPE_BAR;
       goto MENUPAR_BAR_VAR;
  case MENUPAR_VAR:
       mi->type=MENU_ITYPE_VAR;
       MENUPAR_BAR_VAR:
       mi->tstruct=Mem_SAlloc(sizeof(TSBarVar));
       i=j=k=0;
       while (txt[i]==' ') i++;
       while (1)
       {
        if ((txt[i]==0)||(txt[i]==' '))
        {
         ptxt[j]=0;
         j=atoi(ptxt);
         *(mi->tstruct+k)=j;
         if (txt[i]==0) break;
         j=0;
         while (txt[i]==' ') i++;
         k++;
         continue;
        }
        else
        ptxt[j++]=txt[i];
        i++;
       }
       return 1;
  case MENUPAR_LIST:
       mi->type=MENU_ITYPE_LIST;
       i=j=k=0;
       mi->tstruct=Mem_SAlloc(sizeof (TSList));
       tsl=(TSList*)mi->tstruct;
       tsl->name=0;
       tsl->proc=0;
       tsl->next=0;
       while (txt[i]!=0)
       {
        if (j==0)
        {
         while ((txt[i]!=0)&&(txt[i]!='\''))i++;
         if (txt[i]==0) break;
         i++;
        }
        if (txt[i]=='\'')
        {
         ptxt[j]=0;
         if (k==2)              //nowy rekord
         {
          pom=Mem_SAlloc(sizeof (TSList));
          tsl->next=pom;
          tsl=pom;
          tsl->name=0;
          tsl->proc=0;
          tsl->next=0;
          mi->maxvalue++;
          k=0;
          continue;
         }
         if (k==0)
         {
          tsl->name=Mem_SAlloc(j+1);
          strcpy(tsl->name,ptxt);
         }
         else
         {
          tsl->proc=Mem_SAlloc(j+1);
          strcpy(tsl->proc,ptxt);
         }
         i++;
         while ((txt[i]!=0)&&(txt[i]!=',')) i++;
         k++;
         j=0;
        }
        else
        {
         ptxt[j++]=txt[i++];
        }
       }
       return 1;
  case MENUPAR_HIDDEN:
       if (txt[0]=='0') mi->ishidden=0; else mi->ishidden=1;
       return 1;
 }
 return 0;
}

/*
########################################
 Menu_Section2int
 Zamienia nazwe sekcji na liczbe
 Zwraca numer sekcji lub -1
########################################
*/

int Menu_Section2int (char *txt)
{
 int i;
 for (i=0;i<menu_nsections;i++)
 {
  if (strcmp(txt,menu_stab[i].sname)==0) return i;
 }
 return -1;
}

/*
########################################
 Menu_CurrS
 Zwraca aktualna sekcje
########################################
*/

inline int Menu_CurrS()
{
 return menu_stree[menu_streepos];
}

/*
########################################
 Menu_CurrPos
 Zwraca aktualna pozycje w sekcji
########################################
*/

inline int Menu_CurrPos()
{
 return menu_stab[Menu_CurrS()].spos;
}

/*
########################################
 Menu_FindItem
 Zwraca pozycje w sekcji o danym indeksie
########################################
*/

TMenuItem *Menu_FindItem(int s,int p)
{
 TMenuItem *mi=menu_stab[s].itaddr;
 int i;
 for (i=0;i<p;i++) mi=mi->next;
 return mi;
}

/*
########################################
 Menu_Action
 Wykonuje akcje menu
########################################
*/

void Menu_Action (char *action,int k)
{
 int i=0,j=0;
 char c;
 char txt[1024];
 while (j<1024)
 {
  if (j==0)
  {
   while (action[i]==' ') i++;
   if (action[i]=='`')
   {
    i++;
    if (k==0) while ((action[i]!=0)&&(action[i]!='|')) i++;
   }
   else
   {
    if (k==1) while ((action[i]!=0)&&(action[i]!='|')) i++;
   }
  }
  c=action[i++];
  if ((c!=0)&&(c!='|'))
  {
   txt[j++]=c;
  }
  else
  {
   txt[j]=0;
   Expr_Comp(txt);
   if (c==0) break;
   j=0;
  }
 }
}

/*
########################################
 Menu_FindList
 Znajduje odpowiednie pole listy dla danej liczby
########################################
*/

TSList *Menu_FindList (TMenuItem *mi,int v)
{
 TSList *tsl=(TSList*)mi->tstruct;
 int i;
 for (i=0;i<v;i++) if (tsl==0) break; else tsl=tsl->next;
 return tsl;
}

/*
########################################
 Menu_Cmd
 Wykonuje odpowiednia operacje w menu
########################################
*/

void Menu_Cmd (int cmd)
{
 int currs=Menu_CurrS();
 TMenuItem *mi;
 TSList *tsl;
 char txt[1024];
 int i,j,c;
 switch(cmd)
 {
  case MENUCMD_UP:
       if (menu_stab[currs].spos==0) menu_stab[currs].spos=menu_stab[currs].nspos-1;
       else
       menu_stab[currs].spos--;
       mi=Menu_FindItem(currs,menu_stab[currs].spos);
       if (mi->ishidden) Menu_Cmd(MENUCMD_UP);
       break;
  case MENUCMD_DOWN:
       if (menu_stab[currs].spos==menu_stab[currs].nspos-1) menu_stab[currs].spos=0;
       else
       menu_stab[currs].spos++;
       mi=Menu_FindItem(currs,menu_stab[currs].spos);
       if (mi->ishidden) Menu_Cmd(MENUCMD_DOWN);
       break;
  case MENUCMD_LEFT:
       mi=Menu_FindItem(currs,menu_stab[currs].spos);
       if (mi->vref!=0)
       {
        if (!VR_Get(mi->vref,&i,VR_INT)) break;
        if ((mi->type==MENU_ITYPE_BAR)||(mi->type==MENU_ITYPE_VAR))
        {
         i-=((TSBarVar*)mi->tstruct)->delta;
         if (i<((TSBarVar*)mi->tstruct)->min) i=((TSBarVar*)mi->tstruct)->min;
         VR_Set(mi->vref,&i,VR_INT);
        }
        else
        if (mi->type==MENU_ITYPE_ONOFF)
        {
         if (i==0) i=1; else i=0;
         VR_Set(mi->vref,&i,VR_INT);
        }
       }
       if (mi->type==MENU_ITYPE_LIST)
       {
        if (mi->value==0) mi->value=mi->maxvalue; else mi->value--;
        tsl=Menu_FindList(mi,mi->value);
        Menu_Action(tsl->proc,1);
       }
       if (mi->action)
       Menu_Action(mi->action,1);
       break;
  case MENUCMD_RIGHT:
       mi=Menu_FindItem(currs,menu_stab[currs].spos);
       if (mi->vref!=0)
       {
        if (!VR_Get(mi->vref,&i,VR_INT)) break;
        if ((mi->type==MENU_ITYPE_BAR)||(mi->type==MENU_ITYPE_VAR))
        {
         i+=((TSBarVar*)mi->tstruct)->delta;
         if (i>((TSBarVar*)mi->tstruct)->max) i=((TSBarVar*)mi->tstruct)->max;
         VR_Set(mi->vref,&i,VR_INT);
        }
        else
        if (mi->type==MENU_ITYPE_ONOFF)
        {
         if (i==0) i=1; else i=0;
         VR_Set(mi->vref,&i,VR_INT);
        }
       }
       if (mi->type==MENU_ITYPE_LIST)
       {
        if (mi->value==mi->maxvalue) mi->value=0; else mi->value++;
        tsl=Menu_FindList(mi,mi->value);
        Menu_Action(tsl->proc,1);
       }
       if (mi->action)
       Menu_Action(mi->action,1);
       break;
  case MENUCMD_ENTER:
       mi=Menu_FindItem(currs,menu_stab[currs].spos);
       if (mi->action)
       {
        if(mi->action[0]==':')
        {
         if (mi->action[1]!=-1)
         menu_stree[++menu_streepos]=mi->action[1];
        }
        else
        {
         Menu_Action(mi->action,0);
        }
       }
       if (mi->type==MENU_ITYPE_LIST)
       {
        tsl=Menu_FindList(mi,mi->value);
        Menu_Action(tsl->proc,0);
       }
       break;
  case MENUCMD_ESC:
       if (menu_streepos==0)
       exit(0);                 //wyjscie
       else
       menu_streepos--;
       break;
 }
}

/*
########################################
 Menu_Draw
 Rysuje menu
########################################
*/

void Menu_Draw()
{
 TMenuItem *mi;
 int spos=Menu_CurrS();
 int mpos;
 int i=0;
 int y;
 int vrefv;
 int w;
 TSList *tsl;
 TRect drect,srect;
 mpos=Menu_CurrPos();
 mi=menu_stab[spos].itaddr;
 y=vid_buf.sheight/2-menu_stab[spos].nspos*Surf_GetFntH(menu_font_std)/2;
 while (mi!=NULL)
 {
  if (mi->ishidden==0)
  {
   if (i==mpos)
   Surf_Print(menu_x,y,menu_fontcolor_h,menu_font_h,&vid_buf,mi->txt);
   else
   Surf_Print(menu_x,y,menu_fontcolor_std,menu_font_std,&vid_buf,mi->txt);
  }
  else
  Surf_Print(menu_x,y,menu_fontcolor_hidden,menu_font_std,&vid_buf,mi->txt);
  //Surf_Printf8X8(0,y,menu_fontcolor_std,menu_font_std,&vid_buf,"%s",mi->vref);
  if (mi->vref)
  {
   if (mi->type==MENU_ITYPE_VAR)
   {
    if (VR_Get(mi->vref,&vrefv,VR_INT))
       Surf_Printf(menu_x+menu_width-6*Surf_GetFntW(menu_font_std),y,menu_fontcolor_std,menu_font_std,&vid_buf,"%6d",vrefv);
   }
   else
   if ((mi->type==MENU_ITYPE_BAR)&&(menu_barwidth>0))
   {
    if (VR_Get(mi->vref,&vrefv,VR_INT))
    {
     //bar
     drect.rx=0;
     drect.ry=0;
     drect.rw=menu_barsurf->swidth;
     drect.rh=menu_barsurf->sheight;
     srect.rx=menu_x+menu_width-menu_barwidth-2*menu_barpw;
     srect.ry=y+Surf_GetFntH(menu_font_std)/2-menu_barsurf->sheight/2;
     srect.rw=menu_barsurf->swidth;
     srect.rh=menu_barsurf->sheight;
     Surf_CBlt(&drect,&srect,menu_barsurf,&vid_buf);
     //punkt
     w=((TSBarVar*)mi->tstruct)->max-((TSBarVar*)mi->tstruct)->min;
     vrefv=vrefv-((TSBarVar*)mi->tstruct)->min;
     w=((float)vrefv/(float)w)*((float)menu_barwidth-menu_barwsk->swidth);
     drect.rw=menu_barwsk->swidth;
     drect.rh=menu_barwsk->sheight;
     srect.rx=menu_x+menu_width-menu_barwidth-menu_barpw+w;
     srect.ry=y+Surf_GetFntH(menu_font_std)/2-menu_barwsk->sheight/2;
     srect.rw=menu_barwsk->swidth;
     srect.rh=menu_barwsk->sheight;
     Surf_CBlt(&drect,&srect,menu_barwsk,&vid_buf);
    }
   }
   else
   if (mi->type==MENU_ITYPE_ONOFF)
   {
    if (VR_Get(mi->vref,&vrefv,VR_INT))
    {
     if (vrefv)
     {
      if (menu_ons)
      {
       drect.rx=0;
       drect.ry=0;
       drect.rw=menu_ons->swidth;
       drect.rh=menu_ons->sheight;
       srect.rx=menu_x+menu_width-menu_ons->swidth;
       srect.ry=y+Surf_GetFntH(menu_font_std)/2-menu_ons->sheight/2;
       srect.rw=menu_ons->swidth;
       srect.rh=menu_ons->sheight;
       Surf_CBlt(&drect,&srect,menu_ons,&vid_buf);
      }
      else
      Surf_Print(menu_x+menu_width-2*Surf_GetFntW(menu_font_std),y,menu_fontcolor_std,menu_font_std,&vid_buf,"on");
     }
     else
     {
      if (menu_ons)
      {
       drect.rx=0;
       drect.ry=0;
       drect.rw=menu_offs->swidth;
       drect.rh=menu_offs->sheight;
       srect.rx=menu_x+menu_width-menu_offs->swidth;
       srect.ry=y+Surf_GetFntH(menu_font_std)/2-menu_offs->sheight/2;
       srect.rw=menu_offs->swidth;
       srect.rh=menu_offs->sheight;
       Surf_CBlt(&drect,&srect,menu_offs,&vid_buf);
      }
      else
      Surf_Print(menu_x+menu_width-3*Surf_GetFntW(menu_font_std),y,menu_fontcolor_std,menu_font_std,&vid_buf,"off");
     }
    }
   }
  }
  else
  if (mi->type==MENU_ITYPE_LIST)
  {
   tsl=Menu_FindList (mi,mi->value);
   if (tsl!=0)
   {
    Surf_Printf(menu_x+menu_width-(strlen(tsl->name)+2)*Surf_GetFntW(menu_font_std),y,menu_fontcolor_std,menu_font_std,&vid_buf,"<%s>",tsl->name);
   }
  }
  y+=Surf_GetFntH(menu_font_std);
  mi=mi->next;
  i++;
 }
 Menu_DrawAlt();
}

/*
########################################
 Menu_DrawAlt
 Pisze opis
########################################
*/

void Menu_DrawAlt()
{
 TMenuItem *mi=Menu_FindItem(Menu_CurrS(),Menu_CurrPos());
 TAlt *alt=mi->alt;
 if (!alt) return;
 int nlines=0,starty;
 while (alt) {alt=alt->next; nlines++;}
 starty=vid_buf.sheight-nlines*Surf_GetFntH(menu_font_alt);
 alt=mi->alt;
 while (alt)
 {
  Surf_Print(menu_x,starty,menu_fontcolor_alt,menu_font_alt,&vid_buf,alt->line);
  starty+=Surf_GetFntH(menu_font_alt);
  alt=alt->next;
 }
}

/*
########################################
 Menu_GetKeyEvents
 Obsluga klawiszy w menu
########################################
*/

void Menu_GetKeyEvents()
{
 int k;
 int c;
 for (k=0;k<128;k++) if (Key_CGet(k)==1)
 {
  switch (k)
  {
   case K_UP:           Menu_Cmd (MENUCMD_UP); break;
   case K_DOWN:         Menu_Cmd (MENUCMD_DOWN); break;
   case K_LEFT:         Menu_Cmd (MENUCMD_LEFT); break;
   case K_RIGHT:        Menu_Cmd (MENUCMD_RIGHT); break;
   case K_PGDOWN:       Menu_Cmd (MENUCMD_LEFT); break;
   case K_PGUP:         Menu_Cmd (MENUCMD_RIGHT); break;
   case K_ENTER:        Menu_Cmd (MENUCMD_ENTER); break;
   case K_ESC:          Menu_Cmd (MENUCMD_ESC); break;
   case K_F12:          vid_prtscr=1; break;
   case K_TLD:          Con_OpenClose();
  } //switch
 } //for (...)
}

/*
########################################
 Menu_VidApply
 Ustawia pozycje menu zgodnie z danym trybem
########################################
*/

void Menu_VidApply()
{
 menu_x=(vid_buf.swidth-menu_width)/2;
}

/*
########################################
 Menu_LoadBar
 Wczytuje pasek z pliku bmp i tworzy go o zadanej szerokosci
 Jesli ok 1 : 0
########################################
*/

int Menu_LoadBar (TCF *cf,char *name,char *wskname,int w)
{
 TSurf *psurf=0;
 int i,j,ww;
 if (menu_barsurf!=0) Surf_Dispose((void*)&menu_barsurf);
 if (Surf_LoadFromBMP(cf,(void*)&psurf,name)==0) return 0;
 ww=psurf->swidth*2+w-2;
 Surf_Create((void*)&menu_barsurf,ww,psurf->sheight);
 //prawo
 for (i=0;i<psurf->sheight;i++)
     memcpy(menu_barsurf->sbits+i*ww,psurf->sbits+i*psurf->swidth,psurf->swidth-1);
 //srodek
 for (j=0;j<w;j++)
     for (i=0;i<psurf->sheight;i++)
         *(menu_barsurf->sbits+i*ww+j+psurf->swidth-1)=*(psurf->sbits+i*psurf->swidth+psurf->swidth-1);
 //lewo
 for (i=0;i<psurf->sheight;i++)
     for (j=0;j<psurf->swidth-1;j++)
         *(menu_barsurf->sbits+i*ww+j+psurf->swidth-1+w)=*(psurf->sbits+i*psurf->swidth+(psurf->swidth-2-j));
 menu_barpw=psurf->swidth-1;
 Surf_Dispose((void*)&psurf);
 //wsk
 if (menu_barwsk!=0) Surf_Dispose((void*)&menu_barwsk);
 if (Surf_LoadFromBMP(cf,(void*)&menu_barwsk,wskname)==0) return 0;
 menu_barwidth=w;
 return 1;
}

#endif
