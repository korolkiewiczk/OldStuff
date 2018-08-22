.386
.model flat,C

.code

include dpmi.inc

;xstub32.asm

extern stub32zerosel:word,stub32zerodesc:byte, \
cpu_name:byte,cpu_ver:dword,cpu_brand:dword,cpu_feat:dword,cpu_efeat:dword

_xstub32init proc C uses ebx edi
        ;pobranie selektora zero danych i kodu
        xor ax,ax
        mov cx,1
        int 31h

        ;ustawienie selektorow
        mov stub32zerosel,ax

        ;sel. zero danych
        mov ax,0ch
        mov bx,stub32zerosel
        mov edi,offset stub32zerodesc
        int 31h

        ;ustawienie limitu selektora danych ds i es
        ;SetSelLim ds,0ffffffffh

        ;pobranie typu procesora i informacji o nim
        pushfd
        pop eax
        or eax,200000h
        push eax
        popfd
        pushfd
        pop eax
        and eax,200000h
        jz no_cpu_id

        .586
        xor eax,eax
        cpuid
        mov dword ptr cpu_name,ebx
        mov dword ptr cpu_name+4,edx
        mov dword ptr cpu_name+8,ecx

        xor eax,eax
        inc eax
        cpuid
        mov cpu_ver,eax
        mov cpu_brand,ebx
        mov cpu_efeat,ecx
        mov cpu_feat,edx
        .386
no_cpu_id:
        ret
_xstub32init endp

__int3 proc C
        int 3
        ret
__int3 endp

__peek proc C uses es,ad:dword
        mov es,stub32zerosel
        mov eax,ad
        movzx eax,byte ptr es:[eax]
        ret
__peek endp


;mcon.inc

extern MCON_CUR_X:byte,MCON_CUR_Y:byte,MCON_TEXT_ATTR:byte

;--------------------------------
;MReadKey ()
;       Czeka na nacisniecie klawisza i pobiera go
;       Wyjscie: al-kod ascii klawisza
;                ah-kod scan klawisza
;--------------------------------

MReadKey proc C
        mov ah,10h
        int 16h
        ret
MReadKey endp

;--------------------------------
;MGetKey ()
;       Pobiera klawisz
;       Wyjscie: jesli zf=0 to klawisz wcisniety:
;                al-kod ascii klawisza
;                ah-kod scan klawisza
;--------------------------------

MGetKey proc C
        mov ah,11h
        int 16h
        ret
MGetKey endp

;--------------------------------
;MGotoXY (x:dword,y:dword)
;       Ustawia wsp. kursora na x i y
;       Wejscie: x-wsp. x
;                y-wsp. y
;--------------------------------
MGotoXY proc C uses eax ebx edx,x:dword,y:dword
        mov ax,0200h        
        mov edx,x
        mov MCON_CUR_X,dl
        mov ebx,y
        mov dh,bl
        mov MCON_CUR_Y,dh
        xor bx,bx
        int 10h
        ret
MGotoXY endp

;--------------------------------
;MPrint (x:dword,y:dword,asciizs:addr)
;       Wypisuje stringa (ASCIIZ) na ekranie o wsp. x i y
;       Wejscie: x-wsp. x
;                y-wsp. y
;                asciizs-napis (ASCIIZ)
;--------------------------------

MPrint proc C uses eax edi esi edx ebx es,x:dword,y:dword,asciizs:dword
        mov ax,stub32zerosel
        mov es,ax
        mov edi,0b8000h
        mov esi,asciizs
        mov edx,x
        mov eax,y
        mov bl,160
        mul bl
        mov ebx,x
        shl ebx,1
        add eax,ebx
        add edi,eax
        mov ah,MCON_TEXT_ATTR
Print_draw_chars:
                lodsb
                or al,al
                jz Print_break_loop
                ;
                cmp al,13
                jne Print_no_code13
                ;jesli al=13
                mov eax,x
                sub eax,edx
                shl eax,1
                sub edi,eax
                mov x,edx
                mov ah,MCON_TEXT_ATTR
                jmp Print_draw_chars
Print_no_code13:
                cmp al,10
                jne Print_no_code10
                ;jesli al=10
                add edi,160
                inc y
                jmp Print_draw_chars
Print_no_code10:
                stosw
                inc x
                cmp x,80
                jne Print_draw_chars
                mov x,0
                inc y
                jmp Print_draw_chars
Print_break_loop:
        invoke MGotoXY,x,y
        ret
MPrint endp

;--------------------------------
;MCls ()
;       Czysci ekran tekstowy
;--------------------------------

MCls proc C uses eax edi ecx ebx es
        cld
        mov ax,stub32zerosel
        mov es,ax
        mov edi,0b8000h
        mov al,' '
        mov ah,MCON_TEXT_ATTR
        mov bx,ax
        shl eax,16
        mov ax,bx
        mov ecx,1000
        rep stosd
        invoke MGotoXY,0,0
        ret
MCls endp

;prc.inc

extern prc_parlist:dword


;wywoluje procedure
;prc_addr-adres procedury
;nparams-ilosc parametrow
;outtype-typ zwracanej danej

Prc_ExecProc proc C ,prc_addr:dword, nparams:dword, outtype:dword
        mov ecx,nparams
        jecxz Prc_ExecProc_call
        mov eax,ecx
        shl eax,2        
        add eax,offset prc_parlist        
Prc_ExecProc_loop:
                sub eax,4
                push [eax]                
                loop Prc_ExecProc_loop
Prc_ExecProc_call:
        mov eax,prc_addr
        call eax
        cmp outtype,1          ;czy typ float
        jne Prc_ExecProc_nofloat
        ;float
        fstp outtype
        mov eax,outtype
Prc_ExecProc_nofloat:
        mov ecx,nparams
        shl ecx,2         
        add esp,ecx
        ret
Prc_ExecProc endp

;vga.inc

;--------------------------------
;SetVGAMode
;       Ustawia dany tryb graficzny VGA
;--------------------------------

SetVGAMode proc C,vgamode:dword
        mov eax,vgamode
        int 10h
        ret
SetVGAMode endp

;funkcje

;--------------------------------
;SetColor (int,int,int,int)
;       Ustawia kolor w palecie kolorow karty graficznej
;--------------------------------

SetColor proc C ,col:dword,r:dword,g:dword,b:dword
        mov eax,col
        mov dx,3c8h
        out dx,al
        inc dx
        mov eax,r
        out dx,al
        mov eax,g
        out dx,al
        mov eax,b
        out dx,al
        ret
SetColor endp

;--------------------------------
;GetColor (int,int*,int*,int*)
;       Pobiera kolor z palety kolorow karty graficznej
;--------------------------------

GetColor proc C ,col:dword,r:dword,g:dword,b:dword
        mov eax,col
        mov dx,3c7h
        out dx,al
        mov dx,3c9h
        xor eax,eax
        in al,dx
        mov ecx,r
        mov [ecx],eax
        in al,dx        ;al=g
        mov ecx,g
        mov [ecx],eax
        in al,dx        ;al=b
        mov ecx,b
        mov [ecx],eax
        ret
GetColor endp


;--------------------------------
;SetPalette (char*)
;       Ustawia palete kolorow karty graficznej zamieszczona w buforze
;       Wejscie: bf-bufor
;--------------------------------

SetPalette proc C uses esi ,bf:dword
        mov dx,3c8h
        xor al,al
        out dx,al
        inc dx
        mov esi,bf
        mov ecx,768
        rep outsb
        ret
SetPalette endp

;--------------------------------
;Wait4VSync()
;       Czeka na powrot pionowy monitora
;--------------------------------

Wait4VSync proc C
        mov dx,3dah
Wait4VSync_l1:
        in al,dx
        test al,8h
        jnz Wait4VSync_l1

Wait4VSync_l2:
        in al,dx
        test al,8h
        jz Wait4VSync_l2
        ret
Wait4VSync endp

;vesa.inc

;--------------------------------
;SetVESAMode (int,int)
;       Ustawia tryb VESA z atrybutami
;       Uzywane rejestry: ax,bx
;       Wejscie: mode-tryb graficzny VESA
;                attr-atrybuty (14-bit=1-uzywaj LFB)
;--------------------------------

SetVESAMode proc C uses ebx,vesamode:dword,vattr:dword
        mov eax,4f02h
        mov ebx,vesamode
        or ebx,vattr
        int 10h
        ret
SetVESAMode endp

;surface8.inc

TSurf struct                   ;Podstawowa struktora przechowujaca obraz
        SWidth dd ?             ;Szerokosc powierzchni
        SHeight dd ?            ;Wysokosc powierzchni
        SBits dd ?              ;Wskaznik do bajtow powierzchni
TSurf ends

TRect struct                   ;Struktora przechowuje prostokat
        RX dd ?                 ;Pozycja X prostokata
        RY dd ?                 ;Pozycja Y prostokata
        RW dd ?                 ;Szerokosc prostokata
        RH dd ?                 ;Wysokosc prostokata
TRect ends

;--------------------------------
;DrawSurface8 (TRect*,TRect*,TSurf*,TSurf*)
;       Rysuje powierzchnie 8-bitowa na innej powierzchni 8-bitowej (np. ekranu) bez skalowania
;       Rejestry:eax-adres do prostokata (TRect) zrodlowego
;                ebx-adres do prostokata (TRect) docelowego
;                esi-adres do zrodlowej struktory TSurf
;                edi-adres do docelowej struktory TSurf
;       Wyjscie: eax-bajty powierzchni docelowej lub NULL jesli nie bylo kopiowania
;--------------------------------

DrawSurface8 proc C uses edi esi ebx,_srect:dword,_drect:dword,_ssurf:dword,_dsurf:dword
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
        mov descbits,0
        cld
        mov eax,_srect
        mov ebx,_drect
        mov esi,_ssurf
        mov edi,_dsurf

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

        mov eax,[esi+8]                ;SBits
        mov srcbits,eax

        mov eax,[edi+8]                ;SBits
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

DrawSurface8_end:
        mov eax,descbits
        ret
DrawSurface8 endp

;--------------------------------
;DrawSurface8S (TRect*,TRect*,TSurf*,TSurf*)
;       Rysuje powierzchnie 8-bitowa na innej powierzchni 8-bitowej (np. ekranu) ze skalowaniem
;       Rejestry:eax-adres do prostokata (TRect) zrodlowego
;                ebx-adres do prostokata (TRect) docelowego
;                esi-adres do zrodlowej struktory TSurf
;                edi-adres do docelowej struktory TSurf
;       Wyjscie: eax-bajty powierzchni docelowej lub NULL jesli nie bylo kopiowania
;       Uwaga: Rozmiary obu powierzchni musza sie miescic w przedziale 0-65535
;--------------------------------

DrawSurface8S proc C uses edi esi ebx,_srect:dword,_drect:dword,_ssurf:dword,_dsurf:dword
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
        mov eax,_srect
        mov ebx,_drect
        mov esi,_ssurf
        mov edi,_dsurf
        mov edx,[esi]                   ;SWidth
        mov srcwidth,edx
        mov descbits,0

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
        
        mov eax,[esi+8]                ;SBits
        mov srcbits,eax

        mov eax,[edi+8]                ;SBits
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
        jmp DrawSurface8S_end

        ;jesli ta sama linia
DrawSurface8S_no_next_line:
        ;
        add edi,dsjump
        mov ebx,dword ptr savepozx
        mov edx,dword ptr savepozx+4
        dec repy
        jnz DrawSurface8S_drawloopY

DrawSurface8S_end:
        mov eax,descbits
        ret
DrawSurface8S endp

;--------------------------------
;DrawSurface8C (TRect*,TRect*,TSurf*,TSurf*)
;       Rysuje powierzchnie przezroczysta 8-bitowa na innej powierzchni 8-bitowej (np. ekranu) bez skalowania
;       Rejestry:eax-adres do prostokata (TRect) zrodlowego
;                ebx-adres do prostokata (TRect) docelowego
;                esi-adres do zrodlowej struktory TSurf
;                edi-adres do docelowej struktory TSurf
;       Wyjscie: eax-bajty powierzchni docelowej lub NULL jesli nie bylo kopiowania
;--------------------------------

DrawSurface8C proc C uses edi esi ebx,_srect:dword,_drect:dword,_ssurf:dword,_dsurf:dword
        local srect:TRect,drect:TRect   ;prostokaty pomocnicze
        local ssjump,dsjump:dword       ;skoki dla poweirzchni zrodlowej i docelowej
        local srcbits,descbits:dword    ;wskazniki do powierzchni
        local repx,repy:dword           ;ilosc powt. x i y
        local sshfx,sshfy:dword         ;przesuniecie powierzchni zrodlowej
        local dshfx,dshfy:dword         ;przesuniecie powierzchni docelowej
        mov sshfx,0
        mov sshfy,0
        mov dshfx,0
        mov dshfy,0
        mov descbits,0
        cld
        mov eax,_srect
        mov ebx,_drect
        mov esi,_ssurf
        mov edi,_dsurf

        ;Sprawdzenie czy prostokat zrodlowy nie wychodzi poza obszar powierzchni
        ;if (eax.RX<0)
        cmp dword ptr [eax],0           ;S.RX
        jl DrawSurface8C_SRXL0           ;S.RX<0
        mov edx,[eax]                   ;RX
        mov srect.RX,edx                ;RX
        jmp DrawSurface8C_jmp01          ;omin
DrawSurface8C_SRXL0:
        mov srect.RX,0                  ;0
        mov edx,[eax]                   ;S.RX
        neg edx                         ;-S.RX
        mov dshfx,edx

        ;if (eax.RY<0)
DrawSurface8C_jmp01:
        cmp dword ptr [eax+4],0         ;S.RY
        jl DrawSurface8C_SRYL0           ;S.RY<0
        mov edx,[eax+4]                 ;RY  
        mov srect.RY,edx                ;RY
        jmp DrawSurface8C_jmp02          ;omin
DrawSurface8C_SRYL0:
        mov srect.RY,0                  ;0
        mov edx,[eax+4]                 ;S.RY
        neg edx                         ;-S.RY
        mov dshfy,edx

        ;if (eax.RX+eax.RW>esi.SWidth)
DrawSurface8C_jmp02:
        mov edx,[eax+8]                 ;RW
        add edx,[eax]                   ;RX+RW
        cmp edx,[esi]                   ;SWidth
        jg DrawSurface8C_SRXGW           ;S.RX+S.RW>S.SWitdth
        sub edx,srect.RX                ;(RX+RW)-srect.RX
        mov srect.RW,edx
        jmp DrawSurface8C_jmp03          ;omin
DrawSurface8C_SRXGW:
        mov edx,[esi]                   ;SWidth
        sub edx,srect.RX                ;RX
        mov srect.RW,edx                ;SWidth-srect.RX
DrawSurface8C_jmp03:

        ;if (eax.RY+eax.RH>esi.SHeight)
        mov edx,[eax+12]                ;RH
        add edx,[eax+4]                 ;RY+RH
        cmp edx,[esi+4]                 ;SHeight
        jg DrawSurface8C_SRYGH           ;S.RX+S.RW>S.SHeight
        sub edx,srect.RY                ;(RY+RH)-srect.RY
        mov srect.RH,edx
        jmp DrawSurface8C_jmp04          ;omin
DrawSurface8C_SRYGH:
        mov edx,[esi+4]                 ;SHeight
        sub edx,srect.RY                ;RY
        mov srect.RH,edx                ;SHeight-srect.RY
DrawSurface8C_jmp04:
        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8C_end
        cmp srect.RH,0
        jle DrawSurface8C_end

        ;Sprawdzenie czy prostokat docelowy nie wychodzi poza obszar powierzchni
        ;if (ebx.RX<0)
        cmp dword ptr [ebx],0           ;D.RX
        jl DrawSurface8C_DRXL0           ;D.RX<0
        mov edx,[ebx]                   ;RX
        mov drect.RX,edx                ;RX
        jmp DrawSurface8C_jmp05          ;omin
DrawSurface8C_DRXL0:
        mov drect.RX,0                  ;0
        mov edx,[ebx]                   ;D.RX
        neg edx                         ;-D.RX
        mov sshfx,edx

        ;if (ebx.RY<0)
DrawSurface8C_jmp05:
        cmp dword ptr [ebx+4],0         ;D.RY
        jl DrawSurface8C_DRYL0           ;D.RY<0
        mov edx,[ebx+4]                 ;RY  
        mov drect.RY,edx                ;RY
        jmp DrawSurface8C_jmp06          ;omin
DrawSurface8C_DRYL0:
        mov drect.RY,0                  ;0
        mov edx,[ebx+4]                 ;D.RY
        neg edx                         ;-D.RY
        mov sshfy,edx

        ;if (ebx.RX+ebx.RW>edi.SWidth)
DrawSurface8C_jmp06:
        mov edx,[ebx+8]                 ;RW
        add edx,[ebx]                   ;RX+RW
        cmp edx,[edi]                   ;SWidth
        jg DrawSurface8C_DRXGW           ;D.RX+D.RW>D.SWitdth
        sub edx,drect.RX                ;(RX+RW)-drect.RX
        mov drect.RW,edx
        jmp DrawSurface8C_jmp07          ;omin
DrawSurface8C_DRXGW:
        mov edx,[edi]                   ;SWidth
        sub edx,drect.RX                ;RX
        mov drect.RW,edx                ;SWidth-drect.RX
DrawSurface8C_jmp07:

        ;if (ebx.RY+ebx.RH>edi.SHeight)
        mov edx,[ebx+12]                ;RH
        add edx,[ebx+4]                 ;RY+RH
        cmp edx,[edi+4]                 ;SHeight
        jg DrawSurface8C_DRYGH           ;D.RX+D.RW>D.SHeight
        sub edx,drect.RY                ;(RY+RH)-drect.RY
        mov drect.RH,edx
        jmp DrawSurface8C_jmp08          ;omin
DrawSurface8C_DRYGH:
        mov edx,[edi+4]                 ;SHeight
        sub edx,drect.RY                ;RY
        mov drect.RH,edx                ;SHeight-drect.RY
DrawSurface8C_jmp08:

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
        jle DrawSurface8C_end
        cmp srect.RH,0
        jle DrawSurface8C_end
        
        ;czy prostokat docelowy jest w widoczny
        cmp drect.RW,0
        jle DrawSurface8C_end
        cmp drect.RH,0
        jle DrawSurface8C_end

        ;zapamietanie wskaznikow do bajtow powierzchni

        mov eax,[esi+8]                ;SBits
        mov srcbits,eax

        mov eax,[edi+8]                ;SBits
        mov descbits,eax
         
        ;ilosc powtorzen zalezy od tego ktora powierzchnia jest mniejsza dla danej osi

        ;os y
        mov eax,drect.RH
        cmp eax,srect.RH
        jg DrawSurface8C_repys          ;srect.RH>drect.RH
        mov repy,eax
        jmp DrawSurface8C_jmp09
DrawSurface8C_repys:
        mov eax,srect.RH
        mov repy,eax
DrawSurface8C_jmp09:

        ;os x
        mov eax,drect.RW
        cmp eax,srect.RW
        jg DrawSurface8C_repxs          ;srect.RW>drect.RW
        mov repx,eax
        jmp DrawSurface8C_jmp10
DrawSurface8C_repxs:
        mov eax,srect.RW
        mov repx,eax
DrawSurface8C_jmp10:

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

        ;przenies skoki do rejestrow dla uzyskania lepszej szybkosci
        mov edx,dsjump

        mov ebx,repy

DrawSurface8C_drawloopY:
                mov ecx,repx
DrawSurface8C_drawloopX:
                        mov al,[esi]
                        or al,al
                        jz DrawSurface8C_no_copy_byte
                        mov [edi],al
DrawSurface8C_no_copy_byte:
                        inc esi
                        inc edi
                        loop DrawSurface8C_drawloopX
                add esi,ssjump
                add edi,edx     ;dsjump
        dec ebx
        jnz DrawSurface8C_drawloopY

DrawSurface8C_end:
        mov eax,descbits
        ret
DrawSurface8C endp

;--------------------------------
;DrawSurface8CS (TRect*,TRect*,TSurf*,TSurf*)
;       Rysuje powierzchnie 8-bitowa na innej powierzchni 8-bitowej (np. ekranu) ze skalowaniem
;       Rejestry:eax-adres do prostokata (TRect) zrodlowego
;                ebx-adres do prostokata (TRect) docelowego
;                esi-adres do zrodlowej struktory TSurf
;                edi-adres do docelowej struktory TSurf
;       Wyjscie: eax-bajty powierzchni docelowej lub NULL jesli nie bylo kopiowania
;       Uwaga: Rozmiary obu powierzchni musza sie miescic w przedziale 0-65535
;--------------------------------

DrawSurface8CS proc C uses edi esi ebx,_srect:dword,_drect:dword,_ssurf:dword,_dsurf:dword
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
        mov eax,_srect
        mov ebx,_drect
        mov esi,_ssurf
        mov edi,_dsurf
        mov edx,[esi]                   ;SWidth
        mov srcwidth,edx
        mov descbits,0

        ;Sprawdzenie czy prostokat zrodlowy nie wychodzi poza obszar powierzchni
        ;if (eax.RX<0)
        cmp dword ptr [eax],0           ;S.RX
        jl DrawSurface8CS_SRXL0          ;S.RX<0
        mov edx,[eax]                   ;RX
        mov srect.RX,edx                ;RX
        jmp DrawSurface8CS_jmp01         ;omin
DrawSurface8CS_SRXL0:
        mov srect.RX,0                  ;0
        mov edx,[eax]                   ;S.RX
        neg edx                         ;-S.RX
        mov dshfx,edx

        ;if (eax.RY<0)
DrawSurface8CS_jmp01:
        cmp dword ptr [eax+4],0         ;S.RY
        jl DrawSurface8CS_SRYL0          ;S.RY<0
        mov edx,[eax+4]                 ;RY  
        mov srect.RY,edx                ;RY
        jmp DrawSurface8CS_jmp02         ;omin
DrawSurface8CS_SRYL0:
        mov srect.RY,0                  ;0
        mov edx,[eax+4]                 ;S.RY
        neg edx                         ;-S.RX
        mov dshfy,edx

        ;if (eax.RX+eax.RW>esi.SWidth)
DrawSurface8CS_jmp02:
        mov edx,[eax+8]                 ;RW
        add edx,[eax]                   ;RX+RW
        cmp edx,[esi]                   ;SWidth
        jg DrawSurface8CS_SRXGW          ;S.RX+S.RW>S.SWitdth
        sub edx,srect.RX                ;(RX+RW)-srect.RX
        mov srect.RW,edx
        jmp DrawSurface8CS_jmp03         ;omin
DrawSurface8CS_SRXGW:
        mov edx,[esi]                   ;SWidth
        sub edx,srect.RX                ;RX
        mov deshfx,1
        mov srect.RW,edx                ;SWidth-srect.RX
DrawSurface8CS_jmp03:

        ;if (eax.RY+eax.RH>esi.SHeight)
        mov edx,[eax+12]                ;RH
        add edx,[eax+4]                 ;RY+RH
        cmp edx,[esi+4]                 ;SHeight
        jg DrawSurface8CS_SRYGH          ;S.RX+S.RW>S.SHeight
        sub edx,srect.RY                ;(RY+RH)-srect.RY
        mov srect.RH,edx
        jmp DrawSurface8CS_jmp04         ;omin
DrawSurface8CS_SRYGH:
        mov edx,[esi+4]                 ;SHeight
        sub edx,srect.RY                ;RY
        mov deshfy,1
        mov srect.RH,edx                ;SHeight-srect.RY
DrawSurface8CS_jmp04:
        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8CS_end
        cmp srect.RH,0
        jle DrawSurface8CS_end

        ;Sprawdzenie czy prostokat docelowy nie wychodzi poza obszar powierzchni
        ;if (ebx.RX<0)
        cmp dword ptr [ebx],0           ;D.RX
        jl DrawSurface8CS_DRXL0          ;D.RX<0
        mov edx,[ebx]                   ;RX
        mov drect.RX,edx                ;RX
        jmp DrawSurface8CS_jmp05         ;omin
DrawSurface8CS_DRXL0:
        mov drect.RX,0                  ;0
        mov edx,[ebx]                   ;D.RX
        shl edx,16
        neg edx                         ;-D.RX
        mov sshfx,edx

        ;if (ebx.RY<0)
DrawSurface8CS_jmp05:
        cmp dword ptr [ebx+4],0         ;D.RY
        jl DrawSurface8CS_DRYL0          ;D.RY<0
        mov edx,[ebx+4]                 ;RY  
        mov drect.RY,edx                ;RY
        jmp DrawSurface8CS_jmp06         ;omin
DrawSurface8CS_DRYL0:
        mov drect.RY,0                  ;0
        mov edx,[ebx+4]                 ;D.RY
        shl edx,16
        neg edx                         ;-D.RY
        mov sshfy,edx

        ;if (ebx.RX+ebx.RW>edi.SWidth)
DrawSurface8CS_jmp06:
        mov edx,[ebx+8]                 ;RW
        add edx,[ebx]                   ;RX+RW
        cmp edx,[edi]                   ;SWidth
        jg DrawSurface8CS_DRXGW          ;D.RX+D.RW>D.SWitdth
        sub edx,drect.RX                ;(RX+RW)-drect.RX
        mov drect.RW,edx
        jmp DrawSurface8CS_jmp07         ;omin
DrawSurface8CS_DRXGW:
        mov edx,[edi]                   ;SWidth
        sub edx,drect.RX                ;RX
        mov drect.RW,edx                ;SWidth-drect.RX
DrawSurface8CS_jmp07:

        ;if (ebx.RY+ebx.RH>edi.SHeight)
        mov edx,[ebx+12]                ;RH
        add edx,[ebx+4]                 ;RY+RH
        cmp edx,[edi+4]                 ;SHeight
        jg DrawSurface8CS_DRYGH          ;D.RX+D.RW>D.SHeight
        sub edx,drect.RY                ;(RY+RH)-drect.RY
        mov drect.RH,edx
        jmp DrawSurface8CS_jmp08         ;omin
DrawSurface8CS_DRYGH:
        mov edx,[edi+4]                 ;SHeight
        sub edx,drect.RY                ;RY
        mov drect.RH,edx                ;SHeight-drect.RY
DrawSurface8CS_jmp08:

        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8CS_end
        cmp srect.RH,0
        jle DrawSurface8CS_end
        
        ;czy prostokat docelowy jest w widoczny
        cmp drect.RW,0
        jle DrawSurface8CS_end
        cmp drect.RH,0
        jle DrawSurface8CS_end

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
        je DrawSurface8CS_deshfx_e_0
        mov eax,srect.RW
        shl eax,16
        mov ebx,wspx
        xor edx,edx
        div ebx
        mov ebx,drect.RW
        sub ebx,eax
        mov deshfx,ebx
DrawSurface8CS_deshfx_e_0:
        cmp deshfy,0
        je DrawSurface8CS_deshfy_e_0
        mov eax,srect.RH
        shl eax,16
        mov ebx,wspy
        xor edx,edx
        div ebx
        mov ebx,drect.RH
        sub ebx,eax
        mov deshfy,ebx
DrawSurface8CS_deshfy_e_0:

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
        jle DrawSurface8CS_end
        cmp repy,0
        jle DrawSurface8CS_end

        ;obliczenie skoku docelowego
        mov eax,[edi]                   ;SWidth
        sub eax,repx                    ;SWidth-repx
        mov dsjump,eax

        ;zapamietanie wskaznikow do bajtow powierzchni
        
        mov eax,[esi+8]                ;SBits
        mov srcbits,eax

        mov eax,[edi+8]                ;SBits
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

        ;wyliczenie pozy
        mov eax,sshfy
        xor edx,edx
        mov ecx,65536
        div ecx
        mov pozy,edx

        mov edx,esi

DrawSurface8CS_drawloopY:
        mov ecx,repx
        mov dword ptr savepozx,ebx
        mov dword ptr savepozx+4,edx
DrawSurface8CS_drawloopX:
                mov esi,ebx
                shrd esi,edx,16
                add ebx,wspx
                adc edx,0
                add esi,srcbits
                mov al,[esi]
                or al,al
                jz DrawSurface8CS_no_copy_byte
                mov [edi],al
DrawSurface8CS_no_copy_byte:
                inc edi
                loop DrawSurface8CS_drawloopX
        mov ecx,wspy
        add pozy,ecx
        mov ecx,pozy
        cmp ecx,65535
        jna DrawSurface8CS_no_next_line

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
        jnz DrawSurface8CS_drawloopY
        jmp DrawSurface8CS_end

        ;jesli ta sama linia
DrawSurface8CS_no_next_line:
        ;
        add edi,dsjump
        mov ebx,dword ptr savepozx
        mov edx,dword ptr savepozx+4
        dec repy
        jnz DrawSurface8CS_drawloopY

DrawSurface8CS_end:
        mov eax,descbits
        ret
DrawSurface8CS endp

;--------------------------------
;DrawSurface8F (TRect*,TRect*,TSurf*,TSurf*,int)
;       Rysuje powierzchnie 8-bitowa na innej powierzchni 8-bitowej (np. ekranu) bez skalowania z cieniowaniem
;       Wejscie: eax-adres do prostokata (TRect) zrodlowego
;                ebx-adres do prostokata (TRect) docelowego
;                ecx-stopien cieniowania (w zaleznosci od FADE8TabSize w fade8.inc)
;                esi-adres do zrodlowej struktory TSurface
;                edi-adres do docelowej struktory TSurface
;       Wyjscie: eax-bajty powierzchni docelowej lub NULL jesli nie bylo kopiowania
;--------------------------------

DrawSurface8F proc C uses edi esi ebx,_srect:dword,_drect:dword,_ssurf:dword,_dsurf:dword,_fade:dword
        local srect:TRect,drect:TRect   ;prostokaty pomocnicze
        local ssjump,dsjump:dword       ;skoki dla poweirzchni zrodlowej i docelowej
        local srcbits,descbits:dword    ;wskazniki do powierzchni
        local repx,repx2,repy:dword     ;ilosc powt. x i y
        local sshfx,sshfy:dword         ;przesuniecie powierzchni zrodlowej
        local dshfx,dshfy:dword         ;przesuniecie powierzchni docelowej
        mov sshfx,0
        mov sshfy,0
        mov dshfx,0
        mov dshfy,0
        mov descbits,0
        cld
        mov eax,_srect
        mov ebx,_drect
        mov ecx,_fade
        mov esi,_ssurf
        mov edi,_dsurf

        ;Sprawdzenie czy prostokat zrodlowy nie wychodzi poza obszar powierzchni
        ;if (eax.RX<0)
        cmp dword ptr [eax],0           ;S.RX
        jl DrawSurface8F_SRXL0           ;S.RX<0
        mov edx,[eax]                   ;RX
        mov srect.RX,edx                ;RX
        jmp DrawSurface8F_jmp01          ;omin
DrawSurface8F_SRXL0:
        mov srect.RX,0                  ;0
        mov edx,[eax]                   ;S.RX
        neg edx                         ;-S.RX
        mov dshfx,edx

        ;if (eax.RY<0)
DrawSurface8F_jmp01:
        cmp dword ptr [eax+4],0         ;S.RY
        jl DrawSurface8F_SRYL0           ;S.RY<0
        mov edx,[eax+4]                 ;RY  
        mov srect.RY,edx                ;RY
        jmp DrawSurface8F_jmp02          ;omin
DrawSurface8F_SRYL0:
        mov srect.RY,0                  ;0
        mov edx,[eax+4]                 ;S.RY
        neg edx                         ;-S.RY
        mov dshfy,edx

        ;if (eax.RX+eax.RW>esi.SWidth)
DrawSurface8F_jmp02:
        mov edx,[eax+8]                 ;RW
        add edx,[eax]                   ;RX+RW
        cmp edx,[esi]                   ;SWidth
        jg DrawSurface8F_SRXGW           ;S.RX+S.RW>S.SWitdth
        sub edx,srect.RX                ;(RX+RW)-srect.RX
        mov srect.RW,edx
        jmp DrawSurface8F_jmp03          ;omin
DrawSurface8F_SRXGW:
        mov edx,[esi]                   ;SWidth
        sub edx,srect.RX                ;RX
        mov srect.RW,edx                ;SWidth-srect.RX
DrawSurface8F_jmp03:

        ;if (eax.RY+eax.RH>esi.SHeight)
        mov edx,[eax+12]                ;RH
        add edx,[eax+4]                 ;RY+RH
        cmp edx,[esi+4]                 ;SHeight
        jg DrawSurface8F_SRYGH           ;S.RX+S.RW>S.SHeight
        sub edx,srect.RY                ;(RY+RH)-srect.RY
        mov srect.RH,edx
        jmp DrawSurface8F_jmp04          ;omin
DrawSurface8F_SRYGH:
        mov edx,[esi+4]                 ;SHeight
        sub edx,srect.RY                ;RY
        mov srect.RH,edx                ;SHeight-srect.RY
DrawSurface8F_jmp04:
        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8F_end
        cmp srect.RH,0
        jle DrawSurface8F_end

        ;Sprawdzenie czy prostokat docelowy nie wychodzi poza obszar powierzchni
        ;if (ebx.RX<0)
        cmp dword ptr [ebx],0           ;D.RX
        jl DrawSurface8F_DRXL0           ;D.RX<0
        mov edx,[ebx]                   ;RX
        mov drect.RX,edx                ;RX
        jmp DrawSurface8F_jmp05          ;omin
DrawSurface8F_DRXL0:
        mov drect.RX,0                  ;0
        mov edx,[ebx]                   ;D.RX
        neg edx                         ;-D.RX
        mov sshfx,edx

        ;if (ebx.RY<0)
DrawSurface8F_jmp05:
        cmp dword ptr [ebx+4],0         ;D.RY
        jl DrawSurface8F_DRYL0           ;D.RY<0
        mov edx,[ebx+4]                 ;RY  
        mov drect.RY,edx                ;RY
        jmp DrawSurface8F_jmp06          ;omin
DrawSurface8F_DRYL0:
        mov drect.RY,0                  ;0
        mov edx,[ebx+4]                 ;D.RY
        neg edx                         ;-D.RY
        mov sshfy,edx

        ;if (ebx.RX+ebx.RW>edi.SWidth)
DrawSurface8F_jmp06:
        mov edx,[ebx+8]                 ;RW
        add edx,[ebx]                   ;RX+RW
        cmp edx,[edi]                   ;SWidth
        jg DrawSurface8F_DRXGW           ;D.RX+D.RW>D.SWitdth
        sub edx,drect.RX                ;(RX+RW)-drect.RX
        mov drect.RW,edx
        jmp DrawSurface8F_jmp07          ;omin
DrawSurface8F_DRXGW:
        mov edx,[edi]                   ;SWidth
        sub edx,drect.RX                ;RX
        mov drect.RW,edx                ;SWidth-drect.RX
DrawSurface8F_jmp07:

        ;if (ebx.RY+ebx.RH>edi.SHeight)
        mov edx,[ebx+12]                ;RH
        add edx,[ebx+4]                 ;RY+RH
        cmp edx,[edi+4]                 ;SHeight
        jg DrawSurface8F_DRYGH           ;D.RX+D.RW>D.SHeight
        sub edx,drect.RY                ;(RY+RH)-drect.RY
        mov drect.RH,edx
        jmp DrawSurface8F_jmp08          ;omin
DrawSurface8F_DRYGH:
        mov edx,[edi+4]                 ;SHeight
        sub edx,drect.RY                ;RY
        mov drect.RH,edx                ;SHeight-drect.RY
DrawSurface8F_jmp08:

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
        jle DrawSurface8F_end
        cmp srect.RH,0
        jle DrawSurface8F_end
        
        ;czy prostokat docelowy jest w widoczny
        cmp drect.RW,0
        jle DrawSurface8F_end
        cmp drect.RH,0
        jle DrawSurface8F_end

        ;zapamietanie wskaznikow do bajtow powierzchni

        mov eax,[esi+8]                ;SBits
        mov srcbits,eax

        mov eax,[edi+8]                ;SBits
        mov descbits,eax
         
        ;ilosc powtorzen zalezy od tego ktora powierzchnia jest mniejsza dla danej osi

        ;os y
        mov eax,drect.RH
        cmp eax,srect.RH
        jg DrawSurface8F_repys          ;srect.RH>drect.RH
        mov repy,eax
        jmp DrawSurface8F_jmp09
DrawSurface8F_repys:
        mov eax,srect.RH
        mov repy,eax
DrawSurface8F_jmp09:

        ;os x
        mov eax,drect.RW
        cmp eax,srect.RW
        jg DrawSurface8F_repxs          ;srect.RW>drect.RW
        mov repx,eax
        jmp DrawSurface8F_jmp10
DrawSurface8F_repxs:
        mov eax,srect.RW
        mov repx,eax
DrawSurface8F_jmp10:

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
        mov ecx,FADE8TabShift

        ;przenies skoki do rejestrow dla uzyskania lepszej szybkosci
        mov edx,dsjump

DrawSurface8F_drawloopY:
                mov eax,repx
                mov repx2,eax
DrawSurface8F_drawloopX:
                        xor eax,eax
                        mov al,[esi]
                        shl eax,cl
                        mov al,[ebx+eax]
                        mov [edi],al
                        inc esi
                        inc edi
                        dec repx2
                        jnz DrawSurface8F_drawloopX
                add esi,ssjump
                add edi,edx     ;dsjump
                dec repy
                jnz DrawSurface8F_drawloopY

DrawSurface8F_end:
        mov eax,descbits
        ret
DrawSurface8F endp

;--------------------------------
;DrawSurface8FS (TRect*,TRect*,TSurf*,TSurf*,int)
;       Rysuje powierzchnie 8-bitowa na innej powierzchni 8-bitowej (np. ekranu) ze skalowaniem z cieniowaniem
;       Wejscie: eax-adres do prostokata (TRect) zrodlowego
;                ebx-adres do prostokata (TRect) docelowego
;                ecx-stopien cieniowania (w zaleznosci od FADE8TabSize w fade8.inc)
;                esi-adres do zrodlowej struktory TSurface
;                edi-adres do docelowej struktory TSurface
;       Wyjscie: eax-bajty powierzchni docelowej lub NULL jesli nie bylo kopiowania
;       Uwaga: Rozmiary obu powierzchni musza sie miescic w przedziale 0-65535
;--------------------------------

DrawSurface8FS proc C uses edi esi ebx,_srect:dword,_drect:dword,_ssurf:dword,_dsurf:dword,_fade:dword
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
        local fadetab:dword,tshf:dword
        mov sshfx,0
        mov sshfy,0
        mov dshfx,0
        mov dshfy,0
        mov deshfx,0
        mov deshfy,0
        mov esi,_ssurf
        mov eax,[esi]                   ;SWidth
        mov srcwidth,eax
        mov eax,FADE8Tab
        mov fadetab,eax
        mov ecx,_fade
        add fadetab,ecx
        mov eax,FADE8TabShift
        mov tshf,eax
        mov descbits,0
        mov eax,_srect
        mov ebx,_drect                
        mov edi,_dsurf

        ;Sprawdzenie czy prostokat zrodlowy nie wychodzi poza obszar powierzchni
        ;if (eax.RX<0)
        cmp dword ptr [eax],0           ;S.RX
        jl DrawSurface8FS_SRXL0          ;S.RX<0
        mov edx,[eax]                   ;RX
        mov srect.RX,edx                ;RX
        jmp DrawSurface8FS_jmp01         ;omin
DrawSurface8FS_SRXL0:
        mov srect.RX,0                  ;0
        mov edx,[eax]                   ;S.RX
        neg edx                         ;-S.RX
        mov dshfx,edx

        ;if (eax.RY<0)
DrawSurface8FS_jmp01:
        cmp dword ptr [eax+4],0         ;S.RY
        jl DrawSurface8FS_SRYL0          ;S.RY<0
        mov edx,[eax+4]                 ;RY  
        mov srect.RY,edx                ;RY
        jmp DrawSurface8FS_jmp02         ;omin
DrawSurface8FS_SRYL0:
        mov srect.RY,0                  ;0
        mov edx,[eax+4]                 ;S.RY
        neg edx                         ;-S.RX
        mov dshfy,edx

        ;if (eax.RX+eax.RW>esi.SWidth)
DrawSurface8FS_jmp02:
        mov edx,[eax+8]                 ;RW
        add edx,[eax]                   ;RX+RW
        cmp edx,[esi]                   ;SWidth
        jg DrawSurface8FS_SRXGW          ;S.RX+S.RW>S.SWitdth
        sub edx,srect.RX                ;(RX+RW)-srect.RX
        mov srect.RW,edx
        jmp DrawSurface8FS_jmp03         ;omin
DrawSurface8FS_SRXGW:
        mov edx,[esi]                   ;SWidth
        sub edx,srect.RX                ;RX
        mov deshfx,1
        mov srect.RW,edx                ;SWidth-srect.RX
DrawSurface8FS_jmp03:

        ;if (eax.RY+eax.RH>esi.SHeight)
        mov edx,[eax+12]                ;RH
        add edx,[eax+4]                 ;RY+RH
        cmp edx,[esi+4]                 ;SHeight
        jg DrawSurface8FS_SRYGH          ;S.RX+S.RW>S.SHeight
        sub edx,srect.RY                ;(RY+RH)-srect.RY
        mov srect.RH,edx
        jmp DrawSurface8FS_jmp04         ;omin
DrawSurface8FS_SRYGH:
        mov edx,[esi+4]                 ;SHeight
        sub edx,srect.RY                ;RY
        mov deshfy,1
        mov srect.RH,edx                ;SHeight-srect.RY
DrawSurface8FS_jmp04:
        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8FS_end
        cmp srect.RH,0
        jle DrawSurface8FS_end

        ;Sprawdzenie czy prostokat docelowy nie wychodzi poza obszar powierzchni
        ;if (ebx.RX<0)
        cmp dword ptr [ebx],0           ;D.RX
        jl DrawSurface8FS_DRXL0          ;D.RX<0
        mov edx,[ebx]                   ;RX
        mov drect.RX,edx                ;RX
        jmp DrawSurface8FS_jmp05         ;omin
DrawSurface8FS_DRXL0:
        mov drect.RX,0                  ;0
        mov edx,[ebx]                   ;D.RX
        shl edx,16
        neg edx                         ;-D.RX
        mov sshfx,edx

        ;if (ebx.RY<0)
DrawSurface8FS_jmp05:
        cmp dword ptr [ebx+4],0         ;D.RY
        jl DrawSurface8FS_DRYL0          ;D.RY<0
        mov edx,[ebx+4]                 ;RY  
        mov drect.RY,edx                ;RY
        jmp DrawSurface8FS_jmp06         ;omin
DrawSurface8FS_DRYL0:
        mov drect.RY,0                  ;0
        mov edx,[ebx+4]                 ;D.RY
        shl edx,16
        neg edx                         ;-D.RY
        mov sshfy,edx

        ;if (ebx.RX+ebx.RW>edi.SWidth)
DrawSurface8FS_jmp06:
        mov edx,[ebx+8]                 ;RW
        add edx,[ebx]                   ;RX+RW
        cmp edx,[edi]                   ;SWidth
        jg DrawSurface8FS_DRXGW          ;D.RX+D.RW>D.SWitdth
        sub edx,drect.RX                ;(RX+RW)-drect.RX
        mov drect.RW,edx
        jmp DrawSurface8FS_jmp07         ;omin
DrawSurface8FS_DRXGW:
        mov edx,[edi]                   ;SWidth
        sub edx,drect.RX                ;RX
        mov drect.RW,edx                ;SWidth-drect.RX
DrawSurface8FS_jmp07:

        ;if (ebx.RY+ebx.RH>edi.SHeight)
        mov edx,[ebx+12]                ;RH
        add edx,[ebx+4]                 ;RY+RH
        cmp edx,[edi+4]                 ;SHeight
        jg DrawSurface8FS_DRYGH          ;D.RX+D.RW>D.SHeight
        sub edx,drect.RY                ;(RY+RH)-drect.RY
        mov drect.RH,edx
        jmp DrawSurface8FS_jmp08         ;omin
DrawSurface8FS_DRYGH:
        mov edx,[edi+4]                 ;SHeight
        sub edx,drect.RY                ;RY
        mov drect.RH,edx                ;SHeight-drect.RY
DrawSurface8FS_jmp08:

        ;czy prostokat zrodlowy jest w widoczny
        cmp srect.RW,0
        jle DrawSurface8FS_end
        cmp srect.RH,0
        jle DrawSurface8FS_end
        
        ;czy prostokat docelowy jest w widoczny
        cmp drect.RW,0
        jle DrawSurface8FS_end
        cmp drect.RH,0
        jle DrawSurface8FS_end

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
        je DrawSurface8FS_deshfx_e_0
        mov eax,srect.RW
        shl eax,16
        mov ebx,wspx
        xor edx,edx
        div ebx
        mov ebx,drect.RW
        sub ebx,eax
        mov deshfx,ebx
DrawSurface8FS_deshfx_e_0:
        cmp deshfy,0
        je DrawSurface8FS_deshfy_e_0
        mov eax,srect.RH
        shl eax,16
        mov ebx,wspy
        xor edx,edx
        div ebx
        mov ebx,drect.RH
        sub ebx,eax
        mov deshfy,ebx
DrawSurface8FS_deshfy_e_0:

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
        jle DrawSurface8FS_end
        cmp repy,0
        jle DrawSurface8FS_end

        ;obliczenie skoku docelowego
        mov eax,[edi]                   ;SWidth
        sub eax,repx                    ;SWidth-repx
        mov dsjump,eax

        ;zapamietanie wskaznikow do bajtow powierzchni
        
        mov eax,[esi+8]                ;SBits
        mov srcbits,eax

        mov eax,[edi+8]                ;SBits
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

        ;wyliczenie pozy
        mov eax,sshfy
        xor edx,edx
        mov ecx,65536
        div ecx
        mov pozy,edx

        mov edx,esi

DrawSurface8FS_drawloopY:
        mov eax,repx
        mov repx2,eax
        mov dword ptr savepozx,ebx
        mov dword ptr savepozx+4,edx
DrawSurface8FS_drawloopX:
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
                mov al,[eax+esi]
                mov [edi],al
                inc edi
                dec repx2
                jnz DrawSurface8FS_drawloopX
        mov ecx,wspy
        add pozy,ecx
        mov ecx,pozy
        cmp ecx,65535
        jna DrawSurface8FS_no_next_line

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
        jnz DrawSurface8FS_drawloopY
        jmp DrawSurface8FS_end

        ;jesli ta sama linia
DrawSurface8FS_no_next_line:
        ;
        add edi,dsjump
        mov ebx,dword ptr savepozx
        mov edx,dword ptr savepozx+4
        dec repy
        jnz DrawSurface8FS_drawloopY

DrawSurface8FS_end:
        mov eax,descbits
        ret
DrawSurface8FS endp

;--------------------------------
;DrawSurface8T (TRect*,TRect*,TSurf*,TSurf*,int,int)
;       Rysuje powierzchnie 8-bitowa na innej powierzchni 8-bitowej (np. ekranu) bez skalowania z cieniowaniem i z przezroczystoscia
;       Wejscie: eax-adres do prostokata (TRect) zrodlowego
;                ebx-adres do prostokata (TRect) docelowego
;                ecx-stopien cieniowania (w zaleznosci od FADE8TabSize w fade8.inc)
;                edx-stopien przezroczystosci (w zaleznosci od TRANSP8TabSize w transp8.inc)
;                esi-adres do zrodlowej struktory TSurface
;                edi-adres do docelowej struktory TSurface
;       Wyjscie: eax-bajty powierzchni docelowej lub NULL jesli nie bylo kopiowania
;--------------------------------

DrawSurface8T proc C uses edi esi ebx,_srect:dword,_drect:dword,_ssurf:dword,_dsurf:dword,_fade:dword,_transp:dword
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
        mov eax,_srect
        mov ebx,_drect
        mov ecx,_fade
        mov edx,_transp
        mov esi,_ssurf
        mov edi,_dsurf
        mov descbits,0
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

        mov eax,[esi+8]                ;SBits
        mov srcbits,eax

        mov eax,[edi+8]                ;SBits
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

DrawSurface8T_end:
        mov eax,descbits
        ret
DrawSurface8T endp

;--------------------------------
;DrawSurface8TS (TRect*,TRect*,TSurf*,TSurf*,int,int)
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

DrawSurface8TS proc C uses edi esi ebx,_srect:dword,_drect:dword,_ssurf:dword,_dsurf:dword,_fade:dword,_transp:dword
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
        mov eax,_srect
        mov ebx,_drect
        mov ecx,_fade
        mov edx,_transp
        mov esi,_ssurf
        mov edi,_dsurf
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
        mov descbits,0

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
        
        mov eax,[esi+8]                ;SBits
        mov srcbits,eax

        mov eax,[edi+8]                ;SBits
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
        jmp DrawSurface8TS_end

        ;jesli ta sama linia
DrawSurface8TS_no_next_line:
        ;
        add edi,dsjump
        mov ebx,dword ptr savepozx
        mov edx,dword ptr savepozx+4
        dec repy
        jnz DrawSurface8TS_drawloopY

DrawSurface8TS_end:
        mov eax,descbits
        ret
DrawSurface8TS endp

;zerocpy.inc

;kopiuje 'n' bajtow pamieci z 's' do 'd', ale d jest adresem fizycznym
_cpyd0 proc C uses esi edi es,d:dword,s:dword,n:dword
        cli
        cld
        mov es,stub32zerosel
        mov esi,s
        mov edi,d
        mov eax,n
        mov ecx,eax
        shr ecx,2
        and eax,3
        rep movsd
        mov ecx,eax
        rep movsb
        sti
        ret
_cpyd0 endp

;kopiuje 'n' bajtow pamieci z 's' do 'd', ale s jest adresem fizycznym
_cpys0 proc C uses esi edi ds,d:dword,s:dword,n:dword
        cli
        cld
        mov ds,stub32zerosel
        mov esi,s
        mov edi,d
        mov eax,n
        mov ecx,eax
        shr ecx,2
        and eax,3
        rep movsd
        mov ecx,eax
        rep movsb
        sti
        ret
_cpys0 endp

;wypelnia 'n' bajtow pamieci wartoscia 'v' pamiec pod adresem fizycznym 'm'
_fill0 proc C uses edi es,m:dword,n:dword,v:dword
        cli
        cld
        mov es,stub32zerosel
        mov edi,m
        mov edx,n
        mov eax,v
        mov ecx,edx
        shr ecx,2
        and edx,3
        rep stosd
        mov ecx,edx
        rep stosb
        sti
        ret
_fill0 endp

;text8x8.inc

;funkcje

;--------------------------------
;DrawString8X8 (int,int,int,void*,char*,int,TSurf*)
;       Rysuje ciag ASCIIZ na powierzchni 8-bitowej
;--------------------------------

DrawString8X8 proc C uses ebx esi edi ,x:dword,y:dword,col:dword,charsstart:dword,txt:dword,slen:dword,surf:dword
        local shfx:dword
        local chars_height:dword
        local chars_ystart:dword
        local xl:dword,xl2:dword,xs:dword,xw:dword,clstrt:dword,sstart:dword
        ;
        mov chars_height,8
        mov chars_ystart,0
        mov xs,0
        mov xw,8
        mov edi,surf

        ;Sprawdzenie czy tekst miesci sie na ekranie
        mov eax,y
        cmp eax,-8
        jle DrawString8X8_end_proc        
        cmp eax,[edi+4]         ;SHeight
        jge DrawString8X8_end_proc
        ;
        cmp eax,0
        jge DrawString8X8_yge0
        neg eax
        mov chars_ystart,eax
        mov y,0
DrawString8X8_yge0:
        add eax,8
        cmp eax,[edi+4]         ;SHeight
        jle DrawString8X8_ylesh
        sub eax,[edi+4]
        sub chars_height,eax
DrawString8X8_ylesh:
        ;
        mov esi,txt
        mov eax,x
        cmp eax,[edi]           ;SWidth
        jge DrawString8X8_end_proc
        ;
        cmp eax,0
        jge DrawString8X8_xge0
        neg eax
        mov edx,eax
        shr eax,3
        add esi,eax
        sub slen,eax
        cmp slen,0
        jle DrawString8X8_end_proc
        ;xs=-x-p*8
        shl eax,3
        sub edx,eax
        mov xs,edx
        mov x,0
        ;
DrawString8X8_xge0:        
        mov edx,x
        sub edx,xs
        mov eax,slen
        shl eax,3               ;eax*8
        add edx,eax
        cmp edx,[edi]           ;SWidth
        jle DrawString8X8_no_sub_slen
        sub edx,[edi]           ;SWidth
        shr edx,3
        sub slen,edx
        cmp slen,0
        jle DrawString8X8_end_proc
        mov edx,x
        sub edx,xs
        mov eax,slen
        shl eax,3               ;eax*8
        add edx,eax
        sub edx,[edi]           ;SWidth
        mov ecx,8
        sub ecx,edx
        mov xw,ecx
DrawString8X8_no_sub_slen:

        ;Wyliczenie poczatku (eax=x ebx=y)
        mov eax,y
        mov ecx,[edi]           ;SWidth
        mov shfx,ecx        
        mul ecx
        add eax,x        
        mov edi,[edi+8]         ;SBits
        add edi,eax             ;y*SWidth+x+SBits
        ;
        mov sstart,esi

        ;Rysowanie: edi=Surface.SBits edx=bios_chars_ba                
DrawString8X8_draw_loop:
                movzx eax,byte ptr [esi]        ;pobranie znaku                
                mov edx,charsstart
                shl eax,3
                add edx,eax
                mov ebx,chars_ystart
                mov xl,8
                mov clstrt,8
                push edi
                cmp esi,sstart
                jne DrawString8X8_esinesstart                
                mov eax,xs
                sub xl,eax
                sub clstrt,eax
DrawString8X8_esinesstart:
                cmp slen,1
                jne DrawString8X8_loop_y
                mov eax,xw
                mov xl,eax
DrawString8X8_loop_y:
                        mov eax,xl
                        mov xl2,eax
                        mov al,[edx+ebx]
                        mov ecx,clstrt
DrawString8X8_loop_x:
                                push eax
                                shl eax,1
                                shr eax,cl
                                and eax,1
                                or al,al
                                jz DrawString8X8_no_put_pixel
                                mov eax,col
                                mov [edi],al
DrawString8X8_no_put_pixel:
                                inc edi
                                pop eax
                                dec ecx
                                dec xl2
                                jnz DrawString8X8_loop_x
                        add edi,shfx
                        sub edi,xl
                        inc ebx
                        cmp ebx,chars_height
                        jb DrawString8X8_loop_y
                pop edi
                add edi,xl
                inc esi
                dec slen
                jnz DrawString8X8_draw_loop
        mov eax,1
        jmp DrawString8X8_end_proc_ok
DrawString8X8_end_proc:
        xor eax,eax
DrawString8X8_end_proc_ok:
        ret
DrawString8X8 endp

;--------------------------------
;DrawString8X8S (int,int,TSurf*,void*,char*,int,TSurf*)
;       Rysuje ciag ASCIIZ na powierzchni 8-bitowej z powierzchnia w tle
;       Wykorzystywane sa tylko pierwsze 64 (8*8) bajty powierzchni oraz
;       powierzchnia musi byc rozmiarow 8X8.
;--------------------------------

DrawString8X8S proc C uses ebx esi edi ,x:dword,y:dword,bsurf:dword,charsstart:dword,txt:dword,slen:dword,surf:dword
        local shfx:dword
        local chars_height:dword
        local chars_ystart:dword
        local xl:dword,xl2:dword,xs:dword,xw:dword,clstrt:dword,sstart:dword
        local bsl:dword
        ;
        mov chars_height,8
        mov chars_ystart,0
        mov xs,0
        mov xw,8
        mov edi,surf

        ;Sprawdzenie czy tekst miesci sie na ekranie
        mov eax,y
        cmp eax,-8
        jle DrawString8X8S_end_proc        
        cmp eax,[edi+4]         ;SHeight
        jge DrawString8X8S_end_proc
        ;
        cmp eax,0
        jge DrawString8X8S_yge0
        neg eax
        mov chars_ystart,eax
        mov y,0
DrawString8X8S_yge0:
        add eax,8
        cmp eax,[edi+4]         ;SHeight
        jle DrawString8X8S_ylesh
        sub eax,[edi+4]
        sub chars_height,eax
DrawString8X8S_ylesh:
        ;
        mov esi,txt
        mov eax,x
        cmp eax,[edi]           ;SWidth
        jge DrawString8X8S_end_proc
        ;
        cmp eax,0
        jge DrawString8X8S_xge0
        neg eax
        mov edx,eax
        shr eax,3
        add esi,eax
        sub slen,eax
        cmp slen,0
        jle DrawString8X8S_end_proc
        ;xs=-x-p*8
        shl eax,3
        sub edx,eax
        mov xs,edx
        mov x,0
        ;
DrawString8X8S_xge0:        
        mov edx,x
        sub edx,xs
        mov eax,slen
        shl eax,3               ;eax*8
        add edx,eax
        cmp edx,[edi]           ;SWidth
        jle DrawString8X8S_no_sub_slen
        sub edx,[edi]           ;SWidth
        shr edx,3
        sub slen,edx
        cmp slen,0
        jle DrawString8X8S_end_proc
        mov edx,x
        sub edx,xs
        mov eax,slen
        shl eax,3               ;eax*8
        add edx,eax
        sub edx,[edi]           ;SWidth
        mov ecx,8
        sub ecx,edx
        mov xw,ecx
DrawString8X8S_no_sub_slen:

        ;Wyliczenie poczatku (eax=x ebx=y)
        mov eax,y
        mov ecx,[edi]           ;SWidth
        mov shfx,ecx        
        mul ecx
        add eax,x        
        mov edi,[edi+8]         ;SBits
        add edi,eax             ;y*SWidth+x+SBits
        ;
        mov sstart,esi

        ;Rysowanie: edi=Surface.SBits edx=bios_chars_ba                
DrawString8X8S_draw_loop:
                movzx eax,byte ptr [esi]        ;pobranie znaku                
                mov edx,charsstart
                shl eax,3
                add edx,eax
                mov ebx,chars_ystart
                mov xl,8
                mov clstrt,8
                mov bsl,0
                push edi
                cmp esi,sstart
                jne DrawString8X8S_esinesstart                
                mov eax,xs
                sub xl,eax
                sub clstrt,eax
DrawString8X8S_esinesstart:
                cmp slen,1
                jne DrawString8X8S_loop_y
                mov eax,xw
                mov xl,eax
DrawString8X8S_loop_y:
                        mov eax,xl
                        mov xl2,eax
                        mov al,[edx+ebx]
                        mov ecx,clstrt
DrawString8X8S_loop_x:
                                push eax
                                shl eax,1
                                shr eax,cl
                                and eax,1
                                or al,al
                                jz DrawString8X8S_no_put_pixel
                                mov eax,bsurf
                                mov eax,[eax+8]
                                add eax,bsl
                                mov al,[eax]
                                mov [edi],al
DrawString8X8S_no_put_pixel:
                                inc edi
                                inc bsl
                                pop eax
                                dec ecx
                                dec xl2
                                jnz DrawString8X8S_loop_x
                        add edi,shfx
                        sub edi,xl
                        inc ebx
                        cmp ebx,chars_height
                        jb DrawString8X8S_loop_y
                pop edi
                add edi,xl
                inc esi
                dec slen
                jnz DrawString8X8S_draw_loop
        mov eax,1
        jmp DrawString8X8S_end_proc_ok
DrawString8X8S_end_proc:
        xor eax,eax
DrawString8X8S_end_proc_ok:
        ret
DrawString8X8S endp

;fnt.inc

comment #
typedef struct _TSurf
{
0: int swidth;(4)               //Szerokosc powierzchni
4: int sheight;(4)              //Wysokosc powierzchni
8: unsigned char *sbits;(4)     //Wskaznik do bajtow powierzchni
} TSurf;
#


comment #
typedef struct _TFnt
{
0:      unsigned char ctab[256];(256)   //tablica znakow(['znak']->pozycja znaku)
256:    int width,(4)
260:    height;(4)                      //szerokosc i wysokosc czcionki
264:    int space;(4)                   //odstep pomiedzy kolejnymi znakami
268:    unsigned int charsize;(4)       //dlugosc znaku w bajtach=width*height
272:    unsigned char *fnttab;(4)       //tablica znakow(0/1)
} TFnt;
#

;--------------------------------
;DrawStringFnt (int,int,int,TFnt *,TSurf *,int,char *)
;       Rysuje ciag ASCIIZ dla danej czcionki na powierzchni 8-bitowej
;--------------------------------

DrawStringFnt proc C uses ebx esi edi ,x:dword,y:dword,col:dword,fnt:dword,\
                                       surf:dword,slen:dword,txt:dword
        ;
        local px:dword,py:dword,kx:dword,ky:dword       ;pocztek i koniec fnt
        local widthspace:dword                          ;fnt.width+fnt.space
        local lastx:dword                               ;konowa pozycja x ciagu
        local fnth:dword                                ;koncowa wys. czcionki
        local fntw:dword                                ;koncowa szer. czcionki
        local fntstart:dword                            ;pocz. czcionki
        local surfshf:dword                             ;przesuniecie na nast. linie
        local surfstart:dword                           ;pocz. powierzchni dla rysowania
        local charsize:dword                            ;=fnt.charsize
        local cnty:dword                                ;pomocniczy licznik
        local jmplast:dword                             ;czy skoczyc do konca
        ;
        mov esi,fnt
        mov edi,surf
        mov eax,[esi+256]       ;fnt.width
        mov kx,eax
        add eax,[esi+264]       ;fnt.space
        mov widthspace,eax
        mov eax,[esi+260]       ;fnt.height
        mov ky,eax
        mov px,0
        mov py,0
        mov eax,widthspace
        mul slen
        add eax,x
        mov lastx,eax
        mov eax,[esi+268]
        mov charsize,eax
        mov jmplast,0

        ;x<0?
        mov eax,x
        cmp x,0
        jge DrawStringFnt_d1
        
        ;slen-=(-x/widthspace) | eax=x | x=0        
        neg eax
        xor edx,edx
        div widthspace
        sub slen,eax
        mov x,0

        ;nowy poczatek tekstu
        add txt,eax

        ;(edx=px)>=fnt.width? (jesli tak to utnij znak i dodaj do x)
        cmp edx,[esi+256]       ;fnt.width
        jb DrawStringFnt_d2
        dec slen
        inc txt
        mov eax,widthspace
        sub eax,edx
        add x,eax
        xor edx,edx

DrawStringFnt_d2:
        mov px,edx

DrawStringFnt_d1:
        ;((lastx-fnt.space)>surf.swidth)?
        mov eax,lastx
        sub eax,[esi+264]       ;fnt.space
        cmp eax,[edi]           ;surf.swidth
        jb DrawStringFnt_d3

        ;((lastx-fnt.space-fnt.width-surf.width)<0)?
        sub eax,[edi]           ;surf.swidth
        sub eax,[esi+256]       ;fnt.width
        cmp eax,0
        jl DrawStringFnt_d4

        ;slen-=(lastx-surf.swidth)/widthspace
        mov eax,lastx
        sub eax,[edi]           ;surf.swidth
        xor edx,edx
        div widthspace
        sub slen,eax

        ;kx=widthspace-edx
        mov eax,widthspace
        sub eax,edx
        mov kx,eax

        mov eax,slen
        mov jmplast,eax

        ;kx>fnt.width
        mov eax,kx
        cmp eax,[esi+256]       ;fnt.width
        jbe DrawStringFnt_d3
        mov eax,[esi+256]       ;fnt.width
        mov kx,eax

        jmp DrawStringFnt_d3

DrawStringFnt_d4:
        neg eax
        mov kx,eax
        mov eax,slen
        mov jmplast,eax

DrawStringFnt_d3:
        ;czy napis widoczny
        cmp slen,0
        jle DrawStringFnt_end0

        ;y<0?
        mov eax,y
        cmp eax,0
        jge DrawStringFnt_d5

        ;czy napis widoczny
        add eax,[esi+260]       ;fnt.height
        cmp eax,0
        jle DrawStringFnt_end0

        ;py=-y
        mov eax,y
        neg eax
        mov py,eax
        mov y,0

        jmp DrawStringFnt_d6
DrawStringFnt_d5:
        ;y>surf.sheight? (czy napis widoczny)
        cmp eax,[edi+4]         ;surf.sheight
        jge DrawStringFnt_end0

        ;y+fnt.height>surf.sheight
        add eax,[esi+260]       ;fnt.height
        cmp eax,[edi+4]         ;surf.sheight
        jle DrawStringFnt_d6

        ;ky=surf.sheight-y
        mov eax,[edi+4]         ;surf.sheight
        sub eax,y
        mov ky,eax

DrawStringFnt_d6:
        ;Inicjacja

        ;fnth=ky-py
        mov eax,ky
        sub eax,py
        mov fnth,eax        

        ;surfstart=y*surf.swidth+x
        mov eax,y
        mul dword ptr [edi]     ;surf.swidth
        add eax,x
        add eax,[edi+8]         ;surf.sbits
        mov surfstart,eax
        
        mov ebx,esi             ;ebx=&fnt.ctab
        mov edx,txt

        cmp jmplast,1
        je DrawStringFnt_last        


        ;ETAP I (I znak)
        mov eax,py
        imul eax,dword ptr [esi+256]    ;fnt.width
        add eax,px
        add eax,[esi+272]       ;fnt.fnttab
        mov fntstart,eax

        ;surfshf=surf.swidth-(fnt.width-px)
        mov eax,[edi]           ;surf.swidth
        sub eax,[esi+256]       ;fnt.width
        add eax,px
        mov surfshf,eax

        ;fntw=(fnt.width-px)
        mov eax,[esi+256]       ;fnt.width
        sub eax,px
        mov fntw,eax                

        push edi        

;petla - I znak
                ;eax=ctab[txt[edx]]*fnt.charsize+fnt.fnttab
                xor eax,eax
                mov al,[edx]
                mov al,[ebx+eax]
                cmp al,255
                je DrawStringFnt_loop1_255
                imul eax,charsize
                add eax,fntstart

                mov edi,surfstart

                push esi
                mov esi,eax
                mov ecx,fnth
                mov cnty,ecx
                mov eax,col
                shl eax,8               ;ah=col                

DrawStringFnt_loop1y:        ;y
                        mov ecx,fntw
DrawStringFnt_loop1x:        ;x
                                mov al,[esi]
                                or al,al

                                ;czy przezroczysty
                                jz DrawStringFnt_loop1xd1
                                mov [edi],ah

DrawStringFnt_loop1xd1:
                                inc esi
                                inc edi
                                loop DrawStringFnt_loop1x

                        add edi,surfshf
                        add esi,px
                        dec cnty
                        jnz DrawStringFnt_loop1y

                pop esi

                mov eax,fntw
                add eax,[esi+264]       ;fnt.space
                add surfstart,eax

DrawStringFnt_loop1_255:

                inc edx

                dec slen

        pop edi

        cmp slen,0
        je DrawStringFnt_end

        cmp jmplast,2
        je DrawStringFnt_last


        ;ETAP II (glowny)
        mov eax,py
        imul eax,dword ptr [esi+256]    ;fnt.swidth
        add eax,[esi+272]       ;fnt.fnttab
        mov fntstart,eax

        ;surfshf=surf.swidth-fnt.width i fntw=fnt.width
        mov eax,[edi]           ;surf.swidth
        sub eax,[esi+256]       ;fnt.width
        mov surfshf,eax

        ;fntw=fnt.width
        mov eax,[esi+256]       ;fnt.width
        mov fntw,eax

        push edi

;glowna petla
DrawStringFnt_mainloopc:        ;znaki
                ;eax=ctab[txt[edx]]*fnt.charsize+fnt.fnttab
                xor eax,eax
                mov al,[edx]
                mov al,[ebx+eax]
                cmp al,255
                je DrawStringFnt_mainloop_255
                imul eax,charsize
                add eax,fntstart

                mov edi,surfstart

                push esi
                mov esi,eax
                mov ecx,fnth
                mov cnty,ecx
                mov eax,col
                shl eax,8               ;ah=col                

DrawStringFnt_mainloopy:        ;y
                        mov ecx,fntw
DrawStringFnt_mainloopx:        ;x
                                mov al,[esi]
                                or al,al

                                ;czy przezroczysty
                                jz DrawStringFnt_mainloopxd1
                                mov [edi],ah

DrawStringFnt_mainloopxd1:
                                inc esi
                                inc edi
                                loop DrawStringFnt_mainloopx

                        add edi,surfshf
                        ;~add esi
                        dec cnty
                        jnz DrawStringFnt_mainloopy

                pop esi

DrawStringFnt_mainloop_255:

                mov eax,widthspace
                add surfstart,eax

                inc edx

                dec slen
                cmp slen,1
                ja DrawStringFnt_mainloopc


        pop edi


DrawStringFnt_last:
        ;ETAP III (ostatni znak)
        mov eax,py
        imul eax,dword ptr [esi+256]    ;fnt.swidth
        add eax,[esi+272]       ;fnt.fnttab
        mov fntstart,eax

        ;surfshf=surf.swidth-kx
        mov eax,[edi]           ;surf.swidth
        sub eax,kx
        mov surfshf,eax

        ;px=fnt.width-kx
        mov eax,[esi+256]       ;fnt.width
        sub eax,kx
        mov px,eax

        push edi        

;petla - ostatni znak
                ;eax=ctab[txt[edx]]*fnt.charsize+fnt.fnttab
                xor eax,eax
                mov al,[edx]
                mov al,[ebx+eax]
                cmp al,255
                je DrawStringFnt_loop2_255
                imul eax,charsize
                add eax,fntstart

                mov edi,surfstart

                push esi
                mov esi,eax
                mov ecx,fnth
                mov cnty,ecx
                mov eax,col
                shl eax,8               ;ah=col                

DrawStringFnt_loop2y:        ;y
                        mov ecx,kx              ;fntw=kx
DrawStringFnt_loop2x:        ;x
                                mov al,[esi]
                                or al,al

                                ;czy przezroczysty
                                jz DrawStringFnt_loop2xd1
                                mov [edi],ah

DrawStringFnt_loop2xd1:
                                inc esi
                                inc edi
                                loop DrawStringFnt_loop2x

                        add edi,surfshf
                        add esi,px
                        dec cnty
                        jnz DrawStringFnt_loop2y

                pop esi

DrawStringFnt_loop2_255:

        pop edi

        jmp DrawStringFnt_end

DrawStringFnt_end0:
        xor eax,eax
        ret
DrawStringFnt_end:
        mov eax,1
        ret
DrawStringFnt endp

;--------------------------------
;DrawStringFntS (int,int,TSurf *,TFnt *,TSurf *,int,char *)
;       Rysuje ciag ASCIIZ dla danej czcionki na powierzchni 8-bitowej z powiercznia
;--------------------------------

DrawStringFntS proc C uses ebx esi edi ,x:dword,y:dword,bsurf:dword,fnt:dword,\
                                       surf:dword,slen:dword,txt:dword
        ;
        local px:dword,py:dword,kx:dword,ky:dword       ;pocztek i koniec fnt
        local widthspace:dword                          ;fnt.width+fnt.space
        local lastx:dword                               ;konowa pozycja x ciagu
        local fnth:dword                                ;koncowa wys. czcionki
        local fntw:dword                                ;koncowa szer. czcionki
        local fntstart:dword                            ;pocz. czcionki
        local surfshf:dword                             ;przesuniecie na nast. linie
        local surfstart:dword                           ;pocz. powierzchni dla rysowania
        local charsize:dword                            ;=fnt.charsize
        local cnty:dword                                ;pomocniczy licznik
        local jmplast:dword                             ;czy skoczyc do konca
        local bsurfsbits:dword                          ;adres do powierczni tylnej
        local bsurfsbitspom:dword                       ;adres do powierczni tylnej dla zapamietania
        ;
        mov esi,fnt
        mov edi,surf
        mov eax,[esi+256]       ;fnt.width
        mov kx,eax
        add eax,[esi+264]       ;fnt.space
        mov widthspace,eax
        mov eax,[esi+260]       ;fnt.height
        mov ky,eax
        mov px,0
        mov py,0
        mov eax,widthspace
        mul slen
        add eax,x
        mov lastx,eax
        mov eax,[esi+268]
        mov charsize,eax
        mov jmplast,0
        mov eax,bsurf
        mov eax,[eax+8]         ;bsurf.sbits
        mov bsurfsbitspom,eax

        ;x<0?
        mov eax,x
        cmp x,0
        jge DrawStringFntS_d1
        
        ;slen-=(-x/widthspace) | eax=x | x=0        
        neg eax
        xor edx,edx
        div widthspace
        sub slen,eax
        mov x,0

        ;nowy poczatek tekstu
        add txt,eax

        ;(edx=px)>=fnt.width? (jesli tak to utnij znak i dodaj do x)
        cmp edx,[esi+256]       ;fnt.width
        jb DrawStringFntS_d2
        dec slen
        inc txt
        mov eax,widthspace
        sub eax,edx
        add x,eax
        xor edx,edx

DrawStringFntS_d2:
        mov px,edx

DrawStringFntS_d1:
        ;((lastx-fnt.space)>surf.swidth)?
        mov eax,lastx
        sub eax,[esi+264]       ;fnt.space
        cmp eax,[edi]           ;surf.swidth
        jb DrawStringFntS_d3

        ;((lastx-fnt.space-fnt.width-surf.width)<0)?
        sub eax,[edi]           ;surf.swidth
        sub eax,[esi+256]       ;fnt.width
        cmp eax,0
        jl DrawStringFntS_d4

        ;slen-=(lastx-surf.swidth)/widthspace
        mov eax,lastx
        sub eax,[edi]           ;surf.swidth
        xor edx,edx
        div widthspace
        sub slen,eax

        ;kx=widthspace-edx
        mov eax,widthspace
        sub eax,edx
        mov kx,eax

        mov eax,slen
        mov jmplast,eax

        ;kx>fnt.width
        mov eax,kx
        cmp eax,[esi+256]       ;fnt.width
        jbe DrawStringFntS_d3
        mov eax,[esi+256]       ;fnt.width
        mov kx,eax

        jmp DrawStringFntS_d3

DrawStringFntS_d4:
        neg eax
        mov kx,eax
        mov eax,slen
        mov jmplast,eax

DrawStringFntS_d3:
        ;czy napis widoczny
        cmp slen,0
        jle DrawStringFntS_end0

        ;y<0?
        mov eax,y
        cmp eax,0
        jge DrawStringFntS_d5

        ;czy napis widoczny
        add eax,[esi+260]       ;fnt.height
        cmp eax,0
        jle DrawStringFntS_end0

        ;py=-y
        mov eax,y
        neg eax
        mov py,eax
        mov y,0

        jmp DrawStringFntS_d6
DrawStringFntS_d5:
        ;y>surf.sheight? (czy napis widoczny)
        cmp eax,[edi+4]         ;surf.sheight
        jge DrawStringFntS_end0

        ;y+fnt.height>surf.sheight
        add eax,[esi+260]       ;fnt.height
        cmp eax,[edi+4]         ;surf.sheight
        jle DrawStringFntS_d6

        ;ky=surf.sheight-y
        mov eax,[edi+4]         ;surf.sheight
        sub eax,y
        mov ky,eax

DrawStringFntS_d6:
        ;Inicjacja

        ;fnth=ky-py
        mov eax,ky
        sub eax,py
        mov fnth,eax        

        ;surfstart=y*surf.swidth+x
        mov eax,y
        mul dword ptr [edi]     ;surf.swidth
        add eax,x
        add eax,[edi+8]         ;surf.sbits
        mov surfstart,eax
        
        mov ebx,esi             ;ebx=&fnt.ctab
        mov edx,txt

        cmp jmplast,1
        je DrawStringFntS_last        


        ;ETAP I (I znak)
        mov eax,py
        imul eax,dword ptr [esi+256]    ;fnt.width
        add eax,px
        add bsurfsbitspom,eax
        add eax,[esi+272]       ;fnt.fnttab
        mov fntstart,eax        

        ;surfshf=surf.swidth-(fnt.width-px)
        mov eax,[edi]           ;surf.swidth
        sub eax,[esi+256]       ;fnt.width
        add eax,px
        mov surfshf,eax

        ;fntw=(fnt.width-px)
        mov eax,[esi+256]       ;fnt.width
        sub eax,px
        mov fntw,eax

        push edi        

;petla - I znak
                ;eax=ctab[txt[edx]]*fnt.charsize+fnt.fnttab
                xor eax,eax
                mov al,[edx]
                mov al,[ebx+eax]
                cmp al,255
                je DrawStringFntS_loop1_255
                imul eax,charsize
                add eax,fntstart

                mov edi,surfstart

                push esi
                mov esi,eax
                mov ecx,fnth
                mov cnty,ecx
                mov eax,bsurfsbitspom
                mov bsurfsbits,eax


DrawStringFntS_loop1y:        ;y
                        mov ecx,fntw
DrawStringFntS_loop1x:        ;x
                                mov al,[esi]
                                or al,al

                                ;czy przezroczysty
                                jz DrawStringFntS_loop1xd1
                                mov eax,bsurfsbits
                                mov al,[eax]
                                mov [edi],al

DrawStringFntS_loop1xd1:
                                inc esi
                                inc edi
                                inc bsurfsbits
                                loop DrawStringFntS_loop1x

                        add edi,surfshf
                        add esi,px
                        mov eax,bsurfsbits
                        add eax,px
                        mov bsurfsbits,eax
                        dec cnty
                        jnz DrawStringFntS_loop1y

                pop esi

                mov eax,fntw
                add eax,[esi+264]       ;fnt.space
                add surfstart,eax

DrawStringFntS_loop1_255:

                inc edx

                dec slen

        pop edi

        cmp slen,0
        je DrawStringFntS_end

        cmp jmplast,2
        je DrawStringFntS_last


        ;ETAP II (glowny)
        mov eax,px
        sub bsurfsbitspom,eax
        mov eax,py
        imul eax,dword ptr [esi+256]    ;fnt.swidth
        add eax,[esi+272]       ;fnt.fnttab
        mov fntstart,eax

        ;surfshf=surf.swidth-fnt.width i fntw=fnt.width
        mov eax,[edi]           ;surf.swidth
        sub eax,[esi+256]       ;fnt.width
        mov surfshf,eax

        ;fntw=fnt.width
        mov eax,[esi+256]       ;fnt.width
        mov fntw,eax

        push edi

;glowna petla
DrawStringFntS_mainloopc:        ;znaki
                ;eax=ctab[txt[edx]]*fnt.charsize+fnt.fnttab
                xor eax,eax
                mov al,[edx]
                mov al,[ebx+eax]
                cmp al,255
                je DrawStringFntS_mainloop_255
                imul eax,charsize
                add eax,fntstart

                mov edi,surfstart

                push esi
                mov esi,eax
                mov ecx,fnth
                mov cnty,ecx
                mov eax,bsurfsbitspom
                mov bsurfsbits,eax

DrawStringFntS_mainloopy:        ;y
                        mov ecx,fntw
DrawStringFntS_mainloopx:        ;x
                                mov al,[esi]
                                or al,al

                                ;czy przezroczysty
                                jz DrawStringFntS_mainloopxd1
                                mov eax,bsurfsbits
                                mov al,[eax]
                                mov [edi],al

DrawStringFntS_mainloopxd1:
                                inc esi
                                inc edi
                                inc bsurfsbits
                                loop DrawStringFntS_mainloopx

                        add edi,surfshf
                        ;~add esi
                        dec cnty
                        jnz DrawStringFntS_mainloopy

                pop esi

DrawStringFntS_mainloop_255:

                mov eax,widthspace
                add surfstart,eax

                inc edx

                dec slen
                cmp slen,1
                ja DrawStringFntS_mainloopc


        pop edi


DrawStringFntS_last:
        ;ETAP III (ostatni znak)
        mov eax,py
        imul eax,dword ptr [esi+256]    ;fnt.swidth
        add eax,[esi+272]       ;fnt.fnttab
        mov fntstart,eax

        ;surfshf=surf.swidth-kx
        mov eax,[edi]           ;surf.swidth
        sub eax,kx
        mov surfshf,eax

        ;px=fnt.width-kx
        mov eax,[esi+256]       ;fnt.width
        sub eax,kx
        mov px,eax

        push edi        

;petla - ostatni znak
                ;eax=ctab[txt[edx]]*fnt.charsize+fnt.fnttab
                xor eax,eax
                mov al,[edx]
                mov al,[ebx+eax]
                cmp al,255
                je DrawStringFntS_loop2_255
                imul eax,charsize
                add eax,fntstart

                mov edi,surfstart

                push esi
                mov esi,eax
                mov ecx,fnth
                mov cnty,ecx
                mov eax,bsurfsbitspom
                mov bsurfsbits,eax

DrawStringFntS_loop2y:        ;y
                        mov ecx,kx              ;fntw=kx
DrawStringFntS_loop2x:        ;x
                                mov al,[esi]
                                or al,al

                                ;czy przezroczysty
                                jz DrawStringFntS_loop2xd1
                                mov eax,bsurfsbits
                                mov al,[eax]
                                mov [edi],al

DrawStringFntS_loop2xd1:
                                inc esi
                                inc edi
                                inc bsurfsbits
                                loop DrawStringFntS_loop2x

                        add edi,surfshf
                        add esi,px
                        mov eax,bsurfsbits
                        add eax,px
                        mov bsurfsbits,eax
                        dec cnty
                        jnz DrawStringFntS_loop2y

                pop esi

DrawStringFntS_loop2_255:

        pop edi

        jmp DrawStringFntS_end

DrawStringFntS_end0:
        xor eax,eax
        ret
DrawStringFntS_end:
        mov eax,1
        ret
DrawStringFntS endp


;kb.inc

extern key_scantokeytab:byte,key_keytab:byte,key_isshiftp:byte

;Procedura do obslugi klawiatury

__Int09_Keyboard_Handler proc C
        push eax
        push edx
        push ds
        cli
        xor eax,eax
        in al,60h                       ;al=scan kod+pressed?
        mov dl,al                       ;dl=scan kod+pressed?
        and eax,127                     ;al=scan kod
        shr dl,7                        ;dl=pressed?
        xor dl,1                        ;dl=not pressed?
        cmp eax,42
        jne @f
        mov key_isshiftp,dl
        jmp quit
@@:
        mov al,[key_scantokeytab+eax]
        mov [key_keytab+eax],dl
quit:
        mov al,20h
        out 20h,al
        sti
        pop ds
        pop edx
        pop eax
        iretd
__Int09_Keyboard_Handler endp

;time.inc

Time struct
        current_time dq 0
        flags dd 0
Time ends

;flags:
;[0..2]-rodzaj zwracanego czasu:000-absolutny,001-w milisek,010-w sek,011-w min,100-w godz
;[3..3]-rodzaj timera:0-najlepszy,1-zawsze clock (zegar systemowy)

extern __cpu__tsc__freq:qword,Time_Get:dword
extern __float__time__0_001:real4,\
__float__time__1_0:real4,\
__float__time__60_0:real4,\
__float__time__3600_0:real4,\
__float__time__0_018:real4,\
__float__time__18_2:real4,\
__float__time__1092:real4,\
__float__time__65520:real4,\
__float__time__temp:real4


;--------------------------------
;_InitTime ()
;       Initializuje procedury do obslugi czasu
;       Wyjscie: eax=0-czas jest pobierany z uzyciem zegara systemowego (mala dokladnosc)
;                eax=1-czas jest pobierany z uzyciem zegara procesora (bardzo duza dokladnosc)
;       Uwaga: Funkcja musi byc wywolana na poczatku
;--------------------------------

_InitTime proc C uses edi es
        local t1:dword,t2:dword
        mov eax,cpu_feat
        and eax,10000b
        jnz InitTime_tsc
        xor eax,eax
        mov dword ptr __cpu__tsc__freq,eax
        mov dword ptr __cpu__tsc__freq+4,eax
        xor eax,eax
        ret
InitTime_tsc:
        mov es,stub32zerosel
        mov edi,1132
        mov eax,es:[edi]
InitTime_waitt1:
        cmp eax,es:[edi]
        je InitTime_waitt1
.586p
        rdtsc
        mov t1,eax
        mov t2,edx
        mov ecx,4
InitTime_waittloop:
        mov eax,es:[edi]
InitTime_waitt2:
        cmp eax,es:[edi]
        je InitTime_waitt2
        loop InitTime_waittloop
        rdtsc
.386
        sub eax,t1
        sbb edx,t2
        mov ebx,298188
        mul ebx
        shrd eax,edx,16
        shr edx,16
        mov dword ptr __cpu__tsc__freq,eax
        mov dword ptr __cpu__tsc__freq+4,edx
        xor eax,eax
        inc eax
        ret
_InitTime endp

;--------------------------------
;Time_Get (eax)
;       Pobiera do struktory Time aktualny czas oraz zwraca roznice czasu w danej jednostce (Time.flags)
;       (liczba zmiennopozycyjna 32bit)
;       Wejscie: eax-offset do strutkury Time
;       Wyjscie: st(0)-czas oraz zmieniona struktora
;--------------------------------

.586p

assume eax:ptr Time

__gettime__clock__ proc C uses ebx ,ttime:dword
        mov eax,ttime
        mov ebx,[eax].flags
        and ebx,7
        or ebx,ebx
        jz __gettime__clock__absolute
        cmp ebx,1
        je __gettime__clock__ms
        cmp ebx,2
        je __gettime__clock__s
        cmp ebx,3
        je __gettime__clock__min
        cmp ebx,4
        je __gettime__clock__h
        ffree st(0)
        ret

__gettime__clock__absolute:
        fild [eax].current_time
        mov es,stub32zerosel
        mov ebx,1132
        mov ebx,es:[ebx]
        mov dword ptr [eax].current_time,ebx
        xor ebx,ebx
        mov dword ptr [eax].current_time+4,ebx
        fild [eax].current_time
        fxch
        fsubp st(1),st(0)
        ret

__gettime__clock__ms:
        fld __float__time__0_018
        jmp __gettime__clock__calc_time

__gettime__clock__s:
        fld __float__time__18_2
        jmp __gettime__clock__calc_time

__gettime__clock__min:
        fld __float__time__1092
        jmp __gettime__clock__calc_time

__gettime__clock__h:
        fld __float__time__65520
        jmp __gettime__clock__calc_time

__gettime__clock__calc_time:
        fstp __float__time__temp
        fild [eax].current_time
        mov es,stub32zerosel
        mov ebx,1132
        mov ebx,es:[ebx]
        mov dword ptr [eax].current_time,ebx
        xor ebx,ebx
        mov dword ptr [eax].current_time+4,ebx
        fild [eax].current_time
        fxch
        fsubp st(1),st(0)
        fdiv __float__time__temp
        ret
__gettime__clock__ endp


__gettime__rdtsc__ proc C uses ebx ,ttime:dword
        mov eax,ttime
        mov ebx,[eax].flags
        test ebx,1000b
        jz __gettime__rdtsc__no_clock
        push eax
        call __gettime__clock__
        add esp,4
        ret
__gettime__rdtsc__no_clock:
        and ebx,7
        or ebx,ebx
        jz __gettime__rdtsc__absolute
        cmp ebx,1
        je __gettime__rdtsc__ms
        cmp ebx,2
        je __gettime__rdtsc__s
        cmp ebx,3
        je __gettime__rdtsc__min
        cmp ebx,4
        je __gettime__rdtsc__h
        ffree st(0)
        ret

__gettime__rdtsc__absolute:
        mov ebx,eax
        assume ebx:ptr Time
        fild [ebx].current_time
        rdtsc
        mov dword ptr [ebx].current_time,eax
        mov dword ptr [ebx].current_time+4,edx
        fild [ebx].current_time
        assume ebx:nothing
        fxch
        fsubp st(1),st(0)
        ret

__gettime__rdtsc__ms:
        fld __float__time__0_001
        jmp __gettime__rdtsc__calc_time

__gettime__rdtsc__s:
        fld __float__time__1_0
        jmp __gettime__rdtsc__calc_time

__gettime__rdtsc__min:
        fld __float__time__60_0
        jmp __gettime__rdtsc__calc_time

__gettime__rdtsc__h:
        fld __float__time__3600_0
        jmp __gettime__rdtsc__calc_time

__gettime__rdtsc__calc_time:
        fild __cpu__tsc__freq
        fmul st(0),st(1)
        fstp __float__time__temp
        ffree st(0)
        fincstp
        mov ebx,eax

        assume ebx:ptr Time
        fild [ebx].current_time
        rdtsc
        mov dword ptr [ebx].current_time,eax
        mov dword ptr [ebx].current_time+4,edx
        fild [ebx].current_time
        assume ebx:nothing
        fxch
        fsubp st(1),st(0)
        fdiv __float__time__temp
        ret
__gettime__rdtsc__ endp

assume eax:nothing

.386

;fade8.inc

extern FADE8Tab:dword,FADE8TabShift:dword,FADE8TabSize:dword

;--------------------------------
;InitFADE8Tab (shf:dword,pal:dword)
;       Inicjuje tablice FADE8Tab dla danej palety
;       Wejscie: shf-liczba elementow cieniowania tablicy w postaci 1<<shf
;                pal-paleta kolorow (RGB)
;--------------------------------

InitFADE8Tab proc C uses ebx ,shf:dword,pal:dword
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
        mov ebx,FADE8Tab
        mov color,0        
        mov _64,64
InitFADE8Tab_loop_color:
                mov eax,color
                shl eax,cl
                mov edx,ebx
                add edx,eax
                push edx
                add edx,FADE8TabSize
                dec edx
                mov eax,color
                mov [edx],al
                ;
                pop edx
                add edx,tabsized2
                mov [edx],al
                ;
                ;czesc 1
                mov edx,color
                shl edx,1
                add edx,color
                add edx,pal
                xor eax,eax
                mov al,[edx]
                mov c_r,eax
                mov al,[edx+1]
                mov c_g,eax
                mov al,[edx+2]
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
                                mov al,[edx]
                                mov c_r,eax
                                mov al,[edx+1]
                                mov c_g,eax
                                mov al,[edx+2]
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
                        mov [edx],al
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
                sub al,[edx]
                mov c_r,eax
                mov al,64
                sub al,[edx+1]
                mov c_g,eax
                mov al,64
                sub al,[edx+2]
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
                                mov al,[edx]
                                mov c_r,eax
                                mov al,[edx+1]
                                mov c_g,eax
                                mov al,[edx+2]
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
                        mov [edx],al
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
        ret
InitFADE8Tab endp

;transp8.inc

extern TRANSP8Tab:dword,TRANSP8TabShift:dword,TRANSP8TabSize:dword


;Funkcje

;--------------------------------
;InitTRANSP8Tab (shf:dword,pal:dword)
;       Inicjuje TRANSP8Tab dla danego przesuniecia i palety
;       Wejscie: shf-rozmiar tabliy w postaci 1<<shf
;                pal-paleta kolorow
;       Uwaga: Wymagane jest co najmniej 64k*(1<<shf) pamieci HEAP
;--------------------------------

InitTRANSP8Tab proc C,shf:dword,pal:dword
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
        mov ebx,TRANSP8Tab
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
                mov al,[edx]
                mov c_r1,eax
                mov al,[edx+1]
                mov c_g1,eax
                mov al,[edx+2]
                mov c_b1,eax
                ;
                mov edx,color2
                shl edx,1
                add edx,color2
                add edx,pal
                xor eax,eax
                mov al,[edx]
                mov c_r2,eax
                mov al,[edx+1]
                mov c_g2,eax
                mov al,[edx+2]
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
                                mov al,[edx]
                                mov c_r,eax
                                mov al,[edx+1]
                                mov c_g,eax
                                mov al,[edx+2]
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
                        mov [edx],al
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
        ret
InitTRANSP8Tab endp

;snd.inc

TChannel struct 
 ison dd ?
 sndref dd ?
 pos dd ?
 volume dd ?
 bal dd ?
 sndnum dd ?
 islopped dd ?
TChannel ends

TSnd struct
 bits dd ?
 ssize dd ?
 ;dane dodatkowe na temat dzwieku 
 freq dd ?
 bytesize db ?
 stereo db ?            ;mono/stereo
TSnd ends


SND_BUFSIZE=1024
SND_VOLUMEBITS=8

extern snd_freq:dword,snd_bits:byte,snd_stereo:byte,snd_nchannels:dword,snd_channels:dword,snd_sbportexit:word,snd_buf:dword,snd_bufpage:byte

__Snd_Int_Func proc C
        assume esi:ptr TChannel
        assume ebx:ptr TSnd
        local cntbuf:dword              ;licznik kanalow
        local pbufsnd:dword             ;pozycja bufora dla dzwieku(TSnd)

;Inicjacja
        pushad
        push es
        ;cli
        mov pbufsnd,0
        mov es,stub32zerosel
        mov word ptr es:[0b8000h],1111h
        mov edi,snd_buf
        mov ecx,SND_BUFSIZE/2

        mov cntbuf,ecx
        cmp snd_bufpage,0
        jz loop1_normal_mono_8
        add edi,SND_BUFSIZE/2

;normalny zapis mono 8-bitowy
loop1_normal_mono_8:
                mov esi,snd_channels
                mov ecx,snd_nchannels
                xor edx,edx
loop2_normal_mono_8:
                        cmp [esi].ison,0
                        jz no_channelused_normal_mono_8
                        mov ebx,[esi].sndref
                        mov ebx,[ebx].bits
                        add ebx,[esi].pos
                        add ebx,pbufsnd
                        mov eax,128
                        sub al,byte ptr [ebx]           ;bajt z dzwieku
                        cbw
                        cwde
                        imul eax,[esi].volume
                        sar eax,SND_VOLUMEBITS
                        add edx,eax
no_channelused_normal_mono_8:
                        add esi,sizeof(TChannel)
                        loop loop2_normal_mono_8
                cmp edx,0                               ;jesli byl dzwiek
                jnz is_snd_normal_mono_8
                mov al,128
                jmp addsbyte_normal_mono_8
is_snd_normal_mono_8:
                mov eax,edx
                cmp eax,127
                jng less_127_normal_mono_8
                mov al,127
                jmp addsbyte_normal_mono_8
less_127_normal_mono_8:
                cmp eax,-128
                jnl greater_m128_normal_mono_8
                mov al,-128
                jmp addsbyte_normal_mono_8
greater_m128_normal_mono_8:
                add al,128
addsbyte_normal_mono_8:
                stosb
                inc pbufsnd
                dec cntbuf
                jnz loop1_normal_mono_8

;...(inne tryby)

;aktualizacja tablic snd_channels
        mov ecx,snd_nchannels
        mov esi,snd_channels
upgrade_c_tab:
                cmp [esi].ison,0
                jz ug_d1
                add [esi].pos,SND_BUFSIZE/2
                mov eax,[esi].pos
                add eax,SND_BUFSIZE/2
                mov ebx,[esi].sndref                
                cmp eax,[ebx].ssize
                jbe ug_d1
                mov [esi].ison,0
ug_d1:
                add esi,sizeof(TChannel)
                loop upgrade_c_tab

        ;procedury koncowe
        ;snd_bufpage=1-snd_bufpage
        xor snd_bufpage,1
        and snd_bufpage,1
        ;dla 8-bit:2xe, dla 16-bit:2xf
        sti
        mov dx,snd_sbportexit
        in al,dx
        mov al,20h
        out 20h,al
        pop es
        popad
        iretd
        assume esi:nothing
        assume ebx:nothing
__Snd_Int_Func endp
end
