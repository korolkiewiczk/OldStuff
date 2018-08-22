#ifndef _WINGAPH_H_
#define _WINGAPH_H_
/*
		WINGRAPH by Kamil Korolkiewicz 2007
		Easy using the graphics in win a'la dos-bios.
*/

//type

typedef struct _ModeInfoList
{
	int w,h,bpp;
	struct _ModeInfoList *next;
} ModeInfoList;

//extern

#ifdef __cplusplus
extern "C"
#endif
{
	int InitWinGraph();
	void EnumVidModes(ModeInfoList **);
	int SetVidMode(int,int,int,int);
	void PresentBuf();
	void ClearBuf(int);
	void CloseApp();
	int WinControl();
	void Cursor(int);
	void GetCurPos(int &x,int &y);
	void SetCurPos(int x,int y);
	void SetCursorClip(int,int,int,int);
	int KeyPressed(int);
	int GetKey(int);
	int MouseKeyPressed(int);
	int GetMouseWheelDelta();;
	//asm
	void _memset8(void*,int,int);
	void _memset16(void*,int,int);
	void _memset32(void*,int,int);
	void _memcpy8buf16(void*,void*,void*,int);
#ifdef __cplusplus
}
#endif

extern HINSTANCE wingraph_hinst;
extern HWND wingraph_wnd;

extern LPDIRECT3D8 wingraph_id3d;
extern LPDIRECT3DDEVICE8 wingraph_id3ddev;
extern LPDIRECT3DSURFACE8 wingraph_id3dbb;

extern int pitch;
extern void *buf;
extern int wingraph_scrwidth,wingraph_scrheight,wingraph_scrbpp,wingraph_active;
extern D3DPRESENT_PARAMETERS wingraph_d3dpp;
extern RECT wingraph_clipcursorrect;
extern int wingraph_clipcursor;
extern unsigned short wingraph_palette[256];
extern unsigned char wingraph_keytab[256];
extern unsigned char wingraph_mousekeystate[3];
extern int wingraph_mousewheeldelta;
extern void (*WinCommandFunc)(int);
extern void (*WinSizeFunc)(int,int,int);

//const

#define WINGRAPH_VSYNC 1
#define WINGRAPH_WINDOWED 2
#define WINGRAPH_OVERLAPPEDWINDOW 4

#define WINGRAPH_MLEFT 0
#define WINGRAPH_MMID 1
#define WINGRAPH_MRIGHT 2

//macros

#define SetBuf8(x,y,c) (*(unsigned byte*)((int)buf+(y)*pitch+(x))=c)
#define SetBuf16(x,y,c) (*(unsigned short*)((int)buf+(y)*pitch+((x)<<1))=c)
#define SetBuf32(x,y,c) (*(unsigned int*)((int)buf+(y)*pitch+((x)<<2))=c)
#define RGB16(r,g,b) (unsigned short)((((r)&255)>>3)<<11)|((((g)&255)>>2)<<5)|(((b)&255)>>3)
#define RGB32(r,g,b) (unsigned int)(((r)&255)<<16)|(((g)&255)<<8)|((b)&255)
#define SetPaletteColor(c,r,g,b) wingraph_palette[c]=RGB16(r,g,b)
#define GetScrWidth() wingraph_scrwidth
#define GetScrHeight() wingraph_scrheight
#define GetBPP() wingraph_bpp

#endif
