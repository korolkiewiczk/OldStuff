;  X86 EMULATOR by Kamil Korolkiewicz 2007
;  Plik: rdtsc.asm
;  Opis: Obsluga funkcji procesora [rdtsc] do pobierania zegara CPU

.code

;long long _rdtsc(long long*)
_rdtsc proc uses edi, t:dword
.586p
    rdtsc
.386    
    mov edi,t
    or edi,edi
    jz @f
    mov [edi],eax
    mov [edi+4],edx
@@:
    ret

_rdtsc endp