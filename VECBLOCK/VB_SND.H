#ifndef __VB_SND_H
#define __VB_SND_H
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_SND.H
 Opis: Obsluga dzwieku - plik naglowkowy.
------------------------------------------------
*/

//==========================================

#define SND_BUFSIZE 1024
#define SND_VOLUMEBITS 8

//==========================================

//struktura przechowujaca dzwiek
typedef struct _TSnd
{
 void *bits;                    //wskaznik do bajtow
 unsigned int size;             //rozmiar dzwieku
 //dane dodatkowe na temat dzwieku
 unsigned int freq;
 unsigned char bytesize;        //1-8bitow/2-16bitow
 unsigned char stereo;          //mono/stereo
} TSnd;

//struktura opisujaca dany kanal dzwiekowy
typedef struct _TChannel
{
 int ison;                      //czy kanal uzywany
 struct _TSnd *sndref;          //wskaznik do dzwieku
 int pos;                       //pozycja w dzwieku
 int volume;                    //glosnosc
 int bal;                       //balans (dla stereo)
 int sndnum;                    //numer okreslajacy obiekt do ktorego nalezy dzwiek
 int islopped;                  //okresla czy dzwiek ma byc powtarzany cyklicznie
} TChannel;

typedef TChannel *TChannels;    //wskaznik na kanaly

//======================================

int InitSnd (int,int,int,int,int,int);
void CloseSnd();
int Snd_ResetSB();
void Snd_OutSB(unsigned char);
unsigned char Snd_InSB();
unsigned short Snd_GetSBVer();
int Snd_LoadWAVE(TCF *,char *,TSnd *);
int Snd_FreeSnd(TSnd *);
void Snd_Play(TSnd *,float,float,int);

#endif
