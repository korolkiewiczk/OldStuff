#ifndef _WINOPENGL_H_
#define _WINOPENGL_H_
//winopengl.h

#define OPENGL_FULLSCREEN 1

extern int OpenGL_width;
extern int OpenGL_height;
extern int OpenGL_bpp;
extern int OpenGL_flags;
extern HDC OpenGL_hdc;
extern HGLRC OpenGL_hglrc;
extern HWND OpenGL_hwnd;
extern int OpenGL_initialized;
extern int OpenGL_active;
extern DEVMODE OpenGL_dm;

extern unsigned char OpenGL_keytab[256];
extern unsigned char OpenGL_mousekeystate[3];
extern int OpenGL_mousewheeldelta;

#ifdef __cplusplus
extern "C"
{
#endif
 int OpenGL_Init(int w,int h,int bpp,int flags);
 void OpenGL_SetWindowText(char *t);
 void OpenGL_Close();
 int OpenGL_Present();
 int KeyPressed(int k);
 int GetKey(int k);
 int MouseKeyPressed(int k);
 int GetMouseWheelDelta();
#ifdef __cplusplus
};
#endif

#endif
