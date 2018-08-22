.386

public main

stub32 segment public use32
assume cs:stub32,ds:stub32
nap db 'Kamil',10,13,'$'
modv db 0
main:
        mov ax,0900h
        mov edx,offset nap
        int 21h
        mov ax,0700h
        int 21h
        mov ax,13h
        int 10h
        lp:
                mov edi,0A0000h
                mov ecx,64000/4
                mov al,modv
                mov ah,modv
                shl eax,16
                mov al,modv
                mov ah,modv
                rep stosd
                inc modv
                kp:
                mov ax,0
                int 16h
                cmp ah,1
                je pe
                cmp ah,2
                jne kp
                jmp lp
        pe:
        mov ax,0700h
        int 21h
        mov ax,3h
        int 10h
        mov ebp,esp
        mov byte ptr [ebp],0
        mov ax,4c00h
        int 21h
stub32 ends

end
