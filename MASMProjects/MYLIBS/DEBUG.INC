IFNDEF __DEBUG_INC
__DEBUG_INC=1
;DEBUG.INC by Kamil Korolkiewicz 2005
;Makra i procedury ulatwiajace debugowanie i obsluge programow

include mstdlib\sprintf.inc
include mstdlib\mcon.inc

Debug_buf db 1024 dup(0)

;makra

Debug_rdtsc macro z:req,t:req
        push eax
        push edx
        rdtsc
        if t eq 0
        mov dword ptr z,eax
        mov dword ptr z+4,edx
        else
        sub eax,dword ptr z
        sbb edx,dword ptr z+4
        mov dword ptr z,eax
        mov dword ptr z+4,edx
        endif
        pop edx
        pop eax
endm

Debug_Print macro x:req,y:req,t:req,v:vararg
        local txt
        jmp @f
        txt db t
        db 0
@@:
        invoke SPrintf,addr Debug_buf,addr txt,v
        invoke Print,x,y,addr Debug_buf
endm

ENDIF
