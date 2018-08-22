IFNDEF __STRINGS_INC
__STRINGS_INC=1
;STRINGS.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do obslugi stringow (lancuchow) ASCIIZ

;funkcje

;--------------------------------
;StrLen (str1:dword)
;       Pobiera dlugosc lancucha
;       Wyjscie: eax- dlugosc
;--------------------------------

StrLen proc stdcall uses edx esi,str1:dword
        mov esi,str1
        xor eax,eax
StrLen_count:
                mov dl,[esi]
                inc eax
                inc esi
                or dl,dl
                jnz StrLen_count
        dec eax
        ret
StrLen endp

;--------------------------------
;StrCat (str1,str2:dword)
;       Laczy lancuchy str1 oraz str2 i zapisuje go do str1
;       Wejscie: str1-I lancuch
;                str1-II lancuch
;       Wyjscie: zmieniony str1 oraz eax-dlugosc laczna
;--------------------------------

StrCat proc stdcall uses ebx ecx edx esi edi,str1:dword,str2:dword
        cld
        push ds
        pop es
        mov edi,str1
        mov esi,str2
        invoke StrLen,str1
        add edi,eax
        mov edx,eax
        invoke StrLen,str2
        add edx,eax
        inc eax
        mov ecx,eax
        mov ebx,ecx
        shr ecx,2
        and ebx,3
        rep movsd
        mov ecx,ebx
        rep movsb
        mov eax,edx
        ret
StrCat endp

ENDIF
