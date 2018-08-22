#ifndef __VB_SND_C
#define __VB_SND_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_SND.C
 Opis: Obsluga dzwieku.
------------------------------------------------
*/

//==========================================

#include "vb_snd.h"

//==========================================

//porty sb
unsigned short          snd_sbport2x0;
unsigned short          snd_sbport2x6;
unsigned short          snd_sbport2xa;
unsigned short          snd_sbport2xc;
unsigned short          snd_sbport2xe;
extern unsigned short   snd_sbportexit;

unsigned short snd_sbver;       //wersja sb

unsigned char snd_dma;          //uzywany kanal dma
unsigned char snd_irq;          //uzywane przerwanie
unsigned char snd_irqmask;      //maska przerwan do zapamietania

float snd_mastervol;            //glowna glosnosc dzwieku

extern unsigned int snd_freq;           //czestotliwosc dzwieku
extern unsigned char snd_bytesize;      //liczba bitow(rozdzielczosc dzwieku):1-8bit./2-16bit.
extern unsigned char snd_stereo;        //0-mono,1-stereo

extern unsigned int snd_nchannels;      //liczba uzywanych kanalow
extern TChannels snd_channels;          //kanaly

extern void *snd_buf;                   //bufor
extern unsigned char snd_bufpage;

extern void __Snd_Int_Func();           //funkcja przerwania
__dpmi_paddr __snd_old_int_vec; //poprzednia funkcja przerwania

int snd_initialized;

//rejesty I/O DMA
unsigned short snd_dma_channelac[2]={0x0,0xc0}; //rej. adresowe i licznika(pocz. wart)
unsigned short snd_dma_mask[2]={0xa,0xd4};      //rej. maski kanalu
unsigned short snd_dma_mode[2]={0xb,0xd6};      //rej. trybu
unsigned short snd_dma_cbp[2]={0xc,0xd8};       //rej. przerzutnika
unsigned short snd_dma_page[8]={0x87,0x83,0x81,0x82,0x8f,0x8b,0x89,0x8a}; //rej. strony

//==========================================

/*
########################################
 InitSnd
 Inicjuje dzwiek
 sbport-podstawowy port sb (np.0x220)
 dma-kanal dma (np.1) - dla 0..3 dzwiek 8-bitowy, dla 5..7 dzwiek 16-bitowy
 irq-przerwanie (np.7)
 freq-czestotliwosc dzwieku (np.11025)
 stereo-czy dzwiek mono(0) czy stereo
 nchannels-liczba kanalow dzwiekowych
 Jesli ok 1 : 0
########################################
*/

int InitSnd (int sbport,int dma,int irq,int freq,int stereo,int nchannels)
{
 //inicjacja karty
 snd_sbport2x0=sbport;
 snd_sbport2x6=sbport+0x6;
 snd_sbport2xa=sbport+0xa;
 snd_sbport2xc=sbport+0xc;
 snd_sbport2xe=sbport+0xe;
 if (!Snd_ResetSB()) return 0;
 printf ("sb reset\n");
 //alokacja pamieci na bufor
 void *pt;
 if (snd_buf==0) snd_buf=(void*)(__dpmi_allocate_dos_memory(SND_BUFSIZE>>4,(int*)&pt)*16);
 printf ("alloc dosmem\n");
 if (snd_buf==0) return 0;
 if (dma>3)
 _fill0((void*)((int)snd_buf),SND_BUFSIZE,0x80008000);  //16-bit
 else
 _fill0((void*)((int)snd_buf),SND_BUFSIZE,0x80808080);  //8-bit
 if (Mem_DAlloc((void*)&snd_channels,nchannels*sizeof (TChannel))==0) return 0;
 //inicjuj snd_channels
 memset (snd_channels,0,nchannels*sizeof (TChannel));
 snd_nchannels=nchannels;
 
 //pobierz wersje karty sb (musi byc >= 4.0)
 snd_sbver=Snd_GetSBVer();
 printf ("sb ver\n");
 if ((snd_sbver>>8)<4) return 0;
 
 //inicjacja przerwania
 __dpmi_get_protected_mode_interrupt_vector (irq+8,&__snd_old_int_vec);
 __dpmi_paddr pa;
 pa.offset32=(int)&__Snd_Int_Func;
 pa.selector=_go32_my_cs();
 if (__dpmi_set_protected_mode_interrupt_vector(irq+8,&pa)==-1) return 0;
 snd_irqmask=inportb(0x21);
 outportb(0x21,snd_irqmask&(~(1<<irq)));        //uruchom przerwanie
 snd_irq=irq;
 printf ("set irq %x,%x,%x,%x\n",snd_irqmask,snd_irqmask&(~(1<<irq)),pa.offset32,pa.selector);

 //inicjacja kanalu dma
 int dmat=(dma>3) ? 1 : 0;
 snd_sbportexit=snd_sbport2xe+dmat;             //port wyjsciowy z f. przerwania
 //blokuj kanal
 outportb(snd_dma_mask[dmat],4+(dma&3));
 printf ("start dma init\n");
 //ustelenie trybu przesylu:tryb 'S',inc,auto,mem->sb
 outportb(snd_dma_mode[dmat],0x58+(dma&3));
 printf ("dma mode\n");
 //ustalenie adresu lin
 unsigned int linaddr=((int)snd_buf);
 outportb(snd_dma_cbp[dmat],0);
 outportb(snd_dma_channelac[dmat]+(dma&3)*(dmat+1)*2,linaddr>>dmat);
 outportb(snd_dma_channelac[dmat]+(dma&3)*(dmat+1)*2,linaddr>>8>>dmat);
 printf ("dma addr\n");
 //ustalenie strony
 unsigned char page=(int)snd_buf>>16;
 outportb(snd_dma_page[dma],page);
 printf ("dma pg\n");
 //ustalenie dlugosc bufora: 8-bit: w bajtach / 16-bit: w slowach
 outportb(snd_dma_cbp[dmat],0);
 outportb(snd_dma_channelac[dmat]+(dma&3)*(dmat+1)*2+dmat+1,(SND_BUFSIZE>>dmat)-1);
 outportb(snd_dma_channelac[dmat]+(dma&3)*(dmat+1)*2+dmat+1,(SND_BUFSIZE>>8>>dmat)-1);
 printf ("dma cnt\n");
 //odblokuj kanal
 outportb(snd_dma_mask[dmat],dma&3);
 printf ("dma init ok\n");
 snd_dma=dma;

 //inicjacja sb
 Snd_OutSB(0x41);                       //czestotliwosc dzwieku
 Snd_OutSB(freq>>8);
 Snd_OutSB(freq&255);
 if (dmat)                              //dzwiek 16-bitowy
 Snd_OutSB(0xb6);                       //komenda:output,auto-initialized,digitized sound
 else                                   //dzwiek 8-bitowy
 Snd_OutSB(0xc6);                       //komenda:output,auto-initialized,digitized sound
 if (stereo)
 Snd_OutSB(0x30);                       //tryb:stereo
 else
 Snd_OutSB(0x10);                       //tryb:mono
 Snd_OutSB((unsigned char)(SND_BUFSIZE/2-1));            //rozmiar
 Snd_OutSB((unsigned char)((SND_BUFSIZE/2-1)>>8));
 printf ("SB init\n");
 Snd_OutSB(0xd1);                       //wlacz dzwiek
 snd_stereo=(stereo>0);
 printf ("end\n");

 snd_initialized=1;
 snd_mastervol=1.0;
 return 1;
}

/*
########################################
 CloseSnd
 Zamyka odtwarzanie dzwieku
########################################
*/

void CloseSnd()
{
 if (snd_initialized!=1) return;
 if (snd_dma<4)
 Snd_OutSB(0xda);
 else
 Snd_OutSB(0xd9);
 __dpmi_set_protected_mode_interrupt_vector (snd_irq+8,&__snd_old_int_vec);
 outportb(0x21,snd_irqmask);                    //przywroc przerwanie
 Mem_DFree((void*)&snd_channels);
 snd_initialized=0;
}

/*
########################################
 Snd_ResetSB
 Resetuje karte sb
 Jesli karta istnieje to 1 : 0
########################################
*/

int Snd_ResetSB()
{
 int i;
 outportb(snd_sbport2x6,1);
 for (i=0;i<65536;i++);
 outportb(snd_sbport2x6,0);
 for (i=0;i<65536;i++)
 {
  if (inportb(snd_sbport2xe)&128==128)
     if (inportb(snd_sbport2xa)==0xaa) break;
 }
 return !(i==65536);
}

/*
########################################
 Snd_OutSB
 Zapisuje do karty sb jakas wartosc
########################################
*/
void Snd_OutSB(unsigned char v)
{
 int i=0;
 while (((inportb(snd_sbport2xc)&128)==128)&&(i<65536)) i++;
 outportb(snd_sbport2xc,v);
}

/*
########################################
 Snd_InSB
 Odczytuje z karty sb jakas wartosc i ja zwraca
########################################
*/

unsigned char Snd_InSB()
{
 int i=0;
 while (((inportb(snd_sbport2xe)&128)==0)&&(i<65536)) i++;
 return inportb(snd_sbport2xa);
}

/*
########################################
 Snd_GetSBVer
 Zwraca wersje sb
########################################
*/

unsigned short Snd_GetSBVer()
{
 unsigned char l,h;
 Snd_OutSB(0xe1);
 l=Snd_InSB();
 h=Snd_InSB();
 return (h<<8)+l;
}

/*
########################################
 Snd_LoadWAVE
 Laduje plik dzwiekowy WAVE
 Jesli ok 1 : 0
########################################
*/

int Snd_LoadWAVE(TCF *cf,char *fn,TSnd *snd)
{
 if (CF_Curr(cf,fn)==0) return 0;
 char txt[4];
 unsigned int pom;
 CF_CRead(cf,&txt,0,4);
 if ((txt[0]!='R')||(txt[1]!='I')||(txt[2]!='F')||(txt[3]!='F')) return 0;
 CF_CRead(cf,&txt,0x4,4);
 if ((txt[0]!='W')||(txt[1]!='A')||(txt[2]!='V')||(txt[3]!='E')) return 0;
 unsigned int seek1;
 CF_CRead(cf,&seek1,0x4,4);
 unsigned short stype;
 CF_CRead(cf,&stype,0,2);
 if (stype!=1) return 0;
 CF_CRead(cf,&pom,0,2);
 snd->stereo=(pom>1);
 CF_CRead(cf,&snd->freq,0,4);
 CF_CRead(cf,&pom,6,2);
 snd->bytesize=pom>>3;
 CF_CSeek(cf,0x14+seek1);
 CF_CRead(cf,&txt,0,4);
 if (txt[0]=='f')                       //sekcja 'fact' nie zawsze wystepuje
 {
  int seek2;
  CF_CRead(cf,&seek2,0,4);
  CF_CSeek(cf,seek1+0x14+seek2+4+4);
 }
 CF_CRead(cf,&snd->size,0,4);           //ilosc bajtow
 //wyrownaj dla wolnego pola
 int loadsize=snd->size;
 if ((snd->size%SND_BUFSIZE)!=0) {snd->size=(snd->size/SND_BUFSIZE)*SND_BUFSIZE+SND_BUFSIZE;}
 printf ("%d,%d",snd->size,loadsize);
 //alokacja statyczna
 if (Mem_DAlloc(&snd->bits,snd->size)==0) return 0;
 CF_CRead(cf,snd->bits,0,loadsize);     //wczytuj bajty
 memset(snd->bits+loadsize,0x80808080,snd->size-loadsize);
 return 1;
}

/*
########################################
 Snd_FreeSnd
 Zwalnia pamiec z pliku dzwiekowego
 Jesli ok 1 : 0
########################################
*/

int Snd_FreeSnd(TSnd *snd)
{
 return Mem_DFree(&snd->bits);
}

/*
########################################
 Snd_Play
 Odgrywa dany dzwiek o glosnosci volume, balansie bal i identyfikatorze dzwieku snum
########################################
*/

void Snd_Play(TSnd *snd,float volume,float bal,int snum)
{
 int i;
 int vol=volume*snd_mastervol*(float)(1<<SND_VOLUMEBITS);
 if (vol==0) return;
 for (i=0;i<snd_nchannels;i++)
 {
  if ((snd_channels[i].ison==0))               //odegraj dzwiek
  {
   snd_channels[i].sndref=snd;
   snd_channels[i].pos=0;
   snd_channels[i].volume=vol;
   snd_channels[i].bal=0;                       //<-bal!
   snd_channels[i].sndnum=snum;
   snd_channels[i].ison=1;
   return;
  }
 }
}

/*
########################################
 Snd_Conv4Freq
 Konwertuje dany dzwiek do podanej czestotliwosci
 Zmiejszenie czestotliwosci powoduje utrate danych zapisanych w pamieci
 Jesli ok nowy rozmiar dzwieku : 0
########################################
*/

int Snd_Conv4Freq(TSnd *snd,unsigned int freq)
{
 if (snd==NULL) return 0;
 if (freq>100000) return 0;
 float dx=1/(float)freq;
 float x=0;
 int stereo=(snd->stereo>1)? 2 : 1;
 float t=(snd->size/snd->bytesize/stereo)/(float)snd->freq;
 int spos;
 int i=0;
 int newsize=t*freq*snd->bytesize*stereo;
 void *sndmem;
 if (Mem_DAlloc(&sndmem,newsize)==0) return 0;
 while (x<t)
 {
  if (snd->bytesize==1)         //8-bit
  {
   char sbyte1,sbyte2;
   spos=floor (x*snd->freq);
   sbyte1=128-*(char *)((int)snd->bits+spos);
   sbyte2=128-*(char *)((int)snd->bits+spos+1);
   *(char *)(sndmem+i)=128+(float)(sbyte2-sbyte1)*(x*(float)snd->freq-(float)spos)+sbyte1;
  }
  else                          //16-bit
  {
  }
  x+=dx;
  i++;
 }
 Mem_DFree(&snd->bits);
 if (Mem_DAlloc(&snd->bits,newsize)==0) {Mem_DFree(&sndmem); return 0;}
 memcpy(snd->bits,sndmem,newsize);
 Mem_DFree(&sndmem);
 snd->size=newsize;
 snd->freq=freq;
 return newsize;
}

#endif
