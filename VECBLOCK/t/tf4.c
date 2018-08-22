#include "vbengine.h"

TCF cf;
float hwidth=20;
unsigned char c1,c2,c3;
TFuncVar x={0,0,0,0};
int comp=0;
double pi=3.14f;

void exitp()
{
 Vid_BackTXT();
 CloseKeySys();
 CF_Close(&cf);
 CloseMem();
}

void hline(int x,int y1,int y2,unsigned char c)
{
 int s,i;
 if ((y1>=vid_buf.sheight)||(y2<0)) return;
 if (y1<0)y1=0;
 if (y2>=vid_buf.sheight)y2=vid_buf.sheight-1;
 if (y1>y2) {i=y2; y2=y1; y1=i;}
 i=y1*vid_buf.swidth+x;
 unsigned char *wsk=vid_buf.sbits+i;
 while (y1<=y2)
 {
  *wsk=c;
  wsk+=vid_buf.swidth;
  y1++;
 }
}

void w()
{
 comp=1-comp;
 strcpy(con_str,"x");
 Con_EnterCmd();
 con_edwsk=0;
}

void shw(int x)
{
 hwidth=x;
}

void fdrw()
{
 int i;
 float y,oy;
 int iy,ioy;
 if (comp) return;
 if (con_isenter) Con_PrevCommand();
 char onp[1024];
 Expr_ONP(onp,con_str);
 Con_Print(onp);
 x.value=(hwidth/vid_buf.swidth)*(-vid_buf.swidth-1);
 oy=Expr_Comp (con_str);
 if (Expr_CheckErr(0)!=EXPR_OK) return;
 for (i=0; i<vid_buf.sheight; i++)
 {
  if (((i-vid_buf.sheight/2)%(vid_buf.sheight/(int)hwidth))==0)
  *(vid_buf.sbits+(vid_buf.swidth/2+(i*vid_buf.swidth)))=c1;
  else
  *(vid_buf.sbits+(vid_buf.swidth/2+(i*vid_buf.swidth)))=c2;
 }
 for (i=0; i<vid_buf.swidth; i++)
 {
  if (((i-vid_buf.swidth/2)%(vid_buf.swidth/(int)hwidth))==0)
  *(vid_buf.sbits+(i+(vid_buf.sheight/2)*vid_buf.swidth))=c1;
  else
  *(vid_buf.sbits+(i+(vid_buf.sheight/2)*vid_buf.swidth))=c2;
  x.value=(hwidth/vid_buf.swidth)*(i-vid_buf.swidth/2);
  y=Expr_Comp (con_str);
  iy=vid_buf.sheight/2-y*(vid_buf.sheight/hwidth);
  ioy=vid_buf.sheight/2-oy*(vid_buf.sheight/hwidth);
  hline (i,ioy,iy,c3);
  oy=y;
 }
}

void drw()
{
 Surf_Printf8X8(0,vid_buf.sheight-9,Vid_FindRGB((int)globaltime%10+200,
 (int)globaltime%10+200,(int)globaltime%10+200),&biosfont,&vid_buf,"%d,%d",con_cursorpos,con_edshf);//"%0.3f",globaltime);
}

TFuncVar wf={w,0,0,0};
TFuncVar shwf={shw,1,0,0};

int main()
{
 _xstub32init();
 atexit(exitp);
 InitMem(65536*16,65536*16);
 CF_Load (&cf,"c.cf");
 InitExpr();
 InitVR();
 InitTime();
 InitKeySys();
 InitLoopSys(fdrw,drw);
 TFuncVar q={&exit,1,0,0};
 Expr_AddFV ("exit",&q);
 Expr_AddFV ("x",&x);
 Expr_AddFV ("w",&wf);
 Expr_AddFV ("shw",&shwf);
 VR_AddName("scrwidth",VR_INT,&vid_buf.swidth);
 VR_AddName("scrheight",VR_INT,&vid_buf.sheight);
 VR_AddName("time",VR_DOUBLE,&globaltime);
 VR_AddName("pi",VR_DOUBLE,&pi);
 if (Vid_LoadPalFromBMP(&cf,"conbmp.bmp")==0) return 0;
 InitConsole(&biosfont,Vid_FindRGB(128,128,128),&cf,"conbmp.bmp",0.5,0.1);
 Con_SetnLines (1);
 InitVid();
 Vid_SetMode(0);
 c1=Vid_FindRGB(255,255,255);
 c2=Vid_FindRGB(128,128,128);
 c3=Vid_FindRGB(196,196,196);
 Loop_Entry();
}
