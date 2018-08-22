#ifndef __VB_MEM_C
#define __VB_MEM_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_MEM.C
 Opis: Obsluga pamieci.
------------------------------------------------
*/

//===========================================

#include "vb_mem.h"

//===========================================

int mem_buf[MEM_BUFMAX][2]; //[n][0]-adres do wskaznika,[n][1]-wartosc wskaznika
int mem_wsk;
int mem_initialized;
int mem_size;
void *mem_sheapwsk;
void *mem_dheapwsk;
void *mem_sheaptop;
void *mem_dheaptop;

//===========================================

/*
########################################
 InitMem
 Inicjuje ns bajtow pamieci statycznej i nd dynamicznej.
 Wywolanie tej procedury jest zawsze konieczne przed kazda alokacja pamieci.
 Jesli wszystko ok to 1 : 0
########################################
*/

int InitMem (int ns,int nd)
{
 void *hla;
 if (mem_initialized) return 0;
 mem_size=ns+nd;
 /*if (InitHeapMem (mem_size)==0) return 0; //HEAP_LIN_ADDR
 __dpmi_set_segment_limit(_my_ds(),__dpmi_get_segment_limit(_my_ds())+mem_size);*/
 hla=(void*)sbrk(mem_size);
 mem_wsk=0;
 mem_initialized=1;
 mem_sheapwsk=hla;
 mem_dheapwsk=mem_sheaptop=hla+ns;
 mem_dheaptop=mem_dheapwsk+nd;
 return 1;
}

/*
########################################
 CloseMem
 Zwalnia pamiec przeznaczona dla alokacji. Wywolanie tej procedury
 jest konieczne na koncu dzialania programu.
 Jesli wszystko ok to 1 : 0
########################################
*/

int CloseMem ()
{
 if (mem_initialized==0) return 0;
 //if (FreeHeapMem ()==0) return 0;
 mem_initialized=0;
 return 1;
}

/*
########################################
 Mem_SAlloc
 Alokuje n bajtow pamieci statycznej
 Nie mozna tej pamieci juz zwalniac
 Jesli ok adres : NULL
########################################
*/

void *Mem_SAlloc (int n)
{
 void *r;
 if (mem_initialized==0) return NULL;
 if (mem_sheapwsk+n>=mem_sheaptop) return NULL;
 r=mem_sheapwsk;
 mem_sheapwsk+=n;
 return r;
}

/*
########################################
 Mem_DAlloc
 Alokuje n bajtow pamieci dynamicznej.
 To pamiec mozna potem zwolnic.
 Wywolanie: Mem_Alloc (&wskaznik,nbytes);
 Jesli ok to zwraca 1 : 0 oraz wsk=adres : NULL
########################################
*/

int Mem_DAlloc (void **wsk,int n)
{
 if ((mem_initialized==0)||(mem_wsk==MEM_BUFMAX)) {*wsk=NULL; return 0;}
 if (mem_dheapwsk+n>=mem_dheaptop) {*wsk=NULL; return 0;}
 *wsk=mem_dheapwsk;
 mem_dheapwsk+=n;
 mem_buf[mem_wsk][0]=(int)wsk;
 mem_buf[mem_wsk++][1]=(int)*wsk;
 return 1;
}

/*
########################################
 Mem_DPtr
 Tworzy wskaznik na podany adres (adres musi byc juz zaalaokowany)
 Jesli ok 1 : 0
########################################
*/

int Mem_DPtr(void **wsk,void *w)
{
 int i,c;
 int l=0,p=mem_wsk-1;
 if ((mem_initialized==0)||(mem_wsk==MEM_BUFMAX)) {*wsk=NULL; return 0;}
 if (mem_wsk==0) return -1;
 while (l<=p)
 {
  c=(l+p)/2;
  if (mem_buf[c][1]==(int)w) break;
  if ((int)w<mem_buf[c][1]) p=c-1;
  else l=c+1;
 }
 if (l>p) return 0;
 *wsk=w;//(void*)mem_buf[c][1];
 for (i=mem_wsk;i>c;i--)
 {
  memcpy(mem_buf[i],mem_buf[i-1],8);
 }
 mem_wsk++;
 mem_buf[c][0]=(int)wsk;
 mem_buf[c][1]=(int)*wsk;
 return 1;
}

/*
########################################
 Mem_FindAddr
 Sprawdza czy podany adres byl juz zaalokowany. Jest to funkcja pomocnicza
 dla Mem_Free.
 Jesli tak to zwraca numer komurki mem_buf : -1
########################################
*/

int Mem_FindAddr (void **wsk)
{
 int l=0,p=mem_wsk,s;
 if (mem_wsk==0) return -1;
 while (l<=p)
 {
  s=(l+p)/2;
  if (mem_buf[s][1]==(int)*wsk) break;
  if ((int)*wsk<=mem_buf[s][1]) p=s-1;
  else l=s+1;
 }
 if (mem_buf[s][0]==(int)wsk) return s;
 l=s-1;
 while ((l>=0)&&(mem_buf[l][1]==(int)*wsk)) if (mem_buf[l][0]==(int)wsk) return l;
 else l--;
 p=s+1;
 while ((p<mem_wsk)&&(mem_buf[p][1]==(int)*wsk)) if (mem_buf[p][0]==(int)wsk) return p;
 else p++;
 return -1;
}

/*
########################################
 Mem_DFree
 Zwalnia pamiec dynamiczna
 Jesli ok 1 i wsk=NULL : 0
########################################
*/

int Mem_DFree (void **wsk)
{
 void *wsk1,*wsk2;
 if (mem_initialized==0) return 0;
 int pam,i,pom;
 int addrwsk=Mem_FindAddr(wsk);
 if (addrwsk==-1) return 0;
 wsk1=(int *)mem_buf[addrwsk][1];
 if (addrwsk==mem_wsk-1)
 {
  *wsk=NULL;
  mem_dheapwsk=wsk1;
  mem_wsk--;
  return 1;
 }
 else
 {
  *wsk=NULL;
  wsk2=(int *)mem_buf[addrwsk+1][1];
  pom=(int)wsk2-(int)wsk1;
  for (i=0; i<addrwsk; i++)
   if (mem_buf[i][0]>=(int)wsk2) mem_buf[i][0]-=pom;
  for (i=addrwsk; i<mem_wsk-1; i++)
  {
   if (mem_buf[i+1][0]>=(int)wsk2)
   mem_buf[i][0]=mem_buf[i+1][0]-pom;
   else
   mem_buf[i][0]=mem_buf[i+1][0];
   mem_buf[i][1]=mem_buf[i+1][1]-pom;
   *(int *)(mem_buf[i+1][0])=mem_buf[i][1];     //tego w pascalu bym nie zrobil ;)
  }
  if (pom)
  memcpy (wsk1,wsk2,mem_dheapwsk-wsk2);
  mem_dheapwsk-=pom;
  mem_wsk--;
 }
 return 1;
}

#endif
