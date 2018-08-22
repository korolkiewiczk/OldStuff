IFNDEF __D_TEX8_INC
__D_TEX8_INC=1
;D_TEX8.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Procedury do rysowania teksturowanych trojkatow

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

__d_iu dd 0
__d_iv dd 0

;struktura tekstury

TTex struct
                ShiftWidth dd ?
                TBits dd ?
TTex ends

;procedury

;--------------------------------
;DrawTexTriangle8 (surf:dword,tex:dword,trianglestruct:dword)
;       Rysuje teksturowany trojkat bez korekty perspektywy
;       Wejscie: surf-powierzchnia na ktorej rysujemy (adres na strukture)
;                tex-tekstura zrodlowa (TTex)
;                trianglestruct-adres do struktory trojkata
;--------------------------------

DrawTexTriangle8 proc stdcall uses eax ebx ecx edx esi edi ebp,surf:dword,tex:dword,trianglestruct:dword
        assume ebx:ptr TSurface
        debug_rdtsc t1,0
        debug_rdtsc t2,0
        mov ebx,surf
        mov eax,[ebx].SWidth
        mov __d_SWidth,eax
        mov eax,[ebx].SHeight
        mov __d_SHeight,eax
        mov eax,[ebx].SBits
        mov __d_SBits,eax
        assume ebx:nothing

        assume ebx:ptr TTex
        mov ebx,tex
        mov ecx,[ebx].ShiftWidth
        mov byte ptr __DTT8SHL1+2,cl
        mov byte ptr __DTT8SHL2+2,cl
        mov eax,1
        shl eax,cl
        shl eax,cl
        dec eax
        mov dword ptr __DTT8AND1+2,eax
        mov dword ptr __DTT8AND2+2,eax
        mov eax,1
        shl eax,cl
        dec eax
        mov __d_TWidth,eax
        mov eax,[ebx].TBits
        mov __d_TBits,eax
        assume ebx:nothing

        mov ebx,trianglestruct
        mov es,stub32zerosel

        ;SORTOWANIE
        ;a2>b2?
        fld dword ptr [ebx+4]
        fcomp dword ptr [ebx+4+__T_SIZE]
        fstsw ax
        test ax,4500h
        jz DrawTexTriangle8_a2b2w

        ;a2>c2? dla a2<=b2
        fld dword ptr [ebx+4]
        fcomp dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fstsw ax
        test ax,4500h
        jz DrawTexTriangle8_a2b2m_a2c2w

        ;c2>b2? dla a2<=b2 i a2<=c2
        fld dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fcomp dword ptr [ebx+4+__T_SIZE]
        fstsw ax
        test ax,4500h
        jz DrawTexTriangle8_a2b2m_a2c2m_c2b2w

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

        ;__d_au:=au
        fld dword ptr [ebx+20]
        fstp __d_au
        ;__d_av:=av
        fld dword ptr [ebx+24]
        fstp __d_av
        ;__d_cu:=bu
        fld dword ptr [ebx+20+__T_SIZE]
        fstp __d_cu
        ;__d_cv:=bv
        fld dword ptr [ebx+24+__T_SIZE]
        fstp __d_cv
        ;__d_bu:=cu
        fld dword ptr [ebx+20+__T_SIZE+__T_SIZE]
        fstp __d_bu
        ;__d_bv:=cv
        fld dword ptr [ebx+24+__T_SIZE+__T_SIZE]
        fstp __d_bv

        jmp DrawTexTriangle8_endsort

DrawTexTriangle8_a2b2m_a2c2m_c2b2w:
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

        ;__d_au:=au
        fld dword ptr [ebx+20]
        fstp __d_au
        ;__d_av:=av
        fld dword ptr [ebx+24]
        fstp __d_av
        ;__d_bu:=bu
        fld dword ptr [ebx+20+__T_SIZE]
        fstp __d_bu
        ;__d_bv:=bv
        fld dword ptr [ebx+24+__T_SIZE]
        fstp __d_bv
        ;__d_cu:=cu
        fld dword ptr [ebx+20+__T_SIZE+__T_SIZE]
        fstp __d_cu
        ;__d_cv:=cv
        fld dword ptr [ebx+24+__T_SIZE+__T_SIZE]
        fstp __d_cv

        jmp DrawTexTriangle8_endsort

DrawTexTriangle8_a2b2m_a2c2w:
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

        ;__d_bu:=au
        fld dword ptr [ebx+20]
        fstp __d_bu
        ;__d_bv:=av
        fld dword ptr [ebx+24]
        fstp __d_bv
        ;__d_cu:=bu
        fld dword ptr [ebx+20+__T_SIZE]
        fstp __d_cu
        ;__d_cv:=bv
        fld dword ptr [ebx+24+__T_SIZE]
        fstp __d_cv
        ;__d_au:=cu
        fld dword ptr [ebx+20+__T_SIZE+__T_SIZE]
        fstp __d_au
        ;__d_av:=cv
        fld dword ptr [ebx+24+__T_SIZE+__T_SIZE]
        fstp __d_av

        jmp DrawTexTriangle8_endsort

DrawTexTriangle8_a2b2w:
        ;a2>c2? dla a2>b2
        fld dword ptr [ebx+4]
        fcomp dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fstsw ax
        test ax,4500h
        jz DrawTexTriangle8_a2b2w_a2c2w

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

        ;__d_bu:=au
        fld dword ptr [ebx+20]
        fstp __d_bu
        ;__d_bv:=av
        fld dword ptr [ebx+24]
        fstp __d_bv
        ;__d_au:=bu
        fld dword ptr [ebx+20+__T_SIZE]
        fstp __d_au
        ;__d_av:=bv
        fld dword ptr [ebx+24+__T_SIZE]
        fstp __d_av
        ;__d_cu:=cu
        fld dword ptr [ebx+20+__T_SIZE+__T_SIZE]
        fstp __d_cu
        ;__d_cv:=cv
        fld dword ptr [ebx+24+__T_SIZE+__T_SIZE]
        fstp __d_cv

        jmp DrawTexTriangle8_endsort

DrawTexTriangle8_a2b2w_a2c2w:

        ;c2>b2? dla a2>b2 i a2>c2
        fld dword ptr [ebx+4+__T_SIZE+__T_SIZE]
        fcomp dword ptr [ebx+4+__T_SIZE]
        fstsw ax
        test ax,4500h
        jz DrawTexTriangle8_a2b2w_a2c2w_c2b2w

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

        ;__d_cu:=au
        fld dword ptr [ebx+20]
        fstp __d_cu
        ;__d_cv:=av
        fld dword ptr [ebx+24]
        fstp __d_cv
        ;__d_bu:=bu
        fld dword ptr [ebx+20+__T_SIZE]
        fstp __d_bu
        ;__d_bv:=bv
        fld dword ptr [ebx+24+__T_SIZE]
        fstp __d_bv
        ;__d_au:=cu
        fld dword ptr [ebx+20+__T_SIZE+__T_SIZE]
        fstp __d_au
        ;__d_av:=cv
        fld dword ptr [ebx+24+__T_SIZE+__T_SIZE]
        fstp __d_av

        jmp DrawTexTriangle8_endsort

DrawTexTriangle8_a2b2w_a2c2w_c2b2w:
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

        ;__d_cu:=au
        fld dword ptr [ebx+20]
        fstp __d_cu
        ;__d_cv:=av
        fld dword ptr [ebx+24]
        fstp __d_cv
        ;__d_au:=bu
        fld dword ptr [ebx+20+__T_SIZE]
        fstp __d_au
        ;__d_av:=bv
        fld dword ptr [ebx+24+__T_SIZE]
        fstp __d_av
        ;__d_bu:=cu
        fld dword ptr [ebx+20+__T_SIZE+__T_SIZE]
        fstp __d_bu
        ;__d_bv:=cv
        fld dword ptr [ebx+24+__T_SIZE+__T_SIZE]
        fstp __d_bv

DrawTexTriangle8_endsort:
        ;wszystkie wspolrzedne wierzcholkow do zaokraglane liczb calkowitych
        comment/
        fld __d_a1
        fld __d_b1
        fld __d_c1
        fld __d_a2
        fld __d_b2
        fld __d_c2
        frndint
        fstp __d_c2
        frndint
        fstp __d_b2
        frndint
        fstp __d_a2
        frndint
        fstp __d_c1
        frndint
        fstp __d_b1
        frndint
        fstp __d_a1/

        ;CZY WIDOCZNY
        ;a1>=0?
        fld __d_a1
        fistp __d_int
        cmp __d_int,-1
        jg DrawTexTriangle8_xw0

        ;b1>=0?
        fld __d_b1
        fistp __d_int
        cmp __d_int,-1
        jg DrawTexTriangle8_xw0

        ;c1>=0?
        fld __d_c1
        fistp __d_int
        cmp __d_int,-1
        jg DrawTexTriangle8_xw0

        jmp DrawTexTriangle8_endp

DrawTexTriangle8_xw0:

        mov eax,__d_SWidth
        ;a1<SWidth?
        fld __d_a1
        fistp __d_int
        cmp __d_int,eax
        jl DrawTexTriangle8_xlSW

        ;b1<SWidth?
        fld __d_b1
        fistp __d_int
        cmp __d_int,eax
        jl DrawTexTriangle8_xlSW

        ;c1<SWidth?
        fld __d_c1
        fistp __d_int
        cmp __d_int,eax
        jl DrawTexTriangle8_xlSW

        jmp DrawTexTriangle8_endp

DrawTexTriangle8_xlSW:

        ;c2<0?
        fld __d_c2
        fistp __d_int
        cmp __d_int,0
        jl DrawTexTriangle8_endp

        ;a2>=SWidth?
        mov eax,__d_SHeight
        fld __d_a2
        fistp __d_int
        cmp __d_int,eax
        jge DrawTexTriangle8_endp

        ;a2=c2?
        fld __d_a2
        fistp __d_int
        mov eax,__d_int
        fld __d_c2
        fistp __d_int
        cmp eax,__d_int
        je DrawTexTriangle8_endp

        ;au:=au*TWidth
        fild __d_TWidth
        fld __d_au
        fmul st(0),st(1)
        fstp __d_au
        ;av:=av*TWidth
        fld __d_av
        fmul st(0),st(1)
        fstp __d_av
        ;bu:=bu*TWidth
        fld __d_bu
        fmul st(0),st(1)
        fstp __d_bu
        ;bv:=bv*TWidth
        fld __d_bv
        fmul st(0),st(1)
        fstp __d_bv
        ;bu:=cu*TWidth
        fld __d_cu
        fmul st(0),st(1)
        fstp __d_cu
        ;cv:=cv*TWidth
        fld __d_cv
        fmul st(0),st(1)
        fstp __d_cv
        ffree st(0)
        fincstp
        
        ;WSPOLCZYNNIKI AC
        ;__d_dxac:=(c1-a1)/(c2-a2)
        fld __d_c1
        fsub __d_a1
        fld __d_c2
        fsub __d_a2
        fdivp st(1),st(0)
        fstp __d_dxac
        ;__d_duac:=(cu-au)/(c2-a2)
        fld __d_cu
        fsub __d_au
        fld __d_c2
        fsub __d_a2
        fdivp st(1),st(0)
        fstp __d_duac
        ;__d_dvac:=(cv-av)/(c2-a2)
        fld __d_cv
        fsub __d_av
        fld __d_c2
        fsub __d_a2
        fdivp st(1),st(0)
        fstp __d_dvac

        ;dxac2:=dxac
        fld __d_dxac
        fstp __d_dxac2

        ;duac2:=duac
        fld __d_duac
        fstp __d_duac2

        ;dvac2:=dvac
        fld __d_dvac
        fstp __d_dvac2

        debug_rdtsc t2,1

        ;CZY RYSOWAC BOK AB-AC
        fld __d_a2
        fistp __d_int
        mov eax,__d_int
        fld __d_b2
        fistp __d_int
        cmp eax,__d_int
        je DrawTexTriangle8_bcacs
        cmp __d_int,0
        jl DrawTexTriangle8_bcacs

        ;WSPOLCZYNNIKI AB
        debug_rdtsc t3,0
        ;__d_dxab:=(b1-a1)/(b2-a2)
        fld __d_b1
        fsub __d_a1
        fld __d_b2
        fsub __d_a2
        fdivp st(1),st(0)
        fstp __d_dxab
        ;__d_duab:=(bu-au)/(b2-a2)
        fld __d_bu
        fsub __d_au
        fld __d_b2
        fsub __d_a2
        fdivp st(1),st(0)
        fstp __d_duab
        ;__d_dvab:=(bv-av)/(b2-a2)
        fld __d_bv
        fsub __d_av
        fld __d_b2
        fsub __d_a2
        fdivp st(1),st(0)
        fstp __d_dvab

        ;USTALENIE MINX I MAXX
        ;a1>b1?
        fld __d_a1
        fcomp __d_b1
        fstsw ax
        test ax,4500h
        jz DrawTexTriangle8_a1wb1
        ;a1<=b1
        fld __d_a1
        fistp __d_minx
        fld __d_b1
        fistp __d_maxx
        jmp DrawTexTriangle8_endminmax

DrawTexTriangle8_a1wb1:
        ;a1>b1
        fld __d_b1
        fistp __d_minx
        fld __d_a1
        fistp __d_maxx
        
DrawTexTriangle8_endminmax:

        ;USTAWIENIA PRZED RYSOWANIEM BOKU AB-AC
        ;__d_x1:=__d_x2:=__d_a1
        fld  __d_a1
        fst __d_x1
        fstp __d_x2

        ;__d_u1:=__d_u2:=au
        fld __d_au
        fst __d_u1
        fstp __d_u2

        ;__d_v1:=__d_v2:=av
        fld __d_av
        fst __d_v1
        fstp __d_v2

        ;a2>=0?
        fld __d_a2
        fistp __d_int
        cmp __d_int,0
        jge DrawTexTriangle8_a2w0

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

        ;__d_u1:=__d_u1+__d_duab*(-__d_a2)
        fld __d_a2
        fchs
        fld __d_duab
        fmul st(0),st(1)
        fadd __d_u1
        fstp __d_u1
        ;__d_u2:=__d_u2+__d_duac*(-__d_a2)
        fld __d_duac
        fmul st(0),st(1)
        fadd __d_u2
        fstp __d_u2
        ;__d_v1:=__d_v1+__d_dvab*(-__d_a2)
        fld __d_dvab
        fmul st(0),st(1)
        fadd __d_v1
        fstp __d_v1
        ;__d_v2:=__d_v2+__d_dvac*(-__d_a2)
        fld __d_dvac
        fmul st(0),st(1)
        fadd __d_v2
        fstp __d_v2
        ffree st(0)
        fincstp

        mov ecx,__d_SHeight
        mov __d_y,0

        ;b2>=SHeight?
        fld __d_b2
        fistp __d_int
        cmp __d_int,ecx
        jge DrawTexTriangle8_b2m0

        ;b2<SHeight
        ;ecx:=b2
        fld __d_b2
        fistp __d_int
        mov ecx,__d_int
        inc ecx

        jmp DrawTexTriangle8_b2m0

DrawTexTriangle8_a2w0:

        ;a1>=0
        ;ecx:=b2-a2
        fld __d_b2
        fsub __d_a2
        fistp __d_int
        mov ecx,__d_int
        inc ecx
        ;__d_y:=a2
        fld __d_a2
        fistp __d_y

DrawTexTriangle8_a2d0:

        ;b2<SHeight?
        fld __d_b2
        fistp __d_int
        mov eax,__d_SHeight
        cmp __d_int,eax
        jl DrawTexTriangle8_b2m0

        ;b2>=SHeight
        mov eax,__d_y
        mov ecx,__d_SHeight
        sub ecx,eax

DrawTexTriangle8_b2m0:


        ;RYSOWANIE AB-AC

        ;ecx<1?
        cmp ecx,1
        jl DrawTexTriangle8_bcacs
        ;__d_cnt:=il. lini (ecx)
        mov __d_cnt,ecx
        mov ebx,__d_y
        cld
        debug_rdtsc t3,1
        debug_rdtsc t4,0

        ;dxab>dxac? (rysowanie od AC do AB)
        fld __d_dxab
        fcomp __d_dxac
        fstsw ax
        test ax,4500h
        jnz @f ;DrawTexTriangle8_drawabac
        ;dxab>dxac
        ;zamiana x1 z x2 oraz u1 z u2 i v1 z v2
        fld __d_x1
        fld __d_x2
        fstp __d_x1
        fstp __d_x2
        fld __d_u1
        fld __d_u2
        fstp __d_u1
        fstp __d_u2
        fld __d_v1
        fld __d_v2
        fstp __d_v1
        fstp __d_v2
        ;zamiana dxab z dxac oraz duab z duac i dvab z dvac
        fld __d_dxab
        fld __d_dxac
        fstp __d_dxab
        fstp __d_dxac
        fld __d_duab
        fld __d_duac
        fstp __d_duab
        fstp __d_duac
        fld __d_dvab
        fld __d_dvac
        fstp __d_dvab
        fstp __d_dvac
        mov __d_minx,80000000h
        jmp DrawTexTriangle8_drawabac
@@:
        mov __d_maxx,7fffffffh

        ;rysowanie trojkata od boku AB do AC
DrawTexTriangle8_drawabac:

                ;ix1:=x1; ix2:=x2 (float --> int)
                fld __d_x1
                fld __d_x2
                fistp __d_ix2
                fistp __d_ix1

                ;__d_du:=(u2-u1)/(x2-x1)
                fld __d_x2
                fsub __d_x1
                fld __d_u2
                fsub __d_u1
                fdiv st(0),st(1)
                fstp __d_du

                ;__d_dv:=(v2-v1)/(x2-x1)
                fld __d_v2
                fsub __d_v1
                fdiv st(0),st(1)
                fstp __d_dv
                ffree st(0)
                fincstp

                ;__d_u:=__d_u1\__d_v:=__d_v1
                fld __d_u1
                fld __d_v1
                fstp __d_v
                fstp __d_u

                ;__d_ix1>=__d_minx?
                mov eax,__d_minx
                cmp __d_ix1,eax
                jge DrawTexTriangle8_drawabacix1gminx
                mov __d_ix1,eax
DrawTexTriangle8_drawabacix1gminx:

                ;__d_ix2<=__d_maxx?
                mov eax,__d_maxx
                cmp __d_ix2,eax
                jle DrawTexTriangle8_drawabacix2lmaxx
                mov __d_ix2,eax
DrawTexTriangle8_drawabacix2lmaxx:

                ;ix2<SWidth?
                mov eax,__d_SWidth
                cmp __d_ix2,eax
                jl DrawTexTriangle8_drawabacx2mSW

                ;ix1>=0?
                cmp __d_ix1,-1
                jg DrawTexTriangle8_drawabacx1w0_1
                ;x1<0 i x2>SWidth
                mov ecx,eax

                ;u:=u+du*(-x1)
                fld __d_x1
                fchs
                fmul __d_du
                fadd __d_u
                fstp __d_u
                ;v:=v+dv*(-x1)
                fld __d_x1
                fchs
                fmul __d_dv
                fadd __d_v
                fstp __d_v
                mov __d_ix1,0
                jmp DrawTexTriangle8_drawabacx1w0_3

DrawTexTriangle8_drawabacx1w0_1:
                ;x1>=0 i x2>SWidth
                mov ecx,eax
                sub ecx,__d_ix1
                jmp DrawTexTriangle8_drawabacx1w0_2

DrawTexTriangle8_drawabacx2mSW:
                ;x2<SWidth
                mov ecx,__d_ix2
                sub ecx,__d_ix1

                ;x1<0?
                cmp __d_ix1,-1
                jg DrawTexTriangle8_drawabacx1w0_2
                ;x1<0
                add ecx,__d_ix1

                ;u:=u+du*(-x1)
                fld __d_x1
                fchs
                fmul __d_du
                fadd __d_u
                fstp __d_u
                ;v:=v+dv*(-x1)
                fld __d_x1
                fchs
                fmul __d_dv
                fadd __d_v
                fstp __d_v
                mov __d_ix1,0
                jmp DrawTexTriangle8_drawabacx1w0_3

DrawTexTriangle8_drawabacx1w0_2:
                ;u:=u+du*(ix1-x1)
                fild __d_ix1
                fsub __d_x1
                fld __d_du
                fmul st(0),st(1)
                fadd __d_u
                fstp __d_u

                ;v:=v+dv*(ix1-x1)
                fld __d_dv
                fmulp st(1),st(0)
                fadd __d_v
                fstp __d_v

DrawTexTriangle8_drawabacx1w0_3:
                ;ecx<0?
                cmp ecx,1
                jl DrawTexTriangle8_drawabacd1
                fld __d_u
                fld __d_v
                fld __d_du
                fld __d_dv
                fmul __d_65536
                fistp __d_dv
                fmul __d_65536
                fistp __d_du
                fmul __d_65536
                fistp __d_v
                fmul __d_65536
                fistp __d_u

                ;poczatek lini
                mov edi,__d_SBits
                mul ebx
                add eax,__d_ix1
                add edi,eax
                mov edx,__d_TBits
                mov __d_cntx,ecx
                mov ecx,__d_u
                mov ebp,__d_v

DrawTexTriangle8_line1loop:

                        ;obliczenie poczatku lini
                        mov eax,ebp
                        mov esi,ecx
                        shr eax,16
__DTT8SHL1:             shl eax,0
                        shr esi,16
                        add esi,eax
__DTT8AND1:             and esi,012345678h
                        mov al,es:[edx+esi]
                        mov es:[edi],al

                        ;u:=u+du\v:=v+dv
                        add ecx,__d_du
                        add ebp,__d_dv

                        inc edi
                        dec __d_cntx
                        jnz DrawTexTriangle8_line1loop

DrawTexTriangle8_drawabacd1:

                ;x1:=x1+dxab
                fld __d_x1
                fld __d_x2
                fld __d_u1
                fld __d_u2
                fld __d_v1
                fld __d_v2
                fadd __d_dvac
                fstp __d_v2
                fadd __d_dvab
                fstp __d_v1
                fadd __d_duac
                fstp __d_u2
                fadd __d_duab
                fstp __d_u1
                fadd __d_dxac
                fstp __d_x2
                fadd __d_dxab
                fstp __d_x1

                inc ebx
                dec __d_cnt

        jnz DrawTexTriangle8_drawabac

        debug_rdtsc t4,1

DrawTexTriangle8_bcacs:

        ;CZY RYSOWAC BOK BC-AC
        fld __d_c2
        fistp __d_int
        mov eax,__d_int
        fld __d_b2
        fistp __d_int
        cmp eax,__d_int
        je DrawTexTriangle8_endp
        mov eax,__d_SHeight
        cmp __d_int,eax
        jge DrawTexTriangle8_endp

        ;dxac:=dxac2
        fld __d_dxac2
        fstp __d_dxac

        ;duac:=duac2
        fld __d_duac2
        fstp __d_duac        

        ;dvac:=dvac2
        fld __d_dvac2
        fstp __d_dvac        

        ;WSPOLCZYNNIKI BC
        ;__d_dxbc:=(c1-b1)/(c2-b2)
        fld __d_c1
        fsub __d_b1
        fld __d_c2
        fsub __d_b2
        fdivp st(1),st(0)
        fstp __d_dxbc
        ;__d_dubc:=(cu-bu)/(c2-b2)
        fld __d_cu
        fsub __d_bu
        fld __d_c2
        fsub __d_b2
        fdivp st(1),st(0)
        fstp __d_dubc
        ;__d_dvbc:=(cv-bv)/(c2-b2)
        fld __d_cv
        fsub __d_bv
        fld __d_c2
        fsub __d_b2
        fdivp st(1),st(0)
        fstp __d_dvbc

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

        ;__d_u1:=bu
        fld __d_bu
        fstp __d_u1
        ;__d_u2:=au+(b2-a2)*__d_duac
        fld __d_b2
        fsub __d_a2
        fmul __d_duac
        fadd __d_au
        fstp __d_u2

        ;__d_v1:=bv
        fld __d_bv
        fstp __d_v1
        ;__d_v2:=av+(b2-a2)*__d_dvac
        fld __d_b2
        fsub __d_a2
        fmul __d_dvac
        fadd __d_av
        fstp __d_v2

        ;b2>=0?
        fld __d_b2
        fistp __d_int
        cmp __d_int,0
        jge DrawTexTriangle8_b2w0

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
        mov ecx,__d_SHeight
        mov __d_y,0

        ;__d_u1:=__d_u1+__d_dubc*(-__d_b2)
        fld __d_b2
        fchs
        fld __d_dubc
        fmul st(0),st(1)
        fadd __d_u1
        fstp __d_u1
        ;__d_u2:=__d_u2+__d_duac*(-__d_b2)
        fld __d_duac
        fmul st(0),st(1)
        fadd __d_u2
        fstp __d_u2
        ;__d_v1:=__d_v1+__d_dvbc*(-__d_b2)
        fld __d_dvbc
        fmul st(0),st(1)
        fadd __d_v1
        fstp __d_v1
        ;__d_v2:=__d_v2+__d_dvac*(-__d_b2)
        fld __d_dvac
        fmul st(0),st(1)
        fadd __d_v2
        fstp __d_v2
        ffree st(0)
        fincstp

        ;c2>=SHeight?
        fld __d_c2
        fistp __d_int
        cmp __d_int,ecx
        jge DrawTexTriangle8_c2m0

        ;c2<SHeight
        ;ecx:=c2
        fld __d_c2
        fistp __d_int
        mov ecx,__d_int
        inc ecx

        jmp DrawTexTriangle8_c2m0

DrawTexTriangle8_b2w0:

        ;c1>=0
        ;ecx:=c2-b2
        fld __d_c2
        fsub __d_b2
        fistp __d_int
        mov ecx,__d_int
        inc ecx
        ;__d_y:=b2
        fld __d_b2
        fistp __d_y

DrawTexTriangle8_b2d0:

        ;c2<SHeight?
        fld __d_c2
        fistp __d_int
        mov eax,__d_SHeight
        cmp __d_int,eax
        jl DrawTexTriangle8_c2m0

        ;c2>=SHeight
        mov eax,__d_y
        mov ecx,__d_SHeight
        sub ecx,eax

DrawTexTriangle8_c2m0:


        ;RYSOWANIE BC-AC

        ;ecx<1?
        cmp ecx,1
        jl DrawTexTriangle8_endp
        ;__d_cnt:=il. lini (ecx)
        mov __d_cnt,ecx
        mov ebx,__d_y
        cld

        ;dxac>dxbc? (rysowanie od AC do BC)
        fld __d_dxac
        fcomp __d_dxbc
        fstsw ax
        test ax,4500h
        jnz DrawTexTriangle8_drawbcac
        ;dxac>dxbc
        ;zamiana x1 z x2 oraz u1 z u2 i v1 z v2
        fld __d_x1
        fld __d_x2
        fstp __d_x1
        fstp __d_x2
        fld __d_u1
        fld __d_u2
        fstp __d_u1
        fstp __d_u2
        fld __d_v1
        fld __d_v2
        fstp __d_v1
        fstp __d_v2
        ;zamiana dxac z dxac oraz dubc z duac i dvbc z dvac
        fld __d_dxbc
        fld __d_dxac
        fstp __d_dxbc
        fstp __d_dxac
        fld __d_dubc
        fld __d_duac
        fstp __d_dubc
        fstp __d_duac
        fld __d_dvbc
        fld __d_dvac
        fstp __d_dvbc
        fstp __d_dvac

        ;rysowanie trojkata od boku BC do AC
DrawTexTriangle8_drawbcac:

                ;ix1:=x1; ix2:=x2 (float --> int)
                fld __d_x1                
                fld __d_x2
                fistp __d_ix2
                fistp __d_ix1

                ;__d_du:=(u2-u1)/(x2-x1)
                fld __d_x2
                fsub __d_x1
                fld __d_u2
                fsub __d_u1
                fdiv st(0),st(1)
                fstp __d_du

                ;__d_dv:=(v2-v1)/(x2-x1)
                fld __d_v2
                fsub __d_v1
                fdiv st(0),st(1)
                fstp __d_dv
                ffree st(0)
                fincstp

                ;__d_u:=__d_u1\__d_v:=__d_v1
                fld __d_u1
                fld __d_v1
                fstp __d_v
                fstp __d_u

                ;ix2<SWidth?
                mov eax,__d_SWidth
                cmp __d_ix2,eax
                jl DrawTexTriangle8_drawbcacx2mSW

                ;ix1>=0?
                cmp __d_ix1,-1
                jg DrawTexTriangle8_drawbcacx1w0_1
                ;x1<0 i x2>SWidth
                mov ecx,eax

                ;u:=u+du*(-x1)
                fld __d_x1
                fchs
                fmul __d_du
                fadd __d_u
                fstp __d_u
                ;v:=v+dv*(-x1)
                fld __d_x1
                fchs
                fmul __d_dv
                fadd __d_v
                fstp __d_v
                mov __d_ix1,0
                jmp DrawTexTriangle8_drawbcacx1w0_3

DrawTexTriangle8_drawbcacx1w0_1:
                ;x1>=0 i x2>SWidth
                mov ecx,eax
                sub ecx,__d_ix1
                jmp DrawTexTriangle8_drawbcacx1w0_2

DrawTexTriangle8_drawbcacx2mSW:
                ;x2<SWidth
                mov ecx,__d_ix2
                sub ecx,__d_ix1

                ;x1<0?
                cmp __d_ix1,-1
                jg DrawTexTriangle8_drawbcacx1w0_2
                ;x1<0
                add ecx,__d_ix1

                ;u:=u+du*(-x1)
                fld __d_x1
                fchs
                fmul __d_du
                fadd __d_u
                fstp __d_u
                ;v:=v+dv*(-x1)
                fld __d_x1
                fchs
                fmul __d_dv
                fadd __d_v
                fstp __d_v
                mov __d_ix1,0
                jmp DrawTexTriangle8_drawbcacx1w0_3

DrawTexTriangle8_drawbcacx1w0_2:
                ;u:=u+du*(ix1-x1)
                fild __d_ix1
                fsub __d_x1
                fld __d_du
                fmul st(0),st(1)
                fadd __d_u
                fstp __d_u

                ;v:=v+dv*(ix1-x1)
                fld __d_dv
                fmulp st(1),st(0)
                fadd __d_v
                fstp __d_v

DrawTexTriangle8_drawbcacx1w0_3:

                ;ecx<0?
                cmp ecx,1
                jl DrawTexTriangle8_drawbcacd1

                ;poczatek lini
                mov edi,__d_SBits
                mul ebx
                add eax,__d_ix1
                add edi,eax
                mov edx,__d_TBits

DrawTexTriangle8_line2loop:

                        ;obliczenie poczatku lini
                        fld __d_u
                        fld __d_v
                        fistp __d_iv
                        fistp __d_iu
                        mov eax,__d_iv
                        mov esi,__d_iu
__DTT8SHL2:             shl eax,0
                        add esi,eax
__DTT8AND2:             and esi,012345678h
                        add esi,edx
                        mov al,es:[esi]
                        stosb

                        ;u:=u+du\v:=v+dv
                        fld __d_u
                        fadd __d_du
                        fld __d_v
                        fadd __d_dv
                        fstp __d_v
                        fstp __d_u

                        loop DrawTexTriangle8_line2loop

DrawTexTriangle8_drawbcacd1:

                ;x1:=x1+dxbc
                fld __d_x1
                fadd __d_dxbc
                fstp __d_x1

                ;x2:=x2+dxac
                fld __d_x2
                fadd __d_dxac
                fstp __d_x2

                ;u1:=u1+dubc
                fld __d_u1
                fadd __d_dubc
                fstp __d_u1

                ;u2:=u2+duac
                fld __d_u2
                fadd __d_duac
                fstp __d_u2

                ;v1:=v1+dubc
                fld __d_v1
                fadd __d_dvbc
                fstp __d_v1

                ;v2:=v2+dvac
                fld __d_v2
                fadd __d_dvac
                fstp __d_v2

                inc ebx
                dec __d_cnt

        jnz DrawTexTriangle8_drawbcac
DrawTexTriangle8_endp:
        debug_rdtsc t1,1
        ret
DrawTexTriangle8 endp
comment#
        ;a1>b1?
        fld __d_a1
        fcomp __d_b1
        fstsw ax
        test ax,4500h
        jz DrawTexTriangle8_a1wb1

        ;b1>c1? dla a1<=b1
        fld __d_b1
        fcomp __d_c1
        fstsw ax
        test ax,4500h
        jz DrawTexTriangle8_a1mb1_b1wc1

        ;a1<=b1 i b1<=c1
        fld __d_a1
        fistp __d_minx
        fld __d_c1
        fistp __d_maxx
        jmp DrawTexTriangle8_endminmax

DrawTexTriangle8_a1mb1_b1wc1:
        ;a1<=b1 i b1>c1
        fld __d_b1
        fistp __d_maxx
        ;a1>c1? dla a1<=b1 i b1>c1
        fld __d_a1
        fcomp __d_c1
        fstsw ax
        test ax,4500h
        jz DrawTexTriangle8_a1mb1_a1wc1

        ;a1<=b1, b1>c1 i a1<=c1
        fld __d_a1
        fistp __d_minx
        jmp DrawTexTriangle8_endminmax

DrawTexTriangle8_a1mb1_a1wc1:
        ;a1<=b1, b1>c1 i a1>c1
        fld __d_c1
        fistp __d_minx
        jmp DrawTexTriangle8_endminmax

DrawTexTriangle8_a1wb1:
        ;b1>c1? dla a1>b1
        fld __d_b1
        fcomp __d_c1
        fstsw ax
        test ax,4500h
        jz DrawTexTriangle8_a1wb1_b1wc1

        ;a1>b1 i b1<=c1
        fld __d_b1
        fistp __d_minx
        ;a1>c1? dla a1>b1 i b1<=c1
        fld __d_a1
        fcomp __d_c1
        fstsw ax
        test ax,4500h
        jz DrawTexTriangle8_a1wb1_a1wc1

        ;a1>b1, b1>c1 i a1<=c1
        fld __d_c1
        fistp __d_maxx
        jmp DrawTexTriangle8_endminmax

DrawTexTriangle8_a1wb1_a1wc1:
        ;a1<=b1, b1>c1 i a1>c1
        fld __d_a1
        fistp __d_maxx
        jmp DrawTexTriangle8_endminmax

DrawTexTriangle8_a1wb1_b1wc1:
        ;a1>b1 i b1>c1
        fld __d_c1
        fistp __d_minx
        fld __d_a1
        fistp __d_maxx
        
DrawTexTriangle8_endminmax:#


ENDIF
