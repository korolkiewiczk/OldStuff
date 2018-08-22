IFNDEF __BMP8_INC
__BMP8_INC=1
;BMP8.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do ladowania i wyswietlania bitmap 8-bitowych

;dolaczane pliki

include fileio.inc
include mmem.inc

;struktory

BMP8_IMG struct                 ;Stroktora przechowyjaca 8-bitowy obrazek BMP
        bmp_width dd 0          ;szerokosc bitmapy
        bmp_height dd 0         ;wysokosc bitmapy
        bmp_bits dd 0           ;wsk. do bajtow bitmapy
BMP8_IMG ends

;funkcje

;--------------------------------
;LoadBMP (filename:addr,bm:addr)
;       Wczytuje bitmape z pliku
;       Wejscie: filename-nazwa pliku
;                bm-adres do struktory BMP8_IMG
;       Wyjscie: eax=0 blad,eax=1 ok
;--------------------------------

LoadBMP proc stdcall,filename:dword,bm:dword
        local _BM:word,ir:dword
        pushad
        invoke FOpen,filename,OPEN_ONLY,READ_ONLY
        jc LoadBMP_error_noclose
        mov bx,ax
        ;
        invoke FRead,bx,2,_BM
        cmp _BM,'MB'
        jne LoadBMP_error
        ;
        invoke FSeek,bx,12,ORGIN_START  ;czytaj wys. i szer. bitmap
        invoke FRead,bx,8,addr bm
        ;
        invoke FSeek,bx,1078,ORGIN_START
        mov edi,bm
        mov eax,[edi]           ;bmp_width
        mul dword ptr [edi+4]   ;bmp_height
        invoke AllocMem,eax
        cmp eax,NULL
        je LoadBMP_error
        ;
        mov [edi+8],eax
        mov ecx,[edi+4]         ;bmp_height
        mov ir,1
LoadBMP_read:
                mov eax,[edi+4]                 ;bmp_height
                sub eax,ir                      ;bmp_height-ri
                mul dword ptr [edi]             ;(bmp_height-ri)*bmp_width
                add eax,[edi+8]                 ;bmp_bits
                invoke EFRead,bx,[edi],eax      ;wczytuj do pamieci heap
                inc ir
                loop LoadBMP_read
         mov eax,1
         jmp LoadBMP_end
LoadBMP_error_noclose:
        xor eax,eax
        jmp LoadBMP_noclose
LoadBMP_error:
        xor eax,eax
LoadBMP_end:
        invoke FClose,bx
LoadBMP_noclose:
        popad
        ret
LoadBMP endp


;--------------------------------
;LoadPalFromBMP (filename:dword,pal:dword)
;       Wczytuje palete kolorow z pliku bitmapy
;       Wejscie: filename-nazwa pliku
;                pal-wskaznik bufora z paleta (umieszczonego w pam. heap)
;       Wyjscie: eax=0 blad,eax=1 ok
;--------------------------------

LoadPalFromBMP proc stdcall uses bx,filename:dword,pal:dword
        invoke FOpen,filename,OPEN_ONLY,READ_ONLY
        jc LoadPalFromBMP_error
        mov bx,ax
        invoke FSeek,bx,54,ORGIN_START
        invoke EFRead,bx,768,pal
        invoke FClose,bx
        mov eax,1
        jmp LoadPalFromBMP_end
LoadPalFromBMP_error:
        xor eax,eax
LoadPalFromBMP_end:
        ret
LoadPalFromBMP endp

ENDIF
