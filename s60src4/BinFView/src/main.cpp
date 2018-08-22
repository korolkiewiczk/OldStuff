#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include "keymap.h"
#include "cwin.h"
#include "view.h"
#include "utils.cpp"
#include "cwin.c"
#include "view.cpp"
char readmetext[]=
#include "readme.h"

int SCRWIDTH=240;
int SCRHEIGHT=320;
#define BPP 32

#define MAXFILES 4

CView *view;
CView *fileview[MAXFILES];
CView *helpview;
CView *statview;
int currentview;
int quit=0;
int fontl=1;

char *multimaptab[10]=
{
 "1,';[]",  "2abc",    "3def",
 "4ghi",         "5jkl",   "6mno",
 "7pqrs",        "8tuv",   "9wxyz",
                 "0 -/="
};


void ChangeMode()
{
  if (view)
  {
   for (int i=0; i<MAXFILES; i++)
   {
    fileview[i]->Change(SCRWIDTH,SCRHEIGHT,buf);
   }
   helpview->Change(SCRWIDTH,SCRHEIGHT,buf);
   statview->Change(SCRWIDTH,SCRHEIGHT,buf);
  }
}

void SetBuf()
{
 for (int i=0; i<MAXFILES; i++)
 {
  fileview[i]->ChangeBuf(buf);
 }
 helpview->ChangeBuf(buf);
 statview->ChangeBuf(buf);
}



int AppInit()
{
 for (int i=0; i<MAXFILES; i++)
 {
  fileview[i]=new CView(SCRWIDTH,SCRHEIGHT,buf);
  fileview[i]->SetNo(i+1);
 }
 view=fileview[0];
 currentview=0;
 
 helpview=new CView(SCRWIDTH,SCRHEIGHT,buf);
 helpview->SetString("Help",readmetext);
 helpview->SetNoLoadable();
 
 statview=new CView(SCRWIDTH,SCRHEIGHT,buf);
 statview->SetNoLoadable();
 
 /*int cml=User::CommandLineLength();
 TBuf<256> cl;
 User::CommandLine(cl);
 char a[256];
 if (cml>255) cml=255;
 
 desc2str(cl,a,cml);
 int i=0;
 while ((a[i]!=0)&&(a[i]!=' ')) i++;
 while (a[i]==' ') i++;
 
 if (view->LoadFile(&a[i])==0)
 {
  view->SetString("No file",readmetext);
 }*/
 
 view->SetString("No file",readmetext);
 
 SetVidMode(SCRWIDTH,SCRHEIGHT,BPP,S60GRAPH_AUTOVIDMODE);
 
 return 1;
}

int AppMainLoop()
{
 //zmiana trybu
 if ((GetScrWidth()!=SCRWIDTH)||(GetScrHeight()!=SCRHEIGHT))
 {
  SCRWIDTH=GetScrWidth();
  SCRHEIGHT=GetScrHeight();
  ChangeMode();
 }
 
 //obsluga programu
 
  int i;
  view->SetShift(s60graph_shifton);
  
  for (i=0; i<255; i++) if (KeyPressed(i))
  {
   GetKey(i);
   
   if (view->IsEditBoxOn())
   {
    int mode=view->GetVMode();
    int etype_f=view->EditBoxType()==CView::Edit_Find;
    
    if ((etype_f)&&(mode==CView::Bin_Mode))
    {
     if (i==K_0) i=2+9;
     else
     if (i==K_1) i=2;
     else
     if ((i>=K_2)&&(i<=K_9))
     i=0;
     view->EditBoxKey(i);
    }
    else
    if ((etype_f)&&(mode==CView::Dec_Mode))
    {
     if (i==K_0) i=2+9;
     else
     if ((i>=K_1)&&(i<=K_9)) i-=K_1-2;
     view->EditBoxKey(i);
    }
    else
    if ((etype_f)&&(mode==CView::Oct_Mode))
    {
     if (i==K_0) i=2+9;
     else
     if ((i>=K_1)&&(i<=K_7)) i-=K_1-2;
     else
     if ((i>=K_8)&&(i<=K_9))
     i=0;
     view->EditBoxKey(i);
    }
    else
    {
#define MKEY 225
     if (i==K_0) i=9+MKEY;
     if ((i>=K_1)&&(i<=K_9)) i-=K_1-MKEY;
    
     static int mmk=0,mmpk=-1,mmid=0,mmc=0;
     static float mmtimer=0;
     if ((i>=MKEY)&&(i<=9+MKEY))
     {
      mmk=i-MKEY;
   	  if ((mmk!=mmpk)||(GetTime()-mmtimer>0.75))
   	  //nowy znak
   	  {
   	   mmid=0;
   	   mmtimer=GetTime();
   	   mmc=multimaptab[mmk][0];
   	  }
   	  else
   	  //nastepny znak
   	  {
   	   view->EditBoxKey(K_BKSPACE);
   	   mmid++;
   	   if (mmid>=strlen(multimaptab[mmk])) mmid=0;
   	   mmtimer=GetTime();
   	   mmc=multimaptab[mmk][mmid];
   	  }
   	  mmpk=mmk;
   	  if (mmc)
      {
   	   for (int i=KEYMAPLEN-1; i>=0; i--)
   	   if (keymap[i][1]==mmc)
   	   {
   	    view->EditBoxKey(keymap[i][0]);
   	    break;
   	   }
   	   mmc=0;
      }
     }
     else
     view->EditBoxKey(i);
    }
    continue;
   }
   switch (i)
   {
    case K_UP: view->Command(CView::Cmd_LineUp); break;
    case K_6: view->Command(CView::Cmd_PageUp); break;
    case K_DOWN: view->Command(CView::Cmd_LineDown); break;
    case K_9: view->Command(CView::Cmd_PageDown); break;
    case K_7: view->EnterEditBox(CView::Edit_Pos); break;
    case K_8: view->EnterEditBox(CView::Edit_Find); break;
    case K_ENTER: view->Command(CView::Cmd_NextMode); break;
    case K_0: 
    if (fontl%3<2)
    	view->Command(CView::Cmd_ChangeFont); 
    	else
    	view->Command(CView::Cmd_ChangeFontSmall);
    	fontl++;
    	break;

    case K_LEFT: view->Command(CView::Cmd_Left); break;
    case K_RIGHT: view->Command(CView::Cmd_Right); break;
    case K_ASTERISK: view->Command(CView::Cmd_Home); break;
    case K_CHECK: view->Command(CView::Cmd_End); break;
    case K_CALL: if (view==helpview) view=fileview[currentview]; else view=helpview; break;
    case K_RSK: if ((view==helpview)||(view==statview)) view=fileview[0]; else
         quit=1; break;
         
    case K_1: currentview=0; break;
    case K_2: currentview=1; break;
    case K_3: currentview=2; break;
    case K_4: currentview=3; break;
    
    case K_5: 
         if (view==statview) {view=fileview[currentview]; break;}
         statview->SetString("File statisticts",view->GenStat());
         view=statview;
         break;
   }
   break;
  }
  if ((view!=statview)&&(view!=helpview))
  view=fileview[currentview];
  
  SetBuf();
  view->Show();

 
 return !quit;
}

int AppExit()
{
 return 1;
}