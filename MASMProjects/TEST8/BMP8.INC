IFNDEF __BMP8_INC
__BMP8_INC=1
;BMP8.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do ladowania i wyswietlania bitmap 8-bitowych

;dolaczane pliki

include fileio.inc
include mmem.inc
include surface.inc

;funkcje

;--------------------------------
;LoadBMP8 (filename:addr,surface:addr)
;       Wczytuje bitmape z pliku
;       Wejscie: filename-nazwa pliku
;                surface-adres do struktory TSurface
;       Wyjscie: eax=0 blad,eax=1 ok
;--------------------------------

LoadBMP8 proc stdcall,filename:dword,surface:dword
        local _BM:word,ir:dword,returnc:dword,bmpbits:word
        pushad
        invoke FOpen,filename,OPEN_ONLY,READ_ONLY
        jc LoadBMP8_error_noclose
        mov bx,ax
        ;
        push stub32saveds
        mov ax,stub32savess
        mov stub32saveds,ax
        mov edx,ebp
        sub edx,2
        invoke FRead,bx,2,edx
        pop stub32saveds
        cmp _BM,'MB'
        jne LoadBMP8_error

        ;pobranie bitow na pixel
        push stub32saveds
        mov ax,stub32savess
        mov stub32saveds,ax
        invoke FSeek,bx,1ch,ORGIN_START
        mov edx,ebp
        sub edx,14
        invoke FRead,bx,2,edx
        pop stub32saveds
        cmp bmpbits,8
        jne LoadBMP8_error      ;musi byc 8 bitow na pixel
        ;
        invoke FSeek,bx,12h,ORGIN_START
        invoke FRead,bx,8,surface        ;czytaj SWidth i SHeight powierzchni
        invoke FSeek,bx,1078,ORGIN_START ;idz do poczatku bajtow obr.
        ;
        mov edi,surface
        mov ax,bmpbits
        shr ax,3                ;ax/8
        mov [edi+8],al          ;SBytesPerPix

        ;Przydziel pam. dla powierzchni
        mov eax,edi             ;eax=surface
        invoke AllocSurface
        or eax,eax
        jz LoadBMP8_error
        ;
        mov ecx,[edi+4]         ;bmp_height
        mov ir,1
LoadBMP8_read:
                mov eax,[edi+4]                 ;SHeight
                sub eax,ir                      ;SHeight-ri
                mul dword ptr [edi]             ;(SHeight-ri)*SWidth
                add eax,[edi+13]                ;SBits
                invoke EFRead,bx,[edi],eax      ;wczytuj do pamieci heap
                inc ir
                loop LoadBMP8_read
         mov eax,1
         jmp LoadBMP8_end
LoadBMP8_error_noclose:
        xor eax,eax
        jmp LoadBMP8_noclose
LoadBMP8_error:
        xor eax,eax
LoadBMP8_end:
        push eax
        invoke FClose,bx
        pop eax
LoadBMP8_noclose:
        mov returnc,eax         ;zapamietaj kod powrotu
        popad
        mov eax,returnc
        ret
LoadBMP8 endp


;--------------------------------
;LoadPalFromBMP (filename:dword,pal:dword)
;       Wczytuje palete kolorow z pliku bitmapy
;       Wejscie: filename-nazwa pliku
;                pal-wskaznik bufora z paleta (umieszczonego w pam. heap)
;       Wyjscie: eax=0 blad,eax=1 ok
;--------------------------------

LoadPalFromBMP proc stdcall uses bx,filename:dword,pal:dword
        local bgrr:dword
        invoke FOpen,filename,OPEN_ONLY,READ_ONLY
        jc LoadPalFromBMP_error
        mov bx,ax
        invoke FSeek,bx,54,ORGIN_START

        ;wczytywanie palety
        mov ecx,256
        mov ax,stub32zerosel
        mov es,ax
        mov edi,pal
        push stub32saveds
        mov ax,stub32savess
        mov stub32saveds,ax
        mov edx,ebp
        sub edx,4
LoadPalFromBMP_load_pal:
                invoke FRead,bx,4,edx
                mov al,ss:[edx]         ;->b
                mov ah,ss:[edx+1]       ;->g
                shr al,2                ;b/4
                shr ah,2                ;g/4
                mov es:[edi+2],al       ;b<-
                mov es:[edi+1],ah       ;g<-
                mov al,ss:[edx+2]       ;->r
                shr al,2                ;r/4
                mov es:[edi],al         ;r<-
                add edi,3
                loop LoadPalFromBMP_load_pal
        pop stub32saveds
        invoke FClose,bx
        ;
        mov eax,1
        jmp LoadPalFromBMP_end
LoadPalFromBMP_error:
        xor eax,eax
LoadPalFromBMP_end:
        ret
LoadPalFromBMP endp

ENDIF
