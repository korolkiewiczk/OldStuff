//cwin.h

#define FIXED_SHIFT 10
#define TEXT_FADE1 192
#define TEXT_FADE2 255
#define TEXT_FADEL 30

extern unsigned char biosfont[256*8];
extern unsigned char biosfont8x16[256*16];

class CWin
{
 protected:
 int width,height;
 unsigned int *buf;
 int fontwidth,fontheight;
 public:
 CWin(int w,int h,void *b);
 ~CWin();
 void PutPixel(int x,int y,int c);
 void Line(int x1,int y1,int x2,int y2,int col);
 void HLine(int x1,int y,int x2,int col);
 void VLine(int x,int y1,int y2,int col);
 void Bar(int x1,int y1,int x2,int y2,int col);
 void DrawChar8X8(int x,int y,unsigned int *pal,unsigned char c);
 void DrawChar8X16(int x,int y,unsigned int *pal,unsigned char c);
 void DrawText(int x,int y,unsigned int color,char *s);
 void DrawTextHighlighted(int x,int y,unsigned int color,int fade,char *s);
 void DrawText3X5(int x,int y,unsigned int color,char *s);
 void GTriangle_z(int xa,int ya,int xb,int yb,int xc,int yc,
                unsigned int ca,unsigned int cb,unsigned int cc,
                unsigned int *pal,unsigned int z,unsigned int *zbuf);
 void TTriangle_z(int xa,int ya,int xb,int yb,int xc,int yc,
                float fua,float fva,float fub,float fvb,float fuc,float fvc,
                CTex *tex,
                unsigned int ca,unsigned int cb,unsigned int cc,
                unsigned int z,unsigned int *zbuf);
 void PrintScreen(int neg=0);
 void *GetBuf() {return buf;}
 void Clear();
 virtual void KeyFunc(int k);
 virtual void Process();
 virtual void Change(int w,int h,void *b);
 virtual void Change(void *b);
 virtual void ChangeActiveState(int state);
 void ChangeFontSize(int w,int h);
};
