.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;dane

;kod

include dpmi.inc
include mstdlib\mcon.inc
include mstdlib\mmem.inc
include mstdlib\fileio.inc
include mstdlib\strings.inc
include mstdlib\sprintf.inc
include mstdlib\params.inc
include kb.inc
include vgatext.inc
include vga.inc
include vesa.inc
include surface.inc

scrsx equ 1024
scrsy equ 768
modev equ V1024x768x256
coll equ 15

nazwap db 128 dup (0)
sygn dd 0
wln dd 0
wpt dd 0
owpt dd 0
buft db 128 dup (0)
buff TSurface <scrsx,scrsy,1,scrsx,0>
surf TSurface <scrsx,scrsy,1,scrsx,0a0000h>
pz dd 0
pztxt db 'Pozycja w pliku:%i',0
ftxt db 'f21:%i,f22:%i,f23:%i,f31:%i,f32:%i,f33:%i,fa:%i,fb:%i,dela:%i,delb:%i',0
lnn dd 0
dll dd 0
tb db 3ah dup (0)
wf db 'a.wav',0
pom dd 0

CPY proc stdcall,__f:dword,__t:dword
        mov esi,__f
        mov edi,__t
        mov ecx,wln
        push ds
        mov es,stub32zerosel
        push es
        pop ds
        rep movsw
        pop ds
        ret
CPY endp                

LoadWave proc stdcall,np:dword
        local h:word
        invoke FOpen,np,0,0
        jc procquit
        mov h,ax
        invoke FRead,h,3ah,addr tb

        cmp dword ptr tb+8,'EVAW'
        jne procquitc

        mov eax,dword ptr tb+36h
        mov wln,eax

        invoke AllocMem,wln
        mov wpt,eax

        invoke AllocMem,wln
        mov owpt,eax

        invoke EFRead,h,wln,wpt

        invoke FClose,h
        mov eax,1
        shr wln,1
        ret
procquitc:
        invoke FClose,h
procquit:
        xor eax,eax
        ret
LoadWave endp

SaveWave proc stdcall,np:dword
        local h:word
        invoke FOpen,np,2,1
        jc procquit
        mov h,ax
        invoke FWrite,h,3ah,addr tb

        mov eax,dword ptr tb+36h
        mov wln,eax

        invoke EFWrite,h,wln,wpt

        invoke FClose,h
        mov eax,1
        shr wln,1
        ret
procquitc:
        invoke FClose,h
procquit:
        xor eax,eax
        ret
SaveWave endp

f1 dd 0.0               ;fala
f2 dd 0.0               ;czestotliwo˜† (wy)
f21 dd 90.0             ;czestotliwo˜† (sin)
f22 dd 0.0              ;czestotliwo˜† (wspolczynnik)
f23 dd 10.0             ;czestotliwo˜† (wzmocnienie)na start
pi dd 3.14
_180 dd 180.0
f3 dd 0.0               ;wzmocnienie (wy)
f31 dd 90.0             ;wzmocnienie (sin)
f32 dd 0.0              ;wzmocnienie (wspolczynnik)
f33 dd 0.5              ;wzmocnienie (wzmocnienie)na start
pf2 dd 0.0
pf3 dd 0.0
filtra dw 10000
filtrb dw -10000
dela dd 20000
delb dd -20000
_0001 dd 0.001
_001 dd 0.01
_01 dd 0.1
_1 dd 1.0
_10 dd 10.0
_90 dd 90.0
_100 dd 100.0
_1000 dd 1000.0
__0001 dd 0.001
__001 dd 0.01
__01 dd 0.1
__1 dd 1.0
__10 dd 10.0
__90 dd 90.0
__100 dd 100.0
__1000 dd 1000.0

PW proc
        invoke CPY,owpt,wpt
        finit
        mov edi,wpt
        mov es,stub32zerosel
        mov ecx,wln
PWL:

        fld f21
        fdiv _180
        fmul pi
        fsin
        fmul f23
        fstp f2
        fld f22
        fadd f21
        fstp f21

        fld f31
        fdiv _180
        fmul pi
        fsin
        fmul f33
        fstp f3
        fld f32
        fadd f31
        fstp f31
        
        fld f1
        fadd f2
        fstp f1

        fld f1
        fdiv _180
        fmul pi
        fsin
        fmul f3
        comment ;
        fistp word ptr es:[edi]


        inc edi
        inc edi

        dec ecx
        jnz PWL;
;acomment ;
        mov ax,word ptr es:[edi]
        cmp ax,filtra
        jg PWDd
        cmp ax,filtrb
        jg PWD3
PWDd:
        cwde
        mov ebx,eax
        fild word ptr es:[edi]
        fmul
        fistp word ptr es:[edi]
        mov ax,word ptr es:[edi]
        cwde
        add eax,ebx
        cmp eax,dela
        jl PWD1
        mov eax,dela
PWD1:
        cmp eax,delb
        jg PWD2
        mov eax,delb
PWD2:
        mov es:[edi],ax
PWD3:
        ffree st(0)
        fincstp

        inc edi
        inc edi

        dec ecx
        jnz PWL;

        fld _90
        fstp f21

        fld _90
        fstp f31
        ;mov ebx,360
        ;xor edx,edx
        ;mov eax,pom
        ;div ebx
        ;mov pom,edx
        ;fild pom
        ;fstp f31
PWQ:
        ret
PW endp

__200 dd 200.0
__400 dd 400.0

dline proc
        fistp pom
        mov ebx,100*4
        mov eax,pom
        xor edx,edx
        div ebx
        mov eax,edx
        ;sar edx,5
        ;shl edx,5
        and edx,eax
        mov pom,edx
        cmp pom,100
        jb dlinepq
        cmp pom,300
        jb dlined1
        fild pom
        fsub __400
        ret
dlined1:
        fld __200
        fisub pom
        ret
dlinepq:
        fild pom
        ret
dline endp

PWDist proc
        invoke CPY,owpt,wpt
        finit
        mov edi,wpt
        mov es,stub32zerosel
        mov ecx,wln
        fld f22
        fstp pf2
        fld f32
        fstp pf3
PWDistL:
comment;
        fld f1
        fistp pom
        cmp pom,100/2
        jle PWDistdd1
        fld f22
        fchs
        fstp f22
PWDistdd1:
        cmp pom,-100/2
        jge PWDistdd2
        fld f22
        fchs
        fstp f22
PWDistdd2:

        fld f31
        fistp pom
        cmp pom,100
        jle PWDistdd3
        fld f32
        fchs
        fstp f32
PWDistdd3:
        cmp pom,-100
        jge PWDistdd4
        fld f32
        fchs
        fstp f32
PWDistdd4:;

        fld f21
        call dline
        fdiv __100
        fmul f23
        fstp f2
        fld f22
        fadd f21
        fstp f21

        fld f31
        call dline
        fdiv __100
        fmul f33
        fstp f3
        fild word ptr es:[edi]
        fdiv _1000
        fdiv _100
        fmul f3
        fstp f3
        fld f32
        fadd f31
        fstp f31
        
        fld f1
        fadd f2
        fstp f1

        fld f1
        call dline
        fdiv _100
        fmul f3
        comment;
        fistp word ptr es:[edi]

        inc edi
        inc edi

        dec ecx
        jnz PWDistL;
;acomment ;
        mov ax,word ptr es:[edi]
        cmp ax,filtra
        jg PWDistDd
        cmp ax,filtrb
        jg PWDistD3
PWDistDd:
        cwde
        mov ebx,eax
        fild word ptr es:[edi]
        fmul
        fistp word ptr es:[edi]
        mov ax,word ptr es:[edi]
        cwde
        add eax,ebx
        cmp eax,dela
        jl PWDistD1
        mov eax,dela
PWDistD1:
        cmp eax,delb
        jg PWDistD2
        mov eax,delb
PWDistD2:
        mov es:[edi],ax
PWDistD3:
        ffree st(0)
        fincstp

        inc edi
        inc edi

        dec ecx
        jnz PWDistL;

        fld pf2
        fstp f22
        fld pf3
        fstp f32


        fld _90
        fstp f21

        fld _90
        fstp f31

        fldz
        fstp f1

        fldz
        fstp f2

        fldz
        fstp f3

PWDistQ:
        ret
PWDist endp

Line proc stdcall uses eax ebx ecx edx edi,lsx:dword,lsy:dword,lky:dword,col:dword,srf:dword
        cmp lsy,scrsy
        jb llld11
        mov lsy,0
llld11:
        cmp lky,scrsy
        jb llld12
        mov lky,199
llld12:
        mov ecx,lsy
        cmp ecx,lky
        jb llld1
        xchg ecx,lky
        mov lsy,ecx
llld1:
        mov es,stub32zerosel
        mov ebx,srf
        mov edi,[ebx+13]
        mov ebx,[ebx]
        mov eax,lsy
        mov ecx,lky
        sub ecx,eax
        inc ecx
        mul ebx
        add eax,lsx
        add edi,eax
        mov eax,col
lll:
        mov es:[edi],al
        add edi,ebx
        loop lll
        ret
Line endp

main:
        
        invoke InitHeapMem,4*1024*1024
        call GetParamCount
        or al,al
        jnz d1
        jmp d11
        nopartxt db 'WLINE by Kamil Korolkiewicz 2005. Jako parametr podaj nazwe pliku wav.',13,10,0
d11:
        call Cls
        invoke Print,0,0,addr nopartxt
        jmp programquit1
d1:
        lea esi,nazwap
        mov eax,1
        call GetParam

        invoke LoadWave,addr nazwap
        or eax,eax
        jz programquit1

        invoke CPY,wpt,owpt

        call InitKB

        SetVesaMode modev,0100000000000000b
        mov cx,modev
        call GetLFB
        mov surf.SBits,eax

        invoke AllocMem,scrsx*scrsy
        mov buff.SBits,eax

mainloop:
        mov eax,0
        mov esi,offset buff
        call FillSurface
        IsKeyPressed KEY_ESC
        jnz programquit
        IsKeyPressed KEY_LEFT
        jz nkp1
        add pz,8
nkp1:

        IsKeyPressed KEY_RIGHT
        jz nkp2
        sub pz,8
nkp2:

        IsKeyPressed KEY_UP
        jz nkp3
        add pz,256
nkp3:

        IsKeyPressed KEY_DOWN
        jz nkp4
        sub pz,256
nkp4:

        IsKeyPressed KEY_PAGEUP
        jz nkp5
        add pz,735
nkp5:

        IsKeyPressed KEY_PAGEDOWN
        jz nkp6
        sub pz,735
nkp6:

        IsKeyPressed KEY_ENTER
        jz nkp7
        call PW
        mov byte ptr __KBKeyTab[KEY_ENTER],0
nkp7:

        IsKeyPressed KEY_DELETE
        jz nkp8
        invoke CPY,owpt,wpt
        mov byte ptr __KBKeyTab[KEY_DELETE],0
nkp8:

        IsKeyPressed KEY_F1
        jz nkp9
        fld f22
        fadd _0001
        fstp f22
nkp9:

        IsKeyPressed KEY_F1+1
        jz nkp10
        fld f22
        fsub _0001
        fstp f22
nkp10:

        IsKeyPressed KEY_F1+2
        jz nkp11
        fld f23
        fadd _001
        fstp f23
nkp11:

        IsKeyPressed KEY_F1+3
        jz nkp12
        fld f23
        fsub _001
        fstp f23
nkp12:

        IsKeyPressed KEY_F1+4
        jz nkp13
        fld f32
        fadd _0001
        fstp f32
nkp13:

        IsKeyPressed KEY_F1+5
        jz nkp14
        fld f32
        fsub _0001
        fstp f32
nkp14:

        IsKeyPressed KEY_F1+6
        jz nkp15
        fld f33
        fadd _001
        fstp f33
nkp15:

        IsKeyPressed KEY_F1+7
        jz nkp16
        fld f33
        fsub _001
        fstp f33
nkp16:

        IsKeyPressed KEY_CTRL
        jz nkp17

        fld __0001
        fmul __10
        fstp _0001

        fld __001
        fmul __10
        fstp _001

        fld __01
        fmul __10
        fstp _01

        fld __1
        fmul __10
        fstp _1

        fld __10
        fmul __10
        fstp _10

        fld __100
        fmul __10
        fstp _100

        fld __1000
        fmul __10
        fstp _1000
        jmp kp17

nkp17:

        fld __0001
        fstp _0001

        fld __001
        fstp _001

        fld __01
        fstp _01

        fld __1
        fstp _1

        fld __10
        fstp _10

        fld __100
        fstp _100

        fld __1000
        fstp _1000

kp17:

        IsKeyPressed KEY_SPACE
        jz nkp18
        call PWDist
        mov byte ptr __KBKeyTab[KEY_SPACE],0
nkp18:

        IsKeyPressed KEY_F1+8
        jz nkp19
        add filtra,25
nkp19:

        IsKeyPressed KEY_F1+9
        jz nkp20
        sub filtra,25
nkp20:

        IsKeyPressed 2
        jz nkp21
        add filtrb,25
nkp21:

        IsKeyPressed 3
        jz nkp22
        sub filtrb,25
nkp22:

        IsKeyPressed KEY_HOME
        jz nkp23
        add dela,25
nkp23:

        IsKeyPressed KEY_END
        jz nkp24
        sub dela,25
nkp24:

        IsKeyPressed 4
        jz nkp25
        add delb,25
nkp25:

        IsKeyPressed 5
        jz nkp26
        sub delb,25
nkp26:

        cmp pz,0
        jnl pz1
        mov pz,0
        jmp pz2
pz1:
        mov eax,wln
        cmp pz,eax
        jng pz2
        mov pz,eax
pz2:

        xor eax,eax
        xor ebx,ebx
        mov ecx,22
        mov edx,EFECTS_FADE
        mov esi,offset nazwap
        mov edi,offset buff
        call DrawString8

        invoke SPrintf,addr buft,addr pztxt,pz
        xor eax,eax
        mov ebx,10
        mov ecx,22
        mov edx,EFECTS_FADE
        mov esi,offset buft
        mov edi,offset buff
        call DrawString8

        push delb
        push dela
        movzx eax,filtrb
        cwde
        push eax
        movzx eax,filtra
        cwde
        push eax

        fld f33
        fmul _100
        fistp pom
        push pom

        fld f32
        fmul _1000
        fistp pom
        push pom

        fld f31
        fistp pom
        push pom

        fld f23
        fmul _100
        fistp pom
        push pom

        fld f22
        fmul _1000
        fistp pom
        push pom

        fld f21
        fistp pom
        push pom

        push offset ftxt
        push offset buft
        call SPrintf
        add esp,48

        xor eax,eax
        mov ebx,20
        mov ecx,22
        mov edx,EFECTS_FADE
        mov esi,offset buft
        mov edi,offset buff
        call DrawString8

        mov es,stub32zerosel

        mov ebx,65536/(scrsy/5*4)
        mov eax,dela
        cdq
        idiv ebx
        mov ebx,scrsy/2
        sub ebx,eax
        mov eax,scrsx
        mul ebx
        mov edi,buff.SBits
        add edi,eax
        mov ecx,scrsx
        mov eax,23
        rep stosb

        mov ebx,65536/(scrsy/5*4)
        mov eax,delb
        cdq
        idiv ebx
        mov ebx,scrsy/2
        sub ebx,eax
        mov eax,scrsx
        mul ebx
        mov edi,buff.SBits
        add edi,eax
        mov ecx,scrsx
        mov eax,23
        rep stosb

        mov ebx,65536/(scrsy/5*4)
        movzx eax,filtra
        cwde
        cdq
        idiv ebx
        mov ebx,scrsy/2
        sub ebx,eax
        mov eax,scrsx
        mul ebx
        mov edi,buff.SBits
        add edi,eax
        mov ecx,scrsx
        mov eax,21
        rep stosb

        mov ebx,65536/(scrsy/5*4)
        movzx eax,filtrb
        cwde
        cdq
        idiv ebx
        mov ebx,scrsy/2
        sub ebx,eax
        mov eax,scrsx
        mul ebx
        mov edi,buff.SBits
        add edi,eax
        mov ecx,scrsx
        mov eax,21
        rep stosb

        invoke Line,scrsx/2,0,scrsy-1,4,addr buff
        mov eax,04040404h
        mov edi,surf.SBits
        mov es,stub32zerosel
        add edi,scrsx*scrsy/2
        mov ecx,scrsx/4
        rep stosd
        mov eax,pz
        mov lnn,eax
        mov es,stub32zerosel
        mov edi,wpt
        mov eax,lnn
        shl eax,1
        add edi,eax
        sub edi,scrsx
        mov dll,0
l1:
        mov esi,lnn
        sub esi,scrsx/2
        cmp esi,0
        jl nlll
        cmp esi,wln
        jg nlll
        movzx eax,word ptr es:[edi]
        cwde
        cdq
        ;mov edx,32768
        ;sub edx,eax
        ;mov eax,edx
        mov ebx,65536/(scrsy/5*4)
        idiv ebx
        push eax
        movzx eax,word ptr es:[edi+2]
        cwde
        cdq
        ;mov edx,32768
        ;sub edx,eax
        ;mov eax,edx
        idiv ebx
        pop ebx                 ;[edi]
        push eax                ;[edi+2]
        sub eax,ebx             ;[edi+2]-[edi]
        sar eax,1               ;([edi+2]-[edi])/2
        mov edx,ebx             ;[edi+2]
        add ebx,eax             ;[edi]+([edi+2]-[edi])/2
        pop eax                 ;[edi+2]
        mov ecx,scrsy/2
        sub ecx,edx
        mov edx,ecx
        mov ecx,scrsy/2
        sub ecx,ebx
        mov ebx,ecx
        mov ecx,scrsy/2
        sub ecx,eax
        mov eax,ecx
        invoke Line,dll,edx,ebx,coll,addr buff
        inc dll
        invoke Line,dll,ebx,eax,coll,addr buff
        dec dll
nlll:
        inc lnn
        inc dll
        inc edi
        inc edi
        cmp dll,scrsx-1
        jb l1

        call Wait4VSync
        mov esi,offset buff
        mov edi,offset surf
        call CopySurface
        jmp mainloop

programquit:
        SetTxtMode
        invoke SaveWave,addr wf
        call CloseKB
programquit1:
        call FreeHeapMem
        mov ah,4ch
        int 21h

stub32 ends

end
