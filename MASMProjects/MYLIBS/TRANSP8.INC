IFNDEF __TRANSP8_INC
__TRANSP8_INC=1
;TRANSP8.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do obslugi przezroczystosci w 256 kolorach

;includy

include mstdlib\mmem.inc

;dane

;[scol,dcol,fade]
TRANSP8Tab dd NULL
TRANSP8TabShift dd 0
TRANSP8TabSize dd 0

;Funkcje

;--------------------------------
;InitTRANSP8Tab (shf:dword,pal:dword)
;       Inicjuje TRANSP8Tab dla danego przesuniecia i palety
;       Wejscie: shf-rozmiar tabliy w postaci 1<<shf
;                pal-paleta kolorow
;       Wyjscie: eax=0 blad/eax=1 ok
;       Uwaga: Wymagane jest co najmniej 64k*(1<<shf) pamieci HEAP
;--------------------------------

InitTRANSP8Tab proc stdcall,shf:dword,pal:dword
        local tabsize:dword,tabsizes1:dword
        local color1:dword,color2:dword,i:dword,j:dword
        local wspc:real4,fcr:real4,fcg:real4,fcb:real4
        local c_r:dword,c_g:dword,c_b:dword,c_r1:dword,c_g1:dword,c_b1:dword
        local c_r2:dword,c_g2:dword,c_b2:dword
        local currc:dword,currsum:dword
        local epsr:dword,epsg:dword,epsb:dword
        mov ecx,shf
        mov TRANSP8TabShift,ecx
        mov eax,1
        shl eax,cl
        mov TRANSP8TabSize,eax
        dec eax
        mov tabsizes1,eax
        inc eax
        mov ebx,256
        mul ebx
        mov tabsize,eax
        mov eax,TRANSP8Tab
        cmp eax,NULL
        jne InitTRANSP8Tab_noam
        ;
        invoke AllocMem,tabsize
        cmp eax,NULL
        je InitTRANSP8Tab_endpe
        mov TRANSP8Tab,eax
InitTRANSP8Tab_noam:
        mov ebx,eax
        mov color1,0
InitTRANSP8Tab_loop_color1:
        mov color2,0
InitTRANSP8Tab_loop_color2:
                mov j,0
                mov edx,color1
                shl edx,1
                add edx,color1
                add edx,pal
                xor eax,eax
                mov al,fs:[edx]
                mov c_r1,eax
                mov al,fs:[edx+1]
                mov c_g1,eax
                mov al,fs:[edx+2]
                mov c_b1,eax
                ;
                mov edx,color2
                shl edx,1
                add edx,color2
                add edx,pal
                xor eax,eax
                mov al,fs:[edx]
                mov c_r2,eax
                mov al,fs:[edx+1]
                mov c_g2,eax
                mov al,fs:[edx+2]
                mov c_b2,eax
InitTRANSP8Tab_loop_j:
                        fild j
                        fidiv tabsizes1
                        fstp wspc
                        ;
                        fild c_r2
                        fisub c_r1
                        fmul wspc
                        fiadd c_r1
                        fstp fcr
                        ;
                        fild c_g2
                        fisub c_g1
                        fmul wspc
                        fiadd c_g1
                        fstp fcg
                        ;
                        fild c_b2
                        fisub c_b1
                        fmul wspc
                        fiadd c_b1
                        fstp fcb
                        ;
                        xor eax,eax
                        mov i,eax
                        mov currc,eax
                        not eax
                        mov currsum,eax
InitTRANSP8Tab_loop_i:
                                mov edx,i
                                shl edx,1
                                add edx,i
                                add edx,pal
                                xor eax,eax
                                mov al,fs:[edx]
                                mov c_r,eax
                                mov al,fs:[edx+1]
                                mov c_g,eax
                                mov al,fs:[edx+2]
                                mov c_b,eax
                                ;
                                fld fcr
                                fisub c_r
                                fabs
                                fistp epsr
                                ;
                                fld fcg
                                fisub c_g
                                fabs
                                fistp epsg
                                ;
                                fld fcb
                                fisub c_b
                                fabs
                                fistp epsb
                                ;
                                mov eax,epsr
                                add eax,epsg
                                add eax,epsb
                                cmp eax,currsum
                                ja InitTRANSP8Tab_no_set_new_sum
                                mov currsum,eax
                                mov eax,i
                                mov currc,eax
InitTRANSP8Tab_no_set_new_sum:
                                inc i
                                cmp i,256
                                jb InitTRANSP8Tab_loop_i
                        mov edx,color1
                        shl edx,8
                        shl edx,cl
                        mov eax,color2
                        shl eax,cl
                        add edx,eax
                        add edx,j
                        add edx,TRANSP8Tab
                        mov eax,currc
                        mov fs:[edx],al
                        inc j
                        mov eax,TRANSP8TabSize
                        cmp j,eax
                        jb InitTRANSP8Tab_loop_j
                inc color2
                cmp color2,256
                jb InitTRANSP8Tab_loop_color2
        inc color1
        cmp color1,256
        jb InitTRANSP8Tab_loop_color1
        xor eax,eax
        inc eax
        jmp InitTRANSP8Tab_endp
InitTRANSP8Tab_endpe:
        xor eax,eax
InitTRANSP8Tab_endp:
        ret
InitTRANSP8Tab endp

ENDIF
