IFNDEF __PARAMS_INC
__PARAMS_INC=1
;PARAMS.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do pobierania parametrow programu


;funkcje

;--------------------------------
;GetParamCount ()
;       Pobiera liczbe parametrow
;       Wyjscie: eax-liczba parametrow
;--------------------------------

GetParamCount proc stdcall uses ecx esi
        xor eax,eax
        mov es,stub32pspsel
        movzx ecx,byte ptr es:[80h]
        or ecx,ecx
        jz GetParamCount_endp
        mov esi,81h
GetParamCount_count_strings:
                cmp byte ptr es:[esi],' '
                je GetParamCount_continue
                cmp esi,81h
                je GetParamCount_inc_eax
                cmp byte ptr es:[esi-1],' '
                jne GetParamCount_continue
GetParamCount_inc_eax:
                inc eax
GetParamCount_continue:
                inc esi
                loop GetParamCount_count_strings
GetParamCount_endp:
        ret
GetParamCount endp

;--------------------------------
;GetParam (al,esi)
;       Pobiera parametr wywolania programu (numeracja od 1)
;       Wejscie: al-numer parametru
;                esi-offset do bufora gdzie ma sie znalesc parametr (ASCIIZ)
;--------------------------------

GetParam proc stdcall uses eax esi edi ecx edx
        mov es,stub32pspsel
        movzx ecx,byte ptr es:[80h]
        or ecx,ecx
        jz GetParam_endp
        mov edi,81h
        xor dl,dl
        xor dh,dh
GetParam_get_it:
                mov ah,es:[edi]
                or dh,dh
                jnz GetParam_cut
                cmp ah,' '
                je GetParam_continue
                cmp edi,81h
                je GetParam_next_param
                cmp byte ptr es:[edi-1],' '
                jne GetParam_continue
GetParam_next_param:
                inc dl
                cmp al,dl
                jne GetParam_continue
                mov dh,1
GetParam_cut:
                cmp ah,' '
                je GetParam_end_get
                mov [esi],ah
                inc esi
GetParam_continue:
                inc edi
                loop GetParam_get_it
        cmp al,dl
        jne GetParam_endp
GetParam_end_get:
        mov byte ptr [esi],0
GetParam_endp:
        ret
GetParam endp

ENDIF
