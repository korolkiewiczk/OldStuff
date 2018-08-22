debug=0
gvesa=0
.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;kod

include dpmi.inc
include mstdlib\mcon.inc
include mstdlib\mmem.inc
include surface.inc
include bmp8.inc
include d_data.inc
__T_SIZE=28
if debug eq 0
include d_tex8.inc
else
.586
include d_tex8d.inc
endif
include vga.inc
include vesa.inc
include kb.inc

if gvesa eq 0
ww=320
wh=200
surf TSurface <320,200,1,320,0a0000h>
buf TSurface <320,200,1,320,0>
else
;ww=640
;wh=480
;wm=V640X480X256
ww=1024
wh=768
wm=V1024X768X256
surf TSurface <ww,wh,1,ww,0a0000h>
buf TSurface <ww,wh,1,ww,0>
endif
tex TTex <7,0>
texs TSurface <>
time dd 0
dd 0
pal dd 0
fn db 'tex.bmp',0

triangle dd 0.0       ;a
         dd 0.0
         dd 0
         dd 0
         dd 1
         dd 0.0
         dd 0.0

         dd 0.0        ;b
         dd 199.0
         dd 0
         dd 0
         dd 1
         dd 0.0
         dd 1.0

         dd 319.0       ;c
         dd 199.0
         dd 0
         dd 0
         dd 1
         dd 1.0
         dd 1.0

triangle2 dd 160.0       ;a
         dd 10.0
         dd 0
         dd 0
         dd 2
         dd 1.0
         dd 0.0

         dd 10.0        ;b
         dd 190.0
         dd 0
         dd 0
         dd 2
         dd 0.0
         dd 0.0

         dd 310.0       ;c
         dd 190.0
         dd 0
         dd 0
         dd 2
         dd 1.0
         dd 1.0

one dd 0.1

main:
        finit
        call InitKB
        invoke InitHeapMem,2000000
        invoke AllocMem,ww*wh
        mov buf.SBits,eax
        invoke AllocMem,768
        mov pal,eax
        invoke LoadBMP8,addr fn,addr texs
        or eax,eax
        jz q
        invoke LoadPalFromBMP,addr fn,pal
        or eax,eax
        jz q
        mov eax,texs.SBits
        mov tex.TBits,eax

        if gvesa eq 0
        SetVGAMode
        else
        SetVESAMode wm,0100000000000000b
        mov cx,wm
        call GetLFB
        mov surf.SBits,eax
        endif

        invoke SetPalette,pal
l:
        IsKeyPressed KEY_ESC
        jnz q

        IsKeyPressed 2
        jz w1d
        IsKeyPressed KEY_UP
        jz k11
        ;a2
        fld dword ptr triangle+4
        fsub one
        fstp dword ptr triangle+4
k11:
        IsKeyPressed KEY_DOWN
        jz k21
        ;a2
        fld dword ptr triangle+4
        fadd one
        fstp dword ptr triangle+4
k21:
        IsKeyPressed KEY_RIGHT
        jz k31
        ;a1
        fld dword ptr triangle
        fadd one
        fstp dword ptr triangle
k31:
        IsKeyPressed KEY_LEFT
        jz k41
        ;a1
        fld dword ptr triangle
        fsub one
        fstp dword ptr triangle
k41:
        jmp k5

w1d:
        IsKeyPressed 3
        jz w2d
        IsKeyPressed KEY_UP
        jz k12
        ;b2
        fld dword ptr triangle+4+__T_SIZE
        fsub one
        fstp dword ptr triangle+4+__T_SIZE
k12:
        IsKeyPressed KEY_DOWN
        jz k22
        ;b2
        fld dword ptr triangle+4+__T_SIZE
        fadd one
        fstp dword ptr triangle+4+__T_SIZE
k22:
        IsKeyPressed KEY_RIGHT
        jz k32
        ;b1
        fld dword ptr triangle+__T_SIZE
        fadd one
        fstp dword ptr triangle+__T_SIZE
k32:
        IsKeyPressed KEY_LEFT
        jz k42
        ;b1
        fld dword ptr triangle+__T_SIZE
        fsub one
        fstp dword ptr triangle+__T_SIZE
k42:
        jmp k5

w2d:
        IsKeyPressed 4
        jz w3d
        IsKeyPressed KEY_UP
        jz k13
        ;c2
        fld dword ptr triangle+4+__T_SIZE+__T_SIZE
        fsub one
        fstp dword ptr triangle+4+__T_SIZE+__T_SIZE
k13:
        IsKeyPressed KEY_DOWN
        jz k23
        ;c2
        fld dword ptr triangle+4+__T_SIZE+__T_SIZE
        fadd one
        fstp dword ptr triangle+4+__T_SIZE+__T_SIZE
k23:
        IsKeyPressed KEY_RIGHT
        jz k33
        ;c1
        fld dword ptr triangle+__T_SIZE+__T_SIZE
        fadd one
        fstp dword ptr triangle+__T_SIZE+__T_SIZE
k33:
        IsKeyPressed KEY_LEFT
        jz k43
        ;c1
        fld dword ptr triangle+__T_SIZE+__T_SIZE
        fsub one
        fstp dword ptr triangle+__T_SIZE+__T_SIZE
k43:
        jmp k5

w3d:

        IsKeyPressed KEY_UP
        jz k1
        ;a2
        fld dword ptr triangle+4
        fsub one
        fstp dword ptr triangle+4
        ;b2
        fld dword ptr triangle+4+__T_SIZE
        fsub one
        fstp dword ptr triangle+4+__T_SIZE
        ;c2
        fld dword ptr triangle+4+__T_SIZE+__T_SIZE
        fsub one
        fstp dword ptr triangle+4+__T_SIZE+__T_SIZE
k1:
        IsKeyPressed KEY_DOWN
        jz k2
        ;a2
        fld dword ptr triangle+4
        fadd one
        fstp dword ptr triangle+4
        ;b2
        fld dword ptr triangle+4+__T_SIZE
        fadd one
        fstp dword ptr triangle+4+__T_SIZE
        ;c2
        fld dword ptr triangle+4+__T_SIZE+__T_SIZE
        fadd one
        fstp dword ptr triangle+4+__T_SIZE+__T_SIZE
k2:
        IsKeyPressed KEY_RIGHT
        jz k3
        ;a1
        fld dword ptr triangle
        fadd one
        fstp dword ptr triangle
        ;b1
        fld dword ptr triangle+__T_SIZE
        fadd one
        fstp dword ptr triangle+__T_SIZE
        ;c1
        fld dword ptr triangle+__T_SIZE+__T_SIZE
        fadd one
        fstp dword ptr triangle+__T_SIZE+__T_SIZE
k3:
        IsKeyPressed KEY_LEFT
        jz k4
        ;a1
        fld dword ptr triangle
        fsub one
        fstp dword ptr triangle
        ;b1
        fld dword ptr triangle+__T_SIZE
        fsub one
        fstp dword ptr triangle+__T_SIZE
        ;c1
        fld dword ptr triangle+__T_SIZE+__T_SIZE
        fsub one
        fstp dword ptr triangle+__T_SIZE+__T_SIZE
k4:
;        int 3
k5:
        fld dword ptr triangle+__T_SIZE+__T_SIZE
        fsub dword ptr triangle+__T_SIZE
        fadd dword ptr triangle
        fstp dword ptr triangle2
        fld dword ptr triangle+4+__T_SIZE+__T_SIZE
        fsub dword ptr triangle+4+__T_SIZE
        fadd dword ptr triangle+4
        fstp dword ptr triangle2+4

        fld dword ptr triangle
        fstp dword ptr triangle2+__T_SIZE
        fld dword ptr triangle+4
        fstp dword ptr triangle2+4+__T_SIZE

        fld dword ptr triangle+__T_SIZE+__T_SIZE
        fstp dword ptr triangle2+__T_SIZE+__T_SIZE
        fld dword ptr triangle+4+__T_SIZE+__T_SIZE
        fstp dword ptr triangle2+4+__T_SIZE+__T_SIZE
        
        mov al,0
        mov esi,offset buf
        call FillSurface
;        int 3
        invoke DrawTexTriangle8,addr buf,addr tex,addr triangle
        invoke DrawTexTriangle8,addr buf,addr tex,addr triangle2
        call Wait4VSync
        mov esi,offset buf
        mov edi,offset surf
        call CopySurface
        jmp l
q:
        SetTxtMode
        if debug eq 1
        debug_Print 0,0,'Czasy: %u,%u,%u,%u',t1,t2,t3,t4
        endif

        call FreeHeapMem
        call CloseKB
        mov ax,4c00h
        int 21h

stub32 ends

end
