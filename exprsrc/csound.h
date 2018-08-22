//csound.h

#define DEFAULTTJX 0.001f
#define DEFAULTAJY 1.f
#define DEFAULTVSIZE 0.01f

//wzgledne wymiary miksera
#define MIXERSETUPX 0.1f
#define MIXERSETUPY 0.1f
#define MIXERSETUPHEIGHT 0.1f
#define MIXERSETUPBARHEIGHT 0.67f
#define MIXERSETUPBARWIDTH 0.8f

#define FUNCOSCH 0.2f //wzgledna wysokosc wykresu oscyloskopa

#define DELTAVOL 0.05f
#define DELTAPAN 0.05f

#define DELTAT 0.05f

#define DELTATJX 1.04f
#define DELTAAJY 0.05f
#define DELTAVSIZE 0.01f

//struktura przechowuje informacje nt. odtwarzanej funkcji dzwiekowej
typedef struct _SOUNDFUNCSTRUCT
{
 int    status;  //czy funkcja uzywana
 double x;    //pozycja x funkcji
 float  pos;   //pozycja odtwarzanego dzwieku (sek.)
 float  t1,t2; //przedzial odtwarzania (sek.)
 int    sat;     //czy liczyc z nasyceniem (1)
 int    rep;     //czy powtarzac w przedziale <t1,t2> (1), czy w <t1,inf) (0)
} SOUNDFUNCSTRUCT;

class CSound:CWin
{
 private:
 float Tjx; //czas na jedn. x
 float Ajy; //amplituda na jedn. y
 float vsize; //przestrzen podgladu funkcji (w sek.)
 
 int paused;
 
 int  inputon; //czy wlaczone jest wejscie
 void *inputbuf;    //bufor dla wejscia
 
 CExpr           sexprlist[MAXFUNCCOUNT];
 SOUNDCHANNEL    smixerchannels[MAXFUNCCOUNT];
 SOUNDFUNCSTRUCT sfuncstruct[MAXFUNCCOUNT];
 
#ifdef MENU
 void initmenu();
 void setmenuactivestate(int state);
 void setmenustate();
#endif
 
 void compsoundbuf(int n); //oblicza dany bufor dzwieku na podstawie wyrazenia
 
 void defaults()
 {
  Tjx=DEFAULTTJX;
  Ajy=DEFAULTAJY;
  vsize=DEFAULTVSIZE;
  memset(smixerchannels,0,sizeof(smixerchannels));
  for (int i=0; i<MAXFUNCCOUNT; i++)
  {
   smixerchannels[i].vol=0.5f;
   sfuncstruct[i].t1=0.f;
   sfuncstruct[i].t2=1.f;
   sfuncstruct[i].sat=1;
   sfuncstruct[i].rep=0;
   Reset(i);
  }
 }
 
 float pos2x(float pos)
 {
  return pos/Tjx;
 }
 
 float x2pos(float x)
 {
  return x*Tjx;
 }
 
 void drawosc(int n);
 void drawinfo();
 
 public:
 
 static unsigned int Color[MAXFUNCCOUNT];
 static CSound       *currentsound;
        
 CSound(int w,int h,void *b):CWin(w,h,b)
 {
  memset(sexprlist,0,sizeof(sexprlist));
  memset(sfuncstruct,0,sizeof(sfuncstruct));
  defaults();
  paused=0;
  inputon=0;
#ifdef MENU
  initmenu();
#endif
 }
 
 void ChangeActiveState(int state)
 {
  if (state==0) CSys::StopSound();
#ifdef MENU
  setmenuactivestate(state);
#endif
 }
 
 void          KeyFunc(int k);
 void          Process();
 void          SetExpr(char *name,CExpr *expr,int num);
 void          DelExpr(int num);
 void          NextExpr();
 void          ChangeVolPan(int n,float dvol,float dpan);
 void          Reset(int n);
 static CSound *GetSound() {return currentsound;}
};
