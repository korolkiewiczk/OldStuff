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

TRect struct                  ;Struktora przechowuje prostokat
        RX dd ?                 ;Pozycja X prostokata
        RY dd ?                 ;Pozycja Y prostokata
        RW dd ?                 ;Szerokosc prostokata
        RH dd ?                 ;Wysokosc prostokata
TRect ends

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

;--------------------------------
;CopySurface (esi,edi)
;       Kopiuje powierzchnie zrodlowa do docelowej
;       Wejscie: esi-offset do powierzchni zrodlowej
;                edi-offset do powierzchni docelowej
;       Uwaga: Obie powierzchnie powinny byc rownej wielkosci i ilosci BPP.
;              Moga byc jednak kopiowane powierzchnie o roznej wielkosci i ilosci BPP co prowadzi
;              do nierownosci w kopii.
;--------------------------------

CopySurface proc stdcall uses eax ebx ecx edx esi edi
        cld
        ;ustalenie ilosci bajtow
        mov eax,[esi]           ;s.SWidth
        cmp eax,[edi]           ;if s.SWidth>d.SWidth
        jna CopySurface_less_width                  ;|
        mov eax,[edi]           ;d.SWidth <-----------
        ;
CopySurface_less_width:
        mov ebx,[esi+4]         ;s.SHeight
        cmp ebx,[edi+4]         ;if s.SHeight>d.SHeight
        jna CopySurface_less_height                  ;|
        mov ebx,[edi+4]         ;d.SWidth <------------
        ;
CopySurface_less_height:
        movzx ecx,byte ptr [esi+8] ;SBytesPerPix
        movzx edx,byte ptr [edi+8] ;SBytesPerPix
        cmp ecx,edx                ;if s.SBytesPerPix>d.SBytesPerPix
        jna CopySurface_less_BPP                                  ;|
        mov ecx,edx             ;d.SWidth <-------------------------
CopySurface_less_BPP:
        mul ebx                 ;SWidth*SHeight
        mul ecx                 ;SWidth*SHeight*SBytesPerPix

        ;ustawienie selektorow i wskaznikow
        mov esi,[esi+13]        ;SBits
        mov edi,[edi+13]        ;SBits
        mov dx,stub32zerosel
        mov es,dx
        push ds
        mov ds,dx

        ;kopiowanie
        mov ecx,eax
        shr ecx,2
        and eax,3
        rep movsd
        mov ecx,eax
        rep movsb
        pop ds
        ret
CopySurface endp

;--------------------------------
;FillSurface (al/ax/eax,esi)
;       Wypelnia powierzchnie dana wartoscia
;       Wejscie: al/ax/eax-wartosc (zalezy od BPP powierzchni np. dla BPP=2 parametrem jest ax)
;                esi-offset do powierzchni
;--------------------------------

FillSurface proc stdcall uses eax ebx ecx edx esi edi
        cld
        push eax
        mov eax,[esi]                   ;SWidth
        mul dword ptr [esi+4]           ;SWidth*SHeight
        movzx ebx,byte ptr [esi+8]      ;SBytesPerPix
        mul ebx                         ;SWidth*SHeight*SBytesPerPix
        mov edi,[esi+13]                ;SBits
        mov ecx,eax
        mov ebx,ecx
        shr ecx,2
        and ebx,3
        mov ax,stub32zerosel
        mov es,ax
        pop eax
        ;
        cmp byte ptr [esi+8],1          ;SBytesPerPix
        jne FillSurface_BPP_2
        ;1BPP
        mov ah,al
        mov dx,ax
        shl eax,16
        mov ax,dx
        jmp FillSurface_fill
FillSurface_BPP_2:
        cmp byte ptr [esi+8],2          ;SBytesPerPix
        jne FillSurface_fill
        ;2BPP
        mov dx,ax
        shl eax,16
        mov ax,dx
FillSurface_fill:

        rep stosd
        mov ecx,ebx
        rep stosb
        ret
FillSurface endp

ENDIF
