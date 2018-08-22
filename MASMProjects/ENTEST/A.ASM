.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;kod

include dpmi.inc
include Engine\Engine.asm
include mstdlib\mmemmak.inc
include vga.inc
include kb.inc

;dane

Vert EnVertex <0.0,0.0,0.0,0,1,0,0>
Vert2 EnVertex <>

pp dd 0

__50 dd 50.0
__0 dd 0.0

sx dd 0.0
sy dd 0.0
sz dd 120.0

__1 dd 1.0

main:
        call InitSinCosTab

        call InitKB
        SetVGAMode

        mov ecx,256
        xor eax,eax
pl:
        push eax
        push 0
        push 0
        call SetColor
        inc eax
        loop pl

bll:
        fld __50
        fstp Vert.z
        fld __0
        fst Vert.y
        fstp Vert.x
        mov ecx,0
lp:
        push ecx
        mov eax,offset Vert
        mov ebx,offset Vert2
        mov ecx,1
        call CopyVertexList
        pop ecx
        mov eax,offset Vert2
        mov ebx,ecx
        ;call RotateVertexX
        call RotateVertexY
        ;call RotateVertexZ
        fld sz
        fld sy
        fld sx
        call ShiftVertex
        mov eax,offset Vert2
        call ConvertVertex3DTo2D
        fld Vert2.x
        fistp pp
        mov eax,pp
        fld Vert2.y
        fistp pp
        mov ebx,pp
        fld Vert2.z
        fistp pp
        mov edx,pp
        call putpixel
        inc ecx
        cmp ecx,360
        jne lp

        fld __50
        fstp Vert.z
        fld __0
        fst Vert.x
        fstp Vert.y
        mov ecx,0
lp2:
        push ecx
        mov eax,offset Vert
        mov ebx,offset Vert2
        mov ecx,1
        call CopyVertexList
        pop ecx
        mov eax,offset Vert2
        mov ebx,ecx
        call RotateVertexX
        ;call RotateVertexY
        ;call RotateVertexZ
        fld sz
        fld sy
        fld sx
        call ShiftVertex
        mov eax,offset Vert2
        call ConvertVertex3DTo2D
        fld Vert2.x
        fistp pp
        mov eax,pp
        fld Vert2.y
        fistp pp
        mov ebx,pp
        fld Vert2.z
        fistp pp
        mov edx,pp
        call putpixel
        inc ecx
        cmp ecx,360
        jne lp2

        fld __50
        fstp Vert.y
        fld __0
        fst Vert.x
        fstp Vert.z
        mov ecx,0
lp3:
        push ecx
        mov eax,offset Vert
        mov ebx,offset Vert2
        mov ecx,1
        call CopyVertexList
        pop ecx
        mov eax,offset Vert2
        mov ebx,ecx
        ;call RotateVertexX
        ;call RotateVertexY
        call RotateVertexZ
        fld sz
        fld sy
        fld sx
        call ShiftVertex
        mov eax,offset Vert2
        call ConvertVertex3DTo2D
        fld Vert2.x
        fistp pp
        mov eax,pp
        fld Vert2.y
        fistp pp
        mov ebx,pp
        fld Vert2.z
        fistp pp
        mov edx,pp
        call putpixel
        inc ecx
        cmp ecx,360
        jne lp3

        call Wait4VSync

        push fs
        pop es
        FillMemD 0a0000h,64000,0

        IsKeyPressed KEY_ESC
        jnz q

        IsKeyPressed KEY_UP
        jz d1
        fld sy
        fsub __1
        fstp sy
d1:
        IsKeyPressed KEY_DOWN
        jz d2
        fld sy
        fadd __1
        fstp sy
d2:
        IsKeyPressed KEY_RIGHT
        jz d3
        fld sx
        fadd __1
        fstp sx
d3:
        IsKeyPressed KEY_LEFT
        jz d4
        fld sx
        fsub __1
        fstp sx
d4:
        IsKeyPressed KEY_PAGEUP
        jz d5
        fld sz
        fadd __1
        fstp sz
d5:
        IsKeyPressed KEY_PAGEDOWN
        jz d6
        fld sz
        fsub __1
        fstp sz
d6:

        jmp bll

q:

        SetTXTMode
        call CloseKB

        mov ax,4c00h
        int 21h

;eax-x ebx-y edx-z
putpixel:
        cmp edx,0
        jl putpixel_endp
        cmp eax,0
        jl putpixel_endp
        cmp ebx,0
        jl putpixel_endp
        cmp eax,318
        jae putpixel_endp
        cmp ebx,200
        jae putpixel_endp
        mov edi,ebx
        shl edi,8
        shl ebx,6
        add edi,ebx
        add edi,eax
        mov eax,256
        sub eax,edx
        cmp eax,0
        jnl putpixel_d1
        mov eax,0
putpixel_d1:
        shr eax,2
        mov byte ptr fs:[0a0000h+edi],al
        mov byte ptr fs:[0a0000h+edi+1],al
        mov byte ptr fs:[0a0000h+edi+2],al
        mov byte ptr fs:[0a0000h+edi+320],al
        mov byte ptr fs:[0a0000h+edi+320+320],al
        mov byte ptr fs:[0a0000h+edi+321],al
        mov byte ptr fs:[0a0000h+edi+321+320],al
        mov byte ptr fs:[0a0000h+edi+322],al
        mov byte ptr fs:[0a0000h+edi+322+320],al
putpixel_endp:
        ret

stub32 ends

end
