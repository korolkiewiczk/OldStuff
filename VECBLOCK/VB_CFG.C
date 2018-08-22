#ifndef __VB_CFG_C
#define __VB_CFG_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_CFG.C
 Opis: Obsluga pliku konfiguracji
------------------------------------------------
*/


//===========================================

#include "vb_cfg.h"

//===========================================

/*
########################################
 Cfg_Exec
 Wykonuje polecenia zawarte w danym pliku konfiguracyjnym
 Jesli ok 0 : numer wiersza w ktorym jest ostatni blad lub -1 jesli brak pliku
 Tak wyglada plik konfiguracyny:
 
 nazwa=wyrazenie        //przypisanie
 lub
 wyrazenie              //np. wywolanie funkcji
 ...
 //komentarz
 
########################################
*/

int Cfg_Exec(char *fn)
{
 FILE *f;
 int i,j,ise,err=0,line=0,tc,isstr;
 float w;
 char buf[1024],bufw[1024],*cc;
 char c,c2;
 f=fopen(fn,"rb");
 if (f==NULL) return -1;
 while (!feof(f))
 {
  line++;
  ise=0;
  i=j=tc=isstr=0;
  while (1)
  {
   c=fgetc (f);
   if ((c==13)||(c==10)) {fgetc(f); break;}
   if (c=='"') isstr=1-isstr;
   if (feof(f)) break;
   if (tc==0)
   {
    if (c=='/')
    {
     c=fgetc (f);
     if (c2=='/')
     {
      tc=1;
     }
     else
     {
      if ((isstr==0)&&(c=='=')) {ise=1; continue;}
      if (ise) bufw[j++]=c; else buf[i++]=c;
      if (feof(f)) break;
      if (c2=='"') isstr=1-isstr;
      if ((c2==13)||(c2==10)) {fgetc(f); break;}
      if ((isstr==0)&&(c2=='=')) {ise=1; continue;}
      if (ise) bufw[j++]=c2; else buf[i++]=c2;
     }
    }
    else
    {
     if ((isstr==0)&&(c=='=')) {ise=1; continue;}
     if (ise) bufw[j++]=c; else buf[i++]=c;
    }
   }
  }
  buf[i]=0;
  bufw[j]=0;
  i=j=0;
  while (buf[i]!=0) {if (buf[i]=='\t') buf[i]=' '; i++;}
  while (bufw[j]!=0) {if (bufw[j]=='\t') bufw[j]=' '; j++;}
  i=j=0;
  while ((buf[i]!=0)&&(buf[i]==' ')) i++;
  while ((buf[j]!=0)&&(bufw[j]==' ')) j++;
  if ((buf[i]==0)&&(bufw[j]==0)) continue;
  if (ise)
  {
   w=Expr_Comp(bufw+j);
   if (Expr_CheckErr(0)!=EXPR_OK) err=line;
   else
   {
    cc=strchr(buf+i,' ');
    if (cc!=NULL) *cc=0;
    if (VR_Set(buf+i,&w,VR_FLOAT)==0) err=line;
   }
  }
  else
  {
   Expr_Comp(buf+i);
   if (Expr_CheckErr(0)!=EXPR_OK) err=line;
  }
  //printf ("%d:%s|%s\n",line,buf+i,bufw+j);
 }
 fclose(f);
 return err;
}

#endif
