//glesext.c

#include <stdlib.h>
//gl.h include is requied

#define MAXTEXBIND 128

int glVA_texw[MAXTEXBIND];
int curtex=0;

void GLESEXT_init()
{
 memset(glVA_texw,0,sizeof(glVA_texw));
}

//opengles ext.
#define GL_TEXTURE_WIDTH 1
void glGetTexLevelParameteriv(int,int,int,int *w)
{
 *w=glVA_texw[curtex%MAXTEXBIND];
}

void GLESEXT_BindTexture(int a,int b)
{
 glBindTexture(a,b);
 curtex=b;
}
#define glBindTexture GLESEXT_BindTexture

#define GL_BGRA GL_RGBA
void glFrustum(GLfloat left,GLfloat right,GLfloat bottom,GLfloat top,GLfloat near,GLfloat far)
{
 glFrustumf(left,right,-bottom,-top,near,far);
}

#define glLightModeli glLightModelf

void GLESEXT_TexImage2D(int a,int b,int c,int d,int e,int f,int g,int h,void* buf)
{
 unsigned char *bf;
 bf=(unsigned char*)malloc(d*d*sizeof(int));
 for (int i=0; i<d*d; i++)
 {
  bf[4*i]=*(unsigned char*)((int)buf+4*i+2);
  bf[4*i+1]=*(unsigned char*)((int)buf+4*i+1);
  bf[4*i+2]=*(unsigned char*)((int)buf+4*i+0);
  bf[4*i+3]=*(unsigned char*)((int)buf+4*i+3);
 }
 glTexImage2D(a,b,c,d,e,f,g,h,bf);
 free(bf);

 glVA_texw[curtex%MAXTEXBIND]=d;
}
#define glTexImage2D GLESEXT_TexImage2D

#define GL_UNSIGNED_INT 0x6768
void GLESEXT_glDrawElements(GLenum mode,GLsizei count,GLenum type,const GLvoid *indices)
{
 if (type==GL_UNSIGNED_INT)
 {
  static unsigned short int *indextab_short=0;
  unsigned int *indextab_int=(unsigned int*)indices;
  if (indextab_short) free(indextab_short);
  indextab_short=(unsigned short int *)malloc(count*sizeof(unsigned short int));
  if (indextab_short==0) return;
  for (int i=0; i<count; i++) indextab_short[i]=indextab_int[i];
  glDrawElements(mode,count,GL_UNSIGNED_SHORT,(void*)indextab_short);
 }
 else
 glDrawElements(mode,count,type,(void*)indices);
}

#define glDrawElements GLESEXT_glDrawElements

#define GL_POINT_SIZE GL_SMOOTH_POINT_SIZE_RANGE
