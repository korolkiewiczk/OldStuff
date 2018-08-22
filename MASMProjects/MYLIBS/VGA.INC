IFNDEF __VGA_INC
__VGA_INC=1
;VGA.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Podstawowe funkcje do obslugi grafiki VGA

;zmienne


;makra

;--------------------------------
;SetVGAMode
;       Makro ustawia tryb graficzny VGA
;--------------------------------

SetVGAMode macro
        mov ax,13h
        int 10h
endm

;--------------------------------
;SetTXTMode
;       Makro ustawia tryb tekstowy
;--------------------------------

SetTXTMode macro
        mov ax,3h
        int 10h
endm

;--------------------------------
;GetMode
;       Makro pobiera aktualny tryb graficzny
;--------------------------------

GetMode macro
        mov ax,0f00h
        int 10h
endm

;funkcje

;--------------------------------
;SetColor (al,r:byte,g:byte,b:byte)
;       Ustawia kolor w palecie kolorow karty graficznej
;       Wejscie: al-kolor
;                r,g,b-skladowe koloru
;--------------------------------

SetColor proc stdcall uses eax edx,r:byte,g:byte,b:byte
        mov dx,3c8h
        out dx,al
        inc dx
        mov al,r
        out dx,al
        mov al,g
        out dx,al
        mov al,b
        out dx,al
        ret
SetColor endp

;--------------------------------
;GetColor (al)
;       Pobiera kolor z palety kolorow karty graficznej
;       Wejscie: al-kolor
;       Wyjscie: al-skladowa r
;                ah-skladowa g
;                bl-skladowa b
;--------------------------------

GetColor proc stdcall uses edx
        mov dx,3c7h
        out dx,al
        mov dx,3c9h
        in al,dx
        mov ah,al       ;ah=r
        in al,dx        ;al=g
        mov bl,al       ;bl=g
        in al,dx        ;al=b
        xchg al,bl      ;al=g bl=b
        xchg al,ah      ;al=r ah=g bl=b
        ret
GetColor endp

;--------------------------------
;SetPalette (bf:addr)
;       Ustawia palete kolorow karty graficznej zamieszczona w buforze
;       Wejscie: bf-bufor
;--------------------------------

SetPalette proc stdcall uses eax esi edx ecx ds,bf:dword
        mov dx,3c8h
        xor al,al
        out dx,al
        inc dx
        mov esi,bf
        mov ax,stub32zerosel
        mov ds,ax
        mov cx,768
        rep outsb
        ret
SetPalette endp

;--------------------------------
;GetPalette (bf:addr)
;       Pobiera palete kolorow karty graficznej do bufora
;       Wejscie: bf-bufor
;--------------------------------

GetPalette proc stdcall uses eax edi edx ecx es,bf:dword
        mov dx,3c7h
        xor al,al
        out dx,al
        mov dx,3c9h
        mov edi,bf
        mov ax,stub32zerosel
        mov es,ax
        mov cx,768
        rep insb
        ret
GetPalette endp

;--------------------------------
;Wait4VSync
;       Czeka na powrot pionowy monitora
;--------------------------------

Wait4VSync proc stdcall uses edx eax
        mov dx,3dah
Wait4VSync_l1:
        in al,dx
        test al,8h
        jnz Wait4VSync_l1

Wait4VSync_l2:
        in al,dx
        test al,8h
        jz Wait4VSync_l2
        ret
Wait4VSync endp

ENDIF
