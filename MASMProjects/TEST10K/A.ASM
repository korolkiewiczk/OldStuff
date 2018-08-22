.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

include dpmi.inc
include vesa.inc
include mcon.inc
include vga.inc
include mmem.inc
include surface.inc
include surface8.inc
include bmp8.inc
include aiconv.inc
include kb.inc
include vgatext.inc

rx equ 1024
ry equ 768

nazwa_pliku db 'a.bmp',0
bmp TSurface <>
pe TSurface <rx,ry,1,0,0>
buf TSurface <rx,ry,1,0,0>
bmp2 TSurface <>
pal dd 0
srect TRect <0,0,0,0>
drect TRect <0,0,rx,ry>

.586p

x dd 0
y dd 0
w dd 0
h dd 0
muldelta dd 0

t1 dd 0
t2 dd 0
t3 dd 0

txt db 'Hello world!!!ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstu/.,;[]<>?:"{}\|~`012345689-=+!@#$%^&*()É²°±',0

px db 12 dup (0)

main:
        invoke InitHeapMem,4*1024*1024
        call InitKB
        or eax,eax
        jz q2
        invoke LoadBMP8,addr nazwa_pliku,addr bmp
        invoke LoadBMP8,addr nazwa_pliku,addr bmp2
        invoke AllocMem,rx*ry
        mov buf.SBits,eax
        invoke AllocMem,768
        mov pal,eax
        invoke LoadPalFromBmp,addr nazwa_pliku,pal
        SetVESAMode 105h,0100000000000000b
        invoke SetPalette,pal
        mov cx,105h
        call GetLFB
        cmp eax,0ffffffffh
        je q
        mov pe.SBits,eax
        mov eax,bmp.SWidth
        mov srect.RW,eax
        mov eax,bmp.SHeight
        mov srect.RH,eax
        mov w,rx*256
        mov h,ry*256
        mov x,0
        mov y,0
d:
        rdtsc
        mov t1,eax
        mov eax,x
        sar eax,8
        mov drect.RX,eax
        mov eax,y
        sar eax,8
        mov drect.RY,eax
        mov eax,x
        ;
        mov eax,w
        sar eax,8
        mov drect.RW,eax
        mov eax,h
        sar eax,8
        mov drect.RH,eax
        ;
        mov eax,offset srect
        mov ebx,offset drect
        mov esi,offset bmp
        mov edi,offset buf
        call DrawSurface8S
        ;
        mov esi,offset bmp2
        mov edi,offset bmp
        call CopySurface
        ;
        mov eax,0
        mov ebx,0
        mov ecx,30
        mov edx,VGACHARS_8X16
        mov esi,offset txt
        mov edi,offset buf
        call DrawString8
        mov eax,x
        shr eax,8
        mov ebx,y
        shr ebx,8
        mov ecx,30
        mov edx,VGACHARS_8X8
        mov esi,offset txt
        mov edi,offset buf
        call DrawString8
        mov eax,t3
        invoke UToA,eax,addr px
        mov eax,bmp.SWidth
        shr eax,1
        sub eax,30
        mov ebx,bmp.SWidth
        shr ebx,1
        sub ebx,10
        mov ecx,0
        mov edx,VGACHARS_8X16 or EFECTS_FADE
        mov esi,offset px
        mov edi,offset bmp
        call DrawString8
        ;
;        call Wait4VSync
        ;
        mov esi,offset buf
        mov edi,offset pe
        call CopySurface
        ;
        mov esi,offset buf
        mov al,0
        call FillSurface
        ;
        IsKeyPressed KEY_CTRL
        jz NKS
        mov muldelta,60
        jmp KS
NKS:
        mov muldelta,1
KS:
        IsKeyPressed KEY_ESC
        jnz q
        ;
        IsKeyPressed KEY_UP
        jz NKU
        mov eax,256
        mul muldelta
        sub y,eax
NKU:
        IsKeyPressed KEY_DOWN
        jz NKD
        mov eax,256
        mul muldelta
        add y,eax
NKD:
        IsKeyPressed KEY_LEFT
        jz NKL
        mov eax,256
        mul muldelta
        sub x,eax
NKL:
        IsKeyPressed KEY_RIGHT
        jz NKR
        mov eax,256
        mul muldelta
        add x,eax
NKR:
        IsKeyPressed KEY_PAGEUP
        jz NKPU
        mov eax,341
        mul muldelta
        add w,eax
        add w,eax
        sub x,eax
        mov eax,256
        mul muldelta
        add h,eax
        add h,eax
        sub y,eax
NKPU:
        IsKeyPressed KEY_PAGEDOWN
        jz NKPD
        mov eax,341
        mul muldelta
        sub w,eax
        sub w,eax
        add x,eax
        mov eax,256
        mul muldelta
        sub h,eax
        sub h,eax
        add y,eax
NKPD:
        rdtsc
        mov t2,eax
        mov eax,t2
        sub eax,t1
        mov t3,eax
        jmp d
q:
        SetTXTMode
        invoke FreeHeapMem
        call CloseKB
q2:
        mov ax,4c00h
        int 21h
stub32 ends

end
