;STUB32-DPMI for MASM by Kamil Korolkiewicz 2005
;v. 1.01
;Aby uzyc stub32 nalezy w glownym pliku:
;- zdefiniowac segment stub32 public
;- zdefiniowac etykiete/procedure main oraz napisac: public main
;- na koncu wpisac instrukcje wyjscia: mov ax,4c00h; int 21h
;Skompilowac wpisujac: ml stub32.obj nazwa_programu.asm
;lub:mlstub32 nazwa_programu
;---

.386

include stub32.inc

extrn main:near

public stub32codesel,stub32datasel,stub32zerosel,stub32inthandrec,dpmiver


STACKSIZE=512
;HEAPSIZE=1048576 ;1MB

;segment kodu 16-bitowego

stub16 segment public use16
assume cs:stub16,ds:stub32,ss:program_stack
;kod
stub16_start:

        mov ax,seg stub32
        mov ds,ax
        mov stub32saveds,ax

        ;wypisanie tytulu
        mov ax,0900h
        mov dx,offset stub32title
        int 21h

        ;pobranie DPMI
        mov ax,1687h
        int 2fh
        cmp ax,0
        je dpmipresent

        ;jesli nie ma DPMI to wyjdz z programu
        mov ax,0900h
        mov dx,offset nodpmi
        int 21h
        mov ax,4c01h
        int 21h
        ;
        dpmipresent:
        test bl,1
        jnz _32bitclient

        ;jesli DPMI jest 16-bitowe to wyjdz z programu
        mov ax,0900h
        mov dx,offset nodpmi32
        int 21h
        mov ax,4c01h
        int 21h
        ;
        _32bitclient:
        mov dpmiver,dx ;zapamietaj wersje DPMI
        mov word ptr pmode_entry_point,di
        mov word ptr pmode_entry_point+2,es
        ;
        cmp si,0
        je noallocmem4dpmi

        ;pobierz pamiec dla DPMI
        mov ax,4800h
        mov bx,si
        int 21h
        mov es,di
        ;
        noallocmem4dpmi:
        mov ax,1 ;ma byc tryb 32bit
        call pmode_entry_point
        jnc prot16

        ;jesli nie mozna przejsc do trybu chronionego to wyjdz z programu
        mov ax,0900h
        mov dx,offset noinitdpmi
        int 21h
        mov ax,4c01h
        int 21h

        ;---
        ;16-bitowy tryb chroniony
        ;---
        prot16:
        ;
        mov stub32pspsel,es
        push ds
        pop es
        ;
        xor ax,ax
        mov cx,3
        int 31h ;pobranie selektora dla kodu,danych,ramki pamieci (heap) i selektora zero
        ;
        push ax
        mov ax,3
        int 31h ;inc dla selektora
        mov bx,ax
        pop ax

        ;ustawienie selektorow
        mov stub32codesel,ax
        add ax,bx
        mov stub32datasel,ax
        add ax,bx
        mov stub32zerosel,ax

        ;wyliczenie adresu kodu 32-bitowego
        xor eax,eax
        mov ax,seg stub32
        shl eax,4

        ;ustawienie selektora kodu i danych
        mov word ptr stub32codedesc+2,ax
        mov word ptr stub32datadesc+2,ax
        shr eax,16
        mov byte ptr stub32codedesc+4,al
        mov byte ptr stub32datadesc+4,al
        shr eax,8
        mov byte ptr stub32codedesc+7,al
        mov byte ptr stub32datadesc+7,al

        ;wyliczenie adresu kodu 32-bitowego
        xor eax,eax
        mov ax,seg program_stack
        shl eax,4

        ;kod
        mov ax,0ch
        mov bx,stub32codesel
        mov edi,offset stub32codedesc
        int 31h

        ;dane
        mov ax,0ch
        mov bx,stub32datasel
        mov edi,offset stub32datadesc
        int 31h

        ;sel. zero
        mov ax,0ch
        mov bx,stub32zerosel
        mov edi,offset stub32zerodesc
        int 31h

        ;przejscie do trybu 32-bitowego        
        push dword ptr stub32codesel
        push stub32_start
        db 66h
        retf    ;32-bit retf
        ;program w trybie 32-bitowym

stub16 ends


;segment kodu 32-bitowego

assume cs:stub32,ds:stub32,ss:program_stack
stub32 segment public use32
;dane
        ;napisy bledow
        stub32title db 'STUB32-DPMI for MASM by Kamil Korolkiewicz',13,10,'$'
        nodpmi db 'DPMI no present! Can''t load program',13,10,'$'
        nodpmi32 db 'DPMI is not 32-bit! Can''t load program',13,10,'$'
        noinitdpmi db 'Can''t init DPMI!',13,10,'$'
        ;
        dpmiver dw 0            ;wersja DPMI
        pmode_entry_point dd 0  ;adres wejscia do trybu chronionego        
        ;selektory
        stub32pspsel dw 0             ;selektor na psp
        stub32codesel dw 0      ;selektor kodu
        stub32datasel dw 0      ;selektor danych
        stub32zerosel dw 0      ;selektor zerowy (adres pocz.:00000000h)
        stub32codedesc db 0ffh,0ffh,0,0,0,0fah,0cfh,0
        stub32datadesc db 0ffh,0ffh,0,0,0,0f2h,0cfh,0
        stub32zerodesc db 0ffh,0ffh,0,0,0,0f2h,0cfh,0
        ;przerwania
        stub32actintnum dd 0
        stub32inthandrec DPMI_RMCS <>
        stub32saveds dw 0       ;zapisany rejestr ds

;kod
;int 10h
__stub32_int_10:
        mov stub32actintnum,10h
        jmp stub32interrupthandler

;int 16h
__stub32_int_16:
        mov stub32actintnum,16h
        jmp stub32interrupthandler

;int 21h
__stub32_int_21:
        mov stub32actintnum,21h
        jmp stub32interrupthandler

;procedura obslugi przerwan

stub32interrupthandler:
        push es
        push fs
        push gs
        ;przepisanie rejestrow do stub32inthandrec
        mov stub32inthandrec.__eax,eax
        mov stub32inthandrec.__ebx,ebx
        mov stub32inthandrec.__ecx,ecx
        mov stub32inthandrec.__edx,edx
        mov stub32inthandrec.__edi,esi
        mov stub32inthandrec.__esi,edi
        mov stub32inthandrec.__ebp,ebp
        mov stub32inthandrec.__ss,0
        mov stub32inthandrec.__sp,0
        pushf
        pop stub32inthandrec.__flg
        mov ax,stub32saveds
        mov stub32inthandrec.__ds,ax            ;przypisz rejestr ds z trybu rzeczywistego
        mov stub32inthandrec.__es,ax            ;przypisz rejestr es z trybu rzeczywistego
        mov stub32inthandrec.__fs,fs
        mov stub32inthandrec.__gs,gs
        ;
        mov fs,stub32zerosel
        mov eax,stub32actintnum
        mov bx,fs:[eax*4]
        mov stub32inthandrec.__ip,bx
        mov bx,fs:[eax*4+2]
        mov stub32inthandrec.__cs,bx
        ;
        mov ax,0302h
        xor bx,bx
        xor cx,cx
        mov es,stub32datasel
        mov edi,offset stub32inthandrec
        int 31h
        ;odtworzenie rejestrow
        pop gs
        pop fs
        pop es
        mov eax,stub32inthandrec.__eax
        mov ebx,stub32inthandrec.__ebx
        mov ecx,stub32inthandrec.__ecx
        mov edx,stub32inthandrec.__edx
        mov edi,stub32inthandrec.__edi
        mov esi,stub32inthandrec.__esi
        mov ebp,stub32inthandrec.__ebp        
        ;
        iretd

;poczatek kodu 32-bitowego
stub32_start:

        ;---
        ;32-bitowy tryb chroniony
        ;---
        ;poczatkowe ustawienia selektorow
        mov ds,stub32datasel
        mov es,stub32zerosel
        mov fs,stub32zerosel
        mov gs,stub32zerosel

        ;ustawienie stosu
        mov esp,STACKSIZE

        ;ustawienie przerwan
        ;int 10h
        mov ax,0205h
        mov bl,10h
        mov cx,stub32codesel
        mov edx,offset __stub32_int_10
        int 31h

        ;int 16h
        mov ax,0205h
        mov bl,16h
        mov cx,stub32codesel
        mov edx,offset __stub32_int_16
        int 31h

        ;int 21h
        mov ax,0205h
        mov bl,21h
        mov cx,stub32codesel
        mov edx,offset __stub32_int_21
        int 31h

        ;przejscie do programu
        jmp main

stub32 ends


;stos programu (domyslny)

program_stack segment stack
__program_stack db STACKSIZE dup (?)
program_stack ends


end stub16_start
