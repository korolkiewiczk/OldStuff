//s60sound.h

#define S60SOUND_AOSC_MAXCOUNT 1

class S60Sound_AOSC:public MMdaAudioOutputStreamCallback
{
 private:
 CMdaAudioOutputStream* aos;
 int initialized;
 int count;
 TMdaAudioDataSettings settings;
 TPtrC8 sdesc;
 int sfreq,schannels;
 
 public:
 S60Sound_AOSC() {aos=0; initialized=0;}
 ~S60Sound_AOSC() {Close();}
 void MaoscOpenComplete(TInt aError);
 void MaoscBufferCopied(TInt aError,const TDesC8& aBuffer);
 void MaoscPlayComplete(TInt aError); 
 
 int Init();
 void Close();
 int Set(int freq,int channels);
 void Pause();
 void Resume();
 int Count() {return count;}
 int Initialized() {return initialized;}
 int Update(void *buf,int size);
 int GetPos();
};
