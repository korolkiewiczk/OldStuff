.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

include dpmi.inc
include vga.inc
include mmem.inc
include bmp8.inc
include mcon.inc
include aiconv.inc

img TSurface <>
wsk dd 0
nazwab db 'a.bmp',0
pal dd 0
t1 dd 0
licz db 10 dup (0)

main:
        invoke InitHeapMem,160000
        invoke AllocMem,1024
        mov pal,eax
        int 3
        ;
        SetVGAMode
        invoke LoadBMP8,addr nazwab,addr img
        or eax,eax
        jz pe
        invoke LoadPalFromBMP,addr nazwab,pal
        or eax,eax
        jz pe
        invoke SetPalette,pal
        ;
        ;invoke AllocMem,64000
        ;mov wsk,eax
        mov esi,img.SBits
        mov ecx,64000/4
        mov edi,0a0000h
        ;mov edi,wsk
        mov ax,stub32zerosel
        mov es,ax
        .586p
        rdtsc
        mov t1,eax
        ;
        push ds
        mov ax,stub32zerosel
        mov ds,ax
        rep movsd
        pop ds
        ;
        rdtsc
        sub eax,t1
        invoke UToA,eax,addr licz

        ReadKey
pe:
        SetTXTMode
        invoke Print,0,0,addr licz
        invoke FreeHeapMem
        mov ah,4ch
        int 21h
stub32 ends

program_stack segment stack
db 512 dup (?)
program_stack ends

end
