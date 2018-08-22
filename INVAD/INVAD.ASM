;INVADERS
;Klasyczna gra kosmiczna

;uwagi:
;funkcje moga zmieniac tylko zawartosc rejestrow ax,dx,di,si,es

code segment
assume cs:code,ds:code
org 100h

;stale

SCRWIDTH        equ 320
SCRHEIGHT       equ 200

MAXSTARS        equ 400
SHIFTPOS        equ 6
SHIFTDELTA      equ 3

SHOTLIFE        equ 55

MAXINVAD        equ 4
INVADPOWER      equ 1
INVADDEFY       equ 5
DELTAUPDATE     equ 12

PLAYERPOWER     equ 8

;pola bitowe dla standardowego sterowania

CMD_UP          equ 1
CMD_DOWN        equ 2
CMD_LEFT        equ 4
CMD_RIGHT       equ 8
CMD_SHOT        equ 16

;struktury

;informacje o pocisku wystrzeliwanym przez statek
TShot struc

        x dw ?          ;pozycja x pocisku +0
        y dw ?          ;pozycja y pocisku +2
        life dw ?       ;zycie pocisku     +4
        color dw ?      ;kolor             +6
        direc dw ?      ;kierunek strzalu  +8

TShot ends


;przechowuje informacje o statku kosmicznym
TSpaceShip struc

        x dw ?          ;pozycja x  +0
        y dw ?          ;pozycja y  +2
        power dw ?      ;energia    +4
        stype dw ?      ;typ statku +6
        swidth dw ?     ;szerokosc  +8
        sheight dw ?    ;dlugosc    +10
        boundx dw ?     ;min/max x  +12
        boundy dw ?     ;min/max y  +14
        shot dw ?       ;adres do TShot +16
        counter dw ?    ;licznik do nastepnej zmiany +18
        dir dw ?        ;kierunek przesuwania sie +20

TSpaceShip ends

;przechowuje informacje o gwiezdzie
TStar struc

        x dw ?          ;pozycja x (>>8) +0
        y dw ?          ;pozycja y (>>8) +2
        dist dw ?       ;odleglosc/natezenie [0-15] 15-najblizej +4

TStar ends


;makra

;zwraca w di adres do komorki pamieci ekranu dla wsp. (x,y)
;wykozystuje cx i dx (nie zapamietuje zmian)
GetScrAddr macro x,y

        push cx
        push dx
        mov dx,y
        mov di,dx
        mov cl,8
        shl dx,cl
        mov cl,6
        shl di,cl
        add di,dx
        add di,x
        pop dx
        pop cx

endm

;kod

start:

;Funkcja glowna programu
main:

        ;inicjuj freq
        call Delay

        ;inicjuj seed
        call GetTick
        mov seed,ax

        ;inicjuj klawiature
        xor ax,ax
        mov es,ax
        mov di,9h*4
        mov dx,es:[di]
        mov word ptr [prev_kb_vec],dx
        mov dx,es:[di+2]
        mov word ptr [prev_kb_vec+2],dx

        mov dx,offset kb_interrupt
        mov es:[di],dx
        mov dx,ds
        mov es:[di+2],dx

        ;zeruj bufor klawiatury
        mov ax,ds
        mov es,ax
        mov di,offset kb_on
        mov cx,128/2
        xor ax,ax
        rep stosw

        ;inicjuj tryb 13h
        mov ax,13h
        int 10h

        ;generuj gwiazdy na poczatku
        mov cx,MAXSTARS
        mov si,offset Stars
        mov di,SCRHEIGHT
gen_stars_loop:

                call GenStar
                add si,size TStar
                loop gen_stars_loop


        ;inicjuj przeciwnikow
        mov cx,MAXINVAD
        xor di,di
gen_invad_loop:

                mov dx,di
                mov si,offset InvadShip
                call GenInvad
                inc di
                loop gen_invad_loop

;glowna petla programu
main_loop:

        ;dokonaj animacji tylko w okreslonym czasie
        mov ax,3
        call Delay

comment /
        mov dx,3dah
wfvr1:

                in al,dx
                test al,00001000b
                jnz wfvr1
wfvr2:
                in al,dx
                test al,00001000b
                jz wfvr2/

        mov si,offset Stars
        mov cx,MAXSTARS
        call UpdateStars

        ;ustal pola bitowe sterowania na podst. klawiatury
        mov al,kb_on[72] ;UP

        mov dl,kb_on[80] ;DOWN
        shl dl,1
        or al,dl

        mov dl,kb_on[75] ;LEFT
        shl dl,1
        shl dl,1
        or al,dl

        mov dl,kb_on[77] ;RIGHT
        shl dl,1
        shl dl,1
        shl dl,1
        or al,dl

        mov dl,kb_on[57] ;SPACE
        ;mov dl,kb_on[2] ;SPACE
        shl dl,1
        shl dl,1
        shl dl,1
        shl dl,1
        or al,dl
        xor ah,ah

        mov si,offset PlayerShip
        mov bx,ax
        call ControlShip

        mov si,offset PlayerShip
        call DrawShip

        ;przetworz przeciwnikow
        mov si,offset InvadShip
        mov cx,MAXINVAD
        call UpdateInvad


        ;rysuj pasek stanu gracza (power) na dole ekanu
        mov ax,0a000h
        mov es,ax
        mov ax,word ptr [PlayerShip+4] ;power
        cmp ax,0
        jle quit
        mov di,SCRWIDTH*(SCRHEIGHT-1)
        mov cl,4
        shl ax,cl
        xor dx,dx
        mov cx,PLAYERPOWER
        div cx
        mov cx,SCRWIDTH
        mul cx
        mov cl,4
        shr ax,cl
        mov cx,ax
        shr cx,1
        mov bx,SCRWIDTH/2
        sub bx,cx
        mov ax,0404h
        rep stosw
        xor ax,ax
        mov cx,bx
        rep stosw

        ;zwieksz ilosc pkt
        inc points

no_animation:
        ;sprawdz czy esc (wyjscie z programu)
        mov al,kb_on[1]
        cmp al,1
        jz quit
        jmp main_loop

quit:

        ;wroc do trybu tekstowego
        mov ax,3h
        int 10h

        ;przywroc klawiature
        xor ax,ax
        mov es,ax
        mov di,9h*4
        mov dx,word ptr [prev_kb_vec]
        mov es:[di],dx
        mov dx,word ptr [prev_kb_vec+2]
        mov es:[di+2],dx

        ;wyjdz do DOS

        mov ax,points
        mov bx,5
        mov cx,10
gen_points:
        xor dx,dx
        div cx
        add dx,'0'
        mov byte ptr [abouttext+bx-1],dl
        dec bx
        jnz gen_points


        mov dx,offset abouttext
        mov ax,0900h
        int 21h

        mov ax,4c00h
        int 21h
;end main

;funkcja przerwania klawiatury
kb_interrupt:
        push ax
        push bx

        in al,60h
        xor bx,bx
        mov bl,al
        shr al,7
        xor al,1
        and bl,127
        mov [kb_on+bx],al

        mov al,20h
        out 20h,al
        pop bx
        pop ax
        iret
;end kb_interrupt

;rysuje statek
;si-adres do struktury TSpaceShip
DrawShip:

        push si
        mov bp,sp
        sub sp,2
        ;rysuj statek
        mov ax,0a000h
        mov es,ax

        mov [bp-2],si ;zapamietaj si
        cmp word ptr [si+2],0 ;y
        jl DrawShip_nodraw

        GetScrAddr [si],[si+2] ;x,y

        mov dx,[si+8]   ;swidth
        mov ax,dx
        mov cx,[si+10]  ;sheight

        mov si,[si+6]   ;stype

DrawShip_loop:

                mov cx,dx
                rep movsb
                add di,SCRWIDTH
                sub di,dx
                dec ax
                jnz DrawShip_loop


DrawShip_nodraw:
        ;rysuj pocisk statku
        mov si,[bp-2] ;zachowane si
        mov si,[si+16] ;shot
        cmp word ptr [si+4],0 ;life
        jz DrawShip_noshot
        dec word ptr [si+4] ;life

        GetScrAddr [si],[si+2] ;x,y
        xor al,al
        mov es:[di],al
        mov es:[di-SCRWIDTH],al
        mov es:[di-2*SCRWIDTH],al

        cmp word ptr [si+4],0 ;life
        jz DrawShip_noshot

        ;przesuwamy pocisk w dol/gore
        mov ax,[si+8] ;dir
        add [si+2],ax ;y

        cmp ax,0
        jg DrawShip_shotdown

        cmp word ptr [si+2],2 ;y
        jl DrawShip_noshot
        jmp DrawShip_aftershotcpos

DrawShip_shotdown:
        cmp word ptr [si+2],SCRHEIGHT-1 ;y
        jg DrawShip_noshot

DrawShip_aftershotcpos:

        GetScrAddr [si],[si+2] ;x,y
        mov ax,[si+6] ;color
        ;mov al,1
        mov es:[di],al
        mov es:[di-SCRWIDTH],al
        mov es:[di-2*SCRWIDTH],al

DrawShip_noshot:

        mov sp,bp
        pop si
        ret

;end DrawShip

;rysuje czarny statek (zamazuje poprzedni obszar)
;w si adres do staktu (nie zmienia)
DrawShipNull:

        push si
        mov bp,sp
        sub sp,2
        ;zeruj statek
        mov ax,0a000h
        mov es,ax

        GetScrAddr [si],[si+2] ;x,y

        mov dx,[si+8]   ;swidth
        mov [bp-2],dx     ;@w
        mov cx,[si+10]  ;sheight

        mov dx,[bp-2]     ;@w
        xor ax,ax
DrawShipNull_loop:

                mov cx,dx
                rep stosb
                add di,SCRWIDTH
                sub di,dx
                dec word ptr [bp-2]       ;@w
                jnz DrawShipNull_loop


        ;zeruj pocisk statku
        mov si,[si+16] ;shot
        cmp word ptr [si+4],0 ;life
        jz DrawShipNull_noshot

        GetScrAddr [si],[si+2] ;x,y
        xor al,al
        mov es:[di],al
        mov es:[di-SCRWIDTH],al
        mov es:[di-2*SCRWIDTH],al
DrawShipNull_noshot:

        mov sp,bp
        pop si

        ret

;end DrawShipNull


;kontroluje zachowanie statku w zaleznosci od klawisza
;w si adres do TSpaceShip
;w bx pole bitowe definiujace podstawowe klawisze
ControlShip:

        push si
        test bx,CMD_UP
        jz ControlShip_noup

        mov dx,[si+2]  ;y
        cmp dx,[si+14] ;boundy
        jb ControlShip_noup

        call DrawShipNull
        dec word ptr [si+2] ;y


ControlShip_noup:

        test bx,CMD_DOWN
        jz ControlShip_nodown

        mov dx,SCRHEIGHT
        sub dx,[si+14] ;boundy
        cmp dx,[si+2]  ;y
        jl ControlShip_nodown

        call DrawShipNull
        inc word ptr [si+2] ;y

ControlShip_nodown:

        test bx,CMD_LEFT
        jz ControlShip_noleft

        mov dx,[si]  ;x
        cmp dx,[si+12] ;boundx
        jl ControlShip_noleft

        call DrawShipNull
        dec word ptr [si] ;x

ControlShip_noleft:

        test bx,CMD_RIGHT
        jz ControlShip_noright

        mov dx,SCRWIDTH
        sub dx,[si+12] ;boundx
        cmp dx,[si] ;x
        jl ControlShip_noright

        call DrawShipNull
        inc word ptr [si] ;x

ControlShip_noright:

        test bx,CMD_SHOT
        jz ControlShip_noshot

        call DrawShipNull
        push ds
        pop es
        mov di,[si+16] ;shot
        cmp word ptr [di+4],0 ;life
        ja ControlShip_noshot

        movsw ;x->x
        movsw ;y->y
        mov ax,[si+8-4] ;swidth
        shr ax,1
        add word ptr [di-4],2
        mov word ptr [di],SHOTLIFE ;life

ControlShip_noshot:

        pop si

        ret
;end ControlShip

;generuje nowego przeciwnika
;w si adres do pirwszego elem. struktury TSpaceShip
;w dx numer przeciwnika
GenInvad:

        push bx
        push cx
        push dx
        mov ax,size TSpaceShip
        mul dx
        mov bx,ax

        ;generuj x
        call Rand
        mov dx,[bx+si+12] ;boundx
        mov [bx+si],dx    ;x
        mov cx,SCRWIDTH
        sub cx,dx
        xor dx,dx
        div cx
        add [bx+si],dx    ;x+=rand()%(width-boundx)

        ;generuj y
        call Rand
        mov cx,INVADDEFY
        xor dx,dx
        div cx
        neg dx
        mov [bx+si+2],dx  ;y=-rand()%INVADDEFY

        mov word ptr [bx+si+4],INVADPOWER ;power

        pop dx
        mov ax,size TShot
        mul dx
        add ax,offset InvadShot

        mov [bx+si+16],ax
        mov bx,ax
        call Rand
        mov cx,SHOTLIFE
        xor dx,dx
        div cx
        mov [bx+4],dx ;life

        pop cx
        pop bx
        ret
;end GenInvad

;wyswietla przeciwnikow
;w si adres do TSpaceShip
;w cx ilosc przeciwnikow
UpdateInvad:
        push si
UpdateInvad_loop:
        push cx

        ;rysuj pusty obrazek
        call DrawShipNull

        ;obsluga przeciwnika

        cmp word ptr [si+18],0 ;counter
        jnz UpdateInvad_nochangedir
        mov word ptr[si+18],DELTAUPDATE ;counter

        ;wybierz akcje
        call Rand
        and ax,7
        mov [si+20],ax ;direc

UpdateInvad_nochangedir:

        xor dx,dx ;akcja
        mov ax,[si+20] ;direc

        cmp ax,0
        jne UpdateInvad_j1
        ;gora
        or dx,CMD_UP
        jmp UpdateInvad_endcmd

UpdateInvad_j1:
        cmp ax,1
        jne UpdateInvad_j2
        ;gora-prawo
        or dx,CMD_UP or CMD_RIGHT
        jmp UpdateInvad_endcmd

UpdateInvad_j2:
        cmp ax,2
        jne UpdateInvad_j3
        ;prawo
        or dx,CMD_RIGHT
        jmp UpdateInvad_endcmd

UpdateInvad_j3:
        cmp ax,3
        jne UpdateInvad_j4
        ;dol-prawo
        or dx,CMD_DOWN or CMD_RIGHT
        jmp UpdateInvad_endcmd

UpdateInvad_j4:

        cmp ax,4
        jne UpdateInvad_j5
        ;dol
        or dx,CMD_DOWN
        jmp UpdateInvad_endcmd

UpdateInvad_j5:
        cmp ax,5
        jne UpdateInvad_j6
        ;dol-lewo
        or dx,CMD_DOWN or CMD_LEFT
        jmp UpdateInvad_endcmd

UpdateInvad_j6:
        cmp ax,6
        jne UpdateInvad_j7
        ;lewo
        or dx,CMD_LEFT
        jmp UpdateInvad_endcmd

UpdateInvad_j7:
        cmp ax,7
        jne UpdateInvad_j8
        ;gora-lewo
        or dx,CMD_UP or CMD_LEFT
        jmp UpdateInvad_endcmd

UpdateInvad_j8:

UpdateInvad_endcmd:

        or dx,CMD_SHOT
        push bx
        mov bx,dx
        call ControlShip
        pop bx

        dec word ptr [si+18] ;counter

        call DrawShip

        ;sprawdz kolizje z pociskiem gracza
        mov ax,word ptr [PlayerShot] ;x
        mov dx,[si] ;x
        mov di,[si+8] ;swidth
        sub ax,dx
        cmp ax,0
        jl UpdateInvad_nodestroy
        cmp ax,di
        jae UpdateInvad_nodestroy

        mov ax,word ptr [PlayerShot+2] ;y
        mov dx,[si+2] ;y
        mov di,[si+10] ;sheight
        sub ax,dx
        cmp ax,0
        jl UpdateInvad_nodestroy
        cmp ax,di
        jae UpdateInvad_nodestroy

        ;zniszcz przeciwnika i tworz nowego
        pop cx
        mov dx,MAXINVAD
        sub dx,cx
        pop si
        call GenInvad
        add points,20
        push si
        push cx

UpdateInvad_nodestroy:

        ;sprawdz kolizje pocisku pzeciwnika z graczem
        push bx
        mov bx,[si+16] ;shot
        mov ax,[bx] ;x
        mov dx,word ptr [PlayerShip] ;x
        sub ax,dx
        cmp ax,0
        jl UpdateInvad_nodestroypl
        cmp ax,word ptr [PlayerShip+8] ;swidth
        jae UpdateInvad_nodestroypl

        mov ax,[bx+2] ;y
        mov dx,word ptr [PlayerShip+2] ;y
        sub ax,dx
        cmp ax,0
        jl UpdateInvad_nodestroypl
        cmp ax,word ptr [PlayerShip+10] ;sheight
        jae UpdateInvad_nodestroypl

        mov word ptr [bx+4],0 ;life
        dec word ptr [PlayerShip+4] ;power

UpdateInvad_nodestroypl:

        pop bx

        add si,size TSpaceShip

        pop cx
        dec cx
        jz UpdateInvad_endf
        jmp UpdateInvad_loop

UpdateInvad_endf:

        pop si
        ret

;generuje gwiazde
;w si adres do struktury TStar
;w di wysokosc ekranu
GenStar:

        push ax
        push dx
        push cx

        call Rand
        mov cx,SCRWIDTH shl SHIFTPOS
        xor dx,dx
        div cx
        mov [si+0],dx ;x

        call Rand
        mov dx,di
        mov cl,SHIFTPOS
        shl dx,cl
        mov cx,dx
        xor dx,dx
        div cx
        mov [si+2],dx ;y

        call Rand
        and ax,15
        mov [si+4],ax ;dist

        pop cx
        pop dx
        pop ax

        ret

;end GenStar

;rysuje i przesuwa gwiazdy
;w si adres do pierwszego elementu struktury TStars
;w cx ilosc elementow
UpdateStars:

        push bx
        mov ax,0a000h
        mov es,ax

        ;wyczysc
        push cx
        push si
        mov dx,cx
UpdateStars_loop1:

                mov ax,[si]     ;x
                mov cl,SHIFTPOS
                shr ax,cl
                mov bx,[si+2]   ;y
                shr bx,cl
                GetScrAddr ax,bx
                mov byte ptr es:[di],0

                add si,size TStar
                dec dx
                jnz UpdateStars_loop1

        pop si
        pop cx

        ;zmien pozycje
        mov dx,cx

        push cx
        push si
UpdateStars_loop2:

                mov ax,[si+4] ;dist
                inc ax
                mov cl,SHIFTDELTA
                shl ax,cl
                add [si+2],ax ;y

                mov ax,[si+2] ;y
                mov cl,SHIFTPOS
                shr ax,cl
                cmp ax,SCRHEIGHT
                jb UpdateStars_nonewstar
                ;generuj nowa gwiazde
                mov di,5
                call GenStar

UpdateStars_nonewstar:

                add si,size TStar
                dec dx
                jnz UpdateStars_loop2

        pop si
        pop cx

        ;rysuje gwiazdy
        mov dx,cx
UpdateStars_loop3:

                mov ax,[si]     ;x
                mov cl,SHIFTPOS
                shr ax,cl
                mov bx,[si+2]   ;y
                shr bx,cl
                GetScrAddr ax,bx
                mov ax,[si+4]    ;dist
                add al,16
                mov es:[di],al

                add si,size TStar
                dec dx
                jnz UpdateStars_loop3

        pop bx
        ret
;end UpdateStars


;zwraca w ax czas systemowy
GetTick:

        push di
        mov di,06ch
        mov ax,40h
        mov es,ax
        mov ax,es:[di]
        pop di

        ret

;end GetTick

;czeka ax ms
Delay:

        cmp word ptr [freq],0
        jne Delay_nosetfreq
        cmp word ptr [freq+2],0
        jne Delay_nosetfreq

        ;inicjujemy freq gdy rowny jest 0
        call GetTick
        mov dx,ax
        mov bp,sp
        sub sp,4
Delay_loop1:

                call GetTick
                cmp dx,ax
                jz Delay_loop1

        call GetTick
        mov dx,ax
        ;freq=0
Delay_loop2:

                call GetTick
                add word ptr [freq],1
                adc word ptr [freq+2],0

                mov cx,word ptr [freq+2]
                cmp [bp-2],cx
                mov cx,word ptr [freq]
                cmp [bp-4],cx

                sub ax,4
                cmp dx,ax
                ja Delay_loop2

        mov sp,bp
        ret

Delay_nosetfreq:

        ;tutaj czekamy
        mov bp,sp
        sub sp,4
        xor cx,cx
        mov word ptr [bp-2],cx
        mov word ptr [bp-4],cx

Delay_loop3:

                add word ptr [bp-4],ax
                adc word ptr [bp-2],0

                mov dx,word ptr [freq+2]
                cmp [bp-2],dx
                jb Delay_loop3
                mov dx,word ptr [freq]
                cmp [bp-4],dx
                jb Delay_loop3


        mov sp,bp
        ret

;end Delay

;zwraca wartosc losowa 0..65535
Rand:

        push dx
        push cx
        mov ax,seed
        mov dx,11
        mul dx
        add ax,5
        xor dx,dx
        mov cx,65447
        div cx
        mov seed,dx
        mov ax,dx
        pop cx
        pop dx
        ret
;end Rand

;dane stale

;obrazki statkow 5x5
shippic0 label
db 0,0,7,0,0
db 0,0,7,0,0
db 0,7,1,7,0
db 0,7,7,7,0
db 7,7,4,7,7

;przeciwnik
shippic1 label
db 7,0,4,0,7
db 0,7,7,7,0
db 0,7,1,7,0
db 0,7,7,7,0
db 0,0,7,0,0


;statek gracza
PlayerShot TShot <0,0,0,1,-4>
PlayerShip TSpaceShip <160,170,PLAYERPOWER,shippic0,5,5,10,10,PlayerShot>

;przeciwnicy
InvadShot TShot MAXINVAD dup(<0,0,0,4,4>)
InvadShip TSpaceShip MAXINVAD dup(<100,0,0,shippic1,5,5,10,10,0>)

;stala dla Delay
freq dd 0

abouttext db '      points',13,10,'INVAD BY KAMIL KOROLKIEWICZ 2009. WRITTEN IN ASM.$'

points dw 0

;dane przydzielane

;stala dla Rand
seed dw ?

;wektor klawiatury
prev_kb_vec dd ? ;poprzedni adres wektora przerwania klawiatury
kb_on db 128 dup (?) ;wektor klawiatury

;gwiazdy
Stars TStar MAXSTARS dup(?)


code ends

end start