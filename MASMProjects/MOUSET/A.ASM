.386
.listall

public main

stub32 segment public
assume cs:stub32,ds:stub32

;kod

include dpmi.inc
include mstdlib\mcon.inc
include mstdlib\aiconv.inc
include mstdlib\sprintf.inc
include mouse.inc
include mstdlib\mmem.inc
include mstdlib\mmemmak.inc
include vga.inc
include vesa.inc
include vgatext.inc
include surface.inc
include surface8c.inc
include kb.inc

;dane

_0 equ 19
_1 equ 30

curs db _0,_0, 0, 0, 0, 0, 0, 0, 0, 0
     db _0,_1,_0, 0, 0, 0, 0, 0, 0, 0
     db _0,_1,_1,_0, 0, 0, 0, 0, 0, 0
     db _0,_1,_1,_1,_0, 0, 0, 0, 0, 0
     db _0,_1,_1,_1,_1,_0, 0, 0, 0, 0
     db _0,_1,_1,_1,_1,_1,_0, 0, 0, 0
     db _0,_1,_1,_1,_1,_1,_1,_0, 0, 0
     db _0,_1,_1,_1,_1,_1,_1,_1,_0, 0
     db _0,_1,_1,_1,_1,_1,_1,_1,_1,_0
     db _0,_1,_1,_1,_1,_1,_1,_1,_1,_0
     db _0,_0,_0,_0,_1,_1,_1,_0,_0,_0
     db  0, 0, 0, 0,_0,_1,_1,_1,_0, 0
     db  0, 0, 0, 0, 0,_0,_1,_1,_1,_0
     db  0, 0, 0, 0, 0, 0,_0,_0,_0,_0

scrx equ 640
scry equ 480
scra equ scrx*scry
mode equ V640X480X256

;scrx equ 320
;scry equ 200
;scra equ scrx*scry
;mode equ 13h

mst TMouseState <>
bf dd 0
lfb dd 0
txt db 256 dup (0)

curc db 0

cols dd 1

ssrfc TSurface <scrx,scry,1,0,0>

cursrfc TSurface <10,14,1,0,0>
cre TRect <0,0,10*8,14*8>
cree TRect <0,0,10,14>

tx1 db 'X:%u',0
tx2 db 'Y:%u',0
tx3 db 'color:%u',0
tx4 db 'kol:%u',0

main:

        invoke InitHeapMem,scra*4
        mov eax,offset ssrfc
        call AllocSurface
        invoke AllocMem,scra
        mov bf,eax
        mov ebx,eax
        FillMemB ebx,scra,9
        mov ebx,ssrfc.SBits
        FillMemB ebx,scra,0
        mov ebx,offset curs
        add ebx,stub32baseaddress
        mov cursrfc.SBits,ebx
        ;SetVGAMode
        SetVESAMode mode,0
        ;SetVESAMode 13h,0
        mov cx,mode
        call GetLFB
        mov lfb,eax

        call InitKB
        call InitMouse
        invoke SetMouseCursorPoz,scrx/2,scry/2
        invoke SetMouseCursorRect,0,0,scrx-1,scry-1
;        invoke ShowMouseCursor,MOUSE_SHOW_CURSOR
l:
        invoke GetMouseState,addr mst
        test mst.mouse_key,MOUSE_KEY_RIGHT
        jnz q
        mov ebx,ssrfc.SBits
        FillMemB ebx,scra,0
        movzx eax,mst.mouse_y
        mov ebx,scrx
        mul ebx
        movzx ebx,mst.mouse_x
        add ebx,eax
        mov al,curc
        push ebx
        test mst.mouse_key,MOUSE_KEY_LEFT
        jz qq
        add ebx,bf
        mov ecx,cols
lddd:
        mov fs:[ebx],al
        inc ebx
        inc al
        loop lddd
qq:
        mov ebx,bf
        mov ebp,ssrfc.SBits
        push ds
        push fs
        pop ds
        CopyMemD ebx,ebp,scra
        pop ds
        mov ebp,esp
        movzx ebx,mst.mouse_x
        push ebx
        push offset tx1
        push offset txt
        call SPrintf
        mov esp,ebp
        mov eax,0
        mov ebx,0
        mov ecx,20
        mov edx,EFECTS_FADE
        mov esi,offset txt
        mov edi,offset ssrfc
        call DrawString8
        mov ebp,esp

        movzx ebx,mst.mouse_y
        push ebx
        push offset tx2
        push offset txt
        call SPrintf
        mov esp,ebp
        mov eax,0
        mov ebx,10
        mov ecx,20
        mov edx,EFECTS_FADE
        mov esi,offset txt
        mov edi,offset ssrfc
        call DrawString8
        mov ebp,esp

        movzx ebx,curc
        push ebx
        push offset tx3
        push offset txt
        call SPrintf
        mov esp,ebp
        mov eax,0
        mov ebx,20
        mov ecx,20
        mov edx,EFECTS_FADE
        mov esi,offset txt
        mov edi,offset ssrfc
        call DrawString8
        mov ebp,esp

        mov ebp,esp
        mov ebx,cols
        push ebx
        push offset tx4
        push offset txt
        call SPrintf
        mov esp,ebp
        mov eax,0
        mov ebx,30
        mov ecx,20
        mov edx,EFECTS_FADE
        mov esi,offset txt
        mov edi,offset ssrfc
        call DrawString8
        mov ebp,esp

        pop ebx
        ;add ebx,ssrfc.SBits
        ;mov byte ptr fs:[ebx],_1
        movzx ebx,mst.mouse_x
        mov cre.RX,ebx
        movzx ebx,mst.mouse_y
        mov cre.RY,ebx
        mov eax,offset cree
        mov ebx,offset cre
        mov esi,offset cursrfc
        mov edi,offset ssrfc
        call DrawSurface8CS
        call Wait4VSync
        mov ebx,ssrfc.SBits
        mov ebp,lfb
        push ds
        push fs
        pop ds
        CopyMemD ebx,ebp,scra
        pop ds

        IsKeyPressed KEY_LEFT
        jz llll000
        dec curc
llll000:
        IsKeyPressed KEY_RIGHT
        jz llll001
        inc curc
llll001:
        IsKeyPressed KEY_UP
        jz llll002
        inc cols
llll002:
        IsKeyPressed KEY_DOWN
        jz llll003
        dec cols
llll003:
        mov eax,cols
        and eax,511
        or eax,eax
        jnz llll004
        mov eax,1
llll004:
        mov cols,eax
        jmp l
q:
        invoke ShowMouseCursor,MOUSE_HIDE_CURSOR
        call CloseKB
        SetTxtMode
        mov ax,4c00h
        int 21h

stub32 ends

end
