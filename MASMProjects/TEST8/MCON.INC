IFNDEF __MCON_INC
__MCON_INC=1
;MCON.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do obslogi konsoli (wypisywanie tekstow itd.)


;zmienne

CON_CUR_X db 0          ;wsp. x kursora
CON_CUR_Y db 0          ;wsp. y kursora
CON_TEXT_ATTR db 7      ;atrybuty (kolor,tlo)


;makra

;--------------------------------
;ReadKey
;       Czeka na nacisniecie klawisza i pobiera go
;       Wyjscie: al-kod ascii klawisza
;                ah-kod scan klawisza
;--------------------------------

ReadKey macro stdcall
        mov ah,10h
        int 16h
endm

;--------------------------------
;GetKey
;       Pobiera klawisz
;       Wyjscie: jesli zf=0 to klawisz wcisniety:
;                al-kod ascii klawisza
;                ah-kod scan klawisza
;--------------------------------

GetKey macro
        mov ah,11h
        int 16h
endm

;funkcje

;--------------------------------
;GotoXY (x:word,y:word)
;       Ustawia wsp. kursora na x i y
;       Wejscie: x-wsp. x
;                y-wsp. y
;--------------------------------
GotoXY proc stdcall uses ax bx dx,x:byte,y:byte
        mov ax,0200h
        xor bx,bx
        mov dl,x
        mov CON_CUR_X,dl
        mov dh,y
        mov CON_CUR_Y,dh
        int 10h
        ret
GotoXY endp

;--------------------------------
;Print (x:byte,y:byte,asciizs:addr)
;       Wypisuje stringa (ASCIIZ) na ekranie o wsp. x i y
;       Wejscie: x-wsp. x
;                y-wsp. y
;                asciizs-napis (ASCIIZ)
;--------------------------------

Print proc stdcall uses edi esi edx bx,x:byte,y:byte,asciizs:dword
        mov ax,stub32zerosel
        mov es,ax
        mov edi,0b8000h
        mov esi,asciizs
        movzx edx,x
        movzx eax,y
        mov bl,160
        mul bl
        xor bh,bh
        mov bl,x
        shl bx,1
        add ax,bx
        add edi,eax
        mov ah,CON_TEXT_ATTR
Print_draw_chars:
                lodsb
                or al,al
                jz Print_break_loop
                ;
                cmp al,13
                jne Print_no_code13
                ;jesli al=13
                movzx eax,x
                sub eax,edx
                shl eax,1
                sub edi,eax
                mov x,dl
                mov ah,CON_TEXT_ATTR
                jmp Print_draw_chars
Print_no_code13:
                cmp al,10
                jne Print_no_code10
                ;jesli al=10
                add edi,160
                inc y
                jmp Print_draw_chars
Print_no_code10:
                stosw
                inc x
                cmp x,80
                jne Print_draw_chars
                mov x,0
                inc y
                jmp Print_draw_chars
Print_break_loop:
        invoke GotoXY,x,y
        ret
Print endp

;--------------------------------
;Cls ()
;       Czysci ekran tekstowy
;--------------------------------

Cls proc stdcall uses edi ecx bx
        mov ax,stub32zerosel
        mov es,ax
        mov edi,0b8000h
        mov al,' '
        mov ah,CON_TEXT_ATTR
        mov bx,ax
        shl eax,16
        mov ax,bx
        mov ecx,1000
        rep stosd
        invoke GotoXY,0,0
        ret
Cls endp

ENDIF
