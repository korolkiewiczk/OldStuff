;VGA.INC for DJGPP by Kamil Korolkiewicz 2005/2006
;Podstawowe funkcje do obslugi grafiki VGA

.code

;--------------------------------
;SetVGAMode
;       Ustawia dany tryb graficzny VGA
;--------------------------------

SetVGAMode proc C,vgamode:dword
        mov eax,vgamode
        int 10h
        ret
SetVGAMode endp

;funkcje

;--------------------------------
;SetColor (int,int,int,int)
;       Ustawia kolor w palecie kolorow karty graficznej
;--------------------------------

SetColor proc C ,col:dword,r:dword,g:dword,b:dword
        mov eax,col
        mov dx,3c8h
        out dx,al
        inc dx
        mov eax,r
        out dx,al
        mov eax,g
        out dx,al
        mov eax,b
        out dx,al
        ret
SetColor endp

;--------------------------------
;GetColor (int,int*,int*,int*)
;       Pobiera kolor z palety kolorow karty graficznej
;--------------------------------

GetColor proc C ,col:dword,r:dword,g:dword,b:dword
        mov eax,col
        mov dx,3c7h
        out dx,al
        mov dx,3c9h
        xor eax,eax
        in al,dx
        mov ecx,r
        mov [ecx],eax
        in al,dx        ;al=g
        mov ecx,g
        mov [ecx],eax
        in al,dx        ;al=b
        mov ecx,b
        mov [ecx],eax
        ret
GetColor endp


;--------------------------------
;SetPalette (char*)
;       Ustawia palete kolorow karty graficznej zamieszczona w buforze
;       Wejscie: bf-bufor
;--------------------------------

SetPalette proc C uses esi ,bf:dword
        mov dx,3c8h
        xor al,al
        out dx,al
        inc dx
        mov esi,bf
        mov ecx,768
        rep outsb
        ret
SetPalette endp

;--------------------------------
;Wait4VSync()
;       Czeka na powrot pionowy monitora
;--------------------------------

Wait4VSync proc C
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
