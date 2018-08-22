//clibwin.h

#define MAXBTXTLEN 2048
#define CWCHARHEIGHT 8

class CLibWin:CWin
{
 private:
 char tbuf[MAXBTXTLEN];
 char ebuf[MAXTXTLEN];
 CLib *lib;
 int x1,y1,x2,y2;
 int x10,y10,x20,y20;
 int w0,h0;
 int npos;
 int pos;
 int lpos;
 int lines;
 public:
 static char *Num2Char[9];
 char retbuf[MAXTXTLEN];
 CLibWin(int w,int h,void *b,int _x1,int _y1,int _x2,int _y2,CLib *l):CWin(w,h,b)
  {
   lib=l; x1=_x1; y1=_y1; x2=_x2; y2=_y2; lines=(y2-y1)/CWCHARHEIGHT; pos=npos=lpos=0; ebuf[0]=0;
   x10=x1; y10=y1; x20=x2; y20=y2;
   w0=w;
   h0=h;
   retbuf[0]=0;
  }
 void Change(int w,int h,void *b)
 {
  float dw=(float)w/(float)w0,dh=(float)h/(float)h0;
  width=w;
  height=h;
  x1=(int)((float)x10*dw);
  x2=(int)((float)x20*dw);
  y1=(int)((float)y10*dh);
  y2=(int)((float)y20*dh);
  buf=(unsigned int*)b;
  lines=(y2-y1)/CWCHARHEIGHT; pos/*=npos*/=lpos=0;
  Set(ebuf);
 }
 void Clear() {ebuf[0]=0;}
 void Set(char *t);
 void Draw();
 void KeyFunc(int k);
};
