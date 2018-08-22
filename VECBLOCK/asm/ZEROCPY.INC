;zerocpy.inc
.code

;kopiuje 'n' bajtow pamieci z 's' do 'd', ale d jest adresem fizycznym
_cpyd0 proc C uses esi edi es,d:dword,s:dword,n:dword
        cli
        cld
        mov es,stub32zerosel
        mov esi,s
        mov edi,d
        mov eax,n
        mov ecx,eax
        shr ecx,2
        and eax,3
        rep movsd
        mov ecx,eax
        rep movsb
        sti
        ret
_cpyd0 endp

;kopiuje 'n' bajtow pamieci z 's' do 'd', ale s jest adresem fizycznym
_cpys0 proc C uses esi edi ds,d:dword,s:dword,n:dword
        cli
        cld
        mov ds,stub32zerosel
        mov esi,s
        mov edi,d
        mov eax,n
        mov ecx,eax
        shr ecx,2
        and eax,3
        rep movsd
        mov ecx,eax
        rep movsb
        sti
        ret
_cpys0 endp

;wypelnia 'n' bajtow pamieci wartoscia 'v' pamiec pod adresem fizycznym 'm'
_fill0 proc C uses edi es,m:dword,n:dword,v:dword
        cli
        cld
        mov es,stub32zerosel
        mov edi,m
        mov edx,n
        mov eax,v
        mov ecx,edx
        shr ecx,2
        and edx,3
        rep stosd
        mov ecx,edx
        rep stosb
        sti
        ret
_fill0 endp
