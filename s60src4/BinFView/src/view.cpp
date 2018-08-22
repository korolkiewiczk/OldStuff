CView::CView(int w,int h,void *buf)
{
 filebuf=0; 
 pos=0;
 
 filesize=0;
 vmode=Text_Mode;
 win=new CWin(w,h,buf);
 cursorpos[0]=0;
 cursorpos[1]=0;
 noloadable=0;
 viewno=0;
 filename[0]=0;
 
 editboxon=0;
 memset(editboxstr,0,sizeof(editboxstr));
 editboxpos=0;
 
 findseq[0]=0;
  
 alloccon(8,8);
}

void CView::alloccon(int fontwidth,int fontheight)
{
 win->ChangeFontSize(fontwidth,fontheight);
 VIEW_WIDTH=win->width/win->fontwidth;
 VIEW_HEIGHT=win->height/win->fontheight-1;
 LINEWIDTH_BIN=8*4;
 LINEWIDTH_HEX=16*4;
 LINEWIDTH_DEC=10*4;
 LINEWIDTH_OCT=16*4;
 while (VIEW_WIDTH-6-LINEWIDTH_BIN*9<0)
 {
  LINEWIDTH_BIN/=2;
 }
 while (VIEW_WIDTH-7-LINEWIDTH_HEX*3-LINEWIDTH_HEX<0)
 {
  LINEWIDTH_HEX/=2;
 }
 while (VIEW_WIDTH-7-LINEWIDTH_DEC*4-LINEWIDTH_DEC<0)
 {
  LINEWIDTH_DEC/=2;
 }
 while (VIEW_WIDTH-7-LINEWIDTH_OCT*4<0)
 {
  LINEWIDTH_OCT/=2;
 }
 
 conwidth=win->width/win->fontwidth;
 conheight=win->height/win->fontheight;
 conlen=conwidth*conheight;
 conbuf=new char[conlen+1];
 memset(conbuf,32,conlen);
 conposx=conposy=0;
 conshift=0;
 conbuf[conlen]=0;
}

CView::~CView()
{
 delete win;
 if (filebuf)
 delete filebuf;
}

//zwraca 0 gdy brak pliku, -1 gdy blad alokacji, 1 gdy ok
int CView::LoadFile(char *fn)
{
 if (noloadable) return 0;
 if (fn[0]==0) return 0;
 FILE *f=fopen(fn,"rb"); //otwieramy plik do odczytu, plik edytowany w pamieci
 
 if (!f) 
 {
  if (filebuf) delete filebuf;
  filebuf=new unsigned char [1];
  filebuf[0]=0;
  filesize=0;
  return 0;
 }
 
 //pobierz rozmiar pliku
 fseek(f,0,SEEK_END);
 filesize=ftell(f);
 fseek(f,0,SEEK_SET);
  
 strcpy(filename,fn);
 
 //okresl typ pliku
 int c;
 filetype=File_Text;
 int i=0;
 while (i<filesize)
 {
  c=fgetc(f);
  if ((c<32)&&(c!=0xa)&&(c!=0xd)&&(c!='\t')) {filetype=File_Bin; break;}
  i++;
 }
 fclose(f);
 buf_pos=-1;
 pos=0;
 return 1;
}

int CView::peekbuf(int pos)
{
 if ((buf_pos==-2)||(noloadable))
 {
  if (pos<filesize)
  return filebuf[pos];
  else return 0;
 }
 
 if ((buf_pos==-1)||(pos<buf_pos)||(pos>buf_pos+FILEBUFSIZE))
 //wczytuj plik
 {
  FILE *f=fopen(filename,"rb");
 
  if (filebuf) delete filebuf;
  if (!f) 
  {
   filebuf=new unsigned char [1];
   filebuf[0]=0;
   filesize=0;
   buf_pos=0;
   return 0;
  }
  
  buf_pos=(pos/FILEBUFSIZE)*FILEBUFSIZE-FILEBUFSIZE/2;
  if (buf_pos<0) buf_pos=0;
  else
  if (buf_pos+FILEBUFSIZE<pos) buf_pos+=FILEBUFSIZE/2;
  
  filebuf=new unsigned char [FILEBUFSIZE+1];
  if (!filebuf) return 0;
  fseek(f,buf_pos,SEEK_SET);
  fread(filebuf,FILEBUFSIZE,1,f);
  fclose(f);
 }
 
 int p=pos-buf_pos;
 if ((p>=0)&&(p<filesize))
 return filebuf[p];
 else
 return 0;
}

void CView::SetString(char *name,char *s)
{
 if (filebuf) delete filebuf;
 buf_pos=-2;
 
 filesize=strlen(s);
 
 filebuf=new unsigned char [filesize+1];
 if (!filebuf) return;
 
 strcpy(filename,name);
 strcpy((char*)filebuf,s);
 
 filetype=File_Text;
}

int CView::SaveFile(char *fn)
{
 if (fn[0]==0) return 0;
 FILE *f=fopen(fn,"wb"); //otwieramy plik do odczytu, plik edytowany w pamieci
 
 fwrite(filebuf,filesize,1,f);
 fclose(f);
 return 1;
}

void CView::Show()
{
 cls();
 printf("\n");
 int i,j;
 char buf[64];
 //podglad tekstowy
 if (vmode==Text_Mode)
 {
  int p=pos;
  for (j=0; j<VIEW_HEIGHT; j++)
  {
   if (filetype==File_Bin)
   {
    for (i=0; i<VIEW_WIDTH; i++)
    {
     if (p<filesize)
     {
      unsigned char c;
      c=peekbuf(p++);
      if ((c==0)||(c=='\t')||(c=='\n')||(c==0xd)) putchar(' ');
      else
      putchar(c);
     }
    }
    putchar('\n');
   }
   else
   {
    while ((p<filesize)&&(peekbuf(p)!=0))
    {
     putchar(peekbuf(p));
     if (peekbuf(p++)=='\n') break;
    }
   }
  }
 }
 else
 //podglad binarny
 if (vmode==Bin_Mode)
 {
  int p=(pos/LINEWIDTH_BIN)*LINEWIDTH_BIN;
  printf("POS   +   ");//00       01       02       03       04       05       06       07");
  for (i=0; i<LINEWIDTH_BIN; i++)
  printf("%02X       ",i);
  for (j=0; j<VIEW_HEIGHT-1; j++)
  {
   printf("\n%05X: ",p);
   for (i=0; i<LINEWIDTH_BIN; i++)
   {
    if (p<filesize)
    {
     unsigned char c;
     c=peekbuf(p);
     A2Str_Base(c,2,buf);
     if (i<LINEWIDTH_BIN-1)
     printf("%s ",buf);
     else
     printf("%s",buf);
    }
    p++;
   }
  }
  putchar('\n');
 }
 else
 //podglad hex
 if (vmode==Hex_Mode)
 {
  int p=(pos/LINEWIDTH_HEX)*LINEWIDTH_HEX;
  printf("POS   +");//00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
  for (i=0; i<LINEWIDTH_HEX; i++)
  printf("%02X ",i);
  for (j=0; j<VIEW_HEIGHT-1; j++)
  {
   printf("\n%05X: ",p);
   for (i=0; i<LINEWIDTH_HEX; i++)
   {
    if (p<filesize)
    {
     unsigned char c;
     c=peekbuf(p);
     A2Str_Base(c,16,buf);
     printf("%s ",buf);
    }
    else
    printf("   ",buf);
    p++;
   }
   printf(" ");
   p-=LINEWIDTH_HEX;
   for (i=0; i<LINEWIDTH_HEX; i++)
   {
    if (p<filesize)
    {
     unsigned char c;
     c=peekbuf(p);
     if ((c!=0)&&(c!='\t')&&(c!='\n'))
     putchar(c);
     else
     putchar(' ');
    }
    p++;
   }
  }
  putchar('\n');
 }
 //podglad dec
 if (vmode==Dec_Mode)
 {
  int p=(pos/LINEWIDTH_DEC)*LINEWIDTH_DEC;
  printf("POS   + ");// 0   1   2   3   4   5   6   7   8   9 ");
  for (i=0; i<LINEWIDTH_DEC; i++)
  printf("%02d  ",i);
  for (j=0; j<VIEW_HEIGHT-1; j++)
  {
   printf("\n%05X: ",p);
   for (i=0; i<LINEWIDTH_DEC; i++)
   {
    if (p<filesize)
    {
     unsigned char c;
     c=peekbuf(p);
     A2Str_Base(c,10,buf);
     printf("%s ",buf);
    }
    else
    printf("    ",buf);
    p++;
   }
   printf(" ");
   p-=LINEWIDTH_DEC;
   for (i=0; i<LINEWIDTH_DEC; i++)
   {
    if (p<filesize)
    {
     unsigned char c;
     c=peekbuf(p);
     if ((c!=0)&&(c!='\t')&&(c!='\n'))
     putchar(c);
     else
     putchar(' ');
    }
    p++;
   }
  }
  putchar('\n');
 }
 //podglad oct
 if (vmode==Oct_Mode)
 {
  int p=(pos/LINEWIDTH_OCT)*LINEWIDTH_OCT;
  printf("POS   + ");//0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F ");
  for (i=0; i<LINEWIDTH_OCT; i++)
  printf("%02X  ",i);
  for (j=0; j<VIEW_HEIGHT-1; j++)
  {
   printf("\n%05X: ",p);
   for (i=0; i<LINEWIDTH_OCT; i++)
   {
    if (p<filesize)
    {
     unsigned char c;
     c=peekbuf(p);
     A2Str_Base(c,8,buf);
     if (i<LINEWIDTH_OCT-1)
     printf("%s ",buf);
     else
     printf("%s",buf);
    }
    p++;
   }
  }
  putchar('\n');
 }
 presentcon();
}

void CView::Command(int c)
{
 char bf[1024];
 int i;
 switch(c)
 {
  case Cmd_PageUp: for (i=0; i<SDELTA; i++) changepage(-1); break;
  case Cmd_PageDown: for (i=0; i<SDELTA; i++) changepage(1); break;
  case Cmd_LineUp: for (i=0; i<SDELTA; i++) changeline(-1); break;
  case Cmd_LineDown: for (i=0; i<SDELTA; i++) changeline(1); break;
  case Cmd_Txt: SetVMode(Text_Mode); break;
  case Cmd_Bin: SetVMode(Bin_Mode); break;
  case Cmd_Hex: SetVMode(Hex_Mode); break;
  case Cmd_Dec: SetVMode(Dec_Mode); break;
  case Cmd_Oct: SetVMode(Oct_Mode); break;
  case Cmd_ChangeFont: 
       if (win->fontheight==8) alloccon(8,16);
       else
       alloccon(8,8);
       break;
  case Cmd_ChangeFontSmall: alloccon(4,6); break;
  case Cmd_NextMode:
       vmode++;
       if (vmode>=Last_Mode) vmode=0;
       break;
  
  case Cmd_Right: conshift+=SDELTA; break;
  case Cmd_Left: conshift-=SDELTA; if (conshift<0) conshift=0; break;
  case Cmd_Home: if (shift) conshift=0; else pos=0; break;
  case Cmd_End: pos=filesize-1; break;
  case Cmd_PrintScreen: win->PrintScreen();
 }
 if (pos>=filesize) 
 {
  pos=filesize-1;
  if (filetype==File_Text)
  {
   while ((pos>0)&&(peekbuf(pos--)!='\n'));
   pos++;
   while ((peekbuf(pos)=='\n')||(peekbuf(pos)==0xa)) pos++;
   //pos+=2;
  }
 }
 if (pos<0) pos=0;
}

char *CView::ModeString()
{
 switch (vmode)
 {
  case Text_Mode: return "TEXT";
  case Bin_Mode: return  "BIN";
  case Hex_Mode: return  "HEX";
  case Dec_Mode: return  "DEC";
  case Oct_Mode: return  "OCT";
 }
}

void CView::printf(const char *s,...)
{
 char sbuf[4096];
 va_list args;
 
 va_start(args,s);
 vsprintf(sbuf,s,args);
 va_end(args);
 
 int l=strlen(sbuf);
 int i;
 for (i=0; i<l; i++) 
 {
  putchar(sbuf[i]);
 }
}

void CView::putchar(char c)
{
 if (c==0xd) return;
 if (c=='\n')
 {
  conposx=-conshift;
  conposy++;
 }
 else
 if (c=='\t')
 {
  putchar(' ');
  putchar(' ');
  putchar(' ');
  putchar(' ');
 }
 else
 {
  if ((conposx>=0)&&(conposx<conwidth)&&(conposy<conheight))
  *(conbuf+conwidth*conposy+conposx)=c;
  //DEBUG_Printf("%c",c);
  conposx++;
 }
}

void CView::presentcon()
{
 SetTitle(filename);
 int i,j;
 win->Clear();
 char bf[256];
 win->Bar(0,0,win->width-1,win->fontheight-1,BARCOLOR);
 if (editboxon==0)
 {
  if (viewno)
  sprintf(bf,"%d %s  %05X/%05X  %d  %c",viewno,ModeString(),pos,filesize,conshift,
  (filetype==File_Text)? 'T':'B');
  else
  sprintf(bf,"%s  %05X/%05X  %d  %c",ModeString(),pos,filesize,conshift,
  (filetype==File_Text)? 'T':'B');
  if (win->fontwidth>=8)
  {
   win->DrawText(0,0,0,bf);
   char *s=CutTxt(filename,win->width/win->fontwidth-(int)(strlen(bf)+4));
   int p1=win->width-strlen(s)*win->fontwidth-1;
   //if ((int)(strlen(bf)+4)*win->fontwidth<p1)
   win->DrawText(p1,0,0,s);
  }
  else
  {
   win->DrawText3X5(0,0,0,bf);
   char *s=CutTxt(filename,win->width/win->fontwidth-(int)(strlen(bf)+4));
   int p1=win->width-strlen(s)*win->fontwidth-1;
   //if ((int)(strlen(bf)+4)*win->fontwidth<p1)
   win->DrawText3X5(p1,0,0,s);
  }
 }
 else
 {
  sprintf(bf,"%s",editboxstr);
  if (win->fontwidth>=8)
  {
   win->DrawText(win->fontwidth*editboxpos,0,0,"_");
   win->DrawText(0,0,0,bf);
   char *s=CutTxt(GetBoxText(),win->width/win->fontwidth-(int)(strlen(bf)+4));
   int p1=win->width-strlen(s)*win->fontwidth-1;
   win->DrawText(p1,0,0,s);
  }
  else
  {
   win->DrawText3X5(win->fontwidth*editboxpos,0,0,"_");
   win->DrawText3X5(0,0,0,bf);
   char *s=CutTxt(GetBoxText(),win->width/win->fontwidth-(int)(strlen(bf)+4));
   int p1=win->width-strlen(s)*win->fontwidth-1;
   win->DrawText3X5(p1,0,0,s);
  }
 }
 
 //zaznacz szukane wartosci
 int nfind=0;
 if (findseq[0]!=0)
 {
  int *tab;
  int size=pagesize(1,1);
  if (size+pos>=filesize) size=filesize-pos-1;
  peekbuf(pos);
  int bufpos=(buf_pos!=-2)? buf_pos:0;
  
  if (vmode==Text_Mode)
  {
   nfind=FindText(findseq,(char*)&filebuf[pos-bufpos],size,&tab);
   int l=strlen(findseq);
   if (filetype==File_Bin)
   {
    for(int i=0; i<nfind; i++)
    {
     for (int j=0; j<l; j++)
     {
      int posx=((tab[i]+j)%VIEW_WIDTH-conshift)*win->fontwidth;
      int posy=(tab[i]+j)/VIEW_WIDTH*win->fontheight+win->fontheight;
      win->Bar(posx,posy,
      posx+win->fontwidth,posy+win->fontheight,MARKCOLOR);
     }
    }
   }
   else
   if (nfind)
   {
    int p=pos;
    int lines=0; //liczba lini
    int k=0;
    int strl=0;
    int posx=-conshift*win->fontwidth;
    int posy=win->fontheight;
    while ((p<filesize)&&(lines<VIEW_HEIGHT))
    {
     unsigned char c;
     if (p-pos==tab[k])
     {
      win->Bar(posx,posy,
      posx+win->fontwidth,posy+win->fontheight,MARKCOLOR);
      strl++;
      tab[k]++;
      if ((strl>=l)||(tab[k]==tab[k+1]))
      {
       k++;
       strl=0;
      }
     }
     if (k>=nfind) break;
     c=peekbuf(p++);
     posx+=win->fontwidth;
     if (c=='\n') 
     {
      posx=-conshift*win->fontwidth;
      posy+=win->fontheight;
      lines++;
     }
    }
   }
  }
  else
  {
   int lw=0;
   int fw=0;
   int extra=0;
   int p;
   if (vmode==Bin_Mode)
   {
    lw=LINEWIDTH_BIN;
    fw=8+1;
    p=(pos/LINEWIDTH_BIN)*LINEWIDTH_BIN;
   }
   else
   if (vmode==Hex_Mode)
   {
    lw=LINEWIDTH_HEX;
    fw=2+1;
    extra=1;
    p=(pos/LINEWIDTH_HEX)*LINEWIDTH_HEX;
   }
   else
   if (vmode==Dec_Mode)
   {
    lw=LINEWIDTH_DEC;
    fw=3+1;
    extra=1;
    p=(pos/LINEWIDTH_DEC)*LINEWIDTH_DEC;
   }
   else
   if (vmode==Oct_Mode)
   {
    lw=LINEWIDTH_OCT;
    fw=3+1;
    p=(pos/LINEWIDTH_OCT)*LINEWIDTH_OCT;
   }
   
   if (size+p>=filesize) size=filesize-pos-1;
   nfind=FindText(findseq,(char*)&filebuf[p-bufpos],size,&tab);
   int l=strlen(findseq);
   {
    for(int i=0; i<nfind; i++)
    {
     for (int j=0; j<l; j++)
     {
      int pos_=tab[i]+p;
      if ((pos_>=0x100000)&&(pos_<0x1000000)) pos_=1;
      else
      if ((pos_>=0x1000000)&&(pos_<0x10000000)) pos_=2;
      else
      if (pos_>=0x10000000) pos_=3;
      else
      pos_=0;
      int posx=((tab[i]+j)%lw)*(fw*win->fontwidth)+(7-conshift+pos_)*win->fontwidth;
      int posy=(tab[i]+j)/lw*win->fontheight+2*win->fontheight;
      win->Bar(posx,posy,
      posx+(fw-1)*win->fontwidth,posy+win->fontheight,MARKCOLOR);
      if (extra)
      {
       int posx=((tab[i]+j)%lw)*win->fontwidth+(7-conshift+lw*fw+1+pos_)*win->fontwidth;
       win->Bar(posx,posy,
       posx+win->fontwidth,posy+win->fontheight,MARKCOLOR);
      }
     }
    }
   }
  }
 }
 
 char *pom=new char[conwidth+1];
 for (i=0; i<conheight; i++)
 {
  memcpy(pom,conbuf+i*conwidth,conwidth);
  pom[conwidth]=0;
  if (win->fontwidth>=8)
  win->DrawText(0,i*win->fontheight,TEXTCOLOR,pom);
  else
  win->DrawText3X5(0,i*win->fontheight,TEXTCOLOR,pom);
 }
}

char *CView::GenStat()
{
 static char stat[4096];
 if (filetype==File_Text)
 {
  //liczba lini, slow
  int lines=1,words=0;
  
  int l;
  int j=' ';
  for (l=0; l<filesize; l++)
  {
   int c=peekbuf(l);
   if (c=='\n') lines++;
   else
   if (c!=0xd)
   {
    if (c!=' ')
    {
     if ((j==' ')||(j=='\n')) words++;
    }
   }
   j=c;
  }
  
  sprintf(stat,
  "\
File path: %s\n\
File type: TEXT\n\
File size: %d\n\
Lines: %d\n\
Words: %d\n\
",
 filename,
 filesize,
 lines,
 words);
 }
 else
 {
  sprintf(stat,
  "\
File path: %s\n\
File type: BIN\n\
File size: %d\n\
",
 filename,
 filesize);
 }
 return stat;
}

int CView::pagesize(int dir,int a)
{
 int pagesize_p,pagesize_n;
 
 //ustal rozmiar strony
 if (vmode==Text_Mode)
 {
  if (filetype==File_Text)
  {
   //do przodu
   int p=pos;
   int l=0; //liczba lini
   while ((p<filesize)&&(l<VIEW_HEIGHT-1+a))
   {
    unsigned char c;
    c=peekbuf(p++);
    if (c=='\n') l++;
   }
   pagesize_p=p-pos;
  
   //do tylu
   p=pos;
   l=0; //liczba lini
   while ((p>=0)&&(l<VIEW_HEIGHT))
   {
    unsigned char c;
    c=peekbuf(p--);
    if (c=='\n') l++;
   }
   if (p>0) p+=2;
   pagesize_n=pos-p;
  }
  else
  {
   pagesize_p=pagesize_n=(VIEW_WIDTH)*(VIEW_HEIGHT-1+a);
  }
 }
 else
 if (vmode==Bin_Mode)
 {
  pagesize_p=pagesize_n=LINEWIDTH_BIN*(VIEW_HEIGHT-2+a);
 }
 else
 if (vmode==Hex_Mode)
 {
  pagesize_p=pagesize_n=LINEWIDTH_HEX*(VIEW_HEIGHT-2+a);
 }
 else
 if (vmode==Dec_Mode)
 {
  pagesize_p=pagesize_n=LINEWIDTH_DEC*(VIEW_HEIGHT-2+a);
 }
 else
 if (vmode==Oct_Mode)
 {
  pagesize_p=pagesize_n=LINEWIDTH_OCT*(VIEW_HEIGHT-2+a);
 }
 if (dir>0) 
 return pagesize_p;
 else
 return pagesize_n;
}

void CView::changepage(int dir)
{
 if (dir>0) pos+=pagesize(1);
 if (dir<0) pos-=pagesize(-1);
}

void CView::changeline(int dir)
{
 int linesize_p,linesize_n;
 
 //ustal rozmiar strony
 if (vmode==Text_Mode)
 {
  if (filetype==File_Text)
  {
   //do przodu
   int p=pos;
   while (p<filesize)
   {
    unsigned char c;
    c=peekbuf(p++);
    if (c=='\n') break;
   }
   linesize_p=p-pos;
  
   //do tylu
   p=pos-1;
   int a=0;
   while (p>=0)
   {
    unsigned char c;
    c=peekbuf(p);
    if (c=='\n') a++;
    if (a==2) break;
    p--;
   }
   if (a==2)
   {
    while ((peekbuf(p)<32)&&(a>1)) {if (peekbuf(p)=='\n') a--; p++;}
   }
   linesize_n=pos-p;
  }
  else
  {
   linesize_p=linesize_n=VIEW_WIDTH;
  }
 }
 else
 if (vmode==Bin_Mode)
 {
  linesize_p=linesize_n=LINEWIDTH_BIN;
 }
 else
 if (vmode==Hex_Mode)
 {
  linesize_p=linesize_n=LINEWIDTH_HEX;
 }
 else
 if (vmode==Dec_Mode)
 {
  linesize_p=linesize_n=LINEWIDTH_DEC;
 }
 else
 if (vmode==Oct_Mode)
 {
  linesize_p=linesize_n=LINEWIDTH_OCT;
 }
 if (dir>0) pos+=linesize_p;
 if (dir<0) pos-=linesize_n;
}

void CView::Change(int w,int h,void *b)
{
 win->Change(w,h,b);
 alloccon(win->fontwidth,win->fontheight);
}

void CView::EditBoxKey(int c)
{
 if (c==K_LEFT) {if (editboxpos>0)editboxpos--;}
 else
 if (c==K_RIGHT) 
 {
  if (editboxpos<EDITBUFSIZE-1)
  {
   if (editboxstr[editboxpos]!=0)
   editboxpos++;
  }
 }
 else
 if (c==K_ENTER)
 {
  ExecuteBoxStr(editboxtype);
 }
 else
 if (c==K_ESC)
 {
  ExecuteBoxStr(Edit_Null);
 }
 else
 if (c==K_BKSPACE )
 {
  int i=editboxpos;
  if (i>0)
  {
   while (editboxstr[i]) {editboxstr[i-1]=editboxstr[i]; i++;}
   editboxstr[i-1]=0;
   editboxpos--;
  }
 }
 else
 for (int i=0; i<KEYMAPLEN; i++)
 {
  if (keymap[i][0]==c)
  {
   if ((keymap[i][1+(shift>0)]>=32)&&(keymap[i][1+(shift>0)]<127))
   editboxstr[editboxpos++]=keymap[i][1+(shift>0)];
   if (editboxpos>=EDITBUFSIZE-1) editboxpos=EDITBUFSIZE-2;
  }
 }
}

void CView::ExecuteBoxStr(int type)
{
 int base;
 if (vmode==Text_Mode) base=10; else base=16;
 switch (type)
 {
  case Edit_Pos: 
       pos=Str2A_Base(editboxstr,base); 
       if (pos>=filesize) pos=filesize-1;
       else
       if (pos<0) pos=0;
       break;
  case Edit_Find: 
       switch (vmode)
       {
        case Text_Mode: strcpy(findseq,editboxstr); break;
        case Bin_Mode: strcpy(findseq,GetFindStr_Base(editboxstr,2)); break;
        case Hex_Mode: strcpy(findseq,GetFindStr_Base(editboxstr,16)); break;
        case Dec_Mode: strcpy(findseq,GetFindStr_Base(editboxstr,10)); break;
        case Oct_Mode: strcpy(findseq,GetFindStr_Base(editboxstr,8)); break;
       }
       break;
 }
 
 editboxon=0;
 memset(editboxstr,0,sizeof(editboxstr));
 editboxpos=0;
}

char *CView::GetBoxText()
{
 switch (editboxtype)
 {
  case Edit_Pos: 
       if (vmode==Text_Mode) 
       return "Enter position(DEC)";
       else
       return "Enter position(HEX)";
  case Edit_Find:
       switch (vmode)
       {
        case Text_Mode: return "Find string";
        case Bin_Mode: return "Find (BIN)";
        case Hex_Mode: return "Find (HEX)";
        case Dec_Mode: return "Find (DEC)";
        case Oct_Mode: return "Find (OCT)";
       }
       break;
 }
}
