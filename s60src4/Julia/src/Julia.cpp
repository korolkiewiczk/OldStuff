//Julia.cpp

/*
  S60Graph by Kamil Korolkiewicz 2008
*/

#include <eikstart.h>
#include <coemain.h>
#include <aknapp.h>
#include <akndoc.h>
#include <aknappui.h>
#include <coecntrl.h>
#include <avkon.hrh>
#include <hal.h>

#include "Julia.h"

#define S60GRAPH_AUTOVIDMODE 1

//---------------------------
//S60Graph functions and data
//---------------------------

CPeriodic *s60graph_periodic;
CFbsBitmap *s60graph_fbsbitmap;
int s60graph_scrwidth;
int s60graph_scrheight;
int s60graph_scrbpp=32;
int s60graph_autovidmode=1;
int s60graph_buflocked=0;
int pitch;
void *buf;
unsigned char s60graph_keytab[256];
TCallBack callback;

//set video mode wXhXbpp with freq hz
void SetVidMode(int w,int h,int bpp,int flags)
{
 s60graph_autovidmode=(flags&&S60GRAPH_AUTOVIDMODE)>0;
 if (s60graph_buflocked) s60graph_fbsbitmap->UnlockHeap(ETrue);
 if (s60graph_fbsbitmap)
 {
  delete s60graph_fbsbitmap;
 }
 s60graph_fbsbitmap=new (ELeave) CFbsBitmap;
 TDisplayMode mode;
 switch (bpp)
 {
  case 16: mode=EColor64K; break;
  case 32: mode=EColor16MU; break;
  default: mode=EColor64K; break;
 }
 if (s60graph_fbsbitmap->Create(TSize(w,h),mode)!=KErrNone) User::Exit(-1);
 if (s60graph_buflocked) 
 {
  s60graph_fbsbitmap->LockHeap(ETrue);
  buf=(void*)s60graph_fbsbitmap->DataAddress();
 }
 s60graph_scrwidth=w;
 s60graph_scrheight=h;
 s60graph_scrbpp=bpp;
 pitch=s60graph_scrwidth*(s60graph_scrbpp/8);
}

int KeyPressed(int k)
{
 return s60graph_keytab[k&255]==1;
}

int GetKey(int k)
{
 int kp=s60graph_keytab[k&255]==1;
 s60graph_keytab[k&255]=0;
 return kp;
}

double GetTime()
{
 int freq;
 HAL::Get(HAL::EFastCounterFrequency,freq);
 return (double)User::FastCounter()/(double)freq;
}

#define SetBuf16(x,y,c) (*(unsigned short*)((int)buf+(y)*pitch+((x)<<1))=c)
#define SetBuf32(x,y,c) (*(unsigned int*)((int)buf+(y)*pitch+((x)<<2))=c)
#define RGB16(r,g,b) (unsigned short)((((r)&255)>>3)<<11)|((((g)&255)>>2)<<5)|(((b)&255)>>3)
#define RGB32(r,g,b) (unsigned int)(((r)&255)<<16)|(((g)&255)<<8)|((b)&255)
#define GetScrWidth() s60graph_scrwidth
#define GetScrHeight() s60graph_scrheight
#define GetBPP() s60graph_scrbpp

//
#include "main.cpp"
//

TInt _callbackfunc(TAny *r)
{
 int ret=1;
 CAppUi* ref=(CAppUi*)r;
 if (!s60graph_fbsbitmap) return 1;
 
 s60graph_fbsbitmap->LockHeap(ETrue);
 s60graph_buflocked=1;
 buf=(void*)s60graph_fbsbitmap->DataAddress();
 pitch=s60graph_scrwidth*(s60graph_scrbpp/8);
 
 if (buf)
 ret=AppMainLoop();
 
 s60graph_fbsbitmap->UnlockHeap(ETrue);
 s60graph_buflocked=0;
 
 if (ret==0)
 {
  AppExit();
  ref->Exit();
 }
 ref->iAppView->DrawNow();
 return 1;
}

//---------------------------

//

const TUid KAppUid = {0xAD644914};


CApaDocument* CApplication::CreateDocumentL()
{
 return (static_cast<CApaDocument*>( CDocument::NewL(*this)));
}


TUid CApplication::AppDllUid() const
{
 return KAppUid;
}

//

CDocument* CDocument::NewL(CEikApplication& aApp)
{
 CDocument* self=new (ELeave) CDocument(aApp);
 self->ConstructL();
 return self;
}

void CDocument::ConstructL() {}
CDocument::CDocument(CEikApplication& aApp) : CAknDocument(aApp) {}
CDocument::~CDocument() {}

CEikAppUi* CDocument::CreateAppUiL()
{
 return (static_cast <CEikAppUi*> (new (ELeave)CAppUi));
}

//

void CAppUi::ConstructL()
{
 BaseConstructL(CAknAppUi::EAknEnableSkin);
 iAppView=CAppView::NewL(ApplicationRect());
  
 if (AppInit()==0) User::Exit(-1);
 
 s60graph_periodic=CPeriodic::NewL(CActive::EPriorityLow);
 callback=TCallBack(_callbackfunc,this);
 s60graph_periodic->Start(10,10,callback);

 SetKeyBlockMode(ENoKeyBlock);
}

CAppUi::CAppUi() {}
CAppUi::~CAppUi()
{
 if (iAppView)
 {
  delete s60graph_fbsbitmap;
  delete s60graph_periodic;
  delete iAppView;
  iAppView = NULL;
 }
}

TKeyResponse CAppUi::HandleKeyEventL(const TKeyEvent& kevent,TEventCode ecode)
{
 switch (ecode)
 {
  case EEventKeyDown:
           s60graph_keytab[kevent.iScanCode]=1;
           break;
  case EEventKeyUp:
           s60graph_keytab[kevent.iScanCode]=0;
           break;
  default:
           return EKeyWasNotConsumed;
 }
 return EKeyWasConsumed;
}

void CAppUi::HandleCommandL(TInt aCommand) 
{
 switch(aCommand)
 {
  case EEikCmdExit:
  Exit();
  break;
  default:
  break;
 }
}

void CAppUi::HandleStatusPaneSizeChange()
{
 iAppView->SetRect(ApplicationRect());	
} 

//

CAppView* CAppView::NewL(const TRect& aRect)
{
 CAppView* self = new (ELeave) CAppView;
 self->ConstructL(aRect);
 return self;
}

void CAppView::ConstructL(const TRect& aRect)
{
 CreateWindowL();
 SetRect(aRect);
 ActivateL();
}

CAppView::CAppView() {}
CAppView::~CAppView() {}

void CAppView::Draw( const TRect& /*aRect*/ ) const
{
 CWindowGc& gc=SystemGc();
 TRect rect(Rect());
 int w=rect.Width(),h=rect.Height();
 if (s60graph_autovidmode)
 {
  TSize s;
  if (s60graph_fbsbitmap)
  s=s60graph_fbsbitmap->SizeInPixels();
  else s=TSize(0,0);
  if ((s.iWidth!=w)||(s.iHeight!=h)) 
  {
   SetVidMode(w,h,s60graph_scrbpp,S60GRAPH_AUTOVIDMODE);
   return;
  }
 }
 if ((s60graph_scrwidth==w)&&(s60graph_scrheight==h))
 {
  gc.BitBlt(TPoint(0,0),s60graph_fbsbitmap);
 }
 else
 {
  CFbsBitmap *temp;
  temp=new (ELeave) CFbsBitmap;
  temp->Create(rect.Size(),s60graph_fbsbitmap->DisplayMode());
  
  int dx,dy;
  dx=((s60graph_scrwidth<<16)/w);
  dy=((s60graph_scrheight<<16)/h);
  int x=0,y=0;
  int i,j;
  if (s60graph_scrbpp==16)
  {
   unsigned short *s,*d,*s_;
   s60graph_fbsbitmap->LockHeap(ETrue);
   s_=(unsigned short *)s60graph_fbsbitmap->DataAddress();
   temp->LockHeap(ETrue);
   d=(unsigned short *)temp->DataAddress();
   for (j=0; j<h; j++)
   {
    s=(unsigned short *)((int)s_+(((y>>16)*s60graph_scrwidth)<<1));
    x=0;
    for (i=0; i<w; i++)
    {
     *d=*(unsigned short *)((int)s+((x>>16)<<1));
     d++;
     x+=dx;
    }
    y+=dy;
   }
 
   temp->UnlockHeap(ETrue);
   s60graph_fbsbitmap->UnlockHeap(ETrue);
  }
  else
  if (s60graph_scrbpp==32)
  {
   unsigned int *s,*d,*s_;
   s60graph_fbsbitmap->LockHeap(ETrue);
   s_=(unsigned int *)s60graph_fbsbitmap->DataAddress();
   temp->LockHeap(ETrue);
   d=(unsigned int *)temp->DataAddress();
   for (j=0; j<h; j++)
   {
    s=(unsigned int *)((int)s_+(((y>>16)*s60graph_scrwidth)<<2));
    x=0;
    for (i=0; i<w; i++)
    {
     *d=*(unsigned int *)((int)s+((x>>16)<<2));
     d++;
     x+=dx;
    }
    y+=dy;
   }
 
   temp->UnlockHeap(ETrue);
   s60graph_fbsbitmap->UnlockHeap(ETrue);
  }
  gc.BitBlt(TPoint(0,0),temp);
  delete temp;
 }
}

void CAppView::SizeChanged()
{  
 DrawNow();
}

//

LOCAL_C CApaApplication* NewApplication()
{
 return new CApplication;
}

GLDEF_C TInt E32Main()
{
 return EikStart::RunApplication(NewApplication);
}
