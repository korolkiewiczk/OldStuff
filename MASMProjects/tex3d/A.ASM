gvesa=0
.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;dane

;kod

include dpmi.inc
include mstdlib\mmem.inc
include surface.inc
include bmp8.inc
include d_data.inc
__T_SIZE=28
include d_tex8.inc
include d_fil8.inc
include vga.inc
include vesa.inc
include kb.inc

if gvesa eq 0
ww=320
wh=200
surf TSurface <320,200,1,320,0a0000h>
buf TSurface <320,200,1,320,0>
else
ww=640
wh=480
wm=V640X480X256
surf TSurface <ww,wh,1,ww,0a0000h>
buf TSurface <ww,wh,1,ww,0>
endif
tex TTex <7,0>
texs TSurface <>
pal dd 0
fn db 'tex.bmp',0

PR dd 100.0
delta dd 1.0
SW dd ww/2
SH dd wh/2

;|A(1)     D(4)|
;|             |
;|             |
;|B(2)     C(3)|

X1 dd -100.0
Y1 dd 100.0
Z1 dd 50.0

X2 dd -100.0
Y2 dd -100.0
Z2 dd 50.0

X3 dd 100.0
Y3 dd -100.0
Z3 dd 50.0

X4 dd 100.0
Y4 dd 100.0
Z4 dd 50.0

triangle1 label dword
        ;A
         dd 4 dup (0)
         dd 1
         dd 0.0
         dd 0.0

        ;B
         dd 4 dup (0)
         dd 1
         dd 0.0
         dd 1.0

        ;C
         dd 4 dup (0)
         dd 1
         dd 1.0
         dd 1.0

triangle2 label dword
        ;A
         dd 4 dup (0)
         dd 1
         dd 0.0
         dd 0.0

        ;C
         dd 4 dup (0)
         dd 1
         dd 1.0
         dd 1.0

        ;D
         dd 4 dup (0)
         dd 1
         dd 1.0
         dd 0.0

pom dd 0
Rzutuj:
        fld Z1
        fistp pom
        cmp pom,0
        jz Rzutuj_endp
        fld Z2
        fistp pom
        cmp pom,0
        jz Rzutuj_endp
        fld Z3
        fistp pom
        cmp pom,0
        jz Rzutuj_endp
        fld Z4
        fistp pom
        cmp pom,0
        jz Rzutuj_endp
        ;x=SWidth/2+X()*PR/Z()
        ;y=SHeight/2-Y()*PR/Z()
        ;A(1)
        fld X1
        fmul PR
        fdiv Z1
        fiadd SW
        fst dword ptr triangle1
        fstp dword ptr triangle2
        fld Y1
        fmul PR
        fdiv Z1
        fchs
        fiadd SH
        fst dword ptr triangle1+4
        fstp dword ptr triangle2+4
        ;B(2)
        fld X2
        fmul PR
        fdiv Z2
        fiadd SW
        fstp dword ptr triangle1+__T_SIZE
        fld Y2
        fmul PR
        fdiv Z2
        fchs
        fiadd SH
        fstp dword ptr triangle1+4+__T_SIZE
        ;C(3)
        fld X3
        fmul PR
        fdiv Z3
        fiadd SW
        fst dword ptr triangle1+__T_SIZE+__T_SIZE
        fstp dword ptr triangle2+__T_SIZE
        fld Y3
        fmul PR
        fdiv Z3
        fchs
        fiadd SH
        fst dword ptr triangle1+4+__T_SIZE+__T_SIZE
        fstp dword ptr triangle2+4+__T_SIZE
        ;D(4)
        fld X4
        fmul PR
        fdiv Z4
        fiadd SW
        fstp dword ptr triangle2+__T_SIZE+__T_SIZE
        fld Y4
        fmul PR
        fdiv Z4
        fchs
        fiadd SH
        fstp dword ptr triangle2+4+__T_SIZE+__T_SIZE
Rzutuj_endp:
        ret

main:
        finit
        call InitKB
        invoke InitHeapMem,1024*1024
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
        jz kB
        ;A
        IsKeyPressed KEY_UP
        jz kA1
        ;Y1++
        fld Y1
        fadd delta
        fstp Y1
kA1:
        IsKeyPressed KEY_DOWN
        jz kA2
        ;Y1--
        fld Y1
        fsub delta
        fstp Y1
kA2:
        IsKeyPressed KEY_RIGHT
        jz kA3
        ;X1++
        fld X1
        fadd delta
        fstp X1
kA3:
        IsKeyPressed KEY_LEFT
        jz kA4
        ;X1--
        fld X1
        fsub delta
        fstp X1
kA4:
        IsKeyPressed KEY_PAGEUP
        jz kA5
        ;Z1++
        fld Z1
        fadd delta
        fstp Z1
kA5:
        IsKeyPressed KEY_PAGEDOWN
        jz kA6
        ;Z1--
        fld Z1
        fsub delta
        fstp Z1
kA6:

        jmp kend

kB:
        IsKeyPressed 3
        jz kC
        ;B
        IsKeyPressed KEY_UP
        jz kB1
        ;Y2++
        fld Y2
        fadd delta
        fstp Y2
kB1:
        IsKeyPressed KEY_DOWN
        jz kB2
        ;Y2--
        fld Y2
        fsub delta
        fstp Y2
kB2:
        IsKeyPressed KEY_RIGHT
        jz kB3
        ;X2++
        fld X2
        fadd delta
        fstp X2
kB3:
        IsKeyPressed KEY_LEFT
        jz kB4
        ;X2--
        fld X2
        fsub delta
        fstp X2
kB4:
        IsKeyPressed KEY_PAGEUP
        jz kB5
        ;Z2++
        fld Z2
        fadd delta
        fstp Z2
kB5:
        IsKeyPressed KEY_PAGEDOWN
        jz kB6
        ;Z2--
        fld Z2
        fsub delta
        fstp Z2
kB6:

        jmp kend

kC:

        IsKeyPressed 4
        jz kD
        ;C
        IsKeyPressed KEY_UP
        jz kC1
        ;Y3++
        fld Y3
        fadd delta
        fstp Y3
kC1:
        IsKeyPressed KEY_DOWN
        jz kC2
        ;Y3--
        fld Y3
        fsub delta
        fstp Y3
kC2:
        IsKeyPressed KEY_RIGHT
        jz kC3
        ;X3++
        fld X3
        fadd delta
        fstp X3
kC3:
        IsKeyPressed KEY_LEFT
        jz kC4
        ;X3--
        fld X3
        fsub delta
        fstp X3
kC4:
        IsKeyPressed KEY_PAGEUP
        jz kC5
        ;Z3++
        fld Z3
        fadd delta
        fstp Z3
kC5:
        IsKeyPressed KEY_PAGEDOWN
        jz kC6
        ;Z3--
        fld Z3
        fsub delta
        fstp Z3
kC6:

        jmp kend

kD:

        IsKeyPressed 5
        jz kALL
        ;D
        IsKeyPressed KEY_UP
        jz kD1
        ;Y4++
        fld Y4
        fadd delta
        fstp Y4
kD1:
        IsKeyPressed KEY_DOWN
        jz kD2
        ;Y4--
        fld Y4
        fsub delta
        fstp Y4
kD2:
        IsKeyPressed KEY_RIGHT
        jz kD3
        ;X4++
        fld X4
        fadd delta
        fstp X4
kD3:
        IsKeyPressed KEY_LEFT
        jz kD4
        ;X4--
        fld X4
        fsub delta
        fstp X4
kD4:
        IsKeyPressed KEY_PAGEUP
        jz kD5
        ;Z4++
        fld Z4
        fadd delta
        fstp Z4
kD5:
        IsKeyPressed KEY_PAGEDOWN
        jz kD6
        ;Z4--
        fld Z4
        fsub delta
        fstp Z4
kD6:

        jmp kend

kALL:

        ;ALL
        IsKeyPressed KEY_UP
        jz kALL1
        ;Y1++,Y2++,Y3++,Y4++
        fld Y1
        fadd delta
        fstp Y1
        fld Y2
        fadd delta
        fstp Y2
        fld Y3
        fadd delta
        fstp Y3
        fld Y4
        fadd delta
        fstp Y4

kALL1:
        IsKeyPressed KEY_DOWN
        jz kALL2
        ;Y1--,Y2--,Y3--,Y4--
        fld Y1
        fsub delta
        fstp Y1
        fld Y2
        fsub delta
        fstp Y2
        fld Y3
        fsub delta
        fstp Y3
        fld Y4
        fsub delta
        fstp Y4
kALL2:
        IsKeyPressed KEY_RIGHT
        jz kALL3
        ;X1++,X2++,X3++,X4++
        fld X1
        fadd delta
        fstp X1
        fld X2
        fadd delta
        fstp X2
        fld X3
        fadd delta
        fstp X3
        fld X4
        fadd delta
        fstp X4
kALL3:
        IsKeyPressed KEY_LEFT
        jz kALL4
        ;X1--,X2--,X3--,X4--
        fld X1
        fsub delta
        fstp X1
        fld X2
        fsub delta
        fstp X2
        fld X3
        fsub delta
        fstp X3
        fld X4
        fsub delta
        fstp X4
kALL4:
        IsKeyPressed KEY_PAGEUP
        jz kALL5
        ;Z1++,Z2++,Z3++,Z4++
        fld Z1
        fadd delta
        fstp Z1
        fld Z2
        fadd delta
        fstp Z2
        fld Z3
        fadd delta
        fstp Z3
        fld Z4
        fadd delta
        fstp Z4
kALL5:
        IsKeyPressed KEY_PAGEDOWN
        jz kend
        ;Z1--,Z2--,Z3--,Z4--
        fld Z1
        fsub delta
        fstp Z1
        fld Z2
        fsub delta
        fstp Z2
        fld Z3
        fsub delta
        fstp Z3
        fld Z4
        fsub delta
        fstp Z4

kend:

        mov al,0
        mov esi,offset buf
        call FillSurface
        call Rzutuj
        invoke DrawTexTriangle8,addr buf,addr tex,addr triangle1
        invoke DrawTexTriangle8,addr buf,addr tex,addr triangle2
        ;invoke DrawFilledTriangle8,addr buf,addr triangle1
        ;invoke DrawFilledTriangle8,addr buf,addr triangle2
        call Wait4VSync
        mov esi,offset buf
        mov edi,offset surf
        call CopySurface
        jmp l
q:
        SetTxtMode

        call FreeHeapMem
        call CloseKB
        mov ax,4c00h
        int 21h
stub32 ends

end
