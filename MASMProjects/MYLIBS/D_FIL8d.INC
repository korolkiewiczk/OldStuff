IFNDEF __D_FIL8_INC
__D_FIL8_INC=1
;D_FIL8.ASM for MASM/STUB32 by Kamil Korolkiewicz
;Procedury rysujace trojkaty wypelnione

include surface.inc
include d_data.inc
include debug.inc

t1 dd 0
dd 0
t2 dd 0
dd 0
t3 dd 0
dd 0
t4 dd 0
dd 0

;--------------------------------
;DrawFilledTriangle8 (surf:dword,trianglestruct:dword)
;       Rysuje trojkat wypelniony jednym kolorem
;       Wejscie: surf-powierzchnia na ktorej rysujemy (adres na strukture)
;                trianglestruct-adres do struktory trojkata
;--------------------------------

DrawFilledTriangle8 proc stdcall uses eax ebx ecx edx esi edi,surf:dword,trianglestruct:dword
        assume esi:ptr TSurface
        Debug_rdtsc t4,0
        mov ebx,trianglestruct
        mov esi,surf
        mov es,stub32zerosel
        Debug_rdtsc t1,0
        ;SORTOWANIE
        ;a2>b2?
        fld dword ptr [ebx+4]
        fcomp dword ptr [ebx+4+__T_SIZE]
        fstsw ax
        test ax,4500h
        jz DrawFilledTriangle8_a2b2w

        ;a2>c2? dla a2<=b2
        fld dword ptr [ebx+4]
        fcomp dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fstsw ax
        test ax,4500h
        jz DrawFilledTriangle8_a2b2m_a2c2w

        ;c2>b2? dla a2<=b2 i a2<=c2
        fld dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fcomp dword ptr [ebx+4+__T_SIZE]
        fstsw ax
        test ax,4500h
        jz DrawFilledTriangle8_a2b2m_a2c2m_c2b2w

        ;a2<=b2,a2<=c2,c2<=b2
        ;__d_a1:=a1
        fld dword ptr [ebx]
        fstp __d_a1
        ;__d_a2:=a2
        fld dword ptr [ebx+4]
        fstp __d_a2
        ;__d_c1:=b1
        fld dword ptr [ebx+__T_SIZE]
        fstp __d_c1
        ;__d_c2:=b2
        fld dword ptr [ebx+4+__T_SIZE]
        fstp __d_c2
        ;__d_b1:=c1
        fld dword ptr [ebx+__T_SIZE+__T_SIZE]
        fstp __d_b1
        ;__d_b2:=c2
        fld dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fstp __d_b2
        jmp DrawFilledTriangle8_endsort

DrawFilledTriangle8_a2b2m_a2c2m_c2b2w:
        ;a2<=b2,a2<=c2,c2>b2
        ;__d_a1:=a1
        fld dword ptr [ebx]
        fstp __d_a1
        ;__d_a2:=a2
        fld dword ptr [ebx+4]
        fstp __d_a2
        ;__d_b1:=b1
        fld dword ptr [ebx+__T_SIZE]
        fstp __d_b1
        ;__d_b2:=b2
        fld dword ptr [ebx+4+__T_SIZE]
        fstp __d_b2
        ;__d_c1:=c1
        fld dword ptr [ebx+__T_SIZE+__T_SIZE]
        fstp __d_c1
        ;__d_c2:=c2
        fld dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fstp __d_c2
        jmp DrawFilledTriangle8_endsort

DrawFilledTriangle8_a2b2m_a2c2w:
        ;a2<=b2,a2>c2
        ;__d_b1:=a1
        fld dword ptr [ebx]
        fstp __d_b1
        ;__d_b2:=a2
        fld dword ptr [ebx+4]
        fstp __d_b2
        ;__d_c1:=b1
        fld dword ptr [ebx+__T_SIZE]
        fstp __d_c1
        ;__d_c2:=b2
        fld dword ptr [ebx+4+__T_SIZE]
        fstp __d_c2
        ;__d_a1:=c1
        fld dword ptr [ebx+__T_SIZE+__T_SIZE]
        fstp __d_a1
        ;__d_a2:=c2
        fld dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fstp __d_a2
        jmp DrawFilledTriangle8_endsort

DrawFilledTriangle8_a2b2w:
        ;a2>c2? dla a2>b2
        fld dword ptr [ebx+4]
        fcomp dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fstsw ax
        test ax,4500h
        jz DrawFilledTriangle8_a2b2w_a2c2w

        ;a2>b2,a2<=c2
        ;__d_b1:=a1
        fld dword ptr [ebx]
        fstp __d_b1
        ;__d_b2:=a2
        fld dword ptr [ebx+4]
        fstp __d_b2
        ;__d_a1:=b1
        fld dword ptr [ebx+__T_SIZE]
        fstp __d_a1
        ;__d_a2:=b2
        fld dword ptr [ebx+4+__T_SIZE]
        fstp __d_a2
        ;__d_c1:=c1
        fld dword ptr [ebx+__T_SIZE+__T_SIZE]
        fstp __d_c1
        ;__d_c2:=c2
        fld dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fstp __d_c2
        jmp DrawFilledTriangle8_endsort

DrawFilledTriangle8_a2b2w_a2c2w:

        ;c2>b2? dla a2>b2 i a2>c2
        fld dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fcomp dword ptr [ebx+4+__T_SIZE]
        fstsw ax
        test ax,4500h
        jz DrawFilledTriangle8_a2b2w_a2c2w_c2b2w

        ;a2>b2,a2>c2,c2<=b2
        ;__d_c1:=a1
        fld dword ptr [ebx]
        fstp __d_c1
        ;__d_c2:=a2
        fld dword ptr [ebx+4]
        fstp __d_c2
        ;__d_b1:=b1
        fld dword ptr [ebx+__T_SIZE]
        fstp __d_b1
        ;__d_b2:=b2
        fld dword ptr [ebx+4+__T_SIZE]
        fstp __d_b2
        ;__d_a1:=c1
        fld dword ptr [ebx+__T_SIZE+__T_SIZE]
        fstp __d_a1
        ;__d_a2:=c2
        fld dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fstp __d_a2
        jmp DrawFilledTriangle8_endsort

DrawFilledTriangle8_a2b2w_a2c2w_c2b2w:
        ;a2>b2,a2>c2,c2>b2
        ;__d_c1:=a1
        fld dword ptr [ebx]
        fstp __d_c1
        ;__d_c2:=a2
        fld dword ptr [ebx+4]
        fstp __d_c2
        ;__d_a1:=b1
        fld dword ptr [ebx+__T_SIZE]
        fstp __d_a1
        ;__d_a2:=b2
        fld dword ptr [ebx+4+__T_SIZE]
        fstp __d_a2
        ;__d_b1:=c1
        fld dword ptr [ebx+__T_SIZE+__T_SIZE]
        fstp __d_b1
        ;__d_b2:=c2
        fld dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fstp __d_b2

DrawFilledTriangle8_endsort:
        Debug_rdtsc t1,1
        Debug_rdtsc t2,0

        ;CZY WIDOCZNY
        ;a1>=0?
        fld __d_a1
        fistp __d_int
        cmp __d_int,-1
        jg DrawFilledTriangle8_xw0

        ;b1>=0?
        fld __d_b1
        fistp __d_int
        cmp __d_int,-1
        jg DrawFilledTriangle8_xw0

        ;c1>=0?
        fld __d_b1
        fistp __d_int
        cmp __d_int,-1
        jg DrawFilledTriangle8_xw0

        jmp DrawFilledTriangle8_endp

DrawFilledTriangle8_xw0:

        mov eax,[esi].SWidth
        ;a1<SWidth?
        fld __d_a1
        fistp __d_int
        cmp __d_int,eax
        jl DrawFilledTriangle8_xlSW

        ;b1<SWidth?
        fld __d_b1
        fistp __d_int
        cmp __d_int,eax
        jl DrawFilledTriangle8_xlSW

        ;c1<SWidth?
        fld __d_c1
        fistp __d_int
        cmp __d_int,eax
        jl DrawFilledTriangle8_xlSW

        jmp DrawFilledTriangle8_endp

DrawFilledTriangle8_xlSW:

        ;c2<0?
        fld __d_c2
        fistp __d_int
        cmp __d_int,0
        jl DrawFilledTriangle8_endp

        ;a2>=SWidth?
        mov eax,[esi].SHeight
        fld __d_a2
        fistp __d_int
        cmp __d_int,eax
        jge DrawFilledTriangle8_endp

        ;a2=c2?
        fld __d_a2
        fistp __d_int
        mov eax,__d_int
        fld __d_c2
        fistp __d_int
        cmp eax,__d_int
        je DrawFilledTriangle8_endp


        ;WSPOLCZYNNIK AC
        ;__d_dxac:=(c1-a1)/(c2-a2)
        fld __d_c1
        fsub __d_a1
        fld __d_c2
        fsub __d_a2
        fdivp st(1),st(0)
        fstp __d_dxac

        ;USTALENIE KOLORU
        ;__d_col:=color
        mov eax,[ebx+16]
        mov ah,al
        mov dx,ax
        shl eax,16
        mov ax,dx
        mov __d_col,eax

        Debug_rdtsc t2,1
        Debug_rdtsc t3,0

        ;CZY RYSOWAC BOK AB-AC
        fld __d_a2
        fistp __d_int
        mov eax,__d_int
        fld __d_b2
        fistp __d_int
        cmp eax,__d_int
        je DrawFilledTriangle8_bcacs
        cmp __d_int,0
        jl DrawFilledTriangle8_bcacs

        ;WSPOLCZYNNIK AB
        ;__d_dxab:=(b1-a1)/(b2-a2)
        fld __d_b1
        fsub __d_a1
        fld __d_b2
        fsub __d_a2
        fdivp st(1),st(0)
        fstp __d_dxab

        ;USTAWIENIA PRZED RYSOWANIEM BOKU AB-AC
        ;__d__x1:=__d__x2:=__d__a1
        fld  __d_a1
        fst __d_x1
        fstp __d_x2

        ;a2>=0?
        fld __d_a2
        fistp __d_int
        cmp __d_int,0
        jge DrawFilledTriangle8_a2w0

        ;a2<0
        ;__d_x1:=__d_x1+__d_dxab*(-__d_a2)
        fld __d_a2
        fchs
        fld __d_dxab
        fmul st(0),st(1)
        fadd __d_x1
        fstp __d_x1
        ;__d_x2:=__d_x2+__d_dxac*(-__d_a2)
        fld __d_dxac
        fmul st(0),st(1)
        fadd __d_x2
        fstp __d_x2
        ffree st(0)
        fincstp
        mov ecx,[esi].SHeight
        mov __d_y,0

        ;b2>=SHeight?
        fld __d_b2
        fistp __d_int
        cmp __d_int,ecx
        jge DrawFilledTriangle8_b2m0

        ;b2<SHeight
        ;ecx:=b2
        fld __d_b2
        fistp __d_int
        mov ecx,__d_int

        jmp DrawFilledTriangle8_b2m0

DrawFilledTriangle8_a2w0:

        ;a1>=0
        ;ecx:=b2-a2
        fld __d_b2
        fsub __d_a2
        fistp __d_int
        mov ecx,__d_int
        ;__d_y:=a2
        fld __d_a2
        fistp __d_y

DrawFilledTriangle8_a2d0:

        ;b2<SHeight?
        fld __d_b2
        fistp __d_int
        mov eax,[esi].SHeight
        cmp __d_int,eax
        jl DrawFilledTriangle8_b2m0

        ;b2>=SHeight
        mov eax,__d_y
        mov ecx,[esi].SHeight
        sub ecx,eax

DrawFilledTriangle8_b2m0:


        ;RYSOWANIE AB-AC

        ;__d_cnt:=il. lini (ecx)
        mov __d_cnt,ecx
        mov ebx,__d_y
        cld
        ;dxac2:=dxac
        fld __d_dxac
        fstp __d_dxac2

        ;dxab>dxac? (rysowanie od AC do AB)
        fld __d_dxab
        fcomp __d_dxac
        fstsw ax
        test ax,4500h
        jnz DrawFilledTriangle8_dxabmdxac
        ;dxab>dxac
        ;zamiana x1 z x2
        fld __d_x1
        fld __d_x2
        fstp __d_x1
        fstp __d_x2
        ;zamiana dxab z dxac
        fld __d_dxab
        fld __d_dxac
        fstp __d_dxab
        fstp __d_dxac

DrawFilledTriangle8_dxabmdxac:
        Debug_rdtsc t3,1

        ;rysowanie trojkata od boku AB do AC
DrawFilledTriangle8_drawabac:

                ;ix1:=x1; ix2:=x2 (float --> int)
                fld __d_x1
                fistp __d_ix1
                fld __d_x2
                fistp __d_ix2

                ;ix2<SWidth?
                mov eax,[esi].SWidth
                cmp __d_ix2,eax
                jl DrawFilledTriangle8_drawabacx2mSW

                ;ix1>=0?
                cmp __d_ix1,-1
                jg DrawFilledTriangle8_drawabacx1w0_1
                ;x1<0 i x2>SWidth
                mov ecx,eax
                mov __d_ix1,0
                jmp DrawFilledTriangle8_drawabacx1w0_2

DrawFilledTriangle8_drawabacx1w0_1:
                ;x1>=0 i x2>SWidth
                mov ecx,eax
                sub ecx,__d_ix1
                jmp DrawFilledTriangle8_drawabacx1w0_2

DrawFilledTriangle8_drawabacx2mSW:
                ;x2<SWidth
                mov ecx,__d_ix2
                sub ecx,__d_ix1

                ;x1<0?
                cmp __d_ix1,-1
                jg DrawFilledTriangle8_drawabacx1w0_2
                ;x1<0
                add ecx,__d_ix1
                mov __d_ix1,0

DrawFilledTriangle8_drawabacx1w0_2:
                ;ecx=0?
                jecxz DrawFilledTriangle8_drawabacd1

                ;obliczenie poczatku lini
                mov edi,[esi].SBits
                mul ebx
                add eax,__d_ix1
                add edi,eax

                ;rysowanie lini
                mov eax,__d_col
                mov edx,ecx
                and edx,3
                shr ecx,2
                rep stosd
                mov ecx,edx
                rep stosb
DrawFilledTriangle8_drawabacd1:

                ;x1:=x1+dxab
                fld __d_x1
                fadd __d_dxab
                fstp __d_x1

                ;x2:=x2+dxac
                fld __d_x2
                fadd __d_dxac
                fstp __d_x2

                inc ebx
                dec __d_cnt

        jnz DrawFilledTriangle8_drawabac

DrawFilledTriangle8_bcacs:

        ;CZY RYSOWAC BOK BC-AC
        fld __d_b2
        fistp __d_int
        mov eax,__d_int
        fld __d_c2
        fistp __d_int
        cmp eax,__d_int
        je DrawFilledTriangle8_endp
        mov eax,[esi].SHeight
        cmp __d_int,eax
        jge DrawFilledTriangle8_endp

        ;dxac:=dxac2
        fld __d_dxac2
        fstp __d_dxac        

        ;WSPOLCZYNNIK BC
        ;__d_dxbc:=(c1-b1)/(c2-b2)
        fld __d_c1
        fsub __d_b1
        fld __d_c2
        fsub __d_b2
        fdivp st(1),st(0)
        fstp __d_dxbc

        ;USTAWIENIA PRZED RYSOWANIEM BOKU BC-AC
        ;__d_x1:=__d_b1
        fld  __d_b1
        fstp __d_x1
        ;__d_x2:=a1+(b2-a2)*__d_dxac
        fld __d_b2
        fsub __d_a2
        fmul __d_dxac
        fadd __d_a1
        fstp __d_x2

        ;b2>=0?
        fld __d_b2
        fistp __d_int
        cmp __d_int,0
        jge DrawFilledTriangle8_b2w0

        ;b2<0
        ;__d_x1:=__d_x1+__d_dxbc*(-__d_b2)
        fld __d_b2
        fchs
        fld __d_dxbc
        fmul st(0),st(1)
        fadd __d_x1
        fstp __d_x1
        ;__d_x2:=__d_x2+__d_dxac*(-__d_b2)
        fld __d_dxac
        fmul st(0),st(1)
        fadd __d_x2
        fstp __d_x2
        ffree st(0)
        fincstp
        mov ecx,[esi].SHeight
        mov __d_y,0

        ;c2>=SHeight?
        fld __d_c2
        fistp __d_int
        cmp __d_int,ecx
        jge DrawFilledTriangle8_c2m0

        ;c2<SHeight
        ;ecx:=c2
        fld __d_c2
        fistp __d_int
        mov ecx,__d_int

        jmp DrawFilledTriangle8_c2m0

DrawFilledTriangle8_b2w0:

        ;c1>=0
        ;ecx:=c2-b2
        fld __d_c2
        fsub __d_b2
        fistp __d_int
        mov ecx,__d_int
        ;__d_y:=b2
        fld __d_b2
        fistp __d_y

DrawFilledTriangle8_b2d0:

        ;c2<SHeight?
        fld __d_c2
        fistp __d_int
        mov eax,[esi].SHeight
        cmp __d_int,eax
        jl DrawFilledTriangle8_c2m0

        ;c2>=SHeight
        mov eax,__d_y
        mov ecx,[esi].SHeight
        sub ecx,eax

DrawFilledTriangle8_c2m0:


        ;RYSOWANIE BC-AC

        ;__d_cnt:=il. lini (ecx)
        mov __d_cnt,ecx
        mov ebx,__d_y
        cld

        ;dxac>dxbc? (rysowanie od AC do BC)
        fld __d_dxac
        fcomp __d_dxbc
        fstsw ax
        test ax,4500h
        jnz DrawFilledTriangle8_dxbcmdxac
        ;dxac>dxbc
        ;zamiana x1 z x2
        fld __d_x1
        fld __d_x2
        fstp __d_x1
        fstp __d_x2
        ;zamiana dxbc z dxac
        fld __d_dxbc
        fld __d_dxac
        fstp __d_dxbc
        fstp __d_dxac

DrawFilledTriangle8_dxbcmdxac:

        ;rysowanie trojkata od boku BC do AC
DrawFilledTriangle8_drawbcac:

                ;ix1:=x1; ix2:=x2 (float --> int)
                fld __d_x1
                fistp __d_ix1
                fld __d_x2
                fistp __d_ix2

                ;ix2<SWidth?
                mov eax,[esi].SWidth
                cmp __d_ix2,eax
                jl DrawFilledTriangle8_drawbcacx2mSW

                ;ix1>=0?
                cmp __d_ix1,-1
                jg DrawFilledTriangle8_drawbcacx1w0_1
                ;x1<0 i x2>SWidth
                mov ecx,eax
                mov __d_ix1,0
                jmp DrawFilledTriangle8_drawbcacx1w0_2

DrawFilledTriangle8_drawbcacx1w0_1:
                ;x1>=0 i x2>SWidth
                mov ecx,eax
                sub ecx,__d_ix1
                jmp DrawFilledTriangle8_drawbcacx1w0_2

DrawFilledTriangle8_drawbcacx2mSW:
                ;x2<SWidth
                mov ecx,__d_ix2
                sub ecx,__d_ix1

                ;x1<0?
                cmp __d_ix1,-1
                jg DrawFilledTriangle8_drawbcacx1w0_2
                ;x1<0
                add ecx,__d_ix1
                mov __d_ix1,0

DrawFilledTriangle8_drawbcacx1w0_2:
                ;ecx=0?
                jecxz DrawFilledTriangle8_drawbcacd1

                ;obliczenie poczatku lini
                mov edi,[esi].SBits
                mul ebx
                add eax,__d_ix1
                add edi,eax

                ;rysowanie lini
                mov eax,__d_col
                mov edx,ecx
                and edx,3
                shr ecx,2
                rep stosd
                mov ecx,edx
                rep stosb
DrawFilledTriangle8_drawbcacd1:

                ;x1:=x1+dxbc
                fld __d_x1
                fadd __d_dxbc
                fstp __d_x1

                ;x2:=x2+dxac
                fld __d_x2
                fadd __d_dxac
                fstp __d_x2

                inc ebx
                dec __d_cnt

        jnz DrawFilledTriangle8_drawbcac

DrawFilledTriangle8_endp:
        Debug_rdtsc t4,1
        ret
        assume esi:nothing
DrawFilledTriangle8 endp

ENDIF
