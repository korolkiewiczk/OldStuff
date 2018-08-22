.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;dane

lfb dd 0

;kod

include dpmi.inc
include mstdlib\mcon.inc
include vga.inc
include vesa.inc

r db 0
g db 0
b db 0

main:

        SetVESAMode V1024x768x16M,0
        mov cx,V1024x768x16M
        call GetLFB
        cmp eax,0ffffffffh
        je q
        mov lfb,eax
        ;
        mov es,stub32zerosel
        mov edi,eax
        mov ecx,1024*768
        xor eax,eax
        mov r,al
        mov g,al
        mov b,al
l:
        mov al,r
        mov ah,0
        shl eax,16
        mov al,b
        mov ah,g
        stosd
        inc r
        cmp r,255
        jne d1
        inc g
        cmp g,255
        jne d1
        add b,22
d1:
        loop l
        ;
        ReadKey
        SetTXTMode
q:
        mov ax,4c00h
        int 21h

stub32 ends

end
