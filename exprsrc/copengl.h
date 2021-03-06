//copengl.h

class COpenGL
{
 private:
 static int width,height;
 static unsigned int *buf;
 static HDC hdc;
 static HGLRC hglrc;
#ifdef COPENGL_DIB
 static HBITMAP hbuf;
#else
 static HWND hbuf;
#endif
 static int initialized;
 static int gl_magf,gl_minf;
 static int gl_nearest;
 static int gl_nearest_mipmap_nearest;
 static int gl_linear_mipmap_nearest;
 static int gl_fog;
 static float gl_fogcoltab[4];
 static float gl_fogdensity;
 static int gl_shademodel,gl_flat;
 static int gl_blend;
 public:
 static int Init(int w,int h);  //jedynie trybu 32 bpp
 static void SetTexPar(int target=GL_TEXTURE_2D);
 static void Close();
 static void Present(unsigned int *dbuf);
 static unsigned int *GetBuf() {return buf;};
 static unsigned int Used() {return initialized;}
};
