//csys.h

//definicje ogolne
#define MAXWIN 5
#define MAXLIB 8
#define OPTMIXBUFLEN_SEC 0.08f
#define SOUND_MIXER_SECTIONS (iceil((float)sndbufsize*8.f/(float)SoundBPS/\
(float)SoundChannels/((float)SoundFreq*OPTMIXBUFLEN_SEC))-1)
#define RECORDINPUTSOUNDBUFLEN 65536
#define TITLETEXT "Expr v. 0.91"

#define _MEM(a,b,c) (*(c*)((unsigned int)(a)+(b)*sizeof(c)))

#define VAR_DWORD 0
#define VAR_FLOAT 1
#define VAR_BYTE 2
#define VAR_WORD 3
#define VAR_RGB 4
#define VAR_STR 5

#ifdef MENU
//komendy menu
enum
{
 MENU_FILE=1,
 MENU_CONSOLE,
 MENU_GRAPH,
 MENU_SOUND,
 MENU_HELP,
 MENU_FILE_WINDOW,
 MENU_FILE_WINDOW_CON,
 MENU_FILE_WINDOW_GRAPH,
 MENU_FILE_WINDOW_SOUND,
 MENU_FILE_WINDOW_HELP,
 MENU_FILE_VIDMODE,
 MENU_FILE_VIDMODE_F1,
 MENU_FILE_VIDMODE_F2,
 MENU_FILE_VIDMODE_F3,
 MENU_FILE_VIDMODE_F4,
 MENU_FILE_VIDMODE_F5,
 MENU_FILE_VIDMODE_F6,
 MENU_FILE_VIDMODE_F7,
 MENU_FILE_VIDMODE_F8,
 MENU_FILE_VIDMODE_WINDOWED,
 MENU_FILE_PSCR,
 MENU_FILE_PSCRNEG,
 MENU_FILE_LOADTEX,
 MENU_FILE_LOADDLL,
 MENU_FILE_LOADFONT,
 MENU_FILE_EXIT,
 MENU_CONSOLE_CLEAR,
 MENU_CONSOLE_PUTPATH,
 MENU_CONSOLE_FLIST,
 MENU_CONSOLE_FONTSIZE,
 MENU_CONSOLE_FONTSIZE_8X8,
 MENU_CONSOLE_FONTSIZE_8X16,
 MENU_GRAPH_DMODE,
 MENU_GRAPH_DMODE1,
 MENU_GRAPH_DMODE2,
 MENU_GRAPH_DMODE3,
 MENU_GRAPH_DMODE4,
 MENU_FILE_DCOLOR,
 MENU_FILE_DCOLOR1,
 MENU_FILE_DCOLOR2,
 MENU_FILE_DCOLOR3,
 MENU_FILE_DCOLOR4,
 MENU_FILE_DCOLOR5,
 MENU_FILE_DCOLOR6,
 MENU_FILE_DCOLOR7,
 MENU_FILE_DCOLOR8,
 MENU_GRAPH1,
 MENU_GRAPH2,
 MENU_GRAPH3,
 MENU_GRAPH4,
 MENU_GRAPH5,
 MENU_GRAPH6,
 MENU_HELP_HELP,
 MENU_HELP_ABOUTTEXT,
 MENU_HELP_SYSINFO
} MenuCommands;
#endif

//definicje dodatkowe

class CSys
{
 private:
 //dane prywatne
 static void *windows[MAXWIN];
 static int activewin;
 static CLib varlib;
 static int DLibrariesC;
 static char DLibrariesNames[MAXLIB][64];
 //dane i funkcje prywatne zalezne od systemu
 static int DLibraries[MAXLIB];
 static int Windowed;
 static int VidModes[8][2];
 static void *RecBuf;
 static int RecBufPos;
 static FILE *logfile;
 static int logmode;
 static unsigned char simulatedkeytab[256];
 static void SetCur();
#ifdef MENU
 friend void MenuCommandFunc(int w);
 friend void WinSizeChangeFunc(int w,int h,int a);
 static void InitMenu();
 static int OpenDialog(char *title,char *filter,char *fn,int bufsize);
 public:
 static CMenu Menu;
#endif
 public:
 //publiczne dane
 static unsigned int Color[];
 static int DColor;
 static int PresentWait;
 static int ScrWidth,ScrHeight,ScrBpp;
 static int SoundOn;
 static int SoundFreq,SoundBPS,SoundChannels,SoundMixerBufSize;
 static int SRand0;
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
 
 enum
 {
  Win_Con,
  Win_Graph,
  Win_Sound,
  Win_Help,
  Win_Winlib
 };
  
 //funkcje zalezne od systemu
 static void Init();
 static void Close();
 static void Run();
 static void VidMode(int w,int h);
 static void GetSysRes(int &w,int &h);
 static float GetTime();
 static void Log_Printf(char *s,...);
 static void CursorPos(int &x,int &y);
 static void CursorPosSet(int x,int y);
 static int MouseKey()
 {return MouseKeyPressed(WINGRAPH_MLEFT)|(MouseKeyPressed(WINGRAPH_MRIGHT)<<1);}
 static char *GetClipboardText();
 static FILE *FOpen(char *fn,char *m) {return fopen(fn,m);}
 static int IsValidStr(char *s) {return !IsBadStringPtr(s,1024);}
 static int IsValidWPtr(void *p) {return !IsBadWritePtr(p,4);}
 static int IsValidRPtr(void *p) {return !IsBadWritePtr(p,4);}
 static void Error(char *info,int mode=0);
 static int DLibLoad(char *dlibname);
 static void *AllocMem(int size);
 static void *ReallocMem(void *ptr,int prevsize,int size);
 static void FreeMem(void *ptr);
 static unsigned int MemAvail();
 static unsigned int MemTotal();
 static char *SysInfoString();
 static int SetSound();
 static void StopSound();
 static int IsMixerAvailable();
 static void MixSounds(SOUNDCHANNEL *sc,int n);
 static void RecordInputSound();
 static void *FlushInputSoundBuf();
 static void SimulateKey(int k);
 static int Shell(char *cl);
 static int LoadImage(char *fn,unsigned int **imgbuf,int *width,int *height);
 
 //funkcje niezalezne od systemu
 static void SetDMode(int m);
 static void SetDColor(int c);
 static void SetWindow(void *w,int a) {windows[a]=w;}
 static void SetActiveWin(int w) {activewin=w;}
 static unsigned int FadeColor(unsigned int c,unsigned char fade);
 static int DiscardZeros(char *bf);
 static void inline memset32(void *p,int a,unsigned int b);
 //flagi:
 //0-7-typ, 8-31-dodatkowe
 static void AddVar(char *name,void *addr,int flags);
 static void DelVar(char *name);
 static int SetVarV(char *name,char *charval,int shift=0);
 static int GetVarI(char *name,int shift=0);
 static float GetVarF(char *name,int shift=0);
 static void ExecCfg(char *sec);
};
