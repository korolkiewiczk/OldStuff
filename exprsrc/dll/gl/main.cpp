/*
  BIBLIOTEKA STANOWI PRZYKLAD
  NA JEJ PODSTAWIE MOZNA TWORZYC KOLEJNE FUNKCJE DODAJAC JE DO STRUKTURY DLL_LIB
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl/gl.h>

#include "clib.h"
#include "env.h"

ENV expr_env;

//zmienne

int gl_points=GL_POINTS;
int gl_lines=GL_LINES;
int gl_triangles=GL_TRIANGLES;
int gl_depth_test=GL_DEPTH_TEST;
int gl_line_smooth=GL_LINE_SMOOTH;
int gl_point_smooth=GL_POINT_SMOOTH;

/*
  FUNKCJE
*/



int gl_Vertex3f(float a,float b,float c)
{
 if (((*expr_env.MultiExec>0)||((*expr_env.MultiExec<0)&&(*expr_env.GraphState<Enable3DMode))))
 return 0;
 glVertex3f(a,b,c);
 return 1;
}

int gl_Color4f(float a,float b,float c,float d)
{
 if (((*expr_env.MultiExec>0)||((*expr_env.MultiExec<0)&&(*expr_env.GraphState<Enable3DMode))))
 return 0;
 glColor4f(a,b,c,d);
 return 1;
}

int gl_Begin(int mode)
{
 if (((*expr_env.MultiExec>0)||((*expr_env.MultiExec<0)&&(*expr_env.GraphState<Enable3DMode))))
 return 0;
 glBegin(mode);
 return 1;
}

int gl_End()
{
 if (((*expr_env.MultiExec>0)||((*expr_env.MultiExec<0)&&(*expr_env.GraphState<Enable3DMode))))
 return 0;
 glEnd();
 return 1;
}

int gl_Enable(int e)
{
 if (((*expr_env.MultiExec>0)||((*expr_env.MultiExec<0)&&(*expr_env.GraphState<Enable3DMode))))
 return 0;
 glEnable(e);
 return 1;
}

int gl_Disable(int e)
{
 if (((*expr_env.MultiExec>0)||((*expr_env.MultiExec<0)&&(*expr_env.GraphState<Enable3DMode))))
 return 0;
 glDisable(e);
 return 1;
}

int gl_LineWidth(float w)
{
 if (((*expr_env.MultiExec>0)||((*expr_env.MultiExec<0)&&(*expr_env.GraphState<Enable3DMode))))
 return 0;
 glLineWidth(w);
 return 1;
}

int gl_PointSize(float w)
{
 if (((*expr_env.MultiExec>0)||((*expr_env.MultiExec<0)&&(*expr_env.GraphState<Enable3DMode))))
 return 0;
 glPointSize(w);
 return 1;
}

/*
  BIBLIOTEKA FUNKCJI
*/

ELEMENT dll_lib[]=
{
 {"GL_BEGIN",(void*)gl_Begin,VAL_INT,1,VAL_INT,0},
 {"GL_END",(void*)gl_End,VAL_INT,0,0,0},
 {"GL_VERTEX",(void*)gl_Vertex3f,VAL_INT,3,0,0},
 {"GL_COLOR",(void*)gl_Color4f,VAL_INT,4,0,0},
 {"GL_ENABLE",(void*)gl_Enable,VAL_INT,1,VAL_INT,0},
 {"GL_DISABLE",(void*)gl_Disable,VAL_INT,1,VAL_INT,0},
 {"GL_LINEWIDTH",(void*)gl_LineWidth,VAL_INT,1,0,0},
 {"GL_POINTSIZE",(void*)gl_PointSize,VAL_INT,1,0,0},
 
 {"GL_POINTS",&gl_points,VAL_INT,VAR,0,0},
 {"GL_LINES",&gl_lines,VAL_INT,VAR,0,0},
 {"GL_TRIANGLES",&gl_triangles,VAL_INT,VAR,0,0},
 {"GL_DEPTH_TEST",&gl_depth_test,VAL_INT,VAR,0,0},
 {"GL_LINE_SMOOTH",&gl_line_smooth,VAL_INT,VAR,0,0},
 {"GL_POINT_SMOOTH",&gl_point_smooth,VAL_INT,VAR,0,0}
};


/*
  FUNKCJA REFERENCYJNA
*/

extern "C"
{
 __declspec (dllexport) int RefFunc(void **lib,ENV *env)
 {
  memcpy(&expr_env,(void*)env,sizeof(ENV));
  *lib=dll_lib;
  return sizeof(dll_lib)/sizeof(ELEMENT);
 }
}
