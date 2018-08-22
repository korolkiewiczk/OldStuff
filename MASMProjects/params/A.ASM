.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;dane

l dq 0
param db 128 dup ('i')

;kod

include dpmi.inc
include mstdlib\mcon.inc
include mstdlib\aiconv.inc
include mstdlib\params.inc

main:

        call GetParamCount
        push eax
        invoke UToA,eax,addr l
        call Cls
        invoke Print,0,0,addr l
        pop ecx
        or ecx,ecx
        jz q
        xor dl,dl
        inc dl
d:
        int 3
        mov al,dl
        mov esi,offset param
        call GetParam
        invoke Print,0,dl,addr param
        inc dl
        loop d
q:
        mov ax,4c00h
        int 21h

stub32 ends

end
