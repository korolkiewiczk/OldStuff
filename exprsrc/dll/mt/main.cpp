/*
Biblioteka obslugi macierzy
*/

#define NM 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.c"
#include "clib.h"
#include "env.h"
#include "mt.h"
#include "mathx86.h"

ENV expr_env;

//dane macierze
MT mt[NM];
#define SETM(NO,I,J,V) (*(mt[(NO)%NM].tab+(J)*mt[NO%NM].n+(I))=V)
#define GETM(NO,I,J) (*(mt[(NO)%NM].tab+(J)*mt[NO%NM].n+(I)))

/*
  FUNKCJE
*/

//tworzy macierz mxn z zerami
int Zeros(int no,int n,int m)
{
 if (*expr_env.MultiExec==1) return 0;
 if (mt[no%NM].tab) delete mt[no%NM].tab;
 mt[no%NM].m=m;
 mt[no%NM].n=n;
 mt[no%NM].tab=new float [mt[no%NM].m*mt[no%NM].n];
 if (mt[no%NM].tab==0) return 0;
 memset(mt[no%NM].tab,0,mt[no%NM].m*mt[no%NM].n*sizeof(float));
}

//tworzy macierz jednostkowa nxn
int Eye(int no,int n)
{
 if (*expr_env.MultiExec==1) return 0;
 if (mt[no%NM].tab==0)
 {
  delete mt[no%NM].tab;
 }
 Zeros(no,n,n);
 for (int i=0; i<n; i++) SETM(no,i,i,1);
 return 1;
}

//tworzy macierz z ciagu znakow stosujac konwersje podobna do MATLAB'a
//np. "1 5 3; 2 3 2;" tworzy macierz 2x3
//o wartosci n decyduje pierwszy wiersz
int Def(int no,char *s)
{
 if (*expr_env.MultiExec==1) return 0;
 
 //oposc spacje i "entery" na poczatku
 while ((*s!=0)&&((*s==' ')||(*s=='\n'))) s++;
 
 int i=0;
 //oblicz n
 mt[no%NM].n=0;
 mt[no%NM].m=0;
 int isnum=0;
 while (1) 
 {
  if ( ((s[i]>='0')&&(s[i]<='9')) || (s[i]=='.') || (s[i]=='-') ) isnum++;
  else
  if (s[i]==' ') isnum=0;
  else
  if ((s[i]==0)||(s[i]==';')) 
  break;
  else
  return 0; //nieprawidlowy znak
  
  if (isnum==1)
  {
   isnum++;
   mt[no%NM].n++;
  }
  i++;
 }
 
 //oblicz m na podstawie ilosci znakow ;
 i=0;
 while (s[i]!=0)
 {
  if (s[i]==';') mt[no%NM].m++;
  i++;
 }
 
 
 //tworz macierz
 Zeros(no,mt[no%NM].n,mt[no%NM].m);
 
 int mi=0,mj=0;
 char numbuf[256];
 i=0;
 int j=0;
 int space=0;
 while (1)
 {
  if ( ((s[i]>='0')&&(s[i]<='9')) || (s[i]=='.') || (s[i]=='-') ) 
  {
   numbuf[j++]=s[i];
   space=0;
  }
  else
  if (((s[i]==' ')||(s[i]==';'))&&(space==0))
  {
   numbuf[j]=0;
   j=0;
   if ((mj>=mt[no%NM].n)||(mi>=mt[no%NM].m)) goto delmat;
   SETM(no,mj,mi,atof(numbuf));
   if (s[i]==';')
   {
    mj=0;
    mi++;
   }
   else
   mj++;
   space++;
  }
  else
  if (s[i]==0) break;
  i++;
 }
 
 return 1;
 
delmat:
 delete mt[no%NM].tab;
 mt[no%NM].n=0;
 mt[no%NM].m=0;
 return 0;
}

//laduje macierz z pliku tekstowego
int Load(int no,char *fn)
{
 if (*expr_env.MultiExec==1) return 0;
 FILE *f=expr_env.CSys_FOpen(fn,"rt");
 if (!f) return 0;
 fseek(f,0,SEEK_END);
 int fs=ftell(f);
 fseek(f,0,SEEK_SET);
 char *bf;
 bf=new char [fs];
 fread(bf,1,fs,f);
 int r=Def(no,bf);
 delete bf;
 return r;
}

//ustawia komorke macierzy [i,j] na v (indeksowanie od 0)
float Set(int no,int j,int i,float v)
{
 if ((i<0)||(j<0)||(i>=mt[no%NM].m)||(j>=mt[no%NM].n)) return 0;
 SETM(no,j,i,v);
 return v;
}

//pobiera komorke macierzy [i,j] (indeksowanie od 0)
float Get(int no,float j,float i)
{
 if ((i<0)||(j<0)||(i>=mt[no%NM].m)||(j>=mt[no%NM].n)) return 0;
 return GETM(no,ifloor(j),ifloor(i));
}

//pobiera komorke macierzy [i,j] z liniowa interpolacja (indeksowanie od 0)
float LGet(int no,float j,float i)
{
 int m=mt[no%NM].m;
 int n=mt[no%NM].n;
 if ((i<0)||(j<0)||(i>=m)||(j>=mt[no%NM].n)) return 0;
 int ij=ifloor(j),ii=ifloor(i);
 float x1=j-ij,y1=i-ii;
 float v1=GETM(no,ij,ii);
 float v2=(ij>=n)? 0:GETM(no,ij+1,ii);
 float v3=((ij>=n)||(ii>=m))? 0:GETM(no,ij+1,ii+1);
 float v4=(ii>=m)? 0:GETM(no,ij,ii+1);
 return (v1*(1-x1)+v2*x1)*(1-y1)+(v3*x1+v4*(1-x1))*y1;
}

/*
//generuje macierz opisana funkcja (na liczbach naturalnych)
int Func(int no,int n,int m,char *f)
{
 
}*/

//kopiuje macierz m2 do m1
int Copy(int m1,int m2)
{
 if ( (mt[m1%NM].n!=mt[m2%NM].n) || (mt[m1%NM].m!=mt[m2%NM].m)) return 0;
 memcpy(mt[m1%NM].tab,mt[m2%NM].tab,mt[m1%NM].n*mt[m1%NM].m*sizeof(float));
 return 1;
}

//m1=m2+m3
int Add(int m1,int m2,int m3)
{
 if ( (mt[m2%NM].n!=mt[m3%NM].n) || (mt[m2%NM].m!=mt[m3%NM].m)) return 0;
 int n=mt[m2%NM].n;
 int m=mt[m2%NM].m;
 int pomm=m1;
 if ((m1==m2)||(m1==m3))
 {
  m1=NM-1;
  Zeros(m1,n,m);
 }
 for (int i=0; i<m; i++)
 for (int j=0; j<n; j++) SETM(m1,j,i,GETM(m2,j,i)+GETM(m3,j,i));
 if (pomm!=m1)
 {
  Copy(pomm,m1);
 }
 return 1;
}

//mnorzy macierz m1 przez skalar s
int SMul(int m1,float s)
{
 int n=mt[m1%NM].n;
 int m=mt[m1%NM].m;
 for (int i=0; i<m; i++)
 for (int j=0; j<n; j++) SETM(m1,j,i,GETM(m1,j,i)*s);
 return 1;
}

//m1=m2*m3
int Mul(int m1,int m2,int m3)
{
 if (mt[m2%NM].n!=mt[m3%NM].m) return 0;
 int r=mt[m2%NM].n;
 int a=mt[m2%NM].m;
 int b=mt[m3%NM].n;
 
 int pomm=m1;
 if ((m1==m2)||(m1==m3))
 {
  m1=NM-1;
 }
 Zeros(m1,b,a);
 
 for (int i=0; i<a; i++)
 for (int j=0; j<b; j++)
 {
  float sum=0;
  for (int k=0; k<r; k++)
  {
   sum+=GETM(m2,k,i)*GETM(m3,j,k);
  }
  SETM(m1,j,i,sum);
 }
 
 if (pomm!=m1)
 {
  Copy(pomm,m1);
 }
 return 1;
}

//wyswietla macierz no
int Print(int no)
{
 if (*expr_env.MultiExec!=0) return 0;
 int n=mt[no%NM].n;
 int m=mt[no%NM].m;
 char bf1[32],bf2[256];
 for (int i=0; i<m; i++)
 {
  bf2[0]=0;
  int l=0;
  for (int j=0; j<n; j++)
  {
   sprintf(bf1,"%0.4f  ",GETM(no,j,i));
   l+=strlen(bf1);
   if (l<256)
   strcat(bf2,bf1);
   else break;
  }
  expr_env.expr_conprint(bf2);
 }
 return 1;
}

//Wczytuje macierz z piku bmp
int FromBMP(int no,char *fn)
{
 if (*expr_env.MultiExec) return 0;
 FILE *f;
 char sign[2];
 unsigned short bpp;
 int imgstart,i,j,s;
 unsigned char BGR[4];
 unsigned int width,height;
 float *wsk;
 
 f=expr_env.CSys_FOpen(fn,"rb");
 if (f==0) return 0;
 fread(&sign,1,2,f);
 if ((sign[0]!='B')&&(sign[1]!='M')) {fclose(f); return 0;}
 fseek(f,0xa,SEEK_SET);
 fread(&imgstart,1,4,f);
 fseek(f,0x1c,SEEK_SET);
 fread(&bpp,1,2,f);
 if (bpp!=24) {fclose(f); return 0;}
 fseek(f,0x12,SEEK_SET);
 fread(&width,1,4,f);
 fread(&height,1,4,f);
 
 Zeros(no,width,height);
 
 fseek(f,imgstart,SEEK_SET);
 wsk=(float*)mt[no%NM].tab;
 int extrabits=4-((width*3)%4);
 for (i=0; i<height; i++)
 {
  float *pwsk;
  pwsk=wsk;
  for (j=0; j<width; j++)
  {
   fread(&BGR,1,3,f);
   *wsk=(BGR[2]+BGR[1]+BGR[0])*(1.f/255.f/3.f); //srednia
   wsk++;
  }
  if (extrabits!=4) fread(&BGR,1,extrabits,f);
  wsk=(pwsk+width);
 }
 fclose(f);
 return 1;
}

//m1=m2^-1 - metoda Gaussa
int Inv(int m1,int m2)
{
 if (mt[m2%NM].n!=mt[m2%NM].m) return 0;
 int n=mt[m2%NM].n;
 
 int pomm=m1;
 int eyem=NM-1;
 if (m1==m2)
 {
  m1=NM-1;
  eyem=NM-2;
 }
 Zeros(m1,n,n);
 Copy(m1,m2);
 Eye(eyem,n);
 int i,j,k;
 
 for (i=0; i<n; i++)
 {
  float divv=GETM(m1,i,i);
  if (divv==0)
  //zamien odpowiednie wiersze albo zwroc blad
  {
   for (j=i+1; j<=n; j++)
   {
    if (j==n) return 0;
    if (GETM(m1,i,j)!=0)
    {
     for (k=0; k<n; k++)
     {
      float pom;
      pom=GETM(m1,k,j);
      SETM(m1,k,j,GETM(m1,k,i));
      SETM(m1,k,i,pom);
     }
     for (k=0; k<n; k++)
     {
      float pom;
      pom=GETM(eyem,k,j);
      SETM(eyem,k,j,GETM(eyem,k,i));
      SETM(eyem,k,i,pom);
     }
     divv=GETM(m1,i,i);
     break;
    }
   }
  }
  
  //podziel wiersz i przez m1[i,i]
  for (j=0; j<n; j++)
  {
   SETM(m1,j,i,GETM(m1,j,i)/divv);
  }
  for (j=0; j<n; j++)
  {
   SETM(eyem,j,i,GETM(eyem,j,i)/divv);
  }
  
  //wyruguj wartosci z wierszy 0:i-1
  for (j=0; j<i; j++)
  {
   float rv=GETM(m1,i,j);
   for (k=i; k<n; k++)
   {
    SETM(m1,k,j,GETM(m1,k,j)-rv*GETM(m1,k,i));
   }
   for (k=0; k<n; k++)
   {
    SETM(eyem,k,j,GETM(eyem,k,j)-rv*GETM(eyem,k,i));
   }
  }
  //wyruguj wartosci z wierszy i+1:n
  for (j=i+1; j<n; j++)
  {
   float rv=GETM(m1,i,j);
   for (k=i; k<n; k++)
   {
    SETM(m1,k,j,GETM(m1,k,j)-rv*GETM(m1,k,i));
   }
   for (k=0; k<n; k++)
   {
    SETM(eyem,k,j,GETM(eyem,k,j)-rv*GETM(eyem,k,i));
   }
  }
  
 }
 Copy(m1,eyem);
 return 1;
}

/*
  BIBLIOTEKA FUNKCJI
*/

ELEMENT dll_lib[]=
{
 {"MT_SET",(void*)Set,VAL_FLOAT,4,VAL_INT+VAL_INT*4+VAL_INT*16+VAL_FLOAT*64,0},
 {"MT_GET",(void*)Get,VAL_FLOAT,3,VAL_INT,0},
 {"MT_LGET",(void*)LGet,VAL_FLOAT,3,VAL_INT,0},
 {"MT_DEF",(void*)Def,VAL_INT,2,VAL_INT+VAL_STR*4,0},
 {"MT_LOAD",(void*)Load,VAL_INT,2,VAL_INT+VAL_STR*4,0},
 {"MT_EYE",(void*)Eye,VAL_INT,2,VAL_INT+VAL_INT*4,0},
 {"MT_ZEROS",(void*)Zeros,VAL_INT,3,VAL_INT+VAL_INT*4+VAL_INT*16,0},
 {"MT_ADD",(void*)Add,VAL_INT,3,VAL_INT+VAL_INT*4+VAL_INT*16,0},
 {"MT_SMUL",(void*)SMul,VAL_INT,2,VAL_INT,0},
 {"MT_MUL",(void*)Mul,VAL_INT,3,VAL_INT+VAL_INT*4+VAL_INT*16,0},
 {"MT_PRINT",(void*)Print,VAL_INT,1,VAL_INT,0},
 {"MT_LOADBMP",(void*)FromBMP,VAL_INT,2,VAL_INT+VAL_STR*4,0},
 {"MT_CPY",(void*)Copy,VAL_INT,2,VAL_INT+VAL_INT*4,0},
 {"MT_INV",(void*)Inv,VAL_INT,2,VAL_INT+VAL_INT*4,0}
};


/*
  FUNKCJA REFERENCYJNA
*/

extern "C"
{
 __declspec (dllexport) int RefFunc(void **lib,ENV *env)
 {
  memset(mt,0,sizeof(mt));
  memcpy(&expr_env,(void*)env,sizeof(ENV));
  *lib=dll_lib;
  return sizeof(dll_lib)/sizeof(ELEMENT);
 }
}
