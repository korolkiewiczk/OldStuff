.386p
.model flat,stdcall
option casemap:none

;includy

include fastwin.inc
include gl.inc
include glu.inc
include glut.inc

includelib c:\masm32\lib\gl.lib
includelib c:\masm32\lib\glu32.lib
includelib c:\masm32\lib\glut32.lib
.listall     


;dane

.data

WindowName db 'Nazwa aplikacji',0
WinMessage MSG <>
CzyZamknac db 'Czy zamknac program?',0
zerof dd 0.0
c1 dd 0.1
f1 dq 130.0
f2 dd 1.0
mf1 dq -130.0

ambl dd 0.5,0.5,0.5,1.0
difl dd 0.5,0.5,0.5,1.0
spel dd 1.0,1.0,1.0,1.0
spml dd 1.0,1.0,1.0,1.0


;kod

.code

kbf proc C,key:dword,x:dword,y:dword
        cmp key,27
        jne pq
        invoke ExitProcess,NULL
pq:
        ret
kbf endp

.data
okat dd 0.0
pz dd 1.0
color dd 0.5
ccs dd 10.0
cyl1 dq 40.0
cyl2 dq 2.0
__90 dd -90.0
__270 dd -270.0
tt dd 0.0
.code

render proc C
        local gqo:dword
        local pom:dword
        invoke gluNewQuadric
        mov gqo,eax
        invoke glClear,GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT
        invoke glColor3f,color,color,color
        invoke glPushMatrix
        invoke glRotatef,__90,pz,zerof,zerof
        invoke glRotatef,okat,pz,pz,pz
        invoke gluQuadricDrawStyle,gqo,GLU_FILL
        invoke gluCylinder,gqo,cyl1,cyl2,cyl1,100,100
        invoke glRotatef,__270,pz,zerof,zerof
        invoke glTranslatef,zerof,tt,zerof
        invoke glTranslatef,zerof,__90,zerof
        invoke glColor3f,color,zerof,zerof
        invoke gluSphere,gqo,cyl2,5,5
        invoke glTranslatef,ccs,ccs,ccs
        invoke glColor3f,zerof,color,zerof
        invoke gluSphere,gqo,cyl2,5,5
        invoke glTranslatef,ccs,ccs,ccs
        invoke glColor3f,zerof,zerof,color
        invoke gluSphere,gqo,cyl2,5,5
        invoke glPopMatrix
        invoke glutSwapBuffers
        invoke gluDeleteQuadric,gqo
        fld tt
        fadd c1
        fst tt
        fistp pom
        cmp pom,140
        jne d1
        fldz
        fstp tt
d1:
        ret
render endp

idle proc C
        fld okat
        fadd f2
        fstp okat
        call render
        ret
idle endp

start:
        WinInit

WinMain proc stdcall,hInstance:dword,hprevInstance:dword,lpCmdLine:dword,nCmdShow:dword

        invoke glutInitDisplayMode,GLUT_DOUBLE
        invoke glutCreateWindow,addr WindowName
        invoke glClearColor,c1,c1,c1,f2
        invoke glMatrixMode,GL_PROJECTION
        invoke glLoadIdentity
        invoke glOrtho,mf1,f1,mf1,f1,mf1,f1
        invoke glMatrixMode,GL_MODELVIEW
        invoke glLoadIdentity
        invoke glEnable,GL_DEPTH_TEST
        invoke glutFullScreen
        invoke glutKeyboardFunc,addr kbf
        invoke glutDisplayFunc,addr render
        invoke glutIdleFunc,addr idle
        invoke glEnable,GL_LIGHT0
        invoke glEnable,GL_LIGHTING
        invoke glLightfv,GL_LIGHT0,GL_AMBIENT,addr ambl
        invoke glLightfv,GL_LIGHT0,GL_DIFFUSE,addr difl
        invoke glLightfv,GL_LIGHT0,GL_SPECULAR,addr spel
        invoke glMaterialfv,GL_FRONT_AND_BACK,GL_SPECULAR,addr spml
        invoke glMaterialf,GL_FRONT_AND_BACK,GL_SHININESS,ccs
        invoke glEnable,GL_COLOR_MATERIAL
        invoke glColorMaterial,GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE
        invoke glutMainLoop

        ret
WinMain endp

end start
