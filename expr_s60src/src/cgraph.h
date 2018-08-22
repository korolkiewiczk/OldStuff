//cgraph.h

#define DEFAULT_JWIDTH 10
#define POSDELTA 4.f
#define SCALEDELTA 1.1f
#define MAXFUNCCOUNT 8
#define MAXDRAWINGTIME 0.02
#define DLINES 16
#define MINADELTA 40
#define DEFAULTVANLGE 90*3.141592f/180
#define DEFAULTR 20
#define DEFAULTD 1.0
#define MINPROJECTZ 0.1f
#define DMUL 0.50132f
#define ZBUFMUL 64.f
#define GRAPHTAB_SIZE(a) ((a)*(a)+(2*a))
#define CUBEMAPTEXID 64

typedef struct _FUNCSTRUCT
{
 int color;
 int status; //0-brak, 1-do rysowania, 2-rysowana
} FUNCSTRUCT;

//struktura przechowuje wsp. rzutowanego wiercholka
typedef struct _IPOINT
{
 int x,y;
} IPOINT;

//przechowuje wektor
typedef struct _VEC
{
 float a,b,c;
} VEC;

typedef struct _VEC2
{
 float u,v;
} VEC2;

#ifdef OPENGL
typedef struct _IVEC4
{
 unsigned char a,b,c,d;
} IVEC4;
#endif

#ifdef CG
#define MAXCGPROGRAMS 4
#define CGSPECPROGRAMS 4
typedef struct _CGP
{
 CGprogram vprogram,fprogram;
 CGparameter modelview_projection_matrix;
 CGparameter texture0,texture1,envmap;
 CGparameter ambient;
 CGparameter diffuse;
 CGparameter specular;
 CGparameter shininess;
 CGparameter lightvec;
 CGparameter eyevec;
} CGP;

typedef struct _CGPSpec
{
 CGP cgp[CGSPECPROGRAMS];
} CGPSpec;

typedef struct _CGS
{
 CGcontext cgcontext;
 CGprofile vprofile,fprofile;
 CGPSpec programs[MAXCGPROGRAMS];
 int currentprogram;
 int effect;
} CGS;

char *cg_spec_program_names[CGSPECPROGRAMS]=
{
 "NOTEX_DLIGHT",
 "TEX_DLIGHT",
 "NOTEX_PLIGHT",
 "TEX_PLIGHT"
};

enum
{
 CGPROGRAM_NOTEX_DLIGHT,
 CGPROGRAM_TEX_DLIGHT,
 CGPROGRAM_NOTEX_PLIGHT,
 CGPROGRAM_TEX_PLIGHT
};

enum
{
 CGPROGRAM_DEFAULT,
 CGPROGRAM_BM,
 CGPROGRAM_ENV,
 CGPROGRAM_BMENV,
 CGPROGRAM_MAX
};

enum
{
 CGEFFECT_DEFAULT,
 CGEFFECT_NEGATIVE,
 CGEFFECT_SEPIA,
 CGEFFECT_16COLORS,
 CGEFFECT_8COLORS,
};
#endif

typedef struct _GAMEMODESTRUCT
{
 float grav;
 float friction;
 
 float player_height;
 float player_maxvel;
 float player_acc;
 float player_jumpvel;
 float player_mass;
 
 VEC player_vel;
 VEC2 player_accv;
 
 int moveplayer;
 
 float time;
} GAMEMODESTRUCT;

class CGraph:CWin
{
 private:
 float s[3];
 float cj[3];
 float cp[3];
 float j[3];
 float p[3];
 
 int reqredraw;
 
 int repmode;
 int titles;
 int axison,gridon;
 int geps; //dokladnosc wynikow w postaci 10^(-geps)
 int fitscr; //czy ma dopasowac wykresy do ekranu
 
 float fps;        //liczba klatek na sekunde
 float timer;      //miernik czasu rysowania
 int   frames;     //ilosc wygenerowanych klatek
 float timer0;     //czas poczatkowy dla liczenia sredniej liczby fps
 int   fpsmode;    //tryb pokazywania liczby fps'ow
 
 float cursorx,cursory;
 
 /*zmienne dla funkcji parametrycznej okreslajace przedzial
 zmiennosci zmiennej t (od t1 do t2 ze zmiana dt)*/
 float t1,t2;
 int nt;
 
 //dane dla 3D
 float vangle;       //kat widocznosci
 float A[3]; //katy, np. a[AxisZ] oznacza katy obrotu wokol osi OZ
 float R;    //promien widocznowsci
 float D;    //odleglosci miedzy punktami siatki
 int   N;      //2*R/D
 
 int   width_div_2,height_div_2;
 float sinax,cosax,sinay,cosay,sinaz,cosaz; //obliczone wczesniej sinusy i cosinusy
 
 float lightdist; //odleglosc dla ktorej nie widac juz obiektu
 
 int   changepos3dmode;           //szybkosc zmainy pozycji w trybie 3d
                                //0-rowne D, 1-1/256*R
 
 //wlacz lub wylacz zmiane natezenia koloru w zaleznosci od odleglosci
 //jezeli wyl to natezenie zalezy dla kazdego polozenia od lightdist(255-najjasniejsze)
 int holdlight;
 
 //jesli wlaczone to modyfikowany jest wektor swiatla
 int modlvec;
 
 //punkt kamery
 float xs,ys,zs;
 //punkt kamery zatrzymany
 float hxs,hys;   //srodek punktu rysowania
 float cx1,cy1,cx2,cy2;
 
 //TABLICE DANYCH WYKRESU
 float           *valtab; //tablica z obliczonymi wartosciami funkcji, przydzielana dynamicznie
 IPOINT          *projecttab; //tablica przetworzonych-rzutowanych wierzcholkow
 unsigned char   *colortab; //bufor koloru dla rysowania trojkatow
 VEC             *normaltab; //tablica wektorow normalnych
 VEC2            *texcoordtab; //tablica wspolzednych tekstury
 
 unsigned int    *zbuf;     //bufor glebi dla softa
 
 //status wykonywanych operacji 0-projectbuf,2-normaltab,4-colortab,6-rysowanie.
 //nieparzyste-w trakcie
 int dstate;
 
 int hold;        //czy wstrzymac obliczanie valtab-w tym trybie rysowana jest tylko 1 funkcja
 
 //sposoby rysowania w 3D
 int dmethod;
 enum
 {
  MLINES,
  MFILL,
  MTEX,
  DMMAX
 };
 
 //czy oswietlenie
 int lighting;
 
 //czy oswietlenie rozne dla przedniej i tylnej powierzchni
 int twosidelighting;
 
 //wektor oswietlenia
 float light_vec[4];
 
 //tekstrury dla wykresow
public:
 CTex tex[MAXFUNCCOUNT];
 CTex envmap[6];
private:
  
 //dla opengl
#ifdef OPENGL //***
 //czy uzywac opengl
 int gl_used;
 
 //czy ukrywac niewidoczne sciany
 int gl_cull;
 
 //oswietlenie
 float gl_light_ambient[4];
 float gl_light_diffuse[4];
 float gl_light_specular[4];
 float gl_light_emission[4];
 float gl_shininess;
 
 //wierzcholki
 VEC   *gl_vertextab;    //tablica wierzcholkow
 void  gl_compvertextabref(float x,float y);
 int   gl_compvertextab(CExpr *expr,FUNCSTRUCT *f); //oblicza vertextab dla opengl
 
 //indeksy wierzcholkow
 int   *gl_indextab;     //tablica indeksow
 int   gl_indextabl;     //licznik dla wskaznika na biezacy element
 void  gl_compindextabref8(float x,float y);
 void  gl_compindextabref6(float x,float y);
 int   gl_compindextab(FUNCSTRUCT *f); //oblicza indextab dla opengl
 
 //tablica kolorow
 IVEC4 *gl_colortab; //tablica obliczonych kolorow
 void  gl_compcoltabref(float x,float y);
 void  gl_compcoltab(FUNCSTRUCT *f); //oblicza colortab w kolorze o indeksie color dla opengl
 
 //ustawia odpowiednie parametry opengl
 void gl_update();
 
 //domyslne parametry opengl
 void gl_defaults()
 {
  gl_cull=0;
  gl_update();
 } 
#endif //***


 //listy wyrazen i nazw
 CExpr        dexprlist[MAXFUNCCOUNT+2];
 FUNCSTRUCT   dfuncstruct[MAXFUNCCOUNT];
 int          dstack[MAXFUNCCOUNT];
 int          dstackl;
 

#ifdef MENU
 //funckje menu
 void initmenu();
 void setmenuactivestate(int state);
 void setmenustate();
#endif
 
 //RYSOWANIE FUNKCJI
 int  drawfunc(CExpr *expr,FUNCSTRUCT *f);
 void allocbuffers();
 void begindrawfunc();
 void enddrawfunc();
 
 //Funkcja okregu ***
 int circlefunc(CExpr *expr,FUNCSTRUCT *f,int state,
                float rr,int tabf,int ladd,
                void (CGraph::*reffunc)(float x,float y));
#ifdef OPENGL
#define TPTR_SIZE 7
#else
#define TPTR_SIZE 5
#endif
 //tablica wskaznikow
 void *tptr[TPTR_SIZE];
 //tablica wskaznikow zrodlowych
 void *t0ptr[TPTR_SIZE];
 //tablica rozmiarow wskaznikow
 int  sptr[TPTR_SIZE];
 //nazwy tych wskaznikow
 enum
 {
  vwsk        =0,
  pwsk        =1,
  nwsk        =2,
  cwsk        =3,
  twsk        =4
#ifdef OPENGL
  ,gl_cwsk    =5
  ,gl_vwsk    =6
#endif
 };
 //flagi
 enum
 {
  FVWSK       =1,
  FPWSK       =2,
  FNWSK       =4,
  FCWSK       =8,
  FTWSK       =16
#ifdef OPENGL
  ,FGL_CWSK   =32
  ,FGL_VWSK   =64
#endif
 };
 
 //wartosci obliczone w celu przyspieszenia obliczen
 float        _255_lightdist;
 //wyrazenie dla funckcji circlefunc
 CExpr        *circfunc_expr;
 int          circfunc_dir;
 unsigned int *circfunc_palwsk;
 int          circfunc_disty;
 float        circfunc_D2; //D*D
 float        circfunc_D4; //D*D*D*D
 float        circfunc_1_2_D; //D/2
 float        circfunc_constcol;
 CTex         *circfunc_tex;
 //***
 
 //status rysowania
 enum
 {
  DS_PROJECT  =0,
  DS_NORMAL   =2,
  DS_COL      =4,
  DS_TEX      =6,
  DS_DRAW     =8
 };
 
 //2D
 int   drawfunc_K2DF1(CExpr *expr,FUNCSTRUCT *f);
 int   drawfunc_K2DXY(CExpr *expr,FUNCSTRUCT *f);
 int   drawfunc_K2DF2(CExpr *expr,FUNCSTRUCT *f);
 
 //3D
 int   drawfunc_K3DF2_soft_grid(FUNCSTRUCT *f);
 
 void  drawfunc_K3DF2_soft_fillref(float x,float y);
 int   drawfunc_K3DF2_soft_fill(FUNCSTRUCT *f);
 
 int   drawfunc_K3DF2_soft_fill_light(FUNCSTRUCT *f);
 
 void  drawfunc_K3DF2_soft_fill_texref(float x,float y);
 int   drawfunc_K3DF2_soft_fill_tex(FUNCSTRUCT *f);
 
 int   drawfunc_K3DF2_soft_fill_tex_light(FUNCSTRUCT *f);
#ifdef OPENGL
 //rysowanie dla opengl 
 int   drawfunc_K3DF2_gl_grid(FUNCSTRUCT *f);
 
 int   drawfunc_K3DF2_gl_fill(FUNCSTRUCT *f);
 
 int   drawfunc_K3DF2_gl_fill_light(FUNCSTRUCT *f);
 
 int   drawfunc_K3DF2_gl_fill_tex(FUNCSTRUCT *f);
#endif
 //wyswietl punkt gdzie znajduje sie zrodlo swiatla
 void  drawsun();
 
 //funkcje mapy srodowiska
 char envmapfile[128];
 int loadenvmap(char *fn);
 int envmapon;
 //rysuje srodowisko
 void drawenv();
 
 //funkcje dla 3D
 void inline  rotate(float x,float y,float z,float &xr,float &yr,float &zr);
 int          project(float x,float y,float z,int &xp,int &yp);
 void         sqcircorners(float x,float y,float &x1,float &y1,float &x2,float &y2);
 
 void         compvaltabref(float x,float y);
 int          compvaltab(CExpr *expr,FUNCSTRUCT *f);

 void         compprojecttabref(float x,float y);
 void         compprojecttab(); //tworzy tablice projecttab na podst. valtab
 
 void         compnormaltabref(float x,float y);
#ifdef OPENGL
 void         gl_compnormaltabref(float x,float y);
#endif
 void         compnormaltab(); //tworzy tablice normalnych na podst. valtab
 
 void         compcoltabref(float x,float y);
 void         compcoltab(int color); //oblicza colortab w kolorze o indeksie color
 void         compcoltabnormalref(float x,float y);
 void         compcoltabnormalcconstref(float x,float y);
 void         compcoltabnormal(int color); //oblicza colortab w kolorze o indeksie color
                                   //z uwzglednieniem normalnych wektorow

 void         comptexcoordtabref(float x,float y);
 void         comptexcoordtab(FUNCSTRUCT *f);
 
 void         setzdist() {p[AxisZ]=width_div_2/tan(vangle/2);}
 //iloczyn skalarny wektorow
 float inline dotproduct(VEC *a,VEC *b);
 
#ifdef CG
 //vertex/pixel shader
 CGS cgs;
 int useshaders;
 void initshaders();
 int loadcgprograms();
 int initcg();
 char *buildcgvertexprogramtext(int prog,int type);
 char *buildcgfragmentprogramtext(int prog,int type,int efect);
 int loadcgprogram(int no);
 void bindcgprogram(int state);
 public:
 int UseShaders() {return (gl_used)&&(lighting)&&(useshaders>0);}
 private:
#endif
 
 //tryb gry
 GAMEMODESTRUCT gms;
 int gamemodeon;
 int physicsmodel;
 enum
 {
  PMODEL_SIMPLE,
  PMODEL_ACCURATE
 };
 void updateplayer();
 float getsurfz(float x,float y,float &anglex,float &angley);
 
 
 //funckje informacyjno-obslugowe
 void  handlemouse();
 void  drawinfo();
 void  drawaxis();
 float axisdelta(int axis);
 void  genpalettes();
 void  updatepos();
 void  defaults();
 
 public:
 static CGraph       *currentgraph;
 static unsigned int Color[MAXFUNCCOUNT];
 static unsigned int Palette[MAXFUNCCOUNT][256];
 static int          DMode;
 static int          GraphState;
 
 enum
 {
  AxisX,
  AxisY,
  AxisZ
 };
 
 enum
 {
  K2DF1,  //uk³ad kartezjanski w 2D, funkcja jedno-argumentowa y=f(x)
  K2DF2,  //uk³ad kartezjanski w 2D, funkcja dwu-argumentowa z=f(x,y)
  K3DF2,  //uk³ad kartezjanski w 3D, funkcja dwu-argumentowa z=f(x,y)
  K2DXY,  //uk³ad kartezjanski w 2D, funkcja parametryczna x=x(t), y=y(t)
  MMAX
 };
 
 CGraph(int w,int h,void *b);  //konstruktor
 ~CGraph();                    //destruktor
 
 void          Change(int w,int h,void *b); 
 void          ChangeActiveState(int state); 
 void          KeyFunc(int k);
 void          Process();
 float         FP(float j,int axis) {return s[axis]*j+cp[axis];}
 float         FJ(float p,int axis) {return p/s[axis]+cj[axis];}
 float         Findx0(CExpr *expr,float a,float b,float d,float eps);
 void          SetExpr(char *name,CExpr *expr,int color,int num);
 void          DelExpr(int num);
 void          NextExpr();
 static CGraph *GetGraph() {return currentgraph;}
 void          ChangePos(float d,int axis);
 void          ChangePos3D(float d,float anglex,float anglez);
 void          ChangeScale(float d,int axis);
 void          ChangeMode(int m) {DMode=m%MMAX; reqredraw=3;}
 int           Is3DMode() {return DMode==K3DF2;}
};
