IFNDEF __SURFACE_INC
__SURFACE_INC=1
;SURFACE.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Struktory przechowujace powierzchnie i funkcje do ich obslugi

;struktory

TSurface struct               ;Podstawowa struktora przechowujaca obraz
        SWidth dd ?             ;Szerokosc powierzchni
        SHeight dd ?            ;Wysokosc powierzchni
        SBytesPerPix db ?       ;Liczba bajtow na pixel
        SJump dd ?              ;Skok powierzchni
        SBits dd ?              ;Wskaznik do bajtow powierzchni
TSurface ends

;funkcje

;--------------------------------
;AllocSurface (eax)
;       Przydziela pamiec dla powierzchni (ustawia pole SJump i SBits)
;       Wejscie: eax-adres do struktory TSurface
;       Wyjscie: eax=0 blad (SBits=NULL), eax=1 ok (SBits<>NULL)
;--------------------------------

AllocSurface proc stdcall uses edi edx ebx
        mov edi,eax
        ;Przydzelenie pam.
        mov eax,[edi]           ;eax=this.SWidth
        mul dword ptr [edi+4]   ;eax=this.SWidth*this.SHeight
        movzx ebx,byte ptr [edi+8]
        mul ebx                 ;eax=this.SWidth*this.SHeight*this.SBytesPerPix
        invoke AllocMem,eax     ;przydziel pamiec
        mov [edi+13],eax        ;Ustaw wsk.
        cmp eax,NULL
        je AllocSurface_error
        ;Obliczenie skoku
        mov eax,[edi+4]         ;eax=this.SHeight
        movzx ebx,byte ptr [edi+8]
        mul ebx                 ;eax=this.SHeight*this.SBytesPerPix
        mov [edi+9],eax         ;Ustaw skok
        mov eax,1
        ret     ;ok
AllocSurface_error:
        xor eax,eax
        ret     ;error
AllocSurface endp

ENDIF
