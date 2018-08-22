#ifndef _WINSOUND_H_
#define _WINSOUND_H_
//winsound.h
//Biblioteka obslugujaca bufor dzwiekowy pod win32


#ifdef __cplusplus
extern "C"
#endif
{
	int InitWinSound(HWND);
	int SetSoundBuffer(int,int,int);
	void CloseWinSound();
	void BeginSndBufAccess();
	void EndSndBufAccess();
	int GetSoundPos();
	int GetWSoundPos(int,int*);
	void PauseSound();
	void ResumeSound();
	
	int InitWinSoundInput();
	int SetInputSoundBuffer(int freq,int bps,int channels);
	void CloseWinSoundInput();
	void BeginInputSndBufAccess();
	void EndInputSndBufAccess();
	int GetInputSoundPos();
	int GetRSoundPos(int p,int *bs);
	int GetInputSoundPosDiff(int *s);
#ifdef __cplusplus
}
#endif

//wskaznik do bufora dzwiekowego
extern void *sndbuf;
//rozmiar danych dzwiekowych
extern int sndbufsize;

//wskaznik do wejsciowego bufora dzwiekowego
extern void *isndbuf;
//rozmiar wejsciowego bufora dzwiekowego
extern int isndbufsize;

extern int winsound_freq,winsound_bps,winsound_channels;

#endif
