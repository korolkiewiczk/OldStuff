;DPMI.INC - makra do obslugi DPMI

DPMI_MEM_INFO struct
        Max_Avail_Block dd ?
        Max_Unlocked_Page_Alloc dd ?
        Max_Locked_Page_Alloc dd ?
        Tot_Lin_Addr_Space dd ?
        Tot_Unlocked_Pages dd ?
        Free_Pages dd ?
        Tot_Phys_Pages dd ?
        Free_Lin_Addr_Space dd ?
        Size_Of_Paging_File_Part dd ?
        DPMI_MEM_INFO_RES db 12 dup (?)
DPMI_MEM_INFO ends

DPMI_RMCS struct
        __edi dd ?
        __esi dd ?
        __ebp dd ?
        __res dd ?
        __ebx dd ?
        __edx dd ?
        __ecx dd ?
        __eax dd ?
        __flg dw ?
        __es dw ?
        __ds dw ?
        __fs dw ?
        __gs dw ?
        __ip dw ?
        __cs dw ?
        __sp dw ?
        __ss dw ?
DPMI_RMCS ends

;makra
;Makra zwracaja blad jesli cf=1.

;--------------------------------
;AllocLDTDesc nsel
;       Alokuje deskryptory ldt
;       Uzywane rejestry: ax,cx
;       Wejscie: nsel - liczba deskryptorow
;       Wyjscie: ax - selektor bazowy
;--------------------------------

AllocLDTDesc macro nsel
        xor ax,ax
        mov cx,nsel
        int 31h
endm

;--------------------------------
;FreeLDTDesc sel
;       Uzywane rejestry: ax,bx
;       Zwalnia selektor
;       Wejscie: sel - selektor
;--------------------------------

FreeLDTDesc macro
        xor ax,ax
        inc ax
        mov bx,sel
        int 31h
endm

;--------------------------------
;Seg2Desc seg
;       Zamienia selektor trybu rzeczywistego na selektor
;       Uzywane rejestry: ax,bx
;       Wejscie: seg - segment (np. A000)
;       Wyjscie: ax - selektor
;--------------------------------

Seg2Desc macro seg
        mov ax,0002h
        mov bx,seg
        int 31h
endm

;--------------------------------
;GetSelIncVal
;       Pobiera roznice numerow selektorow
;       Uzywane rejestry: ax
;       wyjscie: ax - roznica numerow selektorow
;--------------------------------

GetSelIncVal macro
        mov ax,0003h
        int 31h
endm

;--------------------------------
;GetSelBA sel
;       Pobiera adres bazowy selektora
;       Uzywane rejestry: ax,bx,dx
;       Wejscie: sel    - selektor
;       Wyjscie: ecx
;--------------------------------

GetSelBA macro sel
        mov ax,0006h
        mov bx,sel
        int 31h
        shl ecx,16
        mov cx,dx
endm

;--------------------------------
;SetSelBA sel,BA
;       Ustawia adres bazowy selektora
;       Uzywane rejestry: ax,bx,edx,ecx
;       Wejscie: sel - selektor
;                BA - adres liniowy
;--------------------------------
        
SetSelBA macro sel,BA
        mov ax,0007h
        mov bx,sel
        mov edx,BA
        mov ecx,edx
        shr ecx,16
        int 31h
endm

;--------------------------------
;SetSelLim sel,lim
;       Ustawia limit selektora
;       Uzywane rejestry: ax,bx,edx,ecx
;       Wejscie: sel - selektor
;                lim - limit
;--------------------------------

SetSelLim macro sel,lim
        mov ax,0008h
        mov bx,sel
        mov edx,lim
        mov ecx,lim
        shr ecx,16
        int 31h
endm

;--------------------------------
;AllocDosMem mems
;       Alokuje pamiec typu DOS
;       Uzywane rejestry: ax,ebx
;       Wejscie: mems - ilosc pamieci w bajtach
;       Wyjscie: ax - DOS'owy segment
;                dx - Selektor
;--------------------------------

AllocDosMem macro mems
        mov ax,0100h
        mov ebx,mems
        shr ebx,4
        int 31h
endm

;--------------------------------
;FreeDosMem sel
;       Zwalnia pamiec typu DOS
;       Uzywane rejestry ax,dx
;       Wejscie: sel - selektor
;--------------------------------

FreeDosMem macro sel
        mov ax,0101h
        mov dx,sel
        int 31h
endm

;--------------------------------
;ResizeDosMem sel,mems
;       Przydziela nowa ilosc pam. typu DOS
;       Uzywane rejestry ax,dx,ebx
;       Wejscie: sel - selektor
;                mems - ilosc pamieci w bajtach
;--------------------------------

ResizeDosMem macro sel,mems
        mov ax,0102h
        mov ebx,mems
        shr ebx,4
        mov dx,sel
        int 31h
endm

;--------------------------------
;GetRMIntVec intn
;       Pobiera wektor przerwan trybu rzeczywistego
;       Uzywane rejestry: ax,bl
;       Wejscie: intn - numer przerwania
;       Wyjscie: cx:dx - seg:ofs przerwania
;--------------------------------

GetRMIntVec macro intn
        mov ax,0200h
        mov bl,intn
        int 31h
endm

;--------------------------------
;SetRMIntVec intn,iseg,iofs
;       Ustawia wektor przerwan trybu rzeczywistego
;       Uzywane rejestry: ax,bl,cx,dx
;       Wejscie: intn - numer przerwania
;                iseg - seg do wektora
;                iofs - ofs do wektora
;--------------------------------

SetRMIntVec macro intn,iseg,iofs
        mov ax,0201h
        mov bl,intn
        mov cx,iseg
        mov dx,iofs
        int 31h
endm

;--------------------------------
;GetIntVec intn
;       Pobiera wektor przerwan trybu chronionego
;       Uzywane rejestry: ax,bl
;       Wejscie: intn - numer przerwania
;       Wyjscie: cx:edx - sel:ofs wektora
;--------------------------------

GetIntVec macro intn
        mov ax,0204h
        mov bl,intn
        int 31h
endm

;--------------------------------
;SetIntVec intn,isel,iofs
;       Ustawia wektor przerwan trybu chronionego
;       Uzywane rejestry: ax,bl,cx,edx
;       Wejscie: intn - numer przerwania
;                isel - selektor wektora
;                iofs - ofset wektora
;--------------------------------

SetIntVec macro intn,isel,iofs
        mov ax,0205h
        mov bl,intn
        mov cx,isel
        mov edx,iofs
        int 31h
endm

;--------------------------------
;SimulateRMInt intn,RMCSofs
;       Symuluje przerwanie trybu rzeczywistego
;       Uzywane rejestry: ax,bx,cx,edx
;       Wejscie: intn - numer przerwania
;                RMCSofs - ofset do struktory DPMI_RMCS
;       Wyjscie: zmieniona struktora DPMI_RMCS
;--------------------------------

SimulateRMInt macro intn,RMCSofs
        mov ax,0300h
        mov bl,intn
        xor bh,bh
        mov cx,10
        mov edx,offset RMCSofs
        int 31h
endm

;--------------------------------
;GetMemInfo DMIofs
;       Pobiera informacje o pamieci
;       Uzywane rejestry: ax,es,edi
;       Wejscie: DMIofs - offset do strukory DPMI_MEM_INFO
;       Wyjscie: zmieniona struktora
;       Uwaga: rejestr ds musi wskazywac na dane
;--------------------------------

GetMemInfo macro DMIofs
        mov ax,ds
        mov es,ax
        mov ax,0500h
        mov edi,DMIofs
        int 31h
endm

;--------------------------------
;AllocMemBlock mems
;       Alokuje pamiec DPMI
;       Uzyte rejestry: ax,ecx,di
;       Wejscie: mems - ilosc pam.
;       Wyjscie: ebx - liniowy adres pamieci
;                esi - uchwyt do pamieci
;--------------------------------

AllocMemBlock macro mems
        mov ax,0501h
        mov ebx,mems
        mov cx,bx
        shr ebx,16
        int 31h
        pushf
        shl ebx,16
        mov bx,cx
        shl esi,16
        mov si,di
        popf
endm

;--------------------------------
;FreeMemBlock handl
;       Zwalnia pamiec DPMI
;       Uzywane rejesty: ax,esi,di
;       Wejscie: handl - uchwyt do pam.
;--------------------------------

FreeMemBlock macro handl
        mov ax,0502h
        mov esi,handl
        mov di,si
        shr esi,16
        int 31h
endm

;--------------------------------
;ResizeMemBlock handl,mems
;       Realokuje pamiec DPMI
;       Uzyte rejestry: ax,esi,di
;       Wejscie: handl - uchwyt do pam.
;                mems - ilosc pam.
;       Wyjscie: ebx - nowy liniowy adres pamieci
;                ecx - nowy uchwyt do pamieci
;--------------------------------

ResizeMemBlock macro handl,mems
        mov ax,0503h
        mov ebx,mems
        mov cx,bx
        shr ebx,16
        mov esi,handl
        mov di,si
        shr esi,16
        int 31h
        pushf
        shl ebx,16
        mov bx,cx
        shl esi,16
        mov si,di
        popf
endm

;--------------------------------
;MapPhysAddr physaddr,mems
;       Mapuje pamiec psychiczna na liniowa
;       Uzywane rejestry: eax,ecx,esi,edi
;       Wejscie: physaddr - adres psychiczny pamieci
;                mems - ilosc pam.
;       Wyjscie: ebx - liniowy adres mapowanej pamieci
;--------------------------------

MapPhysAddr macro physaddr,mems
        mov ax,0800h
        mov ecx,physaddr
        mov ebx,ecx
        shr ebx,16
        mov edi,mems
        mov esi,edi
        shr esi,16
        int 31h
        shl ebx,16
        mov bx,cx
endm

;--------------------------------
;FreePhysAddrMap linaddr
;       Uzywane rejestry: ax,ebx,ecx
;       Zwalnia mapowana pamiec psychiczna
;       Wejscie: linaddr - liniowy adres mapowanej pamieci
;--------------------------------

FreePhysAddrMap macro linaddr
        mov ax,0801h
        mov ecx,linaddr
        mov ebx,ecx
        shr ebx,16
        int 31h
endm
