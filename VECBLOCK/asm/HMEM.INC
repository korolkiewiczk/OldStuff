;HMEM.INC for DJGPP/DPMI by Kamil Korolkiewicz 2005/2006
;Funkcje do oblugi pamieci typu heap
;Jesli ma byc generowana funkcja bledu gdy wystapi blad w czasie alokacji pamieci
;to nalezy przypisac zmiennej __HMEM_is_generate_error wartosc 1

.data

public _DPMI_HEAP_MEM_HANDLE,HEAP_LIN_ADDR

_DPMI_HEAP_MEM_HANDLE dd 0

HEAP_LIN_ADDR dd 0

.code

extern _DPMI_HEAP_MEM_HANDLE:dword,HEAP_LIN_ADDR:dword

;--------------------------------
;InitHeapMem (int)
;       Initializuje pamiec heap.
;       Wejscie: heapmeminbytes-ilosc pamieci heap
;       Wyjscie: eax=0 blad, eax=1 ok
;       Uwaga: Ta procedura musi byc wywolana przed kazda inna procedura do obslugi pamieci
;--------------------------------

InitHeapMem proc C uses ebx esi edi,heapmeminbytes:dword
        cmp _DPMI_HEAP_MEM_HANDLE,0
        jnz InitHeapMem_end

        AllocMemBlock heapmeminbytes
        jc InitHeapMem_end

        mov _DPMI_HEAP_MEM_HANDLE,esi
        mov HEAP_LIN_ADDR,ebx
        GetSelBA ds
        sub HEAP_LIN_ADDR,ecx

        xor eax,eax        
        inc eax
        jmp InitHeapMem_ret

InitHeapMem_end:
        xor eax,eax
InitHeapMem_ret:        
        ret
InitHeapMem endp

;--------------------------------
;FreeHeapMem ()
;       Procedura zwalnia pamiec heap
;       Wyjscie: eax=0 blad, eax=1 ok
;       Uwaga: Procedura powinna byc wywolana na koncu programu
;--------------------------------

FreeHeapMem proc C uses esi edi
        FreeMemBlock _DPMI_HEAP_MEM_HANDLE
        jc FreeHeapMem_end
        xor eax,eax
        inc eax
        ret
FreeHeapMem_end:
        xor eax,eax
        ret
FreeHeapMem endp

end
