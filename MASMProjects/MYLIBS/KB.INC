IFNDEF __KB_INC
__KB_INC=1
;KB.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Modul do obslugi klawiatury.
;gs musi byc stub32zerosel

;stale klawiszy

KEY_UP          equ 72
KEY_DOWN        equ 80
KEY_LEFT        equ 75
KEY_RIGHT       equ 77
KEY_INSERT      equ 82
KEY_DELETE      equ 83
KEY_HOME        equ 71
KEY_END         equ 79
KEY_PAGEUP      equ 73
KEY_PAGEDOWN    equ 81
KEY_ESC         equ 1
KEY_SPACE       equ 57
KEY_LSHIFT      equ 42
KEY_RSHIFT      equ 54
KEY_CTRL        equ 29
KEY_ALT         equ 56
KEY_TAB         equ 15
KEY_ENTER       equ 28
KEY_BACKSPACE   equ 14
KEY_F1          equ 59
KEY_1           equ 2

;dane

__OldKBIntVec fword 0
__KBKeyTab db 128 dup (0)

;Procedura do obslugi klawiatury

__Int09_Keyboard_Handler:
        push eax
        push edx
        push ds
        cli
__I09MS:mov ax,0
        mov ds,ax
        xor eax,eax
        in al,60h                       ;al=scan kod+pressed?
        mov dl,al                       ;dl=scan kod+pressed?
        and eax,127                     ;al=scan kod
        shr dl,7                        ;dl=pressed?
        xor dl,1                        ;dl=not pressed?
        mov [__KBKeyTab+eax],dl
        mov al,20h
        out 20h,al
        sti
        pop ds
        pop edx
        pop eax
        iretd

;makra

;--------------------------------
;IsKeyPressed key
;       Sprawdza czy klawisz jest wcisniety
;       Wyjscie:zf=0 nie/zf=1 tak
;--------------------------------
IsKeyPressed macro key
        mov al,[__KBKeyTab+key]
        or al,al
endm

;--------------------------------
;IsAnyKeyPressed
;       Sprawdza czy jakis klawisz jest wcisniety
;       Wyjscie:eax=0-tak,eax=1-nie
;--------------------------------
IsAnyKeyPressed macro key
        push ebx
        xor eax,eax
@@:
        cmp al,128
        je @f
        mov bl,[__KBKeyTab+eax]
        inc eax
        or bl,bl
        jz @b
@@:
        pop ebx
        shr eax,7
endm


;Funkcje

;--------------------------------
;InitKB ()
;       Inicjuje obluge klawiatury
;       Wyjscie: eax=1 ok/eax=0 blad
;--------------------------------

InitKB proc stdcall uses ebx ecx edx edi
        mov edi,offset __KBKeyTab
        push ds
        pop es
        mov ecx,128/4
        xor eax,eax
        rep stosd
        GetIntVec 9h
        mov word ptr __OldKBIntVec,cx
        mov dword ptr __OldKBIntVec+2,edx
        mov ax,stub32datasel
        mov edx,offset __I09MS+2
        mov [edx],ax
        SetIntVec 9h,cs,offset __Int09_Keyboard_Handler
        pushfd
        pop eax
        and eax,1
        xor eax,1
        ret
InitKB endp

;--------------------------------
;CloseKB ()
;       Zamyka obsluge klawiatury
;       Wyjscie: eax=1 ok/eax=0 blad
;--------------------------------

CloseKB proc stdcall uses ebx ecx edx
        SetIntVec 9h,word ptr __OldKBIntVec,dword ptr __OldKBIntVec+2
        pushfd
        pop eax
        and eax,1
        xor eax,1
        ret
CloseKB endp

ENDIF
