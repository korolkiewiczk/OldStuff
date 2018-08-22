//chelp.h

#define HELPBUFLEN 80
#define NOREADMETXT_TEXT "readme.txt not found! Try menu."

class CHelp:CWin
{
 private:
 char *helplines;
 int nhelpl,helpviewpos;
 int buflen;
 char *filename;
 public:
 CHelp(int w,int h,void *b,char *fn);
 void Load(char *fn);
 void KeyFunc(int k);
 void Process();
 void Change(int w,int h,void *b)
 {
  width=w;
  height=h;
  buf=(unsigned int*)b;
  Load(filename);
 }
};
