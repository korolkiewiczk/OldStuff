//csys.c
//Dla systemu win32

//domyslne wartosci
unsigned int CSys::Color[]=
{
 RGB32(210,210,210),
 RGB32(230,230,230),
 RGB32(128,128,128),
 RGB32(240,0,0),
 RGB32(240,240,0),
 RGB32(128,240,240),
 RGB32(200,200,240),
 RGB32(255,0,0),
 RGB32(0,0,0)
};

//prywatne
void *CSys::windows[MAXWIN];
int CSys::activewin;
CLib CSys::varlib;
int CSys::DLibrariesC=0;
char CSys::DLibrariesNames[MAXLIB][64];

//zalezne od systemu
int CSys::VidModes[8][2]=
{
 {320,240},   //F1
 {640,480},   //F2
 {800,600},   //F3
 {1024,768},  //F4
 {1280,1024}, //F5
 {320,200},   //F6
 {640,400},   //F7
 {1280,800},  //F8
};
int CSys::DLibraries[MAXLIB];
int CSys::Windowed=1;
void *CSys::RecBuf=0;
int CSys::RecBufPos=0;
FILE *CSys::logfile=(FILE*)-1;
unsigned char CSys::simulatedkeytab[256];

//publiczne
int CSys::DColor=0;
int CSys::PresentWait=0;
int CSys::ScrWidth=SCRWIDTH,CSys::ScrHeight=SCRHEIGHT,CSys::ScrBpp=BPP;
int CSys::SoundOn=0;
int CSys::SoundFreq=44100,CSys::SoundBPS=16,CSys::SoundChannels=2;
int CSys::SoundMixerBufSize;
int CSys::SRand0;

//funkcje zalezne od systemu

//obsluga menu

#ifdef MENU

CMenu CSys::Menu;

void MenuCommandFunc(int w)
{
 char bf[1024];
 char bf2[256];
 int vmode=8,dmode=4,dcolor=8;
 w&=0xffff;
 switch (w)
 {
  case MENU_FILE_WINDOW_CON:
       CSys::SimulateKey(K_F4);
       break;
  case MENU_FILE_WINDOW_GRAPH:
       CSys::SimulateKey(K_F5);
       break;
  case MENU_FILE_WINDOW_SOUND:
       CSys::SimulateKey(K_F6);
       break;
  case MENU_FILE_WINDOW_HELP:
       CSys::SimulateKey(K_F1);       
       break;
  case MENU_FILE_DCOLOR1:
       dcolor--;
  case MENU_FILE_DCOLOR2:
       dcolor--;
  case MENU_FILE_DCOLOR3:
       dcolor--;
  case MENU_FILE_DCOLOR4:
       dcolor--;
  case MENU_FILE_DCOLOR5:
       dcolor--;
  case MENU_FILE_DCOLOR6:
       dcolor--;
  case MENU_FILE_DCOLOR7:
       dcolor--;
  case MENU_FILE_DCOLOR8:
       dcolor--;
       CSys::SimulateKey(K_CTRL);
       CSys::SimulateKey(K_1+dcolor);
       break;
       
  case MENU_FILE_VIDMODE_F1:
       vmode--;
  case MENU_FILE_VIDMODE_F2:
       vmode--;
  case MENU_FILE_VIDMODE_F3:
       vmode--;
  case MENU_FILE_VIDMODE_F4:
       vmode--;
  case MENU_FILE_VIDMODE_F5:
       vmode--;
  case MENU_FILE_VIDMODE_F6:
       vmode--;
  case MENU_FILE_VIDMODE_F7:
       vmode--;
  case MENU_FILE_VIDMODE_F8:
       vmode--;
       CSys::SimulateKey(K_CTRL);
       CSys::SimulateKey(K_F1+vmode);
       break;
       
  case MENU_FILE_VIDMODE_WINDOWED:
       CSys::SimulateKey(K_CTRL);
       CSys::SimulateKey(K_W);
       break;
  
  case MENU_FILE_PSCR:
       CSys::SimulateKey(K_F9);
       break;
       
  case MENU_FILE_PSCRNEG:
       CSys::SimulateKey(K_LSHF);
       CSys::SimulateKey(K_F9);
       break;
  
  case MENU_FILE_LOADTEX:
       sprintf(bf2,"Load texture no. %d",CSys::DColor%MAXFUNCCOUNT+1);
       if (CSys::OpenDialog(bf2,"Bitmaps (*.bmp)\0*.bmp\0\0",bf,1024))
       CGraph::GetGraph()->tex[CSys::DColor%MAXFUNCCOUNT].Load(bf);
       break;
  case MENU_FILE_LOADDLL:
       if (CSys::OpenDialog("Load plugin from DLL","DLL files (*.dll)\0*.dll\0\0",bf,1024))
       CSys::DLibLoad(bf);
       break;
       
  case MENU_FILE_LOADFONT:
       if (CSys::OpenDialog("Load 8X8 or 8X16 font from binary file",
       "Binary files (*.bin)\0*.bin\0All (*.*)\0*.*\0",bf,1024))
       {
        FILE *f;
        f=CSys::FOpen(bf,"rb");
        if (!f) break;
        fseek(f,0,SEEK_END);
        int fs=ftell(f);
        fseek(f,0,SEEK_SET);
        if (fs==256*8)
        fread(biosfont,1,256*8,f);
        else
        if (fs==256*16)
        fread(biosfont8x16,1,256*16,f);
        fclose(f);
       }
       break;
       
  case MENU_FILE_EXIT:
       CSys::SimulateKey(K_F8);
       break;
       
  case MENU_CONSOLE_CLEAR:
       CCon::GetCon()->Cls();
       break;
  case MENU_CONSOLE_PUTPATH:
       if (!CSys::activewin==CSys::Win_Con) break;
       if (!CSys::OpenDialog("Put path","All (*.*)\0*.*\0Bitmaps (*.bmp)\0*.bmp\0",bf,1024))
       break;
       char *bf_ptr;
       bf_ptr=bf;
       while (*bf_ptr) 
       {
        if (*bf_ptr=='\\') *bf_ptr='/';
        CCon::GetCon()->Edit(*bf_ptr++);
       }
       /*HGLOBAL bf_mem;
       char *bf_ptr;
       bf_mem=GlobalAlloc(GMEM_MOVEABLE,1024);
       if (bf_mem)
       {
        bf_ptr=bf;
        while (*bf_ptr++) if (*bf_ptr=='\\') *bf_ptr='/';
        bf_ptr=(char*)GlobalLock(bf_mem);
        strcpy(bf_ptr,bf);
        OpenClipboard(wingraph_wnd);
        SetClipboardData(CF_TEXT,bf_mem);
        CloseClipboard();
        GlobalFree(bf_mem);
        CSys::SimulateKey(K_LSHF);
        CSys::SimulateKey(K_ENTER);
       }*/
       break;
  case MENU_CONSOLE_FLIST:
       if (CSys::activewin==CSys::Win_Con) CSys::SimulateKey(K_TAB);
       break;
  case MENU_CONSOLE_FONTSIZE_8X8:
       CCon::GetCon()->ChangeFontSize(8,8);
       CSys::Menu.SetItemInfo(MENU_CONSOLE_FONTSIZE_8X8,CMenu::Checked);
       CSys::Menu.SetItemInfo(MENU_CONSOLE_FONTSIZE_8X16,CMenu::Unchecked);
       break;
  case MENU_CONSOLE_FONTSIZE_8X16:
       CCon::GetCon()->ChangeFontSize(8,16);
       CSys::Menu.SetItemInfo(MENU_CONSOLE_FONTSIZE_8X8,CMenu::Unchecked);
       CSys::Menu.SetItemInfo(MENU_CONSOLE_FONTSIZE_8X16,CMenu::Checked);
       break;
       
  case MENU_GRAPH_DMODE1:
       dmode--;
  case MENU_GRAPH_DMODE2:
       dmode--;
  case MENU_GRAPH_DMODE3:
       dmode--;
  case MENU_GRAPH_DMODE4:
       dmode--;
       CSys::SimulateKey(K_LSHF);
       CSys::SimulateKey(K_CTRL);
       CSys::SimulateKey(K_1+dmode);
       break;
       
  case MENU_HELP_HELP:
       CSys::SimulateKey(K_F1);
       break;
  case MENU_HELP_ABOUTTEXT:
       MessageBox(0,"EXPR for WIN32 by Kamil Korolkiewicz 2008,2009\n\
http://kk.plenty.vbiz.pl","About",MB_ICONINFORMATION);
       break;
  case MENU_HELP_SYSINFO:
       CCon::GetCon()->Put(CSys::SysInfoString());
       break;
  default:
       CSys::SimulateKey((w>>8)&0x7f);
       if (w>>15) CSys::SimulateKey(K_LSHF);
 }
}

void WinSizeChangeFunc(int w,int h,int a)
{
 static int _w,_h;
 if (CSys::Windowed==0) return;
 if (a==0)
 {
  _w=w;
  _h=h;
 }
 else
 {
  _w/=2;
  _w*=2;
  _h/=2;
  _h*=2;
  if ((_w!=CSys::ScrWidth)||(_h!=CSys::ScrHeight)) CSys::VidMode(_w,_h);
 }
}

int CSys::OpenDialog(char *title,char *filter,char *fn,int bufsize)
{
 static OPENFILENAME ofn;
 fn[0]=0;
 memset(&ofn,0,sizeof(ofn));
 ofn.lStructSize=sizeof(ofn);
 ofn.hwndOwner=wingraph_wnd;
 ofn.Flags=OFN_NOCHANGEDIR;
 ofn.lpstrFilter=filter;
 ofn.nFilterIndex=1;
 ofn.lpstrTitle=title;
 ofn.lpstrFile=fn;
 ofn.nMaxFile=bufsize-1;
 return GetOpenFileName(&ofn);
}

void CSys::InitMenu()
{
 int i;
 char bf[64];
 char *s;
 //dodaj elementy podstawowe menu
 //MAIN
 Menu.AddItem(0,"&File",MENU_FILE);
 Menu.AddItem(0,"&Console",MENU_CONSOLE);
 Menu.AddItem(0,"&Graph",MENU_GRAPH);
 Menu.AddItem(0,"&Sound",MENU_SOUND);
 Menu.AddItem(0,"&Help",MENU_HELP);
 
 //FILE
 Menu.AddItem((int)"&File","&Window",MENU_FILE_WINDOW);
 Menu.AddItem(MENU_FILE_WINDOW,"&Console\tF4",MENU_FILE_WINDOW_CON);
 Menu.AddItem(MENU_FILE_WINDOW,"&Graph\tF5",MENU_FILE_WINDOW_GRAPH);
 Menu.AddItem(MENU_FILE_WINDOW,"&Sound\tF6",MENU_FILE_WINDOW_SOUND);
 Menu.AddItem(MENU_FILE_WINDOW,"&Help\tF1",MENU_FILE_WINDOW_HELP);
 Menu.AddItem(MENU_FILE,"&Color",MENU_FILE_DCOLOR);
 for (i=0; i<8; i++)
 {
  int c=CGraph::Color[i];
  unsigned char r=c>>16;
  unsigned char g=c>>8;
  unsigned char b=c;
  sprintf(bf,"%d. RGB(%d,%d,%d)\tCtrl+%d",i+1,r,g,b,i+1);
  s=new char[strlen(bf)+1];
  strcpy(s,bf);
  Menu.AddItem(MENU_FILE_DCOLOR,s,MENU_FILE_DCOLOR1+i);
 }
 Menu.AddItem(MENU_FILE,"&Video mode",MENU_FILE_VIDMODE);
 for (i=0; i<8; i++)
 {
  sprintf(bf,"%dX%d\tCtrl+F%d",VidModes[i][0],VidModes[i][1],i+1);
  s=new char[strlen(bf)+1];
  strcpy(s,bf);
  Menu.AddItem(MENU_FILE_VIDMODE,s,MENU_FILE_VIDMODE_F1+i);
 }
 Menu.AddItem(MENU_FILE_VIDMODE,"---",0);
 Menu.AddItem(MENU_FILE_VIDMODE,"Windowed\tCtrl+W",MENU_FILE_VIDMODE_WINDOWED);
 Menu.AddItem((int)"&File","Print &screen\tF9",MENU_FILE_PSCR);
 Menu.AddItem((int)"&File","Print screen (&negative)\tShift+F9",MENU_FILE_PSCRNEG);
 Menu.AddItem((int)"&File","---",0);
 Menu.AddItem((int)"&File","Load &texture...",MENU_FILE_LOADTEX);
 Menu.AddItem((int)"&File","Load &plugin...",MENU_FILE_LOADDLL);
 Menu.AddItem((int)"&File","Load &font...",MENU_FILE_LOADFONT);
 Menu.AddItem((int)"&File","---",0);
 Menu.AddItem((int)"&File","&Exit\tF8",MENU_FILE_EXIT);
 
 //Console
 Menu.AddItem(MENU_CONSOLE,"&Clear",MENU_CONSOLE_CLEAR);
 Menu.AddItem(MENU_CONSOLE,"&Put path...",MENU_CONSOLE_PUTPATH);
 Menu.AddItem(MENU_CONSOLE,"---",0);
 Menu.AddItem(MENU_CONSOLE,"&Show list of functions\tTAB",MENU_CONSOLE_FLIST);
 Menu.AddItem(MENU_CONSOLE,"---",0);
 Menu.AddItem(MENU_CONSOLE,"&Font size",MENU_CONSOLE_FONTSIZE);
 Menu.AddItem(MENU_CONSOLE_FONTSIZE,"&8X8",MENU_CONSOLE_FONTSIZE_8X8);
 Menu.AddItem(MENU_CONSOLE_FONTSIZE,"8X1&6",MENU_CONSOLE_FONTSIZE_8X16);
 
 //GRAPH
 Menu.AddItem(MENU_GRAPH,"Drawing mode",MENU_GRAPH_DMODE);
 Menu.AddItem(MENU_GRAPH_DMODE,"2D y=f(x)\tCtrl+Shift+1",MENU_GRAPH_DMODE1);
 Menu.AddItem(MENU_GRAPH_DMODE,"2D color=f(x,y)\tCtrl+Shift+2",MENU_GRAPH_DMODE2);
 Menu.AddItem(MENU_GRAPH_DMODE,"3D z=f(x,y)\tCtrl+Shift+3",MENU_GRAPH_DMODE3);
 Menu.AddItem(MENU_GRAPH_DMODE,"2D x=x(t), y=y(t)\tCtrl+Shift+4",MENU_GRAPH_DMODE4);
 
 //HELP
 Menu.AddItem(MENU_HELP,"&Show readme.txt\tF1",MENU_HELP_HELP);
 Menu.AddItem(MENU_HELP,"---",0);
 Menu.AddItem(MENU_HELP,"&About",MENU_HELP_ABOUTTEXT);
 Menu.AddItem(MENU_HELP,"System &info",MENU_HELP_SYSINFO);
 
 Menu.SetWin32(wingraph_wnd);
 WinCommandFunc=MenuCommandFunc;
 WinSizeFunc=WinSizeChangeFunc;
 
 //domyslne ustawienia menu
 Menu.SetItemInfo(MENU_FILE_WINDOW_CON,CMenu::Checked);
 Menu.SetItemInfo(MENU_FILE_VIDMODE_F2,CMenu::Checked);
 Menu.SetItemInfo(MENU_FILE_VIDMODE_WINDOWED,CMenu::Checked);
 Menu.SetItemInfo(MENU_CONSOLE_FONTSIZE_8X8,CMenu::Checked);
 Menu.SetItemInfo(MENU_GRAPH_DMODE1,CMenu::Checked);
 Menu.SetItemInfo(MENU_FILE_DCOLOR1,CMenu::Checked);
}
#endif

void CSys::SetCur()
{
 static int c=1;
 c=1-c;
 Cursor(c);
}

void CSys::Init()
{
 AddVar("scrwidth",&CSys::ScrWidth,VAR_DWORD);
 AddVar("scrheight",&CSys::ScrHeight,VAR_DWORD);
 AddVar("color",&CSys::Color,VAR_RGB);
 AddVar("windowed",&CSys::Windowed,VAR_DWORD);
 AddVar("log",&CSys::logmode,VAR_DWORD);
 ExecCfg(0);
 DelVar("scrwidth");
 DelVar("scrheight");
 DelVar("windowed");
 if (!InitWinGraph()) 
 {
  MessageBox(0,"Could not init window","ERROR!",MB_ICONERROR); 
  exit(-1);
 }

#ifdef MENU
 InitMenu();
#endif
 
 VidMode(ScrWidth,ScrHeight);
 
 //Cursor(0);
 SetWindowText(wingraph_wnd,TITLETEXT);
 SetClassLong(wingraph_wnd,GCL_HICON,(LONG)LoadIcon(wingraph_hinst,MAKEINTRESOURCE(100)));
 SetClassLong(wingraph_wnd,GCL_HCURSOR,(LONG)LoadCursor(0,IDC_CROSS));
 SRand0=(int)(GetTime()*10000);
 srand(SRand0);
}

void CSys::Close()
{
#ifdef OPENGL
 COpenGL::Close();
#endif
 CloseApp();
 for (int i=0; i<DLibrariesC; i++) FreeLibrary((HINSTANCE)DLibraries[i]);
}

void CSys::Run()
{
 while (1)
 {
  if (!WinControl()) break;
  Shell(0);
  
  int md=GetMouseWheelDelta();
  if (md>0) SimulateKey(K_PGUP);
  if (md<0) SimulateKey(K_PGDOWN);
  if (MouseKeyPressed(WINGRAPH_MMID)) {wingraph_mousekeystate[1]=0; SetCur();}
  
  for (int i=0; i<256; i++) wingraph_keytab[i]|=simulatedkeytab[i];
  int shift=0,ctrl=0;
  if (KeyPressed(K_LSHF)) shift=256;
  if (KeyPressed(K_CTRL)) ctrl=65536;
  if (ctrl==0)
  {
   if (GetKey(K_F8)) break;
   if (GetKey(K_F9))
   {
    ((CWin*)windows[activewin])->PrintScreen(shift>0);
   }
   if (KeyPressed(K_F1)) 
   {
    //help
    ((CWin*)windows[activewin])->ChangeActiveState(0);
    activewin=Win_Help; 
    ((CWin*)windows[activewin])->ChangeActiveState(1);
    PresentWait=0;
#ifdef MENU
    Menu.SetItemInfo(MENU_FILE_WINDOW_CON,CMenu::Unchecked);
    Menu.SetItemInfo(MENU_FILE_WINDOW_GRAPH,CMenu::Unchecked);
    Menu.SetItemInfo(MENU_FILE_WINDOW_SOUND,CMenu::Unchecked);
    Menu.SetItemInfo(MENU_FILE_WINDOW_HELP,CMenu::Checked);
#endif
   }
   else
   if (KeyPressed(K_F4)) 
   {
    //con
    ((CWin*)windows[activewin])->ChangeActiveState(0);
    activewin=Win_Con;
    ((CWin*)windows[activewin])->ChangeActiveState(1);
    PresentWait=0;
#ifdef MENU
    Menu.SetItemInfo(MENU_FILE_WINDOW_CON,CMenu::Checked);
    Menu.SetItemInfo(MENU_FILE_WINDOW_GRAPH,CMenu::Unchecked);
    Menu.SetItemInfo(MENU_FILE_WINDOW_SOUND,CMenu::Unchecked);
    Menu.SetItemInfo(MENU_FILE_WINDOW_HELP,CMenu::Unchecked);
#endif
   }
   else
   if (KeyPressed(K_F5))
   {
    //graph
    ((CWin*)windows[activewin])->ChangeActiveState(0);
    activewin=Win_Graph;
    ((CWin*)windows[activewin])->ChangeActiveState(1);
    PresentWait=0;
#ifdef MENU
    Menu.SetItemInfo(MENU_FILE_WINDOW_CON,CMenu::Unchecked);
    Menu.SetItemInfo(MENU_FILE_WINDOW_GRAPH,CMenu::Checked);
    Menu.SetItemInfo(MENU_FILE_WINDOW_SOUND,CMenu::Unchecked);
    Menu.SetItemInfo(MENU_FILE_WINDOW_HELP,CMenu::Unchecked);
#endif
   }
   else
   if (KeyPressed(K_F6))
   {
    //sound
    ((CWin*)windows[activewin])->ChangeActiveState(0);
    activewin=Win_Sound;
    ((CWin*)windows[activewin])->ChangeActiveState(1);
    PresentWait=0;
#ifdef MENU
    Menu.SetItemInfo(MENU_FILE_WINDOW_CON,CMenu::Unchecked);
    Menu.SetItemInfo(MENU_FILE_WINDOW_GRAPH,CMenu::Unchecked);
    Menu.SetItemInfo(MENU_FILE_WINDOW_SOUND,CMenu::Checked);
    Menu.SetItemInfo(MENU_FILE_WINDOW_HELP,CMenu::Unchecked);
#endif
   }
   
   for (int i=0; i<256; i++)
   {
    if (KeyPressed(i)) ((CWin*)windows[activewin])->KeyFunc(i|shift);
   }
   if (MouseKey()) 
   {
    ((CWin*)windows[activewin])->KeyFunc(shift);
    if (activewin==Win_Con) {wingraph_mousekeystate[0]=0; wingraph_mousekeystate[2]=0;}
   }
  }
  else
  {
   if (shift==0)
   {
    for (int i=2; i<MAXFUNCCOUNT+2; i++) 
    if (GetKey(i)) 
    {
     SetDColor(i-2);
    }
   }
   else
   for (int i=2; i<CGraph::MMAX+2; i++) 
   if (GetKey(i)) 
   {
    SetDMode(i-2);
   }
   for (int i=K_F1; i<=K_F8; i++)
   {
    if (GetKey(i))
    {
     VidMode(VidModes[i-K_F1][0],VidModes[i-K_F1][1]);
    }
   }
   if (GetKey(K_F9)) //rozdzielczosc systemowa
   {
    int w,h;
    GetSysRes(w,h);
    VidMode(w,h);
   }
   if (GetKey(K_W))
   {
    Windowed=1-Windowed;
#ifdef MENU
    if (!Windowed) 
    Menu.SetItemInfo(MENU_FILE_VIDMODE_WINDOWED,CMenu::Unchecked);
    else
    Menu.SetItemInfo(MENU_FILE_VIDMODE_WINDOWED,CMenu::Checked);
#endif
    VidMode(ScrWidth,ScrHeight);
   }
  }
  shift=wingraph_keytab[K_LSHF];
  ctrl=wingraph_keytab[K_CTRL];
  memset(wingraph_keytab,0,256);
  wingraph_keytab[K_LSHF]=shift;
  wingraph_keytab[K_CTRL]=ctrl;
  for (int i=0; i<256; i++) 
  {wingraph_keytab[i]&=~simulatedkeytab[i]; simulatedkeytab[i]=0;}
  
#ifdef MENU
  for (int j=0; j<8; j++)
  if (j!=DColor) 
  Menu.SetItemInfo(MENU_FILE_DCOLOR1+j,CMenu::Unchecked);
  else
  Menu.SetItemInfo(MENU_FILE_DCOLOR1+j,CMenu::Checked);

  for (int j=0; j<4; j++)
  if (j!=CGraph::DMode)
  Menu.SetItemInfo(MENU_GRAPH_DMODE1+j,CMenu::Unchecked);
  else
  Menu.SetItemInfo(MENU_GRAPH_DMODE1+j,CMenu::Checked);
#endif

  
  if (((CWin*)windows[activewin])->GetBuf()!=buf) ((CWin*)windows[activewin])->Change(buf);
  ((CWin*)windows[activewin])->Process();
  if (PresentWait==0) PresentBuf();
 }
 for (int i=0; i<MAXWIN; i++) ((CWin*)windows[i])->ChangeActiveState(0);
}

float CSys::GetTime()
{
 long long f,t;
 QueryPerformanceFrequency((PLARGE_INTEGER)&f);
 QueryPerformanceCounter((PLARGE_INTEGER)&t);
 return (float)t/(float)f;
}

int CSys::logmode=0; //0-bez zapisu, 1-zapis
void CSys::Log_Printf(char *s,...)
{
 if (logmode==0) return;
 if ((int)logfile==-1) logfile=FOpen("log.txt","wt");
 else logfile=FOpen("log.txt","at");
 if (!logfile) return;
 va_list args;
 char buf[2048];
 va_start(args,s);
 vsprintf(buf,s,args);
 va_end(args);
 fprintf(logfile,buf);
 fprintf(logfile,"\n");
 fclose(logfile);
}

void CSys::CursorPos(int &x,int &y)
{
 GetCurPos(x,y);
}

void CSys::CursorPosSet(int x,int y)
{
 SetCurPos(x,y);
}

void CSys::VidMode(int w,int h)
{
 if ((w<32)||(h<32)) return;
 memset(wingraph_keytab,0,256);

 if (SetVidMode(w,h,ScrBpp,OVERLAPPED|VSYNC|((Windowed==1)? WINGRAPH_WINDOWED:0))==0)
 {
  if (SetVidMode(ScrWidth,ScrHeight,ScrBpp,
  OVERLAPPED|VSYNC|((Windowed==1)? WINGRAPH_WINDOWED:0))==0)
  {
   if (SetVidMode(640,480,32,0)==0)  //standardowy tryb obslugiwany (prawie) zawsze
   {
    Error("Could not set video mode");
   }
   else
   {
    ScrWidth=640;
    ScrHeight=480;
    ScrBpp=32;
    Windowed=0;
   }
  }
 }
 else
 {
  ScrWidth=w;
  ScrHeight=h;
 }
#ifdef OPENGL
 COpenGL::Init(ScrWidth,ScrHeight);
#endif
 //PresentBuf();
 //Cursor(0);
 SetWindowText(wingraph_wnd,TITLETEXT);
 SetClassLong(wingraph_wnd,GCL_HICON,(LONG)LoadIcon(wingraph_hinst,MAKEINTRESOURCE(100)));
 for (int j=0; j<MAXWIN; j++) if ((CWin*)windows[j])
 ((CWin*)windows[j])->Change(ScrWidth,ScrHeight,buf);
#ifdef MENU
 for (int j=0; j<8; j++)
 if ((VidModes[j][0]==ScrWidth)&&(VidModes[j][1]==ScrHeight))
 CSys::Menu.SetItemInfo(MENU_FILE_VIDMODE_F1+j,CMenu::Checked);
 else
 CSys::Menu.SetItemInfo(MENU_FILE_VIDMODE_F1+j,CMenu::Unchecked);
#endif
}

void CSys::GetSysRes(int &w,int &h)
{
 w=GetSystemMetrics(SM_CXSCREEN);
 h=GetSystemMetrics(SM_CYSCREEN);
}

char *CSys::GetClipboardText()
{
 OpenClipboard(wingraph_wnd);
 char *r=(char*)GetClipboardData(CF_TEXT); 
 CloseClipboard(); 
 return r;
}

void CSys::Error(char *info,int mode)
{
 MessageBox(0,info,"ERROR!",MB_ICONERROR);
 if (mode) return;
#ifdef OPENGL
 COpenGL::Close();
#endif
 CloseApp();
 exit(-1);
}

int CSys::DLibLoad(char *dlibname)
{
 int dlib=0;
 if (DLibrariesC==MAXLIB) return 0;
 for (int i=0; i<DLibrariesC; i++) if (strcmp(dlibname,DLibrariesNames[i])==0) 
      {dlib=DLibraries[i]; break;}
 if (dlib==0)
 {
  dlib=(int)LoadLibrary(dlibname);
  if (dlib==0) return 0;
  DLibraries[DLibrariesC]=dlib;
  strcpy(DLibrariesNames[DLibrariesC],dlibname);
  DLibrariesC++;
 }
 int (*reffunc)(void*,void*)=0;
 reffunc=(int(*)(void*,void*))GetProcAddress((HINSTANCE)dlib,"RefFunc");
 if (reffunc==0) return 0;
 CLib *l=CExpr::GetLib();
 void *list;
 ENV env;
 env.CSys_GetTime=CSys::GetTime;
 env.CSys_FOpen=CSys::FOpen;
 env.MultiExec=&CExpr::MultiExec;
 env.GraphState=&CGraph::GraphState;
 env.expr_conprint=expr_conprint;
 int n=reffunc(&list,&env);
 if (n)
 l->AddList(list,n);
 else return 0;
 return 1;
}

void *CSys::AllocMem(int size)
{
 void *r=GlobalAlloc(GMEM_FIXED,size);
 if (r==0) 
 {
  char bf[128];
  sprintf(bf,"%dK allocation fault (%dK available)",size/1024,MemAvail()/1024);
  Error(bf);
 }
 return r;
}

void *CSys::ReallocMem(void *ptr,int prevsize,int size)
{
 if (!ptr) return 0;
 int s=(prevsize>size)? size:prevsize;
 void *pom=AllocMem(s);
 memcpy(pom,ptr,s);
 FreeMem(ptr);
 ptr=AllocMem(size);
 memcpy(ptr,pom,s);
 FreeMem(pom);
 return ptr;
}

void CSys::FreeMem(void *ptr)
{
 if (ptr)
 GlobalFree(ptr);
}

//>2GB support
typedef struct _MEMORYSTATUSEX {
  DWORD dwLength;
  DWORD dwMemoryLoad;
  DWORDLONG ullTotalPhys;
  DWORDLONG ullAvailPhys;
  DWORDLONG ullTotalPageFile;
  DWORDLONG ullAvailPageFile;
  DWORDLONG ullTotalVirtual;
  DWORDLONG ullAvailVirtual;
  DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX;

BOOL WINAPI (*GlobalMemoryStatusEx)(MEMORYSTATUSEX *)=0;

unsigned int CSys::MemAvail()
{
 /*MEMORYSTATUS ms;
 ms.dwLength=sizeof(MEMORYSTATUS);
 GlobalMemoryStatus(&ms);
 return ms.dwAvailPhys;
 */
 if (GlobalMemoryStatusEx==0) 
 {
  HINSTANCE l=LoadLibrary("kernel32.dll");
  GlobalMemoryStatusEx=(BOOL WINAPI (*)(MEMORYSTATUSEX *))
  GetProcAddress(l,"GlobalMemoryStatusEx");
  if (GlobalMemoryStatusEx==0)
  {
   MEMORYSTATUS ms;
   ms.dwLength=sizeof(MEMORYSTATUS);
   GlobalMemoryStatus(&ms);
   return ms.dwAvailPhys;
  }
 }
 MEMORYSTATUSEX ms;
 ms.dwLength=sizeof(MEMORYSTATUSEX);
 GlobalMemoryStatusEx(&ms);
 return ms.ullAvailPhys;
}

unsigned int CSys::MemTotal()
{
 /*MEMORYSTATUS ms;
 ms.dwLength=sizeof(MEMORYSTATUS);
 GlobalMemoryStatus(&ms);
 return ms.dwTotalPhys;*/
 if (GlobalMemoryStatusEx==0) 
 {
  HINSTANCE l=LoadLibrary("kernel32.dll");
  GlobalMemoryStatusEx=(BOOL WINAPI (*)(MEMORYSTATUSEX *))
  GetProcAddress(l,"GlobalMemoryStatusEx");
  if (GlobalMemoryStatusEx==0)
  {
   MEMORYSTATUS ms;
   ms.dwLength=sizeof(MEMORYSTATUS);
   GlobalMemoryStatus(&ms);
   return ms.dwAvailPhys;
  }
 }
 MEMORYSTATUSEX ms;
 ms.dwLength=sizeof(MEMORYSTATUSEX);
 GlobalMemoryStatusEx(&ms);
 return ms.ullTotalPhys;
}

int CSys::SetSound()
{
 if (SoundOn) return 1;
 if (!InitWinSound(wingraph_wnd)) return 0;
 if (!SetSoundBuffer(SoundFreq,SoundBPS,SoundChannels)) {CloseWinSound(); return 0;}
 SoundOn=1;
 GetWSoundPos(SOUND_MIXER_SECTIONS,&SoundMixerBufSize);
 return 1;
}

void CSys::StopSound()
{
 CloseWinSound();
 CloseWinSoundInput();
 SoundOn=0;
}

int CSys::IsMixerAvailable()
{
 int t=GetWSoundPos(SOUND_MIXER_SECTIONS,0);
 if (t!=-1) 
 {
  CSys::SoundMixerBufSize=t;
  return 1; 
 }
 else return 0;
}

void CSys::MixSounds(SOUNDCHANNEL *sc,int n)
{
 int pos=GetWSoundPos(SOUND_MIXER_SECTIONS,&SoundMixerBufSize);
 if (pos!=-1)
 {
  BeginSndBufAccess();
  if (sndbuf)
  {
   MixSoundChannels(sc,n,SoundBPS,SoundChannels,(void*)((int)sndbuf+pos),SoundMixerBufSize);
  }
  EndSndBufAccess();
 }
 else return;
}

void CSys::RecordInputSound()
{
 if (SoundOn==0) return;
 if (SoundOn!=2)
 {
  CloseWinSoundInput();
  if (!InitWinSoundInput()) return;
  if (!SetInputSoundBuffer(SoundFreq,SoundBPS,SoundChannels)) return;
  SoundOn=2;
 }
 if (RecBuf==0)
 {
  RecBuf=AllocMem(RECORDINPUTSOUNDBUFLEN);
  memset(RecBuf,0,RECORDINPUTSOUNDBUFLEN);
 }
 int s;
 int pos=GetInputSoundPosDiff(&s);
 if (s!=0)
 {
  BeginInputSndBufAccess();
  if (!isndbuf) return;
  //DEBUG_Printf("%d %d %d\n",s,pos,RecBufPos);
  if (RecBufPos+s>RECORDINPUTSOUNDBUFLEN) RecBufPos=0;
  if (pos+s>=isndbufsize)
  {
   int s2=s;
   s=isndbufsize-pos;
   s2-=s;
   //DEBUG_Printf("%d\n",s2);
   RecBufPos+=s2;
   memcpy((void*)((int)RecBuf+RecBufPos+s),(void*)((int)isndbuf),s2);
  }
  memcpy((void*)((int)RecBuf+RecBufPos),(void*)((int)isndbuf+pos),s);
  RecBufPos+=s;
  EndInputSndBufAccess();  
 }
}

void *CSys::FlushInputSoundBuf()
{
 static int lastpos=0;
 static int precpos=0;
 //DEBUG_Printf("%d\n",lastpos);
 if (lastpos+SoundMixerBufSize>=RECORDINPUTSOUNDBUFLEN) lastpos=0;
 void *r=(void*)((int)RecBuf+lastpos);
 if (RecBufPos-precpos<SoundMixerBufSize)
 lastpos+=RecBufPos-precpos;
 else
 lastpos+=SoundMixerBufSize;
 precpos=RecBufPos;
 if (lastpos<0) lastpos=0;
 return r;
}

char *CSys::SysInfoString()
{
 static char inf[1024*8];
 char buf[1024*8],buf2[1024];
 int s=1024;
 OSVERSIONINFO osvinf;
 SYSTEM_INFO sysinf;
 strcpy(inf,"\02---EXPR for win32---\n");
 GetComputerName(buf2,(DWORD*)&s);
 strcat(inf,"\05OS INFO\n");
 strcpy(buf,"Computer name: ");
 s=strlen(buf);
 buf[s]=8;
 buf[s+1]=0;
 strcat(buf,buf2);
 strcat(buf,"\n");
 strcat(inf,buf);
 osvinf.dwOSVersionInfoSize=sizeof(osvinf);
 GetVersionEx(&osvinf);
 sprintf(buf,"Windows version %d.%d (build %d) ",
 (int)osvinf.dwMajorVersion,(int)osvinf.dwMinorVersion,
 (int)osvinf.dwBuildNumber);
 s=strlen(buf);
 buf[s]=8;
 buf[s+1]=0;
 strcat(buf,osvinf.szCSDVersion);
 strcat(buf,"\n");
 strcat(inf,buf);
 GetSystemInfo(&sysinf);
 sprintf(buf,"Num. of processors: %d\n",(int)sysinf.dwNumberOfProcessors);
 strcat(inf,buf);
 sprintf(buf,"Mem avail: %dK\nTotal mem: %dK\n",MemAvail()/1024,MemTotal()/1024);
 strcat(inf,buf);
 sprintf(buf,"Width: %d\nHeight: %d\n",ScrWidth,ScrHeight);
 strcat(inf,buf);
#ifdef OPENGL
 strcat(inf,"\05OPENGL INFO\n");
 sprintf(buf,"GL_VENDOR: %s\nGL_RENDERER: %s\nGL_VERSION: %s\n",
 glGetString(GL_VENDOR),glGetString(GL_RENDERER),glGetString(GL_VERSION));
 strcat(inf,buf);
 /*strcpy(buf,(char*)glGetString(GL_EXTENSIONS));
 char *b=buf;
 while (*b) {if (*b==' ') *b='\n'; b++;}
 strcat(inf,buf);*/
#endif
#ifdef X86
 strcat(inf,"\05CPU SPECIFIC INFO\n");
 int b,c,d;
 asm volatile ("\
 xor %%eax,%%eax;\
 cpuid;\
 ":"=b"(b),"=c"(c),"=d"(d)::"%eax");
 buf2[0]=8;
 _MEM(buf2+1,0,int)=b;
 _MEM(buf2+1,1,int)=d;
 _MEM(buf2+1,2,int)=c;
 buf2[4*3+1]=0;
 sprintf(buf,"CPU vendor id: %s\n",buf2);
 strcat(inf,buf);
 asm volatile ("\
 mov $1,%%eax;\
 cpuid;\
 ":"=c"(c),"=d"(d)::"%eax","%ebx");
 int mmx,sse,sse2,sse3,ia64;
 char *present[2]={"Not present","Present"};
 mmx=d&(1<<23)>>23;
 sse=d&(1<<25)>>25;
 sse2=d&(1<<26)>>26;
 sse3=c&1;
 ia64=d&(1<<30)>>30;
 sprintf(buf,"MMX: %s\nSSE: %s\nSSE2: %s\nSSE3: %s\nIA-64: %s\n",present[mmx],present[sse],
 present[sse2],present[sse3],present[ia64]);
 strcat(inf,buf);
 
 long long f,t,t2;
 QueryPerformanceFrequency((PLARGE_INTEGER)&f);
 QueryPerformanceCounter((PLARGE_INTEGER)&t);
 do
 {
  QueryPerformanceCounter((PLARGE_INTEGER)&t2);
 } while (t==t2);
 t=t2;
 int a1,a2;
 asm volatile ("rdtsc;":"=a"(a1)::"%edx");
 /*do
 {
  QueryPerformanceCounter((PLARGE_INTEGER)&t2);
 } while (t2-t<1000000);
 asm volatile ("rdtsc;":"=a"(a2)::"%edx");
 float cpufreq=(float)(a2-a1)*(float)f/1000000.f;*/
 do
 {
  QueryPerformanceCounter((PLARGE_INTEGER)&t2);
 } while ((float)(t2-t)/(float)f<0.1f);
 asm volatile ("rdtsc;":"=a"(a2)::"%edx");
 float cpufreq=(float)(a2-a1)/0.1f;
 sprintf(buf,"CPU freq: %fMHZ\n",cpufreq/1000000.f);
 strcat(inf,buf);
#endif
 return inf;
}

void CSys::SimulateKey(int k) 
{
 //wingraph_keytab[k]=1;
 simulatedkeytab[k]=1;
}

int CSys::Shell(char *cl)
{
 static HANDLE prdout=0;
 static HANDLE pwrout=0;
 static int mode=0;
 PROCESS_INFORMATION pi;
 char bf[256*2];
 char sysdir[256];
 static unsigned char *wsk;
 static int i;
 static unsigned char rbuf[BUFLEN];
 static unsigned char obuf[BUFLEN];
 
 if (cl!=0)
 {
 GetSystemDirectory(sysdir,256);
 sprintf(bf,"%s\\cmd.exe /Q /C %s",sysdir,cl);

 SECURITY_ATTRIBUTES sattr; 
 sattr.nLength=sizeof(SECURITY_ATTRIBUTES); 
 sattr.bInheritHandle=1; 
 sattr.lpSecurityDescriptor=0; 
 
 if (!CreatePipe(&prdout,&pwrout,&sattr,0)) return 0;
 SetHandleInformation(&pwrout,HANDLE_FLAG_INHERIT,0);
 
 STARTUPINFO si;
 memset(&pi,0,sizeof(PROCESS_INFORMATION));
 memset(&si,0,sizeof(STARTUPINFO));
 si.cb=sizeof(STARTUPINFO);
 si.hStdError=pwrout;
 si.hStdOutput=pwrout;
 si.hStdInput=GetStdHandle(STD_INPUT_HANDLE);
 si.dwFlags=STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
 si.wShowWindow=SW_SHOWMINNOACTIVE;
 if (!CreateProcess(0,bf,0,0,1,0,0,0,&si,&pi)) return 0;
 if (!CloseHandle(pwrout)) return 0;
 i=1;
 wsk=rbuf;
 }
 else if (mode==0) return 0;
 
 int rd,tot;
 obuf[0]=2;

 mode=1;
 int ok=PeekNamedPipe(prdout,rbuf,BUFLEN-1,(DWORD*)&rd,(DWORD*)&tot,0);
 if (tot) ReadFile(prdout,rbuf,BUFLEN-1,(DWORD*)&rd,0);
 if ((!ok)) mode=0;
 else
 {
  rbuf[rd]=0;
  while (1)
  {
   while ((*wsk>=32)&&(i<BUFLEN)) obuf[i++]=*wsk++;
   if (*wsk==0) {wsk=rbuf; break;}
   obuf[i]=0;
   CCon::GetCon()->Put((char*)obuf);
   i=1;
   while ((*wsk!=0)&&(*wsk<32)) wsk++;
   if (*wsk==0) {wsk=rbuf; break;}
  }
 }
 
 if (mode==0)
 {
  CloseHandle(prdout);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  prdout=0;
 }
 
 return 1;
}

#include "exprimg.h"
//laduje obraz z pliku do bufora imgbuf i zwraca jego szerokosc i wysokosc
int CSys::LoadImage(char *fn,unsigned int **imgbuf,int *width,int *height)
{
 FILE *f;
 char sign[2];
 unsigned short bpp;
 int imgstart,palstart;
 unsigned int BGR;
 unsigned int *wsk,*pwsk;
 unsigned int *pal=0;
 int w,h;
 if (fn==0)
 {
  *width=EXPRIMG_WIDTH;
  *height=EXPRIMG_HEIGHT;
  *imgbuf=EXPRIMG;
  return 1;
 }
 
 f=FOpen(fn,"rb");
 if (f==0) return 0;
 fread(&sign,1,2,f);
 if ((sign[0]!='B')&&(sign[1]!='M')) {fclose(f); return 0;}
 fseek(f,0xa,SEEK_SET);
 fread(&imgstart,1,4,f);
 fseek(f,0xe,SEEK_SET);
 fread(&palstart,1,4,f);
 fseek(f,0x1c,SEEK_SET);
 fread(&bpp,1,2,f);
 fseek(f,0x12,SEEK_SET);
 fread(&w,1,4,f);
 fread(&h,1,4,f);
 if (bpp==8) 
 {
  pal=(unsigned int*)AllocMem(4*256);
  fseek(f,palstart+14,SEEK_SET);
  unsigned char *cwsk;
  cwsk=(unsigned char*)pal;
  for (int i=0; i<256; i++)
  {
   fread(&BGR,1,4,f);
   *cwsk=(BGR)&255;
   *(cwsk+1)=(BGR>>8)&255;
   *(cwsk+2)=(BGR>>16)&255;
   *(cwsk+3)=255;
   cwsk+=4;
  }
 }
 else 
 if (bpp!=24) {fclose(f); return 0;}
 
 *width=w;
 *height=h;
 unsigned int *bf;
 bf=(unsigned int*)AllocMem(*width*(*height+1)*sizeof(unsigned int));
 
 fseek(f,imgstart,SEEK_SET);
 wsk=bf+(*width*(*height-1));
 int extrabits;
 if (bpp==24)
 {
  extrabits=4-((*width*3)%4);
  for (int i=0; i<*height; i++)
  {
   pwsk=wsk;
   for (int j=0; j<*width; j++)
   {
    fread(&BGR,1,3,f);
    *(unsigned char*)((int)&BGR+3)=255;
    *wsk=BGR;
    wsk++;
   }
   if (extrabits!=4) fread(&BGR,1,extrabits,f);
   wsk=pwsk-*width;
  }
 }
 else
 if (bpp==8)
 {
  extrabits=4-((*width)%4);
  for (int i=0; i<*height; i++)
  {
   pwsk=wsk;
   for (int j=0; j<*width; j++)
   {
    fread(&BGR,1,1,f);
    *wsk=pal[BGR&255];
    wsk++;
   }
   if (extrabits!=4) fread(&BGR,1,extrabits,f);
   wsk=pwsk-*width;
  }
  FreeMem(pal);
 }
 fclose(f);
 
 *imgbuf=bf;
 return 1;
}

//funckcje niezalezne od systemu

void CSys::SetDMode(int m)
{
 if (m==CGraph::K2DF1)
 {
  CCon::GetCon()->SetPrompt("2DF1>");
 }
 else
 if (m==CGraph::K2DF2)
 {
  CCon::GetCon()->SetPrompt("2DF2>");
 }
 else
 if (m==CGraph::K3DF2)
 {
  CCon::GetCon()->SetPrompt("3DF2>");
 }
 else
 if (m==CGraph::K2DXY)
 {
  CCon::GetCon()->SetPrompt("2DXY>");
 }
 CGraph::GetGraph()->ChangeMode(m);
}

void CSys::SetDColor(int c)
{
 DColor=c%MAXFUNCCOUNT;
}

unsigned int inline CSys::FadeColor(unsigned int c,unsigned char fade)
{
 /*unsigned int r=(((c>>16)&255)*fade)>>8;
 unsigned int g=(((c>>8)&255)*fade)>>8;
 unsigned int b=((c&255)*fade)>>8;
 return RGB32(r,g,b);*/
 //return RGB32((((c>>16)&255)*fade)>>8,(((c>>8)&255)*fade)>>8,((c&255)*fade)>>8);
 return (((((c>>16)&255)*fade)<<8)&0xff0000)|((((c>>8)&255)*fade)&0xff00)|
 ((((c&255)*fade)>>8)&0xff);
}

int CSys::DiscardZeros(char *bf)
{
 int i=strlen(bf)-1;
 while ((i>0)&&(bf[i]=='0')) i--;
 if (bf[i]=='.') i--;
 bf[i+1]=0;
 return i;
}

void inline CSys::memset32(void *p,int a,unsigned int b) 
{
#ifdef X86
 asm volatile ("\
     mov %0,%%edi;\
     mov %2,%%ecx;\
     cld; rep stosl;\
     "::"r"(p),"a"(a),"r"(b):"%ecx","%edi");
#else
 while (b--)
 {
  *(int*)p=a;
  p=(void*)((int)p+4);
 }
#endif
}

void CSys::AddVar(char *name,void *addr,int flags)
{
 ELEMENT e;
 memset(&e,0,sizeof(ELEMENT));
 e.name=name;
 e.fptr=addr;
 e.tag=flags;
 varlib.AddElement(&e);
}

void CSys::DelVar(char *name)
{
 varlib.DelElement(name);
}

int CSys::SetVarV(char *name,char *charval,int shift)
{
 NAME *n;
 n=varlib.Find(name);
 if (!n) return 0;
 while ((*charval<=32)&&(*charval!=0)) charval++;
 char c=*charval;
 if (!c) return 0;
 unsigned int val=0;
 if ((((c>='A')&&(c<='Z'))||((c>='a')&&(c<='z'))||(c=='_')))
 {
  NAME *n2=varlib.Find(charval);
  if (n2)
  {
   _MEM(&val,0,unsigned int)=_MEM(n2->fptr,0,unsigned int);
  }
  else 
  return 0;
 }
 else
 val=(unsigned int)atoi(charval);
 switch (n->tag&0xff)
 {
  case VAR_DWORD:
       _MEM(n->fptr,shift,unsigned int)=val;
       break;
  case VAR_FLOAT:
       _MEM(n->fptr,shift,float)=atof(charval);
       break;
  case VAR_BYTE:
       _MEM(n->fptr,shift,unsigned char)=_MEM(&val,0,unsigned char);
       break;
  case VAR_WORD:
       _MEM(n->fptr,shift,unsigned short)=_MEM(&val,0,unsigned short);
       break;
  case VAR_RGB:
       _MEM(n->fptr,2-shift%3+(shift/3)*4,unsigned char)=_MEM(&val,0,unsigned char);
       break;
  case VAR_STR:
       charval++;
       int sl=strlen(charval)-1;
       memcpy((char*)n->fptr,charval,sl);
       _MEM(n->fptr,sl,unsigned char)=0;
       break;
 }
 return 1;
}

int CSys::GetVarI(char *name,int shift)
{
 NAME *n;
 n=varlib.Find(name);
 if (!n) return 0;
 int val=0;
 switch (n->tag&0xff)
 {
  case VAR_DWORD:
       val=_MEM(n->fptr,shift,int);
       break;
  case VAR_FLOAT:
       val=(int)_MEM(n->fptr,shift,float);
       break;
  case VAR_BYTE:
       val=(int)_MEM(n->fptr,shift,char);
       break;
  case VAR_WORD:
       val=(int)_MEM(n->fptr,shift,short);
       break;
  case VAR_RGB:
       val=(int)_MEM(n->fptr,2-shift%3+(shift/3)*4,unsigned char);
       break;
  case VAR_STR:
       val=strlen((char*)n->fptr);
       break;
 }
 return val;
}

float CSys::GetVarF(char *name,int shift)
{
 NAME *n;
 n=varlib.Find(name);
 if (!n) return 0;
 float val=0;
 switch (n->tag&0xff)
 {
  case VAR_DWORD:
       val=(float)_MEM(n->fptr,shift,int);
       break;
  case VAR_FLOAT:
       val=_MEM(n->fptr,shift,float);
       break;
  case VAR_BYTE:
       val=(float)_MEM(n->fptr,shift,char);
       break;
  case VAR_WORD:
       val=(float)_MEM(n->fptr,shift,short);
       break;
  case VAR_RGB:
       val=(float)_MEM(n->fptr,2-shift%3+(shift/3)*4,unsigned char);
       break;
  case VAR_STR:
       val=(float)strlen((char*)n->fptr);
       break;
 }
 return val;
}

void CSys::ExecCfg(char *sec)
{
 char buf[256];
 char name[256];
 FILE *f;
 f=FOpen("config.cfg","rt");
 if (!f) return;
 char secname[256];
 secname[0]=0;
 int comment=0;
 while (1)
 {
rep:
  if (feof(f)) break;
  fgets(buf,256,f);
  
  int i=0;
  int j;
  int shift=0;
  i=strlen(buf)-1;
  while ((buf[i]<32)&&(i>0)) i--;
  while ((buf[i]==32)&&(i>0)) i--;
  buf[i+1]=0;
  i=0;
  while (buf[i]<=32) i++;
  if (comment) 
  {
   while ((buf[i]!='*')&&(buf[i]>=32)) i++;
   if (buf[i]<32) continue;
   i++;
   if (buf[i]=='/') {comment=0; i++;}
   else continue;
  }
  if ((buf[i]=='/')&&(buf[i+1]=='/')) continue;
  if ((buf[i]=='/')&&(buf[i+1]=='*')) {comment=1; continue;}
  if (buf[i]=='[')
  {
   j=0;
   i++;
   while ((buf[i])&&(buf[i]!=']'))
   {
    secname[j++]=buf[i++];
   }
   secname[j]=0;
   continue;
  }
  
  if (sec!=0)
  {
   if (strcmp(secname,sec)!=0) continue;
  }
  while ((buf[i]!='=')&&(buf[i]!='+'))
  {
   if (buf[i]<32) goto rep;
   i++;
  }
  char c=buf[i];
  buf[i]=0;
  strcpy(name,buf);
  i++;
  
  if (c=='+')
  {
   j=i;
   while (buf[i]!='=')
   {
    if (buf[i]<32) goto rep;
    i++;
   }
   buf[i]=0;
   shift=atoi(buf+j);
   i++;
  }
  j=i;
  
  while (buf[i]>=32)
  {
   if (buf[i]==',')
   {
    buf[i]=0;
    SetVarV(name,buf+j,shift);
    j=i+1;
    shift++;
   }
   i++;
  }
  buf[i]=0;
  SetVarV(name,buf+j,shift);
 }
}
