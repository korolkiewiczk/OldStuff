//ccon.c

CCon *CCon::currentcon;

void CCon::Edit(char c)
{
 int w=wske%EDIT;
 if ((c==8)&&(elen>0)&&(ecursor>0))
 {
  if (ecursor!=elen)
  {
   for (int i=ecursor-1; i<elen; i++) edit[w][i]=edit[w][i+1];
  }
  elen--;
  ecursor--;
  edit[w][elen]=0;
 }
 else
 if ((c==127)&&(elen>0)&&(ecursor!=elen))
 {
  for (int i=ecursor; i<elen; i++) edit[w][i]=edit[w][i+1];
  elen--;
  edit[w][elen]=0;
 }
 else
 if (c=='\n') Enter();
 else
 if ((c!=127)&&(c!=8)&&(elen<BUFLEN-1)&&(c>=32))
 {
  if (ecursor==elen)
  edit[w][ecursor++]=c;
  else
  {
   for (int i=elen-1; i>=ecursor; i--) edit[w][i+1]=edit[w][i];
   edit[w][ecursor++]=c;
  }
  elen++;
  edit[w][elen]=0;
 }
}

void CCon::Enter()
{
 if (edit[wske%EDIT][0]==0) return;
 currentcon=this;
 char *bf=reffunc(edit[wske%EDIT]);
 if (strlen(bf)>=BUFLEN) bf[BUFLEN-1]=0;
 if (bf==0)
 {
  strcpy(lines[wskl%LINES],edit[wske%EDIT]);
 }
 else
 {
  int l=0,l2=0;
  //int lim=width/8-8;
  int sl=strlen(bf);
  char bufl[BUFLEN];
  while (l<sl)
  {
   while ((bf[l]!=0)&&(bf[l]!='\n')) l++;
   memcpy(bufl,bf+l2,l);
   bufl[l-l2]=0;
   //if (strlen(bufl)>lim) {bufl[lim-3]='.'; bufl[lim-2]='.'; bufl[lim-1]='.'; bufl[lim]=0;}
   strcpy(lines[wskl%LINES],bufl);
   l2=++l;
   wskl++;
  }
  wskl--;
 }
 CSys::Log_Printf(edit[wske%EDIT]);
 wskl++;
 if (wske>0)
 {
  if (strcmp(edit[wske%EDIT],edit[(wske-1)%EDIT])==0) wske--;
 }
 wske++;
 strcpy(edit[wske%EDIT],"");
 ecursor=elen=0;
 wskh=wske;
 wsklv=wskl;
 estart=0;
 colshf=0;
}

void CCon::KeyFunc(int k)
{
 int shift=(k>>8)>0;
 if (CSys::MouseKey()==1) k|=K_ENTER;
 if (CSys::MouseKey()==2) k|=K_ESC;
 k&=255;
 if (k==K_TAB)
 {
  libwinon=1-libwinon;
  if (libwinon)
  {
   libwin->Clear();
   libwin->Set("");
  }
 }
 else
 if (k==K_ESC)
 {
  if (libwinon) libwinon=0;
  else
  {             
   strcpy(edit[wske%EDIT],"");
   ecursor=elen=0;
   wskh=wske;
   estart=0;
  }
 }
 else
 if (k==K_LEFT)
 {
  if (shift) {if (colshf>0) colshf--;}
  else
  if (ecursor>0) ecursor--;
 }
 else
 if (k==K_RIGHT)
 {
  if (shift) colshf++;
  else
  if (ecursor<elen) ecursor++;
 }
 else
 if (libwinon)
 {
  libwin->KeyFunc(k);
  int i=0;
  if (libwin->retbuf[0]!=0)
  {
   while (libwin->retbuf[i]!=0)
   {
    Edit(libwin->retbuf[i++]);
   }
   libwin->retbuf[0]=0;
  }
 }
 else
 if (k==K_HOME) ecursor=0;
 else
 if (k==K_END) ecursor=elen;
 else
 if (k==K_UP)
 {
  if (wskh>0)
  strcpy(edit[wske%EDIT],edit[(--wskh)%EDIT]);
  ecursor=elen=strlen(edit[wske%EDIT]);
  estart=0;
 }
 else
 if (k==K_DOWN)
 {
  if (wskh<wske-1)
  strcpy(edit[wske%EDIT],edit[(++wskh)%EDIT]);
  else
  if (wskh<wske)
  {
   strcpy(edit[wske%EDIT],"");
   wskh++;
  }
  ecursor=elen=strlen(edit[wske%EDIT]);
  estart=0;
 }
 else
 if (k==K_PGUP)
 {
  if (wsklv>0)
  wsklv--;
 }
 else
 if (k==K_PGDOWN)
 {
  if (wsklv<wskl)
  wsklv++;
 }
 else
 if ((k==K_ENTER)&&(shift))
 {
  char *t;
  t=CSys::GetClipboardText();
  if (t)
  while (*t) Edit(*(t++));
 }
 else
 {
 int i;
 char c=0;
 for (i=0; i<KEYMAPLEN; i++)
 {
  if (keymap[i][0]==k)
  {
   c=keymap[i][shift+1];
   break;
  }
 }
 //DEBUG_Printf("%d %d %d\n",k,k&255,k>>8);
 if (c=='\n') Enter();
 else
 if (c!=0)
 Edit(c);
 }
 if ((ecursor-estart)<0) estart=ecursor;
 int ewidth=(width-fontwidth)/fontwidth-strlen(prompt);
 if (ecursor-estart>=ewidth) estart=ecursor-ewidth+1;
}

void CCon::Process()
{
 if (fexecsl!=-1)
 {
  char bf[BUFLEN];
  fgets(bf,BUFLEN,fexecstack[fexecsl]);
  int l=strlen(bf);
  while ((l>=0)&&(bf[--l]<=32));
  bf[l+1]=0;
  l=0;
  while ((bf[l]<=32)&&(bf[l]!=0)) l++;
  int i=0;
  while (bf[l]!=0) bf[i++]=bf[l++];
  bf[i]=0;
  if (feof(fexecstack[fexecsl])) {fclose(fexecstack[fexecsl]); fexecsl--;}
  if (((bf[0]!='/')||(bf[1]!='/'))&&(bf[0]!=0))
  {
   if (echo==1)
   {
    strcpy(edit[wske%EDIT],bf);
    Enter();
   }
   else
   {
    reffunc(bf);
   }
  }
 }
 
 Clear();
 HLine(0,height-fontheight-4,width-1,CSys::Color[CSys::CFaded]);
 int plen=1;
 if (prompt)
 {
  plen+=strlen(prompt)*fontwidth;
 }
 int ewidth=(width-fontwidth)/fontwidth-strlen(prompt);
 char *pom=edit[wske%EDIT]+estart+ewidth;
 char cpom=*pom;
 *pom=0;
 DrawTextHighlighted(-estart*fontwidth+plen,height-fontheight-2,
 CSys::Color[CSys::CHighlighted],255,edit[wske%EDIT]);
 *pom=cpom;
 if ((int)(CSys::GetTime()*1000)%(CURSORINTERVAL*2)<CURSORINTERVAL)
 DrawText((ecursor-estart)*fontwidth+plen,height-fontheight-1,
 CSys::Color[CSys::CHighlighted],"_");
 
 if (prompt)
 {
  Bar(0,height-fontheight-2,plen-1,height-1,CSys::Color[CSys::CPattern]);
  DrawText(1,height-fontheight-2,CSys::FadeColor(CGraph::Color[CSys::DColor],128),prompt);
 }
 
 int y=height-2*fontheight-4;
 int l,i;
 i=wsklv-1;
 l=y/fontheight+1;
 if (l>LINES) l=LINES;
 if (l>wsklv) l=wsklv;
 while (l--)
 {
  /*char *s=lines[i%LINES];
  int k=colshf;
  while (k>0) {if (!*s) break; if (*s>=32) k--; s++;}*/
  if (colshf>0)
  {
   DrawTextHighlighted(1-(colshf-1)*fontwidth,y,CSys::Color[CSys::CNormal],220,lines[i%LINES]);
  }
  else
  DrawTextHighlighted(1,y,CSys::Color[CSys::CNormal],220,lines[i%LINES]);
  i--;
  y-=fontheight;
 }
 if (colshf>0)
 {
  y=height-2*fontheight-4;
  while (y>=0)
  {
   Bar(0,y,fontwidth,y+fontheight-1,CSys::Color[CSys::CPattern]);
   DrawText(1,y,CSys::Color[CSys::CNormal],"<");
   y-=fontheight;
  }
 }
 if (libwinon) libwin->Draw();
}

void CCon::Put(char *s)
{
 int l=0,l2=0;
 int sl=strlen(s);
 //if (sl>=BUFLEN) sl=BUFLEN-1;
 char bufl[BUFLEN];
 while (l<sl)
 {
  while ((s[l]!=0)&&(s[l]!='\n')) l++;
  if (l-l2>=BUFLEN) l2=l-BUFLEN+1;
  memcpy(bufl,s+l2,l-l2);
  bufl[l-l2]=0;
  strcpy(lines[wskl%LINES],bufl);
  l2=++l;
  wskl++;
 }
 wsklv=wskl;
 CSys::Log_Printf(s);
}

int CCon::FExec(char *f) 
{
 if (fexecsl==MAXEXECFILES-1)
 {
  CCon::Put("\04Too many files");
  return 0;
 }
 fexecsl++;
 if (f==0)
 {
  fexecstack[fexecsl]=CSys::FOpen("autoexec.txt","rt");
  if (fexecstack[fexecsl]==0) 
  {
   fexecsl--;
   return 0;
  }
 }
 else
 {
  fexecstack[fexecsl]=CSys::FOpen(f,"rt"); 
  if (fexecstack[fexecsl]==0) 
  {
   fexecsl--;
   CCon::Put("\04Error opening file");
   return 0;
  }
 }
 return 1;
}

void CCon::ChangeFontSize(int w,int h)
{
 fontwidth=w;
 fontheight=h;
}
