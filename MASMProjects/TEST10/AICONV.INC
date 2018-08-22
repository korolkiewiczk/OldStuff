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

IToA proc stdcall uses edi edx ecx ebx,i:sdword,s:dword
        cld
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

UToA proc stdcall uses edi edx ecx ebx,i:dword,s:dword
        cld
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
AtoI_loop1:
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

ENDIF
