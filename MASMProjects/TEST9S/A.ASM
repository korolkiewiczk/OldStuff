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

l db 10 dup (0)
l2 db 10 dup (0)

srcrect TRect <0,0,320,200>
srcrect2 TRect <20,20,50,50>
srcrect3 TRect <0,0,1024,768>
srcrect4 TRect <0,0,160,70>
descrect TRect <0,0,320,200>
descrect2 TRect <0,0,100,100>
descrect3 TRect <0,0,320,200>
descrect4 TRect <144,90,32,20>

;kod

main:
        invoke InitHeapMem,1000000
        invoke AllocMem,768
        mov pal,eax
        SetVGAMode
        ReadKey
        ;
;        int 3
        invoke LoadBMP8,addr plik_z_bitmapa,addr bitmapa
        or eax,eax
        jz ep
        invoke LoadPalFromBMP,addr plik_z_bitmapa,pal
        invoke SetPalette,pal

        ;rysuj powierzchnie
        .586p
;        int 3
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
        call DrawSurface8S
        ReadKey
        mov eax,offset srcrect3
        mov ebx,offset descrect3
        call DrawSurface8S
        ReadKey
;        int 3
        mov eax,offset srcrect3
        mov ebx,offset descrect4
        call DrawSurface8S
        ReadKey
comment#
p:
        ;mov eax,offset srcrect
        ;mov ebx,offset descrect
        ;mov esi,offset bitmapa
        ;mov edi,offset pow_ekr
        ;call DrawSurface8S
        
        mov eax,offset srcrect3
        mov ebx,offset descrect4
        mov esi,offset bitmapa
        mov edi,offset pow_ekr
        call DrawSurface8S
        call Wait4VSync
        dec descrect4.RX
        dec descrect4.RY
        inc descrect4.RH
        inc descrect4.RW
        inc descrect4.RH
        inc descrect4.RW
d1:
        GetKey
        mov ax,stub32inthandrec.__flg
        test ax,64
        jnz p
        ;#
ep:
        SetTXTMode
        invoke Print,0,0,addr l
        invoke FreeHeapMem
        mov ah,4ch
        int 21h
stub32 ends

end
