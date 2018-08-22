.386

public main

stub32 segment public

assume cs:stub32,ds:stub32

include dpmi.inc
include aiconv.inc
include strings.inc

n db 'HELLO WORLD!!!',0
liczba db '            '
meminf DPMI_MEM_INFO <>

curx dd 0
cury dd 0

l db '123456789',0
db 15 dup (20h)

i dd 0

p dw 0

ad:
dd 0bff52098h
;dd 0bff77716h
datadesc db 0ffh,0ffh,0,0,0,092h,0cfh,0
codedesc db 0ffh,0ffh,0,0,0,09eh,0cfh,0
ddesc dw 0
cdesc dw 0
cpl0:
dw 0
dd 0
cpl3:
dw 0
dd 0


id db 6 dup (0)

writestr proc C s:dword
  mov ax,stub32zerosel
  mov es,ax
  mov esi,s
  mov eax,cury
  mov ebx,160
  mul ebx
  mov ebx,curx
  shl ebx,1
  ;add eax,ebx
  mov edi,0b8000h
  add edi,eax
  mov ah,7
  .while 1
    mov al,[si]
    .break .if al==0
    stosw
    inc esi
  .endw
  ret
writestr endp


main:
comment #
        int 3
        mov ebx,stub32baseaddress
        add ebx,offset CPL0EP
        mov eax,offset id
        sidt [eax]
        mov edx,[eax+2]
        mov es:[edx],bx
        shr ebx,16
        mov es:[edx+6],bx
        int 0

CPL0EP:
        int 3

        ;AllocLDTDesc 1
        int 3
        Seg2Desc 0
        mov p,ax
        SetSelBA p,81914bf8h
        mov fs,p
        ;Seg2Desc 0b800h
        ;mov es,ax
        ;mov edi,0
        ;mov eax,07070707h
        ;mov es:[edi],eax
        ;mov ax,0900h
        ;mov edx,offset n
        ;int 21h
        ;int 3
        ;mov eax,offset ad
        ;mov [eax+4],es
        ;mov ebx,offset ad
        ;mov ax,stub32codezerosel
        ;mov [ebx+4],ax
        ;mov ax,ds
        ;mov gs,ax
        ;push es
        ;pop ds
        ;push 0bff52098h
        ;jmp fword ptr gs:[ad]
        ;retf
        ;mov ds,ax

        push ds
        push cs
        push offset a
        push stub32codezerosel
        mov eax,stub32baseaddress
        add eax,offset ep
        push eax
        retf
        ep:
        push ds
        pop gs
        mov es,stub32zerosel
        mov ds,stub32zerosel
        push 0
        push 0
        call dword ptr gs:[ad]
        retf
        a:
        pop ds        

        mov eax,stub32baseaddress

        ;ustawienie selektora kodu i danych
        mov word ptr codedesc+2,ax
        shr eax,16
        mov byte ptr codedesc+4,al
        shr eax,8
        mov byte ptr codedesc+7,al

        int 3
        AllocLDTDesc 2
        and ax,1111111111111000b
        mov ddesc,ax
        add ax,8
        mov cdesc,ax

        int 3
        mov eax,offset gd
        sgdt [eax]
        mov ebx,[eax+2]
        xor edx,edx
        sldt dx
        add ebx,edx
        mov eax,dword ptr es:[ebx]
        mov edx,dword ptr es:[ebx+4]
        shr eax,16
        and edx,0ff0000ffh
        mov bl,dl
        shr edx,16
        mov dl,bl
        shl edx,16
        add eax,edx
        movzx ebx,ddesc
        mov edx,dword ptr datadesc
        mov dword ptr es:[eax+ebx],edx
        mov edx,dword ptr datadesc+4
        mov dword ptr es:[eax+ebx+4],edx
        movzx ebx,cdesc
        mov edx,dword ptr codedesc
        mov dword ptr es:[eax+ebx],edx
        mov edx,dword ptr codedesc+4
        mov dword ptr es:[eax+ebx+4],edx

        int 3
        mov ax,cdesc
        or ax,0000000000000100b
        mov word ptr cpl0+4,ax
        mov dword ptr cpl0,offset cpl0ep
        call fword ptr [cpl0]
cpl0ep:         ;cpl=0
        add esp,8

        mov ax,cdesc
        or ax,0000000000000100b
        mov stub32zerosel,ax
        mov es,ax

        int 3


;        mov ax,stub32codesel
;        mov word ptr cpl3+4,ax
;        mov dword ptr cpl3,offset cpl3ep
;        call fword ptr [cpl3]
;cpl3ep:         ;cpl=3
;        add esp,6#

        int 3

        int 20h
        dd 00010108h

        inc cury
        invoke AToU,addr l
        invoke IToA,eax,addr liczba
        invoke writestr,addr liczba
        invoke StrLen,addr l
        invoke IToA,eax,addr liczba
        inc cury
        invoke writestr,addr liczba
        int 3
        invoke StrCat,addr l,addr n
        inc cury
        invoke writestr,addr l
        mov ax,0700h
        int 21h
        ;
        inc cury
        invoke IToA,-25,addr liczba
        invoke writestr,addr liczba
        mov ax,0700h
        int 21h
        inc cury
        ;
        invoke UToA,25567,addr liczba
        invoke writestr,addr liczba
        mov ax,0700h
        int 21h
        inc cury
        ;
        mov ax,stub32zerosel
        mov es,ax
        AllocMemBlock 1024*1024
        mov edi,ebx
        mov ecx,1024*1024
        mov eax,0
        rep stosb
        invoke IToA,ebx,addr liczba
        invoke writestr,addr liczba
        mov ax,0700h
        int 21h
        inc cury

        mov ax,dpmiver
        mov word ptr i,ax
        invoke IToA,addr i,addr liczba
        invoke writestr,addr liczba
        mov ax,0700h
        int 21h
        inc cury

        GetMemInfo offset meminf
        mov i,0
        .repeat
                mov edi,offset meminf
                add edi,i
                invoke IToA,[edi],addr liczba
                invoke writestr,addr liczba
                add i,4
                inc cury
        .until i >= 36

        invoke UToA,stub32baseaddress,addr liczba
        invoke writestr,addr liczba
        mov ax,0700h
        int 21h
        inc cury

        mov ax,4c00h
        int 21h

stub32 ends

end
