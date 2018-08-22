.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;dane

lfb dd 0
px dd 0
py dd 0
p dd 0
pp dd 0
ppp dd 0

sx equ 800
sy equ 600
;sm equ V320X200X16M
sm equ V800X600X16M

;kod

include dpmi.inc
include mstdlib\mcon.inc
include vga.inc
include vesa.inc
include kb.inc

__180 dd 180.0
__l real4 2.0

main:

        xor eax,eax
        mov p,eax
        mov ecx,360
ist:
        fild p
        fldpi
        fmulp st(1),st(0)
        fld __180
        fdivp st(1),st(0)
        fsin
        ;fmul __l
        fstp sintab[eax*4]
        ;fistp sintab[eax*4]
        inc p
        inc eax
        loop ist

        call InitKB
        SetVESAMode sm,0
        mov cx,sm
        call GetLFB
        cmp eax,0ffffffffh
        je q
        mov lfb,eax
        ;
        mov es,stub32zerosel
        mov p,0
l2:
        mov edi,lfb
        mov ecx,sx*sy
        xor eax,eax
        mov py,eax
        mov px,eax
l:
        mov eax,px
        add eax,p
        xor edx,edx
        mov ebx,360
        div ebx
        mov eax,edx
        fld sintab[eax*4]
        fmul __l
        fistp pp
        add __l,1
        ;mov edx,sintab[edx*4]
        ;mov pp,edx
        mov eax,py
        add eax,p
        xor edx,edx
        mov ebx,360
        div ebx
        mov eax,edx
        fld sintab[eax*4]
        fmul __l
        fistp ppp
        mov edx,ppp
        mov eax,py
        add eax,pp
        push px
        add px,edx
        xor eax,px
        pop px
        shl eax,16
        ;xor edx,edx
        ;mov ebx,256
        ;div ebx
        ;mov eax,edx
        ;shl eax,16
        mov edx,px
        shr edx,2
        add eax,edx
        ;mul py
        ;mul ebx
        ;add eax,py
        ;xor eax,py
        ;mul py
        ;imul eax,p
        stosd

        inc px
        cmp px,sx
        jb d
        inc py
        mov px,0
d:
        dec ecx
        jnz l
        ;
        inc p
        IsKeyPressed KEY_ESC
        jz l2

        call CloseKB

        SetTXTMode
q:
        mov ax,4c00h
        int 21h

;sintab real4 360 dup (0.0)
sintab dd 360 dup (0)

stub32 ends

end
