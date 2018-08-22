.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;stale

BUF_SIZE equ 1024

;dane

buffer db BUF_SIZE dup (?)
db 0
par db 100 dup (?)

fhandle dw 0

;kod

include dpmi.inc
include mstdlib\fileio.inc
include mstdlib\mcon.inc
include mstdlib\params.inc

main:

        call GetParamCount
        or eax,eax
        jz qp
        mov al,1
        mov esi,offset par
        call GetParam
        invoke FOpen,addr par,OPEN_ONLY,READ_ONLY
        jc qp
        mov fhandle,ax
        invoke FRead,fhandle,BUF_SIZE,addr buffer
        invoke Print,0,0,addr buffer
        invoke FClose,fhandle
qp:

        mov ax,4c00h
        int 21h

stub32 ends

end
