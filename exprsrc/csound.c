//csound.c

unsigned int CSound::Color[MAXFUNCCOUNT]=
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

CSound *CSound::currentsound;

void CSound::KeyFunc(int k)
{
 float m=1,s=1,dir=1;
 if (k>>8!=0) {m=4; s=1.2; dir=-1;}
 k&=0xff;
 if (m>1)
 {
  if (k==K_UP) Ajy+=DELTAAJY;
  else
  if (k==K_DOWN) Ajy-=DELTAAJY;
  else
  if (k==K_LEFT) Tjx/=DELTATJX;
  else
  if (k==K_RIGHT) Tjx*=DELTATJX;
 }
 else
 {
  if (k==K_UP) ChangeVolPan(CSys::DColor,DELTAVOL,0);
  else
  if (k==K_DOWN) ChangeVolPan(CSys::DColor,-DELTAVOL,0);
  else
  if (k==K_LEFT) ChangeVolPan(CSys::DColor,0,-DELTAPAN);
  else
  if (k==K_RIGHT) ChangeVolPan(CSys::DColor,0,DELTAPAN);
 }
 if (k==K_PGUP) {vsize-=DELTAVSIZE*m; if (vsize<DELTAVSIZE) vsize=DELTAVSIZE;}
 else
 if (k==K_PGDOWN) vsize+=DELTAVSIZE*m;
 else
 if (k==K_SPACE) paused=1-paused;
 else
 if (k==K_TAB) NextExpr();
 else
 if (k==K_HOME) defaults();
 else
 if (k==K_ENTER) Reset(CSys::DColor);
 else
 if (k==K_DEL) DelExpr(CSys::DColor);
 else
 if (k==K_S) sfuncstruct[CSys::DColor].sat=1-sfuncstruct[CSys::DColor].sat;
 else
 if (k==K_R) sfuncstruct[CSys::DColor].rep=1-sfuncstruct[CSys::DColor].rep;
 else
 if (k==K_I) inputon=1-inputon;
 else
 if (k==K_LSBRACKET) sfuncstruct[CSys::DColor].t1-=DELTAT*m;
 else
 if (k==K_RSBRACKET) sfuncstruct[CSys::DColor].t1+=DELTAT*m;
 else
 if (k==K_SEMICOLON) sfuncstruct[CSys::DColor].t2-=DELTAT*m;
 else
 if (k==K_APOSTROPHE) sfuncstruct[CSys::DColor].t2+=DELTAT*m;
 
#ifdef MENU
 setmenustate();
#endif
}

#ifdef MENU
void CSound::initmenu()
{
 CSys::Menu.AddItem(MENU_SOUND,"Pause\tSpace",K_SPACE*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"&Reset\tEnter",K_ENTER*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"---",0);
 CSys::Menu.AddItem(MENU_SOUND,"&Freq++\tShift+Left Arrow",32768+K_LEFT*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"Fr&eq--\tShift+Right Arrow",32768+K_RIGHT*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"&Aplitude++\tShift+Up Arrow",32768+K_UP*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"Apl&itude--\tShift+Down Arrow",32768+K_DOWN*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"---",0);
 CSys::Menu.AddItem(MENU_SOUND,"T1&++\t]",K_RSBRACKET*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"T1&--\t[",K_LSBRACKET*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"&T2++\t'",K_APOSTROPHE*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"T&2--\t;",K_SEMICOLON*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"---",0);
 CSys::Menu.AddItem(MENU_SOUND,"&Satuation\tS",K_S*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"A&uto repeat mode\tR",K_R*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"&Defaults\tHome",K_HOME*256+MENU_SOUND);
 CSys::Menu.AddItem(MENU_SOUND,"---",0);
 CSys::Menu.AddItem(MENU_SOUND,"In&put\tI",K_I*256+MENU_SOUND);
 setmenustate();
 setmenuactivestate(0);
}

void CSound::setmenustate()
{
 CSys::Menu.SetItemInfo(K_SPACE*256+MENU_SOUND,MENU_CH(paused));
 CSys::Menu.SetItemInfo(K_S*256+MENU_SOUND,MENU_CH(sfuncstruct[CSys::DColor].sat));
 CSys::Menu.SetItemInfo(K_R*256+MENU_SOUND,MENU_CH(sfuncstruct[CSys::DColor].rep));
 CSys::Menu.SetItemInfo(K_I*256+MENU_SOUND,MENU_CH(inputon));
}

void CSound::setmenuactivestate(int state)
{
 CSys::Menu.SetItemInfo(MENU_SOUND,MENU_ONOFF(state));
}
#endif

void CSound::compsoundbuf(int n)
{
 unsigned int i;
 float dx=1.f/(Tjx*(float)CSys::SoundFreq);
 //expr_x=sfuncstruct[n].x0+sfuncstruct[n].pos/Tjx;
 unsigned int sbsize=CSys::SoundMixerBufSize*8/CSys::SoundBPS/CSys::SoundChannels;
 int di=CSys::SoundBPS/8;
 int sat=sfuncstruct[n].sat;
 int rep=sfuncstruct[n].rep;
 void *bptr=smixerchannels[n].bufptr;
 int pom,cursory;
 CSys::CursorPos(pom,cursory);
 expr_y=-(cursory-height/2)/(FUNCOSCH*(float)height/2);
 
 int idi=di*CSys::SoundChannels;
 
 for (i=0; i<sbsize; i++)
 {
  if ((inputon==1)&&(inputbuf))
  {
   unsigned int l=i*idi;
   if (di==1)
   expr_t=(float)(*(unsigned char*)((int)inputbuf+l)-128)/127.f;
   else
   expr_t=(float)(*(short*)((int)inputbuf+l))/32767.f;
  }
  expr_x=sfuncstruct[n].x;    
  float y=Ajy*sexprlist[n].Do();
  if (sat)
  {
   if (y>1.f) y=1.f;
   else
   if (y<-1.f) y=-1.f;
  }
  if (di==1)
  *(char*)bptr=ifloor(y*127.f)+128;
  else
  *(short*)bptr=ifloor(y*32767.f);
  bptr=(void*)((int)bptr+di);
  
  sfuncstruct[n].pos=x2pos(sfuncstruct[n].x);
  if (rep)
  {
   if (sfuncstruct[n].pos>sfuncstruct[n].t2)
   {
    sfuncstruct[n].pos=sfuncstruct[n].t1;
    sfuncstruct[n].x=pos2x(sfuncstruct[n].pos);
   }
  }
  
  sfuncstruct[n].x+=dx;
 }
}

//rysuje oscyloskop dla danej funkcji
void CSound::drawosc(int n)
{
 int i,color=Color[n];
 float dx=vsize/(width*Tjx);
 float fh=FUNCOSCH*(float)height/2;
 expr_x=pos2x(sfuncstruct[n].pos-vsize/2);
 int sat=sfuncstruct[n].sat;
 float y1,y2;
 y1=Ajy*sexprlist[n].Do();
 expr_x+=dx;
 
 int idi=CSys::SoundBPS*CSys::SoundChannels/8;
 for (i=1; i<width; i++)
 {
  if ((inputon==1)&&(inputbuf))
  {
   unsigned int l=(int)((float)i*(float)(CSys::SoundMixerBufSize/idi)/(float)width)*idi;
   if (CSys::SoundBPS==8)
   expr_t=(float)(*(unsigned char*)((int)inputbuf+l)-128)/127.f;
   else
   expr_t=(float)(*(short*)((int)inputbuf+l))/32767.f;
  }
  y2=Ajy*sexprlist[n].Do();
  if (sat)
  {
   if (y1>1) y1=1;
   if (y2>1) y2=1;
   if (y1<-1) y1=-1;
   if (y2<-1) y2=-1;
  }
  else
  {
   y1=fmod(y1,1);
   y2=fmod(y2,1);
  }
  int iy1,iy2;
  iy1=ifloor(height/2-y1*fh);
  iy2=ifloor(height/2-y2*fh);
  VLine(i-1,iy1,(iy1+iy2)/2,color);
  VLine(i,(iy1+iy2)/2,iy2,color);
  
  y1=y2;
  expr_x+=dx;
 }
}

void CSound::drawinfo()
{
 char bf[128];
 int i,k;
 
 //lista funkcji
 i=0;
 for (k=0; k<MAXFUNCCOUNT; k++)
 {
  if (sfuncstruct[k].status==0) continue;
  unsigned int c;
  if (CSys::DColor==k) c=Color[k];
  else c=CSys::FadeColor(Color[k],128);
  DrawText3X5(0,i,c,sexprlist[k].ExprStr());
  i+=5;
 }
 
 //dane globalne
 sprintf(bf,"TX=%0.4f (%0.2f) AY=%0.2f V=%0.2f",Tjx,1/Tjx,Ajy,vsize);
 DrawText3X5(width-strlen(bf)*4-1,0,CSys::Color[CSys::CHighlighted],bf);
 
 int px,py,w,h;
 
 //ustawienia miksera
 
 px=ifloor(MIXERSETUPX*(float)width);
 py=ifloor(MIXERSETUPY*(float)height);
 w=width-2*px;
 h=ifloor(MIXERSETUPHEIGHT*(float)height);
 
 sprintf(bf,"MIXER(%dHZ,%dBPS,%s)",CSys::SoundFreq,CSys::SoundBPS,
 (CSys::SoundChannels==1)? "MONO":"STEREO");
 DrawText3X5(width/2-strlen(bf)*2,py-8,CSys::Color[CSys::CHelp],bf);
 
 int bh=ifloor(MIXERSETUPBARHEIGHT*(float)h);
 int bw=ifloor((float)w*(1.f/8.f)*MIXERSETUPBARWIDTH);
 int dw=ifloor((float)w*(1.f/8.f));
 for (i=0; i<MAXFUNCCOUNT; i++)
 {
  int col=Color[i];
  //vol
  VLine(px+bw/2,py,py+bh,col);
  HLine(px+bw/2-2,py+ifloor((1-smixerchannels[i].vol)*(float)bh),
        px+bw/2+2,col);
  //pan
  HLine(px,py+h,px+bw,col);
  VLine(px+ifloor((smixerchannels[i].pan+1)*(float)bw*.5f),py+h-2,py+h+2,col);
  
  px+=dw;
 }
 
 //funkcja
 h=ifloor(FUNCOSCH*(float)height);
 HLine(0,height/2,width-1,CSys::Color[CSys::CFaded]);
 VLine(width/2,(height-h)/2,(height+h)/2,CSys::Color[CSys::CFaded]);
 int isf=0;
 for (i=0; i<MAXFUNCCOUNT; i++)
 {
  if (sfuncstruct[i].status)
  {
   drawosc(i);
   isf=1;
  }
 }
 
 if (!isf)
 {
  #define NOFUNCTEXT "No function defined!"
  DrawText(width/2-strlen(NOFUNCTEXT)*fontwidth/2,height/2-fontheight/2,
  CSys::Color[CSys::CHelp],NOFUNCTEXT);
 }
 
 //dane funckji
 if (sfuncstruct[CSys::DColor].status)
 {
  sprintf(bf,"X=%0.2f POS=%0.2f T1=%0.2f T2=%0.2f SAT=%d REP=%d",
  sfuncstruct[CSys::DColor].x,sfuncstruct[CSys::DColor].pos,
  sfuncstruct[CSys::DColor].t1,sfuncstruct[CSys::DColor].t2,
  sfuncstruct[CSys::DColor].sat,sfuncstruct[CSys::DColor].rep);
  DrawText3X5(0,height-5,Color[CSys::DColor],bf);
  if (sfuncstruct[CSys::DColor].rep)
  {
   VLine(width/2+ifloor((float)width/vsize*(-sfuncstruct[CSys::DColor].pos
   +sfuncstruct[CSys::DColor].t1)),
   (height-h)/2,(height+h)/2,CSys::Color[CSys::CFavour]);
   VLine(width/2+ifloor((float)width/vsize*(-sfuncstruct[CSys::DColor].pos
   +sfuncstruct[CSys::DColor].t2)),
   (height-h)/2,(height+h)/2,CSys::Color[CSys::CFavour]);
  }
 }
 if (inputon)
 {
  DrawText(width-fontwidth*3,height-fontheight,CSys::Color[CSys::CHelp],"MIC");
 }
 //kursor
 /*int cursorx,cursory;
 CSys::CursorPos(cursorx,cursory);
 HLine(cursorx-1,cursory,cursorx+1,CSys::Color[CSys::CNormal]);
 VLine(cursorx,cursory-1,cursory+1,CSys::Color[CSys::CNormal]);*/
}

void CSound::Process()
{
 int i;
 CSys::PresentWait=0;
 
 //miksuj
 if (!CSys::SetSound()) 
 {
  #define SETUPSOUNDTEXT "Setting up sound..."
  DrawText(width/2-strlen(SETUPSOUNDTEXT)*fontwidth/2,height/2-fontheight/2,
  CSys::Color[CSys::CHelp],SETUPSOUNDTEXT);
  return;
 }
 if (inputon) CSys::RecordInputSound();
 if (!CSys::IsMixerAvailable())
 {
  return;
 }
 if (inputon)
 {
  inputbuf=CSys::FlushInputSoundBuf();
  //if (inputbuf==0) inputon=2;
 }
 expr_time=CSys::GetTime();
 Clear();
 //rysuj informacje
 CExpr::MultiExec=CExpr::MultiExec_Multi;
 drawinfo();
 CExpr::MultiExec=CExpr::MultiExec_Once;
 
 CExpr::MultiExec=CExpr::MultiExec_Multi;
 int ise=0;
 if (paused) goto nocomp;
 for (i=0; i<MAXFUNCCOUNT; i++)
 {
  if (sfuncstruct[i].status>0)
  {
   smixerchannels[i].bufptr=CSys::AllocMem(CSys::SoundMixerBufSize);
   smixerchannels[i].offset=0;
   smixerchannels[i].size=CSys::SoundMixerBufSize;
   smixerchannels[i].flags=MIXER_ACTIVE;
   compsoundbuf(i);
   ise=1;
  }
  else
  {
   smixerchannels[i].flags=0;
  }
 }
nocomp:
 CExpr::MultiExec=CExpr::MultiExec_Once;
 if (ise==0)
 {
  smixerchannels[0].bufptr=CSys::AllocMem(CSys::SoundMixerBufSize);
  memset(smixerchannels[0].bufptr,0,CSys::SoundMixerBufSize);
  smixerchannels[0].offset=0;
  smixerchannels[0].size=CSys::SoundMixerBufSize;
  smixerchannels[0].flags=MIXER_ACTIVE;
 }
 
 CSys::MixSounds(smixerchannels,MAXFUNCCOUNT);
 
 if (ise)
 {
 for (i=MAXFUNCCOUNT-1; i>=0; i--)
 {
  if (sfuncstruct[i].status>0)
  {
   CSys::FreeMem(smixerchannels[i].bufptr);
  }
 }
 }
 else
 {
  CSys::FreeMem(smixerchannels[0].bufptr);
 }
 //if (inputon) inputon=1;
}

void CSound::SetExpr(char *name,CExpr *expr,int num)
{
 int n=num%MAXFUNCCOUNT;
 Reset(n);
 sexprlist[n]=expr;
 sfuncstruct[n].status=1;
}

void CSound::DelExpr(int num)
{
 sfuncstruct[num%MAXFUNCCOUNT].status=0;
 NextExpr();
}

void CSound::NextExpr()
{
 int k=CSys::DColor+1;
 for (int i=0; i<MAXFUNCCOUNT; i++) 
 if (sfuncstruct[(k+i)%MAXFUNCCOUNT].status!=0) 
 {
  CSys::DColor=(k+i)%MAXFUNCCOUNT;
  return;
 }
 CSys::DColor=0;
}

void CSound::ChangeVolPan(int n,float dvol,float dpan)
{
 int nmod=n%MAXFUNCCOUNT;
 smixerchannels[nmod].vol+=dvol;
 smixerchannels[nmod].pan+=dpan;
 if (smixerchannels[nmod].vol>1) smixerchannels[nmod].vol=1;
 if (smixerchannels[nmod].vol<0) smixerchannels[nmod].vol=0;
 if (smixerchannels[nmod].pan>1) smixerchannels[nmod].pan=1;
 if (smixerchannels[nmod].pan<-1) smixerchannels[nmod].pan=-1;
}

void CSound::Reset(int n)
{
 sfuncstruct[n].pos=sfuncstruct[n].t1;
 sfuncstruct[n].x=pos2x(sfuncstruct[n].pos);
}
