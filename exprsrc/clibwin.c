//clibwin.c

char *CLibWin::Num2Char[9]=
{
 "ABC","DEF","GHI","JKL","MNO","PQRS","TUV","WXYZ","_"
};

void CLibWin::KeyFunc(int k)
{
 if (k==K_DOWN)
 {
  pos++;
 }
 else
 if (k==K_UP)
 {
  pos--;
 }
 else
 if (k==K_ENTER)
 {
  char pom[MAXBTXTLEN];
  if (tbuf[0]==0) return;
  strcpy(pom,tbuf);
  int k=0,j=0;
  for (int i=0; i<pos; i++) 
  {
   while (pom[j]!='\n') j++;
   k=j+1;
   j++;
  }
  while (pom[j]!='\n') j++;
  pom[j]=0;
  strcpy(retbuf,pom+k);
  NAME *n;
  n=lib->Find(retbuf);
  if ((n->parattr&0x80000000)==0)
  {
   strcat(retbuf,"(");
   k=(n->parattr&0xff)-1;
   int i;
   for (i=0; i<k; i++) 
   {
    if (GET_PAR(n->partypes,i)==VAL_STR) strcat(retbuf,"\"\"");
    strcat(retbuf,",");
   }
   if (GET_PAR(n->partypes,i)==VAL_STR) strcat(retbuf,"\"\"");
   strcat(retbuf,")");
  }
 }
 else
 {
  char c=0;
  if ((k>K_1)&&(k<=K_0))  //***
  {
   strcat(ebuf,Num2Char[k-K_1-1]);
   strcat(ebuf,"|");
  }
  else
  {
   for (int i=0; i<KEYMAPLEN; i++)
   {
    if (keymap[i][0]==(k&255))
    {
     c=keymap[i][2];
     break;
    }
   }
   int l=strlen(ebuf);
   if ((c==8)||(c==127)) 
   {
    l-=2;
    while((ebuf[l]!='|')&&(l>=0)) l--;
    ebuf[l+1]=0;
   }
   else
   if (lib->index(c)!=-1)
   {
    ebuf[l]=c;
    ebuf[l+1]='|';
    ebuf[l+2]=0;
   }
  }
  Set(ebuf);
 }
 if (pos<0) pos=npos-1;
 if (pos>=npos) pos=0;
 if (pos-lpos>=lines)
 {
  lpos=pos-lines+1;
 }
 if (pos-lpos<0)
 {
  lpos=pos;
 }
}

void CLibWin::Set(char *t)
{
 pos=lpos=0;
 char pbuf[MAXTXTLEN];
 char ptxt[256];
 tbuf[0]=0;
 char _t[MAXTXTLEN];
 strcpy(_t,t);
 npos=lib->NListFromTxt(_t,pbuf,'|');
 int i=0,m=0,j=0;;
 while (pbuf[i]!=0)
 {
  if (pbuf[i]=='|') m=1;
  if (m==0)
  {
   ptxt[j++]=pbuf[i];
  }
  else
  if (m==1)
  {
   m=0;
   ptxt[j]=0;
   j=0;
   strcat(tbuf,ptxt);
   strcat(tbuf,"\n");
  }
  i++;
 }
 //DEBUG_Printf("%s\n%s\n",pbuf,tbuf);
}

void CLibWin::Draw()
{
 int cpos=pos-lpos;
 char pom[MAXBTXTLEN];
 strcpy(pom,tbuf);
 int j=0,k=0;
 int x=x1,y=y1;
 static int pcposx,pcposy;
 int cposx,cposy;
 CSys::CursorPos(cposx,cposy);
 if ((cposx!=pcposx)||(cposy!=pcposy))
 {
  int d=0;
  if (cposy<y1) d=-1;
  if (cposy>y2-CWCHARHEIGHT) d=1;
  if (d) pos+=d;
  else
  pos=(cposy-y1)/CWCHARHEIGHT+lpos;
  pcposx=cposx;
  pcposy=cposy;
  if (pos<0) pos=0;
  if (pos>=npos) pos=npos-1;
  if (pos-lpos>=lines)
  {
   lpos=pos-lines+1;
  }
  if (pos-lpos<0)
  {
   lpos=pos;
  }
 }
 Bar(x1,y1,x2,y2,CSys::Color[CSys::CPattern]);
 Bar(x1,y1+cpos*CWCHARHEIGHT,x2,y1+(cpos+1)*CWCHARHEIGHT,CSys::Color[CSys::CFavour]);
 for (int i=0; i<lpos; i++) 
 {
  while (pom[j]!='\n') j++;
  k=j+1;
  j++;
 }
 int cnpos=(npos>lines)? lines:npos;
 for (int i=0; i<cnpos; i++)
 {
  while (pom[j]!='\n') j++;
  pom[j]=0;
  DrawText(x,y,CSys::Color[CSys::CHelp],pom+k);
  k=j+1;
  y+=CWCHARHEIGHT;
 }
 //DrawText(0,0,CSys::Color[CSys::CHelp],ebuf);
}
