#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <cstdio>
#include <cmath>
#include "common.h"

#define DELTAPOS 0.01
#define DELTAPOST 0.5
#define WINDOWTITLE "VFUNC 0.1"

const int width=640;
const int height=480;
const double MINPROJECTZ=0.01;
const double vangle=90.;

int keytab[256];

#include "ff.h"
CFF *ff;
#include "engine.h"
CEngine *engine;

#include "keymap.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
HWND hwnd;
HDC hdc;
HGLRC hrc;
int active=1;


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    MSG msg;
    BOOL bQuit = FALSE;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(100));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "vfunc";
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(100));


    if (!RegisterClassEx(&wcex))
        return 0;

	RECT rect;
	SetRect(&rect,0,0,width,height);
	AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,0);

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "vfunc",
                          WINDOWTITLE,
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          rect.right-rect.left,
                          rect.bottom-rect.top,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hdc, &hrc);
    int pcix,pciy;
    GetCurPos(width,height,pcix,pciy);

	if (lpCmdLine[0]!=0)
	{
		if (lpCmdLine[0]=='"') lpCmdLine++;
		if (lpCmdLine[strlen(lpCmdLine)-1]=='"')lpCmdLine[strlen(lpCmdLine)-1]=0;
		ff=new CFF(lpCmdLine);
		int i=strlen(lpCmdLine)-1;
		while ((i>0)&&(lpCmdLine[i]!='.')) i--;
		lpCmdLine[i]=0;
		while ((i>0)&&(lpCmdLine[i]!='\\')) i--;
		char buf[256];
		strcpy(buf,WINDOWTITLE);
		strcat(buf," [");
		strcat(buf,lpCmdLine+i+(i>0));
		strcat(buf,"]");
		SetWindowText(hwnd,buf);
	}
	else
    ff=new CFF("default.txt");
    engine=new CEngine;
    //printf("cmdline %s",lpCmdLine);

    /* program main loop */
    while (!bQuit)
    {
    	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		if (active==0) WaitMessage();
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

		int shift=1;
		if (keytab[K_LSHF]) {shift=4;}
        if (keytab[K_ESC]) break;
        if (keytab[K_ENTER]) {ff->Update(); keytab[K_ENTER]=0;}
        if (keytab[K_TAB]) {ff->NextPolyMode(); keytab[K_TAB]=0;}
        if (keytab[K_F]) {ff->NextShadeModel(); keytab[K_F]=0;} //f
        if (keytab[K_I]) {engine->ShowInfo(); keytab[K_I]=0;} //i
        if (keytab[K_B]) {engine->ToogleBlend(); keytab[K_B]=0;} //i
        if (keytab[K_J]) engine->UpdateLightVec(); //j
        if (keytab[K_HOME]) {engine->Defaults(); keytab[K_HOME]=0;}
        if (keytab[K_UP]) engine->Translate(0,0,-DELTAPOST*shift);
		if (keytab[K_DOWN]) engine->Translate(0,0,DELTAPOST*shift);
		if (keytab[K_LEFT]) engine->Translate(-DELTAPOST*shift,0,0);
		if (keytab[K_RIGHT]) engine->Translate(DELTAPOST*shift,0,0);
		if (keytab[K_PGUP]) engine->Translate(0,DELTAPOST*shift,0);
		if (keytab[K_PGDOWN]) engine->Translate(0,-DELTAPOST*shift,0);
		if (keytab[K_F1])
		{
			MessageBox(hwnd,"VFUNC v 0.1 by Kamil Korolkiewicz 2009","About",MB_OK|MB_ICONQUESTION);
			keytab[K_F1]=0;
		}
		//POINT p;
		//GetCursorPos(&p);
		int cix,ciy;
		GetCurPos(width,height,cix,ciy);
		double cx=0,cy=0;
		if ((cix!=pcix)||(ciy!=pciy))
		{
			cx=(cix-pcix)/(double)width*2*3.1415;
			cy=(ciy-pciy)/(double)height*2*3.1415;
			SetCurPos(width,height,width/2,height/2);
		}
		//pcix=width/2;
		//pciy=height/2;
		GetCurPos(width,height,pcix,pciy);
		engine->Rotate(cx,0,cy);

		if (keytab[254])
		{
			engine->ChangePos(DELTAPOS*shift);
		}
		if (keytab[255])
		{
			engine->ChangePos(-DELTAPOS*shift);
		}

		engine->SetLight();
		ff->Draw();
		engine->UpdateCamera();
		engine->DrawInfo();
		if (keytab[K_F5]) {ScrShot(); keytab[K_F5]=0;}

		SwapBuffers(hdc);
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hdc, hrc);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return 1;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

		case WM_KEYDOWN: keytab[MapVirtualKey(wParam,0)]=1; break;
  		case WM_KEYUP: keytab[MapVirtualKey(wParam,0)]=0; break;

  		case WM_LBUTTONDOWN: keytab[254]=1; break;
  		case WM_LBUTTONUP: keytab[254]=0; break;

  		case WM_RBUTTONDOWN: keytab[255]=1; break;
  		case WM_RBUTTONUP: keytab[255]=0; break;

  		case WM_ACTIVATEAPP:

		   if (wParam==0)
		   {
			active=0;
		   }
		   else
		   {
			active=1;
		   }
       break;
       case WM_SIZE:
      	DisableOpenGL(hwnd, hdc, hrc);
       	EnableOpenGL(hwnd, &hdc, &hrc);
       	CText::Init();
       	break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hdc, HGLRC* hrc)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hdc = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hdc, &pfd);

    SetPixelFormat(*hdc, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hrc = wglCreateContext(*hdc);

    wglMakeCurrent(*hdc, *hrc);

    glMatrixMode(GL_PROJECTION);
 	glLoadIdentity();
 	/*glFrustum(-MINPROJECTZ*tan(vangle/2),
 	MINPROJECTZ*tan(vangle/2),
 	MINPROJECTZ*tan(vangle/2)*(double)height/(double)width,
 	-MINPROJECTZ*tan(vangle/2)*(double)height/(double)width,MINPROJECTZ,10000);*/
 	gluPerspective(vangle,(float)width/(float)height,MINPROJECTZ,1000);
 	glMatrixMode(GL_MODELVIEW);
 	glEnable(GL_DEPTH_TEST);
}

void DisableOpenGL (HWND hwnd, HDC hdc, HGLRC hrc)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hrc);
    ReleaseDC(hwnd, hdc);
}

