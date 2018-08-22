#ifndef __VB_EXPR_C
#define __VB_EXPR_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_EXPR.C
 Opis: Oblicznie wyrazen, wywolywanie komend.
------------------------------------------------
*/

//===========================================

#include "vb_expr.h"
#include "vb_opf.c"
#include "stdf.c"

//===========================================

TOp expr_operands[OP_MAXOPERANDS]=
{
 {'+',2,10,&Op_Add},
 {'-',2,10,&Op_Sub},
 {'*',2,20,&Op_Mul},
 {'/',2,20,&Op_Div},
 {'^',2,30,&Op_Pow},
 {'!',1,90,&Op_Sil},
 {'%',2,20,&Op_Mod}
};

TFVTab *expr_fv;

char expr_strbuf[EXPR_STRBUFLEN];
int expr_error=1;

//===========================================

/*
########################################
 InitExpr
 Inicjuje modul obslugi wyrazen
 Jesli inicjacja sie powiodla to 1 : 0
########################################
*/

int InitExpr ()
{
 expr_fv=Mem_SAlloc (sizeof (TFVTab));
 if (expr_fv==NULL) return 0;
 int i;
 for (i=0; i<FVTAB_MAXCHARS; i++) expr_fv->namechar[i]=NULL;
 expr_fv->funcvar=NULL;
 Expr_IncludeLib(stdf,STDF_MAX);
 return 1;
}

/*
########################################
 Expr_FVA2I
 Funkcja pomocnicza ktora zwraca indeks dla danego znaku
 -1 jesli niepoprawny znak, FVTAB_MAXCHARS jesli znak pusty
########################################
*/

int Expr_FVA2I (char c)
{
 if (c==0) return FVTAB_MAXCHARS;
 c=toupper (c);
 if ((c>='A')&&(c<='Z')) return c-'A';
 if ((c>='0')&&(c<='9')) return c-'0'+26;
 if (c=='_') return 36;
 return -1;
}

/*
########################################
 Expr_AddFV
 Dodaje zmienna badz funkcje
 Jesli ok to 1 : 0 zas jesli nazwa juz istniala to -1
########################################
*/

int Expr_AddFV (char *name,TFuncVar *fv)
{
 unsigned char buf[1024];
 int i=0,c=0,j,k;
 //czy nazwa jest poprawna
 while (1)
 {
  c=Expr_FVA2I (name[i]);
  if (c==-1) return 0;
  if (c==FVTAB_MAXCHARS) break;
  buf[i++]=c;
 }
 if (i==0) return 0;    //nie sa uwzgledniane nazwy puste
 //wprowadzamy nazwe
 TFVTab *fvtab=expr_fv,*pom;
 for (j=0; j<i; j++)
 {
  c=buf[j];
  if (fvtab->namechar[c]==NULL)    //jesli litera nie posiada juz odgalezien
  {
   pom=Mem_SAlloc (sizeof (TFVTab));
   if (pom==NULL) return 0;
   fvtab->namechar[c]=pom;
   for (k=0; k<FVTAB_MAXCHARS; k++) pom->namechar[k]=NULL;
   pom->funcvar=NULL;
   fvtab=pom;
  }
  else fvtab=fvtab->namechar[c];
 }
 if (fvtab->funcvar==NULL) fvtab->funcvar=fv;
 else return -1;
 return 1;
}

/*
########################################
 Expr_FindFV
 Sprawdza czy dana zmienna lub funkcja istnieje.
 Zwraca adres do struktory TFuncVar : NULL.
########################################
*/

TFuncVar *Expr_FindFV (char *name)
{
 int i=0,c;
 TFVTab *fvtab=expr_fv;
 while (1)
 {
  c=Expr_FVA2I(name[i++]);
  if (c==-1) return NULL;
  if (c==FVTAB_MAXCHARS) break;
  if (fvtab->namechar[c]==NULL) return NULL;
  fvtab=fvtab->namechar[c];
 }
 return fvtab->funcvar;
}

/*
########################################
 Expr_IncludeLib
 Dodaje zbior n funkcji
 Zwraca ilosc pomyslnie zaladowanych funkcji
########################################
*/

int Expr_IncludeLib (TFVLib *lib,int n)
{
 int i;
 for (i=0; i<n; i++) if (Expr_AddFV ((lib+i)->name,&(lib+i)->fv)==0) break;
 return i;
}

/*
########################################
 Expr_FindOp
 Sprawdza czy podany operand jest obslugiwany.
 Zwraca odpowiedni numer operandu w expr_operands : -1
########################################
*/

int Expr_FindOp (char op)
{
 int i;
 for (i=0; i<OP_MAXOPERANDS; i++) if (expr_operands[i].c==op) return i;
 return -1;
}

/*
########################################
 Expr_CheckErr
 Sprawdza czy byl blad i jesli tak to zwraca nazwe bledu
 Jesli wszystko bylo ok 1 : numerbledu i nazwa bledu w str
########################################
*/

int Expr_CheckErr (char *str)
{
 if (expr_error==EXPR_OK) return 1;
 if (str!=NULL)
 switch (expr_error)
 {
  case EXPRERR_INVOP: strcpy(str,"Invalid operand");
  break;
  case EXPRERR_UNDEFNAME: strcpy(str,"Undefined name");
  break;
  case EXPRERR_TMPARAMS: strcpy(str,"Too many params in function");
  break;
  case EXPRERR_TFPARAMS: strcpy(str,"Too few params in function");
  break;
  case EXPRERR_SYNTAX: strcpy(str,"Syntax error");
  break;
  case EXPRERR_UNDEF:
  default:
  strcpy(str,"Undefined error");
 }
 return expr_error;
}

/*
########################################
 Expr_ONP
 Zamienia wyrazenie na wyrazenie w ONP.
 Dokonuje tez wstepnej analizy wyrazenia.
 Jesli ok 1 : 0
########################################
*/

int Expr_ONP (char *onp,const char *s)
{
 //[][0] dla znaku,[][1] dla priorytetu
 unsigned char stack[EXPR_STACKSIZE][2];
 //bufor pomocniczy
 char sbuf[EXPR_BUFLEN];
 TFuncVar *fv;
 int i=0,j=0,k=0,c,t=0,func=0,np,stop=EXPR_STACKSIZE,isstr=0,op,isvr=0,nwp=0;
 while (1)
 {
  c=s[i++];
  if (c==0) break;
  if (c=='"') if (func<2) return EXPRERR_SYNTAX;        //blad skladni: sds"sdf"+2...
  else isstr=1-isstr;
  if (isstr) {onp[j++]=c; continue;}
  if (c==',') if (func<2) return EXPRERR_SYNTAX;        //blad skladni: abc,2,s...
  else
  {
   if (func==2)
   {
    if (np<1) return EXPRERR_TMPARAMS;                  //za duzo parametrow
    if (nwp) return EXPRERR_SYNTAX;                     //blad skladni: fun( ,)
    nwp=1;
   }
   onp[j++]=',';
   continue;
  }
  if (c==' ')
  {
   if (func>0) onp[j++]=' ';
   else
   if (t==-4)                           //jesli nazwa
   {
    sbuf[k]=0;
    if (isvr==0)
    {
    fv=Expr_FindFV (sbuf);
    if (fv==NULL) return EXPRERR_UNDEFNAME;             //brak nazwy=blad
    if ((fv->funcaddr!=NULL)&&(fv->nparams>0))          //...to funkcja z parametrami
    {
     func=1;
     np=fv->nparams;
     nwp=1;
    }
    } else isvr=0;
    k=0;
    while (sbuf[k]!=0) onp[j++]=sbuf[k++];
    t=-2;
   }
   //t=-2;
   continue;
  }
  c=toupper (c);
  if ((func>0)&&(c!='(')&&(c!=')'))     //jesli funkcja to kopiuj chyba ze nawiasy
  {
   if (func==1) return EXPRERR_SYNTAX;  //blad skladni fun abc (...
   if (nwp) {nwp=0; np--;}
   onp[j++]=c;
   continue;
  }
  if (((c>='0')&&(c<='9'))||(c=='.'))   //cyfry
  {
   if (t==-6)
   {
    op=Expr_FindOp ('*');
    if (t==-1) return EXPRERR_INVOP;    //brak '*' czyli niepowodzenie
    onp[j++]=' ';
    while ((stack[stop][1]>expr_operands[op].pr)&&(stop<EXPR_STACKSIZE))
          onp[j++]=stack[stop++][0];
    stack[--stop][0]=expr_operands[op].c;
    stack[stop][1]=expr_operands[op].pr;
    t=-3;
   }
   if (t!=-4)                           //jesli nie bylo liter
   {
    onp[j++]=c;
    t=-3;
   }
   else                                 //czyli nazwa
   {
    if (c=='.') return EXPRERR_SYNTAX;  //blad skladni: nazwa5.
    sbuf[k++]=c;
   }
  }
  else
  if (((c>='A')&&(c<='Z'))||(c=='_')||(c=='$'))   //funkcje, zmienne
  {
   if (c=='$')
   {
   if (isvr==0) isvr=1;
   if (t==-4)
   {                                    //$a$b = $a*$b
    sbuf[k]=0;
    k=0;
    while (sbuf[k]!=0) onp[j++]=sbuf[k++];
    k=0;
    op=Expr_FindOp ('*');
    if (t==-1) return EXPRERR_INVOP;    //brak '*' czyli niepowodzenie
    onp[j++]=' ';
    while ((stack[stop][1]>expr_operands[op].pr)&&(stop<EXPR_STACKSIZE))
          onp[j++]=stack[stop++][0];
    stack[--stop][0]=expr_operands[op].c;
    stack[stop][1]=expr_operands[op].pr;
   }
   }
   if ((t==-6)||(t==-3))                //jesli 2nazwa to 2*nazwa
   {
    op=Expr_FindOp ('*');
    if (t==-1) return EXPRERR_INVOP;    //brak '*' czyli niepowodzenie
    onp[j++]=' ';
    while ((stack[stop][1]>expr_operands[op].pr)&&(stop<EXPR_STACKSIZE))
          onp[j++]=stack[stop++][0];
    stack[--stop][0]=expr_operands[op].c;
    stack[stop][1]=expr_operands[op].pr;
   }
   if (t==-4)                           //kontynuacja
   {
    sbuf[k++]=c;
   }
   else                                 //nowe
   {
    k=0;
    sbuf[k++]=c;
    t=-4;
   }
  }
  else
  if (c=='(')                           //nawias otwierajacy
  {
   if ((func==0)&&((t==-3)||(t==-6)))   //jesli 2(... to 2*(...
   {
    op=Expr_FindOp ('*');
    if (t==-1) return EXPRERR_INVOP;    //brak '*' czyli niepowodzenie
    onp[j++]=' ';
    while ((stack[stop][1]>expr_operands[op].pr)&&(stop<EXPR_STACKSIZE))
          onp[j++]=stack[stop++][0];
    stack[--stop][0]=expr_operands[op].c;
    stack[stop][1]=expr_operands[op].pr;
   }
   if (func>0)
   {
    if (func==1) onp[j++]=' ';
    func++;
    onp[j++]='(';
   }
   else
   if (t==-4)                           //jesli nazwa
   {
    sbuf[k]=0;
    if (isvr==0)
    {
    fv=Expr_FindFV (sbuf);
    if (fv==NULL) return EXPRERR_UNDEFNAME;     //brak nazwy=blad
    if ((fv->funcaddr!=NULL)&&(fv->nparams>0))  //...to funkcja z parametrami
    {
     func=2;
     np=fv->nparams;
     nwp=1;
    }
    else
    {
     vrn:
     k=0;
     while (sbuf[k]!=0) onp[j++]=sbuf[k++];
     op=Expr_FindOp ('*');
     if (t==-1) return EXPRERR_INVOP;    //brak '*' czyli niepowodzenie
     onp[j++]=' ';
     while ((stack[stop][1]>expr_operands[op].pr)&&(stop<EXPR_STACKSIZE))
           onp[j++]=stack[stop++][0];
     stack[--stop][0]=expr_operands[op].c;
     stack[stop][1]=expr_operands[op].pr;
     stack[--stop][0]='(';              //znaczik nawiasu
     stack[stop][1]=0;                  //znaczik nawiasu
     t=-5;
     continue;
    }
    //else if (fv->funcaddr==NULL) return EXPRERR_SYNTAX; //blad skladni: zmienna(...
    } else {isvr=0; goto vrn;}
    k=0;
    while (sbuf[k]!=0) onp[j++]=sbuf[k++];
    onp[j++]=' ';
    onp[j++]='(';
   }
   else
   {
    stack[--stop][0]='(';              //znaczik nawiasu
    stack[stop][1]=0;                  //znaczik nawiasu
   }
   t=-5;
  }
  else
  if (c==')')                           //nawias zamykajacy
  {
   if (func==1) return EXPRERR_SYNTAX;  //blad skladni fun )...
   if (func>1)
   {
    func--;
    onp[j++]=')';
    if (func==1)
    {
     if (np>=1) return EXPRERR_TFPARAMS; //za malo parametrow
     func=0;
    }
   }
   else
   {
    if (t==-5) return EXPRERR_SYNTAX;   //blad skladni ()
    onp[j++]=' ';
    if (t==-4)                          //jesli nazwa
    {
     sbuf[k]=0;
     if (isvr==0)
     {
     fv=Expr_FindFV (sbuf);
     if (fv==NULL) return EXPRERR_UNDEFNAME;    //brak nazwy=blad
     if ((fv->funcaddr!=NULL)&&(fv->nparams>0)) //...to funkcja z parametrami...
        return EXPRERR_SYNTAX;                  //...wiec blad
     } else isvr=0;
     k=0;
     while (sbuf[k]!=0) onp[j++]=sbuf[k++];
    }
    onp[j++]=' ';                       //odzielamy elementy
    while ((stack[stop][1]!=0)&&(stop<EXPR_STACKSIZE))
          onp[j++]=stack[stop++][0];
    if (stop==EXPR_STACKSIZE) return EXPRERR_SYNTAX;    //za duzo nawiasow
    stop++;
   }
   t=-6;
  }
  else                                  //czy operand
  {
   op=Expr_FindOp (c);
   if (op==-1) return EXPRERR_INVOP;    //nie operand czyli niepowodzenie
   else                                 //operand
   if ((t>=0)||(t==-5))
   {
    if (c=='-')
    {
     onp[j++]='0';
     onp[j++]=' ';
     stack[--stop][0]='-';
     stack[stop][1]=255;
    }
    else
    if (expr_operands[op].npars==1)
    {
     onp[j++]=' ';                       //odzielamy elementy
     t=op;
     stack[--stop][0]=expr_operands[t].c;
     stack[stop][1]=expr_operands[t].pr;
    }
    else
    return EXPRERR_SYNTAX;
   }
   else
   {
    if (t==-4)                          //jesli nazwa
    {
     sbuf[k]=0;
     if (isvr==0)
     {
     fv=Expr_FindFV (sbuf);
     if (fv==NULL) return EXPRERR_UNDEFNAME;    //brak nazwy=blad
     if ((fv->funcaddr!=NULL)&&(fv->nparams>0)) //...to funkcja z parametrami...
        return EXPRERR_SYNTAX;                  //...i blad
     } else isvr=0;
     k=0;
     while (sbuf[k]!=0) onp[j++]=sbuf[k++];
    }
    onp[j++]=' ';                       //odzielamy elementy
    t=op;
    while ((stack[stop][1]>=expr_operands[t].pr)&&(stop<EXPR_STACKSIZE))
          onp[j++]=stack[stop++][0];
    stack[--stop][0]=expr_operands[t].c;
    stack[stop][1]=expr_operands[t].pr;
    if (expr_operands[op].npars==1) t=-2;
   }
  }
 }
 if (isstr) return EXPRERR_SYNTAX;      //brak konca
 if (func>0) return EXPRERR_SYNTAX;     //brak konca
 if (t==-4)                             //jesli nazwa
 {
  sbuf[k]=0;
  if (isvr==0)
  {
  fv=Expr_FindFV (sbuf);
  if (fv==NULL) return EXPRERR_UNDEFNAME;             //brak nazwy=blad
  if ((fv->funcaddr!=NULL)&&(fv->nparams>0))          //...to funkcja z parametrami...
     return EXPRERR_SYNTAX;                           //...i blad
  } else isvr=0;
  k=0;
  while (sbuf[k]!=0) onp[j++]=sbuf[k++];
 }
 onp[j++]=' ';
 while (stop<EXPR_STACKSIZE) if (stack[stop][0]!='(') onp[j++]=stack[stop++][0];
 else return EXPRERR_SYNTAX;
 onp[j]=0;
 return 1;
}

/*
########################################
 Expr_Comp
 Oblicza wartosc wyrazenia
 Zwraca obliczona wartosc
 Nazwy zmiennych VR podajemy po znaku $ np. 5*$nazwa-sin($nazwa2)
########################################
*/

float Expr_Comp (const char *expr)
{
 char onpbuf[EXPR_BUFLEN];
 char buf[EXPR_BUFLEN];
 float stack[EXPR_STACKSIZE];
 int prcparlist[FV_MAXPARAMS];
 int sl=strlen (expr);
 int i,j,k,c,t=-2,stop=EXPR_STACKSIZE,intw,par,nw=0,isstr,prevstrl,strl;
 float w;
 TFuncVar *fv;
 TVarRef *vr;
 float dvar;
 if ((sl==0)||(sl>EXPR_BUFLEN)) return 0.0f;    //blad
 expr_error=Expr_ONP (onpbuf,expr);
 if (expr_error!=EXPR_OK) return 0.0f;          //blad
 i=0;
 while (1)
 {
  c=onpbuf[i++];
  if (c==0) break;
  if (c==' ')                           //kolejna dana
  {
   if (t==-3)                           //liczba
   {
    buf[j]=0;
    j=0;
    stack[--stop]=atof (buf);
   }
   else
   if (t==-4)                           //dana
   {
    buf[j]=0;
    if (buf[0]=='$')                    //vr
    {
     /*vr=VR_FindName(buf+1);
     if (vr==NULL) return 0.0f;
     switch (vr->type)
     {
      case VR_VOID:
      case VR_INT: w=((float)*(int*)vr->ref);
                   memcpy (&stack[--stop],&w,4); break;
      case VR_FLOAT: memcpy (&stack[--stop],vr->ref,4); break;
      case VR_DOUBLE: dvar=((float)*(double*)vr->ref);
                      memcpy (&stack[--stop],&dvar,4); break;
     }*/
     if (VR_Get(buf+1,&stack[--stop],VR_FLOAT)==0) {expr_error=EXPRERR_UNDEFNAME; return 0.0f;}
     t=-2;
     continue;
    }
    fv=Expr_FindFV (buf);
    if (fv==NULL) return 0.0f;          //blad
    if (fv->funcaddr==NULL)             //zmienna
    {
     memcpy (&stack[--stop],&fv->value,4);
    }
    else
    if (fv->nparams==0)                 //funkcja bez parametrow
    {
     intw=Prc_ExecProc (fv->funcaddr,fv->nparams,fv->params&1);
     if ((fv->params&1)==EXPR_INT) w=intw; else memcpy(&w,&intw,4);
     memcpy (&stack[--stop],&w,4);
    }
    else                                //funkcja z parametrami
    {
     while (onpbuf[i++]!='(');
     k=0;
     par=fv->params>>1;
     isstr=0;
     strl=0;
     while (1)
     {
      if (onpbuf[i]==0) return 0.0f;
      j=0;
      while (1)
      {
       c=onpbuf[i++];
       if ((isstr==1)&&(c!='"'))
       {
        expr_strbuf[strl++]=c;
        continue;
       }
       if (c==0) return 0.0f;                   //brak konca
       if ((nw==0)&&(c==',')) break;
       if ((nw==0)&&(c=='"'))
       {
        if (isstr==0)
        {
         prevstrl=strl;
         isstr=1;
        }
        else
        {
         isstr=-1;
         expr_strbuf[strl]=0;
        }
       }
       if (isstr==-1)
       {
        while ((c!=')')&&(c!=',')) {c=onpbuf[i++];}
        break;
       }
       if (c=='(') nw++;
       if (c==')') if (nw==0) break; else nw--;
       buf[j++]=c;
      }
      if (isstr==-1)
      {
       intw=(int)expr_strbuf+prevstrl;
       isstr=0;
       strl++;
      }
      else
      {
       buf[j]=0;
       w=Expr_Comp(buf);
       if ((par&1)==EXPR_INT) intw=w; else memcpy(&intw,&w,4);
      }
      memcpy(&prcparlist[k++],&intw,4);
      par>>=1;
      if (c==')') break;
     }
     //for (k=0; k<fv->nparams;k++) memcpy(&prc_parlist[k],&prcparlist[k],4);
     memcpy(prc_parlist,prcparlist,fv->nparams*4);
     intw=Prc_ExecProc (fv->funcaddr,fv->nparams,fv->params&1);
     if ((fv->params&1)==EXPR_INT) w=intw; else memcpy(&w,&intw,4);
     memcpy (&stack[--stop],&w,4);
     i++;
    }
    j=0;
   }
   t=-2;
   continue;
  }
  if (((c>='0')&&(c<='9'))||(c=='.'))   //cyfry
  {
   if (t!=-4)
   {
    if (t!=-3) j=0;
    buf[j++]=c;
    t=-3;
   }
   else
   {
    buf[j++]=c;
   }
  }
  else
  if (((c>='A')&&(c<='Z'))||(c=='_')||(c=='$'))   //znaki
  {
   if (t!=-4) j=0;
   buf[j++]=c;
   t=-4;
  }
  else                                  //operand
  {
   t=Expr_FindOp (c);
   if (t==-1) return 0.0f;
   if (stop+expr_operands[t].npars>EXPR_STACKSIZE) return 0.0f; //blad przepelnienia stosu
   for (k=0; k<expr_operands[t].npars; k++) memcpy (&prc_parlist[k],&stack[stop++],4);
   intw=Prc_ExecProc (expr_operands[t].funcaddr,expr_operands[t].npars,EXPR_FLOAT);
   memcpy (&stack[--stop],&intw,4);
  }
 }
 return stack[stop];
}

#endif
