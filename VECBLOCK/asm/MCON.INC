;MCON.INC for MASM/DJGPP by Kamil Korolkiewicz 2005/2006
;Funkcje do obslogi konsoli (wypisywanie tekstow itd.)


.data

public MCON_CUR_X,MCON_CUR_Y,MCON_TEXT_ATTR

MCON_CUR_X db 0          ;wsp. x kursora
MCON_CUR_Y db 0          ;wsp. y kursora
MCON_TEXT_ATTR db 7      ;atrybuty (kolor,tlo)

.code

extern MCON_CUR_X:byte,MCON_CUR_Y:byte,MCON_TEXT_ATTR:byte

;--------------------------------
;MReadKey ()
;       Czeka na nacisniecie klawisza i pobiera go
;       Wyjscie: al-kod ascii klawisza
;                ah-kod scan klawisza
;--------------------------------

MReadKey proc C
        mov ah,10h
        int 16h
        ret
MReadKey endp

;--------------------------------
;MGetKey ()
;       Pobiera klawisz
;       Wyjscie: jesli zf=0 to klawisz wcisniety:
;                al-kod ascii klawisza
;                ah-kod scan klawisza
;--------------------------------

MGetKey proc C
        mov ah,11h
        int 16h
        ret
MGetKey endp

;--------------------------------
;MGotoXY (x:dword,y:dword)
;       Ustawia wsp. kursora na x i y
;       Wejscie: x-wsp. x
;                y-wsp. y
;--------------------------------
MGotoXY proc C uses eax ebx edx,x:dword,y:dword
        mov ax,0200h        
        mov edx,x
        mov MCON_CUR_X,dl
        mov ebx,y
        mov dh,bl
        mov MCON_CUR_Y,dh
        xor bx,bx
        int 10h
        ret
MGotoXY endp

;--------------------------------
;MPrint (x:dword,y:dword,asciizs:addr)
;       Wypisuje stringa (ASCIIZ) na ekranie o wsp. x i y
;       Wejscie: x-wsp. x
;                y-wsp. y
;                asciizs-napis (ASCIIZ)
;--------------------------------

MPrint proc C uses eax edi esi edx ebx es,x:dword,y:dword,asciizs:dword
        mov ax,stub32zerosel
        mov es,ax
        mov edi,0b8000h
        mov esi,asciizs
        mov edx,x
        mov eax,y
        mov bl,160
        mul bl
        mov ebx,x
        shl ebx,1
        add eax,ebx
        add edi,eax
        mov ah,MCON_TEXT_ATTR
Print_draw_chars:
                lodsb
                or al,al
                jz Print_break_loop
                ;
                cmp al,13
                jne Print_no_code13
                ;jesli al=13
                mov eax,x
                sub eax,edx
                shl eax,1
                sub edi,eax
                mov x,edx
                mov ah,MCON_TEXT_ATTR
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
        invoke MGotoXY,x,y
        ret
MPrint endp

;--------------------------------
;MCls ()
;       Czysci ekran tekstowy
;--------------------------------

MCls proc C uses eax edi ecx ebx es
        cld
        mov ax,stub32zerosel
        mov es,ax
        mov edi,0b8000h
        mov al,' '
        mov ah,MCON_TEXT_ATTR
        mov bx,ax
        shl eax,16
        mov ax,bx
        mov ecx,1000
        rep stosd
        invoke MGotoXY,0,0
        ret
MCls endp
