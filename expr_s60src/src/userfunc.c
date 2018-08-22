//userfunc.c
//interpreter jezyka exprl

#define MAXUSERFUNC 256
#define MAXUSERFUNCSTACKLEN 256

//typy wystepujace w petli
#define USERFUNCLINETYPE_EXPR 0
#define USERFUNCLINETYPE_IF 1
#define USERFUNCLINETYPE_WHILE 2
#define USERFUNCLINETYPE_REP 3
//typy nie wystepujace w petli
#define USERFUNCLINETYPE_NULL 16
#define USERFUNCLINETYPE_ENDIF 17
#define USERFUNCLINETYPE_ELSE 18
#define USERFUNCLINETYPE_ENDWHILE 19
#define USERFUNCLINETYPE_ELSEIF 20

typedef struct _TUserFuncLine
{
 CExpr expr;
 int type; //typ lini
 struct _TUserFuncLine *next,*cnext;
} TUserFuncLine;

typedef struct _TUserFuncList
{
 TUserFuncLine *head;
} TUserFuncList;

typedef struct _TUserFuncStack
{
 TUserFuncLine *lineaddr;
 int tag; //0-normalny,1-warunkowy,bit 1 okresla czy flaga
} TUserFuncStack;

TUserFuncList UserFunc_funclist[MAXUSERFUNC];
TUserFuncStack UserFunc_stack[MAXUSERFUNCSTACKLEN];
int UserFunc_stackl;
TUserFuncLine *UserFunc_currline;
CLib *UserFunc_clib;

int UserFunc_ison=0;
char UserFunc_name[256];
int UserFunc_parcount;
int UserFunc_count=0;
int UserFunc_req;
int UserFunc_checkcnt;

//parametry dla funkcji oznaczane _1,_2,...
float UserFunc_1,UserFunc_2,UserFunc_3,UserFunc_4,
      UserFunc_5,UserFunc_6,UserFunc_7,UserFunc_8;

float inline UserFunc_floop()
{
 TUserFuncLine *currline;
 int lw=0;
 int rl=-1;
 int lwl;
 if (CExpr::MultiExec>0)
 lwl=1024;
 else
 lwl=65536;
 float w=0,c;
 currline=UserFunc_funclist[CExpr::GetExpr()->Tag()].head;
 while (currline)
 {
  switch(currline->type)
  {
   case USERFUNCLINETYPE_EXPR:
        w=currline->expr.Do();
        currline=currline->next;
        break;
   case USERFUNCLINETYPE_IF:
        c=currline->expr.Do();
        if (c==0) currline=currline->cnext;
        else
        currline=currline->next;
        break;
   case USERFUNCLINETYPE_WHILE:
        c=currline->expr.Do();
        lw++;
        if (lw>lwl)
        {
         c=0;
         lw=0;
        }
        if (c==0) 
        {
         lw=0;
         currline=currline->cnext;
        }
        else
        currline=currline->next;
        break;
   case USERFUNCLINETYPE_REP:
        if (rl<0)
        {
         rl=ifloor(currline->expr.Do());
        }
        lw++;
        if (lw>lwl)
        {
         rl=0;
        }
        if (rl<=0) 
        {
         currline=currline->cnext;
         rl=0;
         lw=0;
        }
        else
        currline=currline->next;
        rl--;
        break;
  }
 }
 return w;
}

//funkcja typu f()
float UserFunc_f0()
{
 return UserFunc_floop();
}

//funkcja typu f(a)
float UserFunc_f1(float a)
{
 float _1=UserFunc_1,r;
 UserFunc_1=a;
 r=UserFunc_floop();
 UserFunc_1=_1;
 return r;
}

//funkcja typu f(a,b)
float UserFunc_f2(float a,float b)
{
 float _1=UserFunc_1,_2=UserFunc_2,r;
 UserFunc_1=a;
 UserFunc_2=b;
 r=UserFunc_floop();
 UserFunc_1=_1;
 UserFunc_2=_2;
 return r;
}

//funkcja typu f(a,b,c)
float UserFunc_f3(float a,float b,float c)
{
 float _1=UserFunc_1,_2=UserFunc_2,_3=UserFunc_3,r;
 UserFunc_1=a;
 UserFunc_2=b;
 UserFunc_3=c;
 r=UserFunc_floop();
 UserFunc_1=_1;
 UserFunc_2=_2;
 UserFunc_3=_3;
 return r;
}

//funkcja typu f(a,b,c,d)
float UserFunc_f4(float a,float b,float c,float d)
{
 float _1=UserFunc_1,_2=UserFunc_2,_3=UserFunc_3,_4=UserFunc_4,r;
 UserFunc_1=a;
 UserFunc_2=b;
 UserFunc_3=c;
 UserFunc_4=d;
 r=UserFunc_floop();
 UserFunc_1=_1;
 UserFunc_2=_2;
 UserFunc_3=_3;
 UserFunc_4=_4;
 return r;
}

//funkcja typu f(a,b,c,d,e)
float UserFunc_f5(float a,float b,float c,float d,float e)
{
 float _1=UserFunc_1,_2=UserFunc_2,_3=UserFunc_3,_4=UserFunc_4,_5=UserFunc_5,r;
 UserFunc_1=a;
 UserFunc_2=b;
 UserFunc_3=c;
 UserFunc_4=d;
 UserFunc_5=d;
 r=UserFunc_floop();
 UserFunc_1=_1;
 UserFunc_2=_2;
 UserFunc_3=_3;
 UserFunc_4=_4;
 UserFunc_5=_5;
 return r;
}

//funkcja typu f(a,b,c,d,e,f)
float UserFunc_f6(float a,float b,float c,float d,float e,float f)
{
 float _1=UserFunc_1,_2=UserFunc_2,_3=UserFunc_3,_4=UserFunc_4,_5=UserFunc_5
 ,_6=UserFunc_6,r;
 UserFunc_1=a;
 UserFunc_2=b;
 UserFunc_3=c;
 UserFunc_4=d;
 UserFunc_5=e;
 UserFunc_6=f;
 r=UserFunc_floop();
 UserFunc_1=_1;
 UserFunc_2=_2;
 UserFunc_3=_3;
 UserFunc_4=_4;
 UserFunc_5=_5;
 UserFunc_6=_6;
 return r;
}

//funkcja typu f(a,b,c,d,e,f,g)
float UserFunc_f7(float a,float b,float c,float d,float e,float f,float g)
{
 float _1=UserFunc_1,_2=UserFunc_2,_3=UserFunc_3,_4=UserFunc_4,_5=UserFunc_5
 ,_6=UserFunc_6,_7=UserFunc_7,r;
 UserFunc_1=a;
 UserFunc_2=b;
 UserFunc_3=c;
 UserFunc_4=d;
 UserFunc_5=e;
 UserFunc_6=f;
 UserFunc_7=g;
 r=UserFunc_floop();
 UserFunc_1=_1;
 UserFunc_2=_2;
 UserFunc_3=_3;
 UserFunc_4=_4;
 UserFunc_5=_5;
 UserFunc_6=_6;
 UserFunc_7=_7;
 return r;
}

//funkcja typu f(a,b,c,d,e,f,g,h)
float UserFunc_f8(float a,float b,float c,float d,float e,float f,float g,float h)
{
 float _1=UserFunc_1,_2=UserFunc_2,_3=UserFunc_3,_4=UserFunc_4,_5=UserFunc_5
 ,_6=UserFunc_6,_7=UserFunc_7,_8=UserFunc_8,r;
 UserFunc_1=a;
 UserFunc_2=b;
 UserFunc_3=c;
 UserFunc_4=d;
 UserFunc_5=e;
 UserFunc_6=f;
 UserFunc_7=g;
 UserFunc_8=h;
 r=UserFunc_floop();
 UserFunc_1=_1;
 UserFunc_2=_2;
 UserFunc_3=_3;
 UserFunc_4=_4;
 UserFunc_5=_5;
 UserFunc_6=_6;
 UserFunc_7=_7;
 UserFunc_8=_8;
 return r;
}

int UserFunc_Begin(char *name,int parcount)
{
 if (!CSys::IsValidStr(name)) return 0;
 if (CExpr::MultiExec) return 0;
 if (parcount<0) return 0;
 if (parcount>8) return 0;
 if (UserFunc_ison==1) 
 UserFunc_ison=2;
 else
 UserFunc_ison=1;
 strcpy(UserFunc_name,name);
 UserFunc_parcount=parcount;
 UserFunc_funclist[UserFunc_count].head=
 UserFunc_currline=0;
 UserFunc_stackl=0;
 UserFunc_req=0;
 UserFunc_checkcnt=0;
 return 1;
}

//zwraca typ s i zapisuje do UserFunc_ParseGetbuf expr
char UserFunc_ParseGetTbuf[256];
int UserFunc_ParseGetT(char *s)
{
 char bf[256];
 while ((*s<=32)&&(*s!=0)) s++;
 if (*s==0) return -1;
 if (*s=='#') //dyrektywa
 {
  int i=0;
  s++;
  while (*s>32)
  {
   bf[i++]=toupper(*s);
   s++;
  }
  bf[i]=0;
  while ((*s<=32)&&(*s!=0)) s++;
  
  int r=-1;
  
  //ustal typ
  if (strcmp(bf,"IF")==0)
  {
   strcpy(UserFunc_ParseGetTbuf,s);
   r=USERFUNCLINETYPE_IF;
  }
  else
  if (strcmp(bf,"ENDIF")==0)
  {
   r=USERFUNCLINETYPE_ENDIF;
  }
  else
  if (strcmp(bf,"ELSE")==0)
  {
   r=USERFUNCLINETYPE_ELSE;
  }
  else
  if (strcmp(bf,"ELSEIF")==0)
  {
   strcpy(UserFunc_ParseGetTbuf,s);
   r=USERFUNCLINETYPE_ELSEIF;
  }
  else
  if (strcmp(bf,"WHILE")==0)
  {
   strcpy(UserFunc_ParseGetTbuf,s);
   r=USERFUNCLINETYPE_WHILE;
  }
  else
  if (strcmp(bf,"ENDWHILE")==0)
  {
   r=USERFUNCLINETYPE_ENDWHILE;
  }
  else
  if (strcmp(bf,"REP")==0)
  {
   strcpy(UserFunc_ParseGetTbuf,s);
   r=USERFUNCLINETYPE_REP;
  }
  else
  if (strcmp(bf,"ENDREP")==0)
  {
   r=USERFUNCLINETYPE_ENDWHILE;
  }
  return r;
 }
 else
 //wyrazenie
 {
  strcpy(UserFunc_ParseGetTbuf,s);
  return USERFUNCLINETYPE_EXPR;
 }
}

//przetwarza dany ciag znakow i dodaje do listy wyazen
void UserFunc_ParseStr(char *s)
{
 if (UserFunc_ison==0) return;
 if (UserFunc_ison==2) {UserFunc_ison=1; return;}
 int t=UserFunc_ParseGetT(s);
 if (t==-1)
 {
  CCon::GetCon()->Put("\04Undefined directive");
  return;
 }
 
 if (t<USERFUNCLINETYPE_NULL)
 {
  if (UserFunc_currline==0) 
  {
   UserFunc_currline=new TUserFuncLine;
   memset(UserFunc_currline,0,sizeof(TUserFuncLine));
   UserFunc_currline->expr=CExpr(UserFunc_clib);
  }
  if (UserFunc_funclist[UserFunc_count].head==0)
  UserFunc_funclist[UserFunc_count].head=UserFunc_currline;
  
  UserFunc_currline->type=t;
  
  if (UserFunc_stackl>0)
  {
   do
   {
    UserFunc_stackl--;
    if ((UserFunc_stack[UserFunc_stackl].tag&1)==0)
    UserFunc_stack[UserFunc_stackl].lineaddr->next=UserFunc_currline;
    else
    if ((UserFunc_stack[UserFunc_stackl].tag&1)==1)
    UserFunc_stack[UserFunc_stackl].lineaddr->cnext=UserFunc_currline;
    if (UserFunc_stack[UserFunc_stackl].tag&2) UserFunc_req--;
   }
   while ((UserFunc_req>0)&&(UserFunc_stackl>0));
  }
  if (UserFunc_req<0) UserFunc_req=0;
 }
 
 //DEBUG_Printf("%c: s=%d r=%d %d %s (%s)\n",
 //l++,UserFunc_stackl,UserFunc_req,UserFunc_checkcnt,s,UserFunc_ParseGetTbuf);
 
 s=UserFunc_ParseGetTbuf;
 int i,j;
 char bf[256];
 
 switch (t)
 {
  case USERFUNCLINETYPE_EXPR:
       if (UserFunc_currline->expr.Set(s)==CExpr::NoErr)
       {
        UserFunc_stack[UserFunc_stackl].lineaddr=UserFunc_currline;
        UserFunc_stack[UserFunc_stackl].tag=0;
        UserFunc_stackl++;
        UserFunc_currline=0;
       }
       else
       {
        sprintf(bf,"\04%s",UserFunc_currline->expr.RetStr());
        CCon::GetCon()->Put(bf);
       }
       break;
  case USERFUNCLINETYPE_IF:
  case USERFUNCLINETYPE_WHILE:
  case USERFUNCLINETYPE_REP:
       if (UserFunc_currline->expr.Set(s)!=CExpr::NoErr)
       {
        sprintf(bf,"\04%s",UserFunc_currline->expr.RetStr());
        CCon::GetCon()->Put(bf);
        return;
       }
       UserFunc_stack[UserFunc_stackl].lineaddr=UserFunc_currline;
       UserFunc_stack[UserFunc_stackl].tag=1|2;
       UserFunc_stackl++;
       UserFunc_stack[UserFunc_stackl].lineaddr=UserFunc_currline;
       UserFunc_stack[UserFunc_stackl].tag=0|0;
       UserFunc_stackl++;
       UserFunc_currline=0;
       UserFunc_checkcnt++;
       break;
  case USERFUNCLINETYPE_ELSEIF:
  case USERFUNCLINETYPE_ELSE:
       i=UserFunc_stackl;
       if (UserFunc_stackl>1)
       {
        while (i>0)
        {
         i--;
         if (UserFunc_stack[i].tag&2) 
         {
          if (UserFunc_req==0)
          break;
          UserFunc_req--;
         }
        }
        TUserFuncStack pom;
        pom=UserFunc_stack[i];
        for (j=i; j<UserFunc_stackl-1; j++) UserFunc_stack[j]=UserFunc_stack[j+1];
        UserFunc_stack[UserFunc_stackl-1]=pom;
        UserFunc_stack[i].tag|=2;
       }
       if (t==USERFUNCLINETYPE_ELSEIF) 
       {
        sprintf(bf,"#IF %s",s);
        UserFunc_ParseStr(bf);
       }
       break;
  case USERFUNCLINETYPE_ENDIF:
       UserFunc_req++;
       UserFunc_checkcnt--;
       break;
  case USERFUNCLINETYPE_ENDWHILE:
       UserFunc_req++;
       int i=UserFunc_stackl;
       while (i>0)
       {
        i--;
        if (UserFunc_stack[i].tag&2) 
        {
         if (UserFunc_req<2) break;
         UserFunc_req--;
         UserFunc_stack[i].tag&=1;
        }
       }
       while (UserFunc_stackl>0)
       {
        UserFunc_stackl--;
        if (UserFunc_stack[UserFunc_stackl].tag&2) {UserFunc_stackl++; break;}
        if ((UserFunc_stack[UserFunc_stackl].tag&1)==0)
        UserFunc_stack[UserFunc_stackl].lineaddr->next=UserFunc_stack[i].lineaddr;
        else
        if ((UserFunc_stack[UserFunc_stackl].tag&1)==1)
        UserFunc_stack[UserFunc_stackl].lineaddr->cnext=UserFunc_stack[i].lineaddr;
       }
       UserFunc_checkcnt--;
       break;
 }
}

int UserFunc_End()
{
 if (CExpr::MultiExec) return 0;
 if (UserFunc_ison==0) return 0;
 UserFunc_ison=0;
 if (UserFunc_checkcnt)
 {
  CCon::GetCon()->Put("\04Syntax error");
 }
 
 //dodaj funkcje do listy wyazen
 ELEMENT e;
 e.name=UserFunc_name;
 switch (UserFunc_parcount)
 {
  case 0: e.fptr=(void*)UserFunc_f0; break;
  case 1: e.fptr=(void*)UserFunc_f1; break;
  case 2: e.fptr=(void*)UserFunc_f2; break;
  case 3: e.fptr=(void*)UserFunc_f3; break;
  case 4: e.fptr=(void*)UserFunc_f4; break;
  case 5: e.fptr=(void*)UserFunc_f5; break;
  case 6: e.fptr=(void*)UserFunc_f6; break;
  case 7: e.fptr=(void*)UserFunc_f7; break;
  case 8: e.fptr=(void*)UserFunc_f8; break;
  default: return 0;
 }
 e.parcount=UserFunc_parcount;
 e.rtype=VAL_FLOAT;
 e.tag=UserFunc_count|TAG_EXTRACODE;
 e.partypes=0;
 UserFunc_clib->AddElement(&e);
 UserFunc_count++;
 return 1;
}

int UserFunc_AddVar(char *name)
{
 if (!CSys::IsValidStr(name)) return 0;
 if (CExpr::MultiExec) return 0;
 float *a;
 a=new float;
 ELEMENT e={name,a,VAL_FLOAT,VAR,0,0};
 UserFunc_clib->AddElement(&e);
 return 1;
}

int UserFunc_Del(char *name)
{
 if (!CSys::IsValidStr(name)) return 0;
 if (CExpr::MultiExec) return 0;
 //zmienna bedzie nadal istniec w pamieci, ale jest niewidoczna dla uzytkownika
 //natomiast jest widoczna dla skompilowanego wyrazenia
 UserFunc_clib->DelElement(name);
 return 1;
}

ELEMENT UserFunc_lib[]=
{
 {"_1",&UserFunc_1,VAL_FLOAT,VAR,0,0},
 {"_2",&UserFunc_2,VAL_FLOAT,VAR,0,0},
 {"_3",&UserFunc_3,VAL_FLOAT,VAR,0,0},
 {"_4",&UserFunc_4,VAL_FLOAT,VAR,0,0},
 {"_5",&UserFunc_5,VAL_FLOAT,VAR,0,0},
 {"_6",&UserFunc_6,VAL_FLOAT,VAR,0,0},
 {"_7",&UserFunc_7,VAL_FLOAT,VAR,0,0},
 {"_8",&UserFunc_8,VAL_FLOAT,VAR,0,0},
 {"F_BEGIN",(void*)UserFunc_Begin,VAL_INT,2,VAL_STR|(VAL_INT*4),0},
 {"F_END",(void*)UserFunc_End,VAL_INT,0,0,0},
 {"F_VAR",(void*)UserFunc_AddVar,VAL_INT,1,VAL_STR,0},
 {"F_DEL",(void*)UserFunc_Del,VAL_INT,1,VAL_STR,0}
};

void UserFunc_Init(CLib *library)
{
 library->AddList(UserFunc_lib,sizeof(UserFunc_lib)/sizeof(ELEMENT));
 UserFunc_clib=library;
}
