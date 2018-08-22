#include "vbengine.h"

TSnd snd;
TCF cf;

int main()
{
 int i;
 float vol=1.0;
 _xstub32init();
 InitMem(65536*5,65536*8);
 if (Snd_LoadWAVE(&cf,"../s.wav",&snd)==0) return 0;
 printf("snd load ok:%d,%d,%d,%d,%d\n",snd.bits,snd.size,snd.freq,snd.bytesize,snd.stereo);
 i=Snd_Conv4Freq(&snd,11025);
 printf("snd load ok:%d,%d,%d,%d\n",snd.bits,snd.size,snd.freq,i);
 if (InitSnd(0x220,1,7,11025,0,16)==0) return 0;
 Snd_Play(&snd,1.0,0,0);
 printf("snd init ok:%d\n",snd_nchannels);
 while (1)
 {
  char c=MReadKey();
  if (c==27) break;
  else
  if (c=='a')
  for (i=0;i<snd_nchannels;i++)
  {
   printf("sndref:%d,pos:%d,vol:%d,bal:%d,num:%d,on:%d\n",
   snd_channels[i].sndref,
   snd_channels[i].pos,
   snd_channels[i].volume,
   snd_channels[i].bal,
   snd_channels[i].sndnum,
   snd_channels[i].ison);
  }
  else
  if (c=='s')
  Snd_Play(&snd,vol,0,0);
  else
  if (c=='i')
  {
   CloseSnd();
   InitSnd(0x220,1,7,11025/10,0,4);
   _fill0((void*)((int)snd_buf),SND_BUFSIZE*100,0x12323234);  //8-bit
  }
  else
  if (c=='w') vol*=1.1;
  else
  if (c=='q') vol/=1.1;
  printf ("%d,%f\n",snd_bufpage,vol);
 }
 CloseSnd();
 printf("snd exit ok\n");
 CloseMem();
}
