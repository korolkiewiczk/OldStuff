IFNDEF __FILEIO_INC
__FILEIO_INC=1
;FILEIO.INC for MASM/STUB32 by Kamil Korolkiewicz 2005
;Funkcje do obslugi plikow

;stale

READ_ONLY equ 0
WRITE_ONLY equ 1
READ_WRITE equ 2

OPEN_ONLY equ 0
OPEN_CREATE equ 1
CREATE_ONLY equ 2

ORGIN_START equ 0
ORGIN_CURR_POS equ 1
ORGIN_EOF equ 2

FILEIO_RW_BUF_SIZE=256

;funkcje

;--------------------------------
;FOpen (filename:addr,rout:byte,access:byte)
;       Otwiera/tworzy plik do zapisu i odczytu
;       Wejscie: filename-nazwa pliku (ASCIIZ)
;                rout-procedura do wykonania:
;                       0-Otw¢rz plik
;                       1-Otw¢rz plik, je˜li nie odnaleziony stw¢rz
;                       2-Stw¢rz plik
;                access-dostep do pliku
;                       0-tylko czytanie
;                       1-tylko zapis
;                       2-czytanie/zapis
;       Wyjscie: je˜li cf=1 blad (ax-numer bledu), w przeciwnym wypadku:
;                ax-uchwyt pliku
;       Uwaga: filename (offset do nazwy pliku) musi byc mniejszy lub rowny 0ffffh
;--------------------------------

FOpen proc stdcall uses edx cx,filename:dword,rout:byte,access:byte
        cmp rout,0
        jne FOpen_crout1
        mov ah,3dh
        mov al,access
        mov edx,filename
        int 21h
        push stub32inthandrec.__flg
        popf
        ret
FOpen_crout1:
        cmp rout,1
        jne FOpen_crout2
        mov ah,3dh
        mov al,access
        mov edx,filename
        int 21h
        push stub32inthandrec.__flg
        popf
        jc FOpen_crout2
        ret
FOpen_crout2:
        mov ah,3ch
        mov al,access
        xor cx,cx
        mov edx,filename
        int 21h
        push stub32inthandrec.__flg
        popf
        ret
FOpen endp

;--------------------------------
;FClose (handle:word)
;       Zamyka plik
;       Wejscie: handle-uchwyt pliku
;       Wyjscie: jesli cf=1 to blad
;--------------------------------

FClose proc stdcall uses bx,handle:word
        mov ax,3e00h
        mov bx,handle
        int 21h
        push stub32inthandrec.__flg
        popf
        ret
FClose endp

;--------------------------------
;FRead (handle:word,rbytes:word,bufofs:dword)
;       Czyta z pliku dane do bufora
;       Wejscie: handle-uchwyt do pliku
;                rbytes-liczba bajtow do wczytania
;                bufofs-offset do bufora
;       Wyjscie: jesli cf=1 to jest blad, w przeciwnym wypadku:
;                ax-liczba faktycznie wczytanych bajtow
;       Uwaga: bufofs (offset do bufora) musi byc mniejszy lub rowny 0ffffh
;              Uzywany jest segment danych z trybu rzeczywistego
;--------------------------------

FRead proc stdcall uses edx cx bx,handle:word,rbytes:word,bufofs:dword
        mov ax,3f00h
        mov bx,handle
        mov cx,rbytes
        mov edx,bufofs
        int 21h
        push stub32inthandrec.__flg
        popf
        ret
FRead endp

;--------------------------------
;EFRead (handle:word,rbytes:dword,bufofs:dword)
;       Czyta z pliku dane do bufora umieszczonego w pamieci heap
;       Wejscie: handle-uchwyt do pliku
;                rbytes-liczba bajtow do wczytania
;                bufofs-wskaznik do bufora
;       Wyjscie: jesli cf=1 to jest blad, w przeciwnym wypadku:
;                eax-liczba faktycznie wczytanych bajtow
;--------------------------------

EFRead proc stdcall uses edi esi edx ecx ebx,handle:word,rbytes:dword,bufofs:dword
        local FILEIO_R_BUF [FILEIO_RW_BUF_SIZE] : byte  ;bufor pomocniczy
        local rb:dword                                  ;licznik wczytanych bajtow
        ;
        mov rb,0
        mov eax,rbytes                  ;ile powtorzen
        mov ebx,FILEIO_RW_BUF_SIZE
        xor edx,edx
        div ebx
        ;
        push stub32saveds               ;zapisz stub32saveds
        push edx
        mov ecx,eax
        mov bx,handle
        mov edx,ebp
        sub edx,FILEIO_RW_BUF_SIZE
        mov ax,stub32zerosel
        mov es,ax
        mov ax,stub32savess
        mov stub32saveds,ax
        ;
        jecxz EFread_read_last_bytes
        ;
        xor eax,eax
EFRead_read_bytes:
                push ecx
                mov ax,3f00h
                mov cx,FILEIO_RW_BUF_SIZE
                int 21h
                add rb,eax
                ;
                push stub32inthandrec.__flg
                popf
                jc EFread_end_pop

                ;kopiowanie danych ze stosu do bufora
                push ds
                push ss
                pop ds
                mov esi,edx
                mov edi,bufofs
                mov ecx,FILEIO_RW_BUF_SIZE/4
                rep movsd
                pop ds

                ;zwiekszenie wskaznika bufora
                add bufofs,FILEIO_RW_BUF_SIZE
                ;
                pop ecx
                loop EFread_read_bytes
EFRead_read_last_bytes:
        pop ecx                         ;edx
        jecxz EFread_end
        mov ax,3f00h
        int 21h
        ;
        ;kopiowanie danych ze stosu do bufora
        push ds
        push ss
        pop ds
        mov esi,edx
        mov edi,bufofs
        rep movsb
        pop ds

        push stub32inthandrec.__flg
        popf
        add rb,eax
        jmp EFread_end
EFRead_end_pop:
        add esp,4
EFRead_end:
        pop stub32saveds
        mov eax,rb
        ret
EFRead endp

;--------------------------------
;FWrite (handle:word,wbytes:word,bufofs:dword)
;       Zapisuje do pliku dane do bufora
;       Wejscie: handle-uchwyt do pliku
;                wbytes-liczba bajtow do zapisu
;                bufofs-offset do bufora
;       Wyjscie: jesli cf=1 to jest blad, w przeciwnym wypadku:
;                ax-liczba faktycznie zapisanych bajtow
;       Uwaga: bufofs (offset do bufora) musi byc mniejszy lub rowny 0ffffh
;              Uzywany jest segment danych z trybu rzeczywistego
;--------------------------------

FWrite proc stdcall uses edx cx bx,handle:word,wbytes:word,bufofs:dword
        mov ax,4000h
        mov bx,handle
        mov cx,wbytes
        mov edx,bufofs
        int 21h
        push stub32inthandrec.__flg
        popf
        ret
FWrite endp

;--------------------------------
;EFWrite (handle:word,rbytes:dword,bufofs:dword)
;       Zapisuje do pliku dane z bufora umieszczonego w pamieci heap
;       Wejscie: handle-uchwyt do pliku
;                rbytes-liczba bajtow do zapisu
;                bufofs-wskaznik do bufora
;       Wyjscie: jesli cf=1 to jest blad, w przeciwnym wypadku:
;                eax-liczba faktycznie zapisanych bajtow
;--------------------------------

EFWrite proc stdcall uses edi esi edx ecx ebx,handle:word,rbytes:dword,bufofs:dword
        local FILEIO_R_BUF [FILEIO_RW_BUF_SIZE] : byte  ;bufor pomocniczy
        local rb:dword                                  ;licznik wczytanych bajtow
        ;
        mov rb,0
        mov eax,rbytes                  ;ile powtorzen
        mov ebx,FILEIO_RW_BUF_SIZE
        xor edx,edx
        div ebx
        ;
        push stub32saveds               ;zapisz stub32saveds
        push edx
        mov ecx,eax
        mov bx,handle
        mov edx,ebp
        sub edx,FILEIO_RW_BUF_SIZE
        mov ax,stub32savess
        mov stub32saveds,ax
        ;
        jecxz EFWrite_read_last_bytes
        ;
        xor eax,eax
EFWrite_read_bytes:
                ;kopiowanie danych z bufora do stosu
                push ecx
                push ds
                mov ds,stub32zerosel
                push ss
                pop es
                mov edi,edx
                mov esi,bufofs
                mov ecx,FILEIO_RW_BUF_SIZE/4
                rep movsd
                pop ds
                pop ecx
                
                push ecx
                mov ax,4000h
                mov cx,FILEIO_RW_BUF_SIZE
                int 21h
                add rb,eax
                ;
                push stub32inthandrec.__flg
                popf
                jc EFWrite_end_pop

                ;zwiekszenie wskaznika bufora
                add bufofs,FILEIO_RW_BUF_SIZE
                ;
                pop ecx
                loop EFWrite_read_bytes
EFWrite_read_last_bytes:
        pop ecx                         ;edx
        jecxz EFWrite_end
        ;kopiowanie danych z bufora do stosu
        push ecx
        push ds
        mov ds,stub32zerosel
        push ss
        pop es
        mov edi,edx
        mov esi,bufofs
        rep movsb
        pop ds
        pop ecx

        mov ax,4000h
        int 21h
        ;
        push stub32inthandrec.__flg
        popf
        add rb,eax
        jmp EFWrite_end
EFWrite_end_pop:
        add esp,4
EFWrite_end:
        pop stub32saveds
        mov eax,rb
        ret
EFWrite endp


;--------------------------------
;FSeek (handle:word,position:dword,orgin:byte)
;       Ustawia aktualna pozycje w pliku
;       Wejscie: handle-uchwyt pliku
;                position-pozycja
;                orgin-miesce od ktorego jest liczona pozycja:
;                       0-poczatek pliku
;                       1-aktualna pozycja
;                       2-koniec pliku
;       Wyjscie: jesli cf=1 to blad, jesli nie:
;                eax-nowa pozycja
;--------------------------------

FSeek proc stdcall uses edx ecx bx,handle:word,position:dword,orgin:byte
        mov ah,42h
        mov al,orgin
        mov bx,handle
        mov edx,position
        mov ecx,edx
        shr ecx,16
        int 21h
        push ax
        mov ax,dx
        shl eax,16
        pop ax
        push stub32inthandrec.__flg
        popf
        ret
FSeek endp

;--------------------------------
;FPos (handle:word)
;       Zwraca aktualna pozycje pliku
;       Wejscie: handle-uchwyt pliku
;       Wyjscie: eax-aktualna pozycja
;--------------------------------

FPos proc stdcall uses dx cx bx,handle:word
        mov ax,4201h
        mov bx,handle
        xor dx,dx
        xor cx,cx
        int 21h
        push ax
        mov ax,dx
        shl eax,16
        pop ax
        ret
FPos endp

;--------------------------------
;FSize (handle:word)
;       Zwraca wielkosc pliku
;       Wejscie: handle-uchwyt pliku
;       Wyjscie: eax-wielkosc pozycja
;--------------------------------

FSize proc stdcall uses dx cx bx,handle:word
        local p:dword
        invoke FPos,handle
        mov p,eax
        mov ax,4202h
        mov bx,handle
        xor dx,dx
        xor cx,cx
        int 21h
        push eax
        invoke FSeek,handle,p,ORGIN_START
        pop eax
        ret
FSize endp

ENDIF
