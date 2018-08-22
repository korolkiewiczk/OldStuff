IFNDEF __MMEM_INC
__MMEM_INC=1
;MMEM.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do oblugi pamieci

_DPMI_HEAP_MEM_HANDLE dd 0

HEAP_LIN_ADDR dd 0
HEAP_MEM_SIZE dd 0

HEAP_PTR dd 0           ;aktualny szczyt pamieci heap

HEAP_ALIGN dd 4         ;wyrownanie alokowanej pamieci

NULL=0FFFFFFFFH

;funckje

;--------------------------------
;InitHeapMem (heapmeminbytes : dword)
;       Initializuje pamiec heap.
;       Wejscie: heapmeminbytes-ilosc pamieci heap
;       Wyjscie: eax=0 blad, eax=1 ok
;       Uwaga: Ta procedura musi byc wywolana przed kazda inna procedura do obslugi pamieci
;--------------------------------

InitHeapMem proc stdcall uses ebx ecx esi di,heapmeminbytes:dword
        cmp _DPMI_HEAP_MEM_HANDLE,0
        jne InitHeapMem_end
        AllocMemBlock heapmeminbytes
        jc InitHeapMem_end
        mov _DPMI_HEAP_MEM_HANDLE,esi
        mov HEAP_LIN_ADDR,ebx
        mov eax,heapmeminbytes
        mov HEAP_MEM_SIZE,eax
        mov eax,1
        ret
InitHeapMem_end:
        xor eax,eax
        ret
InitHeapMem endp

;--------------------------------
;FreeHeapMem
;       Procedura zwalnia pamiec heap
;       Wyjscie: eax=0 blad, eax=1 ok
;       Uwaga: Procedura powinna byc wywolana na koncu programu
;--------------------------------

FreeHeapMem proc stdcall uses esi di
        cmp _DPMI_HEAP_MEM_HANDLE,0
        je FreeHeapMem_end
        FreeMemBlock _DPMI_HEAP_MEM_HANDLE
        jc FreeHeapMem_end
        mov _DPMI_HEAP_MEM_HANDLE,0
        mov HEAP_LIN_ADDR,0
        mov HEAP_MEM_SIZE,0
        mov eax,1
        ret
FreeHeapMem_end:
        xor eax,eax
        ret
FreeHeapMem endp

;--------------------------------
;AllocMem (meminbytes : dword)
;       Alokuje meminbytes bajtow pamieci
;       Wejscie: meminbytes-ilosc pam
;       Wyjscie: eax-adres liniowy pam, jesli eax=NULL to blad
;--------------------------------

AllocMem proc stdcall uses edx ebx,meminbytes:dword
        ;wylicznenie potrzebnej pam. z uwzglednieniem wyrownania (HEAP_ALIGN)
        mov eax,meminbytes
        mov ebx,HEAP_ALIGN
        xor edx,edx
        div ebx
        mov eax,meminbytes
        add eax,edx
        ;sprawdzenie czy jest wystarczajaco duzo pam.
        mov ebx,eax
        add ebx,HEAP_PTR
        cmp ebx,HEAP_MEM_SIZE
        ja AllocMem_end
        ;alokacja pam i zwiekszenie wsk.
        mov ebx,eax             ;eax=meminbytes
        mov eax,HEAP_PTR
        add eax,HEAP_LIN_ADDR
        add HEAP_PTR,ebx
        jmp AllocMem_endok
AllocMem_end:
        xor eax,eax
        dec eax
AllocMem_endok:
        ret
AllocMem endp

ENDIF
