.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;kod

include dpmi.inc
include surface.inc
include mstdlib\mcon.inc
include vga.inc
include vesa.inc
include bmp8.inc
include fade8.inc
include surface8.inc
include surface8t.inc
include kb.inc

scrx equ 1024
scry equ 768
scra equ scrx*scry
mode equ V1024X768X256

bmpname db 'tes.bmp',0
imgs TSurface <>
buf  TSurface <scrx,scry,1,0,0>
scrs TSurface <scrx,scry,1,0,0a0000h>
imgt TRect <0,0,1024,768>
imgtt TRect <1024,0,320,200>
imgt2 TRect <0,0,320,200>
scrt TRect <0,0,scrx,scry>
pal dd 0

tr dd 3*8
fd dd 31

przess db 0

iw dd 0
ih dd 0

main:

        invoke InitHeapMem,1024*1024*4
        call InitKB
        invoke AllocMem,scra
        mov buf.SBits,eax
        invoke LoadBMP8,addr bmpname,addr imgs
        invoke AllocMem,768
        mov pal,eax
        invoke LoadPalFromBMP,addr bmpname,pal
        ;SetVGAMode
        SetVESAMode mode,0
        mov cx,mode
        call GetLFB
        mov scrs.SBits,eax
        invoke SetPalette,pal
        invoke InitFADE8Tab,6,pal
        invoke InitTRANSP8Tab,3,pal
        mov eax,imgt2.RW
        shl eax,8
        mov iw,eax
        mov eax,imgt2.RH
        shl eax,8
        mov ih,eax
drw:
        mov eax,offset imgt
        mov ebx,offset scrt
        mov esi,offset imgs
        mov edi,offset buf
        call DrawSurface8
        mov edi,buf.SBits
        add edi,725*scrx
        mov ecx,(scry-725)*scrx
        mov al,przess
dlllll:
        mov fs:[edi],al
        inc al
        inc edi
        loop dlllll
        inc przess
        mov eax,offset imgtt
        mov ebx,offset imgt2
        mov esi,offset imgs
        mov edi,offset buf
        mov edx,tr
        shr edx,3
        mov ecx,fd
        call DrawSurface8TS
        mov eax,iw
        shr eax,8
        mov imgt2.RW,eax
        mov eax,ih
        shr eax,8
        mov imgt2.RH,eax
        IsKeyPressed KEY_ESC
        jnz q
        IsKeyPressed KEY_RIGHT
        jz d1
        inc imgt2.RX
d1:
        IsKeyPressed KEY_LEFT
        jz d2
        dec imgt2.RX
d2:
        IsKeyPressed KEY_UP
        jz d3
        dec imgt2.RY
d3:
        IsKeyPressed KEY_DOWN
        jz d4
        inc imgt2.RY
d4:
        IsKeyPressed KEY_PAGEUP
        jz d5
        inc tr
d5:
        IsKeyPressed KEY_PAGEDOWN
        jz d6
        dec tr
d6:
        IsKeyPressed KEY_HOME
        jz d7
        inc fd
d7:
        IsKeyPressed KEY_END
        jz d8
        dec fd
d8:
        IsKeyPressed KEY_INSERT
        jz d9
        add iw,341
        add ih,256
d9:
        IsKeyPressed KEY_DELETE
        jz d10
        sub iw,341
        sub ih,256
d10:
        IsKeyPressed KEY_CTRL
        jnz d11
        call Wait4VSync
d11:
        mov ebx,64
        mov eax,tr
        xor edx,edx
        div ebx
        mov tr,edx
        mov esi,offset buf
        mov edi,offset scrs
        call CopySurface
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
