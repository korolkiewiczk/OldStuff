#ifndef _MIXER_H_
#define _MIXER_H_
//mixer.h
//Sound mixing

#define MIXER_ACTIVE 1
#define MIXER_MODIFY 2
#define MIXER_REPEAT 4

#define MIXER_FIXEDSHIFT 10

#define MIXER_MAXCHANNELS 256


typedef struct _SOUNDCHANNEL
{
 void *bufptr; //0
 int offset;   //1
 int size;     //2
 int flags;    //3  &MIXER_ACTIVE==0-not used
 float vol;    //4
 float pan;    //5
} SOUNDCHANNEL;

#ifdef __cplusplus
extern "C"
{
#endif       
 void MixSoundChannels(SOUNDCHANNEL *ctab,int n,int bps,int channels,
                       void *sndbuf,int sndbufsize);
#ifdef __cplusplus
}
#endif

#endif
