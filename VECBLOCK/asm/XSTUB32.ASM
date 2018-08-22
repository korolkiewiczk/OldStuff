;xstub32.asm - dodatkowy system inicjujacy.
;Nalezy wywolac procedure _xstub32init na samym poczatku programu

.386
.model flat,C

.data

        public stub32zerosel,stub32zerodesc, \
        cpu_name,cpu_ver,cpu_brand,cpu_feat,cpu_efeat

        ;selektory
        stub32zerosel dw 0      ;selektor zerowy (adres pocz.:00000000h) dla danych
        stub32zerodesc db 0ffh,0ffh,0,0,0,0f2h,0cfh,0
        ;informacje o procesorze
        cpu_name db 13 dup (0)
        cpu_ver dd 0
        cpu_brand dd 0
        cpu_feat dd 0
        cpu_efeat dd 0

.code

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

end
