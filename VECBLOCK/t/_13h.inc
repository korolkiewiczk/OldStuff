.code

set13h proc C
        mov ax,13h
        int 10h
        ret
set13h endp

set3h proc C
        mov ax,3h
        int 10h
        ret
set3h endp

byte0a0000h proc C uses esi es,ad:dword,ci:dword
        mov es,stub32zerosel
        mov esi,ad
        mov eax,ci
        mov es:[esi+0a0000h],al
        ret
byte0a0000h endp
