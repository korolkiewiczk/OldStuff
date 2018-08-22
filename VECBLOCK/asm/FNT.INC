;FNT.INC for DJGPP by Kamil Korolkiewicz 2006
;Rysowanie ciagow ASCIIZ dla czionek

.code

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

