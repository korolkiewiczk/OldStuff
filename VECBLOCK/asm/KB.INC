;KB.INC for DJGPP by Kamil Korolkiewicz 2005/2006
;Modul do obslugi klawiatury.
comment/
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
KEY_1           equ 2/

;dane

.data

key_scantokeytab db 128 dup(0)
key_keytab db 128 dup (0)
key_isshiftp db 0

public key_scantokeytab,key_keytab,key_isshiftp

.code

extern key_scantokeytab:byte,key_keytab:byte,key_isshiftp:byte

;Procedura do obslugi klawiatury

__Int09_Keyboard_Handler proc C
        push eax
        push edx
        push ds
        cli
        xor eax,eax
        in al,60h                       ;al=scan kod+pressed?
        mov dl,al                       ;dl=scan kod+pressed?
        and eax,127                     ;al=scan kod
        shr dl,7                        ;dl=pressed?
        xor dl,1                        ;dl=not pressed?
        cmp eax,42
        jne @f
        mov key_isshiftp,dl
        jmp quit
@@:
        mov al,[key_scantokeytab+eax]
        mov [key_keytab+eax],dl
quit:
        mov al,20h
        out 20h,al
        sti
        pop ds
        pop edx
        pop eax
        iretd
__Int09_Keyboard_Handler endp
