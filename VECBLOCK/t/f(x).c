#include "vbengine.h"

TCF cf;
float hwidth=20,hheight;
unsigned char c1,c2,c3;
TFuncVar x={0,0,0,0};
int comp=0;
double pi=3.14f;
time_f ttt;
TTime tr;
char txt[100];
int mmm,m2,m3;
int isy2=0;

void exitp()
{
 Vid_BackTXT();
 CloseKeySys();
 CF_Close(&cf);
 CloseMem();
 printf ("%d,%x",m2-(mmm-(int)mem_dheapwsk),__dpmi_get_segment_limit(_my_ds()));
}

void hline(int x,int y1,int y2,unsigned char c)
{
 int s,i;
 if (abs(y1-y2)>10000) return;
 if (y1>y2) {i=y2; y2=y1; y1=i;}
 if ((y1>=vid_buf.sheight)||(y2<0)) return;
 if (y1<0)y1=0;
 if (y2>=vid_buf.sheight)y2=vid_buf.sheight-1;
 i=y1*vid_buf.swidth+x;
 unsigned char *wsk=vid_buf.sbits+i;
 while (y1<=y2)
 {
  *wsk=c;
  wsk+=vid_buf.swidth;
  y1++;
 }
}

int prntv()
{
 TVarRefTab *vrt=vr_tab,*stack[100];
 int stacki[100];
 char buf[100];
 int i=0,j=0,st=0,sti=0;
 while (1)
 {
  while(i<VR_MAXCHARS)
  {
   if (vrt->namechar[i]!=NULL)
   {
    buf[j++]=i+'A';
    stack[st++]=vrt;
    stacki[sti++]=i;
    vrt=vrt->namechar[i];
    i=0;
   } else i++;
  }
  if (st==0) break;
  vrt=stack[--st];
  i=stacki[--sti]+1;
  if (j>0)
  {
   buf[j]=0;
   Con_Print(buf);
  }
  j--;
 }
 return st;
}

void w()
{
 comp=1-comp;
 strcpy(con_str,"x");
 Con_EnterCmd();
 con_edwsk=0;
}

void txtp()
{
 Con_Print(txt);
}

void shw(int x)
{
 hwidth=x;
 hheight=(float)vid_buf.sheight/(float)vid_buf.swidth*hwidth;
}

void sy2(int a)
{
 isy2=a;
}

void fdrw()
{
 int i,j;
 float y,oy,y2,oy2;
 int iy,ioy;
 if (comp) return;
 if (con_key==K_ENTER) Con_PrevCommand();
 char onp[1024];
 //Expr_ONP(onp,con_str);
 //Con_Print(onp);
 x.value=(hwidth/vid_buf.swidth)*(-vid_buf.swidth/2-1);
 Time_Get(&tr);
 if (isy2==0)
 oy=Expr_Comp (con_str);
 else
 {
  oy=sqrt(Expr_Comp (con_str));
  oy2=-sqrt(Expr_Comp (con_str));
 }
 ttt=Time_Get(&tr);
 //if (con_key!=0) Con_Printf("%f,%d,%d",ttt,mem_sheaptop-mem_sheapwsk,mem_dheaptop-mem_dheapwsk);
 if (Expr_CheckErr(0)!=EXPR_OK) return;
 j=hheight/2;
 for (i=0; i<vid_buf.sheight; i++)
 {
  if (((i-vid_buf.sheight/2)%(vid_buf.sheight/(int)hheight))==0)
  {
   *(vid_buf.sbits+(vid_buf.swidth/2+(i*vid_buf.swidth)))=c1;
   *(vid_buf.sbits+(vid_buf.swidth/2+(i*vid_buf.swidth))-1)=c1;
   *(vid_buf.sbits+(vid_buf.swidth/2+(i*vid_buf.swidth))+1)=c1;
   Surf_Printf8X8(vid_buf.swidth/2-23,i-4,c1,&biosfont,&vid_buf,"%3d",j--);
  }
  else
  *(vid_buf.sbits+(vid_buf.swidth/2+(i*vid_buf.swidth)))=c2;
 }
 j=-hwidth/2;
 for (i=0; i<vid_buf.swidth; i++)
 {
  if (((i-vid_buf.swidth/2)%(vid_buf.swidth/(int)hwidth))==0)
  {
   if ((j!=0)&&(j!=hwidth/2))
   {
    *(vid_buf.sbits+(i+(vid_buf.sheight/2)*vid_buf.swidth))=c1;
    *(vid_buf.sbits+(i+(vid_buf.sheight/2)*vid_buf.swidth)-vid_buf.swidth)=c1;
    *(vid_buf.sbits+(i+(vid_buf.sheight/2)*vid_buf.swidth)+vid_buf.swidth)=c1;
    Surf_Printf8X8(i-10,vid_buf.sheight/2+2,c1,&biosfont,&vid_buf,"%2d",j);
   }
   j++;
  }
  else
  *(vid_buf.sbits+(i+(vid_buf.sheight/2)*vid_buf.swidth))=c2;
  x.value=(hwidth/vid_buf.swidth)*(i-vid_buf.swidth/2);
  if (isy2==0)
  {
   y=Expr_Comp (con_str);
   iy=vid_buf.sheight/2-y*(vid_buf.sheight/hheight);
   ioy=vid_buf.sheight/2-oy*(vid_buf.sheight/hheight);
   hline (i,ioy,iy,c3);
   oy=y;
  }
  else
  {
   y=sqrt(Expr_Comp (con_str));
   y2=-sqrt(Expr_Comp (con_str));
   iy=vid_buf.sheight/2-y*(vid_buf.sheight/hheight);
   ioy=vid_buf.sheight/2-oy*(vid_buf.sheight/hheight);
   hline (i,ioy,iy,c3);
   iy=vid_buf.sheight/2-y2*(vid_buf.sheight/hheight);
   ioy=vid_buf.sheight/2-oy2*(vid_buf.sheight/hheight);
   hline (i,ioy,iy,c3);
   oy=y;
   oy2=y2;
  }
 }
 Surf_Printf8X8(vid_buf.swidth-7,vid_buf.sheight/2-3,c1,&biosfont,&vid_buf,"");
 Surf_Printf8X8(vid_buf.swidth-10,vid_buf.sheight/2+4,c1,&biosfont,&vid_buf,"x");
 Surf_Printf8X8(vid_buf.swidth/2-4,-1,c1,&biosfont,&vid_buf,"");
 Surf_Printf8X8(vid_buf.swidth/2+6,4,c1,&biosfont,&vid_buf,"y");
 if (isy2==0)Surf_Printf8X8(0,0,Vid_FindRGB(0,0,0),&biosfont,&vid_buf,"f(x)=%s",con_str);
 else Surf_Printf8X8(0,0,Vid_FindRGB(0,0,0),&biosfont,&vid_buf,"y^2=%s",con_str);
}

void drw()
{
 /*Surf_Printf8X8(0,vid_buf.sheight-9,Vid_FindRGB((int)globaltime%10+200,
 (int)globaltime%10+200,(int)globaltime%10+200),&biosfont,&vid_buf,"%d,%d",con_cursorpos,con_edshf);//"%0.3f",globaltime);*/
 #define drw_str "Copyright (c) Kamil Korolkiewicz 2006"
 Surf_Print8X8(vid_buf.swidth-8-strlen(drw_str)*8,vid_buf.sheight-9,Vid_FindRGB(0,0,0),&biosfont,&vid_buf,drw_str);
}

TFuncVar wf={w,0,0,0};
TFuncVar shwf={shw,1,0,0};
TFuncVar txtf={txtp,0,0,0};
TFuncVar delf={VR_Delete,1,0,0};
TFuncVar prntf={prntv,0,0,0};
TFuncVar sy2f={sy2,1,0,0};

int main()
{
 _xstub32init();
 atexit(exitp);
 InitMem(1024*1024*2,1024*1024);
 CF_Load (&cf,"c.cf");
 InitExpr();
 InitVR();
 InitTime();
 InitKeySys();
 InitLoopSys(fdrw,drw);
 Time_InitStruct(&tr,TIME_FLAGS_ABSOLUTE_TIME);
 TFuncVar q={&exit,1,0,0};
 Expr_AddFV ("exit",&q);
 Expr_AddFV ("x",&x);
 Expr_AddFV ("w",&wf);
 Expr_AddFV ("shw",&shwf);
 Expr_AddFV ("txt",&txtf);
 Expr_AddFV ("del",&delf);
 Expr_AddFV ("prt",&prntf);
 Expr_AddFV ("y2",&sy2f);
 m2=(int)mem_dheapwsk;
 VR_New("scrwidth",VR_INT,&vid_buf.swidth);
 VR_New("scrheight",VR_INT,&vid_buf.sheight);
 VR_New("time",VR_DOUBLE,&globaltime);
 VR_New("pi",VR_DOUBLE,&pi);
 VR_New("hwidth",VR_FLOAT,&hwidth);
 VR_New("smem",VR_INT,&mem_sheapwsk);
 VR_New("dmem",VR_INT,&mem_dheapwsk);
 m2=(int)mem_dheapwsk-m2;
 if (Vid_LoadPalFromBMP(&cf,"conbmp.bmp")==0) return 0;
 int i;
 for (i=0;i<768;i++) *(vid_pal+i)=63-*(vid_pal+i);
 *(vid_pal)=63;
 *(vid_pal+1)=63;
 *(vid_pal+2)=63;
 c1=Vid_FindRGB(0,0,0);
 c2=Vid_FindRGB(128,128,128);
 c3=Vid_FindRGB(32,32,32);
 InitConsole(0,Vid_FindRGB(255,255,255),&cf,"conbmp.bmp",0.5,0.1);
 Con_SetnLines (1);
 InitVid();
 Vid_SetMode(0);
 CF_Read(&cf,txt,10,100,"../a2.cfg");
 //__dpmi_set_segment_limit(_my_ds(),0xffffffff);
 if (!CF_Close(&cf)) exit(0);
 Cfg_Exec("a.cfg");
 con_printexprv=1;
 mmm=(int)mem_dheapwsk;
 shw(20);
 Loop_Entry();
}
