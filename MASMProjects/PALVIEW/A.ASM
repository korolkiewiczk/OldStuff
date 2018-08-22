.386
.listall

public main

stub32 segment public
assume cs:stub32,ds:stub32

;kod

include dpmi.inc
include mstdlib\mcon.inc
include mstdlib\mmem.inc
include mstdlib\params.inc
include vga.inc
include bmp8.inc
include vgatext.inc
include mstdlib\aiconv.inc
include surface.inc


se TSurface <320,200,1,320,0a0000h>

fn db 127 dup (0)
pal dd 0

col dd 0
cc dd 0

_10 dd 10
_3200 dd 3200

bf db 100 dup (0)

rc dd 0
gc dd 0
bc dd 0

main:


        call GetParamCount
        or eax,eax
        jz q

        mov al,1
        mov esi,offset fn
        call GetParam

        invoke InitHeapMem,800

        invoke AllocMem,768
        mov pal,eax

        invoke LoadPalFromBMP,addr fn,pal

        SetVGAMode

        invoke SetPalette,pal

mloop:
        mov eax,0
        mov esi,offset se
        call FillSurface
        xor eax,eax
        mov ebx,256
        mov eax,col
        xor edx,edx
        div ebx
        mov col,edx

        mov ecx,256/32
        mov es,stub32zerosel
        xor ebx,ebx
        mov cc,0
dp1:
                push ecx
                mov ecx,32
                xor edx,edx
dp2:
                        push ecx
                        mov ecx,10
                        mov edi,0a0000h
                        push edx
                        mov eax,ebx
                        mul _3200
                        add edi,eax
                        pop edx
                        push edx
                        mov eax,edx
                        mul _10
                        pop edx
                        add edi,eax
dp3:
                                push ecx
                                mov ecx,10
                                mov eax,cc
                                rep stosb
                                add edi,320-10
                                pop ecx
                                loop dp3
                        inc cc
                        inc edx
                        pop ecx
                        loop dp2
                inc ebx
                pop ecx
                loop dp1
        mov edi,0a0000h
        mov eax,col
        mov ebx,32
        xor edx,edx
        div ebx
        mul _3200
        add edi,eax
        mov eax,col
        xor edx,edx
        mov ebx,32
        div ebx
        mov eax,edx
        mul _10
        add edi,eax
        mov byte ptr es:[edi],128

        mov eax,col
        call GetColor
        mov byte ptr rc,al
        mov byte ptr gc,ah
        mov byte ptr bc,bl       

        invoke UToA,col,addr bf
        mov eax,0
        mov ebx,170
        mov ecx,128
        mov edx,0
        mov esi,offset bf
        mov edi,offset se
        call DrawString8

        invoke UToA,rc,addr bf
        mov eax,0
        mov ebx,182
        mov ecx,128
        mov edx,0
        mov esi,offset bf
        mov edi,offset se
        call DrawString8

        invoke UToA,gc,addr bf
        mov eax,30
        mov ebx,182
        mov ecx,128
        mov edx,0
        mov esi,offset bf
        mov edi,offset se
        call DrawString8

        invoke UToA,bc,addr bf
        mov eax,60
        mov ebx,182
        mov ecx,128
        mov edx,0
        mov esi,offset bf
        mov edi,offset se
        call DrawString8


        ReadKey
        cmp ah,72
        jne d1
        sub col,32
d1:
        cmp ah,80
        jne d2
        add col,32
d2:
        cmp ah,75
        jne d3
        dec col
d3:
        cmp ah,77
        jne d4
        inc col
d4:
        cmp ah,1
        je qq


        jmp mloop
qq:
        call FreeHeapMem
q:

        SetTXTMode
        mov ax,4c00h
        int 21h

stub32 ends

end
