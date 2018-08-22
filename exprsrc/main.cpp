//DEFINICJE

#define DC
#define X86
#define OPENGL
#define MENU
#define CG

//INCLUDY
#include <windows.h>
#include <DX8/d3d8.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#ifdef X86
#include "mathx86.h"

#define __ asm("push %eax;pop %eax")
unsigned long long _t0,_t1;
#define _r1 asm volatile("rdtsc":"=a"(*(unsigned int*)\
(&_t0)),"=d"(*(unsigned int*)((int)&_t0+4)))
#define _r2 asm volatile("rdtsc; mov %%eax,(%%ecx); mov %%edx,4(%%ecx);"::"c"(&_t1));\
              DEBUG_Printf("%0.0f\n",(double)_t1-(double)_t0);
#else
#define iround(a) (int)round(a)
#define ifloor(a) (int)(a)
#define iceil(a)  (int)ceil(a)
#endif

//def

#define SCRWIDTH 640
#define SCRHEIGHT 480
#define BPP 32
#define VSYNC (WINGRAPH_VSYNC)
#define OVERLAPPED (WINGRAPH_OVERLAPPEDWINDOW)

//h

#include "wingraph.h"
#include "winsound.h"
#include "mixer\mixer.h"
#include "debug.c"

#ifdef OPENGL
#include <gl/gl.h>
#include <gl/glext.h>
#include "copengl.h"
#else
#ifdef CG
#undef CG
#endif
#endif

#ifdef CG
#include "libcg.h"
#include "libcggl.h"
//#include <cg/cg.h>
//#include <cg/cggl.h>
#endif

#include "keymap.h"
#include "clib.h"

#ifdef MENU
#include "cmenu.h"
#endif

#include "csys.h"
#include "ctex.h"
#include "cexpr.h"
#include "cwin.h"
#include "clibwin.h"
#include "ccon.h"
#include "cgraph.h"
#include "csound.h"
#include "chelp.h"
#include "env.h"


//c

#include "estdlib.c"
#include "csys.c"

#ifdef MENU
#include "cmenu.c"
#endif

#ifdef OPENGL
#include "copengl.c"
#endif

#include "clib.c"
#include "clibwin.c"
#include "cexpr.c"
#include "ctex.c"
#include "cwin.c"
#include "ccon.c"
#include "cgraph.c"
#include "csound.c"
#include "chelp.c"
#include "userfunc.c"

//var

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

//funkcje obslugi tekstury

int tex_load(int n,char *fn)
{
 if (!CSys::IsValidStr(fn)) return 0;
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
 if (!CSys::IsValidStr(s)) return 0;
 return graph->tex[n%MAXFUNCCOUNT].TexFunc1(s,x1,y1,x2,y2);
}

int tex_func3(int n,char *sr,char *sg,char *sb,float x1,float y1,float x2,float y2)
{
 if (!CSys::IsValidStr(sr)) return 0;
 if (!CSys::IsValidStr(sg)) return 0;
 if (!CSys::IsValidStr(sb)) return 0;
 return graph->tex[n%MAXFUNCCOUNT].TexFunc3(sr,sg,sb,x1,y1,x2,y2);
}

int tex_func4(int n,char *sr,char *sg,char *sb,char *sa,float x1,float y1,float x2,float y2)
{
 if (!CSys::IsValidStr(sr)) return 0;
 if (!CSys::IsValidStr(sg)) return 0;
 if (!CSys::IsValidStr(sb)) return 0;
 if (!CSys::IsValidStr(sa)) return 0;
 return graph->tex[n%MAXFUNCCOUNT].TexFunc4(sr,sg,sb,sa,x1,y1,x2,y2);
}

#ifdef CG
int tex_gennormalmap(int n,float height)
{
 if (CExpr::MultiExec==CExpr::MultiExec_Multi) return 0;
 graph->tex[n%MAXFUNCCOUNT].BuildNormalMap(height);
 return 1;
}

int tex_setnormalmap(int n,char *fn)
{
 if (!CSys::IsValidStr(fn)) return 0;
 if (CExpr::MultiExec==CExpr::MultiExec_Multi) return 0;
 graph->tex[n%MAXFUNCCOUNT].SetNormalMap(fn);
 return 1;
}
#endif

ELEMENT texutils[]=
{
 {"TEX_LOAD",(void*)&tex_load,VAL_INT,2,VAL_INT|VAL_STR*4,0},
 {"TEX_SETPAR",(void*)&tex_setpar,VAL_INT,5,VAL_INT,0},
 {"TEX_SETALPHA",(void*)&tex_setalpha,VAL_INT,2,VAL_INT,0},
 {"TEX_FUNC1",(void*)&tex_func1,VAL_INT,6,VAL_INT|VAL_STR*4,0},
 {"TEX_FUNC3",(void*)&tex_func3,VAL_INT,8,VAL_INT|VAL_STR*4|VAL_STR*16|VAL_STR*64,0},
 {"TEX_FUNC4",(void*)&tex_func4,VAL_INT,9,VAL_INT|VAL_STR*4|VAL_STR*16|VAL_STR*64|VAL_STR*256,0}
#ifdef CG
,{"TEX_GENNORMALMAP",(void*)&tex_gennormalmap,VAL_INT,2,VAL_INT,0}
,{"TEX_SETNORMALMAP",(void*)&tex_setnormalmap,VAL_INT,2,VAL_INT|VAL_STR*4,0}
#endif
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
   if (UserFunc_ison==1) 
   {
    //tworz informacje o definiowaniu nowej funkcji
    char bf[256],pombf[64];
    sprintf(pombf,"-----BEGIN------\n");
    sprintf(bf,"%s%s(",pombf,UserFunc_name);
    for (int i=0; i<UserFunc_parcount; i++)
    {
     if (i<UserFunc_parcount-1)
     sprintf(pombf,"_%d,",i+1);
     else
     sprintf(pombf,"_%d",i+1);
     strcat(bf,pombf);
    }
    strcat(bf,")");
    sprintf(compbf,"%s\n{",bf);
   }
   else
   {
    if (strlen(s)>BUFLEN) s2[BUFLEN]=0;
    sprintf(compbf,"%s=%f",s2,ans);
    CSys::DiscardZeros(compbf);
   }
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
  if (UserFunc_ison==0) sprintf(compbf,"}\n------END-------");
  else
  {
   if (strlen(s)>BUFLEN) s2[BUFLEN]=0;
   sprintf(compbf," %s",s2);
  }
 }
 return compbf;
}

int main()
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
 UserFunc_Init(library);
  
 CSys::SetWindow(con,CSys::Win_Con);
 CSys::SetWindow(graph,CSys::Win_Graph);
 CSys::SetWindow(help,CSys::Win_Help);
 CSys::SetWindow(sound,CSys::Win_Sound);
 CSys::SetWindow(libwin,CSys::Win_Winlib);
 CSys::SetActiveWin(CSys::Win_Con);
 CSys::Run();
 CSys::Close();
 return 0;
}
