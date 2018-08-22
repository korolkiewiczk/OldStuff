IFNDEF __SURFACE8T_INC
__SURFACE8T_INC=1
;SURFACE8T.INC fo MASM/STUB32 by Kamil Korolkiewicz 2005
;Procedury do wyswietlania powierzchni 8-bitowych z przezroczystoscia i cieniowaniem

include surface.inc
include fade8.inc
include transp8.inc

;funkcje

;--------------------------------
;DrawSurface8T (eax,ebx,ecx,esi,edi)
;       Rysuje powierzchnie 8-bitowa na innej powierzchni 8-bitowej (np. ekranu) bez skalowania z cieniowaniem i z przezroczystoscia
;       Wejscie: eax-adres do prostokata (TRect) zrodlowego
;                ebx-adres do prostokata (TRect) docelowego
;                ecx-stopien cieniowania (w zaleznosci od FADE8TabSize w fade8.inc)
;                edx-stopien przezroczystosci (w zaleznosci od TRANSP8TabSize w transp8.inc)
;                esi-adres do zrodlowej struktory TSurface
;                edi-adres do docelowej struktory TSurface
;       Wyjscie: eax-bajty powierzchni docelowej lub NULL jesli nie bylo kopiowania
;--------------------------------

DrawSurface8T proc stdcall uses edi esi edx ecx ebx
        local srect:TRect,drect:TRect   ;prostokaty pomocnicze
        local ssjump,dsjump:dword       ;skoki dla poweirzchni zrodlowej i docelowej
        local srcbits,descbits:dword    ;wskazniki do powierzchni
        local repx,repx2,repy:dword     ;ilosc powt. x i y
        local sshfx,sshfy:dword         ;przesuniecie powierzchni zrodlowej
        local dshfx,dshfy:dword         ;przesuniecie powierzchni docelowej
        local transp:dword,t8tab:dword,sh1:dword,sh2:dword
        mov sshfx,0
        mov sshfy,0
        mov dshfx,0
        mov dshfy,0
        mov descbits,NULL
        mov transp,edx
        mov edx,FADE8TabShift
        mov sh1,edx
        mov edx,TRANSP8TabShift
        mov sh2,edx
        mov edx,TRANSP8Tab
        add edx,transp
        mov t8tab,edx
        cld

        ;Sprawdzenie czy prostokat zrodlowy nie wychodzi poza obszar powierzchni
        ;if (eax.RX<0)
        cmp dword ptr [eax],0           ;S.RX
        jl DrawSurface8T_SRXL0           ;S.RX<0
        mov edx,[eax]                   ;RX
        mov srect.RX,edx                ;RX
        jmp DrawSurface8T_jmp01          ;omin
DrawSurface8T_SRXL0:
        mov srect.RX,0                  ;0
        mov edx,[eax]                   ;S.RX
        neg edx                         ;-S.RX
        mov dshfx,edx

        ;if (eax.RY<0)
DrawSurface8T_jmp01:
        cmp dword ptr [eax+4],0         ;S.RY
        jl DrawSurface8T_SRYL0           ;S.RY<0
        mov edx,[eax+4]                 ;RY  
        mov srect.RY,edx                ;RY
        jmp DrawSurface8T_jmp02          ;omin
DrawSurface8T_SRYL0:
        mov srect.RY,0                  ;0
        mov edx,[eax+4]                 ;S.RY
        neg edx                         ;-S.RY
        mov dshfy,edx

        ;if (eax.RX+eax.RW>esi.SWidth)
DrawSurface8T_jmp02:
        mov edx,[eax+8]                 ;RW
        add edx,[eax]                   ;RX+RW
        cmp edx,[esi]                   ;SWidth
        jg DrawSurface8T_SRXGW           ;S.RX+S.RW>S.SWitdth
        sub edx,srect.RX                ;(RX+RW)-srect.RX
        mov srect.RW,edx
        jmp DrawSurface8T_jmp03          ;omin
DrawSurface8T_SRXGW:
        mov edx,[esi]                   ;SWidth
        sub edx,srect.RX                ;RX
        mov srect.RW,edx                ;SWidth-srect.RX
DrawSurface8T_jmp03:

        ;if (eax.RY+eax.RH>esi.SHeight)
        mov edx,[eax+12]                ;RH
        add edx,[eax+4]                 ;RY+RH
        cmp edx,[esi+4]                 ;SHeight
        jg DrawSurface8T_SRYGH           ;S.RX+S.RW>S.SHeight
        sub edx,srect.RY                ;(RY+RH)-srect.RY
        mov srect.RH,edx
        jmp DrawSurface8T_jmp04          ;omin
DrawSurface8T_SRYGH:
        mov edx,[esi+4]                 ;SHeight
        sub edx,srect.RY                ;RY
        mov srect.RH,edx                ;SHeight-srect.RY
DrawSurface8T_jmp04:
        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8T_end
        cmp srect.RH,0
        jle DrawSurface8T_end

        ;Sprawdzenie czy prostokat docelowy nie wychodzi poza obszar powierzchni
        ;if (ebx.RX<0)
        cmp dword ptr [ebx],0           ;D.RX
        jl DrawSurface8T_DRXL0           ;D.RX<0
        mov edx,[ebx]                   ;RX
        mov drect.RX,edx                ;RX
        jmp DrawSurface8T_jmp05          ;omin
DrawSurface8T_DRXL0:
        mov drect.RX,0                  ;0
        mov edx,[ebx]                   ;D.RX
        neg edx                         ;-D.RX
        mov sshfx,edx

        ;if (ebx.RY<0)
DrawSurface8T_jmp05:
        cmp dword ptr [ebx+4],0         ;D.RY
        jl DrawSurface8T_DRYL0           ;D.RY<0
        mov edx,[ebx+4]                 ;RY  
        mov drect.RY,edx                ;RY
        jmp DrawSurface8T_jmp06          ;omin
DrawSurface8T_DRYL0:
        mov drect.RY,0                  ;0
        mov edx,[ebx+4]                 ;D.RY
        neg edx                         ;-D.RY
        mov sshfy,edx

        ;if (ebx.RX+ebx.RW>edi.SWidth)
DrawSurface8T_jmp06:
        mov edx,[ebx+8]                 ;RW
        add edx,[ebx]                   ;RX+RW
        cmp edx,[edi]                   ;SWidth
        jg DrawSurface8T_DRXGW           ;D.RX+D.RW>D.SWitdth
        sub edx,drect.RX                ;(RX+RW)-drect.RX
        mov drect.RW,edx
        jmp DrawSurface8T_jmp07          ;omin
DrawSurface8T_DRXGW:
        mov edx,[edi]                   ;SWidth
        sub edx,drect.RX                ;RX
        mov drect.RW,edx                ;SWidth-drect.RX
DrawSurface8T_jmp07:

        ;if (ebx.RY+ebx.RH>edi.SHeight)
        mov edx,[ebx+12]                ;RH
        add edx,[ebx+4]                 ;RY+RH
        cmp edx,[edi+4]                 ;SHeight
        jg DrawSurface8T_DRYGH           ;D.RX+D.RW>D.SHeight
        sub edx,drect.RY                ;(RY+RH)-drect.RY
        mov drect.RH,edx
        jmp DrawSurface8T_jmp08          ;omin
DrawSurface8T_DRYGH:
        mov edx,[edi+4]                 ;SHeight
        sub edx,drect.RY                ;RY
        mov drect.RH,edx                ;SHeight-drect.RY
DrawSurface8T_jmp08:

        ;poprawinie szer i wys

        mov edx,sshfx
        sub srect.RW,edx
        mov edx,sshfy
        sub srect.RH,edx

        mov edx,dshfx
        sub drect.RW,edx
        mov edx,dshfy
        sub drect.RH,edx

        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8T_end
        cmp srect.RH,0
        jle DrawSurface8T_end
        
        ;czy prostokat docelowy jest w widoczny
        cmp drect.RW,0
        jle DrawSurface8T_end
        cmp drect.RH,0
        jle DrawSurface8T_end

        ;zapamietanie wskaznikow do bajtow powierzchni

        mov eax,[esi+13]                ;SBits
        mov srcbits,eax

        mov eax,[edi+13]                ;SBits
        mov descbits,eax
         
        ;ilosc powtorzen zalezy od tego ktora powierzchnia jest mniejsza dla danej osi

        ;os y
        mov eax,drect.RH
        cmp eax,srect.RH
        jg DrawSurface8T_repys          ;srect.RH>drect.RH
        mov repy,eax
        jmp DrawSurface8T_jmp09
DrawSurface8T_repys:
        mov eax,srect.RH
        mov repy,eax
DrawSurface8T_jmp09:

        ;os x
        mov eax,drect.RW
        cmp eax,srect.RW
        jg DrawSurface8T_repxs          ;srect.RW>drect.RW
        mov repx,eax
        jmp DrawSurface8T_jmp10
DrawSurface8T_repxs:
        mov eax,srect.RW
        mov repx,eax
DrawSurface8T_jmp10:

        ;obliczenie skoku zrodlowego
        mov eax,[esi]                   ;SWidth
        sub eax,repx                    ;SWidth-repx
        mov ssjump,eax

        ;obliczenie skoku docelowego
        mov eax,[edi]                   ;SWidth
        sub eax,repx                    ;SWidth-repx
        mov dsjump,eax

        ;obliczenie poczatku powierzchni zrodlowej
        ;eax=((srect.RY+sshfy)*s.SWidth)+srect.RX+sshfx
        mov eax,srect.RY
        add eax,sshfy
        mul dword ptr [esi]             ;SWidth
        add eax,srect.RX
        add eax,sshfx
        mov esi,eax

        ;obliczenie poczatku powierzchni docelowej
        ;eax=((drect.RY+dshfy)*d.SWidth)+drect.RX+dshfx
        mov eax,drect.RY
        add eax,dshfy
        mul dword ptr [edi]             ;SWidth
        add eax,drect.RX
        add eax,dshfx
        mov edi,eax

        add esi,srcbits
        add edi,descbits

        mov ebx,FADE8Tab
        add ebx,ecx

        ;Ustaw selektory
        push ds
        mov ax,stub32zerosel
        mov ds,ax

DrawSurface8T_drawloopY:
                mov eax,repx
                mov repx2,eax
DrawSurface8T_drawloopX:
                        mov ecx,sh1
                        xor eax,eax
                        mov al,[esi]
                        shl eax,cl
                        mov ecx,sh2
                        movzx eax,byte ptr [ebx+eax]
                        xor edx,edx
                        mov dl,[edi]
                        shl edx,cl
                        shl eax,8
                        shl eax,cl
                        add edx,eax
                        add edx,t8tab
                        mov al,[edx]
                        mov [edi],al
                        inc esi
                        inc edi
                        dec repx2
                        jnz DrawSurface8T_drawloopX
                add esi,ssjump
                add edi,dsjump
                dec repy
                jnz DrawSurface8T_drawloopY
        pop ds                  ;Przywroc selektor danych

DrawSurface8T_end:
        mov eax,descbits
        ret
DrawSurface8T endp

;--------------------------------
;DrawSurface8TS (eax,ebx,ecx,esi,edi)
;       Rysuje powierzchnie 8-bitowa na innej powierzchni 8-bitowej (np. ekranu) ze skalowaniem z cieniowaniem i z przezroczystoscia
;       Wejscie: eax-adres do prostokata (TRect) zrodlowego
;                ebx-adres do prostokata (TRect) docelowego
;                ecx-stopien cieniowania (w zaleznosci od FADE8TabSize w fade8.inc)
;                edx-stopien przezroczystosci (w zaleznosci od TRANSP8TabSize w transp8.inc)
;                esi-adres do zrodlowej struktory TSurface
;                edi-adres do docelowej struktory TSurface
;       Wyjscie: eax-bajty powierzchni docelowej lub NULL jesli nie bylo kopiowania
;       Uwaga: Rozmiary obu powierzchni musza sie miescic w przedziale 0-65535
;--------------------------------

DrawSurface8TS proc stdcall uses edi esi edx ecx ebx
        local srect:TRect,drect:TRect   ;prostokaty pomocnicze
        local dsjump:dword              ;skoki dla poweirzchni zrodlowej i docelowej
        local srcwidth:dword            ;pomocnicza szerokosc powierzchni zrodlowej
        local srcbits,descbits:dword    ;wskazniki do powierzchni
        local repx,repx2,repy:dword    ;ilosc powt. x i y
        local sshfx,sshfy:dword         ;przesuniecie powierzchni zrodlowej
        local dshfx,dshfy:dword         ;przesuniecie powierzchni docelowej
        local deshfx,deshfy:dword
        local wspx,wspy:dword           ;wspolczynniki przy skalowaniu powierzchni
        local pozy:dword                ;pozycja lini y
        local savepozx:qword            ;zapamietana pozycja lini x
        local fadetab:dword,tshf:dword,tshf2:dword,transptab:dword,transp:dword
        mov sshfx,0
        mov sshfy,0
        mov dshfx,0
        mov dshfy,0
        mov deshfx,0
        mov deshfy,0
        mov transp,edx
        mov edx,[esi]                   ;SWidth
        mov srcwidth,edx
        mov edx,FADE8Tab
        mov fadetab,edx
        add fadetab,ecx
        mov edx,TRANSP8Tab
        mov transptab,edx
        mov edx,transp
        add transptab,edx
        mov edx,FADE8TabShift
        mov tshf,edx
        mov edx,TRANSP8TabShift
        mov tshf2,edx
        mov descbits,NULL

        ;Sprawdzenie czy prostokat zrodlowy nie wychodzi poza obszar powierzchni
        ;if (eax.RX<0)
        cmp dword ptr [eax],0           ;S.RX
        jl DrawSurface8TS_SRXL0          ;S.RX<0
        mov edx,[eax]                   ;RX
        mov srect.RX,edx                ;RX
        jmp DrawSurface8TS_jmp01         ;omin
DrawSurface8TS_SRXL0:
        mov srect.RX,0                  ;0
        mov edx,[eax]                   ;S.RX
        neg edx                         ;-S.RX
        mov dshfx,edx

        ;if (eax.RY<0)
DrawSurface8TS_jmp01:
        cmp dword ptr [eax+4],0         ;S.RY
        jl DrawSurface8TS_SRYL0          ;S.RY<0
        mov edx,[eax+4]                 ;RY  
        mov srect.RY,edx                ;RY
        jmp DrawSurface8TS_jmp02         ;omin
DrawSurface8TS_SRYL0:
        mov srect.RY,0                  ;0
        mov edx,[eax+4]                 ;S.RY
        neg edx                         ;-S.RX
        mov dshfy,edx

        ;if (eax.RX+eax.RW>esi.SWidth)
DrawSurface8TS_jmp02:
        mov edx,[eax+8]                 ;RW
        add edx,[eax]                   ;RX+RW
        cmp edx,[esi]                   ;SWidth
        jg DrawSurface8TS_SRXGW          ;S.RX+S.RW>S.SWitdth
        sub edx,srect.RX                ;(RX+RW)-srect.RX
        mov srect.RW,edx
        jmp DrawSurface8TS_jmp03         ;omin
DrawSurface8TS_SRXGW:
        mov edx,[esi]                   ;SWidth
        sub edx,srect.RX                ;RX
        mov deshfx,1
        mov srect.RW,edx                ;SWidth-srect.RX
DrawSurface8TS_jmp03:

        ;if (eax.RY+eax.RH>esi.SHeight)
        mov edx,[eax+12]                ;RH
        add edx,[eax+4]                 ;RY+RH
        cmp edx,[esi+4]                 ;SHeight
        jg DrawSurface8TS_SRYGH          ;S.RX+S.RW>S.SHeight
        sub edx,srect.RY                ;(RY+RH)-srect.RY
        mov srect.RH,edx
        jmp DrawSurface8TS_jmp04         ;omin
DrawSurface8TS_SRYGH:
        mov edx,[esi+4]                 ;SHeight
        sub edx,srect.RY                ;RY
        mov deshfy,1
        mov srect.RH,edx                ;SHeight-srect.RY
DrawSurface8TS_jmp04:
        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8TS_end
        cmp srect.RH,0
        jle DrawSurface8TS_end

        ;Sprawdzenie czy prostokat docelowy nie wychodzi poza obszar powierzchni
        ;if (ebx.RX<0)
        cmp dword ptr [ebx],0           ;D.RX
        jl DrawSurface8TS_DRXL0          ;D.RX<0
        mov edx,[ebx]                   ;RX
        mov drect.RX,edx                ;RX
        jmp DrawSurface8TS_jmp05         ;omin
DrawSurface8TS_DRXL0:
        mov drect.RX,0                  ;0
        mov edx,[ebx]                   ;D.RX
        shl edx,16
        neg edx                         ;-D.RX
        mov sshfx,edx

        ;if (ebx.RY<0)
DrawSurface8TS_jmp05:
        cmp dword ptr [ebx+4],0         ;D.RY
        jl DrawSurface8TS_DRYL0          ;D.RY<0
        mov edx,[ebx+4]                 ;RY  
        mov drect.RY,edx                ;RY
        jmp DrawSurface8TS_jmp06         ;omin
DrawSurface8TS_DRYL0:
        mov drect.RY,0                  ;0
        mov edx,[ebx+4]                 ;D.RY
        shl edx,16
        neg edx                         ;-D.RY
        mov sshfy,edx

        ;if (ebx.RX+ebx.RW>edi.SWidth)
DrawSurface8TS_jmp06:
        mov edx,[ebx+8]                 ;RW
        add edx,[ebx]                   ;RX+RW
        cmp edx,[edi]                   ;SWidth
        jg DrawSurface8TS_DRXGW          ;D.RX+D.RW>D.SWitdth
        sub edx,drect.RX                ;(RX+RW)-drect.RX
        mov drect.RW,edx
        jmp DrawSurface8TS_jmp07         ;omin
DrawSurface8TS_DRXGW:
        mov edx,[edi]                   ;SWidth
        sub edx,drect.RX                ;RX
        mov drect.RW,edx                ;SWidth-drect.RX
DrawSurface8TS_jmp07:

        ;if (ebx.RY+ebx.RH>edi.SHeight)
        mov edx,[ebx+12]                ;RH
        add edx,[ebx+4]                 ;RY+RH
        cmp edx,[edi+4]                 ;SHeight
        jg DrawSurface8TS_DRYGH          ;D.RX+D.RW>D.SHeight
        sub edx,drect.RY                ;(RY+RH)-drect.RY
        mov drect.RH,edx
        jmp DrawSurface8TS_jmp08         ;omin
DrawSurface8TS_DRYGH:
        mov edx,[edi+4]                 ;SHeight
        sub edx,drect.RY                ;RY
        mov drect.RH,edx                ;SHeight-drect.RY
DrawSurface8TS_jmp08:

        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8TS_end
        cmp srect.RH,0
        jle DrawSurface8TS_end
        
        ;czy prostokat docelowy jest w widoczny
        cmp drect.RW,0
        jle DrawSurface8TS_end
        cmp drect.RH,0
        jle DrawSurface8TS_end

        ;wyliczenie wspolczynikow skalowania
        ;wspx = (eax.RW<<16/ebx.RW)
        ;wspy = (eax.RH<<16/ebx.RH)

        push eax
        mov eax,[eax+8]   ;RW
        shl eax,16
        xor edx,edx
        mov ecx,[ebx+8]   ;RW
        div ecx
        mov wspx,eax
        pop eax

        mov eax,[eax+12]  ;RH
        shl eax,16
        xor edx,edx
        mov ecx,[ebx+12]  ;RH
        div ecx
        mov wspy,eax

        ;poprawienie sshfx i sshfy
        ;sshfx=wspx*sshfx
        mov eax,wspx
        mov ebx,sshfx
        mul ebx
        shrd eax,edx,16
        mov sshfx,eax
        ;sshfy=wspy*sshfy
        mov eax,wspy
        mov ebx,sshfy
        mul ebx
        shrd eax,edx,16
        mov sshfy,eax

        ;poprawienie dshfx i dshfy
        ;dshfx=dshfx/wspx
        mov eax,dshfx
        shl eax,16
        mov ebx,wspx
        xor edx,edx
        div ebx
        mov dshfx,eax
        ;dshfy=dshfy/wspy
        mov eax,dshfy
        shl eax,16
        mov ebx,wspy
        xor edx,edx
        div ebx
        mov dshfy,eax

        ;poprawienie deshfx i deshfy
        cmp deshfx,0
        je DrawSurface8TS_deshfx_e_0
        mov eax,srect.RW
        shl eax,16
        mov ebx,wspx
        xor edx,edx
        div ebx
        mov ebx,drect.RW
        sub ebx,eax
        mov deshfx,ebx
DrawSurface8TS_deshfx_e_0:
        cmp deshfy,0
        je DrawSurface8TS_deshfy_e_0
        mov eax,srect.RH
        shl eax,16
        mov ebx,wspy
        xor edx,edx
        div ebx
        mov ebx,drect.RH
        sub ebx,eax
        mov deshfy,ebx
DrawSurface8TS_deshfy_e_0:

        ;zainicjowanie repx i repy
        ;repx=drect.RW-dshfx-deshfx
        ;repx=drect.RH-dshfy-deshfy

        mov eax,drect.RW
        sub eax,dshfx
        sub eax,deshfx
        mov repx,eax
        mov eax,drect.RH
        sub eax,dshfy
        sub eax,deshfy
        mov repy,eax

        ;czy prostokat jest widoczny

        cmp repx,0
        jle DrawSurface8TS_end
        cmp repy,0
        jle DrawSurface8TS_end

        ;obliczenie skoku docelowego
        mov eax,[edi]                   ;SWidth
        sub eax,repx                    ;SWidth-repx
        mov dsjump,eax

        ;zapamietanie wskaznikow do bajtow powierzchni
        
        mov eax,[esi+13]                ;SBits
        mov srcbits,eax

        mov eax,[edi+13]                ;SBits
        mov descbits,eax

        ;oblicznie poczatku powierzchni zrodlowej
        ;eax=((srect.RY<<16+(sshfy and 0ffff0000)*s.SWidth)+srect.RX<<16+sshfx
        ;pozycja x pow.:edx:ebx
        mov eax,srect.RY
        shl eax,16
        mov ebx,sshfy
        and ebx,0ffff0000h
        add eax,ebx
        mul dword ptr [esi]             ;SWidth
        mov ebx,srect.RX
        shl ebx,16
        add eax,ebx
        adc edx,0
        add eax,sshfx
        adc edx,0
        mov ebx,eax                     ;edx:ebx
        mov esi,edx

        ;oblicznie poczatku powierzchni docelowej
        ;eax=((drect.RY+dshfy)*d.SWidth)+drect.RX+dshfx
        mov eax,drect.RY
        add eax,dshfy
        mul dword ptr [edi]             ;SWidth
        add eax,drect.RX
        add eax,dshfx
        mov edi,eax

        add edi,descbits

        ;Ustaw selektory
        push ds
        mov ax,stub32zerosel
        mov ds,ax

        ;wyliczenie pozy
        mov eax,sshfy
        xor edx,edx
        mov ecx,65536
        div ecx
        mov pozy,edx

        mov edx,esi

DrawSurface8TS_drawloopY:
        mov eax,repx
        mov repx2,eax
        mov dword ptr savepozx,ebx
        mov dword ptr savepozx+4,edx
DrawSurface8TS_drawloopX:
                mov esi,ebx
                shrd esi,edx,16
                add ebx,wspx
                adc edx,0
                add esi,srcbits
                xor eax,eax
                mov al,[esi]
                mov ecx,tshf
                mov esi,fadetab
                shl eax,cl
                movzx esi,byte ptr [eax+esi]
                mov ecx,tshf2
                movzx eax,byte ptr [edi]
                shl eax,cl
                shl esi,8
                shl esi,cl
                add esi,eax
                add esi,transptab
                mov al,[esi]
                mov [edi],al
                inc edi
                dec repx2
                jnz DrawSurface8TS_drawloopX
        mov ecx,wspy
        add pozy,ecx
        mov ecx,pozy
        cmp ecx,65535
        jna DrawSurface8TS_no_next_line

        ;jesli nastepna linia
        and ecx,0ffff0000h
        mov eax,srcwidth
        mul ecx
        mov esi,edx
        mov ebx,dword ptr savepozx
        mov edx,dword ptr savepozx+4
        add ebx,eax
        adc edx,esi
        sub pozy,ecx
        ;
        add edi,dsjump
        dec repy
        jnz DrawSurface8TS_drawloopY
        jmp DrawSurface8TS_end_draw

        ;jesli ta sama linia
DrawSurface8TS_no_next_line:
        ;
        add edi,dsjump
        mov ebx,dword ptr savepozx
        mov edx,dword ptr savepozx+4
        dec repy
        jnz DrawSurface8TS_drawloopY

DrawSurface8TS_end_draw:
        pop ds                  ;Przywroc selektor danych

DrawSurface8TS_end:
        mov eax,descbits
        ret
DrawSurface8TS endp

ENDIF
