//copengl.c
//Dla systemu s60

#include "sysext/glesext.c"

int COpenGL::width,COpenGL::height;
unsigned int *COpenGL::buf;

EGLDisplay COpenGL::iEglDisplay;
EGLContext COpenGL::iEglContext;
EGLSurface COpenGL::iEglSurface;
EGLConfig COpenGL::config;
EGLConfig *COpenGL::configList;
int COpenGL::configSize;
int COpenGL::numOfConfigs;

int COpenGL::initialized;
int COpenGL::gl_magf,COpenGL::gl_minf;
int COpenGL::gl_nearest;
int COpenGL::gl_nearest_mipmap_nearest;
int COpenGL::gl_linear_mipmap_nearest;
int COpenGL::gl_fog;
float COpenGL::gl_fogcoltab[4];
float COpenGL::gl_fogdensity;
int COpenGL::gl_shademodel,COpenGL::gl_flat;
int COpenGL::gl_blend;

int COpenGL::Init(int w,int h)
{
 configList=0;
 configSize=0;
 numOfConfigs=0;
 iEglDisplay=eglGetDisplay(EGL_DEFAULT_DISPLAY);
 eglInitialize(iEglDisplay,NULL,NULL);
 eglGetConfigs(iEglDisplay,configList,configSize,&numOfConfigs);
 configSize=numOfConfigs;
 configList=(EGLConfig*)User::Alloc(sizeof(EGLConfig)*configSize);
 const int attrib_list[] = {EGL_BUFFER_SIZE,32,
                            EGL_DEPTH_SIZE,16,
 EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_NONE};
 eglChooseConfig(iEglDisplay,attrib_list,configList,configSize,&numOfConfigs);
 config = configList[0];

 const EGLint attrib_list2[]={EGL_WIDTH,
                              w,
                              EGL_HEIGHT,
                              h,
                              EGL_NONE};

 iEglSurface=eglCreatePbufferSurface(iEglDisplay,config,attrib_list2);
 User::Free(configList);
 iEglContext=eglCreateContext(iEglDisplay,config,NULL,NULL);
 eglMakeCurrent(iEglDisplay,iEglSurface,iEglSurface,iEglContext);

 //glVAInit(1024*1024/30); //(3+4+3+2)*4*1MB/30=2MB
 GLESEXT_init();
 
 gl_magf=GL_LINEAR;
 gl_minf=GL_LINEAR_MIPMAP_LINEAR;
 gl_nearest=GL_NEAREST;
 gl_nearest_mipmap_nearest=GL_NEAREST_MIPMAP_NEAREST;
 gl_linear_mipmap_nearest=GL_LINEAR_MIPMAP_NEAREST;
 gl_fog=0;
 gl_fogcoltab[0]=gl_fogcoltab[1]=gl_fogcoltab[2]=gl_fogcoltab[3]=1.f;
 gl_fogdensity=0.1f;
 gl_shademodel=GL_SMOOTH,gl_flat=GL_FLAT;
 gl_blend=0;
 CSys::AddVar("GL_TEXTURE_MAG_FILTER",&gl_magf,VAR_DWORD);
 CSys::AddVar("GL_TEXTURE_MIN_FILTER",&gl_minf,VAR_DWORD);
 CSys::AddVar("GL_NEAREST",&gl_nearest,VAR_DWORD);
 CSys::AddVar("GL_NEAREST_MIPMAP_NEAREST",&gl_nearest_mipmap_nearest,VAR_DWORD);
 CSys::AddVar("GL_LINEAR_MIPMAP_NEAREST",&gl_linear_mipmap_nearest,VAR_DWORD);
 CSys::AddVar("GL_FOG",&gl_fog,VAR_DWORD);
 CSys::AddVar("GL_FOGCOLOR",&gl_fogcoltab,VAR_FLOAT);
 CSys::AddVar("GL_FOGDENSITY",&gl_fogdensity,VAR_FLOAT);
 CSys::AddVar("GL_SHADEMODEL",&gl_shademodel,VAR_DWORD);
 CSys::AddVar("GL_FLAT",&gl_flat,VAR_DWORD);
 CSys::AddVar("GL_BLEND",&gl_blend,VAR_DWORD);
 CSys::ExecCfg("opengl");
 /*CSys::DelVar("GL_FOG");
 CSys::DelVar("GL_LINEAR_MIPMAP_NEAREST");
 CSys::DelVar("GL_NEAREST_MIPMAP_NEAREST");
 CSys::DelVar("GL_NEAREST");
 CSys::DelVar("GL_TEXTURE_MIN_FILTER");
 CSys::DelVar("GL_TEXTURE_MAG_FILTER");*/
 if (gl_fog) 
 {
  glEnable(GL_FOG);
  glFogfv(GL_FOG_COLOR,(GLfloat*)&gl_fogcoltab);
  glFogf(GL_FOG_DENSITY,gl_fogdensity);
 }
 if (gl_blend)
 {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 }
 glShadeModel(gl_shademodel);

 //ok.
 initialized=1;
 width=w;
 height=h;
 return 1;
}

void COpenGL::SetTexPar()
{
 glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,(GLfloat)gl_magf);
 glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,(GLfloat)gl_minf);
}

void COpenGL::Close()
{
 if (!initialized) return;
 glFlush();
 
 eglMakeCurrent(iEglDisplay,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
 eglDestroySurface(iEglDisplay,iEglSurface);
 eglDestroyContext(iEglDisplay,iEglContext);
 eglTerminate(iEglDisplay);
 
 buf=0;
 initialized=0;
 width=0;
 height=0;
}

void COpenGL::Present(unsigned int *dbuf)
{
 glFlush();
 eglCopyBuffers(iEglDisplay,iEglSurface,s60graph_fbsbitmap);
}
