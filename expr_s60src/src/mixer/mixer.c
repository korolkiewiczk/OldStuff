//mixer.c
//Sound mixing
//BY Kamil Korolkiewicz 09/2008

#include "mixer/mixer.h"

static int _mixersum,_mixersumL,_mixersumR;

//mixer functions

//8BPS, mono
void _startmixfunc_8_mono()
{
 _mixersum=0;
}

void _mixfunc_8_mono(int c[])
{
 _mixersum+=((-128+(int)*(unsigned char*)(c[0]+c[1]))*c[4])>>MIXER_FIXEDSHIFT;
}

void _endmixfunc_8_mono(void *sndbuf,int i)
{
 if (_mixersum>127) _mixersum=127;
 if (_mixersum<-128) _mixersum=-128;
 *(unsigned char*)((int)sndbuf+i)=128+_mixersum;
}

//8BPS, stereo
void _startmixfunc_8_stereo()
{
 _mixersumL=0;
 _mixersumR=0;
}

void _mixfunc_8_stereo(int c[])
{
 int v=((-128+(int)*(unsigned char*)(c[0]+c[1]))*c[4])>>MIXER_FIXEDSHIFT;
 _mixersumL+=(v*c[5])>>MIXER_FIXEDSHIFT;
 _mixersumR+=(v*c[6])>>MIXER_FIXEDSHIFT;
}

void _endmixfunc_8_stereo(void *sndbuf,int i)
{
 if (_mixersumL>127) _mixersumL=127;
 if (_mixersumL<-128) _mixersumL=-128;
 if (_mixersumR>127) _mixersumR=127;
 if (_mixersumR<-128) _mixersumR=-128;
 *(unsigned char*)((int)sndbuf+i)=128+_mixersumL;
 *(unsigned char*)((int)sndbuf+i+1)=128+_mixersumR;
}

//16BPS, mono
void _startmixfunc_16_mono()
{
 _mixersum=0;
}

void _mixfunc_16_mono(int c[])
{
 _mixersum+=(((int)*(short*)(c[0]+c[1]*2))*c[4])>>MIXER_FIXEDSHIFT;
}

void _endmixfunc_16_mono(void *sndbuf,int i)
{
 if (_mixersum>32767) _mixersum=32767;
 if (_mixersum<-32768) _mixersum=-32768;
 *(short*)((int)sndbuf+i)=_mixersum;
}

//16BPS, stereo
void _startmixfunc_16_stereo()
{
 _mixersumL=0;
 _mixersumR=0;
}

void _mixfunc_16_stereo(int c[])
{
 int v=(((int)*(short*)(c[0]+c[1]*2))*c[4])>>MIXER_FIXEDSHIFT;
 _mixersumL+=(v*c[5])>>MIXER_FIXEDSHIFT;
 _mixersumR+=(v*c[6])>>MIXER_FIXEDSHIFT;
}

void _endmixfunc_16_stereo(void *sndbuf,int i)
{
 if (_mixersumL>32767) _mixersumL=32767;
 if (_mixersumL<-32768) _mixersumL=-32768;
 if (_mixersumR>32767) _mixersumR=32767;
 if (_mixersumR<-32768) _mixersumR=-32768;
 *(short*)((int)sndbuf+i)=_mixersumL;
 *(short*)((int)sndbuf+i+2)=_mixersumR;
}

//mixes active sound channels from ctab to sndbuf
//for each channel: bufptr data must no less than sndbufsize
void MixSoundChannels(SOUNDCHANNEL *ctab,int n,int bps,int channels,
                      void *sndbuf,int sndbufsize)
{
 int i,j,k;
 void (*_startmixfunc)();
 void (*_mixfunc)(int []);
 void (*_endmixfunc)(void *,int);

 //choose mixer function 
 if (bps==8)
 {
  if (channels==1)
  {
   _startmixfunc=_startmixfunc_8_mono;
   _mixfunc=_mixfunc_8_mono;
   _endmixfunc=_endmixfunc_8_mono;
  }
  else
  if (channels==2)
  {
   _startmixfunc=_startmixfunc_8_stereo;
   _mixfunc=_mixfunc_8_stereo;
   _endmixfunc=_endmixfunc_8_stereo;
  }
  else return;
 } 
 else
 if (bps==16)
 {
  if (channels==1)
  {
   _startmixfunc=_startmixfunc_16_mono;
   _mixfunc=_mixfunc_16_mono;
   _endmixfunc=_endmixfunc_16_mono;
  }
  else
  if (channels==2)
  {
   _startmixfunc=_startmixfunc_16_stereo;
   _mixfunc=_mixfunc_16_stereo;
   _endmixfunc=_endmixfunc_16_stereo;
  }
  else return;
 }
 else return;
 
 //comp size of buf
 int size=sndbufsize/channels/(bps/8);
 
 //set active channels
 int channelstab[MIXER_MAXCHANNELS][8]; //0ptr,1offset,2size,3flags,4vol,5panL,6panR,7&ctab
 int l=0;
 
 if (n>=MIXER_MAXCHANNELS) n=MIXER_MAXCHANNELS;
 for (j=0; j<n; j++)
 {
  if(((ctab+j)->flags&MIXER_ACTIVE)&&((ctab+j)->offset<(ctab+j)->size))
  {
   channelstab[l][0]=(int)(ctab+j)->bufptr;
   channelstab[l][1]=(ctab+j)->offset;
   channelstab[l][2]=(ctab+j)->size;
   channelstab[l][3]=(ctab+j)->flags;
   channelstab[l][4]=(int)((ctab+j)->vol*(1<<MIXER_FIXEDSHIFT));
   channelstab[l][5]=(int)((1-(ctab+j)->pan)*(1<<MIXER_FIXEDSHIFT)*0.5f);
   channelstab[l][6]=(int)((1+(ctab+j)->pan)*(1<<MIXER_FIXEDSHIFT)*0.5f);
   channelstab[l][7]=(int)(ctab+j);
   l++;
  }
 }
 
 k=0;
 int d=bps*channels/8;
 for (i=0; i<size; i++)
 {
  _startmixfunc();
  for (j=0; j<l; j++)
  {
   _mixfunc(channelstab[j]);
   channelstab[j][1]++;
   if (channelstab[j][1]>=channelstab[j][2])
   {
    if (channelstab[j][3]&MIXER_REPEAT)
    {
     channelstab[j][1]=0;
    }
    else
    {
     if (channelstab[j][3]&MIXER_MODIFY)
     ((SOUNDCHANNEL*)channelstab[j][7])->offset=channelstab[j][1];
     memcpy(channelstab[j],channelstab[l-1],sizeof(channelstab[0]));
     l--;
    }
   }
  }
  _endmixfunc(sndbuf,k);
  k+=d;
 }
 
 //check for modify flag
 for (j=0; j<l; j++)
 {
  if (channelstab[j][3]&MIXER_MODIFY)
  ((SOUNDCHANNEL*)channelstab[j][7])->offset=channelstab[j][1];
 }
 
}
