//copengl.c
//Dla systemu win32

/*
 RENDERING DO DIB A DO OKNA
 Rendering do bitmapy DIB bedzie wy³acznie wykonywany software'owo. Jednak
 kopiowanie do wlasciwego bufora przebiega juz szybko.
 Rendering do okna jest wspierany sprzetowo, jednak uzyskanie obrazu za pomoca
 glReadPixels moze byc nienajszybsze. Ostatecznie jednak zdecydowanie bardziej
 oplaca sie te drugie rozwiazanie.
 Jezeli zdefiniowana jest zmienna COPENGL_DIB to wykorzystywana jest pierwsza
 metoda, w przeciwnym razie druga.
*/

int COpenGL::width;
int COpenGL::height;
unsigned int *COpenGL::buf;
HDC COpenGL::hdc;
HGLRC COpenGL::hglrc;
#ifdef COPENGL_DIB
HBITMAP COpenGL::hbuf;
#else
HWND COpenGL::hbuf;
#endif
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

#ifdef CG
void (*glActiveTexture)(GLenum)=0;
#endif

int COpenGL::Init(int w,int h)
{
#ifdef COPENGL_DIB
//wykorzystaj bitmape DIB do generowania obrazu

 //inicjacja bufora DIB
 BITMAPINFO bi;
 memset(&bi,0,sizeof(bi));
 bi.bmiHeader.biSize=sizeof(bi);
 bi.bmiHeader.biWidth=w;
 bi.bmiHeader.biHeight=h;
 bi.bmiHeader.biPlanes=1;
 bi.bmiHeader.biBitCount=32;
 bi.bmiHeader.biCompression=BI_RGB;
 if (hdc) DeleteDC(hdc);
 if (hbuf) DeleteObject(hbuf);
 hdc=CreateCompatibleDC(0);
 hbuf=CreateDIBSection(hdc,&bi,DIB_RGB_COLORS,(void**)&buf,0,0);
 if (hbuf==0) return 0;
 if (!SelectObject(hdc,hbuf)) return 0;
 
 //inicjacja opengl
 PIXELFORMATDESCRIPTOR pfd;
 int format;
 // set the pixel format for the DC
 memset(&pfd,sizeof(pfd),0);
 pfd.nSize=sizeof(pfd);
 pfd.nVersion=1;
 pfd.dwFlags=PFD_DRAW_TO_BITMAP|PFD_SUPPORT_OPENGL|PFD_SUPPORT_GDI;
 pfd.iPixelType=PFD_TYPE_RGBA;
 pfd.cColorBits=32;
 pfd.cDepthBits=32;
 pfd.iLayerType=PFD_MAIN_PLANE;
 format=ChoosePixelFormat(hdc,&pfd);
 SetPixelFormat(hdc,format,&pfd);
 hglrc=wglCreateContext(hdc);
 wglMakeCurrent(hdc,hglrc);
#else
//wykorzystaj okno do generowania obrazu (szybsze)
 if(!initialized)
 {
  WNDCLASS copengl_wndc=
  {0,DefWindowProc,0,0,0/*hinst*/,0/*icon*/,0/*cursor*/,0/*bgrnd*/,0,"copengl"};
  copengl_wndc.hInstance=wingraph_hinst;
  if (!RegisterClass(&copengl_wndc)) return 0;
  if (!(hbuf=CreateWindow("copengl",0,WS_POPUP,0,0,0,0,0,0,wingraph_hinst,0))) return 0;
  SetWindowPos(hbuf,HWND_BOTTOM,0,0,w,h,SWP_NOACTIVATE|SWP_HIDEWINDOW);
 }
 else
 MoveWindow(hbuf,0,0,w,h,0);
 
 hdc=GetDC(hbuf);
 
 //inicjacja opengl
 PIXELFORMATDESCRIPTOR pfd;
 int format;
 // set the pixel format for the DC
 memset(&pfd,sizeof(pfd),0);
 pfd.nSize=sizeof(pfd);
 pfd.nVersion=1;
 pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
 pfd.iPixelType=PFD_TYPE_RGBA;
 pfd.cColorBits=32;
 pfd.cDepthBits=32;
 pfd.iLayerType=PFD_MAIN_PLANE;
 format=ChoosePixelFormat(hdc,&pfd);
 SetPixelFormat(hdc,format,&pfd);
 if (hglrc) wglDeleteContext(hglrc);
 hglrc=wglCreateContext(hdc);
 wglMakeCurrent(hdc,hglrc);
#endif

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
 
#ifdef CG
 if (glActiveTexture==0)
 {
  glActiveTexture=(void (*)(GLenum))wglGetProcAddress("glActiveTexture");
  if (glActiveTexture==0) glActiveTexture=(void (*)(GLenum))COpenGL::SetTexPar;
 }
#endif

 //ok.
 initialized=1;
 width=w;
 height=h;
 
 return 1;
}

void COpenGL::SetTexPar(int target)
{
 if (target==GL_TEXTURE_2D)
 {
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,gl_magf);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,gl_minf);
 }
 else
 if (target==GL_TEXTURE_CUBE_MAP)
 {
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,gl_magf);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,gl_minf);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT_ARB);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT_ARB);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_MIRRORED_REPEAT_ARB);
 }
}

void COpenGL::Close()
{
 if (!initialized) return;
 glFlush();
 wglMakeCurrent(0,0);
 wglDeleteContext(hglrc);
#ifdef COPENGL_DIB 
 DeleteObject(hbuf);
 DeleteDC(hdc);
#else
 ReleaseDC(hbuf,hdc);
 DestroyWindow(hbuf);
 UnregisterClass("copengl",wingraph_hinst);
#endif
 buf=0;
 initialized=0;
 width=0;
 height=0;
}

void COpenGL::Present(unsigned int *dbuf)
{
 if (!initialized) return;
#ifdef COPENGL_DIB
 glFlush();
#ifdef X86 
 asm("cld; rep movsd"::"S"(buf),"D"(dbuf),"c"(width*height));
#else
 memcpy(dbuf,buf,width*height*4);
#endif
#else
 MSG msg;
 while (PeekMessage(&msg,hbuf,0,0,PM_REMOVE)) ;
 glFlush();
 glReadPixels(0,0,width,height,GL_BGRA,GL_UNSIGNED_BYTE,dbuf);
#endif
}
