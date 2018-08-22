//estdlib.c

//wewnetrzne dane i funkcje

#define EXPR_MAXLEVELS 2

CExpr expr_commonexpr[EXPR_MAXLEVELS][MAXSTRINGS];
FILE *expr_commonfiles[MAXSTRINGS];
int expr_commonfilessize[MAXSTRINGS];
unsigned char *expr_commonfilesbuf[MAXSTRINGS];
int expr_strhash[EXPR_MAXLEVELS][MAXSTRINGS];

void init_estdlib(CLib *l)
{
 for (int j=0; j<EXPR_MAXLEVELS; j++)
 {
  for (int i=0; i<MAXSTRINGS; i++) 
  {
   expr_commonexpr[j][i]=CExpr(l);
  }
 }
 memset(expr_strhash,0,sizeof(expr_strhash));
 memset(expr_commonfiles,0,sizeof(expr_commonfiles));
 memset(expr_commonfilesbuf,0,sizeof(expr_commonfilesbuf));
}

int expr_level=0;


//FUNKCJE MATEMATYCZNE

//trygonometria

float expr_sin(float a)
{
 return sin(a);
}

float expr_cos(float a)
{
 return cos(a);
}

float expr_tan(float a)
{
 return tan(a);
}

float expr_ctan(float a)
{
 return 1/tan(a);
}

float expr_asin(float a)
{
 return asin(a);
}

float expr_acos(float a)
{
 return acos(a);
}

float expr_atan(float a)
{
 return atan(a);
}

float expr_actan(float a)
{
 return atan(1/a);
}

//f. wykladniczo-logarytmiczne

float expr_sinh(float a)
{
 return sinh(a);
}

float expr_cosh(float a)
{
 return cosh(a);
}

float expr_tanh(float a)
{
 return tanh(a);
}

float expr_ctanh(float a)
{
 return 1/tanh(a);
}

float expr_asinh(float a)
{
 return asinh(a);
}

float expr_acosh(float a)
{
 return acosh(a);
}

float expr_atanh(float a)
{
 return atanh(a);
}

float expr_actanh(float a)
{
 return atanh(1/a);
}

float expr_exp(float a)
{
 return exp(a);
}

float expr_log(float a)
{
 return log10(a);
}

float expr_ln(float a)
{
 return log(a);
}

float expr_logb(float a,float b)
{
 return log(a)/log(b);
}

//f. wielomianowe

float expr_sqrt(float a)
{
 return sqrt(a);
}

float expr_abs(float a)
{
 return fabs(a);
}

float expr_floor(float a)
{
 return floor(a);
}

float expr_ceil(float a)
{
 return ceil(a);
}

float expr_frac(float a)
{
 return a-floor(a);
}

float expr_round(float a)
{
 return round(a);
}

float expr_min(float a,float b)
{
 return (a>b)? b:a;
}

float expr_max(float a,float b)
{
 return (a>b)? a:b;
}

float expr_dist(float a,float b)
{
 return sqrt(a*a+b*b);
}

float expr_rnd()
{
 return (float)rand()*(1.f/RAND_MAX);
}

int expr_srnd(int s)
{
 if (s==0) s=CSys::SRand0;
 srand(s);
 return s;
}

//funkcje specjalne

float expr_gamma(float a)
{
 return tgammaf(a+1);
}

//funkcje warunkowe

//podaje obliczona wartosc a jesli c, w przeciwnym wypadu b
//funkcja zawsze wykonuje wszystkie wyrazenia
float expr_ifv(float c,float a,float b)
{
 if (c) return a; else return b;
}

//ZMIENNE

float expr_x,expr_y,expr_t,expr_k,expr_time;

//STALE

float expr_pi=3.1415926535897932384626433832795f;
float expr_e=2.7182818284590452353602874713527f;

//funkcje dla zmiennych

//ustaw x=fx i y=fy dla f. parametrycznej
float expr_setxy(float fx,float fy)
{
 expr_x=fx;
 expr_y=fy;
 return 1;
}

//ustaw x=fx i y=fy dla f. parametrycznej wd. funkcji biegunowej r=f(a)
float expr_setxypol(float f)
{
 expr_x=cos(expr_t)*f;
 expr_y=sin(expr_t)*f;
 return 1;
}


//FUNKCJE KONSOLI

//wypisuje wartosci n funkcji zmiennej x od a do b
//zwraca wartosc pola pod wykresem danej funkcji
float expr_table(char *f,float a,float b,int n)
{
 if (CExpr::MultiExec) return 0;
 if (!CSys::IsValidStr(f)) return 0;
 char bf[128];
 float d=(b-a)/(float)(n-1);
 float p=0;
 CExpr e(CExpr::GetLib());
 if (e.Set(f)!=CExpr::NoErr) 
 {
  sprintf(bf,"\04%s",e.RetStr());
  CCon::GetCon()->Put(bf);
  return 0;
 }
 expr_x=a;
 float v=e.Do(),v1;
 for (int i=1; i<n; i++)
 {
  sprintf(bf,"f(%0.4f)=%0.4f",expr_x,v);
  CCon::GetCon()->Put(bf);
  expr_x+=d;
  v1=e.Do();
  p+=(v1+v)*d/2;
  v=v1;
 }
 sprintf(bf,"f(%0.4f)=%0.4f",expr_x,v);
 CCon::GetCon()->Put(bf);
 return p;
}

//wyswietla w konsoli dany tekst
int expr_conprint(char *s)
{
 if (CSys::IsValidStr(s))
 CCon::GetCon()->Put(s);
 return 1;
}

//wyswietla w konsoli liczbe calkowita w postaci liczby o bazie b
int expr_conprint_i_base(int a,int b)
{
 if (CExpr::MultiExec) return 0;
 if (b<2) return 0;
 char bf[128];
 char bf_out[128];
 int i=0;
 //sprintf(bf,"%X",a);
 while (a!=0)
 {
  int c=a%b;
  if (c>9) c+='A'-10;
  else c+='0';
  bf[i++]=c;
  a/=b;
 }
 int j=0;
 bf_out[i--]=0;
 while (i>=0) bf_out[j++]=bf[i--];
 CCon::GetCon()->Put(bf_out);
 return 1;
}

//traktuje ciag znakow jako liczbe calkowita  o bazie b
int expr_i_base(char *a,int b)
{
 //if (CExpr::MultiExec) return 0;
 if (b<2) return 0;
 int l=0;
 while(*a)
 {
  l*=b;
  int c=*a++;
  if (c>='a') c=c-'a'+10;
  else
  if (c>='A') c=c-'A'+10;
  else
  c=c-'0';
  l+=c;
 }

 return l;
}

//czysci konsole
int expr_cls()
{
 CCon::GetCon()->Cls();
 return 1;
}

//czy ma byc wyswietlana zawartosc wykonywanego pliku
int expr_echo(int e)
{
 CCon::GetCon()->Echo(e);
 return 1;
}

int expr_exec(char *f)
{
 if (CSys::IsValidStr(f))
 return CCon::GetCon()->FExec(f);
 else return 0;
}

//INNE

//suma od i=a do i=b wyrazenia
float expr_sum(char *f,int a,int b)
{
 if (expr_level==EXPR_MAXLEVELS) return 0;
 int c=CExpr::GetExpr()->StrCount();
 if (expr_strhash[expr_level][c]!=CExpr::GetExpr()->StrHash(c))
 {
  if (!CSys::IsValidStr(f)) return 0;
  //DEBUG_Printf("0x%X\n",expr_commonexpr[expr_level][c].Do);
  if (expr_commonexpr[expr_level][c].Set(f)!=CExpr::NoErr) 
  {
   if (CExpr::MultiExec==CExpr::MultiExec_Once)
   {
    char bf[256];
    sprintf(bf,"\04%s",expr_commonexpr[expr_level][c].RetStr());
    CCon::GetCon()->Put(bf);
   }
   return 0;
  }
  expr_strhash[expr_level][c]=CExpr::GetExpr()->StrHash(c);
  //DEBUG_Printf("%d %d %s\n",c,expr_strhash[c],f);
 }
 float w=0;
 CExpr *e;
 e=&expr_commonexpr[expr_level][c];
 expr_level++;
 for (int i=a; i<=b; i++)
 {
  expr_k=i;
  w+=e->Do();
 }
 expr_level--;
 return w;
}

//pochodna wyrazenia
#define DIFFDELTA 0.01
float expr_diff(char *f)
{
 if (expr_level==EXPR_MAXLEVELS) return 0;
 int c=CExpr::GetExpr()->StrCount();
 if (expr_strhash[expr_level][c]!=CExpr::GetExpr()->StrHash(c))
 {
  if (!CSys::IsValidStr(f)) return 0;
  if (expr_commonexpr[expr_level][c].Set(f)!=CExpr::NoErr) 
  {
   if (CExpr::MultiExec==CExpr::MultiExec_Once)
   {
    char bf[256];
    sprintf(bf,"\04%s",expr_commonexpr[expr_level][c].RetStr());
    CCon::GetCon()->Put(bf);
   }
   return 0;
  }
  expr_strhash[expr_level][c]=CExpr::GetExpr()->StrHash(c);
  //DEBUG_Printf("%d %d %s\n",c,expr_strhash[c],f);
 }
 float f1,f2;
 expr_level++;
 expr_x-=DIFFDELTA/2;
 f1=expr_commonexpr[expr_level-1][c].Do();
 expr_x+=DIFFDELTA;
 f2=expr_commonexpr[expr_level-1][c].Do();
 expr_x-=DIFFDELTA/2;
 expr_level--;
 return (f2-f1)/DIFFDELTA;
}

float expr_integral(char *f,float a,float b)
{
 if (expr_level==EXPR_MAXLEVELS) return 0;
 int c=CExpr::GetExpr()->StrCount();
 if (expr_strhash[expr_level][c]!=CExpr::GetExpr()->StrHash(c))
 {
  if (!CSys::IsValidStr(f)) return 0;
  if (expr_commonexpr[expr_level][c].Set(f)!=CExpr::NoErr) 
  {
   if (CExpr::MultiExec==CExpr::MultiExec_Once)
   {
    char bf[256];
    sprintf(bf,"\04%s",expr_commonexpr[expr_level][c].RetStr());
    CCon::GetCon()->Put(bf);
   }
   return 0;
  }
  expr_strhash[expr_level][c]=CExpr::GetExpr()->StrHash(c);
 }
 static float sum,xp;
 if ((expr_x<a)||(expr_x>b)||(expr_x<xp)) {xp=a; sum=0;}
 else
 {
  float x=expr_x;
  expr_level++;
  expr_x=xp;
  float y1=expr_commonexpr[expr_level-1][c].Do();
  expr_x=x;
  float y2=expr_commonexpr[expr_level-1][c].Do();
  sum+=(y1+y2)*(x-xp)*(1/2.f);
  expr_level--;
  xp=x;
 }
 return sum;
}

//ustawia kolor rysowania
int expr_setcolor(int c)
{
 if (CExpr::MultiExec) return 0;
 CSys::SetDColor(c);
 return c;
}

//ustawia tryb rysowania
int expr_setmode(int m)
{
 if (CExpr::MultiExec) return 0;
 CSys::SetDMode(m);
 return m;
}

//ustawia tryb graficzny
int expr_setvidmode(int x,int y)
{
 if (CExpr::MultiExec) return 0;
 CSys::VidMode(x,y);
 return 1;
}

//ustawia tryb dzwieku (freq,bps,kanaly)
int expr_setsoundmode(int freq,int bps,int ch)
{
 if (CExpr::MultiExec) return 0;
 CSys::StopSound();
 CSys::SoundFreq=freq;
 if ((bps!=8)&&(bps!=16)) bps=16;
 if ((ch!=1)&&(ch!=2)) ch=2;
 CSys::SoundBPS=bps;
 CSys::SoundChannels=ch;
 return 1;
}

int expr_sysinfo()
{
 if (CExpr::MultiExec) return 0;
 CCon::GetCon()->Put(CSys::SysInfoString());
 return 1;
}

int expr_shell(char *cl)
{
 if (CExpr::MultiExec) return 0;
 return CSys::Shell(cl);
}

//wczytuje bajt z pozycji pos z danego pliku i zwraca wartosc z przedzialu [0..1]
float expr_file(char *f,int pos)
{
 /*static FILE *f=0;
 static CExpr e(CExpr::GetLib());
 static int sid=-1;
 if (sid!=CExpr::GetExpr()->GetStrId())
 {
  if (f) fclose(f);
  f=CSys::FOpen(fn,"rb");
  sid=CExpr::GetExpr()->GetStrId();
 }*/
 /*int c=CExpr::GetExpr()->StrCount();
 if (expr_strhash[expr_level][c]!=CExpr::GetExpr()->StrHash(c))
 {
  if (!CSys::IsValidStr(f)) return 0;
  if (expr_commonfiles[c]) fclose(expr_commonfiles[c]);
  expr_commonfiles[c]=CSys::FOpen(f,"rb");
  expr_strhash[expr_level][c]=CExpr::GetExpr()->StrHash(c);
 }
 if (expr_commonfiles[c]==0) return 0;
 if (fseek(expr_commonfiles[c],pos,SEEK_SET)!=0) return 0;
 unsigned int b=fgetc(expr_commonfiles[c]);
 if (b>255) b=0;
 return (float)b/255.f;*/
 int c=CExpr::GetExpr()->StrCount();
 if (expr_strhash[expr_level][c]!=CExpr::GetExpr()->StrHash(c))
 {
  if (!CSys::IsValidStr(f)) return 0;
  if (expr_commonfiles[c]) fclose(expr_commonfiles[c]);
  expr_commonfiles[c]=CSys::FOpen(f,"rb");
  if (expr_commonfiles[c]==0) return 0;
  expr_strhash[expr_level][c]=CExpr::GetExpr()->StrHash(c);
  fseek(expr_commonfiles[c],0,SEEK_END);
  int s=ftell(expr_commonfiles[c]);
  fseek(expr_commonfiles[c],0,SEEK_SET);
  CSys::FreeMem(expr_commonfilesbuf[c]);
  expr_commonfilesbuf[c]=(unsigned char *)CSys::AllocMem(s);
  fread(expr_commonfilesbuf[c],1,s,expr_commonfiles[c]);
  expr_commonfilessize[c]=s;
 }
 if (expr_commonfilesbuf[c]==0) return 0;
 if ((pos<0)||(pos>expr_commonfilessize[c])) return 0;
 return (float)expr_commonfilesbuf[c][pos]/255.f;
}

float expr_varget(char *name,int shift)
{
 return CSys::GetVarF(name,shift);
}

float expr_varset(char *name,char *val,int shift)
{
 return CSys::SetVarV(name,val,shift);
}

float expr_varsetf(char *name,float f,int shift)
{
 char bf[64];
 sprintf(bf,"%f",f);
 return CSys::SetVarV(name,bf,shift);
}

//DLL

//laduje biblioteke o nazwie name
int expr_loadlib(char *name)
{
 if (CExpr::MultiExec) return 0;
 return CSys::DLibLoad(name);
}

//BIBLIOTEKA

ELEMENT expr_estdlib[]=
{
 {"SIN",(void*)expr_sin,VAL_FLOAT,1,VAL_FLOAT,0},
 {"COS",(void*)expr_cos,VAL_FLOAT,1,VAL_FLOAT,0},
 {"TAN",(void*)expr_tan,VAL_FLOAT,1,VAL_FLOAT,0},
 {"CTAN",(void*)expr_ctan,VAL_FLOAT,1,VAL_FLOAT,0},
 {"ASIN",(void*)expr_asin,VAL_FLOAT,1,VAL_FLOAT,0},
 {"ACOS",(void*)expr_acos,VAL_FLOAT,1,VAL_FLOAT,0},
 {"ATAN",(void*)expr_atan,VAL_FLOAT,1,VAL_FLOAT,0},
 {"ACTAN",(void*)expr_actan,VAL_FLOAT,1,VAL_FLOAT,0},
 {"SINH",(void*)expr_sinh,VAL_FLOAT,1,VAL_FLOAT,0},
 {"COSH",(void*)expr_cosh,VAL_FLOAT,1,VAL_FLOAT,0},
 {"TANH",(void*)expr_tanh,VAL_FLOAT,1,VAL_FLOAT,0},
 {"CTANH",(void*)expr_ctanh,VAL_FLOAT,1,VAL_FLOAT,0},
 {"ASINH",(void*)expr_asinh,VAL_FLOAT,1,VAL_FLOAT,0},
 {"ACOSH",(void*)expr_acosh,VAL_FLOAT,1,VAL_FLOAT,0},
 {"ATANH",(void*)expr_atanh,VAL_FLOAT,1,VAL_FLOAT,0},
 {"ACTANH",(void*)expr_actanh,VAL_FLOAT,1,VAL_FLOAT,0},
 {"EXP",(void*)expr_exp,VAL_FLOAT,1,VAL_FLOAT,0},
 {"LN",(void*)expr_ln,VAL_FLOAT,1,VAL_FLOAT,0},
 {"LOG",(void*)expr_log,VAL_FLOAT,1,VAL_FLOAT,0},
 {"LOGB",(void*)expr_logb,VAL_FLOAT,2,VAL_FLOAT+VAL_FLOAT*4,0},
 {"SQRT",(void*)expr_sqrt,VAL_FLOAT,1,VAL_FLOAT,0},
 {"ABS",(void*)expr_abs,VAL_FLOAT,1,VAL_FLOAT,0},
 {"FLOOR",(void*)expr_floor,VAL_FLOAT,1,VAL_FLOAT,0},
 {"CEIL",(void*)expr_ceil,VAL_FLOAT,1,VAL_FLOAT,0},
 {"FRAC",(void*)expr_frac,VAL_FLOAT,1,VAL_FLOAT,0},
 {"ROUND",(void*)expr_round,VAL_FLOAT,1,VAL_FLOAT,0},
 {"MIN",(void*)expr_min,VAL_FLOAT,2,VAL_FLOAT+VAL_FLOAT*4,0},
 {"MAX",(void*)expr_max,VAL_FLOAT,2,VAL_FLOAT+VAL_FLOAT*4,0},
 {"DIST",(void*)expr_dist,VAL_FLOAT,2,VAL_FLOAT+VAL_FLOAT*4,0},
 {"RND",(void*)expr_rnd,VAL_FLOAT,0,0,0},
 {"SRND",(void*)expr_srnd,VAL_INT,1,VAL_INT,0},
 {"GAMMA",(void*)expr_gamma,VAL_FLOAT,1,VAL_FLOAT,0},
 {"IFV",(void*)expr_ifv,VAL_FLOAT,3,VAL_FLOAT+VAL_FLOAT*4+VAL_FLOAT*16},
 {"TABLE",(void*)expr_table,VAL_FLOAT,4,VAL_STR+VAL_FLOAT*4+VAL_FLOAT*16+VAL_INT*64,0},
 {"PRINT",(void*)expr_conprint,VAL_INT,1,VAL_STR,0},
 {"BPRINT",(void*)expr_conprint_i_base,VAL_INT,2,VAL_INT|VAL_INT*4,0},
 {"STRIB",(void*)expr_i_base,VAL_INT,2,VAL_STR|VAL_INT*4,0},
 {"CLS",(void*)expr_cls,VAL_INT,0,0,0},
 {"ECHO",(void*)expr_echo,VAL_INT,1,VAL_INT,0},
 {"EXEC",(void*)expr_exec,VAL_INT,1,VAL_STR,0},
 {"SUM",(void*)expr_sum,VAL_FLOAT,3,VAL_STR+VAL_INT*4+VAL_INT*16,0},
 {"DIFF",(void*)expr_diff,VAL_FLOAT,1,VAL_STR,0},
 {"INTEGRAL",(void*)expr_integral,VAL_FLOAT,3,VAL_STR,0},
 {"COLOR",(void*)expr_setcolor,VAL_INT,1,VAL_INT,0},
 {"MODE",(void*)expr_setmode,VAL_INT,1,VAL_INT,0},
 {"VIDMODE",(void*)expr_setvidmode,VAL_INT,2,VAL_INT+VAL_INT*4,0},
 {"SOUNDMODE",(void*)expr_setsoundmode,VAL_INT,3,VAL_INT+VAL_INT*4+VAL_INT*16,0},
 {"SYSINFO",(void*)expr_sysinfo,VAL_INT,0,0,0},
 {"SHELL",(void*)expr_shell,VAL_INT,1,VAL_STR,0},
 {"FILE",(void*)expr_file,VAL_FLOAT,2,VAL_STR+VAL_INT*4,0},
 {"VARGET",(void*)expr_varget,VAL_FLOAT,2,VAL_STR|VAL_INT*4,0},
 {"VARSET",(void*)expr_varset,VAL_FLOAT,3,VAL_STR|VAL_STR*4|VAL_INT*16,0},
 {"VARSETF",(void*)expr_varsetf,VAL_FLOAT,3,VAL_STR|VAL_INT*16,0},
 {"LOADLIB",(void*)expr_loadlib,VAL_INT,1,VAL_STR,0},
 {"PAR",(void*)expr_setxy,VAL_FLOAT,2,0,0},
 {"POL",(void*)expr_setxypol,VAL_FLOAT,1,0,0},
 {"TIME",&expr_time,VAL_FLOAT,VAR,0,0},
 {"X",&expr_x,VAL_FLOAT,VAR,0,0},
 {"Y",&expr_y,VAL_FLOAT,VAR,0,0},
 {"T",&expr_t,VAL_FLOAT,VAR,0,0},
 {"K",&expr_k,VAL_FLOAT,VAR,0,0},
 {"PI",&expr_pi,VAL_FLOAT,VAR,0,0},
 {"E",&expr_e,VAL_FLOAT,VAR,0,0}
};
