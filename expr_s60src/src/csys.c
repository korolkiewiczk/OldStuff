//csys.c
//Dla systemu s60 3rd edition

#include <sysext/str2desc.c>

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
 RGB32(230,50,50),
 RGB32(0,0,0)
};

//prywatne
void *CSys::windows[MAXWIN];
int CSys::activewin;
CLib CSys::varlib;

//dane i funkcje prywatne zalezne od systemu
int CSys::AutoVidMode=1;

int CSys::nokbstack[MAXNOKBSTACK],CSys::nokbstacki=0;
int CSys::multimappingon=0;
char *CSys::multimaptab[10]=
{
 ",'\"<>:|1[]",  "abc2",    "def3",
 "ghi4",         "jkl5",   "mno6",
 "pqrs7",        "tuv8",   "wxyz9",
                 "/=_+-*0 "
};

char *CSys::pathtab[MAXPATH]={"e:\\expr\\","c:\\expr\\"};
FILE *CSys::logfile=(FILE*)-1;
RLibrary CSys::DLibraries[MAXLIB];
int CSys::CursorPosX;
int CSys::CursorPosY;
int CSys::MouseKeyOn=0;
unsigned char CSys::simulatedkeytab[256];

//publiczne
int CSys::DColor=0;
int CSys::PresentWait=0;
int CSys::ScrWidth=SCRWIDTH,CSys::ScrHeight=SCRHEIGHT,CSys::ScrBpp=BPP;
int CSys::DLibrariesC=0;
char CSys::DLibrariesNames[MAXLIB][64];
int CSys::SoundOn=0;
int CSys::SoundFreq=8000,CSys::SoundBPS=16,CSys::SoundChannels=2;
int CSys::SoundMixerBufSize;
int CSys::SRand0;

void CSys::Init()
{
 AddVar("color",&Color,VAR_RGB);
 AddVar("log",&logmode,VAR_DWORD);
 ExecCfg(0);
 
#ifdef OPENGL
 COpenGL::Init(ScrWidth,ScrHeight);
#endif
 SRand0=(int)(GetTime()*10000);
 srand(SRand0);
 CursorPosSet(ScrWidth/2,ScrHeight/2);
 
#ifdef MENU
 InitMenu();
#endif
}

void CSys::Close()
{
 for (int i=0; i<MAXWIN; i++) ((CWin*)windows[i])->ChangeActiveState(0);
#ifdef OPENGL
 COpenGL::Close();
#endif
 for (int i=0; i<DLibrariesC; i++) DLibraries[i].Close();
}

//obsluga menu

#ifdef MENU

CMenu CSys::Menu;

void MenuCommandFunc(int w)
{
 char bf[1024];
 char bf2[256];
 TBuf<256> msg;
 TBuf<1> nullmsg;
 CEikonEnv *env=CEikonEnv::Static();
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
  	   str2desc("EXPR for SymbianOS by Kamil Korolkiewicz 2008,2009\n\
http://kk.plenty.vbiz.pl",msg);
	   str2desc("",nullmsg);
       env->InfoWinL(msg,nullmsg);
       break;
  case MENU_HELP_SYSINFO:
       CCon::GetCon()->Put(CSys::SysInfoString());
       break;
  default:
       CSys::SimulateKey((w>>8)&0x7f);
       if (w>>15) CSys::SimulateKey(K_LSHF);
 }
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
 Menu.AddItem(MENU_FILE,"&Window",MENU_FILE_WINDOW);
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
 
 Menu.AddItem(MENU_FILE,"Print &screen\tF9",MENU_FILE_PSCR);
 Menu.AddItem(MENU_FILE,"Print screen (&negative)\tShift+F9",MENU_FILE_PSCRNEG);
 Menu.AddItem(MENU_FILE,"---",0);
 Menu.AddItem(MENU_FILE,"Load &texture...",MENU_FILE_LOADTEX);
 //Menu.AddItem(MENU_FILE,"Load &plugin...",MENU_FILE_LOADDLL);
 Menu.AddItem(MENU_FILE,"Load &font...",MENU_FILE_LOADFONT);
 Menu.AddItem(MENU_FILE,"---",0);
 Menu.AddItem(MENU_FILE,"&Exit\tF8",MENU_FILE_EXIT);
 
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
 
 
 //domyslne ustawienia menu
 Menu.SetItemInfo(MENU_FILE_WINDOW_CON,CMenu::Checked);
 Menu.SetItemInfo(MENU_FILE_VIDMODE_F2,CMenu::Checked);
 Menu.SetItemInfo(MENU_FILE_VIDMODE_WINDOWED,CMenu::Checked);
 Menu.SetItemInfo(MENU_CONSOLE_FONTSIZE_8X8,CMenu::Checked);
 Menu.SetItemInfo(MENU_GRAPH_DMODE1,CMenu::Checked);
 Menu.SetItemInfo(MENU_FILE_DCOLOR1,CMenu::Checked);
}

int CSys::OpenDialog(char *title,char *filter,char *fn,int bufsize)
{
 TFileName filen;
 str2desc("",filen);
 fn[0]=0;
 if (AknCommonDialogs::RunSelectDlgLD(filen,R_MEMORY_SELECTION_DIALOG))
 {
  int i=0;
  int s=filen.Length();
  if (s>=bufsize) s=bufsize-1;
  for(i=0; i<s; i++)
  {
   fn[i]=filen[i];
  }
  fn[i]=0;
  return 1;
 }
 return 0;
}

#endif //MENU

int CSys::Run()
{
  
  if ((ScrWidth!=s60graph_scrwidth)||(ScrHeight!=s60graph_scrheight))
  {
   AutoVidMode=1;
   VidMode(s60graph_scrwidth,s60graph_scrheight);
  }
  AutoVidMode=0;
  
  
  //mapowanie klawiszy
  
  unsigned char keytab[256];
  memset(keytab,0,256);
  int ps=0;
  
  for (int i=0; i<256; i++) keytab[i]|=simulatedkeytab[i];
  
  int shift=0,ctrl=0;
  if (keytab[K_CTRL]) 
  {
   ctrl=65536;
   memcpy(s60graph_keytab,keytab,256);
   goto ctrlpressed;
  }
  
  //shift
  if (KeyPressed(18))
  {
   keytab[K_LSHF]=1;
   AddNoBlockedKey(18);
  }
  if (keytab[K_LSHF])
  shift=256;
  
  //dla konsoli
  if ((activewin==Win_Con)||(vkeyb->IsActive())
  )
  {
   for (int i=0; i<9; i++)
   {
    if (KeyPressed(i+49))
    {
     keytab[K_1+i]=1;
    }
   }
   if (KeyPressed(48))
   keytab[K_0]=1;
  }
  //dla trybu wykresu
  else
  if (activewin==Win_Graph)
  {
   for (int i=0; i<4; i++)
   {
    if (KeyPressed(i+49))
    {
     keytab[K_1+i]=1;
    }
   }
   
   if ((CGraph::DMode==CGraph::K2DF1)||(CGraph::DMode==CGraph::K2DF2))
   {
    //5
    if (KeyPressed(49+4)) keytab[K_R]=1;
    //7
   	if (KeyPressed(49+6)) keytab[K_C]=1;
   	//8
    if (KeyPressed(49+7)) keytab[K_HOME]=1;
    //*
    if (KeyPressed(42)) keytab[K_F]=1;
    //0
    if (KeyPressed(48)) keytab[K_0]=1;
    //#
    if (KeyPressed(127)) keytab[K_Q]=1;
   }
   else
   if (CGraph::GetGraph()->Is3DMode())
   {
    //5
    if (KeyPressed(49+4)) keytab[K_R]=1;
   	//7
   	if (KeyPressed(49+6)) keytab[K_L]=1;
   	//8
   	if (KeyPressed(49+7)) keytab[K_V]=1;
   	//*
    if (KeyPressed(42)) keytab[K_N]=1;
   	//0
    if (KeyPressed(48)) keytab[K_H]=1;
    //#
    if (KeyPressed(127)) keytab[K_M]=1;
   }
   else
   if (CGraph::DMode==CGraph::K2DXY)
   {
    //5
    if (KeyPressed(49+4)) keytab[K_RSBRACKET]=1;
   	//7
   	if (KeyPressed(49+6)) keytab[K_APOSTROPHE]=1;
   	//8
   	if (KeyPressed(49+7)) keytab[K_LSBRACKET]=1;
    //*
    if (KeyPressed(42)) keytab[K_SEMICOLON]=1;
    //0
    if (KeyPressed(48)) keytab[K_MINUS]=1;
    //#
    if (KeyPressed(127)) keytab[K_EQU]=1;
   }
   
   //wspolne
   //6
   if (KeyPressed(49+5)) keytab[K_PGUP]=1;
   //9
   if (KeyPressed(49+8)) keytab[K_PGDOWN]=1;
  }
  else
  if (activewin==Win_Sound)
  //dla trybu dzwieku
  {
   //1
   if (KeyPressed(49)) keytab[K_SPACE]=1;
   //2
   if (KeyPressed(49+1)) keytab[K_S]=1;
   //3
   if (KeyPressed(49+2)) keytab[K_R]=1;
   //4
   if (KeyPressed(49+3)) keytab[K_LSBRACKET]=1;
   //5
   if (KeyPressed(49+4)) keytab[K_RSBRACKET]=1;
   //6
   if (KeyPressed(49+5)) keytab[K_PGUP]=1;
   //7
   if (KeyPressed(49+6)) keytab[K_SEMICOLON]=1;
   //8
   if (KeyPressed(49+7)) keytab[K_APOSTROPHE]=1;
   //9
   if (KeyPressed(49+8)) keytab[K_PGDOWN]=1;
   //0
   if (KeyPressed(48)) keytab[K_HOME]=1;
  }
  else
  //dla trybu pomocy
  if (activewin==Win_Help)
  {
   //6
   if (KeyPressed(49+5)) keytab[K_PGUP]=1;
   //9
   if (KeyPressed(49+8)) keytab[K_PGDOWN]=1;
  }


  if (shift) ps=1;
  if ((activewin==Win_Graph)&&(CGraph::GetGraph()->Is3DMode())) ps=(shift>0)*3+1;
  
  //up
  if (KeyPressed(16))
  {
   if(ps)
   {
    //dla konsoli
    if (activewin==Win_Con)
    {
     SetDColor(DColor-1);
    }
    else
    //dla trybu wykresu
    if (activewin==Win_Graph)
    {
     movecursor(0,-ps);
   	 AddNoBlockedKey(16);
    }
    else
    keytab[K_UP]=1;
   }
   else
   keytab[K_UP]=1;
  }
  
  //down
  if (KeyPressed(17))
  {
   if(ps) 
   {
    //dla konsoli
    if (activewin==Win_Con)
    {
     SetDColor(DColor+1);
    }
    else
    //dla trybu wykresu
    if (activewin==Win_Graph)
    {
     movecursor(0,ps);
   	 AddNoBlockedKey(17);
    }
    else
    keytab[K_DOWN]=1;
   }
   else
   keytab[K_DOWN]=1;
  }
  
  //left
  if (KeyPressed(14))
  {
   if(ps) 
   {
    //dla konsoli
    if (activewin==Win_Con)
    {
     keytab[K_HOME]=1;
     shift=0;
    }
    else
    //dla trybu wykresu
    if (activewin==Win_Graph)
    {
     movecursor(-ps,0);
   	 AddNoBlockedKey(14);
    }
    else
    keytab[K_LEFT]=1;
   }
   else
   keytab[K_LEFT]=1;
  }
  
  //right
  if (KeyPressed(15))
  {
   if(ps) 
   {
    //dla konsoli
    if (activewin==Win_Con)
    {
     keytab[K_END]=1;
     shift=0;
    }
    else
    //dla trybu wykresu
    if (activewin==Win_Graph)
    {
     movecursor(ps,0);
   	 AddNoBlockedKey(15);
    }
    else
    keytab[K_RIGHT]=1;
   }
   else
   keytab[K_RIGHT]=1;
  }
  
  //enter
  MouseKeyOn=0;
  if (KeyPressed(167))
  {
   if(shift) 
   {
    shift=0;
    keytab[K_LSHF]=0;
    MouseKeyOn=2;
    AddNoBlockedKey(167);
   }
   else
   if (activewin==Win_Graph)
   {
   	MouseKeyOn=1;
    AddNoBlockedKey(167);
   }
   else
   keytab[K_ENTER]=1;
  }
  
  //c
  if (KeyPressed(1))
  {
   if (!shift)
   keytab[K_BKSPACE]=1;
   else
   keytab[K_DEL]=1;
  }
  
  //call
  if (KeyPressed(196))
  {
   keytab[K_F10]=1;
   if (!vkeyb->IsActive())
   {
    if ((activewin==Win_Con)&&(shift==0)) keytab[K_TAB]=1;
    else
    {
     vkeyb->SetActive();
     keytab[K_F10]=0;
    }
   }
  }
  
  //r
  if (KeyPressed(165))
  {
   if (activewin!=Win_Con)
   keytab[K_F4]=1;
   else
   keytab[K_F5]=1;
  }
  
  //zamiana! 127<->42 ok
  //*
  if (KeyPressed(42))
  {
   //wirtualny *
   keytab[224]=1;
  }
  
  //#
  if (KeyPressed(127))
  {
   //wirtualny .
   keytab[225]=1;
  }
  
  //klawisze *,# dla trybu konsoli
  if ((activewin==Win_Con)
  &&(!vkeyb->IsActive())
  )
  {
   if (shift==0)
   {
   	if (keytab[224]==1)
   	{
   	 keytab[224]=0;
   	 multimappingon=1-multimappingon;
   	}
   }
  }
 
  memcpy(s60graph_keytab,keytab,256);
  
  //multimapping klawiszy
  static int mmk=0,mmpk=0,mmid=0,mmc=0;
  static float mmtimer=0;
  if (((activewin==Win_Con)&&(multimappingon)&&(shift==0))
  &&(!vkeyb->IsActive())&&(CCon::GetCon()->libwinon==0))
  {
   int i;
   for (i=K_1; i<=K_0; i++)
   {
   	if (GetKey(i)==1) break;
   }
   if (i<=K_0)
   {
    mmk=i-K_1;
   	if ((mmk!=mmpk)||(GetTime()-mmtimer>0.75))
   	//nowy znak
   	{
   	 mmid=0;
   	 mmtimer=GetTime();
   	 mmc=multimaptab[mmk][0];
   	}
   	else
   	//nastepny znak
   	{
   	 s60graph_keytab[K_BKSPACE]=1;
   	 mmid++;
   	 if (mmid>=strlen(multimaptab[mmk])) mmid=0;
   	 mmtimer=GetTime();
   	 mmc=multimaptab[mmk][mmid];
   	}
   	mmpk=mmk;
   }
   if (mmc)
   {
   	for (int i=KEYMAPLEN-1; i>=0; i--)
   	if (keymap[i][1]==mmc)
   	{
   	 s60graph_keytab[keymap[i][0]]=1;
   	 break;
   	}
   	mmc=0;
   }
  }
  
  //dla wirtualnej klawiatury
  if (vkeyb->IsActive())
  {
   int i;
   for (i=0; i<256; i++)
   {
    if (KeyPressed(i)) 
    {
     vkeyb->KeyFunc(i|shift);
     int s=vkeyb->GetScan();
     if (s)
     {
      if (s!=255) s60graph_keytab[s]=1;
      s60graph_keytab[i]=0;
     }
    }
   }
  }

  if (ctrl==0)
  {
   if (GetKey(K_F8)) return 0;
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
   if (GetKey(K_F4)) 
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
   if (GetKey(K_F5))
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
   if (GetKey(K_F6))
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
   
   if (GetKey(K_BKSPACE)) ((CWin*)windows[activewin])->KeyFunc(K_BKSPACE|shift);
   for (int i=0; i<256; i++)
   {
    if (GetKey(i)) ((CWin*)windows[activewin])->KeyFunc(i|shift);
   }
   if (MouseKey()) ((CWin*)windows[activewin])->KeyFunc(shift);
  }
  else
  {
ctrlpressed:
   if (keytab[K_LSHF]) shift=256;
   if (shift==0)
   {
    for (int i=2; i<MAXFUNCCOUNT+2; i++) if (GetKey(i)) SetDColor(i-2);
   }
   else
   for (int i=2; i<CGraph::MMAX+2; i++) if (GetKey(i)) SetDMode(i-2);
   memset(s60graph_keytab,0,256);
  }
  //memset(s60graph_keytab,0,256);
  
  memset(simulatedkeytab,0,256);
  
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
  
  FlushNoKeyBlockedStack();
  if (((CWin*)windows[activewin])->GetBuf()!=buf) ((CWin*)windows[activewin])->Change(buf);
  //dla libwin
  if (((CWin*)windows[4])->GetBuf()!=buf) ((CWin*)windows[4])->Change(buf);
  ((CWin*)windows[activewin])->Process();
  if (vkeyb->IsActive())
  {
   vkeyb->Change(ScrWidth,ScrHeight,buf);
   vkeyb->Draw(shift);
  }
  
  //rysuj kursor myszy
  if ((activewin==Win_Graph)&&(cursormoved))
  {
   cursormoved=0;
   int i,j;
   j=CursorPosY;
   for (i=CursorPosX-CURSORLEN; i<=CursorPosX+CURSORLEN; i++)
   {
   	if (!((i<0)||(j<0)||(i>=ScrWidth)||(j>=ScrHeight)))
   	{
   	 unsigned int c;
   	 c=*(unsigned int*)((int)buf+(j*pitch+i*sizeof(int)));
   	 unsigned char r,g,b;
   	 r=(255-((c>>16)&255));
   	 g=(255-((c>>8)&255));
   	 b=(255-((c)&255));
   	 SetBuf32(i,j,RGB32(r,g,b));
   	}
   }
   i=CursorPosX;
   for (j=CursorPosY-CURSORLEN; j<=CursorPosY+CURSORLEN; j++)
   {
   	if (!((i<0)||(j<0)||(i>=ScrWidth)||(j>=ScrHeight)||(j==CursorPosY)))
   	{
   	 unsigned int c;
   	 c=*(unsigned int*)((int)buf+(j*pitch+i*sizeof(int)));
   	 unsigned char r,g,b;
   	 r=(255-((c>>16)&255));
   	 g=(255-((c>>8)&255));
   	 b=(255-((c)&255));
   	 SetBuf32(i,j,RGB32(r,g,b));
   	}
   }
  }
  if ((activewin==Win_Con)&&(multimappingon))
  {
   ((CWin*)windows[activewin])->DrawText3X5(ScrWidth-4*3-1,0,0xffffff,"ABC");
  }
  
 return 1;
}

float CSys::GetTime()
{
 return _GetTime();
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
 x=CursorPosX;
 y=CursorPosY;
}


int CSys::cursormoved=0;
void CSys::movecursor(int dx,int dy)
{
 CursorPosX+=dx;
 CursorPosY+=dy;
 if (CursorPosX<0) CursorPosX=0;
 if (CursorPosY<0) CursorPosY=0;
 if (CursorPosX>=ScrWidth) CursorPosX=ScrWidth;
 if (CursorPosY>=ScrHeight) CursorPosY=ScrHeight;
 cursormoved=1;
}

void CSys::VidMode(int w,int h)
{
 if ((w<32)||(h<32))
 {
  w=240;
  h=320;
  AutoVidMode=1;	
 }
 SetVidMode(w,h,ScrBpp,AutoVidMode);
 w=s60graph_scrwidth;
 h=s60graph_scrheight;
 ScrWidth=w;
 ScrHeight=h;
 CursorPosSet(ScrWidth/2,ScrHeight/2);
 memset(s60graph_keytab,0,256);
#ifdef OPENGL
 COpenGL::Close();
#endif
 
#ifdef OPENGL
 COpenGL::Init(ScrWidth,ScrHeight);
#endif
 for (int j=0; j<MAXWIN; j++) if ((CWin*)windows[j])
 ((CWin*)windows[j])->Change(ScrWidth,ScrHeight,buf);
 Log_Printf("Current mode: %dX%d",ScrWidth,ScrHeight);
}

void CSys::GetSysRes(int &w,int &h)
{
 w=s60graph_scrwidth;
 h=s60graph_scrheight;
}

FILE *CSys::FOpen(char *fn,char *m) 
{
 char b[256];
 FILE *r;
 r=0;
 
 strcpy(b,pathtab[0]);
 strcat(b,fn);
 r=fopen(b,m);
 if (r) return r;
 
 strcpy(b,pathtab[1]);
 strcat(b,fn);
 r=fopen(b,m);
 if (r) return r;
 
 strcpy(b,fn);
 r=fopen(b,m);
 if (r) return r;
 
 return r;
}

char *CSys::GetClipboardText()
{
 return "";
}

void CSys::Error(char *info,int mode)
{
 if (mode) return;
#ifdef OPENGL
 COpenGL::Close();
#endif
 TBuf<256> b;
 str2desc(info,b);
 User::Panic(b,-1);
 User::Exit(-1);
}

//biblioteki dodatkowe sa umieszczone w programie
ENV expr_env;
#include "dlib/fractal.cpp"	
#include "dlib/obj.cpp"	
#include "dlib/bmp.cpp"	
int CSys::DLibLoad(char *dlibname)
{
 expr_env.CSys_GetTime=CSys::GetTime;
 expr_env.CSys_FOpen=CSys::FOpen;
 expr_env.MultiExec=&CExpr::MultiExec;
 expr_env.GraphState=&CGraph::GraphState;
 char *pom=dlibname;
 while (*pom) 
 {
  if ((*pom>='a')&&(*pom<='z')) *pom+='A'-'a';
  pom++;
 }
 if (strcmp(dlibname,"FRACTAL")==0)
 {
  CExpr::GetLib()->AddList(fractal_lib,sizeof(fractal_lib)/sizeof(ELEMENT));
  return 1;
 }
 else
 if (strcmp(dlibname,"OBJ")==0)
 {
  CExpr::GetLib()->AddList(obj_lib,sizeof(obj_lib)/sizeof(ELEMENT));
  return 1;
 }
 else
 if (strcmp(dlibname,"BMP")==0)
 {
  CExpr::GetLib()->AddList(bmp_lib,sizeof(bmp_lib)/sizeof(ELEMENT));
  return 1;
 }
 return 0;
}

void *CSys::AllocMem(int size)
{
 void *r=User::Alloc(size);
 if (r==0) Error("Allocation fault");
 return r;
}

void *CSys::ReallocMem(void *ptr,int prevsize,int size)
{
 void *r=User::ReAlloc(ptr,size);
 if (r==0) Error("Allocation fault");
 return r;
}

void CSys::FreeMem(void *ptr)
{
 if (ptr)
 User::Free(ptr);
}

unsigned int CSys::MemAvail()
{
 int m;
 HAL::Get(HAL::EMemoryRAMFree,m);
 return m;
}

unsigned int CSys::MemTotal()
{
 int m;
 HAL::Get(HAL::EMemoryRAM,m);
 return m;
}

int CSys::SetSound()
{
 if (SoundOn) return 1;
 if (SoundBPS!=16) SoundBPS=16;
 if ((SoundChannels!=1)&&(SoundChannels!=2)) SoundChannels=2;
 if (!InitS60Sound(SOUND_MIXER_LEN)) return 0;
 SoundFreq=SetSoundBuffer(SoundFreq,SoundBPS,SoundChannels);
 SoundOn=1;
 SoundMixerBufSize=sndbufsize;
 return 1;
}

void CSys::StopSound()
{
 CloseS60Sound();
 SoundOn=0;
}

int CSys::IsMixerAvailable()
{
 int t=GetWSoundPos();
 if (t!=-1) 
 return 1; 
 else 
 return 0;
}

void CSys::MixSounds(SOUNDCHANNEL *sc,int n)
{
 int p=GetWSoundPos();
 SoundMixerBufSize=sndbufsize;
 if (p!=-1)
 {
  BeginSndBufAccess();
  if (sndbuf)
  {
   MixSoundChannels(sc,n,SoundBPS,SoundChannels,(void*)((int)sndbuf),SoundMixerBufSize);
  }
  EndSndBufAccess();
 }
 else return;
}

void CSys::RecordInputSound()
{
 return;
}

void *CSys::FlushInputSoundBuf()
{
 return 0;
}

char *CSys::SysInfoString()
{
 static char inf[1024];
 char buf[1024];
 
 strcpy(inf,"\02---EXPR for s60---\n");
 strcat(inf,"\05OS INFO\n");
 TVersion ver;
 ver=User::Version();
 sprintf(buf,"Symbian ver. %d.%d (build %d)\n",ver.iMajor,ver.iMinor,
 ver.iBuild);
 strcat(inf,buf);
 sprintf(buf,"Mem avail: %dK\nTotal mem: %dK\nWidth: %d\nHeight: %d\n",
 MemAvail()/1024,MemTotal()/1024,ScrWidth,ScrHeight);
 strcat(inf,buf);
#ifdef OPENGL
 strcat(inf,"\05OPENGL INFO\n");
 sprintf(buf,"GL_VENDOR: %s\nGL_RENDERER: %s\nGL_VERSION: %s\n",
 glGetString(GL_VENDOR),glGetString(GL_RENDERER),glGetString(GL_VERSION));
 strcat(inf,buf);
#endif
 strcat(inf,"\05CPU SPECIFIC INFO\n");
 int cpufreq;
 HAL::Get(HAL::ECPUSpeed,cpufreq);
 sprintf(buf,"CPU freq: %0.0fMHZ\n",cpufreq/1000.f);
 strcat(inf,buf);
 return inf;
}

//funckcje niezalezne od systemu

unsigned int inline CSys::FadeColor(unsigned int c,unsigned char fade)
{
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

void CSys::SimulateKey(int k) 
{
 simulatedkeytab[k]=1;
}

int CSys::Shell(char *cl)
{
 CCon::GetCon()->Put("Shell is not supported in Symbian OS");
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
 if (DColor<0) DColor+=MAXFUNCCOUNT;
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
