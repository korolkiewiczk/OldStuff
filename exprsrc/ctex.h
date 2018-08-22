//ctex.h

#define MAXMIPMAPS 16

class CTex
{
 private:
 int width,height;
 int shift;
 float x0,y0,w,h;
 int texid;
 unsigned int *b[MAXMIPMAPS];
 int cubemaptex_side;
#ifdef OPENGL
 int texbinded;
#ifdef CG
 int ntexid;
 int ntexshift;
 int ntexwidth,ntexheight;
 float nmapheight;
 unsigned int *nmap[MAXMIPMAPS];
 public:
 void BuildNormalMap(float h);
 int SetNormalMap(char *fn);
#endif
#endif
 public:
 int peekshift,peekmask;
 unsigned int *peekbuf;
 CTex() 
 {
  memset(this,0,sizeof(CTex));
  w=h=1.f;
  shift=-1;
#ifdef CG
  ntexshift=-1;
#endif
 }
 ~CTex();
 int Load(char *fn);
 void BuildMipMaps(unsigned int *tbuf[],int twidth,int tshift);
 void SetAlpha(float a); //a=<0, 1>
 int TexFunc1(char *s,float x1,float y1,float x2,float y2);
 int TexFunc3(char *sr,char *sg,char *sb,float x1,float y1,float x2,float y2);
 int TexFunc4(char *sr,char *sg,char *sb,char *sa,float x1,float y1,float x2,float y2);
 int IsLoaded() {return (int)b[0]>0;}
 void SetParameters(float a,float b,float c,float d) {x0=a; y0=b; w=1.f/c; h=1.f/d;}
 void SetAsCubeMapTex(int side,int id) {cubemaptex_side=side+1; texid=id;}
#ifdef OPENGL
 void SetGL(int mode);
 int GetTexId() {return texid;}
#ifdef CG
 int GetNTexId() {return ntexid;}
#endif
#endif
 float GetU(float x) {return (x-x0)*w;}
 float GetV(float y) {return (y-y0)*h;}
 int GetSize(int lev) {return width>>lev;}
 int GetMaxLev() {return shift;}
 void SetPeekLev(int lev) {peekbuf=b[lev]; peekshift=(shift-lev); peekmask=(1<<(shift-lev))-1;}
 unsigned int inline Peek(int u,int v)
 {
  return *(unsigned int*)(((int)peekbuf+((u&peekmask)+((v&peekmask)<<peekshift))
  *sizeof(unsigned int)));
 }
};
