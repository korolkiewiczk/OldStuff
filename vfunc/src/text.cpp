#include <gl/gl.h>
#include <windows.h>
#include "text.h"

extern HWND hwnd;
CText::CText()
{
	Init();
}

void CText::Init()
{
	HDC hdc=GetDC(hwnd);
	SelectObject(hdc,GetStockObject(SYSTEM_FONT));
  	wglUseFontBitmaps(hdc,0,255,1);
}

void CText::Draw(float x,float y,int color,char *s,int is3d,float z)
{
	if (is3d==0)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
	}
	glRasterPos3f(x,y,z);
	glColor3ub((color>>16)&0xff,(color>>8)&0xff,color&0xff);
	glListBase(1);
	glCallLists(strlen(s),GL_UNSIGNED_BYTE,s);

	if (is3d==0)
	{
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}
