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
 ",.|1'\"<>:[]","abc2","def3",
 "ghi4","jkl5","mno6",
 "pqrs7","tuv8","wxyz9",
         "+-*/=0_"
};

char *CSys::pathtab[MAXPATH]={"e:\\expr\\","c:\\data\\","c:\\expr\\"};
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
 CSys::AddVar("color",&CSys::Color,VAR_RGB);
 ExecCfg(0);
 CSys::DelVar("color");
 
#ifdef OPENGL
 COpenGL::Init(ScrWidth,ScrHeight);
#endif
 SRand0=(int)(GetTime()*10000);
 srand(SRand0);
 CursorPosX=ScrWidth/2;
 CursorPosY=ScrHeight/2;
}

void CSys::Close()
{
 for (int i=0; i<MAXWIN; i++) ((CWin*)windows[i])->ChangeActiveState(0);
#ifdef OPENGL
 COpenGL::Close();
#endif
 for (int i=0; i<DLibrariesC; i++) DLibraries[i].Close();
}

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
  
  int shift=0,ctrl=0;
  
  //shift
  if (KeyPressed(18))
  {
   keytab[K_LSHF]=1;
   shift=256;
   AddNoBlockedKey(18);
  }
  
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
  }
  //dla trybu wykresu
  else
  if (activewin==Win_Graph)
  {
   if (shift==0)
   for (int i=0; i<4; i++)
   {
    if (KeyPressed(i+49))
    {
     keytab[K_1+i]=1;
    }
   }
   else
   {
    //1
    if (KeyPressed(49)) keytab[K_HOME]=1;
    //2
    if (KeyPressed(49+1)) keytab[K_H]=1;
    //3
    if (KeyPressed(49+2)) keytab[K_J]=1;
    //4
    if (KeyPressed(49+3)) keytab[K_K]=1;
   }
   //5
   if (KeyPressed(49+4)) keytab[K_R]=1;
   //6
   if (KeyPressed(49+5)) keytab[K_PGUP]=1;
   if (CGraph::GetGraph()->Is3DMode())
   {
   	//7
   	if (KeyPressed(49+6)) keytab[K_U]=1;
   	//8
   	if (KeyPressed(49+7)) keytab[K_I]=1;
   }
   else
   {
   	//7
   	if (KeyPressed(49+6)) keytab[K_X]=1;
   	//8
   	if (KeyPressed(49+7)) keytab[K_C]=1;
   }
   //9
   if (KeyPressed(49+8)) keytab[K_PGDOWN]=1;
  }
  else
  if (activewin==Win_Sound)
  //dla trybu dzwieku
  {
   //1
   if (KeyPressed(49)) keytab[K_HOME]=1;
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
  }
  
  //0
  if (KeyPressed(48))
  {
   if ((activewin==Win_Graph)&&(CGraph::GetGraph()->Is3DMode())&&(!vkeyb->IsActive())
   )
   keytab[K_L]=1;
   else
   if (activewin==Win_Sound)
   //spacja dla trybu dzwieku
   keytab[K_SPACE]=1;
   else
   keytab[K_0]=1;
  }
  
  int ps=0;
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
     CursorPosY-=ps;
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
     CursorPosY+=ps;
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
     CursorPosX-=ps;
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
     CursorPosX+=ps;
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
  
  //l
  if (KeyPressed(164))
  {
   if (shift)
   keytab[K_F1]=1;
   else
   keytab[K_F4]=1;
  }
  
  //r
  if (KeyPressed(165))
  {
   if (shift)
   keytab[K_F8]=1;
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
  
  //klawisze *,# dla trybu 3D
  if ((activewin==Win_Graph)&&(CGraph::GetGraph()->Is3DMode())
  &&(!vkeyb->IsActive())
  )
  {
   if (keytab[224]==1) keytab[K_N]=1;
   if (keytab[225]==1) keytab[K_M]=1;
  }
  
  //klawisze *,# dla trybu 2D
  if ((activewin==Win_Graph)&&(!CGraph::GetGraph()->Is3DMode())
  &&(!vkeyb->IsActive())
  )
  {
   if (keytab[224]==1) keytab[K_F]=1;
   if (keytab[225]==1) keytab[K_Z]=1;
  }
  
  if (activewin==Win_Sound)
  //klawisze *,# dla trybu dzwieku
  {
   if (keytab[224]==1) keytab[K_MINUS]=1;
   if (keytab[225]==1) keytab[K_EQU]=1;
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
  &&(!vkeyb->IsActive())
  )
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
    ((CWin*)windows[activewin])->PrintScreen();
   }
   if (KeyPressed(K_F1)) 
   {
    //help
    ((CWin*)windows[activewin])->ChangeActiveState(0);
    activewin=Win_Help; 
    ((CWin*)windows[activewin])->ChangeActiveState(1);
    PresentWait=0;
   }
   else
   if (KeyPressed(K_F4)) 
   {
    //con
    ((CWin*)windows[activewin])->ChangeActiveState(0);
    activewin=Win_Con;
    ((CWin*)windows[activewin])->ChangeActiveState(1);
    PresentWait=0;
   }
   else
   if (KeyPressed(K_F5))
   {
    //graph
    ((CWin*)windows[activewin])->ChangeActiveState(0);
    activewin=Win_Graph;
    ((CWin*)windows[activewin])->ChangeActiveState(1);
    PresentWait=0;
   }
   else
   if (KeyPressed(K_F6))
   {
    //sound
    ((CWin*)windows[activewin])->ChangeActiveState(0);
    activewin=Win_Sound;
    ((CWin*)windows[activewin])->ChangeActiveState(1);
    PresentWait=0;
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
   //brak obslugi klawisza ctrl
   /*if (shift==0)
   {
    for (int i=2; i<MAXFUNCCOUNT+2; i++) if (GetKey(i)) SetDColor(i-2);
   }
   else
   for (int i=2; i<CGraph::MMAX+2; i++) if (GetKey(i)) SetDMode(i-2);
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
    VidMode(ScrWidth,ScrHeight);
   }*/
  }
  //memset(s60graph_keytab,0,256);
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
 return 1;
}

float CSys::GetTime()
{
 return _GetTime();
}

void CSys::Log_Printf(char *s,...)
{
 //nie uzywane w s60
 
 /*if ((int)logfile==-1) logfile=FOpen("log.txt","wt");
 else logfile=FOpen("log.txt","at");
 if (!logfile) return;
 va_list args;
 char buf[2048];
 va_start(args,s);
 vsprintf(buf,s,args);
 va_end(args);
 fprintf(logfile,buf);
 fprintf(logfile,"\n");
 fclose(logfile);*/
}

void CSys::CursorPos(int &x,int &y)
{
 /*POINT p;
 GetCursorPos(&p);
 x=p.x;
 y=p.y;*/
 x=CursorPosX;
 y=CursorPosY;
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
 
 strcpy(b,pathtab[2]);
 strcat(b,fn);
 r=fopen(b,m);
 if (r) return r;
 
 return r;
}

int CSys::FileExists(char *fn) 
{
 FILE *f; 
 if (f=FOpen(fn,"r")) {fclose(f); return 1;} 
 else return 0;
}

char *CSys::GetClipboardText()
{
 /*OpenClipboard(0); 
 char *r=(char*)GetClipboardData(CF_TEXT); 
 CloseClipboard(); */
 return 0;
}

void CSys::Error(char *info)
{
#ifdef OPENGL
 COpenGL::Close();
#endif
 //_LIT(errtxt,"ERROR!");
 TBuf<256> b;
 str2desc(info,b);
 User::Panic(b,-1);
 User::Exit(-1);
}

int CSys::DLibLoad(char *dlibname)
{
 if (DLibrariesC==MAXLIB) return 0;
 for (int i=0; i<DLibrariesC; i++) if (strcmp(dlibname,DLibrariesNames[i])==0) 
      return 1;
 TBuf<256> path;
 TBuf<256> fn;
 str2desc(dlibname,fn);
 
 str2desc(pathtab[0],path);
 if (KErrNone==DLibraries[DLibrariesC].Load(fn,path)) goto ok;
 str2desc(pathtab[1],path);
 if (KErrNone==DLibraries[DLibrariesC].Load(fn,path)) goto ok;
 str2desc(pathtab[2],path);
 if (KErrNone==DLibraries[DLibrariesC].Load(fn,path)) goto ok;
 
 return 0;
ok:
 int (*reffunc)(void*,void*)=0;
 reffunc=(int(*)(void*,void*))DLibraries[DLibrariesC].Lookup(1);
 if (reffunc==0) return 0;
 CLib *l=CExpr::GetLib();
 void *list;
 ENV env;
 env.CSys_GetTime=CSys::GetTime;
 env.CSys_FOpen=CSys::FOpen;
 env.MultiExec=&CExpr::MultiExec;
 int n=reffunc(&list,&env);
 if (n)
 l->AddList(list,n);
 strcpy(DLibrariesNames[DLibrariesC],dlibname);
 DLibrariesC++;
 return 1;
}

int CSys::EnumVModes(char *buf)
{
 sprintf(buf,"%dX%d\n",s60graph_scrwidth,s60graph_scrheight);
 return 1;
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
 sprintf(buf,"Mem avail: %dK\nTotal mem: %dK\n",MemAvail()/1024,MemTotal()/1024);
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

void CSys::SimulateKey(int k) 
{
 simulatedkeytab[k]=1;
}

int CSys::Shell(char *cl)
{
 CCon::GetCon()->Put("Shell is not supported in Symbian OS");
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
       _MEM(n->fptr,2-shift%3+(shift/3)*4,unsigned char)  =_MEM(&val,0,unsigned char);
       break;
  case VAR_STR:
       n->fptr=(void*)charval;
       break;
 }
 return n->tag;
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
