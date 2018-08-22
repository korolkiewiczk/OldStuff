//PRC.INC - wywolywanie procedur z parametrami

.data

MAXPAR=31

public prc_parlist

prc_parlist dd MAXPAR dup (0)

.code

extern prc_parlist:dword


;wywoluje procedure
;prc_addr-adres procedury
;nparams-ilosc parametrow
;outtype-typ zwracanej danej

Prc_ExecProc proc C ,prc_addr:dword, nparams:dword, outtype:dword
        mov ecx,nparams
        jecxz Prc_ExecProc_call
        mov eax,ecx
        shl eax,2        
        add eax,offset prc_parlist        
Prc_ExecProc_loop:
                sub eax,4
                push [eax]                
                loop Prc_ExecProc_loop
Prc_ExecProc_call:
        mov eax,prc_addr
        call eax
        cmp outtype,1          ;czy typ float
        jne Prc_ExecProc_nofloat
        ;float
        fstp outtype
        mov eax,outtype
Prc_ExecProc_nofloat:
        mov ecx,nparams
        shl ecx,2         
        add esp,ecx
        ret
Prc_ExecProc endp
