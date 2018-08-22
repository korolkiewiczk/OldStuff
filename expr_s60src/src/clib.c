//clib.c

int CLib::index(char c)
{
 if ((c>='A')&&(c<='Z')) return c-'A'+10;
 if ((c>='a')&&(c<='z')) return c-'a'+10;
 if ((c>='0')&&(c<='9')) return c-'0';
 if (c=='_') return 'Z'-'A'+11;
 return -1;
}

char CLib::unindex(char i)
{
 if ((i>=10)&&(i<=10+'Z'-'A')) return 'A'+i-10;
 if ((i>=0)&&(i<=9)) return i+'0';
 if (i=='Z'-'A'+11) return '_';
 return 0;
}

CLib::CLib()
{
 memset(&root,0,sizeof(ND));
}

void CLib::AddElement(ELEMENT *e)
{
 unsigned int i,j,sl=strlen(e->name);
 if (sl>MAXNAMELEN) sl=MAXNAMELEN;
 int itab[MAXNAMELEN];
 for (i=0; i<sl; i++)
 {
  itab[i]=index(e->name[i]);
  if (itab[i]==-1) return;
 }
 ND *nd;
 nd=&root;
 for (i=0; i<sl; i++)
 {
  j=itab[i];
  if (nd->l[j]==0) 
  {
   nd->l[j]=new ND;
   memset(nd->l[j],0,sizeof(ND));
  }
  nd=nd->l[j];
 }
 if (nd->n==0)
 nd->n=new NAME;
 nd->n->fptr=e->fptr;
 i=e->parcount;
 if (i==VAR)
 //jesli zmienna
 {
  nd->n->parattr=((e->rtype&255)<<8)|0x80000000;
  nd->n->partypes=0;
 }
 else
 {
  if (i>15) i=15;
  nd->n->parattr=(i&255)|((e->rtype&255)<<8);
  nd->n->partypes=0;
  for (j=0; j<i; j++)
  {
   nd->n->partypes|=((e->partypes>>(j<<1))&3)<<(j<<1);
  }
 }
 nd->n->tag=e->tag;
}

void CLib::DelElement(char *name)
{
 int i,j,sl=strlen(name);
 if (sl>MAXNAMELEN) sl=MAXNAMELEN;
 int itab[MAXNAMELEN];
 for (i=0; i<sl; i++)
 {
  itab[i]=index(name[i]);
  if (itab[i]==-1) return;
 }
 ND *nd;
 nd=&root;
 for (i=0; i<sl; i++)
 {
  j=itab[i];
  if (nd->l[j]==0) return;
  nd=nd->l[j];
 }
 if (nd->n)
 {
  //delete nd->n;
  nd->n=0;
 }
}

void CLib::AddList(void *elist,int n)
{
 int i;
 ELEMENT *e;
 e=(ELEMENT*)elist;
 for (i=0; i<n; i++)
 {
  AddElement(e);
  e++;
 }
}

CLib::CLib(void *elist,int n)
{
 memset(&root,0,sizeof(ND));
 AddList(elist,n);
}

NAME *CLib::Find(char *name)
{
 int i,j,sl=strlen(name);
 if (sl>MAXNAMELEN) sl=MAXNAMELEN;
 int itab[MAXNAMELEN];
 for (i=0; i<sl; i++)
 {
  itab[i]=index(name[i]);
  if (itab[i]==-1) return 0;
 }
 ND *nd;
 nd=&root;
 for (i=0; i<sl; i++)
 {
  j=itab[i];
  if (nd->l[j]==0) return 0;
  nd=nd->l[j];
 }
 return nd->n;
}

int CLib::NListFromTxt(char *_t,char *ret,char schar)
{
 int w=0;
 int t[MAXTXTLEN];
 int postab[MAXTXTLEN];
 int i,k=0,j=0;
 char pomt[MAXTXTLEN];
 char pomt2[MAXTXTLEN];
 int tl;
 pomt2[0]=0;
 ret[0]=0;
 
 postab[0]=0;
 i=0;
 while(_t[i]!=0)
 {
  if(_t[i]==schar) postab[++k]=i+1; 
  else t[i]=index(_t[i]);
  if (t[i]==-1) return 0;
  if ((_t[i]>='a')&&(_t[i]<='z')) _t[i]-='a'-'A';
  i++;
 }
 postab[++k]=-1;
 
 int stack[MAXTXTLEN][3];
 ND *nstack[MAXTXTLEN];
 int sl;
 ND *n;

 sl=0;
 stack[0][0]=0;
 stack[0][1]=0;
 stack[0][2]=-1;
 nstack[0]=&root;
 while (sl>=0)
 {
  tl=stack[sl][2];
  if (tl>=0)
  {
   pomt[tl]=stack[sl][0];
  }
  j=stack[sl][1];
  n=nstack[sl];
  sl--;
  if (postab[j+1]!=-1)
  {
   for (i=postab[j]; i<postab[j+1]-1; i++)
   {
    if (n->l[t[i]]!=0)
    {
     sl++;
     stack[sl][0]=_t[i];
     stack[sl][1]=j+1;
     stack[sl][2]=tl+1;
     nstack[sl]=n->l[t[i]];
    }
   }
  }
  else
  {
   for (i=0; i<NLETTERS; i++)
   {
    if (n->l[i]!=0)
    {
     sl++;
     stack[sl][0]=unindex(i);
     stack[sl][1]=j;
     stack[sl][2]=tl+1;
     nstack[sl]=n->l[i];
    }
   }
   if (n->n!=0)
   {
    pomt[tl+1]=schar;
    pomt[tl+2]=0;
    strcat(pomt2,pomt);
    w++;
   }
  }
 }
 i=0;
 int j2=strlen(pomt2);
 ret[j2--]=0;
 while (pomt2[i]!=0)
 {
  k=i;
  while ((pomt2[k]!=0)&&(pomt2[k]!=schar)) k++;
  int d=k-i;
  for (j=0; j<d; j++) ret[j2-d+j]=pomt2[j+i];
  ret[j2]=schar;
  j2-=d+1;
  i=k+1;
 }
 return w;
}
