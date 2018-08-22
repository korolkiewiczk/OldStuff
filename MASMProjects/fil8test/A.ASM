.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;kod

include dpmi.inc
include mstdlib\mcon.inc
include surface.inc
include d_data.inc
__T_SIZE=20
.586p
include d_fil8d.inc
include vga.inc
include debug.inc
.listall

;

surf TSurface <320,200,1,320,0a0000h>
time dd 0
dd 0

triangle dd 160.0 ;a
         dd 10.0
         dd 0
         dd 0
         dd 1

         dd 190.0 ;b
         dd 120.0
         dd 0
         dd 0
         dd 1

         dd 10.0  ;c
         dd 140.0
         dd 0
         dd 0
         dd 0

triangle2 dd 160.0;a
         dd 10.0
         dd 0
         dd 0
         dd 1

         dd 10.0
         dd 120.0 ;b
         dd 0
         dd 0
         dd 1

         dd 190.0
         dd 140.0 ;c 
         dd 0
         dd 0
         dd 0

triangle3 dd 160.0;a
         dd 100.0
         dd 0
         dd 0
         dd 1

         dd 10.0
         dd 210.0 ;b
         dd 0
         dd 0
         dd 1

         dd 190.0
         dd 240.0 ;c 
         dd 0
         dd 0
         dd 0

triangle4 dd 0.0;a
         dd 0.0
         dd 0
         dd 0
         dd 1

         dd 320.0
         dd 200.0 ;b
         dd 0
         dd 0
         dd 1

         dd 0.0
         dd 200.0 ;c 
         dd 0
         dd 0
         dd 0

putpx:
        mov es,stub32zerosel
        mov edi,0a0000h
        mov ecx,320
        xchg eax,ebx
        mul ecx
        add eax,ebx
        add edi,eax
        mov byte ptr es:[edi],4
        ret


main:

        SetVGAMode
        invoke DrawFilledTriangle8,addr surf,addr triangle
        mov eax,160
        mov ebx,10
        call putpx
        mov eax,10
        mov ebx,140
        call putpx
        mov eax,190
        mov ebx,120
        call putpx
        ReadKey
        SetTXTMode
        Debug_Print 0,0,'Czasy: %u,%u,%u,%u',t1,t2,t3,t4
        ReadKey

        SetVGAMode
        invoke DrawFilledTriangle8,addr surf,addr triangle2
        mov eax,160
        mov ebx,10
        call putpx
        mov eax,10
        mov ebx,120
        call putpx
        mov eax,190
        mov ebx,140
        call putpx
        ReadKey
        SetTXTMode
        Debug_Print 0,0,'Czasy: %u,%u,%u,%u',t1,t2,t3,t4
        ReadKey

        SetVGAMode
        invoke DrawFilledTriangle8,addr surf,addr triangle3
        ReadKey

        SetTXTMode
        Debug_Print 0,0,'Czasy: %u,%u,%u,%u',t1,t2,t3,t4
        ReadKey

        SetVGAMode
        invoke DrawFilledTriangle8,addr surf,addr triangle4
        ReadKey

        SetTXTMode
        Debug_Print 0,0,'Czasy: %u,%u,%u,%u',t1,t2,t3,t4
        ReadKey

        mov ax,4c00h
        int 21h

stub32 ends

end
