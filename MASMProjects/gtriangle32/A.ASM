.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;stale

scrwidth equ 1024
scrheight equ 768

;dane

Hellotxt db 'Program dziaˆaˆ w trybie graficznym 1024x768 przy 16777216 kolorach',0

lfbla dd 0
l dd 0
ls db 10 dup (0)

Triangle struct
        x1 dd 0         ;0
        y1 dd 0         ;4
        x2 dd 0         ;8
        y2 dd 0         ;12
        x3 dd 0         ;16
        y3 dd 0         ;20
        r1 db 0         ;24
        g1 db 0         ;25
        b1 db 0         ;26
        r2 db 0         ;27
        g2 db 0         ;28
        b2 db 0         ;29
        r3 db 0         ;30
        g3 db 0         ;31
        b3 db 0         ;32
Triangle ends

MyTriangle Triangle <scrwidth/2,0, 10,scrheight-10, scrwidth-10,scrheight-10, 255,0,0, 0,255,0, 0,0,255>
MyTriangle2 Triangle <scrwidth/2,0, 10,scrheight-10, scrwidth-10,scrheight-10, 0,0,0, 255,0,0, 180,0,0>
MyTriangle3 Triangle <scrwidth/2,0, 10,scrheight-10, scrwidth-10,scrheight-10, 0,0,0, 0,0,0, 0,0,0>

;kod

.nolist
include dpmi.inc
include vesa.inc
include vga.inc
include mstdlib\mcon.inc
include mstdlib\aiconv.inc
.list

.486

;esi-offset do struktory Triangle

DrawGTriangle32 proc stdcall
        local wspx1:dword,wspx2:dword,wspr1:dword,wspg1:dword,wspb1:dword
        local wspr2:dword,wspg2:dword,wspb2:dword
        local dx1:dword,dx2:dword
        local drr1:dword,drg1:dword,drb1:dword,drr2:dword,drg2:dword,drb2:dword
        local dy1:dword
        local dlr,dlg,dlb:dword
        local tr,tg,tb:dword
        local shfbuf:dword
        local cntx:dword
        pushad
        ;
        mov eax,[esi+8]                 ;x2
        sub eax,[esi]                   ;x1
        mov ebx,[esi+12]                ;y2
        sub ebx,[esi+4]                 ;y1
        cdq
        shld edx,eax,16
        shl eax,16
        idiv ebx                        ;(x2-x1)<<16/(y2-y1)
        mov wspx1,eax
        ;
        mov eax,[esi+16]                ;x3
        sub eax,[esi]                   ;x1
        mov ebx,[esi+20]                ;y3
        sub ebx,[esi+4]                 ;y1
        cdq
        shld edx,eax,16
        shl eax,16
        idiv ebx                        ;(x3-x1)<<16/(y3-y1)
        mov wspx2,eax
        ;

        movzx eax,byte ptr [esi+27]     ;r2
        movzx ebx,byte ptr [esi+24]     ;r1
        sub eax,ebx                     ;r2-r1
        mov ebx,[esi+12]                ;y2
        sub ebx,[esi+4]                 ;y1
        shl eax,16
        cdq
        idiv ebx                        ;(r2-r1)<<16/y2-y1
        mov wspr1,eax
        ;
        movzx eax,byte ptr [esi+28]     ;g2
        movzx ebx,byte ptr [esi+25]     ;g1
        sub eax,ebx                     ;g2-g1
        mov ebx,[esi+12]                ;y2
        sub ebx,[esi+4]                 ;y1
        shl eax,16
        cdq
        idiv ebx                        ;(g2-g1)<<16/y2-y1
        mov wspg1,eax
        ;
        movzx eax,byte ptr [esi+29]     ;b2
        movzx ebx,byte ptr [esi+26]     ;b1
        sub eax,ebx                     ;b2-b1
        mov ebx,[esi+12]                ;y2
        sub ebx,[esi+4]                 ;y1
        shl eax,16
        cdq
        idiv ebx                        ;(b2-b1)<<16/y2-y1
        mov wspb1,eax
        ;

        movzx eax,byte ptr [esi+30]     ;r3
        movzx ebx,byte ptr [esi+24]     ;r1
        sub eax,ebx                     ;r3-r1
        mov ebx,[esi+20]                ;y3
        sub ebx,[esi+4]                 ;y1
        shl eax,16
        cdq
        idiv ebx                        ;(r3-r1)<<16/y3-y1
        mov wspr2,eax
        ;
        movzx eax,byte ptr [esi+31]     ;g3
        movzx ebx,byte ptr [esi+25]     ;g1
        sub eax,ebx                     ;g3-g1
        mov ebx,[esi+20]                ;y3
        sub ebx,[esi+4]                 ;y1
        shl eax,16
        cdq
        idiv ebx                        ;(g3-g1)<<16/y3-y1
        mov wspg2,eax
        ;
        movzx eax,byte ptr [esi+32]     ;b3
        movzx ebx,byte ptr [esi+26]     ;b1
        sub eax,ebx                     ;b2-b1
        mov ebx,[esi+20]                ;y3
        sub ebx,[esi+4]                 ;y1
        shl eax,16
        cdq
        idiv ebx                        ;(b3-b1)<<16/y3-y1
        mov wspb2,eax
        ;
        mov es,stub32zerosel
        ;
        mov ecx,[esi+12]                ;y2
        sub ecx,[esi+4]                 ;y1
        ;
        mov eax,[esi]                   ;x1
        shl eax,16
        mov dx1,eax
        mov dx2,eax
        mov eax,[esi+4]                 ;y1
        mov dy1,eax
        ;
        movzx eax,byte ptr [esi+24]     ;r1
        shl eax,16
        mov drr1,eax
        mov drr2,eax
        movzx eax,byte ptr [esi+25]     ;g1
        shl eax,16
        mov drg1,eax
        mov drg2,eax
        movzx eax,byte ptr [esi+26]     ;b1
        shl eax,16
        mov drb1,eax
        mov drb2,eax
        ;
DrawGTriangle32_draw_loop_y:
        mov esi,dx2
        sub esi,dx1
        mov eax,dx2
        shr eax,16
        mov cntx,eax
        mov ebx,dx1
        shr ebx,16
        or esi,esi
        jz DrawGTriangle32_draw_loop_x_e
        mov eax,dy1
        mov edx,scrwidth
        mul edx
        mov edx,dx1
        shr edx,16
        add eax,edx
        shl eax,2
        add eax,lfbla
        mov edi,eax
        ;r
        mov eax,drr2
        sub eax,drr1
        cdq
        shld edx,eax,16
        shl eax,16
        idiv esi                 ;drr2-drr1<<16/dx2-dx1
        mov dlr,eax
        ;g
        mov eax,drg2
        sub eax,drg1
        cdq
        shld edx,eax,16
        shl eax,16
        idiv esi                 ;drg2-drg1<<16/dx2-dx1
        mov dlg,eax
        ;b
        mov eax,drb2
        sub eax,drb1
        cdq
        shld edx,eax,16
        shl eax,16
        idiv esi                 ;drb2-drb1<<16/dx2-dx1
        mov dlb,eax
        ;
        mov eax,drr1
        mov tr,eax
        ;
        mov eax,drg1
        mov tg,eax
        ;
        mov eax,drb1
        mov tb,eax

DrawGTriangle32_draw_loop_x:
                mov edx,tr
                mov eax,tg
                and edx,000ff0000h
                shr eax,16
                shl eax,8
                or eax,edx
                mov edx,tb
                shr edx,16
                or eax,edx
                stosd
                ;
                mov eax,dlr
                add tr,eax
                mov eax,dlg
                add tg,eax
                mov eax,dlb
                add tb,eax
                ;
                inc ebx
                cmp ebx,cntx
                jb DrawGTriangle32_draw_loop_x
DrawGTriangle32_draw_loop_x_e:       
        mov eax,wspx1
        add dx1,eax
        mov eax,wspx2
        add dx2,eax
        ;
        mov eax,wspr1
        add drr1,eax
        mov eax,wspg1
        add drg1,eax
        mov eax,wspb1
        add drb1,eax
        mov eax,wspr2
        add drr2,eax
        mov eax,wspg2
        add drg2,eax
        mov eax,wspb2
        add drb2,eax
        ;
        inc dy1
        dec ecx
        or ecx,ecx
        jnz DrawGTriangle32_draw_loop_y
        popad
        ret
DrawGTriangle32 endp

main:
        ;inicjacja
        SetVESAMode V1024x768x16M,4000h
        mov cx,V1024x768x16M
        call GetLFB
        cmp eax,0ffffffffh
        jz endprog
        mov lfbla,eax
        .586p
        rdtsc
        mov l,eax
        mov esi,offset MyTriangle
        call DrawGTriangle32
        rdtsc
        sub eax,l
        mov l,eax
        ReadKey
        mov esi,offset MyTriangle2
        call DrawGTriangle32
        ReadKey
drawloop:
        mov esi,offset MyTriangle3
        call DrawGTriangle32
        inc MyTriangle3.r1
        cmp MyTriangle3.r1,255
        jne d1
        dec MyTriangle3.r1
        inc MyTriangle3.g2
d1:
        cmp MyTriangle3.g2,255
        jne d2
        dec MyTriangle3.g2
        inc MyTriangle3.b3
d2:
        GetKey
        mov ax,stub32inthandrec.__flg
        and ax,64
        jnz drawloop
        ReadKey
        SetTXTMode
        invoke UToA,l,addr ls
        invoke Print,0,0,addr Hellotxt
        invoke Print,0,1,addr ls
endprog:
        mov ax,4c00h
        int 21h

stub32 ends

end
