//chelp.c

CHelp::CHelp(int w,int h,void *b,char *fn):CWin(w,h,b)
{
 fontwidth=8;
 fontheight=16;
 helplines=0;
 Load(fn);
}

void CHelp::Load(char *fn)
{
 FILE *f;
 filename=fn;
 f=CSys::FOpen(fn,"rt");
 nhelpl=0;
 helpviewpos=0;
 buflen=width/fontwidth-1;
 if (buflen>=HELPBUFLEN) buflen=HELPBUFLEN-1;
 if (helplines) delete helplines;
 if (!f) 
 {
  helplines=new char [strlen(NOREADMETXT_TEXT)+1];
  strcpy(helplines,NOREADMETXT_TEXT);
  nhelpl=1;
  helpviewpos=0;
  return;
 }
 
 fseek(f,0,SEEK_END);
 int fs=ftell(f);
 fseek(f,0,SEEK_SET);
 helplines=new char [fs*4];
 
 char bf[4096];
 bf[0]=0;
 while (1)
 {
  char *wsk;
  wsk=fgets(bf,4096,f);
  if (wsk==0) break;
  while (bf[0]!=0)
  {
   int l=strlen(bf);
   wsk=0;
   if ((l>buflen)&&(bf[buflen]>32))
   {
    wsk=bf;
    int l=buflen;
    while ((*wsk)&&(*wsk!=' ')&&(l>0)) {wsk++; l--;}
    if (!((*wsk==0)||(l==0)))
    {
     wsk=&bf[buflen];
     while ((*wsk>32)&&(wsk>bf)) wsk--;
     while ((*wsk<=32)&&(wsk>bf)) wsk--;
     *(++wsk)=0;
    }
   }
   else 
   if (l>buflen) {wsk=bf+buflen; *wsk=0;}
   l=strlen(bf);
   while ((l>=0)&&(bf[l]<=32)) bf[l--]=0;
   strcpy(helplines+(nhelpl++)*(buflen+1),bf);
   if (wsk)
   strcpy(bf,wsk+1);
   else
   bf[0]=0;
  }
 }
 fclose(f);
}

void CHelp::KeyFunc(int k)
{
 if (k==K_UP) helpviewpos--;
 else
 if (k==K_DOWN) helpviewpos++;
 else
 if (k==K_PGUP) helpviewpos-=height/fontheight;
 else
 if (k==K_PGDOWN) helpviewpos+=height/fontheight;
 
 if (helpviewpos<0) helpviewpos=0;
 else
 if (helpviewpos>nhelpl-1) helpviewpos=nhelpl-1;
}

void CHelp::Process()
{
 Clear();
 int y=0;
 int h=height/fontheight;
 int w=helpviewpos;
 if (w+h>nhelpl) h=nhelpl-w;
 for (int i=0; i<h; i++)
 {
  DrawText(0,y,CSys::Color[CSys::CHelp],helplines+(w+i)*(buflen+1));
  y+=fontheight;
 }
}
