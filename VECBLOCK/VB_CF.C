#ifndef __VB_CF_C
#define __VB_CF_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_CF.C
 Opis: Obsluga plikow polaczanych w jeden ("common file");
------------------------------------------------
*/

//==========================================

#include "vb_cf.h"

//==========================================

/*
########################################
 CF_Load
 Laduje glowny plik
 Jesli ok 1 : jesli plik niewlasciwie utworzony lub blad alokacji -1 : 0
########################################
*/

int CF_Load(TCF *cf,const char *fname)
{
 int i,j,pos;
 char c;
 char fnbuf[1024];
 cf->f=fopen(fname,"rb");
 if (cf->f==NULL) return 0;
 fread (cf->fhead.sign,1,2,cf->f);
 if ((cf->fhead.sign[0]!='C')&&(cf->fhead.sign[1]!='F')) return 0;
 fread (&cf->fhead.nf,4,1,cf->f);
 if (Mem_DAlloc ((void*)&cf->ftab,cf->fhead.nf*sizeof(TFTab))==0) return -1;
 //wczytywanie rekordow
 for (i=0; i<cf->fhead.nf; i++)
 {
  if (feof(cf->f)) return -1;
  j=0;
  while (1)
  {
   fread (&c,1,1,cf->f);
   fnbuf[j++]=c;
   if (c==0) break;
   if (feof(cf->f)) return -1;
  }
  if (Mem_DAlloc((void*)&cf->ftab[i].name,j)==0) return -1;
  strcpy(cf->ftab[i].name,fnbuf);
  fread (&pos,4,1,cf->f);
  cf->ftab[i].pos=pos;
 }
 cf->currfile=0;
 cf->isloaded=CF_LOADED;
 cf->fo=NULL;
 return 1;
}

/*
########################################
 CF_Close
 Zamyka glowny plik i zwalnia pamiec
 Jesli ok 1 : 0
########################################
*/

int CF_Close(TCF *cf)
{
 int i;
 if ((cf==NULL)||(cf->isloaded!=CF_LOADED)) return 0;
 fclose (cf->f);
 cf->f=NULL;
 if (cf->fo!=NULL) {close (cf->fo); cf->fo=NULL;}
 for (i=cf->fhead.nf-1; i>=0; i--)
 {
  if (Mem_DFree ((void*)&cf->ftab[i].name)==0) return 0;
 }
 if (Mem_DFree((void*)&cf->ftab)==0) return 0;
 cf->isloaded=0;
 return 1;
}

/*
########################################
 CF_Read
 Czyta z okreslonego pliku
 Podana nazwa jest nazwa pliku w pliku glownym.
 Jesli pierwszy parametr to null lub ..\nazwa to odczytuje z plku dyskowego
 Parametr s dotyczy przesuniecia w pliku.
 Jesli ok liczba wczytanych bajtow : -1
########################################
*/

int CF_Read (TCF *cf,void *buf,int s,int n,const char *fn)
{
 int i;
 if ((cf==NULL)||((fn[0]=='.')&&(fn[1]=='.')&&((fn[2]=='\\')||(fn[2]=='/'))))
 {
  if ((fn[0]=='.')&&(fn[1]=='.')) fn+=3;
  if (cf->fo!=NULL) close (cf->fo);
  cf->fo=fopen (fn,"rb");
  if (cf->fo==NULL) return -1;
  fseek(cf->fo,s,SEEK_SET);
  int r=fread (buf,1,n,cf->fo);
  close (cf->fo);
  cf->fo=NULL;
  return r;
 }
 if (cf->isloaded!=CF_LOADED) return -1;
 if (cf->fo!=NULL) {close (cf->fo); cf->fo=NULL;}
 for (i=0; i<cf->fhead.nf; i++)
 {
  if (strcmp(fn,cf->ftab[i].name)==0)                     //plik znaleziony
  {
   if (fseek(cf->f,cf->ftab[i].pos+s,SEEK_SET)!=0) return -1;
   if (i<(cf->fhead.nf-1))
      if ((cf->ftab[i+1].pos-cf->ftab[i].pos-s)<n) n=cf->ftab[i+1].pos-cf->ftab[i].pos-s;
   cf->currfile=i;
   return fread(buf,1,n,cf->f);
  }
 }
 return -1;
}

/*
########################################
 CF_Curr
 Ustawia dany plik jako aktualny
 Jesli ok 1 : Brak pliku 0 : -1
########################################
*/

int CF_Curr(TCF *cf,const char *fn)
{
 int i;
 if (cf==NULL) return -1;
 if ((fn[0]=='.')&&(fn[1]=='.')&&((fn[2]=='\\')||(fn[2]=='/')))
 {
  if ((fn[0]=='.')&&(fn[1]=='.')) fn+=3;
  if (cf->fo!=NULL) close (cf->fo);
  cf->fo=fopen (fn,"rb");
  if (cf->fo==NULL) return 0;
  return 1;
 }
 if (cf->isloaded!=CF_LOADED) return -1;
 if (cf->fo!=NULL) {close (cf->fo); cf->fo=NULL;}
 for (i=0; i<cf->fhead.nf; i++)
 {
  if (strcmp(fn,cf->ftab[i].name)==0)                     //plik znaleziony
  {
   if (fseek(cf->f,cf->ftab[i].pos,SEEK_SET)!=0) return -1;
   cf->currfile=i;
   return 1;
  }
 }
 return 0;
}

/*
########################################
 CF_CRead
 Kontynuje czytanie z aktualnego pliku
 Jesli ok liczba wczytanych bajtow : -1
########################################
*/

int CF_CRead (TCF *cf,void *buf,int s,int n)
{
 if (cf==NULL) return -1;
 if (cf->fo!=NULL)
 {
  fseek(cf->fo,s,SEEK_CUR);
  int r=fread (buf,1,n,cf->fo);
  return r;
 }
 if (cf->isloaded!=CF_LOADED) return -1;
 if (s!=0) fseek (cf->f,s,SEEK_CUR);
 int pos=ftell(cf->f);
 if (cf->currfile<cf->fhead.nf-1)
      if ((cf->ftab[cf->currfile+1].pos-pos)<n)
         n=cf->ftab[cf->currfile+1].pos-pos;
 return fread(buf,1,n,cf->f);
}

/*
########################################
 CF_CSeek
 Przeskakuje na pozycje s w aktualnym pliku
 Jesli ok 1 : 0
########################################
*/

int CF_CSeek (TCF *cf,int s)
{
 if (cf==NULL) return -1;
 if (cf->fo!=NULL)
 {
  if (fseek (cf->fo,s,SEEK_SET)!=0) return 0;
  return 1;
 }
 if (cf->isloaded!=CF_LOADED) return -1;
 if (s!=0) if (fseek (cf->f,cf->ftab[cf->currfile].pos+s,SEEK_SET)!=0) return 0;
 return 1;
}

#endif
