IFNDEF __FADE8_INC
__FADE8_INC=1
;FADE8.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do obslugi tablicy fade dla 256 kolorow

;Includy

include mstdlib\mmem.inc

;dane

FADE8Tab dd NULL
FADE8TabShift dd 0
FADE8TabSize dd 0

;Funkcje

;--------------------------------
;InitFADE8Tab (shf:dword,pal:dword)
;       Inicjuje tablice FADE8Tab dla danej palety
;       Wejscie: shf-liczba elementow cieniowania tablicy w postaci 1<<shf
;                pal-paleta kolorow (RGB)
;       Wyjscie: eax=0 blad/eax=1 ok
;--------------------------------

InitFADE8Tab proc stdcall uses eax ebx ecx edx,shf:dword,pal:dword
        local color:dword,i:dword,j:dword,tabsize:dword,tabsized2:dword
        local deltar:real4,deltag:real4,deltab:real4,curr:real4,curg:real4,curb:real4
        local c_r:dword,c_g:dword,c_b:dword
        local epsr:dword,epsg:dword,epsb:dword
        local currsum:dword,currc:dword
        local _64:dword
        ;
        mov ecx,shf
        mov FADE8TabShift,ecx
        mov eax,1
        shl eax,cl
        mov FADE8TabSize,eax
        mov ebx,256
        mul ebx
        mov tabsize,eax
        mov eax,FADE8TabSize
        dec eax
        shr eax,1
        mov tabsized2,eax
        ;
        mov eax,FADE8Tab
        cmp eax,NULL
        jne InitFADE8Tab_noam
        ;
        invoke AllocMem,tabsize
        cmp eax,NULL
        je InitFADE8Tab_endpe
        mov FADE8Tab,eax
InitFADE8Tab_noam:
        mov ebx,eax
        mov color,0
InitFADE8Tab_loop_color:
                mov eax,color
                shl eax,cl
                mov edx,ebx
                add edx,eax
                push edx
                add edx,FADE8TabSize
                dec edx
                mov eax,color
                mov fs:[edx],al
                ;
                pop edx
                add edx,tabsized2
                mov fs:[edx],al
                ;
                ;czesc 1
                mov edx,color
                shl edx,1
                add edx,color
                add edx,pal
                xor eax,eax
                mov al,fs:[edx]
                mov c_r,eax
                mov al,fs:[edx+1]
                mov c_g,eax
                mov al,fs:[edx+2]
                mov c_b,eax
                ;
                fild c_r
                fidiv tabsized2
                fstp deltar
                fild c_g
                fidiv tabsized2
                fstp deltag
                fild c_b
                fidiv tabsized2
                fstp deltab
                ;
                fldz
                fstp curr
                fldz
                fstp curg
                fldz
                fstp curb
                ;
                mov i,0
InitFADE8Tab_loop_i:
                        xor eax,eax
                        mov j,eax
                        mov currc,eax
                        not eax
                        mov currsum,eax                        
InitFADE8Tab_loop_j:
                                mov edx,j
                                shl edx,1
                                add edx,j
                                add edx,pal
                                xor eax,eax
                                mov al,fs:[edx]
                                mov c_r,eax
                                mov al,fs:[edx+1]
                                mov c_g,eax
                                mov al,fs:[edx+2]
                                mov c_b,eax
                                ;
                                fld curr
                                fisub c_r
                                fabs
                                fistp epsr
                                ;
                                fld curg
                                fisub c_g
                                fabs
                                fistp epsg
                                ;
                                fld curb
                                fisub c_b
                                fabs
                                fistp epsb
                                mov eax,epsr
                                add eax,epsg
                                add eax,epsb
                                cmp eax,currsum
                                ja InitFADE8Tab_no_set_new_sum
                                mov currsum,eax
                                mov eax,j
                                mov currc,eax
InitFADE8Tab_no_set_new_sum:
                                inc j
                                cmp j,256
                                jb InitFADE8Tab_loop_j
                        mov eax,color
                        shl eax,cl
                        mov edx,ebx
                        add edx,eax
                        add edx,i
                        mov eax,currc
                        mov fs:[edx],al
                        ;
                        fld curr
                        fadd deltar
                        fstp curr
                        ;
                        fld curg
                        fadd deltag
                        fstp curg
                        ;
                        fld curb
                        fadd deltab
                        fstp curb
                        ;
                        inc i
                        mov eax,tabsized2
                        cmp i,eax
                        jb InitFADE8Tab_loop_i
                ;
                ;czesc 2
                mov edx,color
                shl edx,1
                add edx,color
                add edx,pal
                xor eax,eax
                mov al,64
                sub al,fs:[edx]
                mov c_r,eax
                mov al,64
                sub al,fs:[edx+1]
                mov c_g,eax
                mov al,64
                sub al,fs:[edx+2]
                mov c_b,eax
                ;
                fild c_r
                fidiv tabsized2
                fstp deltar
                fild c_g
                fidiv tabsized2
                fstp deltag
                fild c_b
                fidiv tabsized2
                fstp deltab
                ;
                mov eax,64
                mov _64,eax
                fild _64
                fstp curr
                fild _64
                fstp curg
                fild _64
                fstp curb
                ;
                mov eax,FADE8TabSize
                sub eax,2
                mov i,eax
InitFADE8Tab_loop_i2:
                        xor eax,eax
                        mov j,eax
                        mov currc,eax
                        not eax
                        mov currsum,eax                        
InitFADE8Tab_loop_j2:
                                mov edx,j
                                shl edx,1
                                add edx,j
                                add edx,pal
                                xor eax,eax
                                mov al,fs:[edx]
                                mov c_r,eax
                                mov al,fs:[edx+1]
                                mov c_g,eax
                                mov al,fs:[edx+2]
                                mov c_b,eax
                                ;
                                fld curr
                                fisub c_r
                                fabs
                                fistp epsr
                                ;
                                fld curg
                                fisub c_g
                                fabs
                                fistp epsg
                                ;
                                fld curb
                                fisub c_b
                                fabs
                                fistp epsb
                                mov eax,epsr
                                add eax,epsg
                                add eax,epsb
                                cmp eax,currsum
                                ja InitFADE8Tab_no_set_new_sum2
                                mov currsum,eax
                                mov eax,j
                                mov currc,eax
InitFADE8Tab_no_set_new_sum2:
                                inc j
                                cmp j,256
                                jb InitFADE8Tab_loop_j2
                        mov eax,color
                        shl eax,cl
                        mov edx,ebx
                        add edx,eax
                        add edx,i
                        mov eax,currc
                        mov fs:[edx],al
                        ;
                        fld curr
                        fsub deltar
                        fstp curr
                        ;
                        fld curg
                        fsub deltag
                        fstp curg
                        ;
                        fld curb
                        fsub deltab
                        fstp curb
                        ;
                        dec i
                        mov eax,tabsized2
                        cmp i,eax
                        ja InitFADE8Tab_loop_i2
                inc color
                cmp color,256
                jb InitFADE8Tab_loop_color
        xor eax,eax
        inc eax
        jmp InitFADE8Tab_endp
InitFADE8Tab_endpe:
        xor eax,eax
InitFADE8Tab_endp:
        ret
InitFADE8Tab endp


ENDIF
