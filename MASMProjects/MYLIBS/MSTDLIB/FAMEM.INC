IFNDEF __FAMEM_INC
__FAMEM_INC=1
;FAMEM.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do obslugi FreeAllocMem-pamieci o dowolnej alokacji i zwalnianiu

;includy

include mmem.inc

;stale

FAMEMTAB_MAX_POS=65536/5

;struktory

TFAMemTabCell struct
        mem_lin_addr dd 0
        mem_null_flag db 0
TFAMemTabCell ends

;dane

FAMem_Tab_Linear_Addr dd 0

FAMem_Curr_Ptr dd 0
FAMem_High_Ptr dd 0

;Makra

GetFAPtr macro reg:req,pt:req
        mov reg,[pt]
        mov reg,fs:[reg]
endm

;Funkcje

;--------------------------------
;InitFAMem ()
;       Initializuje pamiec FAMem (wymaga wczesniejszego wywolania funkcji InitHeapMem z modulu mmem.inc)
;       Wyjscie: eax=0 blad/eax=1 ok
;--------------------------------

InitFAMem proc stdcall uses eax ecx edi
        call MemAvail
        cmp eax,FAMEMTAB_MAX_POS+1
        jbe InitFAMem_endpz
        invoke AllocMem,FAMEMTAB_MAX_POS*sizeof (TFAMemTabCell)
        cmp eax,NULL
        je InitFAMem_endpz
        mov FAMem_Tab_Linear_Addr,eax
        mov edi,eax
        mov ecx,FAMEMTAB_MAX_POS
        mov eax,NULL
        assume edi:ptr TFAMemTabCell
InitFAMem_fill_null:
                mov fs:[edi].mem_lin_addr,eax
                mov fs:[edi].mem_null_flag,al
                add edi,sizeof (TFAMemTabCell)
                loop InitFAMem_fill_null
        assume edi:nothing
        mov FAMem_Curr_Ptr,0
        mov FAMem_High_Ptr,0
        xor eax,eax
        inc eax
        jmp InitFAMem_endp
InitFAMem_endpz:
        xor eax,eax
InitFAMem_endp:
        ret
InitFAMem endp

;--------------------------------
;AllocFAMem (meminbytes:dword)
;       Alokuje pamiec FAMem
;       Wejscie: meminbytes-ilosc alokowanej pam.
;       Wyjscie: eax=NULL blad/eax-offset do pozycji w FAMem_Tab_Linear_Addr
;--------------------------------

AllocFAMem proc stdcall uses ecx edx esi edi,meminbytes:dword
        assume edi:ptr TFAMemTabCell
        ;
        call FAMemAvail
        cmp eax,meminbytes
        jb AllocFAMem_endpz
        ;
        mov eax,FAMem_Curr_Ptr
        cmp eax,FAMEMTAB_MAX_POS*sizeof (TFAMemTabCell)
        jae AllocFAMem_endpz
        ;
        cmp eax,FAMem_High_Ptr
        jne AllocFAMem_d1
        ;
        invoke AllocMem,meminbytes
        mov edi,FAMem_Tab_Linear_Addr
        add edi,FAMem_Curr_Ptr
        mov fs:[edi].mem_lin_addr,eax
        xor al,al
        mov fs:[edi].mem_null_flag,al
        mov eax,FAMem_Curr_Ptr
        add eax,FAMem_Tab_Linear_Addr
        add FAMem_Curr_Ptr,sizeof (TFAMemTabCell)
        add FAMem_High_Ptr,sizeof (TFAMemTabCell)
        jmp AllocFAMem_endp
        ;
AllocFAMem_d1:
        call FindHighFATabNPtr
        cmp eax,NULL
        je AllocFAMem_endpz
        ;
        mov edi,FAMem_Tab_Linear_Addr
        add edi,eax
        mov eax,fs:[edi].mem_lin_addr+sizeof (TFAMemTabCell)
        sub eax,fs:[edi].mem_lin_addr
        cmp meminbytes,eax
        ja AllocFAMem_a
        jb AllocFAMem_b

        ;jesli rowne
        xor al,al
        mov fs:[edi].mem_null_flag,al
        ;
        call FindHighFATabNPtr
        cmp eax,NULL
        jne AllocFAMem_nceh1
        mov eax,FAMem_High_Ptr
        mov FAMem_Curr_Ptr,eax
        mov eax,edi
        jmp AllocFAMem_endp
AllocFAMem_nceh1:
        mov FAMem_Curr_Ptr,eax
        mov eax,edi
        jmp AllocFAMem_endp
AllocFAMem_b:

        ;jesli mniejsze
        mov ecx,eax
        sub ecx,meminbytes
        sub HEAP_PTR,ecx
        mov eax,edi
        push ecx
        push edi
        mov edi,fs:[edi].mem_lin_addr+sizeof (TFAMemTabCell)
        mov esi,edi
        sub edi,ecx
        ;
        assume eax:ptr TFAMemTabCell
        mov ecx,fs:[eax].mem_lin_addr
        mov eax,FAMem_High_Ptr
        sub eax,sizeof (TFAMemTabCell)
        add eax,FAMem_Tab_Linear_Addr
        mov eax,fs:[eax].mem_lin_addr
        assume eax:nothing
        sub eax,ecx
        cld
        mov ecx,eax
        push ds
        mov es,stub32zerosel
        mov ds,stub32zerosel
        shr ecx,2
        and eax,3
        rep movsd
        mov ecx,eax
        rep movsb
        pop ds
        pop edi
        ;
        xor edx,edx
        mov ecx,edi
        sub ecx,FAMem_Tab_Linear_Addr
        mov eax,FAMem_High_Ptr
        sub eax,ecx
        sub eax,sizeof (TFAMemTabCell)
        mov ecx,sizeof (TFAMemTabCell)
        div ecx
        pop esi
        cmp eax,0
        jz AllocFAMem_eaxe1
        mov ecx,eax
        push edi
        add edi,sizeof (TFAMemTabCell)
AllocFAMem_copy_down_loop1:
                mov eax,fs:[edi].mem_lin_addr
                sub eax,esi
                mov fs:[edi].mem_lin_addr,eax
                add edi,sizeof (TFAMemTabCell)
                loop AllocFAMem_copy_down_loop1
        ;
        pop edi
AllocFAMem_eaxe1:
        xor al,al
        mov fs:[edi].mem_null_flag,al
        call FindHighFATabNPtr
        cmp eax,NULL
        jne AllocFAMem_nceh2
        mov eax,FAMem_High_Ptr
        mov FAMem_Curr_Ptr,eax
        mov eax,edi
        jmp AllocFAMem_endp
AllocFAMem_nceh2:
        mov FAMem_Curr_Ptr,eax
        mov eax,edi
        jmp AllocFAMem_endp
        ;
AllocFAMem_a:

        ;jesli wieksze
        mov ecx,meminbytes
        sub ecx,eax
        mov esi,HEAP_PTR
        add esi,HEAP_LIN_ADDR
        invoke AllocMem,ecx
        cmp eax,NULL
        je AllocFAMem_endpz
        ;
        mov eax,edi
        push ecx
        push edi
        mov edi,esi
        add edi,ecx
        assume eax:ptr TFAMemTabCell
        mov ecx,fs:[eax].mem_lin_addr
        mov eax,esi
        assume eax:nothing
        sub eax,ecx
        std
        mov ecx,eax
        push ds        
        mov es,stub32zerosel
        mov ds,stub32zerosel
        shr ecx,2
        and eax,3
        rep movsd
        mov ecx,eax
        rep movsb
        pop ds
        pop edi
        ;
        xor edx,edx
        mov ecx,edi
        sub ecx,FAMem_Tab_Linear_Addr
        mov eax,FAMem_High_Ptr
        sub eax,ecx
        sub eax,sizeof (TFAMemTabCell)
        mov ecx,sizeof (TFAMemTabCell)
        div ecx
        pop esi
        cmp eax,0
        jz AllocFAMem_eaxe02
        mov ecx,eax
        push edi
        add edi,sizeof (TFAMemTabCell)
AllocFAMem_copy_down_loop2:
                mov eax,fs:[edi].mem_lin_addr
                add eax,esi
                mov fs:[edi].mem_lin_addr,eax
                add edi,sizeof (TFAMemTabCell)
                loop AllocFAMem_copy_down_loop2
        ;
        pop edi
AllocFAMem_eaxe02:
        xor al,al
        mov fs:[edi].mem_null_flag,al
        call FindHighFATabNPtr
        cmp eax,NULL
        jne AllocFAMem_nceh3
        mov eax,FAMem_High_Ptr
        mov FAMem_Curr_Ptr,eax
        mov eax,edi
        jmp AllocFAMem_endp
AllocFAMem_nceh3:
        mov FAMem_Curr_Ptr,eax
        mov eax,edi
        jmp AllocFAMem_endp

AllocFAMem_endpz:
        mov eax,NULL
AllocFAMem_endp:
        ret
        assume edi:nothing
AllocFAMem endp

;--------------------------------
;ReallocFAMem (wsk:dword,meminbytes:dword)
;       Realokuje pamiec FAMem
;       Wejscie: wsk-dany wskaznik
;                meminbytes-ilosc alokowanej pam.
;       Wyjscie: eax=0 blad/eax=1 ok
;--------------------------------

ReallocFAMem proc stdcall uses ecx edx esi edi,wsk:dword,meminbytes:dword
        assume edi:ptr TFAMemTabCell
        ;
        call FAMemAvail
        cmp eax,meminbytes
        jb ReallocFAMem_endpz
        ;
        mov eax,wsk
        mov edi,FAMEMTAB_MAX_POS*sizeof (TFAMemTabCell)
        add edi,FAMem_Tab_Linear_Addr
        cmp eax,edi
        jae ReallocFAMem_endpz
        cmp eax,FAMem_Tab_Linear_Addr
        jb ReallocFAMem_endpz
        ;
        mov edi,FAMem_High_Ptr
        add edi,FAMem_Tab_Linear_Addr
        sub edi,sizeof (TFAMemTabCell)
        cmp eax,edi
        jne ReallocFAMem_d1
        ;
        cmp fs:[edi].mem_null_flag,0
        jne ReallocFAMem_endpz
        mov eax,HEAP_PTR
        add eax,HEAP_LIN_ADDR
        sub eax,fs:[edi].mem_lin_addr
        sub eax,meminbytes
        cmp eax,0
        jl ReallocFAMem_alloc_mem
        sub HEAP_PTR,eax
        jmp ReallocFAMem_no_alloc_mem
ReallocFAMem_alloc_mem:
        neg eax
        invoke AllocMem,eax
ReallocFAMem_no_alloc_mem:
        xor eax,eax
        inc eax
        jmp ReallocFAMem_endp
        ;
ReallocFAMem_d1:
        ;
        mov edi,eax
        cmp fs:[edi].mem_null_flag,0
        jne ReallocFAMem_endpz
        mov eax,fs:[edi].mem_lin_addr+sizeof (TFAMemTabCell)
        sub eax,fs:[edi].mem_lin_addr
        cmp meminbytes,eax
        ja ReallocFAMem_a
        jb ReallocFAMem_b

        ;jesli rowne
        xor eax,eax
        inc eax
        jmp ReallocFAMem_endp
ReallocFAMem_b:

        ;jesli mniejsze
        mov ecx,eax
        sub ecx,meminbytes
        sub HEAP_PTR,ecx
        mov eax,edi
        push ecx
        push edi
        mov edi,fs:[edi].mem_lin_addr+sizeof (TFAMemTabCell)
        mov esi,edi
        sub edi,ecx
        ;
        assume eax:ptr TFAMemTabCell
        mov ecx,fs:[eax].mem_lin_addr
        mov eax,FAMem_High_Ptr
        sub eax,sizeof (TFAMemTabCell)
        add eax,FAMem_Tab_Linear_Addr
        mov eax,fs:[eax].mem_lin_addr
        assume eax:nothing
        sub eax,ecx
        cld
        mov ecx,eax
        push ds
        mov es,stub32zerosel
        mov ds,stub32zerosel
        shr ecx,2
        and eax,3
        rep movsd
        mov ecx,eax
        rep movsb
        pop ds
        pop edi
        ;
        xor edx,edx
        mov ecx,edi
        sub ecx,FAMem_Tab_Linear_Addr
        mov eax,FAMem_High_Ptr
        sub eax,ecx
        sub eax,sizeof (TFAMemTabCell)
        mov ecx,sizeof (TFAMemTabCell)
        div ecx
        pop esi
        cmp eax,0
        jz ReallocFAMem_eaxe1
        mov ecx,eax
        push edi
        add edi,sizeof (TFAMemTabCell)
ReallocFAMem_copy_down_loop1:
                mov eax,fs:[edi].mem_lin_addr
                sub eax,esi
                mov fs:[edi].mem_lin_addr,eax
                add edi,sizeof (TFAMemTabCell)
                loop ReallocFAMem_copy_down_loop1
        ;
        pop edi
ReallocFAMem_eaxe1:
        xor eax,eax
        inc eax
        jmp ReallocFAMem_endp
        ;
ReallocFAMem_a:

        ;jesli wieksze
        mov ecx,meminbytes
        sub ecx,eax
        mov esi,HEAP_PTR
        add esi,HEAP_LIN_ADDR
        invoke AllocMem,ecx
        cmp eax,NULL
        je ReallocFAMem_endpz
        ;
        mov eax,edi
        push ecx
        push edi
        mov edi,esi
        add edi,ecx
        assume eax:ptr TFAMemTabCell
        mov ecx,fs:[eax].mem_lin_addr
        mov eax,esi
        assume eax:nothing
        sub eax,ecx
        std
        mov ecx,eax
        push ds        
        mov es,stub32zerosel
        mov ds,stub32zerosel
        shr ecx,2
        and eax,3
        rep movsd
        mov ecx,eax
        rep movsb
        pop ds
        pop edi
        ;
        xor edx,edx
        mov ecx,edi
        sub ecx,FAMem_Tab_Linear_Addr
        mov eax,FAMem_High_Ptr
        sub eax,ecx
        sub eax,sizeof (TFAMemTabCell)
        mov ecx,sizeof (TFAMemTabCell)
        div ecx
        pop esi
        cmp eax,0
        jz ReallocFAMem_eaxe02
        mov ecx,eax
        push edi
        add edi,sizeof (TFAMemTabCell)
ReallocFAMem_copy_down_loop2:
                mov eax,fs:[edi].mem_lin_addr
                add eax,esi
                mov fs:[edi].mem_lin_addr,eax
                add edi,sizeof (TFAMemTabCell)
                loop ReallocFAMem_copy_down_loop2
        ;
        pop edi
ReallocFAMem_eaxe02:
        xor eax,eax
        inc eax
        jmp ReallocFAMem_endp

ReallocFAMem_endpz:
        xor eax,eax
ReallocFAMem_endp:
        ret
        assume edi:nothing
ReallocFAMem endp

;--------------------------------
;FreeFAMem (wsk:dword)
;       Zwalnia pamiec FAMem
;       Wejscie: wsk-wskaznik do zwolnienia
;       Wyjscie: eax=0 blad/eax=1 ok
;--------------------------------

FreeFAMem proc stdcall uses edi,wsk:dword
        assume edi:ptr TFAMemTabCell
        mov eax,wsk
        cmp eax,FAMem_Tab_Linear_Addr
        jb FreeFAMem_error
        ;
        sub eax,FAMem_Tab_Linear_Addr
        mov edi,FAMem_High_Ptr
        sub edi,sizeof (TFAMemTabCell)
        cmp eax,edi
        jb FreeFAMem_d1
        ja FreeFAMem_error
        ;
        mov edi,wsk
        mov al,fs:[edi].mem_null_flag
        or al,al
        jnz FreeFAMem_error
        mov eax,NULL
        mov fs:[edi].mem_null_flag,al
        mov eax,HEAP_LIN_ADDR
        add eax,HEAP_PTR
        sub eax,fs:[edi].mem_lin_addr
        sub HEAP_PTR,eax
        sub FAMem_High_Ptr,sizeof (TFAMemTabCell)
        jmp FreeFAMem_endp
        ;
FreeFAMem_d1:
        mov edi,wsk
        mov al,fs:[edi].mem_null_flag
        or al,al
        jnz FreeFAMem_error
        mov eax,NULL
        mov fs:[edi].mem_null_flag,al
        xor eax,eax
        inc eax
        jmp FreeFAMem_endp
FreeFAMem_error:
        call FindHighFATabNPtr
        cmp eax,NULL
        jne FreeFAMem_ncehe
        mov eax,FAMem_High_Ptr
        mov FAMem_Curr_Ptr,eax
        jmp FreeFAMem_endppe
FreeFAMem_ncehe:
        mov FAMem_Curr_Ptr,eax
FreeFAMem_endppe:
        xor eax,eax
        jmp FreeFAMem_endp2
FreeFAMem_endp:
        call FindHighFATabNPtr
        cmp eax,NULL
        jne FreeFAMem_nceh
        mov eax,FAMem_High_Ptr
        mov FAMem_Curr_Ptr,eax
        jmp FreeFAMem_endpp
FreeFAMem_nceh:
        mov FAMem_Curr_Ptr,eax
FreeFAMem_endpp:
        xor eax,eax
        inc eax
FreeFAMem_endp2:
        call GetHighFAMemPtr
        ret
        assume edi:nothing
FreeFAMem endp

;--------------------------------
;FAMemAvail ()
;       Pobiera wolna pamiec FAMem
;       Wyjscie: eax-ilosc pam
;--------------------------------
FAMemAvail proc stdcall uses ebx ecx edi
        call MemAvail
        push eax
        xor ebx,ebx
        mov edi,FAMem_Tab_Linear_Addr
        add edi,FAMem_High_Ptr
        sub edi,sizeof (TFAMemTabCell)
        cmp edi,FAMem_Tab_Linear_Addr
        jb FAMemAvail_endp
        assume edi:ptr TFAMemTabCell
        ;
        mov ecx,FAMem_High_Ptr
        sub ecx,sizeof (TFAMemTabCell)
        or ecx,ecx
        jz FAMemAvail_endp
        sub edi,sizeof (TFAMemTabCell)
FAMemAvail_loop1:
                mov al,fs:[edi].mem_null_flag
                or al,al
                jz FAMemAvail_nosum
                mov eax,fs:[edi].mem_lin_addr+sizeof (TFAMemTabCell)
                sub eax,fs:[edi].mem_lin_addr
                add ebx,eax
FAMemAvail_nosum:
                sub edi,sizeof (TFAMemTabCell)
                sub ecx,sizeof (TFAMemTabCell)
                jnz FAMemAvail_loop1
FAMemAvail_endp:
        pop eax
        add eax,ebx
        ret
FAMemAvail endp

;--------------------------------
;FindHighFATabNPtr ()
;       Szuka ostatnia wolna komorke w FAMem_Tab_Linear_Addr
;       Wyjscie: eax=ta Komorka
;--------------------------------

FindHighFATabNPtr proc stdcall uses edi
        mov edi,FAMem_Tab_Linear_Addr
        add edi,FAMem_High_Ptr
        sub edi,sizeof (TFAMemTabCell)
        cmp edi,FAMem_Tab_Linear_Addr
        jb FindHighFATabNPtr_endpz
        assume edi:ptr TFAMemTabCell
FindHighFATabNPtr_loop:
                mov al,fs:[edi].mem_null_flag
                or al,al
                jnz FindHighFATabNPtr_loop_endl
                ;
                sub edi,sizeof (TFAMemTabCell)
                mov eax,edi
                sub eax,FAMem_Tab_Linear_Addr
                cmp eax,0
                jl FindHighFATabNPtr_endpz
                jmp FindHighFATabNPtr_loop
                ;
FindHighFATabNPtr_loop_endl:
        mov eax,edi
        sub eax,FAMem_Tab_Linear_Addr
        jmp FindHighFATabNPtr_endp
FindHighFATabNPtr_endpz:
        mov eax,NULL
FindHighFATabNPtr_endp:
        ret
        assume edi:nothing
FindHighFATabNPtr endp

;--------------------------------
;GetHighFAMemPtr ()
;       Ustawia FAMem_High_Ptr
;--------------------------------

GetHighFAMemPtr proc stdcall uses eax edi
        mov edi,FAMem_Tab_Linear_Addr
        add edi,FAMem_High_Ptr
        sub edi,sizeof (TFAMemTabCell)
        cmp edi,FAMem_Tab_Linear_Addr
        jb GetHighFAMemPtr_endp
        assume edi:ptr TFAMemTabCell
        push edi
GetHighFAMemPtr_loop:
                mov al,fs:[edi].mem_null_flag
                or al,al
                jz GetHighFAMemPtr_break_loop
                sub edi,sizeof (TFAMemTabCell)
                ;
                mov eax,edi
                sub eax,FAMem_Tab_Linear_Addr
                cmp eax,0
                jl GetHighFAMemPtr_break_loop
                jmp GetHighFAMemPtr_loop
GetHighFAMemPtr_break_loop:
        pop eax
        sub eax,edi
        or eax,eax
        jz GetHighFAMemPtr_endp
        sub FAMem_High_Ptr,eax
        mov eax,fs:[edi].mem_lin_addr+sizeof (TFAMemTabCell)
        sub eax,HEAP_LIN_ADDR
        mov HEAP_PTR,eax
        call FindHighFATabNPtr
        cmp eax,NULL
        je GetHighFAMemPtr_hec
        mov FAMem_Curr_Ptr,eax
        jmp GetHighFAMemPtr_endp
GetHighFAMemPtr_hec:
        mov eax,FAMem_High_Ptr
        mov FAMem_Curr_Ptr,eax
GetHighFAMemPtr_endp:
        ret
        assume edi:nothing
GetHighFAMemPtr endp

ENDIF
