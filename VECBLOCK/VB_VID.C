#ifndef __VB_VID_C
#define __VB_VID_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_VID.C
 Opis: Obsluga trybow graficznych
------------------------------------------------
*/

//==========================================

#include "vb_vid.h"

//==========================================

TVidMode vid_modes[VID_MAXMODES];

TSurf vid_surf;         //pow. ekranu
TSurf vid_buf;          //pow. bufora
int vid_nmodes=0;
int vid_bsize;          //rozmiar bufora ekranu
int vid_lfb;            //czy lfb, jesli != 0 to adres do pam
int vid_isvga;          //czy tryb vga
int vid_gran;           //dla trybu 'bankowego'
unsigned char *vid_pal; //paleta kolorow
int vid_wait=1;         //czy czekac na powrot pionowy
int vid_modenum;        //numer aktualnego trybu graficznego
int vid_prtscr=0;       //czy dana klatka wymaga zrzutu ekranu
int vid_scrshootn=0;    //numer pliku screenshota

/*
########################################
 InitVid
 Inicjuje liste dostepnych trybow graficznych (tylko 8 bitowych)
 Zwraca ilosc dostepnych trybow
########################################
*/

int InitVid()
{
 char __vesainfoblock[256];
 char __vesainfo[512];
 int __vesainfoblockseg=-1;
 void *__vesainfoblockptr;
 int __vesainfoseg=-1;
 void *__vesainfoptr;
 void *__vesainfoblocksp;
 __dpmi_regs regs;
 void *modelist;
 unsigned short modenum;
 int pt;
 //standardowy tryb vga
 vid_modes[0].modeval=VGA320x200;
 vid_modes[0].scrwidth=320;
 vid_modes[0].scrheight=200;
 vid_modes[0].lfb=0;
 vid_nmodes=1;
 //dla vesa info
 if (__vesainfoseg==-1) __vesainfoseg=__dpmi_allocate_dos_memory(32,(int*)&pt);
 if (__vesainfoseg==-1) return vid_nmodes;
 regs.x.ax=0x4f00;
 regs.x.di=0;
 regs.x.es=__vesainfoseg;
 __dpmi_int(0x10,&regs);
 if ((regs.h.ah==1)||(regs.h.al!=0x4f)) return vid_nmodes;
 __vesainfoptr=&__vesainfo;
 _cpys0(__vesainfoptr,(void*)(__vesainfoseg<<4),512);
 //dla mode info
 if (__vesainfoblockseg==-1) __vesainfoblockseg=__dpmi_allocate_dos_memory(16,(int*)&pt);
 if (__vesainfoblockseg==-1) return vid_nmodes;
 __vesainfoblocksp=(void*)(__vesainfoblockseg<<4);
 __vesainfoblockptr=&__vesainfoblock;
 //wsk dla vesa info
 modelist=(void*)*(int*)((int)__vesainfoptr+0xe);
 modelist=(void*)((int)((int)modelist>>16)<<4)+((int)modelist&65535);
 while (vid_nmodes<VID_MAXMODES)
 {
  _cpys0(&modenum,modelist,2);
  if (modenum==0xffff) break;
  regs.x.ax=0x4f01;
  regs.x.cx=modenum;
  regs.x.di=0;
  regs.x.es=__vesainfoblockseg;
  __dpmi_int (0x10,&regs);
  if ((regs.h.ah==0)&&(regs.h.al==0x4f))
  {
   _cpys0(__vesainfoblockptr,__vesainfoblocksp,256);
   if (*(unsigned char*)(__vesainfoblockptr+0x19)==8)
   {
    vid_modes[vid_nmodes].modeval=modenum;
    vid_modes[vid_nmodes].scrwidth=*(unsigned short*)(__vesainfoblockptr+0x12);
    vid_modes[vid_nmodes].scrheight=*(unsigned short*)(__vesainfoblockptr+0x14);
    if ((*(unsigned char*)(__vesainfoblockptr)&128)==0)         //brak lfb
       vid_modes[vid_nmodes].lfb=0;
    else                                                        //jest lfb
       vid_modes[vid_nmodes].lfb=*(int*)(__vesainfoblockptr+0x28);
    vid_nmodes++;
   }
  }
  modelist+=2;
 }
 return vid_nmodes;
}

/*
########################################
 Vid_SetMode
 Ustawia dany tryb graficzny
 Jesli ok (jesli lfb 2 : 1 (dla vga 1)) : 0
########################################
*/

int Vid_SetMode (int modenum)
{
 __dpmi_meminfo meminfo;
 if (modenum>=vid_nmodes) return 0;
 int mode=vid_modes[modenum].modeval;
 if (vid_buf.sbits!=0) Mem_DFree((void*)&vid_buf.sbits);
 //memset(&vid_surf,0,sizeof(TSurf));
 //memset(&vid_buf,0,sizeof(TSurf));
 vid_bsize=vid_modes[modenum].scrwidth*vid_modes[modenum].scrheight;
 if (Mem_DAlloc((void*)&vid_buf.sbits,vid_bsize)==0)           //alokacja dynamiczna
 {
  Vid_SetMode(vid_modenum);
  return 0;
 }
 memset(vid_buf.sbits,0,vid_bsize);
 vid_surf.swidth=vid_buf.swidth=vid_modes[modenum].scrwidth;
 vid_surf.sheight=vid_buf.sheight=vid_modes[modenum].scrheight;
 if (mode<0x100)                                //dla vga
 {
  SetVGAMode(mode);
  vid_surf.sbits=(char*)0xa0000;
  vid_isvga=1;
  vid_lfb=0;
 }
 else                                           //dla vesa
 {
  if (vid_modes[modenum].lfb==0)        //brak lfb
  {
   vid_lfb=0;
   vid_surf.sbits=(char*)0xa0000;
   vid_gran=65536;//*(short*)((void*)__vesainfoblockptr+0x4)*1024;
  }
  else                                  //jest lfb
  {
   meminfo.size=vid_bsize*2;
   vid_lfb=meminfo.address=vid_modes[modenum].lfb;
   __dpmi_physical_address_mapping(&meminfo);
   vid_surf.sbits=(char*)meminfo.address;
  }
  SetVESAMode(mode,(vid_lfb==0)? 0 : 1<<14);
  vid_isvga=0;
 }
 if (vid_pal!=0) SetPalette(vid_pal);
 vid_modenum=modenum;
 return (vid_lfb)? 2 : 1;
}

/*
########################################
 Vid_BackTXT
 Wraca do trybu tekstowego
########################################
*/

#define Vid_BackTXT() SetVGAMode(VGATEXT)

/*
########################################
 Vid_Swap
 Rysuje powierzchnie bufora na ekranie
########################################
*/

void Vid_Swap()
{
 if (vid_wait) Wait4VSync();
 if (vid_prtscr) {Vid_PrintScr(); vid_prtscr=0;}
 if ((vid_lfb)||(vid_isvga))            //jesli tryb vga lub jest lfb
 {
  _cpyd0(vid_surf.sbits,vid_buf.sbits,vid_bsize);
 }
 else                                   //tryb 'bankowy'
 {
  int i,b=(vid_bsize/vid_gran)+1;
  char *bf=vid_buf.sbits;
  __dpmi_regs regs;
  for (i=0; i<b; i++)
  {
   regs.x.ax=0x4f05;
   regs.x.bx=0;
   regs.x.dx=i;
   __dpmi_int(0x10,&regs);
   _cpyd0(vid_surf.sbits,bf,vid_gran);
   bf+=vid_gran;
  }
 }
}

/*
########################################
 Vid_LoadPalFromBMP
 Laduje palete z pliku BMP 8 bitowego
 Jesli ok 1 : 0
########################################
*/

int Vid_LoadPalFromBMP (TCF *cf,const char *fn)
{
 int i,r;
 unsigned char *wsk;
 unsigned char pom;
 if (CF_Curr (cf,fn)==0) return 0;
 if (CF_CSeek(cf,54)==0) return 0;
 if (vid_pal==0) vid_pal=Mem_SAlloc(768);
 if (vid_pal==0) return 0;
 wsk=vid_pal;
 for (i=0; i<256; i++)
 {
  CF_CRead(cf,wsk,0,3);
  CF_CRead(cf,&r,0,1);
  pom=*wsk>>2;
  *wsk=*(wsk+2)>>2;
  *(wsk+1)=*(wsk+1)>>2;
  *(wsk+2)=pom;
  wsk+=3;
 }
 return 1;
}

/*
########################################
 Vid_SetBrightness
 Ustawia jasnosc ekranu.
 Jasnosc powinna byc w przedziale -63..63, musi tez byc zainicjowana paleta
 Jesli ok 1 : 0
########################################
*/

int Vid_SetBrightness (int b)
{
 if (vid_pal==0) return 0;
 unsigned char palbuf[768];
 unsigned char *wsk=palbuf;
 int col;
 int i;
 for (i=0; i<256*3; i++)
 {
  col=(*(vid_pal+i))+b;
  if (col<0) col=0;
  else
  if (col>63) col=63;
  *wsk=col;
  wsk++;
 }
 SetPalette(&palbuf);
 return 1;
}

/*
########################################
 Vid_FindRGB
 Szuka koloru w palecie ktory jest najbardziej zblizony do podanego w RGB
 Zwraca numer koloru w palecie lub -1
########################################
*/

int Vid_FindRGB(int r,int g,int b)
{
 if (vid_pal==0) return -1;
 int bestc=0,i,j=0;
 int col,mincol=0x7fffffff;
 r>>=2;
 g>>=2;
 b>>=2;
 for (i=0; i<256; i++,j+=3)
 {
  col=abs(r-(int)*(vid_pal+j))+abs(g-(int)*(vid_pal+j+1))+abs(b-(int)*(vid_pal+j+2));
  if (col<mincol) {mincol=col; bestc=i;}
 }
 return bestc;
}

/*
########################################
 Vid_InitFadeTab
 Inicjuje tablice fade
 Jesli ok 1 : 0
########################################
*/

int Vid_InitFadeTab(int shf)
{
 FADE8Tab=Mem_SAlloc ((1<<shf)*256);
 if (FADE8Tab==0) return 0;
 InitFADE8Tab(shf,vid_pal);
 return 1;
}

/*
########################################
 Vid_InitTranspTab
 Inicjuje tablice transp
 Jesli ok 1 : 0
########################################
*/

int Vid_InitTranspTab(int shf)
{
 TRANSP8Tab=Mem_SAlloc ((1<<shf)*256*256);
 if (TRANSP8Tab==0) return 0;
 InitTRANSP8Tab(shf,vid_pal);
 return 1;
}

/*
########################################
 Vid_PrintScr
 Zapisuje zrzut ekranu do pliku bmp
 Jesli ok 1 : 0
########################################
*/

int Vid_PrintScr()
{
 FILE *f;
 char name[1024];
 char bufBM[]="BM";
 int s1=vid_bsize+1024+54;
 int zero=0,_1024_54=1024+54,_40=40,_1=1,_8=8;
 int i;
 unsigned char *wsk,r,g,b;
 struct date d;
 struct time t;
 getdate(&d);
 gettime(&t);
 sprintf(name,"screenshoot(%04d-%02d-%02d %02d-%02d-%02d).bmp",d.da_year,d.da_mon,d.da_day,t.ti_hour,t.ti_min,t.ti_sec);
 f=fopen(name,"wb");
 fwrite (&bufBM,2,1,f);                         //2
 fwrite (&s1,4,1,f);                            //6
 fwrite (&zero,4,1,f);                          //10
 fwrite (&_1024_54,4,1,f);                      //14
 fwrite (&_40,4,1,f);                           //18
 fwrite (&vid_buf.swidth,4,1,f);                //22
 fwrite (&vid_buf.sheight,4,1,f);               //26
 fwrite (&_1,2,1,f);                            //28
 fwrite (&_8,2,1,f);                            //30
 fwrite (&zero,4,1,f);                          //34
 fwrite (&vid_bsize,4,1,f);                     //38
 fwrite (&zero,4,1,f);                          //42
 fwrite (&zero,4,1,f);                          //46
 fwrite (&zero,4,1,f);                          //50
 fwrite (&zero,4,1,f);                          //54
 wsk=vid_pal;
 for (i=0; i<256; i++)
 {
  r=(*wsk++)<<2;
  g=(*wsk++)<<2;
  b=(*wsk++)<<2;
  fwrite (&b,1,1,f);
  fwrite (&g,1,1,f);
  fwrite (&r,1,1,f);
  fwrite (&zero,1,1,f);
 }
 wsk=vid_buf.sbits+vid_bsize-vid_buf.swidth;
 for (i=0; i<vid_buf.sheight; i++)
 {
  fwrite(wsk,1,vid_buf.swidth,f);
  wsk-=vid_buf.swidth;
 }
 fclose(f);
 vid_scrshootn++;
}

#endif
