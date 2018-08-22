IFNDEF __VESA_INC
__VESA_INC=1
;VESA.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Procedury i makra do obslugi VESA

;makra

;Makra zwracaja blad jesli ah=1. Jesli funkcja jest dostepna to al=4f

;--------------------------------
;SetVESAMode mode,attr
;       Makro ustawia tryb VESA z atrybutami
;       Uzywane rejestry: ax,bx
;       Wejscie: mode-tryb graficzny VESA
;                attr-atrybuty (14-bit=1-uzywaj LFB)
;--------------------------------

SetVESAMode macro mode,attr
        mov ax,4f02h
        mov bx,mode
        or bx,attr
        int 10h
endm

;--------------------------------
;GetCurrVESAMode
;       Pobiera aktualny tryb graficzny VESA
;       Uzywane rejestry: ax
;       Wyjscie: bx-tryb
;--------------------------------

GetCurrVESAMode macro
        mov ax,4f03h
        int 10h
endm

ENDIF
