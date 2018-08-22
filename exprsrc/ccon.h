//ccon.h

#define BUFLEN 256
#define LINES 256
#define EDIT 128
#define CURSORINTERVAL 500
#define MAXEXECFILES 8

class CCon:CWin
{
 private:
 char lines[LINES][BUFLEN];
 char edit[EDIT][BUFLEN];
 char *prompt;
 int wskl,wsklv,wske,wskh;
 int colshf;
 int ecursor,elen,estart;
 CLibWin *libwin;
 int echo;
 
 FILE *fexecstack[MAXEXECFILES];
 int fexecsl;
 
 char *(*reffunc)(char *);
 void Enter();
 public:
 int libwinon;
 static CCon *currentcon;
 CCon(int w,int h,void *b,char *(*r)(char *),CLibWin *lw):CWin(w,h,b)
 {
  lines[0][0]=0;
  edit[0][0]=0;
  colshf=0;
  prompt="";
  reffunc=r;
  estart=ecursor=elen=wskl=wsklv=wske=wskh=0;
  libwin=lw;
  libwinon=0;
  fexecsl=-1;
  FExec(0);
  CSys::AddVar("font_width",&fontwidth,VAR_DWORD);
  CSys::AddVar("font_height",&fontheight,VAR_DWORD);
  CSys::ExecCfg("con");
  echo=0;
 }
 void Change(int w,int h,void *b)
 {
  width=w;
  height=h;
  buf=(unsigned int*)b;
  SetPrompt(prompt);
 }
 void Edit(char c);
 void KeyFunc(int k);
 void Process();
 void Put(char *s);
 void SetPrompt(char *s) {prompt=s;}
 int FExec(char *f);
 void Echo(int e) {echo=e;}
 void Cls() {echo=estart=ecursor=elen=wskl=wsklv=wske=wskh=0; strcpy(edit[0],"");}
 static CCon *GetCon() {return currentcon;}
 void ChangeFontSize(int w,int h);
};
