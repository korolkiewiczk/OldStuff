#include "common.h"
#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>

extern HWND hwnd;

double GetTime()
{
	long long f,t;
 	QueryPerformanceFrequency((PLARGE_INTEGER)&f);
 	QueryPerformanceCounter((PLARGE_INTEGER)&t);
 	return (double)t/(double)f;
}

//todo: width i height nie s¹ zgodne z w³aœciwym rozmiarem wyœwietlanego pola
void ScrShot()
{
	int vdims[4];
	glGetIntegerv(GL_VIEWPORT,(GLint*)&vdims);
	int width=vdims[2];
	int height=vdims[3];
	unsigned int *buf=new unsigned int [width*height];
	FILE *f;
	glFlush();
	#define GL_BGR                                  0x80E0
 	glReadPixels(0,0,width,height,GL_BGR,GL_UNSIGNED_BYTE,(GLvoid*)buf);
	char bufBM[]="BM";
	int s=width*height*3;
	int s1=s+54;
	int zero=0,_54=54,_40=40,_1=1,bpp=24;
	//int i,j;
	//unsigned int c;
	char fn[256];
	float t=(float)(GetTime()*1000);
	sprintf(fn,"IMG%0.0f.bmp",t);
	f=fopen(fn,"wb");
	if (!f) return;

	fwrite (&bufBM,2,1,f);                         //2
	fwrite (&s1,4,1,f);                            //6
	fwrite (&zero,4,1,f);                          //10
	fwrite (&_54,4,1,f);                           //14
	fwrite (&_40,4,1,f);                           //18
	fwrite (&width,4,1,f);                         //22
	fwrite (&height,4,1,f);                        //26
	fwrite (&_1,2,1,f);                            //28
	fwrite (&bpp,2,1,f);                           //30
	fwrite (&zero,4,1,f);                          //34
	fwrite (&s,4,1,f);                             //38
	fwrite (&zero,4,1,f);                          //42
	fwrite (&zero,4,1,f);                          //46
	fwrite (&zero,4,1,f);                          //50
	fwrite (&zero,4,1,f);                          //54
	/*unsigned int *wsk;
	wsk=(buf+width*(height-1));
	for (i=0; i<height; i++)
	{
		for (j=0; j<width; j++)
		{
			c=wsk[j];
			fwrite(&c,3,1,f);
		}
		wsk-=width;
	}*/
	fwrite(buf,width*height*3,1,f);
	fclose(f);
	delete []buf;
}

/*
  Returns relative cursor position
*/
void GetCurPos(int width,int height,int &x,int &y)
{
 POINT p;
 RECT r1,r2;
 r2.left=r2.top=0;
 GetCursorPos(&p);
 {
  GetWindowRect(hwnd,&r1);
  AdjustWindowRect(&r2,GetWindowLong(hwnd,GWL_STYLE),0);
  x=p.x-r1.left+r2.left;
  y=p.y-r1.top+r2.top;
  GetClientRect(hwnd,&r1);
  if ((r1.right!=width)&&(r1.right>0))
  {
   x=(int)((float)x*(float)width/(float)(r1.right)+0.5f);
  }
  if ((r1.bottom!=height)&&(r1.bottom>0))
  {
   y=(int)((float)y*(float)width/(float)(r1.bottom)+0.5f);
  }
 }
}

/*
  Sets relative cursor position
*/
void SetCurPos(int width,int height,int x,int y)
{
 {
  RECT r1,r2,r3;
  r2.left=r2.top=0;
  GetWindowRect(hwnd,&r1);
  AdjustWindowRect(&r2,GetWindowLong(hwnd,GWL_STYLE),0);
  GetClientRect(hwnd,&r3);
  if (r3.right!=width)
  {
   x=(int)((float)x*(float)(r3.right)/(float)width+.5f);
  }
  if (r3.bottom!=height)
  {
   y=(int)((float)y*(float)(r3.bottom)/(float)height+0.5f);
  }
  x+=r1.left-r2.left;
  y+=r1.top-r2.top;
 }
 SetCursorPos(x,y);
}
