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

rx equ 1280
ry equ 1024

nazwa_pliku db 'a2.bmp',0
bmp TSurface <>
pe TSurface <rx,ry,1,0,0>
buf TSurface <rx,ry,1,0,0>
pal dd 0
srect TRect <60,60,320,200>
drect TRect <0,0,rx,ry>

.586p

x dd 0
y dd 0

t1 dd 0
t2 dd 0

l dd 10 dup (0)

main:
        invoke InitHeapMem,4*1024*1024
        call InitKB
        or eax,eax
        jz q2
        invoke LoadBMP8,addr nazwa_pliku,addr bmp
        invoke AllocMem,rx*ry
        mov buf.SBits,eax
        invoke AllocMem,768
        mov pal,eax
        invoke LoadPalFromBmp,addr nazwa_pliku,pal
        SetVESAMode 107h,0000000000000000b
        invoke SetPalette,pal
        MapPhysAddr 0c0000000h,4000000
        mov pe.SBits,ebx
;        invoke AllocMem,1024*768
;        mov pe.SBits,eax
        mov x,320*256*2
        mov y,200*256*2
d:
        mov eax,x
        sar eax,8
        mov srect.RW,eax
        mov eax,y
        sar eax,8
        mov srect.RH,eax
        rdtsc
        mov t1,eax
        mov eax,offset srect
        mov ebx,offset drect
        mov esi,offset bmp
        mov edi,offset pe
        call DrawSurface8S
        call Wait4VSync
;        mov esi,offset buf
;        mov edi,offset pe
;        call CopySurface
        rdtsc
        mov t2,eax
        IsKeyPressed 1
        jnz q
        IsKeyPressed 72
        jz d
        cmp x,256*20
        jb d2
        sub x,409
        sub y,256
        jmp d
d2:
        dec drect.RX
        dec drect.RY
        mov drect.RW,1280*10
        mov drect.RH,1024*10
        jmp d
q:
        SetTXTMode
        invoke FreeHeapMem
        mov eax,t2
        sub eax,t1
        invoke UToA,eax,addr l
        invoke Print,0,0,addr l
        call CloseKB
q2:
        mov ax,4c00h
        int 21h
stub32 ends

end
