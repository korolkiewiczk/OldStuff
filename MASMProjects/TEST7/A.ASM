.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;dane
nazwa_pliku db 'plik.txt',0
fhandle dw 0
buf db 4000 dup(0)
memwsk dd 0

nazwa_bitmapy db 'a.bmp',0
;bitmapa BMP8_IMG <>

;kod
include dpmi.inc
include fileio.inc
include mcon.inc
include mmem.inc
include vga.inc
include bmp8.inc
main:
;        int 3
        invoke FOpen,addr nazwa_pliku,OPEN_CREATE,READ_ONLY
        mov fhandle,ax
        jc prog_end
        invoke FSize,fhandle
        invoke FRead,fhandle,ax,addr buf
        mov CON_TEXT_ATTR,1
        invoke Print,0,0,addr buf
        mov CON_TEXT_ATTR,7
        Readkey
        invoke FClose,fhandle
        ;
;        int 3
        invoke InitHeapMem,200*1024*1024
        or eax,eax
        jz prog_end
        invoke AllocMem,10000
        cmp eax,NULL
        je prog_end
        mov memwsk,eax
        ;
        mov ecx,1000
        mov es,stub32zerosel
        mov esi,0b8000h
        mov edi,memwsk
        push ds
        mov ds,stub32zerosel
        rep movsd
        pop ds
        invoke Cls
        ReadKey
        mov esi,memwsk
        mov edi,0b8000h
        mov ecx,1000
        push ds
        mov ds,stub32zerosel
        rep movsd
        pop ds
        ReadKey
        ;invoke AllocMem,1*1024*1024
        ;mov memwsk,eax
        ;cmp eax,NULL
        ;je prog_end
        ;int 3
        ;invoke FOpen,addr nazwa_pliku,OPEN_CREATE,READ_ONLY
        ;invoke FSize,fhandle
        ;invoke EFRead,fhandle,eax,memwsk
        ;invoke FClose,fhandle
        ;ReadKey
        ;mov memwsk,eax
        ;mov edi,eax
        ;mov eax,11111111h
        ;mov ecx,1024*1024/4
        ;rep stosd

;        int 3
        SetVGAMode
        mov al,1
        invoke SetColor,255,255,240
        mov edi,0a0000h
        mov ecx,64000
        mov bx,320
dp:
        mov eax,edi
        xor dx,dx
        div bx
        mov es:[edi],dl
        inc edi
        loop dp
        ReadKey
        invoke AllocMem,768
        mov memwsk,eax
        invoke GetPalette,eax
        mov edi,memwsk
        mov ecx,768
ng:
        mov al,es:[edi]
        not al
        mov es:[edi],al
        inc edi
        loop ng
        invoke SetPalette,[memwsk]
        ReadKey
        SetTXTMode

        invoke FreeHeapMem
        or eax,eax
        jz prog_end
        .586p
        rdtsc
        mov ax,4c00h
        int 21h
        txt db 'e',0
prog_end:
        invoke Print,0,0,addr txt
        mov ax,4c00h
        int 21h
stub32 ends

end
