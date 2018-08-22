#ifndef __VB_VR_C
#define __VB_VR_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_VR.C
 Opis: Obsluga nazw zmiennych
------------------------------------------------
*/

//==========================================

#include "vb_vr.h"

//==========================================

TVarRefTab *vr_tab;

/*
########################################
 InitVR
 Inicjuje modul obslugi nazw zmiennych
 Jesli inicjacja sie powiodla to 1 : 0
########################################
*/

int InitVR ()
{
 int i;
 if (Mem_DAlloc ((void*)&vr_tab,sizeof (TVarRefTab))==0) return 0;
 for (i=0; i<VR_MAXCHARS; i++) vr_tab->namechar[i]=NULL;
 for (i=0; i<VR_MAXCHARS; i++) vr_tab->prevchar[i]=NULL;
 vr_tab->v=NULL;
 return 1;
}

/*
########################################
 VR_CIndex
 Funkcja pomocnicza ktora zwraca indeks dla danego znaku
 -1 jesli niepoprawny znak, VR_MAXCHARS jesli znak pusty
########################################
*/

int VR_CIndex (char c)
{
 if (c==0) return VR_MAXCHARS;
 if ((c>='0')&&(c<='9')) return c-'0'+26;
 if (c=='_') return 36;
 c=toupper (c);
 if ((c>='A')&&(c<='Z')) return c-'A';
 return -1;
}

/*
########################################
 VR_New
 Dodaje nazwe
 Jesli ok to 1 : 0 zas jesli nazwa juz istniala to -1
########################################
*/

int VR_New (char *name,int type,void *ref)
{
 unsigned char buf[1024];
 int i=0,c=0,j,k;
 //czy nazwa jest poprawna
 while (1)
 {
  c=VR_CIndex (name[i]);
  if (c==-1) return 0;
  if (c==VR_MAXCHARS) break;
  buf[i++]=c;
 }
 if (i==0) return 0;    //nie sa uwzgledniane nazwy puste
 //wprowadzamy nazwe
 TVarRefTab *vrt=vr_tab,*pom;
 for (j=0; j<i; j++)
 {
  c=buf[j];
  if (vrt->namechar[c]==NULL)    //jesli litera nie posiada juz odgalezien
  {
   pom=vrt;
   if (Mem_DAlloc ((void*)&vrt->namechar[c],sizeof (TVarRefTab))==0) return 0;
   vrt=vrt->namechar[c];
   for (k=0; k<VR_MAXCHARS; k++) vrt->namechar[k]=NULL;
   for (k=0; k<VR_MAXCHARS; k++) vrt->prevchar[k]=NULL;
   if (Mem_DPtr((void*)&vrt->prevchar[c],pom)==0) return 0;
   vrt->v=NULL;
  }
  else vrt=vrt->namechar[c];
 }
 if (vrt->v==NULL)
 {
  if (Mem_DAlloc((void*)&vrt->v,sizeof(TVarRef))==0) return 0;
  vrt->v->type=type;
  vrt->v->ref=ref;
 }
 else return -1;
 return 1;
}

/*
########################################
 VR_Delete
 Kasuje nazwe
 Jesli ok 1 : 0
########################################
*/

int VR_Delete(char *name)
{
 int i=0,c,j,k,am=(int)mem_dheapwsk;
 TVarRefTab *vrt=vr_tab,*pom;
 while (1)
 {
  c=VR_CIndex(name[i++]);
  if (c==-1) return NULL;
  if (c==VR_MAXCHARS) break;
  if (vrt->namechar[c]==NULL) return 0;
  vrt=vrt->namechar[c];
 }
 if (vrt->v==NULL) return 0;
 if (Mem_DFree((void*)&vrt->v)==0) return 0;
 i--;
 while (1)
 {
  j=0;
  for (k=0; k<VR_MAXCHARS; k++) if (vrt->namechar[k]!=NULL) j++;
  if (j>0) break;
  c=VR_CIndex(name[--i]);
  pom=vrt->prevchar[c];
  if (pom==NULL) break;
  if (Mem_DFree((void*)&vrt->prevchar[c])==0) return -1;
  vrt=pom;
  if (Mem_DFree((void*)&vrt->namechar[c])==0) return -2;
 }
 //Con_Printf("%d",am-(int)mem_dheapwsk);
 return 1;
}

/*
########################################
 VR_FindName
 Sprawdza czy dana nazwa istnieje
 Zwraca adres do struktory TVarRef : NULL.
########################################
*/

TVarRef *VR_FindName (char *name)
{
 int i=0,c;
 TVarRefTab *vrt=vr_tab;
 while (1)
 {
  c=VR_CIndex(name[i++]);
  if (c==-1) return NULL;
  if (c==VR_MAXCHARS) break;
  if (vrt->namechar[c]==NULL) return NULL;
  vrt=vrt->namechar[c];
 }
 return vrt->v;
}

/*
########################################
 VR_Set_f
 Ustawia wartosc danej nazwy typu float
 Jesli nazwa istnieje to 1 : 0
########################################
*/

int VR_Set_f (char *name,float a)
{
 TVarRef *v;
 v=VR_FindName(name);
 if (v==NULL) return 0;
 memcpy(v->ref,&a,sizeof(float));
 return 1;
}

/*
########################################
 VR_Set_d
 Ustawia wartosc danej nazwy typu double
 Jesli nazwa istnieje to 1 : 0
########################################
*/

int VR_Set_d (char *name,double a)
{
 TVarRef *v;
 v=VR_FindName(name);
 if (v==NULL) return 0;
 memcpy(v->ref,&a,sizeof(double));
 return 1;
}

/*
########################################
 VR_Set_i
 Ustawia wartosc danej nazwy typu int
 Jesli nazwa istnieje to 1 : 0
########################################
*/

int VR_Set_i (char *name,int a)
{
 TVarRef *v;
 v=VR_FindName(name);
 if (v==NULL) return 0;
 memcpy(v->ref,&a,sizeof(int));
 return 1;
}

/*
########################################
 VR_Set
 Ustawia wartosc danej nazwy dowolnego typu n r typu type
 Jesli nazwa istnieje to 1 : 0
########################################
*/

int VR_Set (char *name,void *r,int type)
{
 TVarRef *v;
 int size,ival;
 float fval;
 double dval;
 void *ref;
 v=VR_FindName(name);
 if (v==NULL) return 0;
 switch (v->type)
 {
  case VR_INT:
                 if (type==VR_INT)
                 ival=*(int*)r;
                 else
                 if (type==VR_FLOAT)
                 ival=*(float*)r;
                 else
                 if (type==VR_DOUBLE)
                 ival=*(double*)r;
                 ref=&ival;
                 size=4;
                 break;
                
  case VR_FLOAT:
                 if (type==VR_INT)
                 fval=*(int*)r;
                 else
                 if (type==VR_FLOAT)
                 fval=*(float*)r;
                 else
                 if (type==VR_DOUBLE)
                 fval=*(double*)r;
                 ref=&fval;
                 size=4;
                 break;
  case VR_DOUBLE:
                 if (type==VR_INT)
                 dval=*(int*)r;
                 else
                 if (type==VR_FLOAT)
                 dval=*(float*)r;
                 else
                 if (type==VR_DOUBLE)
                 dval=*(double*)r;
                 ref=&dval;
                 size=8;
                 break;
 }
 memcpy(v->ref,ref,size);
 return 1;
}

/*
########################################
 VR_Get_f
 Zwraca wartosc danej nazwy typu float
########################################
*/

float VR_Get_f (char *name)
{
 TVarRef *v;
 float a;
 v=VR_FindName(name);
 if (v==NULL) return 0;
 memcpy(&a,v->ref,sizeof(float));
 return a;
}

/*
########################################
 VR_Get_d
 Zwraca wartosc danej nazwy typu double
########################################
*/

double VR_Get_d (char *name)
{
 TVarRef *v;
 double a;
 v=VR_FindName(name);
 if (v==NULL) return 0;
 memcpy(&a,v->ref,sizeof(double));
 return a;
}

/*
########################################
 VR_Get_i
 Zwraca wartosc danej nazwy typu int
########################################
*/

int VR_Get_i (char *name)
{
 TVarRef *v;
 int a;
 v=VR_FindName(name);
 if (v==NULL) return 0;
 memcpy(&a,v->ref,sizeof(int));
 return a;
}

/*
########################################
 VR_Get
 Pobiera wartosc danej nazwy dowolnego typu do r o typie type
 Jesli nazwa istnieje to 1 : 0
########################################
*/

int VR_Get (char *name,void *r,int type)
{
 TVarRef *v;
 int size,ival;
 float fval;
 double dval;
 void *ref;
 v=VR_FindName(name);
 if (v==NULL) return 0;
 switch (type)
 {
  case VR_INT:
                 if (v->type==VR_INT)
                 ival=*(int*)v->ref;
                 else
                 if (v->type==VR_FLOAT)
                 ival=*(float*)v->ref;
                 else
                 if (v->type==VR_DOUBLE)
                 ival=*(double*)v->ref;
                 ref=&ival;
                 size=4;
                 break;
                
  case VR_FLOAT:
                 if (v->type==VR_INT)
                 fval=*(int*)v->ref;
                 else
                 if (v->type==VR_FLOAT)
                 fval=*(float*)v->ref;
                 else
                 if (v->type==VR_DOUBLE)
                 fval=*(double*)v->ref;
                 ref=&fval;
                 size=4;
                 break;
  case VR_DOUBLE:
                 if (v->type==VR_INT)
                 dval=*(int*)v->ref;
                 else
                 if (v->type==VR_FLOAT)
                 dval=*(float*)v->ref;
                 else
                 if (v->type==VR_DOUBLE)
                 dval=*(double*)v->ref;
                 ref=&dval;
                 size=8;
                 break;
 }
 memcpy(r,ref,size);
 return 1;
}

#endif
