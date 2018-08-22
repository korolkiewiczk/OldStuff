.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;moduly

include dpmi.inc
include vga.inc
include mmem.inc
include surface.inc
include surface8.inc
include bmp8.inc
include mcon.inc
include aiconv.inc

;dane

pal dd 0 ;wsk. na pal
plik_z_bitmapa db 'a.bmp',0
bitmapa TSurface <>

pow_ekr TSurface <320,200,1,320,0a0000h>
buf TSurface <320,200,1,0,0>

l db 10 dup (0)

srcrect TRect <0,0,320,200>
srcrect2 TRect <150,150,30,1000>
srcrect3 TRect <320,0,320,200>
srcrect4 TRect <-64,-64,60,60>
descrect TRect <0,0,320,200>
descrect2 TRect <20,10,30,70>
descrect3 TRect <0,0,320,200>
descrect4 TRect <0,0,128,128>

;kod

main:
        invoke InitHeapMem,1000000
        invoke AllocMem,768
        mov pal,eax
        SetVGAMode
        ;
;        int 3
        invoke LoadBMP8,addr plik_z_bitmapa,addr bitmapa
        or eax,eax
        jz ep
        mov eax,offset buf
        call AllocSurface
        invoke LoadPalFromBMP,addr plik_z_bitmapa,pal
        invoke SetPalette,pal

        ;rysuj powierzchnie
        .586p
        rdtsc
        push eax
        mov eax,offset srcrect
        mov ebx,offset descrect
        mov esi,offset bitmapa
        mov edi,offset pow_ekr
        call DrawSurface8
        rdtsc
        pop edx
        sub eax,edx
        invoke UToA,eax,addr l
        ReadKey
;        int 3
        mov eax,offset srcrect2
        mov ebx,offset descrect2
        call DrawSurface8
        ReadKey
        mov eax,offset srcrect3
        mov ebx,offset descrect3
        call DrawSurface8
        ReadKey
        mov eax,offset srcrect4
        mov ebx,offset descrect4
        call DrawSurface8
        ReadKey
        mov srcrect4.RX,0
        mov srcrect4.RY,0
p:
        int 3
        mov al,255
        mov esi,offset buf
        call FillSurface
        mov eax,offset srcrect4
        mov ebx,offset descrect
        mov esi,offset bitmapa
        mov edi,offset buf
        call DrawSurface8S
        call Wait4VSync
        mov esi,offset buf
        mov edi,offset pow_ekr
        call CopySurface
        ;inc srcrect4.RX
        ;mov eax,bitmapa.SWidth
        ;cmp srcrect4.RX,eax
        ;jng d1
        ;mov srcrect4.RX,-320
        ;sub descrect.RX,3200
        ;sub descrect.RY,2000
        ;dec descrect.RX
        ;dec descrect.RY
;        add descrect.RW,320
 ;       add descrect.RH,200
        add scrrect4.RW,1
        add srcrect4.RW,1
        ;mov eax,bitmapa.SHeight
        ;sub eax,200
        ;cmp srcrect4.RY,eax
        ;jg ep
d1:
        ReadKey
        cmp ah,1
        jnz p
        ;
ep:
        mov esi,offset bitmapa
        mov edi,offset pow_ekr
;        int 3
        call CopySurface
        ReadKey
        ReadKey
        SetTXTMode
        invoke Print,0,0,addr l
        invoke FreeHeapMem
        mov ah,4ch
        int 21h
stub32 ends

end
