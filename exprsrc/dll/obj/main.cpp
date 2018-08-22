/*
  OBJ files loading and presenting
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl/gl.h>
#include "debug.c"

#include "clib.h"
#include "env.h"

ENV expr_env;

#define FOPEN expr_env.CSys_FOpen
#define MAXOBJ 256
#define MAXVCOUNT (1024*1024/4)//32786
#define MAXFCOUNT 16

typedef struct _POINT3F
{
 float x,y,z;
} POINT3F;

typedef struct _NORMAL3F
{
 float nx,ny,nz;
} NORMAL3F;

typedef struct _TEXCOORD2F
{
 float u,v;
} TEXCOORD2F;

typedef struct _OBJ
{
 POINT3F *p;
 NORMAL3F *n;
 TEXCOORD2F *t;
 int count;
 float scale;
 POINT3F pos;
 POINT3F rot;
 float color[4];
} OBJ;

OBJ objtab[MAXOBJ];


/*
  FUNKCJE
*/

float point[MAXVCOUNT][3];
float norm[MAXVCOUNT][3];
float texc[MAXVCOUNT][2];
int indp[MAXVCOUNT];
int indn[MAXVCOUNT];
int indt[MAXVCOUNT];

int loadobj(int no,char *name)
{
 if (*expr_env.MultiExec!=0) return 0;
 FILE *f;
 char bf[1024];
 float a,b,c;
 int linefaceind[MAXFCOUNT][3];
 int v_l=0,vn_l=0,vt_l=0;
 int iv_l=0,ivn_l=0,ivt_l=0;
 
 f=FOPEN(name,"rt");
 if (!f) return 0;
 while (!feof(f))
 {
  fgets(bf,1024,f);
  char *s=bf;
  while ((*s!=0)&&(*s>32)) s++;
  *s=0;
  s++;
  while (*s==' ') s++;
  if (strcmp(bf,"v")==0)
  {
   //point
   sscanf(s,"%f %f %f",&a,&b,&c);
   point[v_l][0]=a;
   point[v_l][1]=c;
   point[v_l][2]=-b;
   v_l++;
  }
  else
  if (strcmp(bf,"vt")==0)
  {
   //tex coord
   sscanf(s,"%f %f",&a,&b);
   texc[vt_l][0]=a;
   texc[vt_l][1]=b;
   vt_l++;
  }
  else
  if (strcmp(bf,"vn")==0)
  {
   //normal
   sscanf(s,"%f %f %f",&a,&b,&c);
   norm[vn_l][0]=a;
   norm[vn_l][1]=c;
   norm[vn_l][2]=-b;
   vn_l++;
  }
  else
  if (strcmp(bf,"f")==0)
  {
   int i=0; //str count
   int j=0; //point count
   int k=0; //tab count
   memset(linefaceind,0,sizeof(linefaceind));
   int num=0;
   while (*s)
   {
    if (*s=='/')
    {
     if (num)
     {
      bf[i]=0;
      linefaceind[j][k]=atoi(bf);
      i=0;
      num=0;
     }
     k++;
    }
    else
    if (*s>32)
    {
     if ((*s>='0')&&(*s<='9'))
     {
      bf[i++]=*s;
      num=1;
     }
    }
    else
    if (num)
    {
     //last v
     bf[i]=0;
     linefaceind[j][k]=atoi(bf);
     i=0;
     k=0;
     j++;
     while (*s==' ') s++;
     s--;
     num=0;
    }
    s++;
   }
   for (i=0; i<j; i++)
   {
    //create triangles from face indexes
    if (i<3)
    {
     int k=2-i%3+i/3;
     if (linefaceind[k][0]!=0)
     indp[iv_l++]=linefaceind[k][0]-1;
     if (linefaceind[k][1]!=0)
     indt[ivt_l++]=linefaceind[k][1]-1;
     if (linefaceind[k][2]!=0)
     indn[ivn_l++]=linefaceind[k][2]-1;
    }
    if (i>=3)
    {
     int k0=0,k1=i-1,k2=i;
     if (linefaceind[k2][0]!=0)
     indp[iv_l++]=linefaceind[k2][0]-1;
     if (linefaceind[k2][1]!=0)
     indt[ivt_l++]=linefaceind[k2][1]-1;
     if (linefaceind[k2][2]!=0)
     indn[ivn_l++]=linefaceind[k2][2]-1;
     if (linefaceind[k1][0]!=0)
     indp[iv_l++]=linefaceind[k1][0]-1;
     if (linefaceind[k1][1]!=0)
     indt[ivt_l++]=linefaceind[k1][1]-1;
     if (linefaceind[k1][2]!=0)
     indn[ivn_l++]=linefaceind[k1][2]-1;
     if (linefaceind[k0][0]!=0)
     indp[iv_l++]=linefaceind[k0][0]-1;
     if (linefaceind[k0][1]!=0)
     indt[ivt_l++]=linefaceind[k0][1]-1;
     if (linefaceind[k0][2]!=0)
     indn[ivn_l++]=linefaceind[k0][2]-1;
    }
   }
  }
 }
 if (no<0) no=0;
 no%=MAXOBJ;
 if (objtab[no].p) free(objtab[no].p);
 if (objtab[no].n) free(objtab[no].n);
 if (objtab[no].t) free(objtab[no].t);
 memset(&objtab[no],0,sizeof(objtab[no]));
 if ((v_l)&&(iv_l))
 {
  objtab[no].p=(POINT3F*)malloc(iv_l*sizeof(POINT3F));
  for (int i=0; i<iv_l; i++)
  {
   memcpy(&objtab[no].p[i],point[indp[i]],sizeof(POINT3F));
  }

  objtab[no].count=iv_l;
 }
 if ((vn_l)&&(ivn_l))
 {
  objtab[no].n=(NORMAL3F*)malloc(iv_l*sizeof(NORMAL3F));
  for (int i=0; i<iv_l; i++)
  {
   memcpy(&objtab[no].n[i],norm[indn[i]],sizeof(NORMAL3F));
  }
 }
 if ((vt_l)&&(ivt_l))
 {
  objtab[no].t=(TEXCOORD2F*)malloc(iv_l*sizeof(TEXCOORD2F));
  for (int i=0; i<iv_l; i++)
  {
   memcpy(&objtab[no].t[i],texc[indt[i]],sizeof(TEXCOORD2F));
  }               
 }
 fclose(f);
 objtab[no].scale=1.f;
 memset(&objtab[no].pos,0,sizeof(objtab[no].pos));
 objtab[no].color[0]=1.f;
 objtab[no].color[1]=1.f;
 objtab[no].color[2]=1.f;
 objtab[no].color[3]=1.f;
 return iv_l/3;
}

int drawobj(int no)
{
 if (*expr_env.MultiExec>0) return 0;
 if (!(*expr_env.GraphState&EnableOpenGL)) return 0;
 if (!(*expr_env.GraphState&Enable3DMode)) return 0;
 if (no<0) no=0;
 no%=MAXOBJ;

 int pm[2];
 glGetIntegerv(GL_POLYGON_MODE,(GLint*)&pm);
 if (!(*expr_env.GraphState&FillMode))
 glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
 else
 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
 
 glColor4fv((GLfloat*)&objtab[no].color);
 if (objtab[no].p) 
 {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3,GL_FLOAT,0,(void*)objtab[no].p);
 }
 
 if ((objtab[no].n)&&(*expr_env.GraphState&EnableLight))
 {
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT,0,(void*)objtab[no].n);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  //glShadeModel(GL_FLAT);
 }
 
 if ((objtab[no].t)&&(*expr_env.GraphState&EnableTexture))
 {
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2,GL_FLOAT,0,(void*)objtab[no].t);
  glEnable(GL_TEXTURE_2D);
 }
 glPushMatrix();
 glRotatef(objtab[no].rot.x,1,0,0);
 glRotatef(objtab[no].rot.y,0,1,0);
 glRotatef(objtab[no].rot.z,0,0,1);
 glTranslatef(objtab[no].pos.x,objtab[no].pos.y,objtab[no].pos.z);
 glScalef(objtab[no].scale,objtab[no].scale,objtab[no].scale);
 glDrawArrays(GL_TRIANGLES,0,objtab[no].count);
 glPopMatrix();
  
 if (objtab[no].p) 
 {
  glDisableClientState(GL_VERTEX_ARRAY);
 }
 
 if ((objtab[no].n)&&(*expr_env.GraphState&EnableLight))
 {
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_NORMALIZE);
  glDisable(GL_COLOR_MATERIAL);
 }
 
 if ((objtab[no].t)&&(*expr_env.GraphState&EnableTexture))
 {
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisable(GL_TEXTURE_2D);
 }
 glPolygonMode(GL_FRONT_AND_BACK,pm[1]);
 return 1;
}

int setobjpar(int no,float s,float x,float y,float z,float rx,float ry,float rz)
{
 objtab[no].scale=s;
 objtab[no].pos.x=x;
 objtab[no].pos.y=z;
 objtab[no].pos.z=-y;
 objtab[no].rot.x=rx;
 objtab[no].rot.y=ry;
 objtab[no].rot.z=rz;
 return 1;
}

int setobjcol(int no,float r,float g,float b,float a)
{
 objtab[no].color[0]=r;
 objtab[no].color[1]=g;
 objtab[no].color[2]=b;
 objtab[no].color[3]=a;
 return 1;
}

/*
  BIBLIOTEKA FUNKCJI
*/

ELEMENT dll_lib[]=
{
 {"OBJ_LOAD",(void*)loadobj,VAL_INT,2,VAL_INT|VAL_STR*4,0},
 {"OBJ_DRAW",(void*)drawobj,VAL_INT,1,VAL_INT,0},
 {"OBJ_SETPAR",(void*)setobjpar,VAL_INT,8,VAL_INT,0},
 {"OBJ_SETCOL",(void*)setobjcol,VAL_INT,5,VAL_INT,0}
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
