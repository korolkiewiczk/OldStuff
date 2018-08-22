.386
public main
stub32 segment public
assume cs:stub32,ds:stub32
include dpmi.inc
include kb.inc
include aiconv.inc
include mcon.inc

pc db 8 dup (0)
z db '   ',0

Printcode:
        invoke UToA,eax,addr pc
        invoke Print,0,CON_CUR_Y,addr z
        invoke Print,0,CON_CUR_Y,addr pc
        inc CON_CUR_Y
        ret
main:
        call InitKB
pl:
        IsKeyPressed 1
        jnz q
        mov CON_CUR_Y,0
        invoke Print,0,CON_CUR_Y,addr z
        mov ecx,128
        mov esi,offset __KBKeyTab
s:
        mov al,[esi]
        cmp al,0
        je d
        mov eax,128
        sub eax,ecx
        call Printcode
d:
        inc esi
        loop s
        jmp pl
q:
        call CloseKB
        mov ax,4c00h
        int 21h
stub32 ends

end
