IFNDEF __SPRINTF_INC
__SPRINTF_INC=1
;SPRINTF.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do formatowania tekstu

;includy

include aiconv.inc

;Funkcje

;--------------------------------
;SPrintf (buf,txt[,arg1])
;       Kopiuje do bufora tekst z uwzglednieniem formatowania
;       Wejscie: buf-bufor
;                txt-tekst
;                arg1-opcjonalne argumenty
;--------------------------------

__sprintf_t_buf db 32 dup (0)
__sprintf_copy_bf:
        lodsb
        or al,al
        jz __sprintf_copy_bf_end
        stosb
        jmp __sprintf_copy_bf
__sprintf_copy_bf_end:
        ret

__sprintf_fill__:
        push esi
        mov esi,offset __sprintf_t_buf
__sprintf_fill__count_loop__:
                mov al,[esi]
                dec ecx
                inc esi
                or al,al
                jnz __sprintf_fill__count_loop__
        pop esi
        inc ecx
        cmp ecx,0
        jnl __sprintf_fill__nzecx
        xor ecx,ecx
__sprintf_fill__nzecx:
        mov eax,edx
        rep stosb
        xor edx,edx                
        ret

SPrintf proc C uses edi esi edx ecx ebx eax,buf:dword,txt:dword,arg1:vararg
        mov edi,buf
        mov esi,txt
        lea ebx,arg1
        xor ecx,ecx
        xor edx,edx
        push ds
        pop es
SPrintf_main_loop:
                mov al,[esi]
                or al,al
                jz SPrintf_endp
                cmp al,'\'
                je SPrintf_case_back_slash
                cmp al,'%'
                je SPrintf_case_a
                mov [edi],al
                ;
                inc esi
                inc edi
                jmp SPrintf_main_loop
                ;
SPrintf_case_back_slash:
                mov ah,[esi+1]
                cmp ah,'n'
                je SPrintf_bs_n
                cmp ah,'t'
                je SPrintf_bs_t
                cmp ah,'\'
                je SPrintf_bs_bs
                inc esi
                jmp SPrintf_case_back_slash_end
SPrintf_bs_bs:
                mov [edi],al
                inc esi
                jmp SPrintf_case_back_slash_end
SPrintf_bs_t:
                inc esi
                mov eax,20202020h
                stosd
                stosd
                dec edi
                jmp SPrintf_case_back_slash_end
SPrintf_bs_n:
                inc esi
                mov byte ptr [edi],13
                inc edi
                mov byte ptr [edi],10
SPrintf_case_back_slash_end:
                inc esi
                inc edi
                jmp SPrintf_main_loop
                ;
SPrintf_case_a:
                inc esi
                mov ah,[esi]
                cmp ah,'u'
                je SPrintf_case_a_utoa
                cmp ah,'i'
                je SPrintf_case_a_itoa
                cmp ah,'x'
                je SPrintf_case_a_htoad
                cmp ah,'X'
                je SPrintf_case_a_htoau
                cmp ah,'s'
                je SPrintf_case_a_copystr
                cmp ah,'%'
                je SPrintf_case_a_p
                cmp ah,'0'
                je SPrintf_case_zeros
                jb SPrintf_case_a_d1
                cmp ah,'9'
                ja SPrintf_case_a_d1
                jmp SPrintf_case_integer
SPrintf_case_a_d1:
                jmp SPrintf_case_a_end
SPrintf_case_integer:
                push ' '
                mov ecx,offset __sprintf_t_buf
                jmp SPrintf_case_pad_l
SPrintf_case_zeros:
                push '0'
                inc esi
                mov ecx,offset __sprintf_t_buf
SPrintf_case_pad_l:
                mov ah,[esi]
                cmp ah,'0'
                jb SPrintf_case_pad
                cmp ah,'9'
                ja SPrintf_case_pad
                inc esi
                mov [ecx],ah
                inc ecx
                jmp SPrintf_case_pad_l
SPrintf_case_pad:
                mov byte ptr [ecx],0
                pop edx
                push offset __sprintf_t_buf
                call AToU
                mov ecx,eax
                dec esi
                jmp SPrintf_case_a
SPrintf_case_a_p:
                mov [edi],al
                inc edi
                jmp SPrintf_case_a_end
SPrintf_case_a_utoa:
                mov eax,ss:[ebx]
                push offset __sprintf_t_buf
                push eax
                call UToA
                or edx,edx
                jz SPrintf_case_a_utoa_nf
                call __sprintf_fill__
SPrintf_case_a_utoa_nf:
                push esi
                mov esi,offset __sprintf_t_buf
                call __sprintf_copy_bf
                pop esi
                add ebx,4
                jmp SPrintf_case_a_end
SPrintf_case_a_itoa:
                mov eax,ss:[ebx]
                push offset __sprintf_t_buf
                push eax
                call IToA
                or edx,edx
                jz SPrintf_case_a_itoa_nf
                call __sprintf_fill__
SPrintf_case_a_itoa_nf:
                push esi
                mov esi,offset __sprintf_t_buf
                call __sprintf_copy_bf
                pop esi
                add ebx,4
                jmp SPrintf_case_a_end
SPrintf_case_a_htoad:
                push 0
                jmp SPrintf_case_a_htoa
SPrintf_case_a_htoau:
                push 1
SPrintf_case_a_htoa:
                mov eax,ss:[ebx]
                push offset __sprintf_t_buf
                push eax
                call HToA
                or edx,edx
                jz SPrintf_case_a_htoa_nf
                call __sprintf_fill__
SPrintf_case_a_htoa_nf:
                push esi
                mov esi,offset __sprintf_t_buf
                call __sprintf_copy_bf
                pop esi
                add ebx,4
                jmp SPrintf_case_a_end
SPrintf_case_a_copystr:
                push esi
                mov esi,ss:[ebx]
                call __sprintf_copy_bf
                pop esi
                add ebx,4
SPrintf_case_a_end:
                inc esi
                jmp SPrintf_main_loop
SPrintf_endp:
        mov byte ptr [edi],0
        ret
SPrintf endp

ENDIF
