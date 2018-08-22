IFNDEF __KB_INC
__KB_INC=1
;KB.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Modul do obslugi klawiatury.

;dane

__OldKBIntVec fword 0
__KBKeyTab db 128 dup (0)

;Procedura do obslugi klawiatury

__Int09_Keyboard_Handler proc uses eax edx
        cli
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
        iretd
__Int09_Keyboard_Handler endp

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

;Funkcje

;--------------------------------
;InitKB ()
;       Inicjuje obluge klawiatury
;       Wyjscie: eax=1 ok/eax=0 blad
;--------------------------------

InitKB proc stdcall uses bx cx edx
        GetIntVec 9h
        mov word ptr __OldKBIntVec,cx
        mov dword ptr __OldKBIntVec+2,edx
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

CloseKB proc stdcall uses bx cx edx
        SetIntVec 9h,word ptr __OldKBIntVec,dword ptr __OldKBIntVec+2
        pushfd
        pop eax
        and eax,1
        xor eax,1
        ret
CloseKB endp

ENDIF
