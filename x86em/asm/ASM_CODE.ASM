.386
.model flat,C

.code


;rdtsc.asm

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

;bt.asm

;zapis n bajtow
_memsetb proc uses esi ,d:dword,a:dword,n:dword
    movzx eax,byte ptr a
    mov ah,al
    movzx ecx,ax
    shl eax,8
    or eax,ecx
    mov ecx,n
    mov edx,ecx
    mov esi,d
    shr ecx,2
    and edx,3
    rep stosd
    mov ecx,edx
    rep stosb
    ret
_memsetb endp

;zapis n slow
_memsetw proc uses esi ,d:dword,a:dword,n:dword
    movzx eax,word ptr a
    movzx ecx,ax
    shl eax,8
    or eax,ecx
    mov ecx,n
    mov edx,ecx
    mov esi,d
    shr ecx,1
    and edx,1
    rep stosd
    mov ecx,edx
    rep stosw
    ret
_memsetw endp

;zapis n podwojnych slow
_memsetd proc uses esi ,d:dword,a:dword,n:dword
    mov eax,a
    mov ecx,n
    mov esi,d
    rep stosd
    ret
_memsetd endp


;kopia n bajtow
_memcpyb proc uses esi edi ,d:dword,s:dword,n:dword
    mov ecx,n
    mov edx,ecx
    mov esi,d
    mov edi,s
    shr ecx,2
    and edx,3
    rep movsd
    mov ecx,edx
    rep movsb
    ret
_memcpyb endp

;kopia n slow
_memcpyw proc uses esi edi ,d:dword,s:dword,n:dword
    mov ecx,n
    mov edx,ecx
    mov esi,d
    mov edi,s
    shr ecx,1
    and edx,1
    rep movsd
    mov ecx,edx
    rep movsw
    ret
_memcpyw endp

;zapis n podwojnych slow
_memcpyd proc uses esi ,d:dword,s:dword,n:dword
    mov ecx,n
    mov esi,d
    mov edi,s
    rep movsd
    ret
_memcpyd endp
    
end
