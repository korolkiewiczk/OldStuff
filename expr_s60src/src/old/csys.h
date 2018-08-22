//csys.h

#define MAXWIN 5
#define MAXLIB 8
#define MAXPATH 3
#define MAXNOKBSTACK 4
#define OPTMIXBUFLEN_SEC 0.08f
#define SOUND_MIXER_LEN (((ifloor(OPTMIXBUFLEN_SEC*(float)SoundFreq)*SoundBPS*SoundChannels/8)/16)*16)
#define TITLETEXT "expr v. 0.7"

#define _MEM(a,b,c) (*(c*)((unsigned int)(a)+(b)*sizeof(c)))

#define VAR_DWORD 0
#define VAR_FLOAT 1
#define VAR_BYTE 2
#define VAR_WORD 3
#define VAR_RGB 4
#define VAR_STR 5

class CSys
{
 private:
 //dane prywatne
 static void *windows[MAXWIN];
 static int activewin;
 static CLib varlib;
 //dane i funkcje prywatne zalezne od systemu
 static int nokbstack[MAXNOKBSTACK],nokbstacki;
 static void AddNoBlockedKey(int c) {nokbstack[nokbstacki%MAXNOKBSTACK]=c; nokbstacki++;}
 static void FlushNoKeyBlockedStack() 
 {
  while (nokbstacki>0)
  {
   s60graph_keytab[nokbstack[(nokbstacki-1)%MAXNOKBSTACK]]=1;
   nokbstacki--;
  }
 }
 static int multimappingon;
 static char *multimaptab[10];
 static char *pathtab[MAXPATH];
 static FILE *logfile;
 static RLibrary DLibraries[MAXLIB];
 static int AutoVidMode;
 static int CursorPosX,CursorPosY,MouseKeyOn;
 static unsigned char simulatedkeytab[256];
 
 public:
 //publiczne dane
 static unsigned int Color[];
 static int DColor;
 static int PresentWait;
 static int ScrWidth,ScrHeight,ScrBpp;
 static int DLibrariesC;
 static char DLibrariesNames[MAXLIB][64];
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
 static int Run();
 static void VidMode(int w,int h);
 static void GetSysRes(int &w,int &h);
 static float GetTime();
 static void Log_Printf(char *s,...);
 static void CursorPos(int &x,int &y);
 static void CursorPosSet(int x,int y){CursorPosX=x; CursorPosY=y;}
 static int MouseKey()
 {return MouseKeyOn;}
 static char *GetClipboardText();
 static int FileExists(char *fn);
 static FILE *FOpen(char *fn,char *m);
 static int IsValidStr(char *s) {return 1;}
 static int IsValidWPtr(void *p) {return 1;}
 static int IsValidRPtr(void *p) {return 1;}
 static void Error(char *info);
 static int DLibLoad(char *dlibname);
 static int EnumVModes(char *buf);
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
 
 //funkcje niezalezne od systemu
 static void SetWindow(void *w,int a) {windows[a]=w;}
 static void SetActiveWin(int w) {activewin=w;}
 static unsigned int FadeColor(unsigned int c,unsigned char fade);
 static int DiscardZeros(char *bf);
 static void SetDMode(int m);
 static void SetDColor(int c);
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
