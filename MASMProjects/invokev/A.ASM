.386

public main

stub32 segment public
assume cs:stub32,ds:stub32,ss:program_stack

;dane

txt1 db 'Kamil\nKamil\nKamil\tKamil\n',0
txt2 db 'a=%u',0
txt3 db '-1=%i 2=%u',0
txt4 db '%1x %1X %s %0 %% % % \f\f\d\',0
txt5 db 10 dup (0)
txt6 db 'AbCdEf12',0
txt7 db '12321AcA',0
txt8 db 'eax=%08X ebx=%08X ecx=%08X edx=%08X edi=%08X esi=%08X esp=%08X\ncs=%04X ds=%04X ss=%010X es=%04X fs=%04X gs=%04X\n',0
txt9 db '%u',0
t dd 0
b dd 128 dup (?)

;kod

include dpmi.inc
include mstdlib\mcon.inc
include mstdlib\aiconv.inc
include mstdlib\sprintf.inc
include mstdlib\mtime.inc
include mstdlib\mmemmak.inc

;dane

tm Time <>

main:

        call Cls
        invoke SPrintf,addr b,addr txt1
        invoke Print,0,0,addr b
        invoke SPrintf,addr b,addr txt2,1
        invoke Print,0,3,addr b
        invoke SPrintf,addr b,addr txt3,-1,2
        invoke Print,0,4,addr b
        invoke SPrintf,addr b,addr txt4,addr txt1,addr txt2,addr txt3
        invoke Print,0,5,addr b
        invoke HToA,9abcdefh,addr txt5,0
        invoke Print,0,6,addr txt5
        invoke HToA,0abcdefh,addr txt5,1
        invoke Print,0,7,addr txt5

        invoke AToH,addr txt6
        invoke HToA,eax,addr txt5,0
        invoke Print,0,8,addr txt5
        invoke AToH,addr txt7
        invoke HToA,eax,addr txt5,0
        invoke Print,0,9,addr txt5

        push gs
        push fs
        push ss
        push es
        push ds
        push cs
        invoke SPrintf,addr b,addr txt8,eax,ebx,ecx,edx,edi,esi,esp
        add esp,18h
        invoke Print,0,10,addr b

        call InitTime
        invoke SPrintf,addr b,addr txt9,__CPU__TSC__Freq
        invoke Print,0,13,addr b
        push ds
        pop es
        int 3
        mov eax,offset tm
        mov ebx,1
        call InitTimeStruct
        mov ecx,1000*1000*1000
p:      loop p
        call GetTime
        int 3
        fistp t
        invoke UToA,t,addr b
        invoke Print,0,14,addr b

        mov ax,4c00h
        int 21h

stub32 ends

program_stack segment stack
db 64000 dup (?)
program_stack ends

end
