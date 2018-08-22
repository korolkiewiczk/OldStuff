IFNDEF __FASTWIN_INC
__FASTWIN_INC=1
;fastwin.inc
;Biblioteka ulatwiajaca obsluge aplikacji WIN32.

;============================================================================

;includy

.nolist
.nocref
include c:\masm32\include\windows.inc
include c:\masm32\include\kernel32.inc
include c:\masm32\include\user32.inc
include c:\masm32\include\gdi32.inc
.cref
.list

includelib c:\masm32\lib\kernel32.lib
includelib c:\masm32\lib\user32.lib
includelib c:\masm32\lib\gdi32.lib

;============================================================================
;Tutaj znajduja sie zmienne gdzie przechowywane sa informacje o procesorze.


.data

        cpu_name db 12 dup (0)
        db 0
        cpu_ver dd 0
        cpu_brand dd 0
        cpu_feat dd 0
        cpu_efeat dd 0


;============================================================================

;--------------------------------
;WinInit - macro wywolujace funkcje glowna programu: WinMain oraz pobiera
;          informacje o procesorze. Funkcje ta nalezy zadeklarowac zgodnie
;          z prototypem ponizej. Na jej koncu wystarczy umiescic instrukcje
;          ret. Dodatkowo ponizej znajdziemy prototyp procedury okna: WndProc.
;--------------------------------

WinMain proto stdcall :dword, :dword, :dword, :dword
WndProc proto stdcall :dword, :dword, :dword, :dword

WinInit macro

        ;Pobranie typu procesora i informacji o nim

        pushfd
        pop eax
        or eax,200000h
        push eax
        popfd
        pushfd
        pop eax
        and eax,200000h
        jz @f
        
        .586p
        xor eax,eax
        cpuid
        mov dword ptr cpu_name,ebx
        mov dword ptr cpu_name+4,edx
        mov dword ptr cpu_name+8,ecx
        
        xor eax,eax
        inc eax
        cpuid
        mov cpu_ver,eax
        mov cpu_brand,ebx
        mov cpu_efeat,ecx
        mov cpu_feat,edx
@@:
        .386p        

        ;ecx=GetModuleHandle (NULL);
        invoke GetModuleHandle, NULL
        push eax
        ;eax=GetCommandLine ();
        call GetCommandLine
        pop ecx
        invoke WinMain,ecx,NULL,eax,NULL
        invoke ExitProcess,NULL
endm

;============================================================================

.code

;--------------------------------
;CreateWin (
;           HInst:dword,WName:dword,WStyle:dword,WinPr:dword,
;           WinIcon:dword,WinCur:dword,WinBG:dword,
;           WinX:dword,WinY:dword,WinW:dword,WinH:dword);
;       Tworzy okno
;       Wejscie:
;               HInst   - uchwyt aplikacji
;               WName   - nazwa okna
;               WStyle  - styl okna
;               WinPr   - procedura okna
;               WinIcon - ikonka okna
;               WinCur  - kursor okna
;               WinBG   - kolor okna
;               WinX    - pozycja x okna
;               WinY    - pozycja y okna
;               WinW    - Szerokosc okna
;               WinH    - Wysokosc okna
;       Wyjscie: eax-uchwyt okna
;--------------------------------

CreateWin proc stdcall,                                                  \
HInst:dword,WName:dword,WStyle:dword,WinPr:dword,                        \
WinIcon:dword,WinCur:dword,WinBG:dword,                                  \
WinX:dword,WinY:dword,WinW:dword,WinH:dword

        local WinClass:WNDCLASSEX

        mov WinClass.cbSize,sizeof(WNDCLASSEX)
        mov WinClass.style,CS_HREDRAW or CS_VREDRAW
        mov eax,WinPr
        mov WinClass.lpfnWndProc,eax
        mov WinClass.cbClsExtra,0
        mov WinClass.cbWndExtra,0
        mov eax,HInst
        mov WinClass.hInstance,eax
        mov eax,WinIcon
        or eax,eax
        jnz cwd1
        invoke LoadIcon,NULL,IDI_WINLOGO
        jmp cwqd1
cwd1:
        invoke LoadIcon,HInst,eax
cwqd1:
        mov WinClass.hIcon,eax
        mov WinClass.hIconSm,eax

        mov eax,WinCur
        or eax,eax
        jnz cwd2
        invoke LoadCursor,NULL,IDC_ARROW
        jmp cwqd2
cwd2:
        invoke LoadCursor,HInst,eax
cwqd2:
        mov WinClass.hCursor,eax

        mov eax,WinBG
        mov WinClass.hbrBackground,eax
        mov WinClass.lpszMenuName,NULL
        mov eax,WName
        mov WinClass.lpszClassName,eax
        invoke RegisterClassEx,addr WinClass

        invoke CreateWindowEx,NULL,WName,WName,WStyle,WinX,WinY,
                              WinW,WinH,NULL,NULL,HInst,NULL

        push eax
        invoke ShowWindow,eax,SW_SHOW
        pop eax
        push eax
        invoke UpdateWindow,eax
        pop eax

        ret
CreateWin endp

;--------------------------------
;GetMsg(msgs:dword)
;       Pobiera wiadomosci za pomoca GetMessage
;       Wejscie: msgs - adres do struktury msg
;       Wyjscie: eax=0-wiadomosc WM_QUIT / eax=-1-blad
;--------------------------------

GetMsg proc stdcall,msgs:dword
        invoke GetMessage,msgs,NULL,0,0
        or eax,eax
        jz gmendp
        cmp eax,-1
        jz gmendp
        invoke TranslateMessage,msgs
        invoke DispatchMessage,msgs
        mov eax,1
gmendp:
        ret
GetMsg endp

;--------------------------------
;PeekMsg(msgs:dword)
;       Pobiera wiadomosci za pomoca PeekMessage
;       Wejscie: msgs - adres do struktury msg
;       Wyjscie: eax=0-nie ma wiadomosci / eax=1-jest wiadomosc / eax=-1-blad
;--------------------------------

PeekMsg proc stdcall,msgs:dword
        invoke PeekMessage,msgs,NULL,0,0,PM_REMOVE
        or eax,eax
        jz gmendp
        cmp eax,-1
        jz gmendp
        invoke TranslateMessage,msgs
        invoke DispatchMessage,msgs
        mov eax,1
gmendp:
        ret
PeekMsg endp

ENDIF
