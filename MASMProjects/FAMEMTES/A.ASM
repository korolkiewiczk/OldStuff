.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;dane

tekst1 db 'N-nowy wskaznik\nR-realokuj wskaznik\n0..9-usun wybrany wskaznik\nQ-wyjdz\nHEAP_PTR=%u\nFAMem_Tab_Linear_Addr=%X\nFAMem_Curr_Ptr=%u\nFAMem_High_Ptr=%u\ncptr=%u\nerror=%u\nlast time=%u\nfree mem=%u',0
tekst2 db 'NO.%2u fap:%X mla:%X mnf:%X',0
cptr dd 0
dcptr dd 0
er dd 0
lastt dd 0
fama dd 0
faptrs dd 50 dup (?)
bf db 256 dup (0)

;kod

.listall

include dpmi.inc
include mstdlib\mcon.inc
include mstdlib\strings.inc
include mstdlib\sprintf.inc
include mstdlib\mmem.inc
include mstdlib\famem.inc
include mstdlib\mmemmak.inc

.list

Printf macro x,y,txt
        invoke SPrintf,addr bf,addr txt
        invoke Print,x,y,addr bf
endm

Printfa macro x,y,txt,arg:vararg
        invoke SPrintf,addr bf,addr txt,arg
        invoke Print,x,y,addr bf
endm


main:

        invoke InitHeapMem,250*1024*1024     ;100000h
        or eax,eax
        jz wyjdzzprogramu
        call InitFAMem
        or eax,eax
        jz wyjdzzprogramu
glownapetla:
        call Cls
        call FAMemAvail
        mov fama,eax
        Printfa 0,0,tekst1,HEAP_PTR,FAMem_Tab_Linear_Addr,FAMem_Curr_Ptr,FAMem_High_Ptr,cptr,er,lastt,fama
        call drawgraph
        ReadKey
        mov er,0
        invoke ChrUpCase,al
        cmp al,'Q'
        je wyjdzzprogramu
        cmp al,'N'
        jne d1
        push eax
.586p
        rdtsc
.386
        mov lastt,eax
        and eax,255
        add eax,256
        invoke AllocFAMem,eax
        push eax
.586p
        rdtsc
        sub eax,lastt
        mov lastt,eax
.386
        pop eax
        cmp eax,NULL
        jnz derrr
        mov er,1
        jmp derrr2
derrr:
        push eax
        CopyMemD offset tekst1,fs:[eax],lengthof tekst1
        pop eax
derrr2:
        mov edi,offset faptrs
        add edi,cptr
        mov [edi],eax
        mov eax,FAMem_Curr_Ptr
        mov ebx,5
        xor edx,edx
        div ebx
        mov ebx,4
        mul ebx
        mov cptr,eax
        pop eax
d1:
        cmp al,'R'
        jne drrr

        push eax
        mov eax,dcptr
        shl eax,2
        mov ebx,faptrs[eax]
        mov lastt,eax
.586p
        rdtsc
.386
        and eax,255
        add eax,256
        invoke ReallocFAMem,ebx,eax
        pop eax
drrr:
        push eax
        cmp al,'0'
        jb d2
        cmp al,'9'
        ja d2
        sub al,'0'
        movzx eax,al
        shl eax,2
        mov cptr,eax
        mov edi,offset faptrs
        add edi,eax
        mov eax,[edi]
        push eax
.586p
        rdtsc
.386
        mov lastt,eax
        pop eax
        invoke FreeFAMem,eax
        push eax
.586p
        rdtsc
        sub eax,lastt
        mov lastt,eax
.386
        pop eax
        or eax,eax
        jnz derrrrr
        mov er,1
derrrrr:
        mov eax,FAMem_Curr_Ptr
        mov ebx,5
        xor edx,edx
        div ebx
        mov ebx,4
        mul ebx
        mov cptr,eax
d2:
        pop eax
        cmp ah,72
        jne d3
        cmp dcptr,0
        je d4
        dec dcptr
d3:
        cmp ah,80
        jne d4
        cmp dcptr,FAMEMTAB_MAX_POS
        ja d4
        inc dcptr
d4:
        jmp glownapetla
wyjdzzprogramu:
        call FreeHeapMem
        mov ax,4c00h
        int 21h

drawdumpmem:
        mov es,stub32zerosel
        mov edi,0b8000h+160*12
        mov esi,dcptr
        mov esi,faptrs[esi*4]
        cmp esi,0
        je drawdumpmem_endp
        mov esi,fs:[esi]
        mov ebx,13
        mov ah,7
        push ds
        mov dx,fs
        mov ds,dx
drawdumpmem_loop1:
                mov ecx,36
drawdumpmem_loop2:
                        lodsb
                        stosw
                        loop drawdumpmem_loop2
                add edi,(80-36)*2
                dec ebx
                jnz drawdumpmem_loop1
        pop ds
drawdumpmem_endp:
        ret

drawline:
        cld
        push edi
        push ecx
        push eax
        mov al,'Ã'
        mov ah,7
        stosw
        mov al,'Ä'
        mov ecx,39
        rep stosw
        pop eax
        pop ecx
        pop edi
        ret

drawgraph:
        call drawdumpmem
        xor edx,edx
        mov eax,FAMem_Curr_Ptr
        mov ebx,5
        div ebx
        mov esi,eax
        xor edx,edx
        mov eax,FAMem_High_Ptr
        mov ebx,5
        div ebx
        mov ebp,eax
        mov es,stub32zerosel
        mov edi,0b8000h+80
        mov al,'³'
        mov ah,7
        mov ecx,25
        xor edx,edx
drawgraph_drawline:
                push eax
                push edx
                movzx eax,dl
                xor edx,edx
                mov ebx,2
                div ebx
                mov ebx,edx
                pop edx
                pop eax
                cmp ebx,1
                jne drawgraph_d1
                call drawline
                jmp drawgraph_d5
drawgraph_d1:
                mov al,'³'
                mov es:[edi],ax
                mov ebx,edx
                shr ebx,1
                cmp esi,ebx
                jne drawgraph_d2
                mov al,'C'
                mov es:[edi-2],ax
drawgraph_d2:
                cmp ebp,ebx
                jne drawgraph_d3
                mov al,'H'
                mov es:[edi-4],ax
drawgraph_d3:
                push eax
                mov eax,cptr
                shr eax,2
                cmp eax,ebx
                jne drawgraph_d4
                mov al,'P'
                mov ah,7
                mov es:[edi-6],ax
drawgraph_d4:
                cmp ebx,dcptr
                jne drawgraph_d41
                mov CON_TEXT_ATTR,1
drawgraph_d41:
                push edi
                mov edi,ebx
                shl edi,2
                add edi,ebx
                add edi,FAMem_Tab_Linear_Addr
                movzx eax,byte ptr fs:[edi+4]
                Printfa 41,dl,tekst2,ebx,faptrs[ebx*4],dword ptr fs:[edi],eax
                pop edi
                pop eax
                mov CON_TEXT_ATTR,7
drawgraph_d5:
                add edi,160
                inc dl
                dec ecx
                jnz drawgraph_drawline
        ret
stub32 ends

end
