IFNDEF __VGATEXT_INC
__VGATEXT_INC=1
;VGATEXT.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do wyswietlania stringow na powierzchniach ze zbioru liter VGA

;includy
include strings.inc

;stale

VGACHARS_8X8            equ 0
VGACHARS_8X16           equ 1

NOEFECTS                equ 0
EFECTS_FADE             equ 1*65536

;funkcje

;--------------------------------
;DrawString8 (eax,ebx,ecx,edx,esi,edi)
;       Rysuje string na powierzchni 8-bitowej
;       Wejscie: eax-pozycja x stringu
;                ebx-pozycja y stringu
;                ecx-kolor
;                edx-0..15-rodzaj znakow,16..31-efekty
;                esi-offset do lancucha ASCIIZ
;                edi-offset do TSurface (powierzchnia docelowa)
;       Wyjscie: eax-liczba narysowanych znakow
;--------------------------------

DrawString8 proc stdcall uses ebx ecx edx esi edi
        local chars_height:dword,bios_chars_type:byte
        local slen:dword,efects:dword
        local shfx:dword
        local scolor:byte,ccolor:byte
        ;
        cld
        ;
        cmp dx,VGACHARS_8X8
        jne DrawString8_no_VGACHARS_8X8
        mov chars_height,8
        mov byte ptr [__MODSHF+2],3
        mov bios_chars_type,3
        jmp DrawString8_calc_string
DrawString8_no_VGACHARS_8X8:
        cmp dx,VGACHARS_8X16
        jne DrawString8_end_proc
        mov chars_height,16
        mov byte ptr [__MODSHF+2],4
        mov bios_chars_type,6

        ;Sprawdzenie czy tekst miesci sie na ekranie
DrawString8_calc_string:
        ;efekty
        shr edx,16
        mov efects,edx
        ;
        cmp byte ptr [edi+8],1  ;SBytesPerPix
        jne DrawString8_end_proc
        ;
        mov edx,chars_height
        neg edx
        cmp ebx,edx
        jle DrawString8_end_proc
        cmp ebx,[edi+4]         ;SHeight
        jge DrawString8_end_proc
        ;
        cmp eax,[edi]           ;SWidth
        jge DrawString8_end_proc
        push eax
        mov edx,eax
        invoke StrLen,esi
        mov slen,eax
        shl eax,3               ;eax*8
        add edx,eax
        cmp edx,-8
        jle DrawString8_end_proc
        cmp edx,[edi]           ;SWidth
        jle DrawString8_no_sub_slen
        sub edx,[edi]           ;SWidth
        shr edx,3
        sub slen,edx
        cmp slen,0
        jle DrawString8_end_proc
DrawString8_no_sub_slen:
        pop eax

        ;Wyliczenie poczatku (eax=x ebx=y)
        mov edx,[edi]           ;SWidth
        mov shfx,edx
        sub shfx,8
        imul ebx,edx
        add eax,ebx
        mov edi,[edi+13]        ;SBits
        add edi,eax             ;y*SWidth+x+SBits
        ;
        push ebp
        push ecx
        mov ax,1130h
        mov bh,bios_chars_type
        int 10h                 ;pobierz poczatek bufora znakow bios
        pop ecx
        mov eax,ebp
        pop ebp
        xor ebx,ebx
        mov bx,stub32inthandrec.__es
        shl ebx,4
        add eax,ebx
        mov edx,eax
        ;
        mov ax,stub32zerosel
        mov es,ax

        ;Rysowanie: edi=Surface.SBits edx=bios_chars_ba
        mov scolor,cl
        mov ecx,slen
DrawString8_draw_loop:
                mov al,scolor
                mov ccolor,al
                xor eax,eax
                lodsb                           ;pobranie znaku
                push edx
__MODSHF:       shl eax,0
                add edx,eax
                xor ebx,ebx
                push edi
DrawString8_loop_y:
                        mov al,es:[edx+ebx]
                        push ecx
                        mov ecx,8
DrawString8_loop_x:
                                push eax
                                shl eax,1
                                shr eax,cl
                                and eax,1
                                or al,al
                                jz DrawString8_no_put_pixel
                                mov al,ccolor
                                mov es:[edi],al
DrawString8_no_put_pixel:
                                inc edi
                                pop eax
                                loop DrawString8_loop_x
                        pop ecx
                        cmp efects,NOEFECTS
                        je DrawSurface8_no_efects
                        inc ccolor
DrawSurface8_no_efects:
                        add edi,shfx
                        inc ebx
                        cmp ebx,chars_height
                        jb DrawString8_loop_y
                pop edi
                pop edx
                add edi,8
                loop DrawString8_draw_loop
        ;
        mov eax,slen
        jmp DrawString8_end_proc_ok
DrawString8_end_proc:
        xor eax,eax
DrawString8_end_proc_ok:
        ret
DrawString8 endp

ENDIF
