IFNDEF __MOUSE_INC
__MOUSE_INC=1
;MOUSE.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do obslugi myszki

;struktury

TMouseState struct
        mouse_x dw 0
        mouse_y dw 0
        mouse_key db 0
TMouseState ends

;stale

MOUSE_SHOW_CURSOR equ 1
MOUSE_HIDE_CURSOR equ 0
MOUSE_KEY_LEFT equ 1
MOUSE_KEY_RIGHT equ 2
MOUSE_KEY_CENTER equ 4

;dane

__IS__INT33H__MOUSE dd 0

;funkcje

;--------------------------------
;InitMouse ()
;       Inicjuje myszke
;       Wyjscie: eax=0 blad/eax=1 ok
;--------------------------------

InitMouse proc stdcall uses ebx
        xor eax,eax
        int 33h
        or eax,eax
        jz InitMouse_endp
        push 1
        pop eax
InitMouse_endp:
        mov __IS__INT33H__MOUSE,eax
        ret
InitMouse endp

;--------------------------------
;ShowMouseCursor (state:dword)
;       Wyswietla lub chowa kursor myszy
;       Wejscie: state=0 ukryj kursor myszy,state<>0 pokaz kursor myszy
;--------------------------------

ShowMouseCursor proc stdcall uses eax,state:dword
        mov eax,__IS__INT33H__MOUSE
        or eax,eax
        jz ShowMouseCursor_endp
        mov eax,state
        or eax,eax
        jz ShowMouseCursor_hide
        mov eax,1
        int 33h
        jmp ShowMouseCursor_endp
ShowMouseCursor_hide:
        mov eax,2
        int 33h
ShowMouseCursor_endp:
        ret
ShowMouseCursor endp

;--------------------------------
;GetMouseState (s:TMouseState)
;       Pobiera status myszy do s:TMouseState
;       Wejscie: s-adres do struktury TMouseState
;       Wyjscie: zmieniona struktora s
;--------------------------------

GetMouseState proc stdcall uses eax ebx ecx edx,s:dword
        mov eax,__IS__INT33H__MOUSE
        or eax,eax
        jz GetMouseState_endp
        mov ax,3
        int 33h
        mov eax,s
        assume eax:ptr TMouseState
        mov [eax].mouse_key,bl
        mov [eax].mouse_x,cx
        mov [eax].mouse_y,dx
GetMouseState_endp:
        ret
        assume eax:nothing
GetMouseState endp

;--------------------------------
;SetMouseCursorPoz (x:dword,y:dword)
;       Ustawia pozycje kursora myszy
;       Wejscie: x-pozycja x
;                y-pozycja y
;--------------------------------

SetMouseCursorPoz proc stdcall uses eax ecx edx,x:dword,y:dword
        mov eax,__IS__INT33H__MOUSE
        or eax,eax
        jz SetMouseCursorPoz_endp
        mov ax,4
        mov ecx,x
        mov edx,y
        int 33h
SetMouseCursorPoz_endp:
        ret
SetMouseCursorPoz endp

;--------------------------------
;SetMouseCursorRect (x1,y1,x2,y2:dword)
;       Ustawia prostokat po ktorym porusza sie po ekranie
;       Wejscie: x1,y1,x2,y2-wpolrzedne prostokata
;--------------------------------

SetMouseCursorRect proc stdcall uses eax ecx edx,x1:dword,y1:dword,x2:dword,y2:dword
        mov eax,__IS__INT33H__MOUSE
        or eax,eax
        jz SetMouseCursorRect_endp
        mov ax,7
        mov ecx,x1
        mov edx,x2
        int 33h
        mov ax,8
        mov ecx,y1
        mov edx,y2
        int 33h
SetMouseCursorRect_endp:
        ret
SetMouseCursorRect endp

ENDIF
