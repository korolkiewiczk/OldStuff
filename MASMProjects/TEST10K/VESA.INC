IFNDEF __VESA_INC
__VESA_INC=1
;VESA.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Procedury i makra do obslugi VESA

;stale

;lista trybow VESA

;VESA v1.0
V640x400x256            equ     100h
V640x480x256            equ     101h
V800x600x256            equ     103h
V1024x768x256           equ     105h
V1280x1024x256          equ     107h
V80x60                  equ     108h
V132x25                 equ     109h
V132x43                 equ     10ah
V132x50                 equ     10bh
V132x60                 equ     10ch
;VESA v1.2
V320x200x32K            equ     10dh
V320x200x64K            equ     10eh
V320x200x16M            equ     10fh
V640x480x32K            equ     110h
V640x480x64K            equ     111h
V640x480x16M            equ     112h
V800x600x32K            equ     113h
V800x600x64K            equ     114h
V800x600x16M            equ     115h
V1024x768x32K           equ     116h
V1024x768x64K           equ     117h
V1024x768x16M           equ     118h
V1280x1024x32K          equ     119h
V1280x1024x64K          equ     11ah
V1280x1024x16M          equ     11bh

;makra

;Makra zwracaja blad jesli ah=1. Jesli funkcja jest dostepna to al=4f

;--------------------------------
;SetVESAMode mode,attr
;       Makro ustawia tryb VESA z atrybutami
;       Uzywane rejestry: ax,bx
;       Wejscie: mode-tryb graficzny VESA
;                attr-atrybuty (14-bit=1-uzywaj LFB)
;--------------------------------

SetVESAMode macro mode,attr
        mov ax,4f02h
        mov bx,mode
        or bx,attr
        int 10h
endm

;--------------------------------
;GetCurrVESAMode
;       Pobiera aktualny tryb graficzny VESA
;       Uzywane rejestry: ax
;       Wyjscie: bx-tryb
;--------------------------------

GetCurrVESAMode macro
        mov ax,4f03h
        int 10h
endm

;funkcje

;--------------------------------
;GetModeInfo (cx,edi)
;       Pobiera informacje o danym trybie graficznym do bufora (256b)
;       Wejscie: cx-tryb VESA
;                edi-offset do bufora
;       Wyjscie: eax=0 blad/eax=1 ok
;       Uwaga: offset do bufora (edi) musi byc < 65536
;--------------------------------

GetModeInfo proc stdcall uses es
        cmp edi,10000h
        jae GetModeInfo_error
        mov ax,4f01h
        int 10h
        or ah,ah
        jz GetModeInfo_ok
GetModeInfo_error:
        xor eax,eax
        ret
GetModeInfo_ok:
        xor eax,eax
        inc eax
        ret
GetModeInfo endp

;--------------------------------
;GetVESAInfo (edi)
;       Pobiera informacje o VESA do bufora (512b)
;       Wejscie: edi-offset bufora
;       Wyjscie: eax=0 blad/eax=1 ok
;       Uwaga: offset do bufora (edi) musi byc < 65536
;--------------------------------

GetVESAInfo proc stdcall uses es
        cmp edi,10000h
        jae GetVESAInfo_error
        int 10h
        xor ah,ah
        jz GetVESAInfo_ok
GetVESAInfo_error:
        xor eax,eax
        ret
GetVESAInfo_ok:
        xor eax,eax
        inc eax
        ret
GetVESAInfo endp

;--------------------------------
;GetLFB (cx)
;       Pobiera adres liniowy do linear frame buffer dla danego trybu
;       Wejscie: cx-tryb VESA
;       Wyjscie: eax<>0-eax=LFB/eax=FFFFFFFFh blad
;       Uwaga: Wystarczy aby procedura byla wywolana tylko raz
;--------------------------------

GetLFB proc stdcall uses edi ebx ecx esi
        local moderec[256]:byte
        mov edi,ebp
        sub edi,256
        push stub32saveds
        mov ax,stub32savess
        mov stub32saveds,ax
        call GetModeInfo
        or eax,eax
        jz GetLFB_error
        pop stub32saveds
        MapPhysAddr dword ptr moderec[28h],1600*1200*4
        jnc GetLFB_ok
GetLFB_error:
        xor eax,eax
        dec eax
        jmp GetLFB_end
GetLFB_ok:
        mov eax,ebx
GetLFB_end:
        ret
GetLFB endp

ENDIF
