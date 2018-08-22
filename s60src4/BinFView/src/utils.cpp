void A2Str_Base(int a,int b,char *s)
{
 if (b<2) return;
 char bf[128];
 int i=0;
 int x=0xff;
 while (x!=0)
 {
  int c=a%b;
  if (c>9) c+='A'-10;
  else c+='0';
  bf[i++]=c;
  a/=b;
  x/=b;
 }
 int j=0;
 s[i--]=0;
 while (i>=0) s[j++]=bf[i--];
}

unsigned int Str2A_Base(char *a,int b)
{
 if (b<2) return 0;
 int l=0;
 while(*a)
 {
  l*=b;
  int c=*a++;
  if (c>='a') c=c-'a'+10;
  else
  if (c>='A') c=c-'A'+10;
  else
  c=c-'0';
  l+=c;
 }

 return l;
}

void str2desc(char *str, TDes& desc)
{
 TPtrC8 ptr(reinterpret_cast<const TUint8*>(str));
 desc.Copy(ptr);
}

void desc2str(TDes& desc,char *str,int bufsize)
{
 int i=0;
 int s=desc.Length();
 if (s>=bufsize) s=bufsize-1;
 for(i=0; i<s; i++)
 {
  str[i]=desc[i];
 }
 str[i]=0;
}

int OpenDialog(char *fn,int bufsize)
{
 TFileName filen;
 str2desc("",filen);
 fn[0]=0;
 if (AknCommonDialogs::RunSelectDlgLD(filen,R_MEMORY_SELECTION_DIALOG))
 {
  /*int i=0;
  int s=filen.Length();
  if (s>=bufsize) s=bufsize-1;
  for(i=0; i<s; i++)
  {
   fn[i]=filen[i];
  }
  fn[i]=0;*/
  desc2str(filen,fn,bufsize);
  return 1;
 }
 return 0;
}

void SetTitle(char *t)
{
 
}

int QuitMessage()
{
 TBuf<256> msg;
 TBuf<2> nullmsg;
 str2desc("Do you want to exit",msg);
 str2desc("",nullmsg);
 CEikonEnv *env=CEikonEnv::Static();
 return env->QueryWinL(msg,nullmsg);
}

char *CutTxt(char *t,int l)
{
 static char cut[4096];
 int a=strlen(t);
 if (a<l) return t;
 int i;
 cut[0]='.'; cut[1]='.'; cut[2]='.';
 for (i=3; i<l; i++)
 {
  cut[i]=t[a-l+i];
 }
 cut[i]=0;
 if (i==3) return "";
 else
 return cut;
}

char *GetFindStr_Base(char *s,int base)
{
 static char findstr[256];
 if (s[0]==0) return "";
 int i=0,k=0;
 char bf[256];
 while (s[i]!=0)
 {
  int j=0;
  while ((s[i]!=0)&&(s[i]!=' '))
  {
   bf[j++]=s[i++];
  }
  bf[j]=0;
  unsigned int w=Str2A_Base(bf,base);
  if (w<256)
  findstr[k++]=w;
  else
  if (w<65536)
  {
   findstr[k++]=w&255;
   findstr[k++]=w>>8;
  }
  else
  if (w<16777216)
  {
   findstr[k++]=w&255;
   findstr[k++]=(w>>8)&255;
   findstr[k++]=(w>>16)&255;
  }
  else
  {
   findstr[k++]=w&255;
   findstr[k++]=(w>>8)&255;
   findstr[k++]=(w>>16)&255;
   findstr[k++]=(w>>24)&255;
  }
  while (s[i]==' ') i++;
 }
 findstr[k]=0;
 return findstr;
}

int FindText(char *x,char *y,int len,int **tab)
{
 
 //kmp
 /*static int p[160*80*2];
 static int w[160*80*2];
 int nw=0;
 
 int m=strlen(x);
 int n=len;
 p[0]=0;
 p[1]=0;
 int i,j,t;
 t=0;
 for (j=2; j<=m; j++)
 {
  while ((t>0)&&(x[t+1-1]!=x[j-1])) t=p[t];
  if (x[t+1-1]==x[j-1]) t=t+1;
  p[j]=t;
 }
 
 j=0;
 for (i=1; i<=n-(m-j)+1;)
 {
  j=p[j];
  int i0=i,i1=i-j;
  while ((j<m)&&(x[j+1-1]==y[i-1])) {i++;j++;}
  if (j==m) w[nw++]=i1-1;
  if (i0==i) i++;
 }
 *tab=w;
 return nw;*/

 //bruteforce
 static int w[160*80*2];
 int nw=0;
 int m=strlen(x);
 int n=len;
 for (int i=0; i<n; i++)
 {
  int j=0;
  int k=i;
  while ((j<m)&&(k<n)&&(x[j]==y[k])) {j++; k++;}
  if (j==m) w[nw++]=i;
 }
 *tab=w;
 return nw;
}

void inline _memset32(void *p,int a,unsigned int b) 
{
 while (b--)
 {
  *(int*)p=a;
  p=(void*)((int)p+4);
 }
}