//s60sound.cpp

#include <MdaAudioOutputStream.h>
#include <Mda\Common\Audio.h>
#include "s60sound.h"
//mediaclientaudiostream.lib req

void S60Sound_AOSC::MaoscOpenComplete(TInt aError)
{
 if (aError!=KErrNone) return;
 initialized=1;
 aos->SetPriority(EMdaPriorityNormal,EMdaPriorityPreferenceNone);
 aos->SetVolume(aos->MaxVolume());
 Set(sfreq,schannels);
}

void S60Sound_AOSC::MaoscBufferCopied(TInt aError,const TDesC8& aBuffer)
{
 count--;
}

void S60Sound_AOSC::MaoscPlayComplete(TInt aError)
{
 count=0;
}
 
int S60Sound_AOSC::Init()
{
 if (aos) return 1;
 initialized=0;
 aos=CMdaAudioOutputStream::NewL(*this);
 if (!aos) return 0;
 aos->Open(&settings);
 count=0;
 sfreq=11025;
 schannels=2;
 return 1;
}

void S60Sound_AOSC::Close()
{
 if (aos)
 {
  aos->Stop();
  delete aos;
  aos=0;
 }
 initialized=0;
}

//zwraca faktyczna czest. probkowania
int S60Sound_AOSC::Set(int freq,int channels)
{
 TInt samplerate,ch;
 if (freq<11025)
 {
  samplerate=TMdaAudioDataSettings::ESampleRate8000Hz;
  freq=8000;
 }
 else
 if (freq<12000)
 {
  samplerate=TMdaAudioDataSettings::ESampleRate11025Hz;
  freq=11025;
 }
 else
 if (freq<16000)
 {
  samplerate=TMdaAudioDataSettings::ESampleRate12000Hz;
  freq=12000;
 }
 else
 if (freq<22050)
 {
  samplerate=TMdaAudioDataSettings::ESampleRate16000Hz;
  freq=16000;
 }
 else
 if (freq<24000)
 {
  samplerate=TMdaAudioDataSettings::ESampleRate22050Hz;
  freq=22050;
 }
 else
 if (freq<32000)
 {
  samplerate=TMdaAudioDataSettings::ESampleRate24000Hz;
  freq=24000;
 }
 else
 if (freq<44100)
 {
  samplerate=TMdaAudioDataSettings::ESampleRate32000Hz;
  freq=32000;
 }
 else
 if (freq<48000)
 {
  samplerate=TMdaAudioDataSettings::ESampleRate44100Hz;
  freq=44100;
 }
 else
 if (freq<64000)
 {
  samplerate=TMdaAudioDataSettings::ESampleRate48000Hz;
  freq=64000;
 }
 else
 if (freq<96000)
 {
  samplerate=TMdaAudioDataSettings::ESampleRate64000Hz;
  freq=64000;
 }
 else
 {
  samplerate=TMdaAudioDataSettings::ESampleRate96000Hz;
  freq=96000;
 }
 if (channels==1) ch=TMdaAudioDataSettings::EChannelsMono;
 else
 ch=TMdaAudioDataSettings::EChannelsStereo;
 if ((aos)&&(initialized))
 aos->SetAudioPropertiesL(samplerate,ch);
 sfreq=freq;
 schannels=channels;
 return freq;
}

void S60Sound_AOSC::Pause()
{
 if ((!aos)||(!initialized)) return;
 aos->SetVolume(0);
}

void S60Sound_AOSC::Resume()
{
 if ((!aos)||(!initialized)) return;
 aos->SetVolume(aos->MaxVolume());
}

int S60Sound_AOSC::Update(void *buf,int size)
{
 if ((!aos)||(!initialized)) return 0;
 sdesc.Set((TUint8*)buf,size);
 count++;
 TRAPD(err,aos->WriteL(sdesc));
 return 1;
}

int S60Sound_AOSC::GetPos()
{
 if ((!aos)||(!initialized)) return 0;
 return aos->GetBytes();
}


//
//INTERFACE
//

S60Sound_AOSC s60sound_aosc;

//wskaznik do bufora dzwiekowego
void *sndbuf;
void *S60Sound_sndbuftab[2];
int S60Sound_sndbufcount=0;
int S60Sound_sndbufpos;
//rozmiar danych dzwiekowych
int sndbufsize;


//sbs okresla rozmiar bufora dzwiekowego
int InitS60Sound(int sbs)
{
 if (S60Sound_sndbuftab[0])
 {
  delete S60Sound_sndbuftab[1];
  delete S60Sound_sndbuftab[0];
 }
 S60Sound_sndbuftab[0]=(void*)new unsigned char [sbs];
 S60Sound_sndbuftab[1]=(void*)new unsigned char [sbs];
 sndbufsize=sbs;
 S60Sound_sndbufpos=0;
 return s60sound_aosc.Init();
}

//ustawia bufor dzwiekowy i go odtwarza w petli
//zwraca faktyczna czestotliwosc probkowania
int SetSoundBuffer(int freq,int /*bps*/,int channels)
{
 //bps=16
 return s60sound_aosc.Set(freq,channels);
}

//zamyka modul dzwiekowy
void CloseS60Sound()
{
 if (S60Sound_sndbuftab[0])
 {
  delete S60Sound_sndbuftab[1];
  delete S60Sound_sndbuftab[0];
  S60Sound_sndbuftab[0]=0;
  S60Sound_sndbuftab[1]=0;
 }
 sndbufsize=0;
 s60sound_aosc.Close();
}

//rozpocznij dostep do bufora dzwiekowego, zwraca do niego wskaznik
void BeginSndBufAccess()
{
 sndbuf=S60Sound_sndbuftab[S60Sound_sndbufcount];
}

//zakoncz dostep do bufora dzwiekowego
void EndSndBufAccess()
{
 if (s60sound_aosc.Update(sndbuf,sndbufsize)==0) return;
 S60Sound_sndbufcount=1-S60Sound_sndbufcount;
 S60Sound_sndbufpos+=sndbufsize;
}

//podaje aktualna pozycje w buforze dzwiekowym
int GetSoundPos()
{
 return s60sound_aosc.GetPos();
}

//jesli bufor jest do zapisu zwraca jego wielkosc, else -1
int GetWSoundPos()
{
 if (s60sound_aosc.Initialized()==0) return -1;
 if (s60sound_aosc.Count()>1) return -1;
 int pos=GetSoundPos();
 if ((S60Sound_sndbufpos-pos>sndbufsize)&&(pos>0))
 return -1;
 if (pos==0) S60Sound_sndbufpos=0;
 return sndbufsize;
}

//wstrzymuje dzwiek
void PauseSound()
{
 s60sound_aosc.Pause();
}

//kontunuuje odtwarzanie dzwieku
void ResumeSound()
{
 s60sound_aosc.Resume();
}
