//main.cpp

//DEFINICJE

/*#define DC
#define X86*/
#define OPENGL
#define OPENGLES

//INCLUDY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#undef _STRICT_ANSI
#include <math.h>
#include <ctype.h>

float inline round(float a) 
{
 return floor(a+0.5f);
}

float asinh(float a) 
{
 return log(a+sqrt(a*a+1));
}

float acosh(float a) 
{
 return log(a+sqrt(a*a-1));
}

float atanh(float a) 
{
 return 0.5*log((1+a)/(1-a));
}

float inline tgammaf(float a) 
{
 return 0;
}

#ifdef X86
#include "mathx86.h"

#define __ asm("push %eax;pop %eax")
unsigned long long t0,t1;
#define _r1 asm volatile("rdtsc":"=a"(*(unsigned int*)(&t0)),"=d"(*(unsigned int*)((int)&t0+4)))
#define _r2 asm volatile("rdtsc; mov %%eax,(%%ecx); mov %%edx,4(%%ecx);"::"c"(&t1));\
              DEBUG_Printf("%0.0f\n",(double)t1-(double)t0);
#else
#define iround(a) (int)round(a)
#define ifloor(a) (int)floor(a)
#define iceil(a)  (int)ceil(a)
#endif


#include "s60sound.cpp"
#include "mixer\mixer.c"
#include "debug.c"

#ifdef OPENGL
#include <gles/gl.h>
#include <gles/egl.h>
#endif

#define SCRWIDTH 240
#define SCRHEIGHT 320
#define BPP 32

#include "clib.h"
#include "csys.h"
#ifdef OPENGL
#include "copengl.h"
#include "copengl.c"
#endif
#include "ctex.h"
#include "cexpr.h"
#include "cwin.h"
#include "clibwin.h"
#include "ccon.h"
#include "cgraph.h"
#include "csound.h"
#include "chelp.h"
#include "keymap.h"
#include "env.h"
#include "sysext\cvkeyb.h"
CVKeyb *vkeyb;

#include "csys.c"
#include "clib.c"
#include "clibwin.c"
#include "cexpr.c"
#include "estdlib.c"
#include "ctex.c"
#include "cwin.c"
#include "ccon.c"
#include "cgraph.c"
#include "csound.c"
#include "chelp.c"
#include "sysext\cvkeyb.c"
#include "userfunc.c"

#if (DEFAULTR>20)
d ;)
#endif


CCon *con;
CGraph *graph;
CHelp *help;
CSound *sound;

CLib *library;
CLibWin *libwin;
CExpr *expr;

//rozszezenia estdlib
float ans;
ELEMENT anse={"ANS",&ans,VAL_FLOAT,VAR,0,0};

int tex_load(int n,char *fn)
{
 return graph->tex[n%MAXFUNCCOUNT].Load(fn);
}

int tex_setpar(int n,float a,float b,float c,float d)
{
 graph->tex[n%MAXFUNCCOUNT].SetParameters(a,b,c,d);
 return 1;
}

int tex_setalpha(int n,float a)
{
 graph->tex[n%MAXFUNCCOUNT].SetAlpha(a);
 return 1;
}

int tex_func1(int n,char *s,float x1,float y1,float x2,float y2)
{
 return graph->tex[n%MAXFUNCCOUNT].TexFunc1(s,x1,y1,x2,y2);
}

int tex_func3(int n,char *sr,char *sg,char *sb,float x1,float y1,float x2,float y2)
{
 return graph->tex[n%MAXFUNCCOUNT].TexFunc3(sr,sg,sb,x1,y1,x2,y2);
}

int tex_func4(int n,char *sr,char *sg,char *sb,char *sa,float x1,float y1,float x2,float y2)
{
 return graph->tex[n%MAXFUNCCOUNT].TexFunc4(sr,sg,sb,sa,x1,y1,x2,y2);
}

ELEMENT texutils[]=
{
 {"TEX_LOAD",(void*)&tex_load,VAL_INT,2,VAL_INT|VAL_STR*4,0},
 {"TEX_SETPAR",(void*)&tex_setpar,VAL_INT,5,VAL_INT,0},
 {"TEX_SETALPHA",(void*)&tex_setalpha,VAL_INT,2,VAL_INT,0},
 {"TEX_FUNC1",(void*)&tex_func1,VAL_INT,6,VAL_INT|VAL_STR*4,0},
 {"TEX_FUNC3",(void*)&tex_func3,VAL_INT,8,VAL_INT|VAL_STR*4|VAL_STR*16|VAL_STR*64,0},
 {"TEX_FUNC4",(void*)&tex_func4,VAL_INT,9,VAL_INT|VAL_STR*4|VAL_STR*16|VAL_STR*64|VAL_STR*256,0}
};

char compbf[BUFLEN*2];
char *comp(char *s)
{
 int noaddf=0;
 if (s[0]=='/') {s++; noaddf=1;}
 else
 if (s[0]=='>') {s++; noaddf=2;}
 else
 if (s[0]=='<') {s++; noaddf=3;}
 else
 if (s[0]==';') {strcpy(compbf,s); compbf[0]=5; return compbf;}
 char s2[BUFLEN+1];
 strcpy(s2,s);
 int ok=expr->Set(s);
 if ((ok==CExpr::NoErr)&&(UserFunc_ison==0))
 {
  if (noaddf<2)
  {
   ans=expr->Do();
   if (strlen(s)>BUFLEN) s2[BUFLEN]=0;
   sprintf(compbf,"%s=%f",s2,ans);
   CSys::DiscardZeros(compbf);
  }
  if (!noaddf)
  {
   CGraph::GetGraph()->SetExpr(s,expr,CSys::DColor,CSys::DColor);
   CSound::GetSound()->SetExpr(s,expr,CSys::DColor);
  }
  if (noaddf==2) 
  {
   CGraph::GetGraph()->SetExpr(s,expr,MAXFUNCCOUNT,MAXFUNCCOUNT);
   sprintf(compbf,"BEGIN: %s",s);
  }
  else
  if (noaddf==3) 
  {
   CGraph::GetGraph()->SetExpr(s,expr,MAXFUNCCOUNT+1,MAXFUNCCOUNT+1);
   sprintf(compbf,"END: %s",s);
  }
 }
 else
 if (UserFunc_ison==0)
 sprintf(compbf,"\04%s",expr->RetStr());
 else
 {
  if (ok==CExpr::NoErr) expr->Do();
  UserFunc_ParseStr(s);
  if (strlen(s)>BUFLEN) s2[BUFLEN]=0;
  strcpy(compbf,s2);
 }
 return compbf;
}

//if returns 0 program exits else goes to MainLoop()
int AppInit()
{
 CSys::Init();
 library=new CLib;
 expr=new CExpr(library);
 library->AddElement(&anse);
 library->AddList(texutils,sizeof(texutils)/sizeof(ELEMENT));
 library->AddList(expr_estdlib,sizeof(expr_estdlib)/sizeof(ELEMENT));
 init_estdlib(library);
 
 libwin=new CLibWin(CSys::ScrWidth,CSys::ScrHeight,buf,
                    20,20,CSys::ScrWidth-20,CSys::ScrHeight*2/3,library);
    
 con=new CCon(CSys::ScrWidth,CSys::ScrHeight,buf,comp,libwin);
 graph=new CGraph(CSys::ScrWidth,CSys::ScrHeight,buf);
 sound=new CSound(CSys::ScrWidth,CSys::ScrHeight,buf);
 CGraph::currentgraph=graph;
 CSound::currentsound=sound;
 CCon::currentcon=con;
 CSys::SetDMode(CGraph::K2DF1);
 help=new CHelp(CSys::ScrWidth,CSys::ScrHeight,buf,"readme.txt");
 vkeyb=new CVKeyb(CSys::ScrWidth,CSys::ScrHeight,buf);
 UserFunc_Init(library);
 
 CSys::SetWindow(con,CSys::Win_Con);
 CSys::SetWindow(graph,CSys::Win_Graph);
 CSys::SetWindow(help,CSys::Win_Help);
 CSys::SetWindow(sound,CSys::Win_Sound);
 CSys::SetWindow(libwin,CSys::Win_Winlib);
 CSys::SetActiveWin(CSys::Win_Con);
 return 1;
}

//if returns 0 program goes to AtExit()
int AppMainLoop()
{
 return CSys::Run();
}

void AppExit()
{
 CSys::Close();
}
