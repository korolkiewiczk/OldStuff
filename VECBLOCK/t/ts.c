#include "vbengine.h"

int __(int _) {return _;}

//TSurf surf={100,50,0};
TSurf *surf;
TSurf *surf2;
TSurf tsurf={8,8,0};
TRect r1={0,0,320,200};
TRect r2={0,0,320,200};
TRect ar1={0,0,40,20};
TRect ar2={40,40,40,20};

int main()
{
 /*_xstub32init();
 InitMem(10000,2*1024*1024);
 /*printf ("mem=%x\n",mem_dheapwsk);
 char buf[1024];
 memset(&buf,0,1024);
 TCF cf;
 int w=CF_Load(&cf,"p.cf");
 printf ("%d,%d\n",cf.fhead.nf,cf.isloaded);
 int i;
 for (i=0; i<cf.fhead.nf; i++) printf ("%s,%d\n",cf.ftab[i].name,cf.ftab[i].pos);
 printf ("%d\n",w);
 if (w!=1) return 0;
 w=CF_Read(&cf,&buf,1,123,"a.txt");
 printf ("cf_read:%d\n",w);
 void *wsk=buf+w;
 w=CF_Curr (&cf,"kat/test.txt");
 printf ("cf_curr:%d\n",w);
 w=CF_CRead(&cf,wsk,5870,123);
 printf ("cf_cread:%d\n",w);
 printf ("%s\n",buf);
 w=CF_Close(&cf);
 printf ("%d\n",w);
 printf ("mem=%x\n",mem_dheapwsk);
 CloseMem();*/
 /*scanf("%s",buf);
 int w=CF_Read(NULL,&buf,1023,buf);
 if (w==-1) return 0;
 buf[w]=0;
 printf ("%d,%s",w,buf);*/
 _xstub32init();
 int n,w,i,j;
 InitMem(2000000,2*1024*1024);
 InitExpr();
 n=InitVid();
 /*while (1)
 {
 printf ("%d\n",n);
 for (i=0;i<n;i++)
 {
  printf ("%d: modenum=%4x lfb=%10x\t%dx%d\n",i,vid_modes[i].modeval,vid_modes[i].lfb,vid_modes[i].scrwidth,vid_modes[i].scrheight);
 }
 scanf("%d",&w);
 w=Vid_SetMode(w);
 if (w==0) return 0;
 for (j=0; j<vid_buf.sheight; j++)
 for (i=0; i<vid_buf.swidth; i++) *(vid_buf.sbits+(j*vid_buf.swidth+i))=i*j;
 Vid_Swap();
 MReadKey();
 Vid_BackTXT();
 printf ("%x,%x\n",vid_lfb,vid_surf.sbits);
 }*/
 TCF cf;
 /*if (CF_Load(&cf,"p.cf")==0) return 0;
 if (Vid_LoadPalFromBMP(&cf,"a1.bmp")==0) return 0;
 if (Surf_LoadFromBMP(&cf,&surf,"a1.bmp")==0) return 0;*/
 if (CF_Load(&cf,"c.cf")!=1) return 0;
 if (Vid_LoadPalFromBMP(&cf,"bm2.bmp")==0) return 0;
 if (Surf_LoadFromBMP(&cf,&surf,"bm2.bmp")==0) return 0;
 if (Surf_LoadFromBMP(&cf,&surf2,"bm1.bmp")==0) return 0;
 if (Vid_InitFadeTab(5)==0) return 0;
 if (Vid_InitTranspTab(4)==0) return 0;
 ar2.rw=ar1.rw=surf2->swidth;
 ar2.rh=ar1.rh=surf2->sheight;
 Surf_Alloc(&tsurf);
 for (j=0; j<8; j++)
 for (i=0; i<8; i++) *(tsurf.sbits+((j<<3)+i))=Vid_FindRGB(6*j+10,40,10);
 //memset(tsurf.sbits,3,64);
 Vid_SetMode(0);
 //if (Surf_Alloc(&surf)==0) return 0;
 //printf ("%x\n",surf.sbits);
 //if (Surf_Create(&surf,40,20)==0) return 0;
 /*for (j=0;j<20;j++)
 for (i=0;i<40;i++) *(surf->sbits+j*40+i)=i^j;*/
 r1.rw=surf->swidth;
 r1.rh=surf->sheight;
 r2.rw=vid_buf.swidth;
 r2.rh=vid_buf.sheight;
 vid_wait=1;
 i=j=0;
 unsigned short p;
 //for (j=0;j<32<<4;j++)//(r2.rx=-vid_buf.swidth;r2.rx<=320;r2.rx++)
 while (1)
 {
  memset(vid_buf.sbits,0,vid_buf.swidth*vid_buf.sheight);
  Surf_SBlt (&r1,&r2,surf,&vid_buf);
  //Surf_FSBlt (&r1,&r2,surf,&vid_buf,i);
  Surf_TBlt (&ar1,&ar2,surf2,&vid_buf,j,i);
  //DrawSurface8C (&ar1,&ar2,surf2,&vid_buf);
  Surf_Printf8X8(0,0,Vid_FindRGB(63,0,10),biosfont,&vid_buf,"%d",i);
  Vid_Swap();
  p=MReadKey()>>8;
  if (p==1) break;
  if (p==K_PGUP) i++;
  if (p==K_PGDOWN) i--;
  if (p==K_HOME) j++;
  if (p==K_END) j--;
  if (p==K_UP) ar2.ry--;
  if (p==K_LEFT) ar2.rx--;
  if (p==K_RIGHT) ar2.rx++;
  if (p==K_DOWN) ar2.ry++;
  i%=1<<TRANSP8TabShift;
 }
 MReadKey();
 Vid_SetBrightness(6);
 Surf_Print8X8(100,100,Vid_FindRGB(10,60,60),biosfont,&vid_buf,"Kamil");
 Surf_Print8X8(100,-2,Vid_FindRGB(60,0,30),biosfont,&vid_buf,"Kamil");
 Surf_Print8X8(-6,100,Vid_FindRGB(50,0,50),biosfont,&vid_buf,"Kamil");
 Surf_Print8X8(100,198,Vid_FindRGB(0,0,60),biosfont,&vid_buf,"Kamil");
 Surf_Print8X8(307,130,Vid_FindRGB(0,60,0),biosfont,&vid_buf,"Kamil");
 Surf_Print8X8(100,50,(int)&tsurf,biosfont,&vid_buf,"abc\nef\tg\n");
 Surf_Printf8X8(10,70,(int)&tsurf,biosfont,&vid_buf,"abc%d",20);
 Surf_Print8X8(-28,170,Vid_FindRGB(50,50,40),biosfont,&vid_buf,"Kamildsfkdfkdjfkgfksgkfgkdgdfgfd_df_df_g_kam__gfdgdfgfhhhhhhhfhghgf545345435435hgjhgjgjhjjgfjkkdfgkdfkgdfkgkdfgkdfgkfdgdfg");
 Surf_Print8X8(-59,10,Vid_FindRGB(0,0,60),biosfont,&vid_buf,"fdfdfdKamil");
 Surf_Print8X8(100,-198,3,biosfont,&vid_buf,"Kamil");
 Surf_Print8X8(300,196,6,biosfont,&vid_buf,"453345Ka45mil");
 Vid_Swap();
 MReadKey();
 Vid_SetBrightness(-4);
 /*for (j=0;j<200;j++)
 for (i=0;i<320;i++) *(vid_buf.sbits+j*320+i)=Vid_FindRGB(i/4,(i+j)/4,i*j/64);*/
 Vid_Swap();
 MReadKey();
 Vid_BackTXT();
 CF_Close(&cf);
 /*printf ("%x,%x,%x,%d,%d,%x\n",vid_pal,&surf,surf,surf->swidth,surf->sheight,surf->sbits);
 printf ("mem=%x\n",mem_dheapwsk);
 unsigned char *wsk=vid_pal;
 for (i=0; i<64; i++)
 {
  printf("%2x,%2x,%2x|",*wsk,*(wsk+1),*(wsk+2));
  wsk+=3;
 }
 printf ("%x,%d,%d,%x\n",surf2,surf2->swidth,surf2->sheight,surf2->sbits);
 __(0x21436587);
 printf ("aw1=%x,aw2=%x,asb=%x\n",mem_buf[0][0],mem_buf[1][0],mem_buf[2][0]);
 printf ("w1=%x,w2=%x,sb=%x\n",mem_buf[0][1],mem_buf[1][1],mem_buf[2][1]);
 Surf_Free(&surf);
 printf ("%x,%d,%d,%x\n",surf2,surf2->swidth,surf2->sheight,surf2->sbits);
 printf ("aw1=%x,aw2=%x,asb=%x\n",mem_buf[0][0],mem_buf[1][0],mem_buf[2][0]);
 printf ("w1=%x,w2=%x,sb=%x\n",mem_buf[0][1],mem_buf[1][1],mem_buf[2][1]);
 __(0x21436587);
 Surf_Dispose(&surf2);
 printf ("%x,%d,%d,%x\n",surf2,surf2->swidth,surf2->sheight,surf2->sbits);
 printf ("aw1=%x,aw2=%x,asb=%x\n",mem_buf[0][0],mem_buf[1][0],mem_buf[2][0]);
 printf ("w1=%x,w2=%x,sb=%x\n",mem_buf[0][1],mem_buf[1][1],mem_buf[2][1]);
 CloseMem();*/
 CloseMem();
}
