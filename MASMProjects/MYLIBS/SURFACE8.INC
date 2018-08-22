IFNDEF __SURFACE8_INC
__SURFACE8_INC=1
;SURFACE8.INC fo MASM/STUB32 by Kamil Korolkiewicz 2005
;Procedury do wyswietlania powierzchni 8-bitowych

include surface.inc

;funkcje

;--------------------------------
;DrawSurface8 (eax,ebx,esi,edi)
;       Rysuje powierzchnie 8-bitowa na innej powierzchni 8-bitowej (np. ekranu) bez skalowania
;       Wejscie: eax-adres do prostokata (TRect) zrodlowego
;                ebx-adres do prostokata (TRect) docelowego
;                esi-adres do zrodlowej struktory TSurface
;                edi-adres do docelowej struktory TSurface
;       Wyjscie: eax-bajty powierzchni docelowej lub NULL jesli nie bylo kopiowania
;--------------------------------

DrawSurface8 proc stdcall uses edi esi edx ecx ebx
        local srect:TRect,drect:TRect   ;prostokaty pomocnicze
        local ssjump,dsjump:dword       ;skoki dla poweirzchni zrodlowej i docelowej
        local srcbits,descbits:dword    ;wskazniki do powierzchni
        local repx,repy:dword           ;ilosc powt. x i y
        local sshfx,sshfy:dword         ;przesuniecie powierzchni zrodlowej
        local dshfx,dshfy:dword         ;przesuniecie powierzchni docelowej
        local erepx:dword               ;zmienna pomocnicza
        mov sshfx,0
        mov sshfy,0
        mov dshfx,0
        mov dshfy,0
        mov descbits,NULL
        cld

        ;Sprawdzenie czy prostokat zrodlowy nie wychodzi poza obszar powierzchni
        ;if (eax.RX<0)
        cmp dword ptr [eax],0           ;S.RX
        jl DrawSurface8_SRXL0           ;S.RX<0
        mov edx,[eax]                   ;RX
        mov srect.RX,edx                ;RX
        jmp DrawSurface8_jmp01          ;omin
DrawSurface8_SRXL0:
        mov srect.RX,0                  ;0
        mov edx,[eax]                   ;S.RX
        neg edx                         ;-S.RX
        mov dshfx,edx

        ;if (eax.RY<0)
DrawSurface8_jmp01:
        cmp dword ptr [eax+4],0         ;S.RY
        jl DrawSurface8_SRYL0           ;S.RY<0
        mov edx,[eax+4]                 ;RY  
        mov srect.RY,edx                ;RY
        jmp DrawSurface8_jmp02          ;omin
DrawSurface8_SRYL0:
        mov srect.RY,0                  ;0
        mov edx,[eax+4]                 ;S.RY
        neg edx                         ;-S.RY
        mov dshfy,edx

        ;if (eax.RX+eax.RW>esi.SWidth)
DrawSurface8_jmp02:
        mov edx,[eax+8]                 ;RW
        add edx,[eax]                   ;RX+RW
        cmp edx,[esi]                   ;SWidth
        jg DrawSurface8_SRXGW           ;S.RX+S.RW>S.SWitdth
        sub edx,srect.RX                ;(RX+RW)-srect.RX
        mov srect.RW,edx
        jmp DrawSurface8_jmp03          ;omin
DrawSurface8_SRXGW:
        mov edx,[esi]                   ;SWidth
        sub edx,srect.RX                ;RX
        mov srect.RW,edx                ;SWidth-srect.RX
DrawSurface8_jmp03:

        ;if (eax.RY+eax.RH>esi.SHeight)
        mov edx,[eax+12]                ;RH
        add edx,[eax+4]                 ;RY+RH
        cmp edx,[esi+4]                 ;SHeight
        jg DrawSurface8_SRYGH           ;S.RX+S.RW>S.SHeight
        sub edx,srect.RY                ;(RY+RH)-srect.RY
        mov srect.RH,edx
        jmp DrawSurface8_jmp04          ;omin
DrawSurface8_SRYGH:
        mov edx,[esi+4]                 ;SHeight
        sub edx,srect.RY                ;RY
        mov srect.RH,edx                ;SHeight-srect.RY
DrawSurface8_jmp04:
        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8_end
        cmp srect.RH,0
        jle DrawSurface8_end

        ;Sprawdzenie czy prostokat docelowy nie wychodzi poza obszar powierzchni
        ;if (ebx.RX<0)
        cmp dword ptr [ebx],0           ;D.RX
        jl DrawSurface8_DRXL0           ;D.RX<0
        mov edx,[ebx]                   ;RX
        mov drect.RX,edx                ;RX
        jmp DrawSurface8_jmp05          ;omin
DrawSurface8_DRXL0:
        mov drect.RX,0                  ;0
        mov edx,[ebx]                   ;D.RX
        neg edx                         ;-D.RX
        mov sshfx,edx

        ;if (ebx.RY<0)
DrawSurface8_jmp05:
        cmp dword ptr [ebx+4],0         ;D.RY
        jl DrawSurface8_DRYL0           ;D.RY<0
        mov edx,[ebx+4]                 ;RY  
        mov drect.RY,edx                ;RY
        jmp DrawSurface8_jmp06          ;omin
DrawSurface8_DRYL0:
        mov drect.RY,0                  ;0
        mov edx,[ebx+4]                 ;D.RY
        neg edx                         ;-D.RY
        mov sshfy,edx

        ;if (ebx.RX+ebx.RW>edi.SWidth)
DrawSurface8_jmp06:
        mov edx,[ebx+8]                 ;RW
        add edx,[ebx]                   ;RX+RW
        cmp edx,[edi]                   ;SWidth
        jg DrawSurface8_DRXGW           ;D.RX+D.RW>D.SWitdth
        sub edx,drect.RX                ;(RX+RW)-drect.RX
        mov drect.RW,edx
        jmp DrawSurface8_jmp07          ;omin
DrawSurface8_DRXGW:
        mov edx,[edi]                   ;SWidth
        sub edx,drect.RX                ;RX
        mov drect.RW,edx                ;SWidth-drect.RX
DrawSurface8_jmp07:

        ;if (ebx.RY+ebx.RH>edi.SHeight)
        mov edx,[ebx+12]                ;RH
        add edx,[ebx+4]                 ;RY+RH
        cmp edx,[edi+4]                 ;SHeight
        jg DrawSurface8_DRYGH           ;D.RX+D.RW>D.SHeight
        sub edx,drect.RY                ;(RY+RH)-drect.RY
        mov drect.RH,edx
        jmp DrawSurface8_jmp08          ;omin
DrawSurface8_DRYGH:
        mov edx,[edi+4]                 ;SHeight
        sub edx,drect.RY                ;RY
        mov drect.RH,edx                ;SHeight-drect.RY
DrawSurface8_jmp08:

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
        jle DrawSurface8_end
        cmp srect.RH,0
        jle DrawSurface8_end
        
        ;czy prostokat docelowy jest w widoczny
        cmp drect.RW,0
        jle DrawSurface8_end
        cmp drect.RH,0
        jle DrawSurface8_end

        ;zapamietanie wskaznikow do bajtow powierzchni

        mov eax,[esi+13]                ;SBits
        mov srcbits,eax

        mov eax,[edi+13]                ;SBits
        mov descbits,eax
         
        ;ilosc powtorzen zalezy od tego ktora powierzchnia jest mniejsza dla danej osi

        ;os y
        mov eax,drect.RH
        cmp eax,srect.RH
        jg DrawSurface8_repys          ;srect.RH>drect.RH
        mov repy,eax
        jmp DrawSurface8_jmp09
DrawSurface8_repys:
        mov eax,srect.RH
        mov repy,eax
DrawSurface8_jmp09:

        ;os x
        mov eax,drect.RW
        cmp eax,srect.RW
        jg DrawSurface8_repxs          ;srect.RW>drect.RW
        mov repx,eax
        jmp DrawSurface8_jmp10
DrawSurface8_repxs:
        mov eax,srect.RW
        mov repx,eax
DrawSurface8_jmp10:

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

        ;Ustaw selektory
        push ds
        mov ax,stub32zerosel
        mov es,ax
        mov ds,ax

        ;przenies skoki do rejestrow dla uzyskania lepszej szybkosci
        mov eax,ssjump
        mov edx,dsjump

        mov ecx,repx
        and ecx,3
        mov erepx,ecx
        mov ecx,repx
        shr ecx,2
        mov repx,ecx

        mov ebx,repy

DrawSurface8_drawloopY:
                mov ecx,repx
                rep movsd       ;kopiuj powierzchnie
                mov ecx,erepx
                rep movsb       ;kopiuj pozostale bajty
                add esi,eax     ;ssjump
                add edi,edx     ;dsjump
        dec ebx
        jnz DrawSurface8_drawloopY

        pop ds                  ;Przywroc selektor danych

DrawSurface8_end:
        mov eax,descbits
        ret
DrawSurface8 endp

;--------------------------------
;DrawSurface8S (eax,ebx,esi,edi)
;       Rysuje powierzchnie 8-bitowa na innej powierzchni 8-bitowej (np. ekranu) ze skalowaniem
;       Wejscie: eax-adres do prostokata (TRect) zrodlowego
;                ebx-adres do prostokata (TRect) docelowego
;                esi-adres do zrodlowej struktory TSurface
;                edi-adres do docelowej struktory TSurface
;       Wyjscie: eax-bajty powierzchni docelowej lub NULL jesli nie bylo kopiowania
;       Uwaga: Rozmiary obu powierzchni musza sie miescic w przedziale 0-65535
;--------------------------------

DrawSurface8S proc stdcall uses edi esi edx ecx ebx
        local srect:TRect,drect:TRect   ;prostokaty pomocnicze
        local dsjump:dword              ;skoki dla poweirzchni zrodlowej i docelowej
        local srcwidth:dword            ;pomocnicza szerokosc powierzchni zrodlowej
        local srcbits,descbits:dword    ;wskazniki do powierzchni
        local repx,repy:dword           ;ilosc powt. x i y
        local sshfx,sshfy:dword         ;przesuniecie powierzchni zrodlowej
        local dshfx,dshfy:dword         ;przesuniecie powierzchni docelowej
        local deshfx,deshfy:dword
        local wspx,wspy:dword           ;wspolczynniki przy skalowaniu powierzchni
        local pozy:dword                ;pozycja lini y
        local savepozx:qword            ;zapamietana pozycja lini x
        mov sshfx,0
        mov sshfy,0
        mov dshfx,0
        mov dshfy,0
        mov deshfx,0
        mov deshfy,0
        mov edx,[esi]                   ;SWidth
        mov srcwidth,edx
        mov descbits,NULL

        ;Sprawdzenie czy prostokat zrodlowy nie wychodzi poza obszar powierzchni
        ;if (eax.RX<0)
        cmp dword ptr [eax],0           ;S.RX
        jl DrawSurface8S_SRXL0          ;S.RX<0
        mov edx,[eax]                   ;RX
        mov srect.RX,edx                ;RX
        jmp DrawSurface8S_jmp01         ;omin
DrawSurface8S_SRXL0:
        mov srect.RX,0                  ;0
        mov edx,[eax]                   ;S.RX
        neg edx                         ;-S.RX
        mov dshfx,edx

        ;if (eax.RY<0)
DrawSurface8S_jmp01:
        cmp dword ptr [eax+4],0         ;S.RY
        jl DrawSurface8S_SRYL0          ;S.RY<0
        mov edx,[eax+4]                 ;RY  
        mov srect.RY,edx                ;RY
        jmp DrawSurface8S_jmp02         ;omin
DrawSurface8S_SRYL0:
        mov srect.RY,0                  ;0
        mov edx,[eax+4]                 ;S.RY
        neg edx                         ;-S.RX
        mov dshfy,edx

        ;if (eax.RX+eax.RW>esi.SWidth)
DrawSurface8S_jmp02:
        mov edx,[eax+8]                 ;RW
        add edx,[eax]                   ;RX+RW
        cmp edx,[esi]                   ;SWidth
        jg DrawSurface8S_SRXGW          ;S.RX+S.RW>S.SWitdth
        sub edx,srect.RX                ;(RX+RW)-srect.RX
        mov srect.RW,edx
        jmp DrawSurface8S_jmp03         ;omin
DrawSurface8S_SRXGW:
        mov edx,[esi]                   ;SWidth
        sub edx,srect.RX                ;RX
        mov deshfx,1
        mov srect.RW,edx                ;SWidth-srect.RX
DrawSurface8S_jmp03:

        ;if (eax.RY+eax.RH>esi.SHeight)
        mov edx,[eax+12]                ;RH
        add edx,[eax+4]                 ;RY+RH
        cmp edx,[esi+4]                 ;SHeight
        jg DrawSurface8S_SRYGH          ;S.RX+S.RW>S.SHeight
        sub edx,srect.RY                ;(RY+RH)-srect.RY
        mov srect.RH,edx
        jmp DrawSurface8S_jmp04         ;omin
DrawSurface8S_SRYGH:
        mov edx,[esi+4]                 ;SHeight
        sub edx,srect.RY                ;RY
        mov deshfy,1
        mov srect.RH,edx                ;SHeight-srect.RY
DrawSurface8S_jmp04:
        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8S_end
        cmp srect.RH,0
        jle DrawSurface8S_end

        ;Sprawdzenie czy prostokat docelowy nie wychodzi poza obszar powierzchni
        ;if (ebx.RX<0)
        cmp dword ptr [ebx],0           ;D.RX
        jl DrawSurface8S_DRXL0          ;D.RX<0
        mov edx,[ebx]                   ;RX
        mov drect.RX,edx                ;RX
        jmp DrawSurface8S_jmp05         ;omin
DrawSurface8S_DRXL0:
        mov drect.RX,0                  ;0
        mov edx,[ebx]                   ;D.RX
        shl edx,16
        neg edx                         ;-D.RX
        mov sshfx,edx

        ;if (ebx.RY<0)
DrawSurface8S_jmp05:
        cmp dword ptr [ebx+4],0         ;D.RY
        jl DrawSurface8S_DRYL0          ;D.RY<0
        mov edx,[ebx+4]                 ;RY  
        mov drect.RY,edx                ;RY
        jmp DrawSurface8S_jmp06         ;omin
DrawSurface8S_DRYL0:
        mov drect.RY,0                  ;0
        mov edx,[ebx+4]                 ;D.RY
        shl edx,16
        neg edx                         ;-D.RY
        mov sshfy,edx

        ;if (ebx.RX+ebx.RW>edi.SWidth)
DrawSurface8S_jmp06:
        mov edx,[ebx+8]                 ;RW
        add edx,[ebx]                   ;RX+RW
        cmp edx,[edi]                   ;SWidth
        jg DrawSurface8S_DRXGW          ;D.RX+D.RW>D.SWitdth
        sub edx,drect.RX                ;(RX+RW)-drect.RX
        mov drect.RW,edx
        jmp DrawSurface8S_jmp07         ;omin
DrawSurface8S_DRXGW:
        mov edx,[edi]                   ;SWidth
        sub edx,drect.RX                ;RX
        mov drect.RW,edx                ;SWidth-drect.RX
DrawSurface8S_jmp07:

        ;if (ebx.RY+ebx.RH>edi.SHeight)
        mov edx,[ebx+12]                ;RH
        add edx,[ebx+4]                 ;RY+RH
        cmp edx,[edi+4]                 ;SHeight
        jg DrawSurface8S_DRYGH          ;D.RX+D.RW>D.SHeight
        sub edx,drect.RY                ;(RY+RH)-drect.RY
        mov drect.RH,edx
        jmp DrawSurface8S_jmp08         ;omin
DrawSurface8S_DRYGH:
        mov edx,[edi+4]                 ;SHeight
        sub edx,drect.RY                ;RY
        mov drect.RH,edx                ;SHeight-drect.RY
DrawSurface8S_jmp08:

        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8S_end
        cmp srect.RH,0
        jle DrawSurface8S_end
        
        ;czy prostokat docelowy jest w widoczny
        cmp drect.RW,0
        jle DrawSurface8S_end
        cmp drect.RH,0
        jle DrawSurface8S_end

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
        je DrawSurface8S_deshfx_e_0
        mov eax,srect.RW
        shl eax,16
        mov ebx,wspx
        xor edx,edx
        div ebx
        mov ebx,drect.RW
        sub ebx,eax
        mov deshfx,ebx
DrawSurface8S_deshfx_e_0:
        cmp deshfy,0
        je DrawSurface8S_deshfy_e_0
        mov eax,srect.RH
        shl eax,16
        mov ebx,wspy
        xor edx,edx
        div ebx
        mov ebx,drect.RH
        sub ebx,eax
        mov deshfy,ebx
DrawSurface8S_deshfy_e_0:

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
        jle DrawSurface8S_end
        cmp repy,0
        jle DrawSurface8S_end

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

DrawSurface8S_drawloopY:
        mov ecx,repx
        mov dword ptr savepozx,ebx
        mov dword ptr savepozx+4,edx
DrawSurface8S_drawloopX:
                mov esi,ebx
                shrd esi,edx,16
                add ebx,wspx
                adc edx,0
                add esi,srcbits
                mov al,[esi]
                mov [edi],al
                inc edi
                loop DrawSurface8S_drawloopX
        mov ecx,wspy
        add pozy,ecx
        mov ecx,pozy
        cmp ecx,65535
        jna DrawSurface8S_no_next_line

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
        jnz DrawSurface8S_drawloopY
        jmp DrawSurface8S_end_draw

        ;jesli ta sama linia
DrawSurface8S_no_next_line:
        ;
        add edi,dsjump
        mov ebx,dword ptr savepozx
        mov edx,dword ptr savepozx+4
        dec repy
        jnz DrawSurface8S_drawloopY

DrawSurface8S_end_draw:
        pop ds                  ;Przywroc selektor danych

DrawSurface8S_end:
        mov eax,descbits
        ret
DrawSurface8S endp

ENDIF
