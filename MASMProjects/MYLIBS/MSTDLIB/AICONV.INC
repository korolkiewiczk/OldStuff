IFNDEF __AICONV_INC
__AICONV_INC=1
;AICONV for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do zamiany liczb na stringi (ASCIIZ) i na odwrot


;funkcje

;--------------------------------
;IToA (i:sdword,s:addr)
;       Zamienia liczbe (SDWORD) na string (ASCIIZ)
;       Wejscie: i - liczba
;                s - string (adres)
;--------------------------------

IToA proc stdcall uses edi edx ecx ebx eax,i:sdword,s:dword
        mov edi,s
        mov eax,i
        xor ecx,ecx
        ;
        mov ebx,eax
        shr ebx,31
        or bl,bl
        ;
        jz IToA_jump1
        mov byte ptr [edi],'-'
        inc edi
        neg eax
        ;
IToA_jump1:
        mov ebx,10
IToA_loop1:
                xor edx,edx
                idiv ebx
                add dl,'0'
                push edx
                inc ecx
                or eax,eax
                jnz IToA_loop1
IToA_loop2:
                pop eax
                mov [edi],al
                inc edi
                loop IToA_loop2
        xor al,al
        mov [edi],al
        ret
IToA endp


;--------------------------------
;UToA (i:dword,s:addr)
;       Zamienia liczbe (DWORD) na string (ASCIIZ)
;       Wejscie: i - liczba
;                s - string (adres)
;--------------------------------

UToA proc stdcall uses edi edx ecx ebx eax,i:dword,s:dword
        mov edi,s
        mov eax,i
        mov ebx,10
        xor ecx,ecx
        ;
UToA_loop1:
                xor edx,edx
                idiv ebx
                add dl,'0'
                push edx
                inc ecx
                or eax,eax
                jnz UToA_loop1
UToA_loop2:
                pop eax
                mov [edi],al
                inc edi
                loop UToA_loop2
        xor al,al
        mov [edi],al
        ret
UToA endp

;--------------------------------
;HToA (h:dword,s:addr,upcase:dword)
;       Zamienia liczbe (DWORD) na string (ASCIIZ) w hex malymi literami lub duzymi
;       Wejscie: h - liczba
;                s - string (adres)
;                upcase - =0-male litery,=1-duze
;--------------------------------

__htoatab db '0123456789abcdef'
          db '0123456789ABCDEF'

HToA proc stdcall uses eax ebx ecx edx edi,h:dword,s:dword,upcase:dword
        mov edi,s
        mov eax,h
        mov ebx,upcase
        and ebx,1
        shl ebx,4
        add ebx,offset __htoatab
        xor ecx,ecx
HToA_loop1:
                mov edx,eax
                and eax,0fh
                xlatb
                push eax
                mov eax,edx
                shr eax,4
                inc ecx
                or eax,eax
                jnz HToA_loop1
HToA_loop2:
                pop eax
                mov [edi],al
                inc edi
                loop HToA_loop2
        xor al,al
        mov [edi],al
        ret
HToA endp

;--------------------------------
;AToI (s:addr) : dword
;       Zamienia string (ASCIIZ) na liczbe (SDWORD)
;       Wejscie: s - string (adres)
;       Wyjscie: eax - liczba (SDWORD)
;--------------------------------

AToI proc stdcall uses esi edx ecx ebx,s:dword
        mov esi,s
        ;
        mov cl,[esi]
        or cl,cl
        jz AToI_end1
        ;
        cmp cl,'-'
        jne AToI_next1
        inc esi
        ;
AToI_next1:
        mov ebx,10
        xor eax,eax
        xor ecx,ecx
AToI_loop1:
                mov cl,[esi]
                sub cl,'0'
                add eax,ecx
                mov cl,[esi+1]
                or cl,cl
                jz AToI_break1
                mul ebx
                inc esi
                jmp AToI_loop1
AToI_break1:
        mov esi,s
        mov cl,[esi]
        cmp cl,'-'
        jne AToI_end1
        neg eax
AToI_end1:
        ret
AToI endp

;--------------------------------
;AToU (s:addr)
;       Zamiena string (ASCIIZ) na liczba (DWORD)
;       Wejscie: s - adres na string
;       Wyjscie: eax - liczba po zamianie
;--------------------------------

AToU proc stdcall uses esi edx ecx ebx,s:dword
        mov esi,s
        mov cl,[esi]
        or cl,cl
        jz AToU_endp1
        mov ebx,10
        xor eax,eax
        xor ecx,ecx
AToU_loop1:
                mov cl,[esi]
                sub cl,'0'
                add eax,ecx
                mov cl,[esi+1]
                or cl,cl
                jz AToU_endp1
                mul ebx
                inc esi
                jmp AToU_loop1
AToU_endp1:
        ret
AToU endp

;--------------------------------
;AToH (s:addr)
;       Zamienia string (ASCIIZ) na cyfre w hex
;       Wejscie: s - string (adres)
;       Wyjscie: eax - liczba
;--------------------------------

AToH proc stdcall uses ecx esi s:dword
        mov esi,s
        mov al,[esi]
        or al,al
        jz AToH_endp
        xor eax,eax
        xor ecx,ecx
AToH_loop1:
                mov cl,[esi]
                cmp cl,'a'
                jae AToH_char_a_d
                cmp cl,'A'
                jae AToH_char_a_u
                ;
                sub cl,'0'
                jmp AToH_put_it
AToH_char_a_d:
                sub cl,'a'-10
                jmp AToH_put_it
AToH_char_a_u:
                sub cl,'A'-10
AToH_put_it:
                or eax,ecx
                mov cl,[esi+1]
                or cl,cl
                jz AToH_endp
                shl eax,4
                inc esi
                jmp AToH_loop1
AToH_endp:
        ret
AToH endp

ENDIF
