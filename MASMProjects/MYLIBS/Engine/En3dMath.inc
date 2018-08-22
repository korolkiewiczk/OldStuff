IFNDEF __EN3DMATH_INC
__EN3DMATH_INC=1
;En3dMath-funkcje do obliczen matematycznych dla grafiki 3d.

;dane

En3dMathSinTab float 360 dup (0)
En3dMathCosTab float 360 dup (0)

;Funkcje

;--------------------------------
;InitSinCosTab ()
;       Inicjuje En3dMathSinTab i En3dMathCosTab
;--------------------------------

InitSinCosTab proc stdcall uses ecx esi
        local i:dword
        local _180:dword
        mov ecx,360
        mov i,0
        mov esi,offset En3dMathSinTab
        mov _180,180
        fldpi
        fidiv _180
InitSinCosTab_sin_loop:
                fild i
                fmul st(0),st(1)
                fsin
                fstp dword ptr [esi]
                add esi,sizeof (float)
                inc i
                loop InitSinCosTab_sin_loop
        mov ecx,360
        mov i,0
        mov esi,offset En3dMathCosTab
InitSinCosTab_cos_loop:
                fild i
                fmul st(0),st(1)
                fcos
                fstp dword ptr [esi]
                add esi,sizeof (float)
                inc i
                loop InitSinCosTab_cos_loop
        fstp i
        ret
InitSinCosTab endp

;--------------------------------
;GetVectorWidth (eax,ebx)
;       Pobiera dlugosc wektora
;       Wejscie: eax,ebx-offsety do wierzcholkow
;       Wyjscie: st(0)-dlugosc wektora
;       Operacja: sqrt (ebx*ebx-eax*eax)
;--------------------------------

GetVectorWidth proc stdcall
        assume eax:ptr EnVertex
        assume ebx:ptr EnVertex

        fld [ebx].x
        fsub [eax].x
        fmul st(0),st(0)

        fld [ebx].y
        fsub [eax].y
        fmul st(0),st(0)

        fld [ebx].z
        fsub [eax].z
        fmul st(0),st(0)

        faddp st(1),st(0)
        faddp st(1),st(0)

        fsqrt                   ;sqrt (([ebx].x-[eax].x)^2+([ebx].y-[eax].y)^2+([ebx].z-[eax].z)^2)

        ret

        assume eax:nothing
        assume ebx:nothing
GetVectorWidth endp

;--------------------------------
;AddVectors (eax,ebx,ecx)
;       Dodaje 2 wektory
;       Wejscie: eax,ebx-dodawane wektory
;                ecx-wektor wynikowy
;       Operacja: ecx=eax+ebx
;--------------------------------

AddVectors proc stdcall
        assume eax:ptr EnVertex
        assume ebx:ptr EnVertex
        assume ecx:ptr EnVertex
        
        fld [eax].x
        fadd [ebx].x
        fstp [ecx].x

        fld [eax].y
        fadd [ebx].y
        fstp [ecx].y

        fld [eax].z
        fadd [ebx].z
        fstp [ecx].z

        ret

        assume eax:nothing
        assume ebx:nothing
        assume ecx:nothing
AddVectors endp

;--------------------------------
;SubVectors (eax,ebx,ecx)
;       Odejmue 2 wektory
;       Wejscie: eax,ebx-odejmowane wektory
;                ecx-wektor wynikowy
;       Operacja: ecx=ebx-eax
;--------------------------------

SubVectors proc stdcall
        assume eax:ptr EnVertex
        assume ebx:ptr EnVertex
        assume ecx:ptr EnVertex
        
        fld [ebx].x
        fsub [eax].x
        fstp [ecx].x

        fld [ebx].y
        fsub [eax].y
        fstp [ecx].y

        fld [ebx].z
        fsub [eax].z
        fstp [ecx].z

        ret

        assume eax:nothing
        assume ebx:nothing
        assume ecx:nothing
SubVectors endp

;--------------------------------
;MulVecByVal (eax,ebx,st(0))
;       Mnozy wektor prze liczbe rzeczywista
;       Wescie: eax-wektor zrodlowy
;               ebx-wektor docelowy
;               st(0)-liczba
;       Operacja: ebx=eax*st(0)/zwalnia st(0)
;--------------------------------

MulVecByVal proc stdcall
        assume eax:ptr EnVertex
        assume ebx:ptr EnVertex
        
        fld [eax].x
        fmul st(0),st(1)
        fstp [ebx].x

        fld [eax].y
        fmul st(0),st(1)
        fstp [ebx].y

        fld [eax].z
        fmul st(0),st(1)
        fstp [ebx].z

        ffree st(0)
        fincstp

        ret

        assume eax:nothing
        assume ebx:nothing
MulVecByVal endp

;--------------------------------
;DotProduct (eax,ebx)
;       Oblicza iloczyn skalarny 2 wektorow
;       Wejscie: eax,ebx-wektory
;       Wyjscie: st(0)-iloczyn skalarny
;       Operacja: iloczyn_skalarny (eax,ebx)
;--------------------------------

DotProduct proc stdcall
        assume eax:ptr EnVertex
        assume ebx:ptr EnVertex
        
        fld [eax].x
        fmul [ebx].x

        fld [eax].x
        fmul [ebx].x

        fld [eax].x
        fmul [ebx].x

        faddp st(1),st(0)
        faddp st(1),st(0)

        ret
        assume eax:nothing
        assume ebx:nothing
DotProduct endp

;--------------------------------
;VecProduct (eax,ebx,ecx)
;       Oblicza wektorowe mnozenie 2 wektorow
;       Wejscie: eax,ebx-wektory
;       Wyjscie: ecx-wektor wyjsciowy
;       Operacja: ecx=wektorowe_mnozenie_wektorow (eax,ebx)
;--------------------------------

VecProduct proc stdcall
        assume eax:ptr EnVertex
        assume ebx:ptr EnVertex
        assume ecx:ptr EnVertex

        fld [eax].y
        fmul [ebx].z
        fld [eax].z
        fmul [ebx].y
        faddp st(1),st(0)
        fstp [ecx].x

        fld [eax].z
        fmul [ebx].x
        fld [eax].x
        fmul [ebx].z
        faddp st(1),st(0)
        fstp [ecx].y

        fld [eax].x
        fmul [ebx].y
        fld [eax].y
        fmul [ebx].x
        faddp st(1),st(0)
        fstp [ecx].z

        ret

        assume eax:nothing
        assume ebx:nothing
        assume ecx:nothing
VecProduct endp

;--------------------------------
;RotateVertexX (eax,ebx)
;       Obraca wierzcholek wokol osi x
;       Wejscie: eax-wierzcholek
;                ebx-kat (0..359)
;       Operacja: eax=obroc_wokol_wierzcholka_x (eax,ebx)
;--------------------------------

RotateVertexX proc stdcall
        assume eax:ptr EnVertex

        fld [eax].y
        fmul En3dMathCosTab[ebx*sizeof (float)]
        fld [eax].z
        fmul En3dMathSinTab[ebx*sizeof (float)]
        fsubp st(1),st(0)

        fld [eax].y
        fmul En3dMathSinTab[ebx*sizeof (float)]
        fld [eax].z
        fmul En3dMathCosTab[ebx*sizeof (float)]
        faddp st(1),st(0)

        fstp [eax].z
        fstp [eax].y

        ret

        assume eax:nothing
RotateVertexX endp

;--------------------------------
;RotateVertexY (eax,ebx)
;       Obraca wierzcholek wokol osi y
;       Wejscie: eax-wierzcholek
;                ebx-kat (0..359)
;       Operacja: eax=obroc_wokol_wierzcholka_y (eax,ebx)
;--------------------------------

RotateVertexY proc stdcall
        assume eax:ptr EnVertex

        fld [eax].z
        fmul En3dMathSinTab[ebx*sizeof (float)]
        fld [eax].x
        fmul En3dMathCosTab[ebx*sizeof (float)]
        faddp st(1),st(0)

        fld [eax].z
        fmul En3dMathCosTab[ebx*sizeof (float)]
        fld [eax].x
        fmul En3dMathSinTab[ebx*sizeof (float)]
        fsubp st(1),st(0)

        fstp [eax].z
        fstp [eax].x

        ret

        assume eax:nothing
RotateVertexY endp

;--------------------------------
;RotateVertexZ (eax,ebx)
;       Obraca wierzcholek wokol osi z
;       Wejscie: eax-wierzcholek
;                ebx-kat (0..359)
;       Operacja: eax=obroc_wokol_wierzcholka_z (eax,ebx)
;--------------------------------

RotateVertexZ proc stdcall
        assume eax:ptr EnVertex

        fld [eax].y
        fmul En3dMathSinTab[ebx*sizeof (float)]
        fld [eax].x
        fmul En3dMathCosTab[ebx*sizeof (float)]
        faddp st(1),st(0)

        fld [eax].y
        fmul En3dMathCosTab[ebx*sizeof (float)]
        fld [eax].x
        fmul En3dMathSinTab[ebx*sizeof (float)]
        fsubp st(1),st(0)

        fstp [eax].y
        fstp [eax].x

        ret

        assume eax:nothing
RotateVertexZ endp

;--------------------------------
;ShiftVertex (eax,st(0),st(1),st(2))
;       Przesuwa wierzcholek
;       Wejscie: eax-wierzcholek
;                st(0),st(1),st(2)-odpowiednio przesuniecie dla osi x,y,z
;       Operacja: eax=przesun_wierzcholek_xyz (eax,st(0),st(1),st(2))
;--------------------------------

ShiftVertex proc stdcall
        assume eax:ptr EnVertex

        fadd [eax].x
        fstp [eax].x

        fadd [eax].y
        fstp [eax].y

        fadd [eax].z
        fstp [eax].z

        ret

        assume eax:nothing
ShiftVertex endp

;--------------------------------
;ScaleVertex (eax,st(0))
;       Skaluje wierzcholek
;       Wejscie: eax-wierzcholek
;                st(0)-skala
;       Uwaga:st(0) jest zwalniane
;       Operacja: eax=skaluj_wierzcholek_xyz (eax,st(0))
;--------------------------------

ScaleVertex proc stdcall
        assume eax:ptr EnVertex

        fld [eax].x
        fmul st(0),st(1)
        fstp [eax].x

        fld [eax].y
        fmul st(0),st(1)
        fstp [eax].y

        fld [eax].z
        fmul st(0),st(1)
        fstp [eax].z

        ffree st(0)
        fincstp

        ret

        assume eax:nothing
ScaleVertex endp

;--------------------------------
;ConvertVertex3DTo2D (eax)
;       Rzutuje wierzcholek na powierzchnie
;       Wejscie: eax-wierzcholek
;       Operacja: eax=Rzutuj_wierzcholek (eax)
;--------------------------------       

ConvertVertex3DTo2D proc stdcall
        assume eax:ptr EnVertex

        cmp [eax].w,0
        jnz ConvertVertex3DTo2D_endp

        fld [eax].x
        fmul Engine_Camera_Dist
        fdiv [eax].z
        fadd Engine_Scr_XCenter
        fstp [eax].x

        fld [eax].y
        fmul Engine_Camera_Dist
        fdiv [eax].z
        fadd Engine_Scr_YCenter
        fstp [eax].y

ConvertVertex3DTo2D_endp:

        ret

        assume eax:nothing
ConvertVertex3DTo2D endp

;--------------------------------
;NormalizeVector (eax,ebx)
;       Normalizuje wektor
;       Wejscie: eax-wektor zrodlowy
;                ebx-wektor docelowy
;--------------------------------

NormalizeVector proc stdcall
        assume eax:ptr EnVertex
        assume ebx:ptr EnVertex
        local ___val:dword

        call GetVectorWidth
        ftst
        fstsw word ptr ___val
        test word ptr ___val,4000h
        jz NormalizeVector_d1
        fistp ___val
        inc ___val
        fild ___val
NormalizeVector_d1:

        fld [eax].x
        fdiv
        fstp [ebx].x

        fld [eax].y
        fdiv
        fstp [ebx].y

        fld [eax].z
        fdiv
        fstp [ebx].z

        ret

        assume eax:nothing
        assume ebx:nothing
NormalizeVector endp

;--------------------------------
;CopyVertexList (eax,ebx,ecx)
;       Kopiuje z eax do ebx ecx wiercholkow
;       Wejscie: eax,ebx-ofsety do wierzcholkow
;                ecx-ich liczba
;--------------------------------

CopyVertexList proc stdcall uses eax edx esi edi

        push ds
        pop es
        mov esi,eax
        mov edi,ebx
        mov eax,ecx
        mov ecx,sizeof (EnVertex)
        mul ecx
        mov ecx,eax
        shr ecx,2
        and eax,3
        rep movsd
        mov ecx,eax
        rep movsb

        ret
CopyVertexList endp

ENDIF
