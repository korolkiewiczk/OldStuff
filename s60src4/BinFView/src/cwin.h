//cwin.h

#define FIXED_SHIFT 10
#define TEXT_FADE1 192
#define TEXT_FADE2 255
#define TEXT_FADEL 30
#define _MEM(a,b,c) (*(c*)((unsigned int)(a)+(b)*sizeof(c)))
#define BACKGROUNDCOLOR RGB32(0,0,128)

extern unsigned char biosfont[256*8];
extern unsigned char biosfont8x16[256*16];

class CWin
{
 public:
 int width,height;
 unsigned int *buf;
 int fontwidth,fontheight;
 
 enum
 {
  CNormal,
  CHighlighted,
  CFaded,
  CFavour,
  CHelp,
  CNum,
  COp,
  CTxt,
  CPattern,
  CMax
 };
        
 CWin(int w,int h,void *b);
 ~CWin();
 unsigned int FadeColor(unsigned int c,unsigned char fade);
 void PutPixel(int x,int y,int c);
 void Line(int x1,int y1,int x2,int y2,int col);
 void HLine(int x1,int y,int x2,int col);
 void VLine(int x,int y1,int y2,int col);
 void Bar(int x1,int y1,int x2,int y2,int col);
 void DrawChar8X8(int x,int y,unsigned int *pal,unsigned char c);
 void DrawChar8X16(int x,int y,unsigned int *pal,unsigned char c);
 void DrawText(int x,int y,unsigned int color,char *s);
 void DrawText3X5(int x,int y,unsigned int color,char *s);

 void PrintScreen(int neg=0);
 void Clear();
 virtual void Change(int w,int h,void *b);
 virtual void Change(void *b);
 virtual void ChangeActiveState(int state);
 void ChangeFontSize(int w,int h);
};
