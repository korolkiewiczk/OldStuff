//cgraph.c

#define UD 0x7fc00000
#define IS_UD(a) ((*(unsigned int*)((&a))&UD)==UD)
#define INFM 0xff800000
#define IS_INFM(a) (*(unsigned int*)((&a))==INFM)
#define INFP 0x7f800000
#define IS_INFP(a) (*(unsigned int*)((&a))==INFP)

#ifdef OPENGL
#define GLCOLOR(c) ((float)((c>>16)&255)/255.f),\
                   ((float)((c>>8)&255)/255.f),\
                   ((float)(c&255)/255.f)
#endif

unsigned int CGraph::Color[MAXFUNCCOUNT]=
{
 RGB32(255,255,255),
 RGB32(255,0,0),
 RGB32(0,255,0),
 RGB32(0,0,255),
 RGB32(255,255,0),
 RGB32(255,0,255),
 RGB32(0,255,255),
 RGB32(255,128,0),
};

unsigned int CGraph::Palette[MAXFUNCCOUNT][256];

CGraph *CGraph::currentgraph;
int CGraph::DMode=CGraph::K2DF1;
int CGraph::GraphState;


void CGraph::KeyFunc(int k)
{
 if (k!=256+K_LSHF)
 if (reqredraw<=1)
 reqredraw=1;
 float m=1,s=1,dir=1;
 if (k>>8!=0) {m=4; s=1.2; dir=-1;}
 k&=0xff;
 
 float changepos3ddelta=m;
 if (changepos3dmode==0) changepos3ddelta*=D;
 else changepos3ddelta*=R*(1.f/256.f);
 if (k==K_LEFT)
 {
  ChangePos(-POSDELTA*m,AxisX);
 }
 else
 if (k==K_RIGHT)
 {
  ChangePos(POSDELTA*m,AxisX);
 }
 else
 if (k==K_UP)
 {
  ChangePos(POSDELTA*m,AxisY);
 }
 else
 if (k==K_DOWN)
 {
  ChangePos(-POSDELTA*m,AxisY);
 }
 else
 if (k==K_HOME) 
 {
  defaults();
#ifdef OPENGL
  gl_defaults();
#endif
 }
 else
 if (k==K_PGUP)
 {
  if (Is3DMode()==0)
  {
   ChangeScale(SCALEDELTA*s,AxisX);
   ChangeScale(SCALEDELTA*s,AxisY);
  }
  else
  ChangePos(POSDELTA*m,AxisZ);
 }
 else
 if (k==K_PGDOWN)
 {
  if (Is3DMode()==0)
  {
   ChangeScale(1/SCALEDELTA/s,AxisX);
   ChangeScale(1/SCALEDELTA/s,AxisY);
  }
  else
  ChangePos(-POSDELTA*m,AxisZ);
 }
 else
 if (k==K_DEL)
 {
  DelExpr(CSys::DColor);
  reqredraw=2;
 }
 else
 if ((k==K_TAB)&&(hold==0)) NextExpr();
 else
 if (k==K_MINUS)
 {
  nt-=(int)m*50;
  if (nt<50) nt=50;
 }
 else
 if (k==K_EQU)
 {
  nt+=(int)m*50;
 }
 else
 if (k==K_LSBRACKET)
 {
  t1-=pow(10,-geps+2)*m;
 }
 else
 if (k==K_RSBRACKET)
 {
  t1+=pow(10,-geps+2)*m;
 }
 else
 if (k==K_SEMICOLON)
 {
  t2-=pow(10,-geps+2)*m;
 }
 else
 if (k==K_APOSTROPHE)
 {
  t2+=pow(10,-geps+2)*m;
 }
#ifdef CG
 if (k==K_TILDE)
 {
  if (dir<0)
  {
   if (useshaders>=0)
   useshaders=1-useshaders;
  }
  else
  {
   cgs.currentprogram++;
   if (cgs.currentprogram>=CGPROGRAM_MAX) cgs.currentprogram=0;
  }
 }
 else
 if (k==K_EXT0)
 {
  cgs.currentprogram=CGPROGRAM_DEFAULT;
 }
 else
 if (k==K_EXT1)
 {
  cgs.currentprogram=CGPROGRAM_BM;
 }
 else
 if (k==K_EXT2)
 {
  cgs.currentprogram=CGPROGRAM_ENV;
 }
 else
 if (k==K_EXT3)
 {
  cgs.currentprogram=CGPROGRAM_BMENV;
 }
 else
 if (k==K_EXT4)
 {
  cgs.effect=CGEFFECT_DEFAULT;
  loadcgprograms();
 }
 else
 if (k==K_EXT5)
 {
  cgs.effect=CGEFFECT_NEGATIVE;
  loadcgprograms();
 }
 else
 if (k==K_EXT6)
 {
  cgs.effect=CGEFFECT_SEPIA;
  loadcgprograms();
 }
 else
 if (k==K_EXT7)
 {
  cgs.effect=CGEFFECT_8COLORS;
  loadcgprograms();
 }
 else
 if (k==K_EXT8)
 {
  cgs.effect=CGEFFECT_16COLORS;
  loadcgprograms();
 }
#endif
 else
 if (k==K_Q)
 {
  geps+=(int)dir;
  if (geps<0) geps=0;
  else
  if (geps>6) geps=6;
 }
 else
 if (k==K_E)
 {
  if (envmapon>=0)
  envmapon=1-envmapon;
 }
 if (k==K_R)
 {
  if (gamemodeon==0)
  repmode=1-repmode;
 }
 else
 if (k==K_Z)
 {
  titles=1-titles;
 }
 else
 if (k==K_C)
 {
  gridon=1-gridon;
 }
 else
 if (k==K_G)
 {
  twosidelighting=1-twosidelighting;
#ifdef OPENGL
  gl_update();
#endif
 }
 else
 if ((k==K_H)&&(Is3DMode()))
 {
  if (hold) hold=0; else hold=-1;
 }
 else
 if (k==K_J)
 {
  modlvec=1-modlvec;
 }
 else
 if (k==K_K)
 {
  holdlight=1-holdlight;
  if (hold) hold=-1;
 }
 else
 if (k==K_L)
 {
  if (dir==1)
  lightdist*=SCALEDELTA;
  else
  lightdist/=SCALEDELTA;
  if (lightdist<D) lightdist=D;
  if (hold) hold=-1;
 }
 else
 if (k==K_X)
 {
  axison=1-axison;
 }
 else
 if ((k==K_N)&&(hold==0)&&(Is3DMode()))
 {
  if (dir==1)
  D/=2;
  else
  D*=2;
  if (R<4*D) D/=2;
  if (D<DEFAULTD/256) D=DEFAULTD/256;
  reqredraw=2;
 }
 else
 if ((k==K_M)&&(hold==0)&&(Is3DMode()))
 {
  R+=D*dir;
  if (R<4*D) R=4*D;
  reqredraw=2;
 }
 else
 if (k==K_V)
 {
  vangle+=dir*(3.141592f/180.f);
  if (vangle<(3.141592f/180.f)*5) vangle=(3.141592f/180.f)*5;
  if (vangle>(3.141592f/180.f)*170) vangle=(3.141592f/180.f)*170;
  setzdist();
  reqredraw=2;
#ifdef OPENGL
  gl_update();
#endif  
 }
 else
 if (k==K_B)
 {
  static int prm;
  if (gamemodeon) 
  {
   gamemodeon=0;
   repmode=prm;
  }
  else
  {
   gamemodeon=1;
   gms.time=CSys::GetTime();
   prm=repmode;
   repmode=1;
  }
 }
 else
 if (k==K_SLASH)
 {
  if (physicsmodel==PMODEL_SIMPLE) physicsmodel=PMODEL_ACCURATE;
  else
  physicsmodel=PMODEL_SIMPLE;
 }
 else
 if (k==K_BACKSLASH)
 {
  fpsmode=1-fpsmode;
  if (fpsmode==1)
  {
   frames=0;
   timer0=CSys::GetTime();
  }
 }
 else
 if (k==K_SPACE)
 {
  if (gamemodeon) gms.moveplayer|=2;
 }
 else
 if ((k==K_0)&&(dfuncstruct[CSys::DColor].status!=0))
 {
  float ey=expr_y;
  expr_y=FJ(height-cursory,AxisY);
  cursorx=FP(Findx0(&dexprlist[CSys::DColor],FJ(cursorx,AxisX),FJ(width,AxisX)
  ,1.f/this->s[AxisX],0.5f*pow(10,-geps)),AxisX);
  CSys::CursorPosSet((int)cursorx,(int)cursory);
  expr_y=ey;
 }
 else
 if (k==K_W)
 {
  ChangePos3D(changepos3ddelta,A[AxisX],A[AxisZ]);
 }
 else
 if (k==K_S)
 {
  ChangePos3D(-changepos3ddelta,A[AxisX],A[AxisZ]);
 }
 else
 if (k==K_A)
 {
  ChangePos3D(changepos3ddelta,0,A[AxisZ]+3.141592/2);
 }
 else
 if (k==K_D)
 {
  ChangePos3D(changepos3ddelta,0,A[AxisZ]-3.141592/2);
 }
 else
 if (k==K_F)
 {
  fitscr=1-fitscr;
  if (fitscr)
  this->s[AxisY]/=(float)width/(float)height;
  else
  this->s[AxisY]*=(float)width/(float)height;
  updatepos();
 }
 else
 if (k==K_T)
 {
  if (dmethod==MTEX)
  dmethod=MFILL; else dmethod=MTEX;
#ifdef OPENGL
  gl_update();
#endif
  reqredraw=2;
  if (hold) hold=-1;
 }
 else
 if (k==K_U)
 {
  if (dmethod>MLINES)
  dmethod=MLINES; else dmethod=MFILL;
#ifdef OPENGL
  gl_update();
#endif
  reqredraw=2;
  if (hold) hold=-1;
 }
 else
 if (k==K_I)
 {
  lighting=1-lighting;
#ifdef OPENGL
  gl_update();
#endif
  reqredraw=2;
  if (hold) hold=-1;
 }
 if (k==K_P)
 {
  changepos3dmode=1-changepos3dmode;
 }
//sterowanie opengl 
#ifdef OPENGL
 else
 if (k==K_O)
 {
  gl_used=1-gl_used;
  if (gl_used)
  gl_update();
  reqredraw=2;
  if (hold) hold=-1;
 }
 else
 if (k==K_Y)
 {
  gl_cull=1-gl_cull;
  gl_update();
  reqredraw=2;
 }
#endif 
 
 //tryby
 for (int i=K_1; i<CGraph::MMAX+K_1; i++) if (k==i) CSys::SetDMode(i-K_1);
 
 //mysz
 int mk=CSys::MouseKey();
 if (mk)
 {
  if (Is3DMode())
  ChangePos3D(changepos3ddelta/2*((mk==2)? -1.f:1.f),A[AxisX],A[AxisZ]);
  else
  {
   ChangePos(POSDELTA*m*(2*(mk==1)-1)*(cursorx-width_div_2)/100,AxisX);
   ChangePos(POSDELTA*m*(2*(mk==1)-1)*(-cursory+height_div_2)/100,AxisY);
  }
 }

#ifdef MENU 
 //obsluz menu
 setmenustate();
#endif
}

#ifdef MENU
void CGraph::initmenu()
{
 CSys::Menu.AddItem(MENU_GRAPH,"---",0);
 CSys::Menu.AddItem(MENU_GRAPH,"Gr&id\tC",K_C*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH,"A&xis\tX",K_X*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH,"Tit&les\tZ",K_Z*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH,"&Fit to screen\tF",K_F*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH,"Auto &repeat mode\tR",K_R*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH,"Precision&++\tQ",K_Q*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH,"Precision&--\tShift+Q",32768+K_Q*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH,"Find X&0\t0",K_0*256+MENU_GRAPH);
 
 CSys::Menu.AddItem(MENU_GRAPH,"---",0);
 CSys::Menu.AddItem(MENU_GRAPH,"&3D options",MENU_GRAPH1);
 CSys::Menu.AddItem(MENU_GRAPH1,"&Fill mode\tU",K_U*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"L&ighting on\tI",K_I*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"&Modify light vec\tJ",K_J*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"&Fade on distance\tK",K_K*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"&Texturing\tT",K_T*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"&Environment map\tE",K_E*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"---",0);
 CSys::Menu.AddItem(MENU_GRAPH1,"Light distance&++\tL",K_L*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"Light distance&--\tShift+L",32768+K_L*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"&Cells++\tN",K_N*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"Ce&lls--\tShift+N",32768+K_N*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"&Radius++\tM",K_M*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"R&adius--\tShift+M",32768+K_M*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"&View angle++\tV",K_V*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"Vie&w angle--\tShift+V",32768+K_V*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"---",0);
 CSys::Menu.AddItem(MENU_GRAPH1,"&Hold mode\tH",K_H*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"Small ste&p\tP",K_P*256+MENU_GRAPH);
#ifdef OPENGL
 CSys::Menu.AddItem(MENU_GRAPH1,"---",0);
 CSys::Menu.AddItem(MENU_GRAPH1,"Re&nderer",MENU_GRAPH3);
 CSys::Menu.AddItem(MENU_GRAPH3,"&Open GL\tO",K_O*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH3,"&Software Renderer\tO",K_O*256+MENU_GRAPH+1);
#ifdef CG
 CSys::Menu.AddItem(MENU_GRAPH1,"---",0);
 CSys::Menu.AddItem(MENU_GRAPH1,"&Shaders",MENU_GRAPH5);
 CSys::Menu.AddItem(MENU_GRAPH5,"Enabled\tShift+~",32768+K_TILDE*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH5,"---",0);
 CSys::Menu.AddItem(MENU_GRAPH5,"&Phong shading\t~",K_EXT0*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH5,"Phong shading with &bumpmap\t~",
 K_EXT1*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH5,"&Environment mapping\t~",K_EXT2*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH5,"E&nvironment mapping with bumpmap\t~",
 K_EXT3*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH1,"E&ffects",MENU_GRAPH6);
 CSys::Menu.AddItem(MENU_GRAPH6,"&Disabled",K_EXT4*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH6,"&Negative",K_EXT5*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH6,"&Sepia",K_EXT6*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH6,"&8 colors",K_EXT7*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH6,"&16 colors",K_EXT8*256+MENU_GRAPH);
#endif
#endif
 
 CSys::Menu.AddItem(MENU_GRAPH,"---",0);
 CSys::Menu.AddItem(MENU_GRAPH,"&Parametic function options",MENU_GRAPH2);
 CSys::Menu.AddItem(MENU_GRAPH2,"P&oints++\t=",K_EQU*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH2,"Po&ints--\t-",K_MINUS*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH2,"T1&++\t]",K_RSBRACKET*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH2,"T1&--\t[",K_LSBRACKET*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH2,"&T2++\t'",K_APOSTROPHE*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH2,"T&2--\t;",K_SEMICOLON*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH,"---",0);
 CSys::Menu.AddItem(MENU_GRAPH,"&Defaults\tHome",K_HOME*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH,"---",0);
 CSys::Menu.AddItem(MENU_GRAPH,"&Game mode\tB",K_B*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH,"P&hysics model",MENU_GRAPH4);
 CSys::Menu.AddItem(MENU_GRAPH4,"&Simple\t/",K_SLASH*256+MENU_GRAPH);
 CSys::Menu.AddItem(MENU_GRAPH4,"&Accurate\t/",K_SLASH*256+MENU_GRAPH+1);
 
 setmenustate();
 
 setmenuactivestate(0);
}

void CGraph::setmenustate()
{
 CSys::Menu.SetItemInfo(K_C*256+MENU_GRAPH,MENU_CH(gridon));
 CSys::Menu.SetItemInfo(K_X*256+MENU_GRAPH,MENU_CH(axison));
 CSys::Menu.SetItemInfo(K_Z*256+MENU_GRAPH,MENU_CH(titles));
 CSys::Menu.SetItemInfo(K_F*256+MENU_GRAPH,MENU_CH(!fitscr));
 CSys::Menu.SetItemInfo(K_R*256+MENU_GRAPH,MENU_CH(repmode));
 
 CSys::Menu.SetItemInfo(K_U*256+MENU_GRAPH,MENU_CH((dmethod==MFILL)||(dmethod==MTEX)));
 CSys::Menu.SetItemInfo(K_I*256+MENU_GRAPH,MENU_CH(lighting));
 CSys::Menu.SetItemInfo(K_J*256+MENU_GRAPH,MENU_CH(modlvec));
 CSys::Menu.SetItemInfo(K_K*256+MENU_GRAPH,MENU_CH(!holdlight));
 CSys::Menu.SetItemInfo(K_T*256+MENU_GRAPH,MENU_CH(dmethod==MTEX));
 CSys::Menu.SetItemInfo(K_E*256+MENU_GRAPH,MENU_CH(envmapon));
 CSys::Menu.SetItemInfo(K_H*256+MENU_GRAPH,MENU_CH(hold));
 CSys::Menu.SetItemInfo(K_P*256+MENU_GRAPH,MENU_CH(changepos3dmode));
 CSys::Menu.SetItemInfo(K_B*256+MENU_GRAPH,MENU_CH(gamemodeon));
 CSys::Menu.SetItemInfo(K_SLASH*256+MENU_GRAPH,MENU_CH(physicsmodel==PMODEL_SIMPLE));
 CSys::Menu.SetItemInfo(K_SLASH*256+MENU_GRAPH+1,MENU_CH(physicsmodel==PMODEL_ACCURATE));
#ifdef OPENGL
 CSys::Menu.SetItemInfo(K_O*256+MENU_GRAPH,MENU_CH(gl_used));
 CSys::Menu.SetItemInfo(K_O*256+MENU_GRAPH+1,MENU_CH(!gl_used));
#ifdef CG
 CSys::Menu.SetItemInfo(32768+K_TILDE*256+MENU_GRAPH,MENU_CH(UseShaders()));
 CSys::Menu.SetItemInfo(K_EXT0*256+MENU_GRAPH,
 MENU_CH(cgs.currentprogram==CGPROGRAM_DEFAULT));
 CSys::Menu.SetItemInfo(K_EXT1*256+MENU_GRAPH,
 MENU_CH(cgs.currentprogram==CGPROGRAM_BM));
 CSys::Menu.SetItemInfo(K_EXT2*256+MENU_GRAPH,
 MENU_CH(cgs.currentprogram==CGPROGRAM_ENV));
 CSys::Menu.SetItemInfo(K_EXT3*256+MENU_GRAPH,
 MENU_CH(cgs.currentprogram==CGPROGRAM_BMENV));
 
 CSys::Menu.SetItemInfo(K_EXT4*256+MENU_GRAPH,
 MENU_CH(cgs.effect==CGEFFECT_DEFAULT));
 CSys::Menu.SetItemInfo(K_EXT5*256+MENU_GRAPH,
 MENU_CH(cgs.effect==CGEFFECT_NEGATIVE));
 CSys::Menu.SetItemInfo(K_EXT6*256+MENU_GRAPH,
 MENU_CH(cgs.effect==CGEFFECT_SEPIA));
 CSys::Menu.SetItemInfo(K_EXT7*256+MENU_GRAPH,
 MENU_CH(cgs.effect==CGEFFECT_8COLORS));
 CSys::Menu.SetItemInfo(K_EXT8*256+MENU_GRAPH,
 MENU_CH(cgs.effect==CGEFFECT_16COLORS));
#endif
#endif
}

void CGraph::setmenuactivestate(int state)
{
  //CSys::Menu.SetItemInfo(MENU_GRAPH,MENU_ONOFF(state));
  //CSys::Menu.SetItemInfo(MENU_GRAPH_DMODE,MENU_ONOFF(state));
  CSys::Menu.SetItemInfo(K_C*256+MENU_GRAPH,MENU_ONOFF(state));
  CSys::Menu.SetItemInfo(K_X*256+MENU_GRAPH,MENU_ONOFF(state));
  CSys::Menu.SetItemInfo(K_Z*256+MENU_GRAPH,MENU_ONOFF(state));
  CSys::Menu.SetItemInfo(K_F*256+MENU_GRAPH,MENU_ONOFF(state));
  CSys::Menu.SetItemInfo(K_R*256+MENU_GRAPH,MENU_ONOFF(state));
  CSys::Menu.SetItemInfo(K_Q*256+MENU_GRAPH,MENU_ONOFF(state));
  CSys::Menu.SetItemInfo(32768+K_Q*256+MENU_GRAPH,MENU_ONOFF(state));
  CSys::Menu.SetItemInfo(K_0*256+MENU_GRAPH,MENU_ONOFF(state));
  
  CSys::Menu.SetItemInfo(MENU_GRAPH1,MENU_ONOFF(state));
  CSys::Menu.SetItemInfo(MENU_GRAPH2,MENU_ONOFF(state));
  CSys::Menu.SetItemInfo(MENU_GRAPH3,MENU_ONOFF(state));
  CSys::Menu.SetItemInfo(MENU_GRAPH4,MENU_ONOFF(state));
  
  CSys::Menu.SetItemInfo(K_HOME*256+MENU_GRAPH,MENU_ONOFF(state));
  CSys::Menu.SetItemInfo(K_B*256+MENU_GRAPH,MENU_ONOFF(state));

#ifdef CG
  CSys::Menu.SetItemInfo(MENU_GRAPH5,MENU_ONOFF(state&(useshaders>=0)));
  CSys::Menu.SetItemInfo(MENU_GRAPH6,MENU_ONOFF(state&(useshaders>=0)));
#endif
}
#endif

void CGraph::updatepos()
{
 cj[AxisX]=j[AxisX]-p[AxisX]/2/s[AxisX];
 cj[AxisY]=j[AxisY]-p[AxisY]/2/s[AxisY];
 cj[AxisZ]=j[AxisZ]-p[AxisZ]/2/s[AxisZ];
 cp[AxisX]=p[AxisX]/2-s[AxisX]*j[AxisX];
 cp[AxisY]=p[AxisY]/2-s[AxisY]*j[AxisY];
 cp[AxisZ]=p[AxisZ]/2-s[AxisZ]*j[AxisZ];
}
 
void CGraph::defaults()
{
 j[AxisX]=0;
 j[AxisY]=0;
 j[AxisZ]=0;
 s[AxisX]=p[AxisX]/DEFAULT_JWIDTH;
 if (fitscr)
 s[AxisY]=p[AxisY]/DEFAULT_JWIDTH;
 else
 s[AxisY]=p[AxisY]/DEFAULT_JWIDTH*(float)width/(float)height;
 s[AxisZ]=p[AxisZ]/DEFAULT_JWIDTH;
 A[AxisX]=0;
 A[AxisY]=0; //nie uzywane
 A[AxisZ]=0;
 t1=-5;
 t2=5;
 nt=1000;
 light_vec[0]=0.f;
 light_vec[1]=1.f;
 light_vec[2]=0.f;
 R=DEFAULTR;
 modlvec=0;
 hold=0;
 twosidelighting=0;
 gamemodeon=0;
 memset(&gms.player_vel,0,sizeof(VEC));
 memset(&gms.player_accv,0,sizeof(VEC2));
 D=DEFAULTD;
 int cx,cy;
 CSys::CursorPos(cx,cy);
 cursorx=cx;
 cursory=cy;
 updatepos();
}

CGraph::CGraph(int w,int h,void *b):CWin(w,h,b)
{
 p[AxisX]=width;
 p[AxisY]=height;
 width_div_2=width>>1;
 height_div_2=height>>1;
 vangle=DEFAULTVANLGE;
 setzdist();
 N=0;
 valtab=0;
 fitscr=0;
 changepos3dmode=0;
 defaults();
 
 memset(dexprlist,0,sizeof(dexprlist));
 memset(dfuncstruct,0,sizeof(dfuncstruct));
 dexprlist[MAXFUNCCOUNT]=CExpr(CExpr::GetLib());
 dexprlist[MAXFUNCCOUNT+1]=CExpr(CExpr::GetLib());
 valtab=0;
 projecttab=0;
 normaltab=0;
 colortab=0;
 texcoordtab=0;
 zbuf=0;
#ifdef OPENGL
 gl_colortab=0;
 gl_vertextab=0;
 gl_indextab=0;

 gl_light_ambient[0]=0.0f;
 gl_light_ambient[1]=0.0f;
 gl_light_ambient[2]=0.0f;
 gl_light_ambient[3]=1.f;
 gl_light_diffuse[0]=1.f;
 gl_light_diffuse[1]=1.f;
 gl_light_diffuse[2]=1.f;
 gl_light_diffuse[3]=1.f;
 gl_light_specular[0]=0.5f;
 gl_light_specular[1]=0.5f;
 gl_light_specular[2]=0.5f;
 gl_light_specular[3]=1.f;
 gl_light_emission[0]=0.0f;
 gl_light_emission[1]=0.0f;
 gl_light_emission[2]=0.0f;
 gl_light_emission[3]=1.f;
 gl_shininess=120.f;
 gl_used=1;
 CSys::AddVar("gl_light_ambient",&gl_light_ambient,VAR_FLOAT);
 CSys::AddVar("gl_light_diffuse",&gl_light_diffuse,VAR_FLOAT);
 CSys::AddVar("gl_light_specular",&gl_light_specular,VAR_FLOAT);
 CSys::AddVar("gl_shininess",&gl_shininess,VAR_FLOAT);
 
#ifdef CG
 initshaders();
#endif
#endif

 reqredraw=1;
 repmode=0;
 titles=1;
 dmethod=MLINES;
 lighting=0;
 gridon=0;
 axison=1;
 dstackl=0;
 geps=3;
 fpsmode=0;
 light_vec[3]=0.f;
 lightdist=DEFAULTR*4;
 zbuf=(unsigned int*)CSys::AllocMem(width*height*sizeof(unsigned int));
 for (int i=0; i<MAXFUNCCOUNT; i++) tex[i].Load(0);
 for (int i=0; i<6; i++) {envmap[i].SetAsCubeMapTex(i,CUBEMAPTEXID); envmap[i].Load(0);}
 
 //dla trybu gry standardowe ustawienia
 gms.grav=9.81f;
 gms.friction=0.25;
 gms.player_height=1.8f;
 gms.player_mass=70.f*1000.f;
 gms.player_maxvel=8.f;
 gms.player_acc=15.f;
 gms.player_jumpvel=5.f;
 physicsmodel=PMODEL_ACCURATE; //model dokladny
 CSys::AddVar("player_maxvel",&gms.player_maxvel,VAR_FLOAT);
 CSys::AddVar("player_height",&gms.player_height,VAR_FLOAT);
 CSys::AddVar("player_acc",&gms.player_acc,VAR_FLOAT);
 CSys::AddVar("player_jumpvel",&gms.player_jumpvel,VAR_FLOAT);
 CSys::AddVar("gravity",&gms.grav,VAR_FLOAT);
 CSys::AddVar("friction",&gms.friction,VAR_FLOAT);
 CSys::ExecCfg("game");

 //zmienne trybu wykresu
 CSys::AddVar("graphcolor",&Color,VAR_RGB);
 CSys::AddVar("lightvec",&light_vec,VAR_FLOAT);
 envmapfile[0]=0;
 CSys::AddVar("envmap",envmapfile,VAR_STR);
 
 CSys::AddVar("pos",&j,VAR_FLOAT);
 CSys::AddVar("angle",&A,VAR_FLOAT);
 
 CSys::ExecCfg("graph");
 CSys::DelVar("graphcolor");
#ifdef MENU
 initmenu();
#endif
  
#ifdef OPENGL
 gl_defaults();
#endif

 genpalettes();
 
 holdlight=0;
 envmapon=0;
 if (envmapfile[0]!=0)
 {
  if (loadenvmap(envmapfile)) 
  {
   envmapon=1;
   lightdist=255;
   holdlight=1;
  }
 }
}

CGraph::~CGraph()
{
 CSys::FreeMem(zbuf);
 CSys::FreeMem(valtab);
 CSys::FreeMem(projecttab);
 CSys::FreeMem(normaltab);
 CSys::FreeMem(colortab);
 CSys::FreeMem(texcoordtab);
#ifdef OPENGL   
 CSys::FreeMem(gl_colortab);
 CSys::FreeMem(gl_vertextab);
 CSys::FreeMem(gl_indextab);
#endif
}

void CGraph::Change(int w,int h,void *b)
{
 width=w;
 height=h;
 CSys::FreeMem(zbuf);
 zbuf=(unsigned int*)CSys::AllocMem(width*height*sizeof(unsigned int));
 width_div_2=width>>1;
 height_div_2=height>>1;
 buf=(unsigned int*)b;
 p[AxisX]=width;
 p[AxisY]=height;
 setzdist();
 s[AxisX]=p[AxisX]/DEFAULT_JWIDTH;
 if (fitscr)
 s[AxisY]=p[AxisY]/DEFAULT_JWIDTH;
 else
 s[AxisY]=p[AxisY]/DEFAULT_JWIDTH*(float)width/(float)height;
 s[AxisZ]=p[AxisZ]/DEFAULT_JWIDTH;
 reqredraw=3;
 updatepos();
#ifdef OPENGL
 gl_update();
#ifdef CG
 initshaders();
#endif
#endif
}

void CGraph::ChangeActiveState(int state)
{
 if (state==0) enddrawfunc();
 else
 reqredraw=3;
#ifdef MENU
 setmenuactivestate(state);
#endif
}

//dla trybu gry

float CGraph::getsurfz(float x,float y,float &anglex,float &angley)
{
 float x0=floor(x/D)*D,y0=ceil(y/D)*D;
 int tab=-N*iceil((y0-cy1)/D)+ifloor((x0-cx1)/D)+N;
 if ((tab<0)||(tab>=N*N)) return 0;
 
 int trno;
 
 //do ktorego trojkata nalezy punkt
 trno=((x0*(y0-D)+x*y0+(x0+D)*y-x*(y0-D)-x0*y-(x0+D)*y0)>0)? 1:2;
 float x1,y1,z1,x2,y2,z2,x3,y3,z3;
 
#ifdef OPENGL
 if (gl_used)
 {
  if (trno==1)
  {
   x2=x0;
   y2=y0;
   z2=((VEC*)(gl_vertextab+tab))->b;
   x1=x0+D;
   y1=y0;
   z1=((VEC*)(gl_vertextab+tab+1))->b;
   x3=x0+D;
   y3=y0-D;
   z3=((VEC*)(gl_vertextab+tab+N+1))->b;
  }
  else
  {
   x1=x0;
   y1=y0-D;
   z1=((VEC*)(gl_vertextab+tab+N))->b;
   x2=x0+D;
   y2=y0-D;
   z2=((VEC*)(gl_vertextab+tab+N+1))->b;
   x3=x0;
   y3=y0;
   z3=((VEC*)(gl_vertextab+tab))->b;
  }
 }
 else
#endif
 {
  if (trno==1)
  {
   x2=x0;
   y2=y0;
   z2=*(float*)(valtab+tab);
   x1=x0+D;
   y1=y0;
   z1=*(float*)(valtab+tab+1);
   x3=x0+D;
   y3=y0-D;
   z3=*(float*)(valtab+tab+N+1);
  }
  else
  {
   x1=x0;
   y1=y0-D;
   z1=*(float*)(valtab+tab+N);
   x2=x0+D;
   y2=y0-D;
   z2=*(float*)(valtab+tab+N+1);
   x3=x0;
   y3=y0;
   z3=*(float*)(valtab+tab);
  }
 }
 float z12=z1+(x-x1)*(z1-z2)/(x1-x2);
 float z13=z1+(y-y1)*(z1-z3)/(y1-y3);
 float y23=(x-x2)*(y2-y3)/(x2-x3)+y2;
 float x23=(y-y2)*(x2-x3)/(y2-y3)+x2;
 float z23y=(y23-y2)*(z2-z3)/(y2-y3)+z2;
 float z23x=(x23-x2)*(z2-z3)/(x2-x3)+z2;
 if (y1==y23) y23+=0.001;
 float z123=(y-y1)*(z12-z23y)/(y1-y23)+z12;
 
 //okresc katy
 if (x1==x23) x23+=0.001;
 anglex=atan((z23x-z13)/(x1-x23));
 if (y1==y23) y23+=0.001;
 angley=atan((z23y-z12)/(y1-y23));
 
 return z123;
}

void CGraph::updateplayer()
{
 //ustal zmiane czasu
 float currtime=CSys::GetTime();
 float deltatime=currtime-gms.time;
 //DEBUG_Printf("%f\n",deltatime);
 gms.time=currtime;
 int jump=gms.moveplayer&2;
 
 //modyfikuj wektor predkosci
 float vx,vy;
 float dv0=sqrt(gms.player_vel.a*gms.player_vel.a+gms.player_vel.b*gms.player_vel.b);
 vx=gms.player_vel.a+gms.player_accv.u*deltatime;
 vy=gms.player_vel.b+gms.player_accv.v*deltatime;
 float dv=sqrt(vx*vx+vy*vy);
 /*if (dv>gms.player_maxvel)
 {
  gms.player_vel.a*=gms.player_maxvel/dv;
  gms.player_vel.b*=gms.player_maxvel/dv;
 }*/
 
 if ((dv<gms.player_maxvel)||(dv0>dv))
 {
  gms.player_vel.a=vx;
  gms.player_vel.b=vy;
 }
 
 if (physicsmodel==PMODEL_SIMPLE)
 {
  if (dv>0)
  {
   gms.player_accv.u=-gms.player_vel.a/dv*gms.player_acc;
   gms.player_accv.v=-gms.player_vel.b/dv*gms.player_acc;
  }
  if ((gms.moveplayer==0)&&(dv<gms.player_maxvel*0.05))
  {
   gms.player_vel.a=0;
   gms.player_vel.b=0;
   gms.player_accv.u=0;
   gms.player_accv.v=0;
  }
 }
 else
 {
  gms.player_accv.u=0;
  gms.player_accv.v=0;
 }
 
 gms.player_vel.c-=gms.grav*deltatime;
 
 //przesun sie o wektor predkosci
 j[AxisX]+=gms.player_vel.a*deltatime;
 j[AxisY]+=gms.player_vel.b*deltatime;
 j[AxisZ]+=gms.player_vel.c*deltatime;
 updatepos();
 
 //sprawdz kolizje
 float x=j[AxisX],y=j[AxisY],z=j[AxisZ]-gms.player_height;
 if ((x<cx1)||(x>cx2)||(y>cy1)||(y<cy2)) return;
 
 float anglex,angley;
 float z123=getsurfz(x,y,anglex,angley);
 
 //przeciecie w (x,y,z123)
 if (z<z123)
 {
  j[AxisZ]=z123+gms.player_height;
  if (jump) 
  {
   gms.player_vel.c=gms.player_jumpvel;
  }
  else
  if (physicsmodel==PMODEL_ACCURATE)
  {
   
   //rozloz wektor predkosci
   dv=sqrt(gms.player_vel.a*gms.player_vel.a+gms.player_vel.c*gms.player_vel.c);
   float vzx=fabs(dv*sin(anglex));
   dv=sqrt(gms.player_vel.b*gms.player_vel.b+gms.player_vel.c*gms.player_vel.c);
   float vzy=fabs(dv*sin(angley));
   gms.player_vel.c=-sqrt(vzx*vzx+vzy*vzy);
  
   /*float dvx=sqrt(gms.player_vel.a*gms.player_vel.a+gms.player_vel.c*gms.player_vel.c);
   float vanglex=acos(gms.player_vel.a/dvx);
   float vpx=dvx*cos(vanglex-anglex);
   gms.player_vel.a=vpx*cos(anglex);
   
   float dvy=sqrt(gms.player_vel.b*gms.player_vel.b+gms.player_vel.c*gms.player_vel.c);
   float vangley=acos(gms.player_vel.b/dvy);
   float vpy=dvy*cos(vangley-angley);
   gms.player_vel.b=vpy*cos(angley);
   
   float ax2,ay2;
   float z123_2=getsurfz(x+gms.player_vel.a,y+gms.player_vel.b,ax2,ay2);
   float dvxy=sqrt(gms.player_vel.a*gms.player_vel.a+gms.player_vel.b*gms.player_vel.b);
   float dvxyz=sqrt(gms.player_vel.a*gms.player_vel.a+gms.player_vel.b*gms.player_vel.b
   +gms.player_vel.c*gms.player_vel.c);
   if (dvxy==0) gms.player_vel.c=0;
   else
   {
    float anglez=asin((z123_2-z123)/
    sqrt(gms.player_vel.a*gms.player_vel.a+gms.player_vel.b*gms.player_vel.b+
    (z123_2-z123)*(z123_2-z123)));
    float vanglez=asin(gms.player_vel.c/dvxyz);
    float vpz=dvxyz*cos(vanglez-anglez);
    gms.player_vel.c=vpz*sin(anglez);
   }*/
   
   //gms.player_vel.c=-(vpx*sin(anglex)+vpy*sin(angley));
   //DEBUG_Printf("%f %f %f %f\n",gms.player_vel.a,gms.player_vel.b,gms.player_vel.c,
   //sin(anglex)*cos(vanglex-anglex)+sin(angley)*cos(vangley-angley));
   
   
   //okresl wektor sily
   float F=gms.grav*gms.player_mass;
   
   float F1x=F*sin(anglex);
   float F2x=F*cos(anglex);
   float Ffx=F2x*gms.friction;
   //F1x=(fabs(F1x)-fabs(FTx)>0)? F1x-FTx:0;
   
   float F1y=F*sin(angley);
   float F2y=F*cos(angley);
   float Ffy=F2y*gms.friction;
   //F1y=(fabs(F1y)-fabs(FTy)>0)? F1y-FTy:0;
   
   float Fx=F1x*cos(anglex);
   float Fy=F1y*cos(angley);
   
   //DEBUG_Printf("ax %f ay %f| x %f %f %f| y %f %f %f| Fx %f Fy %f| %f %f \n",
   //anglex,angley,F1x,F2x,Ffx,F1y,F2y,Ffy,
   //Fx/gms.player_mass*deltatime,Fy/gms.player_mass*deltatime,sin(anglex),cos(anglex));
   
   gms.player_vel.a+=Fx/gms.player_mass*deltatime;
   gms.player_vel.b+=Fy/gms.player_mass*deltatime;
   
   //ustal tarcie
   dv=sqrt(gms.player_vel.a*gms.player_vel.a+gms.player_vel.b*gms.player_vel.b);
   if (dv>0)
   {
    Ffx*=-gms.player_vel.a/dv;
    Ffy*=-gms.player_vel.b/dv;
    Ffx*=cos(anglex);
    Ffy*=cos(angley);
    float vx=gms.player_vel.a,vy=gms.player_vel.b;
    gms.player_vel.a+=Ffx/gms.player_mass*deltatime;
    gms.player_vel.b+=Ffy/gms.player_mass*deltatime;
    if (vx*gms.player_vel.a<0) gms.player_vel.a=0;
    if (vy*gms.player_vel.b<0) gms.player_vel.b=0;
   }
   else
   {
    gms.player_vel.c=0;
   }
  }
  else
  {
   gms.player_vel.c=0;
  }
 }
}

//znajduje m. zerowe funkcji z dokladnoscia eps
float CGraph::Findx0(CExpr *expr,float a,float b,float d,float eps)
{
 float x1=a;
 float x2=a+d;
 int i=10000;
 while (x1<=b)
 {
  expr_x=x1;
  float y1=expr->Do();
  expr_x=x2;
  float y2=expr->Do();
  if (fabs(y1)<=eps) return x1;
  if (y1*y2<0)
  //szukaj metoda polowienia przedzialow miejsce zerowe
  {
   float s=(y1>0)? 1:-1;
   float y;
   do
   {
    expr_x=0.5f*(x1+x2);
    y=expr->Do();
    if (y*s>0) x1=expr_x; else x2=expr_x;
    i--;
   } while ((fabs(y)>eps)&&(i>0));
   return expr_x;
  }
  x1=x2;
  x2+=d;
 }
 return 0;
}

void CGraph::genpalettes()
{
 int i,j;
 for (i=0; i<MAXFUNCCOUNT; i++)
  for (j=0; j<256; j++) Palette[i][j]=CSys::FadeColor(Color[i],j);
}

int CGraph::drawfunc_K2DF1(CExpr *expr,FUNCSTRUCT *f)
{
 unsigned int color=Color[f->color];
 int i;
 float x1,y1,x2,y2;
 f->status=2;
 
 x1=FJ(0,AxisX);
 for (i=1; i<width; i++)
 {
  x2=FJ((float)i,AxisX);
  expr_x=x1;
  y1=height-FP(expr->Do(),AxisY)+0.5;
  expr_x=x2;
  y2=height-FP(expr->Do(),AxisY)+0.5;
  if (!(IS_UD(y1)&&IS_UD(y2)))
  {
   if (IS_UD(y1)) y1=height-FP(0,AxisY);
   else
   if (IS_INFM(y1)) y1=-1000000;
   else
   if (IS_INFP(y1)) y1=1000000;
   if (IS_UD(y2)) y2=height-FP(0,AxisY);
   else
   if (IS_INFM(y2)) y2=-1000000;
   else
   if (IS_INFP(y2)) y2=1000000;
   if ((fabs(y1-y2)<1000)||((fabs(y1-y2)<10000)&&(y1*y2>0)))
   {
    VLine(i-1,(int)y1,(int)((y1+y2)/2),color);
    VLine(i,(int)((y1+y2)/2),(int)y2,color);
   }
  }
  x1=x2;
 }
 return 1;
}

int CGraph::drawfunc_K2DXY(CExpr *expr,FUNCSTRUCT *f)
{
 unsigned int *palwsk=Palette[f->color];
 float x1,y1,x2,y2;
 if (f->status==1)
 {
  f->status=2;
  expr_t=t1;
 }
 float dt=(t2-t1)/(float)(nt-1);
 if (fabs(dt)<=0.000001f) return 1;
 expr->Do();
 x1=expr_x;
 y1=expr_y;
 if (IS_UD(x1)) x1=0;
 else
 if (IS_INFM(x1)) x1=-10000;
 else
 if (IS_INFP(x1)) x1=10000;
  
 if (IS_UD(y1)) y1=0;
 else
 if (IS_INFM(y1)) y1=-10000;
 else
 if (IS_INFP(y1)) y1=10000;
 expr_t+=dt;
 
 int i=0;
 
 while (expr_t<=t2)
 {
  float w=expr->Do();
  unsigned int color;
  if (w!=0)
  {
   color=palwsk[ifloor(w*255)&255];
  }
  else goto no_draw;
  x2=expr_x;
  y2=expr_y;
  
  if (IS_UD(x2)) x2=0;
  else
  if (IS_INFM(x2)) x2=-10000;
  else
  if (IS_INFP(x2)) x2=10000;
  
  if (IS_UD(y2)) y2=0;
  else
  if (IS_INFM(y2)) y2=-10000;
  else
  if (IS_INFP(y2)) y2=10000;
  
  Line(iround(FP(x1,AxisX)),iround(height-FP(y1,AxisY)),
       iround(FP(x2,AxisX)),iround(height-FP(y2,AxisY)),color);
no_draw:  
  x1=x2;
  y1=y2;
  expr_t+=dt;
  i++;
  if (i>64)
  {
   if (CSys::GetTime()-timer>MAXDRAWINGTIME) 
   {
    return 0;
   }
   i=0;
  }
 }
 expr_t=t2;
 float w=expr->Do();
 unsigned int color;
 if (w!=0)
 {
  color=palwsk[ifloor(w*255)&255];
 }
 else return 1;
 x2=expr_x;
 y2=expr_y;
  
 if (IS_UD(x2)) x2=0;
 else
 if (IS_INFM(x2)) x2=-10000;
 else
 if (IS_INFP(x2)) x2=10000;
  
 if (IS_UD(y2)) y2=0;
 else
 if (IS_INFM(y2)) y2=-10000;
 else
 if (IS_INFP(y2)) y2=10000;
  
 Line(iround(FP(x1,AxisX)),iround(height-FP(y1,AxisY)),
      iround(FP(x2,AxisX)),iround(height-FP(y2,AxisY)),color);
 return 1;
}

int CGraph::drawfunc_K2DF2(CExpr *expr,FUNCSTRUCT *f)
{
 int i;
 static int j,k;
 static unsigned int *bf;
 unsigned int *dbf;
 if (f->status==1)
 {
  bf=buf;
  j=0;
  k=0;
  f->status=2;
 }
 
 int d=height/DLINES;
 float dx=1.f/s[AxisX];
 float exprx0=FJ(0,AxisX);
 for (; j<d; j++)
 {
  for(; k<DLINES; k++)
  {
   if (CSys::GetTime()-timer>MAXDRAWINGTIME) 
   {
    return 0;
   }
   dbf=bf+k*d*width;
   expr_y=FJ(-(j+k*d)+height,AxisY);
   expr_x=exprx0;
   i=width;
   unsigned int *palwsk=Palette[f->color];
   while(i--)
   {
    register float w=expr->Do();
    if (w!=0)
    {
     *dbf=palwsk[ifloor(w*255)&255];
    }
    dbf++;
    expr_x+=dx;
   }
  }
  bf+=width;
  k=0;
 }
 return 1;
}


//funkcje dla 3D
void inline CGraph::rotate(float x,float y,float z,float &xr,float &yr,float &zr)
{
 //OZ
 xr=x*cosaz+y*sinaz;
 yr=-x*sinaz+y*cosaz;
 //OX
 y=yr;
 yr=y*cosax+z*sinax;
 zr=-y*sinax+z*cosax;
 
 //brak obslugi obrotu wokow osi OY dla przyspeszenia obliczen
 /*//OY
 x=xr;
 z=zr;
 xr=x*cosay+z*sinay;
 zr=z*cosay-x*sinay;*/
}

int inline CGraph::project(float x,float y,float z,int &xp,int &yp)
{
 if (z<MINPROJECTZ) return 0;
 xp=iround(x*p[AxisZ]/z)+width_div_2;
 yp=-iround(y*p[AxisZ]/z)+height_div_2;
 return 1;
}

void inline CGraph::sqcircorners(float x,float y,float &x1,float &y1,float &x2,float &y2)
{
 x1=floor((x-R)/D)*D;
 y1=ceil((y+R)/D)*D;
 x2=ceil((x+R)/D)*D;
 y2=floor((y-R)/D)*D;
}

float inline CGraph::dotproduct(VEC *a,VEC *b)
{
 return a->a*b->a+a->b*b->b+a->c*b->c;
}

//ogolna funkcja wykonujaca operacje z reffunc w okregu
int CGraph::circlefunc(CExpr *expr,FUNCSTRUCT *f,int state,
                       float rr,int tabf,int ladd,
                       void (CGraph::*reffunc)(float x,float y))
{
 static float y;
 static float rp;
 static int t;

 float R2=rr*rr;
 
 //poczatkowa inicjacja danych
 _255_lightdist=255.f/lightdist;
 circfunc_expr=expr;
 circfunc_D2=D*D;
 circfunc_D4=circfunc_D2*circfunc_D2;
 circfunc_1_2_D=D/2.f;
 if (f)
 {
  circfunc_tex=&tex[f->color];
 }
 
 int rtab[TPTR_SIZE];
 int nt=0;
 if (tabf&FVWSK) rtab[nt++]=vwsk;
 if (tabf&FPWSK) rtab[nt++]=pwsk;
 if (tabf&FNWSK) rtab[nt++]=nwsk;
 if (tabf&FCWSK) rtab[nt++]=cwsk;
 if (tabf&FTWSK)  rtab[nt++]=twsk;
#ifdef OPENGL
 if (tabf&FGL_CWSK) rtab[nt++]=gl_cwsk;
 if (tabf&FGL_VWSK) rtab[nt++]=gl_vwsk;
#endif

 t0ptr[vwsk]=(void*)(valtab+N);
 t0ptr[pwsk]=(void*)(projecttab+N);
 t0ptr[nwsk]=(void*)(normaltab+N);
 t0ptr[cwsk]=(void*)(colortab+N);
 t0ptr[twsk]=(void*)(texcoordtab+N);
#ifdef OPENGL
 t0ptr[gl_cwsk]=(void*)(gl_colortab+N);
 t0ptr[gl_vwsk]=(void*)(gl_vertextab+N);
#endif
 
 sptr[vwsk]=sizeof(float);
 sptr[pwsk]=sizeof(IPOINT);
 sptr[nwsk]=sizeof(VEC);
 sptr[cwsk]=sizeof(unsigned char);
 sptr[twsk]=sizeof(VEC2);
#ifdef OPENGL
 sptr[gl_cwsk]=sizeof(IVEC4);
 sptr[gl_vwsk]=sizeof(VEC);
#endif
 if (f) circfunc_palwsk=Palette[f->color];
 
 if (state==0)
 {
  y=ceil((hys+rr)/D)*D;
  rp=rr-D*DMUL;
  t=0;
 }
 
 for (; t<2; t++)
 {
 circfunc_dir=2*t-1;
 while ((t==0)? (y>=hys):(y<=hys))
 {
  if (CSys::GetTime()-timer>MAXDRAWINGTIME) 
  {
   return 0;
  }
  float x1,x2,sq;
  int sqint,tab;
  
  sq=sqrt(R2-rp*rp);
  x1=floor((-sq+hxs)/D)*D;
  x2=ceil((sq+hxs)/D)*D;
  tab=-N*iceil((y-cy1)/D)+ifloor((x1-cx1)/D);
  
  circfunc_disty=ifloor(ZBUFMUL*(y-ys));
  circfunc_disty*=circfunc_disty;
  
  for (int j=0; j<nt; j++)
  {
   tptr[rtab[j]]=(void*)((int)t0ptr[rtab[j]]+tab*sptr[rtab[j]]);
  }
  sqint=(int)((x2-x1)/D)+ladd;
  for (int i=0; i<sqint; i++)
  {
   (this->*reffunc)(x1,y);
   x1+=D;
   for (int j=0; j<nt; j++)
   {
    tptr[rtab[j]]=(void*)((int)tptr[rtab[j]]+sptr[rtab[j]]);
   }
  }
  y+=D*circfunc_dir;
  rp-=D;
 }
 rp=rr-D*DMUL;
 y=floor((hys-rr)/D)*D+(1-ladd)*D;
 }
 return 1;
}

void CGraph::compvaltabref(float x,float y)
{
 expr_x=x;
 expr_y=y;
 float register z=circfunc_expr->Do();
 if (IS_UD(z)) z=0;
 else
 if (IS_INFM(z)) z=-10000;
 else
 if (IS_INFP(z)) z=10000;
 *(float*)tptr[vwsk]=z;
}

//przeprowadz obliczenia dla ogladanego wykresu w 3D
int CGraph::compvaltab(CExpr *expr,FUNCSTRUCT *f)
{
 int status;
 if (f->status==1)
 {
  f->status=2;
  status=0;
 } else status=1;

 if (!circlefunc(expr,0,status,R,FVWSK,1,&CGraph::compvaltabref)) return 0;
 return 1;
}

//rzutuj wierzcholki do bufora w celu przyspieszenia obliczen
//dla softa

void CGraph::compprojecttabref(float x,float y)
{
 float z=*(float*)(tptr[vwsk])-zs;
 //DEBUG_Printf("%f %f %f\n",x,y,z);
 float xr,yr,zr;
 int xp,yp;
 rotate(x-xs,y-ys,z,xr,yr,zr);
 int v=project(xr,zr,yr,xp,yp);
 if (v)
 {
  ((IPOINT*)tptr[pwsk])->x=xp;
  ((IPOINT*)tptr[pwsk])->y=yp;
 }
 else
 {
  ((IPOINT*)tptr[pwsk])->x=UD;
 }
}

void CGraph::compprojecttab()
{
 int status=1;
 
 if (dstate==DS_PROJECT)
 {
  dstate++;
  status=0;
 }
 if (circlefunc(0,0,status,R,FVWSK|FPWSK,1,&CGraph::compprojecttabref)) dstate++;
}

void CGraph::compnormaltabref(float x,float y)
{
 VEC *normwsk=(VEC*)tptr[nwsk];
 float z1,z2,z3,z4;
 z1=*((float*)tptr[vwsk]-N);
 z2=*((float*)tptr[vwsk]+1);
 z3=*((float*)tptr[vwsk]+N);
 z4=*((float*)tptr[vwsk]-1);
   
 normwsk->a=circfunc_1_2_D*(z4-z2);
 normwsk->c=circfunc_1_2_D*(z1-z3);
   
 float d=1.f/sqrt(normwsk->a*normwsk->a+circfunc_D4+normwsk->c*normwsk->c);
 normwsk->a=normwsk->a*d;
 normwsk->b=circfunc_D2*d;
 normwsk->c=normwsk->c*d;
}

#ifdef OPENGL
void CGraph::gl_compnormaltabref(float x,float y)
{
 VEC *normwsk=(VEC*)tptr[nwsk];
 float z1,z2,z3,z4;
 z1=((VEC*)tptr[gl_vwsk]-N)->b;
 z2=((VEC*)tptr[gl_vwsk]+1)->b;
 z3=((VEC*)tptr[gl_vwsk]+N)->b;
 z4=((VEC*)tptr[gl_vwsk]-1)->b;
 /*float z1,z2,z3,z4,z5,z6,z7,z8;
 z1=((VEC*)tptr[gl_vwsk]-N)->b;
 z2=((VEC*)tptr[gl_vwsk]+1-N)->b;
 z3=((VEC*)tptr[gl_vwsk]+1)->b;
 z4=((VEC*)tptr[gl_vwsk]+1+N)->b;
 z5=((VEC*)tptr[gl_vwsk]+N)->b;
 z6=((VEC*)tptr[gl_vwsk]-1+N)->b;
 z7=((VEC*)tptr[gl_vwsk]-1)->b;
 z8=((VEC*)tptr[gl_vwsk]-1-N)->b;*/
   
 normwsk->a=circfunc_1_2_D*(z4-z2);
 normwsk->c=circfunc_1_2_D*(z1-z3);
 /*normwsk->a=D*(2*(z7-z3)+z6-z4-z2+z8)/8;
 normwsk->c=D*(2*(z5-z1)+z6+z4-z2-z8)/8;*/
   
 float d=1.f/sqrt(normwsk->a*normwsk->a+circfunc_D4+normwsk->c*normwsk->c);
 normwsk->a=normwsk->a*d;
 normwsk->b=circfunc_D2*d;
 normwsk->c=normwsk->c*d;
}
#endif

void CGraph::compnormaltab()
{
 if (hold>0) {dstate+=2; return;}
 int status;
 if (dstate==DS_NORMAL)
 {
  status=0;
  dstate++;
 } else status=1;
 
#ifdef OPENGL
 if (gl_used)
 {
  if (circlefunc(0,0,status,R-D,FGL_VWSK|FNWSK,1,&CGraph::gl_compnormaltabref)) dstate++;
 }
 else
#endif
 if (circlefunc(0,0,status,R-D,FVWSK|FNWSK,1,&CGraph::compnormaltabref)) dstate++;
}

void CGraph::compcoltabref(float x,float y)
{
 float z=*(float*)tptr[vwsk]-zs;
 x-=xs;   
 y-=ys;
 float d=sqrt(x*x+y*y+z*z);
 if (d>lightdist) *(unsigned char*)tptr[cwsk]=0;
 else
 *(unsigned char*)tptr[cwsk]=(unsigned char)ifloor(255.f-d*_255_lightdist);
}

//oblicz wartosci jasnosci kolorow dla rysowania trojkatow
void CGraph::compcoltab(int color)
{
 if (holdlight)
 {
  if (hold>0) {dstate+=2; return;}
  int c=iround(lightdist);
  if (c>255)c=255;
  memset(colortab,c,GRAPHTAB_SIZE(N));
  dstate+=2;
  return;
 }
 int status;
 if (dstate==DS_COL)
 {
  status=0;
  dstate++;
 } else status=1;
 if(circlefunc(0,0,status,R,FVWSK|FCWSK,1,&CGraph::compcoltabref)) dstate++;
}

void CGraph::compcoltabnormalref(float x,float y)
{
 float z=*(float*)tptr[vwsk]-zs;
 x-=xs;   
 y-=ys;
 float d=sqrt(x*x+y*y+z*z);
 if (d>lightdist) *(unsigned char*)tptr[cwsk]=0;
 else
 {
  int c=ifloor(dotproduct((VEC*)tptr[nwsk],(VEC*)&light_vec)*(255.f-d*_255_lightdist));
  if (c<0) c=0;
  *(unsigned char*)tptr[cwsk]=(unsigned char)c;
 }
}

void CGraph::compcoltabnormalcconstref(float x,float y)
{
 int c=ifloor(dotproduct((VEC*)tptr[nwsk],(VEC*)&light_vec)*circfunc_constcol);
 if (c<0) c=0;
 *(unsigned char*)tptr[cwsk]=(unsigned char)c;
}

//oblicz wartosci jasnosci kolorow dla rysowania trojkatow
void CGraph::compcoltabnormal(int color)
{
 int status;
 static VEC lightvec;
 float d=1/sqrt(light_vec[0]*light_vec[0]+light_vec[1]*light_vec[1]+light_vec[2]*light_vec[2]);
 memcpy(&lightvec,&light_vec,sizeof(VEC));
 light_vec[0]*=d;
 light_vec[1]*=d;
 light_vec[2]*=d;
  
 if (dstate==DS_COL)
 {
  status=0;
  dstate++;
 } else status=1;
 if (holdlight)
 {
  if (hold>0) {memcpy(&light_vec,&lightvec,sizeof(VEC)); dstate++; return;}
  circfunc_constcol=lightdist;
  if (circfunc_constcol>255) circfunc_constcol=255;
  if(circlefunc(0,0,status,R,FVWSK|FNWSK|FCWSK,1,&CGraph::compcoltabnormalcconstref)) dstate++;
  memcpy(&light_vec,&lightvec,sizeof(VEC));
  return;
 }
 if(circlefunc(0,0,status,R,FVWSK|FNWSK|FCWSK,1,&CGraph::compcoltabnormalref)) dstate++;
 memcpy(&light_vec,&lightvec,sizeof(VEC));
}

void CGraph::comptexcoordtabref(float x,float y)
{
 ((VEC2*)tptr[twsk])->u=circfunc_tex->GetU(x);
 ((VEC2*)tptr[twsk])->v=circfunc_tex->GetV(-y);
}

void CGraph::comptexcoordtab(FUNCSTRUCT *f)
{
 if (hold>0) {dstate+=2; return;}
 int status;
 if (dstate==DS_TEX)
 {
  status=0;
  dstate++;
 } else status=1;
 
 if (circlefunc(0,f,status,R,FTWSK,0,&CGraph::comptexcoordtabref)) dstate++;
}

int CGraph::drawfunc_K3DF2_soft_grid(FUNCSTRUCT *f)
{
 if (dstate<DS_PROJECT+2)
 {
  compprojecttab();
  if (dstate!=DS_PROJECT+2) return 0;
  dstate+=2;
 }
 if (dstate<DS_COL+2)
 {
  compcoltab(f->color);
  if (dstate!=DS_COL+2) return 0;
 }
 
 if (dstate<DS_TEX+2)
 {
  dstate+=2;
 }
 
 static float rp; //
 
 unsigned int *palwsk=Palette[f->color];
 float dx=D*cosaz,dy=D*sinaz;
 float DL;
 float R2;
 //faza 1 - rysowanie przodu
 R2=(R-D)*(R-D);
 DL=R-D*DMUL-D+0.001f;
 
 if (dstate==DS_DRAW)
 {
  rp=R-D*DMUL-D;
  dstate++;
 }
 float _1_D=1/D;
 while (fabs(rp)<=DL)
 {
  if (CSys::GetTime()-timer>MAXDRAWINGTIME) 
  {
   return 0;
  }
  float x1,y1,x2,y2,sq;
  int sqint;
  sq=sqrt(R2-rp*rp);
  x1=-sq*cosaz-rp*sinaz+hxs;
  y1=-sq*sinaz+rp*cosaz+hys;
  x2=sq*cosaz-rp*sinaz+hxs;
  y2=sq*sinaz+rp*cosaz+hys;
  sqint=(int)(expr_dist(x2-x1,y2-y1)*_1_D);
  for(int i=0; i<sqint; i++)
  {
   int tab=-N*iceil((y1-cy1)*_1_D)+ifloor((x1-cx1)*_1_D)+N;
   int xp1,yp1,xp2,yp2,xp3,yp3,xp4,yp4,col;
   
   IPOINT *pwsk=(IPOINT*)((int)projecttab+(tab*sizeof(IPOINT)));
   xp1=pwsk->x;
   if (xp1==UD) goto no_draw;
   xp2=(pwsk+1)->x;
   if (xp2==UD) goto no_draw;
   xp3=(pwsk+N+1)->x;
   if (xp3==UD) goto no_draw;
   xp4=(pwsk+N)->x;
   if (xp4==UD) goto no_draw;
   yp1=pwsk->y;
   yp2=(pwsk+1)->y;
   yp4=(pwsk+N)->y;
   yp3=(pwsk+N+1)->y;
   col=palwsk[*(unsigned char*)((int)colortab+(tab*sizeof(unsigned char)))];
   
   Line(xp1,yp1,xp2,yp2,col);
   Line(xp2,yp2,xp3,yp3,col);
   Line(xp3,yp3,xp4,yp4,col);
   Line(xp4,yp4,xp1,yp1,col);

no_draw:   
   x1+=dx;
   y1+=dy;
  }
  rp-=D;
 }
 return 1;
}

void CGraph::drawfunc_K3DF2_soft_fillref(float x,float y)
{
 IPOINT *prwsk=(IPOINT*)tptr[pwsk];
 unsigned char *colwsk=(unsigned char*)tptr[cwsk];
 int xp1,yp1,xp2,yp2,xp3,yp3,xp4,yp4;
 xp1=prwsk->x;
 if (xp1==UD) return;
 xp2=(prwsk+1)->x;
 if (xp2==UD) return;
 xp3=(prwsk+N+1)->x;
 if (xp3==UD) return;
 xp4=(prwsk+N)->x;
 if (xp4==UD) return;
 yp1=prwsk->y;
 yp2=(prwsk+1)->y;
 yp4=(prwsk+N)->y;
 yp3=(prwsk+N+1)->y;
 int xi=ifloor(ZBUFMUL*(x-xs));
 int zi=ifloor(ZBUFMUL*(*(float*)tptr[vwsk]-zs));
 unsigned int d=xi*xi+circfunc_disty+zi*zi;
 GTriangle_z(xp1,yp1,xp2,yp2,xp3,yp3,*colwsk,*(colwsk+1),*(colwsk+N+1),circfunc_palwsk,d,zbuf);
 GTriangle_z(xp1,yp1,xp3,yp3,xp4,yp4,*colwsk,*(colwsk+N+1),*(colwsk+N),circfunc_palwsk,d,zbuf);
}

int CGraph::drawfunc_K3DF2_soft_fill(FUNCSTRUCT *f)
{
 if (dstate<DS_PROJECT+2)
 {
  compprojecttab();
  if (dstate!=DS_PROJECT+2) return 0;
  dstate+=2;
 }
 if (dstate<DS_COL+2)
 {
  compcoltab(f->color);
  if (dstate!=DS_COL+2) return 0;
 }
 if (dstate<DS_TEX+2)
 {
  dstate+=2;
 }
 
 int status;
 if (dstate==DS_DRAW)
 {
  status=0;
  dstate++;
 } else status=1;
 if (!circlefunc(0,f,status,R-D,FVWSK|FPWSK|FCWSK,0,
 &CGraph::drawfunc_K3DF2_soft_fillref)) return 0;
 return 1;
}

int CGraph::drawfunc_K3DF2_soft_fill_light(FUNCSTRUCT *f)
{
 if (dstate<DS_PROJECT+2)
 {
  compprojecttab();
  if (dstate!=DS_PROJECT+2) return 0;
 }
 if (dstate<DS_NORMAL+2)
 {
  compnormaltab();
  if (dstate!=DS_NORMAL+2) return 0;
 }
 if (dstate<DS_COL+2)
 {
  compcoltabnormal(f->color);
  if (dstate!=DS_COL+2) return 0;
 }
 if (dstate<DS_TEX+2)
 {
  dstate+=2;
 }
 
 int status;
 if (dstate==DS_DRAW)
 {
  status=0;
  dstate++;
 } else status=1;
 if (!circlefunc(0,f,status,R-D,FVWSK|FPWSK|FCWSK,0,
 &CGraph::drawfunc_K3DF2_soft_fillref)) return 0;
 return 1;
}

void CGraph::drawfunc_K3DF2_soft_fill_texref(float x,float y)
{
 IPOINT *prwsk=(IPOINT*)tptr[pwsk];
 VEC2 *texwsk=(VEC2*)tptr[twsk];
 unsigned char *colwsk=(unsigned char*)tptr[cwsk];
 int xp1,yp1,xp2,yp2,xp3,yp3,xp4,yp4;
 xp1=prwsk->x;
 if (xp1==UD) return;
 xp2=(prwsk+1)->x;
 if (xp2==UD) return;
 xp3=(prwsk+N+1)->x;
 if (xp3==UD) return;
 xp4=(prwsk+N)->x;
 if (xp4==UD) return;
 yp1=prwsk->y;
 yp2=(prwsk+1)->y;
 yp4=(prwsk+N)->y;
 yp3=(prwsk+N+1)->y;
 int xi=ifloor(ZBUFMUL*(x-xs));
 int zi=ifloor(ZBUFMUL*(*(float*)tptr[vwsk]-zs));
 unsigned int d=xi*xi+circfunc_disty+zi*zi;

 TTriangle_z(xp1,yp1,xp2,yp2,xp3,yp3,
             texwsk->u,texwsk->v,
             (texwsk+1)->u,(texwsk+1)->v,
             (texwsk+N+1)->u,(texwsk+N+1)->v,
             circfunc_tex,
             *colwsk,*(colwsk+1),*(colwsk+N+1),
             d,zbuf);
 TTriangle_z(xp1,yp1,xp3,yp3,xp4,yp4,
             texwsk->u,texwsk->v,
             (texwsk+N+1)->u,(texwsk+N+1)->v,
             (texwsk+N)->u,(texwsk+N)->v,
             circfunc_tex,
             *colwsk,*(colwsk+N+1),*(colwsk+N),
             d,zbuf);
}

int CGraph::drawfunc_K3DF2_soft_fill_tex(FUNCSTRUCT *f)
{
 if (!tex[f->color].IsLoaded()) 
 {
  return drawfunc_K3DF2_soft_fill(f);
 }
 if (dstate<DS_PROJECT+2)
 {
  compprojecttab();
  if (dstate!=DS_PROJECT+2) return 0;
  dstate+=2;
 }
 if (dstate<DS_COL+2)
 {
  compcoltab(f->color);
  if (dstate!=DS_COL+2) return 0;
 }
 if (dstate<DS_TEX+2)
 {
  comptexcoordtab(f);
  if (dstate!=DS_TEX+2) return 0;
 }
 
 int status;
 if (dstate==DS_DRAW)
 {
  status=0;
  dstate++;
 } else status=1;
 if (!circlefunc(0,f,status,R-D,FVWSK|FPWSK|FCWSK|FTWSK,0,
 &CGraph::drawfunc_K3DF2_soft_fill_texref)) return 0;
 return 1;
}

int CGraph::drawfunc_K3DF2_soft_fill_tex_light(FUNCSTRUCT *f)
{
 if (!tex[f->color].IsLoaded()) 
 {
  return drawfunc_K3DF2_soft_fill_light(f);
 }
 if (dstate<DS_PROJECT+2)
 {
  compprojecttab();
  if (dstate!=DS_PROJECT+2) return 0;
 }
 if (dstate<DS_NORMAL+2)
 {
  compnormaltab();
  if (dstate!=DS_NORMAL+2) return 0;
 }
 if (dstate<DS_COL+2)
 {
  compcoltabnormal(f->color);
  if (dstate!=DS_COL+2) return 0;
 }
 if (dstate<DS_TEX+2)
 {
  comptexcoordtab(f);
  if (dstate!=DS_TEX+2) return 0;
 }
 
 int status;
 if (dstate==DS_DRAW)
 {
  status=0;
  dstate++;
 } else status=1;
 if (!circlefunc(0,f,status,R-D,FVWSK|FPWSK|FCWSK|FTWSK,0,
 &CGraph::drawfunc_K3DF2_soft_fill_texref)) return 0;
 return 1;
}


#ifdef OPENGL

#ifdef CG

void CGraph::initshaders()
{
 useshaders=-1;
 if (!LoadCGLib()) return;
 if (!LoadCGGLLib()) return;
 memset(&cgs,0,sizeof(cgs));
 if (!initcg()) return;
 else
 /*if (!loadcgprogram(CGPROGRAM_DEFAULT,"basic")) return;
 loadcgprogram(CGPROGRAM_BM,"bm");
 loadcgprogram(CGPROGRAM_ENV,"basicenv");
 loadcgprogram(CGPROGRAM_BMENV,"bmenv");*/
 if (!loadcgprograms()) return;
 cgs.currentprogram=CGPROGRAM_DEFAULT;
 useshaders=1;
}

int CGraph::loadcgprograms()
{
 if (!loadcgprogram(CGPROGRAM_DEFAULT)) return 0;
 if (!loadcgprogram(CGPROGRAM_BM)) return 0;
 if (!loadcgprogram(CGPROGRAM_ENV)) return 0;
 if (!loadcgprogram(CGPROGRAM_BMENV)) return 0;
 return 1;
}

int CGraph::initcg()
{
 cgs.cgcontext=cgCreateContext();
 if (cgs.cgcontext==0) return 0;
 
 cgs.vprofile=cgGLGetLatestProfile(CG_GL_VERTEX);
 cgGLSetOptimalOptions(cgs.vprofile);
 cgs.fprofile=cgGLGetLatestProfile(CG_GL_FRAGMENT);
 cgGLSetOptimalOptions(cgs.fprofile);
 return 1;
}

char *CGraph::buildcgvertexprogramtext(int prog,int type)
{
 static char progtxt[1024];
 
 //PHONG SHADING
 if (prog==CGPROGRAM_DEFAULT)
 {
  if (type==CGPROGRAM_NOTEX_DLIGHT)
  {

   strcpy(progtxt,
"void NOTEX_DLIGHT(float4 pos : POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
\
uniform float4x4 MVPMat,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float4 opos0:TEXCOORD0,\
out float3 onormal:TEXCOORD1)\
{\
 opos=mul(MVPMat,pos);\
 opos0=pos;\
 ocol=col;\
 onormal=normal;\
}");

  }
  else
  if (type==CGPROGRAM_TEX_DLIGHT)
  {

   strcpy(progtxt,
"void TEX_DLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
float2 tex:TEXCOORD0,\
\
uniform float4x4 MVPMat,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float2 otex:TEXCOORD0,\
out float4 opos0:TEXCOORD1,\
out float3 onormal:TEXCOORD2)\
{\
 opos=mul(MVPMat,pos);\
 opos0=pos;\
 ocol=col;\
 otex=tex;\
 onormal=normal;\
}");

  }
  else
  if (type==CGPROGRAM_NOTEX_PLIGHT)
  {

   strcpy(progtxt,
"void NOTEX_PLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
\
uniform float4x4 MVPMat,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float4 opos0:TEXCOORD0,\
out float3 onormal:TEXCOORD1)\
{\
 opos=mul(MVPMat,pos);\
 opos0=pos;\
 ocol=col;\
 onormal=normal;\
}");

  }
  else
  if (type==CGPROGRAM_TEX_PLIGHT)
  {

   strcpy(progtxt,
"void TEX_PLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
float2 tex:TEXCOORD0,\
\
uniform float4x4 MVPMat,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float2 otex:TEXCOORD0,\
out float4 opos0:TEXCOORD1,\
out float3 onormal:TEXCOORD2)\
{\
 opos=mul(MVPMat,pos);\
 opos0=pos;\
 ocol=col;\
 otex=tex;\
 onormal=normal;\
}");

  }
 }
 else
 
 //BUMPMAPPING
 if (prog==CGPROGRAM_BM)
 {
  if (type==CGPROGRAM_NOTEX_DLIGHT)
  {
   
   strcpy(progtxt,
"void NOTEX_DLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
float2 tex:TEXCOORD0,\
\
uniform float4x4 MVPMat,\
uniform float3 LightVec,\
uniform float3 EyeVec,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float2 otex:TEXCOORD0,\
out float3 olightvec:TEXCOORD1,\
out float3 oeyevec:TEXCOORD2)\
{\
 opos=mul(MVPMat,pos);\
 ocol=col;\
 otex=tex;\
 float3 tangent=normalize(float3(normal.y,-normal.x,0));\
 float3 binormal=cross(tangent,normal);\
 float3x3 rotation=float3x3(tangent,normal,binormal);\
 olightvec=mul(rotation,LightVec);\
 oeyevec=mul(rotation,EyeVec-pos);\
}");

  }
  else
  if (type==CGPROGRAM_TEX_DLIGHT)
  {
 
   strcpy(progtxt,
"void TEX_DLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
float2 tex:TEXCOORD0,\
\
uniform float4x4 MVPMat,\
uniform float3 LightVec,\
uniform float3 EyeVec,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float2 otex:TEXCOORD0,\
out float3 olightvec:TEXCOORD1,\
out float3 oeyevec:TEXCOORD2)\
{\
 opos=mul(MVPMat,pos);\
 ocol=col;\
 otex=tex;\
 float3 tangent=normalize(float3(normal.y,-normal.x,0));\
 float3 binormal=cross(tangent,normal);\
 float3x3 rotation=float3x3(tangent,normal,binormal);\
 olightvec=mul(rotation,LightVec);\
 oeyevec=mul(rotation,EyeVec-pos);\
}");

  }
  else
  if (type==CGPROGRAM_NOTEX_PLIGHT)
  {
  
   strcpy(progtxt,
"void NOTEX_PLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
float2 tex:TEXCOORD0,\
\
uniform float4x4 MVPMat,\
uniform float3 LightVec,\
uniform float3 EyeVec,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float2 otex:TEXCOORD0,\
out float3 olightvec:TEXCOORD1,\
out float3 oeyevec:TEXCOORD2)\
{\
 opos=mul(MVPMat,pos);\
 ocol=col;\
 otex=tex;\
 float3 tangent=normalize(float3(normal.y,-normal.x,0));\
 float3 binormal=cross(tangent,normal);\
 float3x3 rotation=float3x3(tangent,normal,binormal);\
 olightvec=mul(rotation,LightVec-pos);\
 oeyevec=mul(rotation,EyeVec-pos);\
}");

  }
  else
  if (type==CGPROGRAM_TEX_PLIGHT)
  {
  
   strcpy(progtxt,
"void TEX_PLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
float2 tex:TEXCOORD0,\
\
uniform float4x4 MVPMat,\
uniform float3 LightVec,\
uniform float3 EyeVec,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float2 otex:TEXCOORD0,\
out float3 olightvec:TEXCOORD1,\
out float3 oeyevec:TEXCOORD2)\
{\
 opos=mul(MVPMat,pos);\
 ocol=col;\
 otex=tex;\
 float3 tangent=normalize(float3(normal.y,-normal.x,0));\
 float3 binormal=cross(tangent,normal);\
 float3x3 rotation=float3x3(tangent,normal,binormal);\
 olightvec=mul(rotation,LightVec-pos);\
 oeyevec=mul(rotation,EyeVec-pos);\
}");

  }
 }
 else
 
 //ENVIRONMENT MAPPING
 if (prog==CGPROGRAM_ENV)
 {
  if (type==CGPROGRAM_NOTEX_DLIGHT)
  {
   
   strcpy(progtxt,
"void NOTEX_DLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
\
uniform float4x4 MVPMat,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float4 opos0:TEXCOORD0,\
out float3 onormal:TEXCOORD1)\
{\
 opos=mul(MVPMat,pos);\
 opos0=pos;\
 ocol=col;\
 onormal=normal;\
}");

  }
  else
  if (type==CGPROGRAM_TEX_DLIGHT)
  {
 
   strcpy(progtxt,
"void TEX_DLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
float2 tex:TEXCOORD0,\
\
uniform float4x4 MVPMat,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float2 otex:TEXCOORD0,\
out float4 opos0:TEXCOORD1,\
out float3 onormal:TEXCOORD2)\
{\
 opos=mul(MVPMat,pos);\
 opos0=pos;\
 ocol=col;\
 otex=tex;\
 onormal=normal;\
}");

  }
  else
  if (type==CGPROGRAM_NOTEX_PLIGHT)
  {
  
   strcpy(progtxt,
"void NOTEX_PLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
\
uniform float4x4 MVPMat,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float4 opos0:TEXCOORD0,\
out float3 onormal:TEXCOORD1)\
{\
 opos=mul(MVPMat,pos);\
 opos0=pos;\
 ocol=col;\
 onormal=normal;\
}");

  }
  else
  if (type==CGPROGRAM_TEX_PLIGHT)
  {
  
   strcpy(progtxt,
"void TEX_PLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
float2 tex:TEXCOORD0,\
\
uniform float4x4 MVPMat,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float2 otex:TEXCOORD0,\
out float4 opos0:TEXCOORD1,\
out float3 onormal:TEXCOORD2)\
{\
 opos=mul(MVPMat,pos);\
 opos0=pos;\
 ocol=col;\
 otex=tex;\
 onormal=normal;\
}");

  }
 }
 else
 
 //ENVIRONMENT MAPPING + BUMPMAPPING
 if (prog==CGPROGRAM_BMENV)
 {
  if (type==CGPROGRAM_NOTEX_DLIGHT)
  {
   
   strcpy(progtxt,
"void NOTEX_DLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
float2 tex:TEXCOORD0,\
\
uniform float4x4 MVPMat,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float2 otex:TEXCOORD0,\
out float4 opos0:TEXCOORD1,\
out float3 onormal:TEXCOORD2)\
{\
 opos=mul(MVPMat,pos);\
 ocol=col;\
 otex=tex;\
 opos0=pos;\
 onormal=normal;\
}");

  }
  else
  if (type==CGPROGRAM_TEX_DLIGHT)
  {
 
   strcpy(progtxt,
"void TEX_DLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
float2 tex:TEXCOORD0,\
\
uniform float4x4 MVPMat,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float2 otex:TEXCOORD0,\
out float4 opos0:TEXCOORD1,\
out float3 onormal:TEXCOORD2)\
{\
 opos=mul(MVPMat,pos);\
 ocol=col;\
 otex=tex;\
 opos0=pos;\
 onormal=normal;\
}");

  }
  else
  if (type==CGPROGRAM_NOTEX_PLIGHT)
  {
  
   strcpy(progtxt,
"void NOTEX_PLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
float2 tex:TEXCOORD0,\
\
uniform float4x4 MVPMat,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float2 otex:TEXCOORD0,\
out float4 opos0:TEXCOORD1,\
out float3 onormal:TEXCOORD2)\
{\
 opos=mul(MVPMat,pos);\
 ocol=col;\
 otex=tex;\
 opos0=pos;\
 onormal=normal;\
}");

  }
  else
  if (type==CGPROGRAM_TEX_PLIGHT)
  {
  
   strcpy(progtxt,
"void TEX_PLIGHT(float4 pos:POSITION,\
float4 col:COLOR,\
float3 normal:NORMAL,\
float2 tex:TEXCOORD0,\
\
uniform float4x4 MVPMat,\
\
out float4 opos:POSITION,\
out float4 ocol:COLOR,\
out float2 otex:TEXCOORD0,\
out float4 opos0:TEXCOORD1,\
out float3 onormal:TEXCOORD2)\
{\
 opos=mul(MVPMat,pos);\
 ocol=col;\
 otex=tex;\
 opos0=pos;\
 onormal=normal;\
}");

  }
 }
 
 return progtxt;
}

char *CGraph::buildcgfragmentprogramtext(int prog,int type,int effect)
{
 static char progtxt[1024];
 char effecttxt[256];
 if (effect==CGEFFECT_DEFAULT)
 {
  strcpy(effecttxt,"float4 effect(float4 c){return c;}");
 }
 else
 if (effect==CGEFFECT_NEGATIVE)
 {
  strcpy(effecttxt,"float4 effect(float4 c){return 1-c;}");
 }
 else
 if (effect==CGEFFECT_SEPIA)
 {
  strcpy(effecttxt,"float4 effect(float4 c){return 0.33*(c.r+c.g+c.b)*float4(1,0.9,0.1,1);}");
 }
 else
 if (effect==CGEFFECT_16COLORS)
 {
  strcpy(effecttxt,"float4 effect(float4 c){return floor(c*16)/16;}");
 }
 else
 if (effect==CGEFFECT_8COLORS)
 {
  strcpy(effecttxt,"float4 effect(float4 c){return floor(c*8)/8;}");
 }
 
 //PHONG SHADING
 if (prog==CGPROGRAM_DEFAULT)
 {
  strcpy(progtxt,
"struct LightStruct\
{\
 float4 Ambient;\
 float4 Diffuse;\
 float4 Specular;\
 float Shininess;\
 \
 float3 LightVec;\
 float3 EyeVec;\
};");
  strcat(progtxt,effecttxt);
  if (type==CGPROGRAM_NOTEX_DLIGHT)
  {
   
   strcat(progtxt,
"void NOTEX_DLIGHT(float4 col:COLOR,\
float3 pos:TEXCOORD0,\
float3 norm:TEXCOORD1,\
\
out float4 ocol:COLOR,\
\
uniform LightStruct Light)\
{\
 float3 N=normalize(norm);\
 float3 L=Light.LightVec;\
 float3 H=normalize(L+normalize(Light.EyeVec-pos));\
 float4 litc=lit(dot(N,L),dot(N,H),Light.Shininess);\
 ocol=(Light.Ambient+Light.Diffuse*litc.y+Light.Specular*litc.z)*col;\
 ocol=effect(ocol);\
}");

  }
  else
  if (type==CGPROGRAM_TEX_DLIGHT)
  {
 
   strcat(progtxt,
"void TEX_DLIGHT(float4 col:COLOR,\
float2 texc:TEXCOORD0,\
float3 pos:TEXCOORD1,\
float3 norm:TEXCOORD2,\
\
out float4 ocol:COLOR,\
\
uniform sampler2D Texture0,\
uniform LightStruct Light)\
{\
 float3 N=normalize(norm);\
 float3 L=Light.LightVec;\
 float3 H=normalize(L+normalize(Light.EyeVec-pos));\
 float4 litc=lit(dot(N,L),dot(N,H),Light.Shininess);\
 ocol=(Light.Ambient+Light.Diffuse*litc.y+Light.Specular*litc.z)*col*tex2D(Texture0,texc);\
 ocol=effect(ocol);\
 ocol.w=tex2D(Texture0,texc).w;\
}");

  }
  else
  if (type==CGPROGRAM_NOTEX_PLIGHT)
  {
  
   strcat(progtxt,
"void NOTEX_PLIGHT(float4 col:COLOR,\
float3 pos:TEXCOORD0,\
float3 norm:TEXCOORD1,\
\
out float4 ocol:COLOR,\
\
uniform LightStruct Light)\
{\
 float3 N=normalize(norm);\
 float3 L=normalize(Light.LightVec-pos);\
 float3 H=normalize(Light.LightVec+Light.EyeVec-2*pos);\
 float4 litc=lit(dot(N,L),dot(N,H),Light.Shininess);\
 ocol=(Light.Ambient+Light.Diffuse*litc.y+Light.Specular*litc.z)*col;\
 ocol=effect(ocol);\
}");

  }
  else
  if (type==CGPROGRAM_TEX_PLIGHT)
  {
  
   strcat(progtxt,
"void TEX_PLIGHT(float4 col:COLOR,\
float2 texc:TEXCOORD0,\
float3 pos:TEXCOORD1,\
float3 norm:TEXCOORD2,\
\
out float4 ocol:COLOR,\
\
uniform sampler2D Texture0,\
\
uniform LightStruct Light)\
{\
 float3 N=normalize(norm);\
 float3 L=normalize(Light.LightVec-pos);\
 float3 H=normalize(Light.LightVec+Light.EyeVec-2*pos);\
 float4 litc=lit(dot(N,L),dot(N,H),Light.Shininess);\
 ocol=(Light.Ambient+Light.Diffuse*litc.y+Light.Specular*litc.z)*col*tex2D(Texture0,texc);\
 ocol=effect(ocol);\
 ocol.w=tex2D(Texture0,texc).w;\
}");

  }
 }
 else
 
 //BUMPMAPPING
 if (prog==CGPROGRAM_BM)
 {
  strcpy(progtxt,
"struct LightStruct\
{\
 float4 Ambient;\
 float4 Diffuse;\
 float4 Specular;\
 float Shininess;\
};");
  strcat(progtxt,effecttxt);
  if (type==CGPROGRAM_NOTEX_DLIGHT)
  {
   
   strcat(progtxt,
"void NOTEX_DLIGHT(float4 col:COLOR,\
float2 texc:TEXCOORD0,\
float3 lightvec:TEXCOORD1,\
float3 eyevec:TEXCOORD2,\
\
out float4 ocol:COLOR,\
\
uniform sampler2D Texture1:TEXUNIT1,\
\
uniform LightStruct Light)\
{\
 float3 N=normalize(2*(tex2D(Texture1,texc).xyz-0.5));\
 float3 L=normalize(lightvec);\
 float3 H=normalize(lightvec+normalize(eyevec));\
 float4 litc=lit(dot(N,L),dot(N,H),Light.Shininess);\
 ocol=(Light.Ambient+Light.Diffuse*litc.y+Light.Specular*litc.z)*col;\
 ocol=effect(ocol);\
 ocol.w=1;\
}");

  }
  else
  if (type==CGPROGRAM_TEX_DLIGHT)
  {
 
   strcat(progtxt,
"void TEX_DLIGHT(float4 col:COLOR,\
float2 texc:TEXCOORD0,\
float3 lightvec:TEXCOORD1,\
float3 eyevec:TEXCOORD2,\
\
out float4 ocol:COLOR,\
\
uniform sampler2D Texture0:TEXUNIT0,\
uniform sampler2D Texture1:TEXUNIT1,\
\
uniform LightStruct Light)\
{\
 float3 N=normalize(2*(tex2D(Texture1,texc).xyz-0.5));\
 float3 L=normalize(lightvec);\
 float3 H=normalize(lightvec+normalize(eyevec));\
 float4 litc=lit(dot(N,L),dot(N,H),Light.Shininess);\
 ocol=(Light.Ambient+Light.Diffuse*litc.y+Light.Specular*litc.z)*col*tex2D(Texture0,texc);\
 ocol=effect(ocol);\
 ocol.w=tex2D(Texture0,texc).w;\
}");

  }
  else
  if (type==CGPROGRAM_NOTEX_PLIGHT)
  {
  
   strcat(progtxt,
"void NOTEX_PLIGHT(float4 col:COLOR,\
float2 texc:TEXCOORD0,\
float3 lightvec:TEXCOORD1,\
float3 eyevec:TEXCOORD2,\
\
out float4 ocol:COLOR,\
\
uniform sampler2D Texture1:TEXUNIT1,\
\
uniform LightStruct Light)\
{\
 float3 N=normalize(2*(tex2D(Texture1,texc).xyz-0.5));\
 float3 L=normalize(lightvec);\
 float3 H=normalize(lightvec+eyevec);\
 float4 litc=lit(dot(N,L),dot(N,H),Light.Shininess);\
 ocol=(Light.Ambient+Light.Diffuse*litc.y+Light.Specular*litc.z)*col;\
 ocol=effect(ocol);\
 ocol.w=1;\
}");

  }
  else
  if (type==CGPROGRAM_TEX_PLIGHT)
  {
  
   strcat(progtxt,
"void TEX_PLIGHT(float4 col:COLOR,\
float2 texc:TEXCOORD0,\
float3 lightvec:TEXCOORD1,\
float3 eyevec:TEXCOORD2,\
\
out float4 ocol:COLOR,\
\
uniform sampler2D Texture0:TEXUNIT0,\
uniform sampler2D Texture1:TEXUNIT1,\
\
uniform LightStruct Light)\
{\
 float3 N=normalize(2*(tex2D(Texture1,texc).xyz-0.5));\
 float3 L=normalize(lightvec);\
 float3 H=normalize(lightvec+eyevec);\
 float4 litc=lit(dot(N,L),dot(N,H),Light.Shininess);\
 ocol=(Light.Ambient+Light.Diffuse*litc.y+Light.Specular*litc.z)*col*tex2D(Texture0,texc);\
 ocol=effect(ocol);\
 ocol.w=tex2D(Texture0,texc).w;\
}");

  }
 }
 else
 
 //ENVIRONMENT MAPPING
 if (prog==CGPROGRAM_ENV)
 {
  strcpy(progtxt,effecttxt);
  if (type==CGPROGRAM_NOTEX_DLIGHT)
  {
   
   strcat(progtxt,
"void NOTEX_DLIGHT(float4 col:COLOR,\
float3 pos:TEXCOORD0,\
float3 norm:TEXCOORD1,\
\
out float4 ocol:COLOR,\
\
uniform samplerCUBE EnvMap:TEXUNIT2,\
uniform float3 EyeVec)\
{\
 ocol=texCUBE(EnvMap,reflect(pos-EyeVec,normalize(norm)))*col;\
 ocol=effect(ocol);\
 ocol.w=1;\
}");

  }
  else
  if (type==CGPROGRAM_TEX_DLIGHT)
  {
 
   strcat(progtxt,
"void TEX_DLIGHT(float4 col:COLOR,\
float2 texc:TEXCOORD0,\
float3 pos:TEXCOORD1,\
float3 norm:TEXCOORD2,\
\
out float4 ocol:COLOR,\
\
uniform sampler2D Texture0,\
uniform samplerCUBE EnvMap:TEXUNIT2,\
uniform float3 EyeVec)\
{\
 ocol=lerp(texCUBE(EnvMap,reflect(pos-EyeVec,normalize(norm))),\
 tex2D(Texture0,texc),tex2D(Texture0,texc).w)*col;\
 ocol=effect(ocol);\
 ocol.w=1;\
}");

  }
  else
  if (type==CGPROGRAM_NOTEX_PLIGHT)
  {
  
   strcat(progtxt,
"void NOTEX_PLIGHT(float4 col:COLOR,\
float3 pos:TEXCOORD0,\
float3 norm:TEXCOORD1,\
\
out float4 ocol:COLOR,\
uniform samplerCUBE EnvMap:TEXUNIT2,\
uniform float3 EyeVec)\
{\
 ocol=texCUBE(EnvMap,reflect(pos-EyeVec,normalize(norm)))*col;\
 ocol=effect(ocol);\
 ocol.w=1;\
}");

  }
  else
  if (type==CGPROGRAM_TEX_PLIGHT)
  {
  
   strcat(progtxt,
"void TEX_PLIGHT(float4 col:COLOR,\
float2 texc:TEXCOORD0,\
float3 pos:TEXCOORD1,\
float3 norm:TEXCOORD2,\
\
out float4 ocol:COLOR,\
\
uniform sampler2D Texture0:TEXUNIT0,\
uniform samplerCUBE EnvMap:TEXUNIT2,\
\
uniform float3 EyeVec)\
{\
 ocol=lerp(texCUBE(EnvMap,reflect(pos-EyeVec,normalize(norm))),\
 tex2D(Texture0,texc),tex2D(Texture0,texc).w)*col;\
 ocol=effect(ocol);\
 ocol.w=1;\
}");

  }
 }
 else
 
 //ENVIRONMENT MAPPING + BUMPMAPPING
 if (prog==CGPROGRAM_BMENV)
 {
  strcpy(progtxt,effecttxt);
  if (type==CGPROGRAM_NOTEX_DLIGHT)
  {
   
   strcat(progtxt,
"void NOTEX_DLIGHT(float4 col:COLOR,\
float2 texc:TEXCOORD0,\
float3 pos:TEXCOORD1,\
float3 normal:TEXCOORD2,\
\
out float4 ocol:COLOR,\
uniform sampler2D Texture1:TEXUNIT1,\
uniform samplerCUBE EnvMap:TEXUNIT2,\
uniform float3 EyeVec)\
{\
 normal=normalize(normal);\
 float3 tangent=normalize(float3(normal.y,-normal.x,0));\
 float3 binormal=cross(tangent,normal);\
 float3x3 rotation=transpose(float3x3(tangent,normal,binormal));\
 float3 ntex=normalize(2*(tex2D(Texture1,texc).xyz-0.5));\
 ocol=texCUBE(EnvMap,reflect(pos-EyeVec,mul(rotation,ntex)))*col;\
 ocol=effect(ocol);\
 ocol.w=1;\
}");

  }
  else
  if (type==CGPROGRAM_TEX_DLIGHT)
  {
 
   strcat(progtxt,
"void TEX_DLIGHT(float4 col:COLOR,\
float2 texc:TEXCOORD0,\
float3 pos:TEXCOORD1,\
float3 normal:TEXCOORD2,\
\
out float4 ocol:COLOR,\
uniform sampler2D Texture0:TEXUNIT0,\
uniform sampler2D Texture1:TEXUNIT1,\
uniform samplerCUBE EnvMap:TEXUNIT2,\
uniform float3 EyeVec)\
{\
 normal=normalize(normal);\
 float3 tangent=normalize(float3(normal.y,-normal.x,0));\
 float3 binormal=cross(tangent,normal);\
 float3x3 rotation=transpose(float3x3(tangent,normal,binormal));\
 float3 ntex=normalize(2*(tex2D(Texture1,texc).xyz-0.5));\
 float4 ccube=texCUBE(EnvMap,reflect(pos-EyeVec,mul(rotation,ntex)));\
 ocol=lerp(ccube,tex2D(Texture0,texc),tex2D(Texture0,texc).w)*col;\
 ocol=effect(ocol);\
 ocol.w=1;\
}");

  }
  else
  if (type==CGPROGRAM_NOTEX_PLIGHT)
  {
  
   strcat(progtxt,
"void NOTEX_PLIGHT(float4 col:COLOR,\
float2 texc:TEXCOORD0,\
float3 pos:TEXCOORD1,\
float3 normal:TEXCOORD2,\
\
out float4 ocol:COLOR,\
uniform sampler2D Texture1:TEXUNIT1,\
uniform samplerCUBE EnvMap:TEXUNIT2,\
uniform float3 EyeVec)\
{\
 normal=normalize(normal);\
 float3 tangent=normalize(float3(normal.y,-normal.x,0));\
 float3 binormal=cross(tangent,normal);\
 float3x3 rotation=transpose(float3x3(tangent,normal,binormal));\
 float3 ntex=normalize(2*(tex2D(Texture1,texc).xyz-0.5));\
 ocol=texCUBE(EnvMap,reflect(pos-EyeVec,mul(rotation,ntex)))*col;\
 ocol=effect(ocol);\
 ocol.w=1;\
}");

  }
  else
  if (type==CGPROGRAM_TEX_PLIGHT)
  {
  
   strcat(progtxt,
"void TEX_PLIGHT(float4 col:COLOR,\
float2 texc:TEXCOORD0,\
float3 pos:TEXCOORD1,\
float3 normal:TEXCOORD2,\
\
out float4 ocol:COLOR,\
uniform sampler2D Texture0:TEXUNIT0,\
uniform sampler2D Texture1:TEXUNIT1,\
uniform samplerCUBE EnvMap:TEXUNIT2,\
uniform float3 EyeVec)\
{\
 normal=normalize(normal);\
 float3 tangent=normalize(float3(normal.y,-normal.x,0));\
 float3 binormal=cross(tangent,normal);\
 float3x3 rotation=transpose(float3x3(tangent,normal,binormal));\
 float3 ntex=normalize(2*(tex2D(Texture1,texc).xyz-0.5));\
 float4 ccube=texCUBE(EnvMap,reflect(pos-EyeVec,mul(rotation,ntex)));\
 ocol=lerp(ccube,tex2D(Texture0,texc),tex2D(Texture0,texc).w)*col;\
 ocol=effect(ocol);\
 ocol.w=1;\
}");

  }
 }
 
 return progtxt;
}

int CGraph::loadcgprogram(int i)
{
 char pbuf[1024];
 
 //vertex program

 for (int j=0; j<CGSPECPROGRAMS; j++)
 {
  strcpy(pbuf,buildcgvertexprogramtext(i,j));
  if (cgs.programs[i].cgp[j].fprogram) cgDestroyProgram(cgs.programs[i].cgp[j].fprogram);
  cgs.programs[i].cgp[j].vprogram=
  cgCreateProgram(cgs.cgcontext,CG_SOURCE,pbuf,cgs.vprofile,
  cg_spec_program_names[j],0);
  if (cgs.programs[i].cgp[j].vprogram==0) 
  {
   CGerror e=cgGetError();
   //MessageBox(0,cgGetErrorString(e),"cgerror info",0);
   //CSys::Error((char*)cgGetErrorString(e),1);
   CSys::Log_Printf((char*)cgGetErrorString(e));
   return 0;
  }
  
  cgs.programs[i].cgp[j].modelview_projection_matrix=
  cgGetNamedParameter(cgs.programs[i].cgp[j].vprogram,"MVPMat");
  cgs.programs[i].cgp[j].lightvec=cgGetNamedParameter(cgs.programs[i].cgp[j].vprogram,
  "LightVec");
  cgs.programs[i].cgp[j].eyevec=cgGetNamedParameter(cgs.programs[i].cgp[j].vprogram,"EyeVec");
  
  cgGLLoadProgram(cgs.programs[i].cgp[j].vprogram);
 }
 
 for (int j=0; j<CGSPECPROGRAMS; j++)
 {
  strcpy(pbuf,buildcgfragmentprogramtext(i,j,cgs.effect));
  if (cgs.programs[i].cgp[j].fprogram) cgDestroyProgram(cgs.programs[i].cgp[j].fprogram);
  cgs.programs[i].cgp[j].fprogram=
  cgCreateProgram(cgs.cgcontext,CG_SOURCE,pbuf,cgs.fprofile,cg_spec_program_names[j],0);
  if (cgs.programs[i].cgp[j].fprogram==0) 
  {
   CGerror e=cgGetError();
   //CSys::Error((char*)cgGetErrorString(e),1);
   CSys::Log_Printf((char*)cgGetErrorString(e));
   return 0;
  }
 
  CGprogram cgp=cgs.programs[i].cgp[j].fprogram;
  cgs.programs[i].cgp[j].texture0=cgGetNamedParameter(cgp,"Texture0");
  cgs.programs[i].cgp[j].texture1=cgGetNamedParameter(cgp,"Texture1");
  cgs.programs[i].cgp[j].envmap=cgGetNamedParameter(cgp,"EnvMap");
  cgs.programs[i].cgp[j].ambient=cgGetNamedParameter(cgp,"Light.Ambient");
  cgs.programs[i].cgp[j].diffuse=cgGetNamedParameter(cgp,"Light.Diffuse");
  cgs.programs[i].cgp[j].specular=cgGetNamedParameter(cgp,"Light.Specular");
  cgs.programs[i].cgp[j].shininess=cgGetNamedParameter(cgp,"Light.Shininess");
  if (cgs.programs[i].cgp[j].lightvec==0)
  cgs.programs[i].cgp[j].lightvec=cgGetNamedParameter(cgp,"Light.LightVec");
  if (cgs.programs[i].cgp[j].eyevec==0)
  cgs.programs[i].cgp[j].eyevec=cgGetNamedParameter(cgp,"Light.EyeVec");
  if (cgs.programs[i].cgp[j].eyevec==0)
  cgs.programs[i].cgp[j].eyevec=cgGetNamedParameter(cgp,"EyeVec");
 
  cgGLLoadProgram(cgs.programs[i].cgp[j].fprogram);
 }
 
 return 1;
}

void CGraph::bindcgprogram(int state)
{
 int i=cgs.currentprogram;
 
 if (lighting==0) state=0;
 
 int spec;
 if (dmethod==MTEX)
 {
  if (light_vec[3]==0) spec=CGPROGRAM_TEX_DLIGHT;
  else spec=CGPROGRAM_TEX_PLIGHT;
 }
 else
 {
  if (light_vec[3]==0) spec=CGPROGRAM_NOTEX_DLIGHT;
  else spec=CGPROGRAM_NOTEX_PLIGHT;
 }
 
 static int dm=MLINES;
 if (UseShaders())
 {
  if (state==1)
  {
   dm=dmethod;
   if ((i==CGPROGRAM_BM)||(i==CGPROGRAM_BMENV)) dmethod=MTEX;
  }
  else dmethod=dm;
 }
 
 if (state==1)
 {
    
  if ((cgs.programs[i].cgp[spec].vprogram!=0)
  &&(cgs.programs[i].cgp[spec].fprogram!=0))
  {
   cgGLEnableProfile(cgs.vprofile);
   cgGLBindProgram(cgs.programs[i].cgp[spec].vprogram);
   cgGLEnableProfile(cgs.fprofile);
   cgGLBindProgram(cgs.programs[i].cgp[spec].fprogram);
  }
  float epos[3];

  /*epos[1]=-sinax;
  epos[2]=cosax;
  epos[0]=epos[2]*sinaz;
  epos[2]=epos[2]*cosaz;*/
  epos[0]=j[AxisX];
  epos[1]=j[AxisZ];
  epos[2]=-j[AxisY];
    
  cgGLSetStateMatrixParameter(cgs.programs[i].cgp[spec].modelview_projection_matrix,
  CG_GL_MODELVIEW_PROJECTION_MATRIX,CG_GL_MATRIX_IDENTITY);
  cgGLSetTextureParameter(cgs.programs[i].cgp[spec].texture0,1);
  cgGLSetTextureParameter(cgs.programs[i].cgp[spec].texture1,2);
  cgGLEnableTextureParameter(cgs.programs[i].cgp[spec].texture0);
  cgGLEnableTextureParameter(cgs.programs[i].cgp[spec].texture1);
  cgGLSetParameter4fv(cgs.programs[i].cgp[spec].ambient,gl_light_ambient);
  cgGLSetParameter4fv(cgs.programs[i].cgp[spec].diffuse,gl_light_diffuse);
  cgGLSetParameter4fv(cgs.programs[i].cgp[spec].specular,gl_light_specular);
  cgGLSetParameter1f(cgs.programs[i].cgp[spec].shininess,gl_shininess);
  cgGLSetParameter3fv(cgs.programs[i].cgp[spec].lightvec,light_vec);
  cgGLSetParameter3fv(cgs.programs[i].cgp[spec].eyevec,epos);
 }
 else
 {
  cgGLUnbindProgram(cgs.vprofile);
  cgGLDisableProfile(cgs.vprofile);
  cgGLUnbindProgram(cgs.fprofile);
  cgGLDisableProfile(cgs.fprofile);
  cgGLDisableTextureParameter(cgs.programs[i].cgp[spec].texture0);
  cgGLDisableTextureParameter(cgs.programs[i].cgp[spec].texture1);
 }
}
#endif

void CGraph::gl_update()
{
 if (!gl_used) return;
  
 //perspektywa
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glFrustum(-MINPROJECTZ*tan(vangle/2),
 MINPROJECTZ*tan(vangle/2),
 MINPROJECTZ*tan(vangle/2)*(float)height/(float)width,
 -MINPROJECTZ*tan(vangle/2)*(float)height/(float)width,MINPROJECTZ,100000);
 glMatrixMode(GL_MODELVIEW);
 
 //zbuf
 glEnable(GL_DEPTH_TEST);
 
 //metoda rysowania
 /*int glm=GL_LINE;
 if (dmethod==MLINES) glm=GL_LINE;
 else
 if (dmethod>MLINES) glm=GL_FILL;
 glPolygonMode(GL_FRONT_AND_BACK,glm);*/
 
 //ukrywanie powierzchni
 if (gl_cull)
 {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
 }
 else
 {
  glDisable(GL_CULL_FACE);
 }
 
 //oswietlenie
 /*if (lighting)
 {*/
  glLightfv(GL_LIGHT0,GL_AMBIENT,(GLfloat *)&gl_light_ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,(GLfloat *)&gl_light_diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,(GLfloat *)&gl_light_specular);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,(GLfloat *)&gl_light_ambient);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,twosidelighting);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(GLfloat *)&gl_light_specular);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,(GLfloat *)&gl_light_emission);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,gl_shininess);
 //}
}

void CGraph::gl_compvertextabref(float x,float y)
{
 expr_x=x;
 expr_y=y;
 float register z=circfunc_expr->Do();
 if (IS_UD(z)) z=0;
 else
 if (IS_INFM(z)) z=-10000;
 else
 if (IS_INFP(z)) z=10000;
 ((VEC*)tptr[gl_vwsk])->a=x;
 ((VEC*)tptr[gl_vwsk])->b=z;
 ((VEC*)tptr[gl_vwsk])->c=-y;
}

//przeprowadz obliczenia dla ogladanego wykresu w 3D
int CGraph::gl_compvertextab(CExpr *expr,FUNCSTRUCT *f)
{
 int status;
 if (f->status==1)
 {
  f->status=-1;
  status=0;
 } else status=1;

 if (f->status!=2)
 {
  if (!circlefunc(expr,0,status,R,FGL_VWSK,1,&CGraph::gl_compvertextabref)) return 0;
 }
 if (!gl_compindextab(f)) return 0;
 return 1;
}

void CGraph::gl_compindextabref8(float x,float y)
{
 int shift=((int)tptr[gl_vwsk]-(int)gl_vertextab)/sizeof(VEC);
 
 gl_indextab[gl_indextabl++]=shift;
 gl_indextab[gl_indextabl++]=shift+1;
 
 gl_indextab[gl_indextabl++]=shift+1;
 gl_indextab[gl_indextabl++]=shift+N+1;
 
 gl_indextab[gl_indextabl++]=shift+N+1;
 gl_indextab[gl_indextabl++]=shift+N;
 
 gl_indextab[gl_indextabl++]=shift+N;
 gl_indextab[gl_indextabl++]=shift;
}

void CGraph::gl_compindextabref6(float x,float y)
{
 int shift=((int)tptr[gl_vwsk]-(int)gl_vertextab)/sizeof(VEC);
 
 gl_indextab[gl_indextabl++]=shift;
 gl_indextab[gl_indextabl++]=shift+1;
 gl_indextab[gl_indextabl++]=shift+N+1;
 
 gl_indextab[gl_indextabl++]=shift;
 gl_indextab[gl_indextabl++]=shift+N+1;
 gl_indextab[gl_indextabl++]=shift+N;
}

int CGraph::gl_compindextab(FUNCSTRUCT *f)
{
 int status;
 if (f->status==-1)
 {
  f->status=2;
  status=0;
  gl_indextabl=0;
 } else status=1;
 if (dmethod==MLINES)
 {
  if (!circlefunc(0,0,status,R-D,FGL_VWSK,0,&CGraph::gl_compindextabref8)) return 0;
 }
 else
 {
  if (!circlefunc(0,0,status,R-D,FGL_VWSK,0,&CGraph::gl_compindextabref6)) return 0;
 }
 return 1;
}

void CGraph::gl_compcoltabref(float x,float y)
{
 float z1=((VEC*)tptr[gl_vwsk])->b-zs;
 x-=xs;
 y-=ys;
   
 float d=sqrt(x*x+y*y+z1*z1);
 int col;
 if (d>lightdist) col=0;
 else
 col=circfunc_palwsk[(int)(255.f-d*_255_lightdist)&255];
 ((IVEC4*)tptr[gl_cwsk])->a=((col>>16)&255);
 ((IVEC4*)tptr[gl_cwsk])->b=((col>>8)&255);
 ((IVEC4*)tptr[gl_cwsk])->c=(col&255);
 ((IVEC4*)tptr[gl_cwsk])->d=255;
}

void CGraph::gl_compcoltab(FUNCSTRUCT *f)
{
 if (holdlight)
 {
  if (hold>0) {dstate+=2; return;}
  IVEC4 c;
  int col=iround(lightdist);
  if (col>255) col=255;
  col=Palette[f->color][col];
  c.a=((col>>16)&255);
  c.b=((col>>8)&255);
  c.c=(col&255);
  c.d=255;
  int s=GRAPHTAB_SIZE(N);
  for (int i=0; i<s; i++) {gl_colortab[i]=c;}
  dstate+=2;
  return;
 }

 int status;
 if (dstate==DS_COL)
 {
  status=0;
  dstate++;
 } else status=1;
 
 if (circlefunc(0,f,status,R,FGL_VWSK|FGL_CWSK,0,&CGraph::gl_compcoltabref)) dstate++;
}

int CGraph::drawfunc_K3DF2_gl_grid(FUNCSTRUCT *f)
{
 if (lighting)
 {
  if (dstate<DS_PROJECT+2)
  {
   dstate+=2;
  }
  if (dstate<DS_NORMAL+2)
  {
   compnormaltab();
   if (dstate!=4) return 0;
  }
  if (dstate<DS_COL+2)
  {
   gl_compcoltab(f);
   if (dstate!=DS_COL+2) return 0;
  }
 }
 else
 {
  if (dstate<DS_PROJECT+2)
  {
   dstate+=4;
  }
  if (dstate<DS_COL+2)
  {
   gl_compcoltab(f);
   if (dstate!=DS_COL+2) return 0;
  }
 }
 
 if (lighting) 
 {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
 
  glVertexPointer(3,GL_FLOAT,0,(void*)gl_vertextab);
  glColorPointer(4,GL_UNSIGNED_BYTE,0,(void*)gl_colortab);
  glNormalPointer(GL_FLOAT,0,(void*)normaltab);
  
  glDrawElements(GL_LINES,gl_indextabl,GL_UNSIGNED_INT,(void*)gl_indextab);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_COLOR_MATERIAL);
 }
 else
 {
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
 
  glVertexPointer(3,GL_FLOAT,0,(void*)gl_vertextab);
  glColorPointer(4,GL_UNSIGNED_BYTE,0,(void*)gl_colortab);
  
  glDrawElements(GL_LINES,gl_indextabl,GL_UNSIGNED_INT,(void*)gl_indextab);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
 }
 
 return 1;
}

int CGraph::drawfunc_K3DF2_gl_fill(FUNCSTRUCT *f)
{
 if (dstate<DS_PROJECT+2)
 {
  dstate+=4;
 }
 if (dstate<DS_COL+2)
 {
  gl_compcoltab(f);
  if (dstate!=DS_COL+2) return 0;
 }
 
 glEnableClientState(GL_VERTEX_ARRAY);
 glEnableClientState(GL_COLOR_ARRAY);
 
 glVertexPointer(3,GL_FLOAT,0,(void*)gl_vertextab);
 glColorPointer(4,GL_UNSIGNED_BYTE,0,(void*)gl_colortab);
  
 glDrawElements(GL_TRIANGLES,gl_indextabl,GL_UNSIGNED_INT,(void*)gl_indextab);
  
 glDisableClientState(GL_VERTEX_ARRAY);
 glDisableClientState(GL_COLOR_ARRAY);
 
 return 1;
}

int CGraph::drawfunc_K3DF2_gl_fill_light(FUNCSTRUCT *f)
{
 if (dstate<DS_PROJECT+2)
 {
  dstate+=2;
 }
 if (dstate<DS_NORMAL+2)
 {
  compnormaltab();
  if (dstate!=4) return 0;
 }
 if (dstate<DS_COL+2)
 {
  gl_compcoltab(f);
  if (dstate!=DS_COL+2) return 0;
 }
 
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
 glEnable(GL_COLOR_MATERIAL);
 
 glEnableClientState(GL_VERTEX_ARRAY);
 glEnableClientState(GL_COLOR_ARRAY);
 glEnableClientState(GL_NORMAL_ARRAY);
 
 glVertexPointer(3,GL_FLOAT,0,(void*)gl_vertextab);
 glColorPointer(4,GL_UNSIGNED_BYTE,0,(void*)gl_colortab);
 glNormalPointer(GL_FLOAT,0,(void*)normaltab);
  
 glDrawElements(GL_TRIANGLES,gl_indextabl,GL_UNSIGNED_INT,(void*)gl_indextab);

 glDisableClientState(GL_VERTEX_ARRAY);
 glDisableClientState(GL_COLOR_ARRAY);
 glDisableClientState(GL_NORMAL_ARRAY);
 
 glDisable(GL_LIGHTING);
 glDisable(GL_LIGHT0);
 glDisable(GL_COLOR_MATERIAL);
 
 return 1;
}

int CGraph::drawfunc_K3DF2_gl_fill_tex(FUNCSTRUCT *f)
{
 if (!tex[f->color].IsLoaded()) 
 {
  if (lighting) return drawfunc_K3DF2_gl_fill_light(f);
  else return drawfunc_K3DF2_gl_fill(f);
 }
 if (dstate<DS_PROJECT+2)
 {
  dstate+=2;
 }
 if (lighting)
 {
  if (dstate<DS_NORMAL+2)
  {
   compnormaltab();
   if (dstate!=DS_NORMAL+2) return 0;
  }
 }
 else
 if (dstate<DS_NORMAL+2)
 {
  dstate+=2;
 }
 if (dstate<DS_COL+2)
 {
  gl_compcoltab(f);
  if (dstate!=DS_COL+2) return 0;
 }
 if (dstate<DS_TEX+2)
 {
  comptexcoordtab(f);
  if (dstate!=DS_TEX+2) return 0;
 }
 
 tex[f->color].SetGL(SETGL_ON);
#ifdef CG
 if (UseShaders())
 {
  glActiveTexture(GL_TEXTURE2);
  for (int i=0; i<6; i++) envmap[i].SetGL(SETGL_ON);
  glActiveTexture(GL_TEXTURE0);
 }
#endif
 
 if (lighting)
 {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 
  glVertexPointer(3,GL_FLOAT,0,(void*)gl_vertextab);
  glColorPointer(4,GL_UNSIGNED_BYTE,0,(void*)gl_colortab);
  glNormalPointer(GL_FLOAT,0,(void*)normaltab);
  glTexCoordPointer(2,GL_FLOAT,0,(void*)texcoordtab);
  
  glDrawElements(GL_TRIANGLES,gl_indextabl,GL_UNSIGNED_INT,(void*)gl_indextab);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  
  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHTING);
 }
 else
 {
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 
  glVertexPointer(3,GL_FLOAT,0,(void*)gl_vertextab);
  glColorPointer(4,GL_UNSIGNED_BYTE,0,(void*)gl_colortab);
  glTexCoordPointer(2,GL_FLOAT,0,(void*)texcoordtab);
  
  glDrawElements(GL_TRIANGLES,gl_indextabl,GL_UNSIGNED_INT,(void*)gl_indextab);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 }
 
 tex[f->color].SetGL(SETGL_OFF);
#ifdef CG
 if (UseShaders())
 {
  glActiveTexture(GL_TEXTURE2);
  envmap[0].SetGL(SETGL_OFF);
  glActiveTexture(GL_TEXTURE0);
 }
#endif
 
 return 1;
}

#endif

void CGraph::drawsun()
{
#ifdef OPENGL
 if (light_vec[3]==0)
 {
  float v[3]={light_vec[0]*90000.f,light_vec[1]*90000.f,light_vec[2]*90000.f};
  unsigned char c[4]={255,255,0,255};
  int a[2];
  int size;
  glGetIntegerv(GL_ALIASED_POINT_SIZE_RANGE,(GLint*)&a);
  glGetIntegerv(GL_POINT_SIZE,(GLint*)&size);
  glEnable(GL_POINT_SMOOTH);
  glPointSize(a[1]);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
 
  glVertexPointer(3,GL_FLOAT,0,&v);
  glColorPointer(4,GL_UNSIGNED_BYTE,0,&c);
  
  glDrawArrays(GL_POINTS,0,1);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  glDisable(GL_POINT_SMOOTH);
  glPointSize(size);
 }
 else
 {
  float s=R/256.f;
  float v[3*2*12]=
  {
   light_vec[0]-s,light_vec[1]-s,light_vec[2]-s,
   light_vec[0]+s,light_vec[1]-s,light_vec[2]-s,
   
   light_vec[0]+s,light_vec[1]-s,light_vec[2]-s,
   light_vec[0]+s,light_vec[1]+s,light_vec[2]-s,
   
   light_vec[0]+s,light_vec[1]+s,light_vec[2]-s,
   light_vec[0]-s,light_vec[1]+s,light_vec[2]-s,
   
   light_vec[0]-s,light_vec[1]+s,light_vec[2]-s,
   light_vec[0]-s,light_vec[1]-s,light_vec[2]-s,
   //
   light_vec[0]-s,light_vec[1]-s,light_vec[2]+s,
   light_vec[0]+s,light_vec[1]-s,light_vec[2]+s,
   
   light_vec[0]+s,light_vec[1]-s,light_vec[2]+s,
   light_vec[0]+s,light_vec[1]+s,light_vec[2]+s,
   
   light_vec[0]+s,light_vec[1]+s,light_vec[2]+s,
   light_vec[0]-s,light_vec[1]+s,light_vec[2]+s,
   
   light_vec[0]-s,light_vec[1]+s,light_vec[2]+s,
   light_vec[0]-s,light_vec[1]-s,light_vec[2]+s,
   //
   light_vec[0]-s,light_vec[1]-s,light_vec[2]-s,
   light_vec[0]-s,light_vec[1]-s,light_vec[2]+s,
   
   light_vec[0]+s,light_vec[1]-s,light_vec[2]-s,
   light_vec[0]+s,light_vec[1]-s,light_vec[2]+s,
   
   light_vec[0]+s,light_vec[1]+s,light_vec[2]-s,
   light_vec[0]+s,light_vec[1]+s,light_vec[2]+s,
   
   light_vec[0]-s,light_vec[1]+s,light_vec[2]-s,
   light_vec[0]-s,light_vec[1]+s,light_vec[2]+s,
  };
  
  unsigned char c[4*2*12];
  for (int i=0; i<2*12; i++) {c[4*i]=255; c[4*i+1]=255; c[4*i+2]=0; c[4*i+3]=255;}
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
 
  glVertexPointer(3,GL_FLOAT,0,&v);
  glColorPointer(4,GL_UNSIGNED_BYTE,0,&c);
  
  glDrawArrays(GL_LINES,0,12*2);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
 }
#endif
}

//funkcje mapy srodowiska

int CGraph::loadenvmap(char *fn)
{
 int r=1;
 char bf[512];
 char *dirnames[6]={"right","left","top","bottom","back","front"};
 for (int i=0; i<6; i++)
 {
  sprintf(bf,"%s_%s.bmp",fn,dirnames[i]);
  envmap[i].SetAsCubeMapTex(i,CUBEMAPTEXID);
  r&=envmap[i].Load(bf);
 }
 return r;
}

void CGraph::drawenv()
{
#ifdef OPENGL
 if (envmapon<=0) return;
 
 float v[3*3*2*6]=
 {
  //x
   1, 1,-1,
   1, 1, 1,
   1,-1, 1,
   
   1, 1,-1,
   1,-1, 1,
   1,-1,-1,

  //-x
  -1,-1, 1,
  -1, 1, 1,
  -1, 1,-1,
   
  -1,-1,-1,
  -1,-1, 1,
  -1, 1,-1,
  
  //y
  -1, 1, 1,
   1, 1, 1,
   1, 1,-1,
   
  -1, 1, 1,
   1, 1,-1,
  -1, 1,-1,
   
  //-y
  
   1,-1,-1,
   1,-1, 1,
  -1,-1, 1, 
   
  -1,-1,-1,
   1,-1,-1,
  -1,-1, 1,
  
  //z
   1,-1, 1,
   1, 1, 1,
  -1, 1, 1,
   
  -1,-1, 1,
   1,-1, 1,
  -1, 1, 1,
  
  //-z
  -1, 1,-1,
   1, 1,-1,
   1,-1,-1,
   
  -1, 1,-1,
   1,-1,-1,
  -1,-1,-1
  
 };

 float t[2*3*2*6]=
 {  
  //x
   1, 0,
   0, 0,
   0, 1,
  
   1, 0,
   0, 1,
   1, 1,
  //-x
   1, 1,
   1, 0,
   0, 0,
  
   0, 1,
   1, 1,
   0, 0,
  
  //y
   0, 1,
   1, 1,
   1, 0,
  
   0, 1,
   1, 0,
   0, 0,
  
  //-y
   
   1, 1,
   1, 0,
   0, 0,
  
   0, 1,
   1, 1,
   0, 0,
  
  //z
   
   1, 1,
   1, 0,
   0, 0,
  
   0, 1,
   1, 1,
   0, 0,
  
  //-z
   1, 0,
   0, 0,
   0, 1,
  
   1, 0,
   0, 1,
   1, 1
   
 };
 
 if (gl_used)
 {
 
 float ambientcolor=lightdist*(1.f/255.f);
 if (ambientcolor>1.f) ambientcolor=1.f;
 glColor4f(ambientcolor,ambientcolor,ambientcolor,1);
 /*for (int i=0; i<6; i++) envmap[i].SetGL(1);
 glEnableClientState(GL_VERTEX_ARRAY);
 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 
 glVertexPointer(3,GL_FLOAT,0,&v);
 //glTexCoordPointer(3,GL_FLOAT,0,&v);
 glTexCoordPointer(2,GL_FLOAT,0,&t);

 glDisable(GL_DEPTH_TEST);
 glDrawArrays(GL_TRIANGLES,0,6*2*3);
 glEnable(GL_DEPTH_TEST);
  
 glDisableClientState(GL_VERTEX_ARRAY);
 glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 envmap[0].SetGL(0);*/
 //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
 
 glDisable(GL_DEPTH_TEST);
 glEnableClientState(GL_VERTEX_ARRAY);
 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 
 for (int i=0; i<6; i++)
 {
  envmap[i].SetGL(SETGL_TEX_CUBE_MAP_AS_2D_ON);
  glVertexPointer(3,GL_FLOAT,0,&v[2*3*3*i]);
  glTexCoordPointer(2,GL_FLOAT,0,&t[2*2*3*i]);
 
  glDrawArrays(GL_TRIANGLES,0,2*3);
   
  envmap[i].SetGL(SETGL_TEX_CUBE_MAP_AS_2D_OFF);
 }
 glDisableClientState(GL_VERTEX_ARRAY);
 glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 
 glEnable(GL_DEPTH_TEST);
 }

#endif
}

//rysowanie funkcji
int CGraph::drawfunc(CExpr *expr,FUNCSTRUCT *f)
{
 if (CSys::GetTime()-timer>MAXDRAWINGTIME) return 0;
 
 if (DMode==K2DF1)
 {
  drawfunc_K2DF1(expr,f);
 }
 
 else
 if (DMode==K2DXY)
 {
  if (!drawfunc_K2DXY(expr,f)) return 0;
 }
 
 else
 if (DMode==K2DF2)
 {
  if (!drawfunc_K2DF2(expr,f)) return 0;
 }
 
 else
 if (DMode==K3DF2)
 { 
  //etap 1 - wyliczenie tablicy valtab dla danego okregu O((sx,sy),R)
  if (hold>0) if (f->color!=hold-1) return 1;
  if ((hold<1)&&(dstate==0))
  {
#ifdef OPENGL
   if (gl_used)
   {
    if (gl_compvertextab(expr,f)==0) return 0;
   }
   else
#endif
   if (compvaltab(expr,f)==0) return 0;
  }
  else f->status=2;

  //etap 2 - rysowanie funkcji
#ifdef OPENGL
  if (gl_used)
  {
   if (dmethod==MLINES)
   {
    if (!drawfunc_K3DF2_gl_grid(f)) return 0;
   }
   else
   if (dmethod==MFILL)
   {
    if (lighting)
    {
     if (!drawfunc_K3DF2_gl_fill_light(f)) return 0;
    }
    else
    {
     if (!drawfunc_K3DF2_gl_fill(f)) return 0;
    }
   }
   else
   if (dmethod==MTEX)
   {
    if (!drawfunc_K3DF2_gl_fill_tex(f)) return 0;
   }
  }
  else
#endif
  {
   if (dmethod==MLINES)
   {
    if (!drawfunc_K3DF2_soft_grid(f)) return 0;
   }
   else
   if (dmethod==MFILL)
   {
    if (lighting)
    {
     if (!drawfunc_K3DF2_soft_fill_light(f)) return 0;
    }
    else
    {
     if (!drawfunc_K3DF2_soft_fill(f)) return 0;
    }
   }
   else
   if (dmethod==MTEX)
   {
    if (lighting)
    {
     if (!drawfunc_K3DF2_soft_fill_tex_light(f)) return 0;
    }
    else
    {
     if (!drawfunc_K3DF2_soft_fill_tex(f)) return 0;
    }
   }
  }
 }
 return 1;
}

void CGraph::allocbuffers()
{
 int nalloc=0;
#ifdef OPENGL
 if (!gl_used)
#endif
 {
  if ((!valtab)||(!projecttab)) nalloc+=sizeof(float)+sizeof(IPOINT);
 }
 if ((lighting)&&(!normaltab)) nalloc+=sizeof(VEC);
 if ((dmethod==MTEX)&&(!texcoordtab)) nalloc+=sizeof(VEC2);
#ifdef OPENGL
 if (gl_used)
 {
  if ((!gl_colortab)||(!gl_vertextab)||(!gl_indextab)) 
  nalloc+=sizeof(IVEC4)+sizeof(VEC)+8*sizeof(int);
 }
#endif
  
 //inicjacja wszystkich buforow
 int Npom=(int)(((cx2-cx1>cy1-cy2)? cx2-cx1:cy1-cy2)/D);
 if ((Npom!=N)||(nalloc))
 {
  nalloc+=sizeof(unsigned char);
  CSys::FreeMem(valtab); valtab=0;
  CSys::FreeMem(projecttab); projecttab=0;
  CSys::FreeMem(normaltab); normaltab=0;
  CSys::FreeMem(colortab); colortab=0;
  CSys::FreeMem(texcoordtab); texcoordtab=0;
#ifdef OPENGL
  CSys::FreeMem(gl_colortab); gl_colortab=0;
  CSys::FreeMem(gl_vertextab); gl_vertextab=0;
  CSys::FreeMem(gl_indextab); gl_indextab=0;
#endif
   
  unsigned int memavail=CSys::MemAvail();
  unsigned int minmemavail=CSys::MemTotal()/16;
  unsigned int memreq=GRAPHTAB_SIZE(Npom)*nalloc;

  if (memavail<memreq+minmemavail)
  {
   D*=2;
   Npom/=2;
  }
    
  N=Npom;
  int size=GRAPHTAB_SIZE(N);

#ifdef OPENGL
  if (!gl_used)
#endif
  {
   valtab=(float*)CSys::AllocMem(size*sizeof(float));
   memset(valtab,0,size*sizeof(float));
   
   projecttab=(IPOINT*)CSys::AllocMem(size*sizeof(IPOINT));
   memset(projecttab,0,(size)*sizeof(IPOINT));
  }
    
  if (lighting)
  {
   normaltab=(VEC*)CSys::AllocMem(size*sizeof(VEC));
   memset(normaltab,0,(size)*sizeof(VEC));
  }
  
  colortab=(unsigned char*)CSys::AllocMem(size*sizeof(unsigned char));
  memset(colortab,0,(size)*sizeof(unsigned char));
  
  if (dmethod==MTEX)
  {
   texcoordtab=(VEC2*)CSys::AllocMem(size*sizeof(VEC2));
   memset(texcoordtab,0,(size)*sizeof(VEC2));
  }

#ifdef OPENGL   
  if (gl_used)
  {    
   gl_colortab=(IVEC4*)CSys::AllocMem(size*sizeof(IVEC4));
   memset(gl_colortab,0,(size)*sizeof(IVEC4));
   
   gl_vertextab=(VEC*)CSys::AllocMem(size*sizeof(VEC));
   memset(gl_vertextab,0,(size)*sizeof(VEC));
  
   gl_indextab=(int*)CSys::AllocMem(8*size*sizeof(int));
   memset(gl_indextab,0,8*size*sizeof(int));
  }
#endif
 }
}

void CGraph::begindrawfunc()
{
 expr_time=CSys::GetTime();
 CSys::PresentWait=0;
 GraphState=(FillMode*(dmethod!=MLINES))|
  (EnableTexture*(dmethod==MTEX))|
  (EnableLight*(lighting>0))|
  (Enable3DMode*(DMode==K3DF2))
#ifdef OPENGL
  |(EnableOpenGL*(gl_used>0))
#endif
 ;
 if (DMode==K3DF2)
 {
  //obliczenie sinusow i cosinusow
  sinax=sin(A[AxisX]);
  cosax=cos(A[AxisX]);
  sinay=sin(A[AxisY]);
  cosay=cos(A[AxisY]);
  sinaz=sin(A[AxisZ]);
  cosaz=cos(A[AxisZ]);
  if (modlvec==1)
  {
   if (light_vec[3]==0)
   {
    //ustaw wektor swiatla zgodnie z kierunkiem kamery 
    light_vec[1]=-sinax;
    light_vec[2]=cosax;
    light_vec[0]=light_vec[2]*sinaz;
    light_vec[2]=light_vec[2]*cosaz;
   }
   else
   if (light_vec[3]==1)
   {
    //ustaw wektor swiatla na pozycji kamery
    light_vec[0]=j[AxisX];
    light_vec[1]=j[AxisZ];
    light_vec[2]=-j[AxisY];
   }
  }
  CSys::PresentWait=1;
  xs=j[AxisX];
  ys=j[AxisY];
  zs=j[AxisZ];
#ifdef OPENGL
  if (gl_used)
  {
   glClearColor(GLCOLOR(CSys::Color[CSys::CPattern]),1.f);
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   glRotatef(-A[AxisX]*(180.f/3.141592f),1,0,0);
   glRotatef(-A[AxisZ]*(180.f/3.141592f),0,1,0);
   drawenv();
   if (light_vec[3]==0)
   glLightfv(GL_LIGHT0,GL_POSITION,(GLfloat *)&light_vec);
   glTranslatef(-j[AxisX],-j[AxisZ],j[AxisY]);
   if (light_vec[3]==1)
   glLightfv(GL_LIGHT0,GL_POSITION,(GLfloat *)&light_vec);
#ifdef CG
   if (useshaders==1)
   {
    bindcgprogram(1);
   }
   else
   if (useshaders==0)
   {
    bindcgprogram(0);
   }
#endif
  }
  else
#endif
  if (dmethod!=MLINES)
  {
   memset(zbuf,0xff,width*height*4);
  }
  if (hold==0)
  {
   hxs=j[AxisX];
   hys=j[AxisY];
   sqcircorners(hxs,hys,cx1,cy1,cx2,cy2);
  }
  allocbuffers();
  CSys::PresentWait=1;
 }
 if (repmode) CSys::PresentWait=1;
#ifdef OPENGL
 if ((!gl_used)||(!Is3DMode())) Clear();
 //if (!gl_used) drawenv();
#else
 Clear();
#endif
 if (axison&&(!Is3DMode())) drawaxis();
 CExpr::MultiExec=CExpr::MultiExec_Begin;
 dexprlist[MAXFUNCCOUNT].Do();
 CExpr::MultiExec=CExpr::MultiExec_Multi;
}

void CGraph::enddrawfunc()
{
 if (CExpr::MultiExec==CExpr::MultiExec_Once) return;
 if (DMode==K3DF2)
 {
#ifdef CG
  if (useshaders==1)
  {
   bindcgprogram(0);
  }
#endif
  if (lighting) drawsun();
 }
 
 CExpr::MultiExec=CExpr::MultiExec_End;
 dexprlist[MAXFUNCCOUNT+1].Do();
 
 if (DMode==K3DF2)
 {
#ifdef OPENGL
  if (gl_used)
  {
   COpenGL::Present(buf);
  }
#endif
 }
 drawinfo();
 if (fpsmode==0)
 {
  if (CSys::GetTime()-expr_time>0)
  fps=1/(CSys::GetTime()-expr_time);
 }
 else
 {
  fps=frames/(CSys::GetTime()-timer0);
 }
 frames++;
 CSys::PresentWait=0;
 CExpr::MultiExec=CExpr::MultiExec_Once;
}

void CGraph::drawaxis()
{
 char bf[64],bf0[64];
 int i;
 //rysuj osie
 HLine(0,height-(int)FP(0,AxisY),width-1,CSys::Color[CSys::CFaded]);
 VLine((int)FP(0,AxisX),0,height-1,CSys::Color[CSys::CFaded]);
 //pisz wartosci - os x
 float d=axisdelta(AxisX);
 float x1,x2;
 if ((int)d!=0)
 {
  x1=((int)FJ(0,AxisX)/(int)d)*(int)d;
  x2=(int)FJ(width,AxisX);
 }
 else
 {
  x1=(int)((1/d)*(FJ(0,AxisX)))/(1/d);
  x2=FJ(width,AxisX);
 }
 int x,y=height-(int)FP(0,AxisY);
 while (x1<=x2)
 {
  if (x1!=0)
  {
   x=(int)FP(x1,AxisX);
   if ((x>=0)&&(y>=0)&&(x<width)&&(y<height))
   PutPixel(x,y,CSys::Color[CSys::CNum]);
   if (gridon) VLine(x,0,height,CSys::Color[CSys::CFaded]);
   sprintf(bf0,"%%0.%df",geps);
   sprintf(bf,bf0,x1);
   i=CSys::DiscardZeros(bf);
   DrawText3X5(x-i*2-1,y+2,CSys::Color[CSys::CNum],bf);
  }
  x1+=d;
 }
 //pisz wartosci - os y
 d=axisdelta(AxisY);
 float y1,y2;
 if ((int)d!=0)
 {
  y1=((int)FJ(0,AxisY)/(int)d)*(int)d;
  y2=(int)FJ(height,AxisY);
 }
 else
 {
  y1=(int)((1/d)*(FJ(0,AxisY)))/(1/d);
  y2=FJ(height,AxisY);
 }
 x=(int)FP(0,AxisX);
 while (y1<=y2)
 {
  if (y1!=0)
  {
   y=height-(int)FP(y1,AxisY);
   if ((x>=0)&&(y>=0)&&(x<width)&&(y<height))
   PutPixel(x,y,CSys::Color[CSys::CNum]);
   if (gridon) HLine(0,y,width,CSys::Color[CSys::CFaded]);
   sprintf(bf0,"%%0.%df",geps);
   sprintf(bf,bf0,y1);
   i=CSys::DiscardZeros(bf);
   DrawText3X5(x+2,y-2,CSys::Color[CSys::CNum],bf);
  }
  y1+=d;
 }
}

float CGraph::axisdelta(int axis)
{
 float d;
 float ptab[3]={1,2,5};
 float m;
 int p=0;
 int sc=0;
 if (axis==AxisX) sc=width;
 else
 if (axis==AxisY) sc=height;
 
 d=sc/(FJ(sc,axis)-FJ(0,axis));
 if (d<MINADELTA)
 {
  m=1;
  do
  {
   p++;
   if (p>2) {p=0; m*=10;}
  }
  while (d*(ptab[p]*m)<MINADELTA);
  return ptab[p]*m;
 }
 if (d>=MINADELTA)
 {
  float pm;
  int pp;
  m=0.1;
  p=2;
  pp=0;
  pm=1;
  while (d*(ptab[p]*m)>=MINADELTA)
  {
   pp=p;
   pm=m;
   p--;
   if (p<0) {p=2; m/=10;}
  }
  return ptab[pp]*pm;
 }
 return 0; //dla formalnosci
}

void CGraph::handlemouse()
{
 int cx,cy;
 CSys::CursorPos(cx,cy);
 if ((cx!=(int)cursorx)||(cy!=(int)cursory))
 {
  if (Is3DMode())
  {
   A[AxisZ]+=(cursorx-(float)cx)/100;
   A[AxisX]+=(cursory-(float)cy)/100;
   CSys::CursorPosSet(width_div_2,height_div_2);
   cx=width_div_2;
   cy=height_div_2;
  }
  cursorx=cx;
  cursory=cy;
  if (reqredraw<1)
  reqredraw=1;
  if (DMode==K2DF1) expr_y=FJ(height-cursory,AxisY);
 }
}

void CGraph::drawinfo()
{
 char bf[128],bf0[128];
 int i,k;
 
 if ((titles)&&(dfuncstruct[CSys::DColor].status!=0))
 {
  //lista funkcji
  i=0;
  for (k=0; k<MAXFUNCCOUNT; k++)
  {
   if (dfuncstruct[k].status==0) continue;
   unsigned int c;
   if (CSys::DColor==dfuncstruct[k].color) c=Color[dfuncstruct[k].color];
   else c=Palette[dfuncstruct[k].color][128];
   DrawText3X5(0,i,c,dexprlist[k].ExprStr());
   i+=5;
  }
  
  //liczba klatek na sek.
  sprintf(bf0,"%%0.%df",geps);
  sprintf(bf,bf0,fps);
  DrawText3X5(width-strlen(bf)*4-1,0,CSys::Color[CSys::CHighlighted],bf);
 
  //pozycja kursora
  float x1=FJ(cursorx,AxisX);
  float y1=FJ(height-cursory,AxisY);
  if (!Is3DMode())
  {
   sprintf(bf0,"X=%%0.%df Y=%%0.%df",geps,geps);
   sprintf(bf,bf0,x1,y1);
   DrawText3X5(width-strlen(bf)*4-1,height-5,CSys::Color[CSys::CHighlighted],bf);
  }
  else
  {
#ifdef OPENGL
   if (gl_used)
    DrawText(width-7*fontwidth-1,height-fontheight-6,CSys::Color[CSys::CHelp],"OPENGL");
   else
#endif
    DrawText(width-8*fontwidth-1,height-fontheight-6,CSys::Color[CSys::CHelp],"SOFTWARE");
    DrawText3X5(width-8*fontwidth+1,height-6,CSys::Color[CSys::CFaded],"R E N D E R E R");
  }
  /*HLine((int)cursorx-1,(int)cursory,(int)cursorx+1,CSys::Color[CSys::CNormal]);
  VLine((int)cursorx,(int)cursory-1,(int)cursory+1,CSys::Color[CSys::CNormal]);*/
 
  //wartosci funkcji dla danej pozycji kursora
  if (DMode==K2DF1)
  {
   expr_x=x1;
   sprintf(bf0,"F(%%0.%df)=%%0.%df",geps,geps);
   sprintf(bf,bf0,x1,dexprlist[CSys::DColor].Do());
  }
  else
  if (DMode==K2DXY)
  {
   sprintf(bf0,"t1=%%0.%df t2=%%0.%df n=%%d",geps,geps);
   sprintf(bf,bf0,t1,t2,nt);
  }
  else
  if (DMode==K2DF2)
  {
   expr_x=x1;
   expr_y=y1;
   sprintf(bf0,"F(%%0.%df,%%0.%df)=%%0.%df",geps,geps,geps);
   sprintf(bf,bf0,x1,y1,dexprlist[CSys::DColor].Do());
  }
  else
  if (DMode==K3DF2)
  {
   sprintf(bf0,"X=%%0.%df Y=%%0.%df Z=%%0.%df D=%%0.%df R=%%0.%df V=%%d M=%%dk",
   geps,geps,geps,geps,geps);
   sprintf(bf,bf0,j[AxisX],j[AxisY],j[AxisZ],D,R,N*N,CSys::MemAvail()/1024);
  }
  DrawText3X5(0,height-5,Color[CSys::DColor],bf);
 }
}

void CGraph::Process()
{
 timer=CSys::GetTime();
 handlemouse();
 if (dstackl==-2)
 {
  dstackl=0;
 }
 else
 if (reqredraw)
 {
  if (Is3DMode())
  {
   if (dstackl!=-1)
   {
    if (reqredraw>1) dstackl=-2;
   }
   else dstackl=0;
  }
  else
  {
   if (dstackl!=-1)
   {
    if ((repmode==0)||(reqredraw>1)) dstackl=-2;
   }
   else dstackl=0;
  }
  reqredraw=0;
 }
 
 if (dstackl==-2)
 {
  enddrawfunc();
  return;
 }
 
 if (dstackl==0)
 {
  int i;
  if (dfuncstruct[CSys::DColor].status!=0)
  {
   dstack[dstackl++]=CSys::DColor;
   dfuncstruct[CSys::DColor].status=1;
  }
  for (i=0; i<MAXFUNCCOUNT; i++)
   if ((dfuncstruct[i].status!=0)&&(CSys::DColor!=i))
   {
    dstack[dstackl++]=i;
    dfuncstruct[i].status=1;
   }
  if (dstackl>0)
  begindrawfunc();
  else
  {
#define NOFUNCTEXT "No function defined!"
   Clear();
   DrawText(width_div_2-strlen(NOFUNCTEXT)*fontwidth/2,height_div_2-fontheight/2,
   CSys::Color[CSys::CHelp],NOFUNCTEXT);
  }
 }
 
 if (dstackl>0)
 {
  while (dstackl)
  {
   if (dfuncstruct[dstack[dstackl-1]].status==1)
   {
    dstate=0;
   }
   if (!drawfunc(&dexprlist[dstack[dstackl-1]],&dfuncstruct[dstack[dstackl-1]])) return;
   if (gamemodeon) updateplayer();
   dstackl--;
  }
  enddrawfunc();
  if (!repmode) dstackl=-1;
 }
 gms.moveplayer=0;
 if (hold==-1) hold=CSys::DColor+1;
}

void CGraph::ChangePos(float d,int axis)
{
 j[axis]+=d/s[axis];
 updatepos();
}

void CGraph::ChangePos3D(float d,float anglex,float anglez)
{
 if (gamemodeon)
 {
  d/=fabs(d);
  gms.player_accv.u=d*-sin(anglez)*gms.player_acc;
  gms.player_accv.v=d*cos(anglez)*gms.player_acc;
  gms.moveplayer|=1;
 }
 else
 {
  j[AxisX]-=d*sin(anglez)*cos(anglex);
  j[AxisY]+=d*cos(anglez)*cos(anglex);
  j[AxisZ]+=d*sin(anglex);
 }
 updatepos();
}

void CGraph::ChangeScale(float d,int axis)
{
 s[axis]*=d;
 updatepos();
}

void CGraph::SetExpr(char *name,CExpr *expr,int color,int num)
{
 if (num>=MAXFUNCCOUNT)
 {
  dexprlist[num%(MAXFUNCCOUNT+2)]=expr;
 }
 else
 {
  dexprlist[num%MAXFUNCCOUNT]=expr;
  dfuncstruct[num%MAXFUNCCOUNT].color=color%(sizeof(Color)/sizeof(int));
  dfuncstruct[num%MAXFUNCCOUNT].status=1;
 }
}

void CGraph::DelExpr(int num)
{
 dfuncstruct[num%MAXFUNCCOUNT].status=0;
 NextExpr();
}

void CGraph::NextExpr()
{
 int k=CSys::DColor+1;
 for (int i=0; i<MAXFUNCCOUNT; i++) 
 if (dfuncstruct[(k+i)%MAXFUNCCOUNT].status!=0) 
 {
  CSys::DColor=(k+i)%MAXFUNCCOUNT;
  return;
 }
 CSys::DColor=0;
}
