.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

sh equ 3
ad equ 8

;kod

include dpmi.inc
include surface.inc
include mstdlib\mcon.inc
include vga.inc
include bmp8.inc
include fade8.inc
include surface8f.inc
include kb.inc

bmpname db 'a.bmp',0
imgs TSurface <>
scrs TSurface <320,200,1,0,0a0000h>
imgt TRect <1,1,320,200>
scrt TRect <0,0,320,200>
pal dd 0

main:

        invoke InitHeapMem,1024*1024
        call InitKB
        invoke LoadBMP8,addr bmpname,addr imgs
        invoke AllocMem,768
        mov pal,eax
        invoke LoadPalFromBMP,addr bmpname,pal
        SetVGAMode
        invoke SetPalette,pal
        invoke InitFADE8Tab,6,pal
        mov ecx,FADE8TabSize
        shl ecx,sh
        sub ecx,ad
        shr ecx,1
drw:
        mov eax,offset imgt
        mov ebx,offset scrt
        mov esi,offset imgs
        mov edi,offset scrs
        mov edx,ecx
        shr ecx,sh
        call DrawSurface8F
        mov ecx,edx
        IsKeyPressed KEY_ESC
        jnz q
        IsKeyPressed KEY_RIGHT
        jz d1
        inc ecx
d1:
        IsKeyPressed KEY_LEFT
        jz d2
        dec ecx
d2:
        IsKeyPressed KEY_UP
        jz d3
        add ecx,ad
d3:
        IsKeyPressed KEY_DOWN
        jz d4
        sub ecx,ad
d4:
        mov ebx,FADE8TabSize
        shl ebx,sh
        mov eax,ecx
        xor edx,edx
        div ebx
        mov ecx,edx
        call Wait4VSync
        jmp drw
        ;
q:
        call CloseKB
        SetTXTMode
        call FreeHeapMem
        mov ax,4c00h
        int 21h

stub32 ends

end
