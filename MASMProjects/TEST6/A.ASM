.386

public main

stub32 segment public
assume cs:stub32,ds:stub32
include DPMI.INC
include MCON.INC
x dw 0
y dw 0
nap db 'Kamil Korolkiewicz',0
main:
        mov ax,13h
        int 10h
        ;
        mov dx,3c8h
        mov al,0
        out dx,al
        inc dx
        mov ecx,256
pl:
        out dx,al
        out dx,al
        out dx,al
        inc al
        loop pl
        ;
        mov edi,0a0000h
        xor eax,eax
        mov ecx,64000
p1:
        mov ax,x
        xor ax,y
        stosb
        inc x
        cmp x,320
        jne d1
        mov x,0
        inc y
d1:
        loop p1
        mov ax,0700h
        int 21h
        ;
        mov x,0
        mov y,0
        mov edi,0a0000h
        xor eax,eax
        mov ecx,64000
p2:
        mov ax,x
        and ax,y
        stosb
        inc x
        cmp x,320
        jne d2
        mov x,0
        inc y
d2:
        loop p2
        mov ax,0700h
        int 21h
        ;
        mov x,0
        mov y,0
        mov edi,0a0000h
        xor eax,eax
        mov ecx,64000
p3:
        mov ax,x
        or ax,y
        stosb
        inc x
        cmp x,320
        jne d3
        mov x,0
        inc y
d3:
        loop p3
        mov ax,0700h
        int 21h
        ;
        mov x,0
        mov y,0
        mov edi,0a0000h
        xor eax,eax
        mov ecx,64000
p4:
        mov ax,x
        xor ax,y
        add eax,edi
        stosb
        inc x
        cmp x,320
        jne d4
        mov x,0
        inc y
d4:
        loop p4
        mov ax,0700h
        int 21h
        ;
        mov x,0
        mov y,0
        mov edi,0a0000h
        xor eax,eax
        mov ecx,64000
p5:
        mov ax,x
        mul y
        stosb
        inc x
        cmp x,320
        jne d5
        mov x,0
        inc y
d5:
        loop p5
        mov ax,0700h
        int 21h
        ;
        mov x,0
        mov y,0
        mov edi,0a0000h
        xor eax,eax
        mov ecx,64000
p6:
        mov ax,x
        mul ax
        mul y
        mul y
        add eax,edi
        stosb
        inc x
        cmp x,320
        jne d6
        mov x,0
        inc y
d6:
        loop p6
        mov ax,0700h
        int 21h
        ;
        mov x,0
        mov y,0
        mov edi,0a0000h
        xor eax,eax
        mov ecx,64000
p7:
        mov ax,x
        add ax,x
        add ax,x
        mov bl,byte ptr y
        and al,bl
        add eax,edi
        stosb
        inc x
        cmp x,320
        jne d7
        mov x,0
        inc y
d7:
        loop p7
        mov ax,0700h
        int 21h
        ;
        mov x,0
        mov y,0
        mov edi,0a0000h
        xor eax,eax
        mov ecx,64000/4
p8:
        mov ax,x
        shl eax,16
        mov ax,y
        stosd
        inc x
        cmp x,320
        jne d8
        mov x,0
        inc y
d8:
        loop p8
        mov ax,0700h
        int 21h
        ;
        mov x,0
        mov y,0
        mov edi,0a0000h
        xor eax,eax
        mov ecx,64000
p9:
        mov al,es:[edi]
        xor ax,x
        xor ax,y
        stosb
        inc x
        cmp x,320
        jne d9
        mov x,0
        inc y
d9:
        loop p9
        mov ax,0700h
        int 21h
        ;
        mov x,0
        mov y,0
        mov edi,0a0000h
        xor eax,eax
        mov ecx,64000
p10:
        mov ax,x
        mul ax
        sub ax,x
        mul ax
        sub ax,y
        mul ax
        sub ax,y
        mul ax
        mul ax
        mov ax,dx
        stosb
        inc x
        cmp x,320
        jne d10
        mov x,0
        inc y
d10:
        loop p10
        mov ax,0700h
        int 21h
        ;
        mov x,0
        mov y,0
        mov edi,0a0000h
        xor eax,eax
        mov ecx,64000
p11:
        mov ax,x
        and ax,y
        mul edi
        stosb
        inc x
        cmp x,320
        jne d11
        mov x,0
        inc y
d11:
        loop p11
        mov ax,0700h
        int 21h
        ;
        mov dx,3c8h
        mov al,0
        out dx,al
        inc dx
        mov ecx,256
        mov ah,0
        mov bl,0
pl2:
        out dx,al
        push ax
        mov al,ah
        out dx,al
        mov al,bl
        out dx,al
        pop ax
        inc al
        add ah,4
        add bl,8
        loop pl2
        ;
        mov x,0
        mov y,0
        mov edi,0a0000h
        xor eax,eax
        mov ecx,64000
p12:
        mov ax,x
        or ax,y
        stosb
        inc x
        cmp x,320
        jne d12
        mov x,0
        inc y
d12:
        loop p12
        mov ax,0700h
        int 21h
        ;
        mov ax,3h
        int 10h
        ;int 3
        invoke Print,0,0,addr nap
        invoke Print,70,1,addr nap
        ReadKey
        call Cls
        ReadKey
        invoke Print,10,10,addr nap
        invoke Print,21,21,addr nap
        GetKey
        mov ax,4c00h
        int 21h

stub32 ends

end
