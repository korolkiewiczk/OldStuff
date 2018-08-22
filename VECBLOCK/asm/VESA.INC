;VESA.INC for DJGPP by Kamil Korolkiewicz 2005/2006
;Funckje do obslugi VESA

.code

;--------------------------------
;SetVESAMode (int,int)
;       Ustawia tryb VESA z atrybutami
;       Uzywane rejestry: ax,bx
;       Wejscie: mode-tryb graficzny VESA
;                attr-atrybuty (14-bit=1-uzywaj LFB)
;--------------------------------

SetVESAMode proc C uses ebx,vesamode:dword,vattr:dword
        mov eax,4f02h
        mov ebx,vesamode
        or ebx,vattr
        int 10h
        ret
SetVESAMode endp
