IFNDEF __MMEMMAK_INC
__MMEMMAK_INC=1
;MMEMMAK.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Makra do obslugi pamieci

;Makra

;Kopiuje mems bajtow z ofs1 do ofs2
;Rej:ebx,ecx,esi,edi

CopyMemD macro ofs1,ofs2,mems
        cld
        mov esi,ofs1
        mov edi,ofs2
        mov ecx,mems
        mov ebx,ecx
        shr ecx,2
        and ebx,3
        rep movsd
        mov ecx,ebx
        rep movsb
endm

;Wypelnia mems bajtow wartoscia pat (byte) bufor ofs1
;Rej:eax,ebx,ecx,esi

FillMemB macro ofs1,mems,pat
        cld
        mov edi,ofs1
        mov al,pat
        mov ah,al
        mov bx,ax
        shl eax,16
        mov ax,bx
        mov ecx,mems
        mov ebx,ecx
        shr ecx,2
        and ebx,3
        rep stosd
        mov ecx,ebx
        rep stosb
endm

;Wypelnia mems bajtow wartoscia pat (dword) bufor ofs1
;Rej:eax,ebx,ecx,esi

FillMemD macro ofs1,mems,pat
        cld
        mov edi,ofs1
        mov eax,pat
        mov ecx,mems
        mov ebx,ecx
        shr ecx,2
        and ebx,3
        rep stosd
        mov ecx,ebx
        rep stosb
endm

ENDIF
