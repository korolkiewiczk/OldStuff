;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
;MLAB copyright (c) Kamil Korolkiewicz 2005
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
.386
.listall

public main

stub32 segment public
assume cs:stub32,ds:stub32

;dolaczane pliki

include dpmi.inc
include main.inc
include mlab_game.asm
include mlab_g.asm

;funkcje

SysError proc stdcall,msg:dword
        SetTxtMode
        invoke Print,0,0,msg
        call FreeHeapMem
        mov ax,4c00h
        int 21h
SysError endp

BackScr2Scr proc stdcall uses eax ebx ecx edx esi edi
        cmp islfb,0
        jz bs2sd1
        mov esi,offset backscrsurface
        mov edi,offset scrsurface
        call CopySurface
        ret
bs2sd1:
        mov eax,scrsx*scrsy
        mov esi,backscrsurface.SBits
        mov es,stub32zerosel
        xor edx,edx
        mov ebx,65536
        div ebx
        xor ebx,ebx
        or eax,eax
        jz bs2sd3
        push edx
bs2sl1:
        push eax
        push ebx
        mov ax,4f05h
        mov edx,ebx
        xor ebx,ebx
        int 10h
        pop ebx
        pop eax
        mov ecx,65536/4
        mov edi,scrsurface.SBits
        push ds
        mov ds,stub32zerosel
        rep movsd
        pop ds
        inc ebx
        dec eax
        jnz bs2sl1
        pop edx
bs2sd3:
        mov ecx,edx
        mov eax,4f05h
        mov edx,ebx
        xor ebx,ebx
        int 10h
        mov ebx,ecx
        mov edi,scrsurface.SBits
        shr ecx,2
        and ebx,3
        push ds
        mov ds,stub32zerosel
        rep movsd
        mov ecx,ebx
        rep movsb
        pop ds

        ret        
BackScr2Scr endp

;makra

SError macro r:req,t:req,m:req
        local txt
        local l
        cmp r,t
        jne l
        jmp @f
        txt db m
        db 0
@@:
        invoke SysError,addr txt
l:        
endm

Printt macro x:req,y:req,t:req
        local txt
        jmp @f
        txt db t
        db 0
@@:
        invoke Print,x,y,addr txt
endm

SPrintt macro x:req,y:req,t:req,p:vararg
        local txt
        jmp @f
        txt db t
        db 0
@@:
        invoke SPrintf,addr tbuf1,addr txt,p
        invoke Print,x,y,addr tbuf1
endm
        

main:

;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        call Cls
        Printt 0,0,'Initializing...'

        mov eax,[cpu_name]
        or eax,eax
        SError eax,0,'CPU is not Pentium'

        finit

        ;inicjacja pamieci
        invoke InitHeapMem,1024*1024*4          ;4MB
        SError eax,0,'No available PM memory (4MB)'
        mov __MMEM_is_generate_error,1

        ;inicjacja klawiatury
        call InitKB

        ;inicjacja czasu
        call InitTime
        mov eax,dword ptr __CPU__TSC__Freq
        mov edx,dword ptr __CPU__TSC__Freq+4
        mov ebx,1000000
        div ebx
        mov ebx,eax
        SPrintt 0,1,'CPU clock:%u MHZ',ebx

        call GetLabParams

        ;ladowanie pliku
        Printt 0,2,'Loading gfx...'
        invoke LoadBMP8,addr gfxfname,addr gfxsurface
        SError eax,0,'Can not load g.bmp'

        invoke AllocMem,768
        mov pal,eax
        invoke LoadPalFromBMP,addr gfxfname,pal

        call LoadPlayerList

        ;pobieranie LFB
        mov cx,scrmode
        call GetLFB
        cmp eax,-1
        je nolfb
        mov islfb,1
        mov scrsurface.SBits,eax
nolfb:

        mov eax,offset backscrsurface
        call AllocSurface

        mov eax,offset backsurface
        call AllocSurface

        SetVESAMode scrmode,0100000000000000b
        ;SetVGAMode

        call InitFAMem

        invoke SetPalette,pal

        invoke InitBackSurface,110,0

;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        ;ekran powitalny
        mov ssrect.RX,0
        mov ssrect.RY,58
        mov ssrect.RW,143
        mov ssrect.RH,35
        mov ecx,scrsx/2*256-143/20*256
        mov edx,scrsy/2*256-35/20*256
        ;mov dsrect.RX,scrsx/2-143/20
        ;mov dsrect.RY,scrsy/2-35/20
        ;mov dsrect.RW,143/10
        ;mov dsrect.RH,35/10
entry_scr:
        mov esi,offset backsurface
        mov edi,offset backscrsurface
        call CopySurface

        ;w ecx wsp. tex. x
        mov eax,ecx
        shr eax,8
        mov ebx,scrsx/2
        sub ebx,eax
        shl ebx,1
        mov dsrect.RX,eax
        mov dsrect.RW,ebx

        ;w edx wsp. tex. y
        mov eax,edx
        shr eax,8
        mov ebx,scrsy/2
        sub ebx,eax
        shl ebx,1
        mov dsrect.RY,eax
        mov dsrect.RH,ebx
        
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset gfxsurface
        mov edi,offset backscrsurface
        call DrawSurface8CS

        cmp ecx,2560
        jb entry_scr_d1

        sub ecx,1045
        sub edx,256

        jmp entry_scr_d2

entry_scr_d1:
        push ecx
        push edx
        invoke StrLen,addr entrystring
        mov ebx,eax
        push ebx
        mov eax,offset entrystring
        add eax,entry_pval
        mov dl,[eax]
        mov entry_pval2,dl
        mov byte ptr [eax],0
        push eax
        ;ebx=strlen(entrystring)*4
        shl ebx,2
        ;eax=scrsx/2-strlen(entrystring)*4
        mov eax,scrsx/2
        sub eax,ebx
        mov ebx,scrsy/2+90
        mov ecx,228
        mov edx,1
        mov esi,offset entrystring
        mov edi,offset backscrsurface
        call DrawString8
        pop eax
        mov bl,entry_pval2
        mov [eax],bl
        pop eax
        pop edx
        pop ecx
        cmp entry_pval,eax
        je entry_scr_d2
        inc entry_pval

entry_scr_d2:

        call Wait4VSync
        call BackScr2Scr

        inc entryscrtm
        cmp entryscrtm,60*12
        ja entry_scr_end

        IsKeyPressed KEY_SPACE
        jnz entry_scr_end
        jmp entry_scr

entry_scr_end:

;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        invoke InitBackSurface,0,0
        ;invoke LoadMap,0
        mov eax,offset framet
        mov ebx,TIME_FLAGS_SEC_TIME
        call InitTimeStruct

;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        ;glowna petla
main_loop:
        mov eax,offset framet
        call GetTime
        fstp fps2

        mov esi,offset backsurface
        mov edi,offset backscrsurface
        call CopySurface

        call GetKeyState
        cmp maploaded,0
        jz nomaploaded
        call DrawMap
        call DrawOpp
        cmp menuon,1
        je handle_off
        call HandleCol
        call HandleOpp
handle_off:
        call HandleTime
        call DrawPlayer
nomaploaded:
        cmp endgame,1
        jne noendgame
        call GetPlayerName
noendgame:
        call DrawInfo
        call DrawMenu
        cmp playerkill,0
        jz noplayerkill
        call CloseMap
        mov playerkill,0
        mov eax,offset statetxt1
        mov actualstatetxt,eax
noplayerkill:

        cmp iswait4vsync,0
        jz nowait4vsync
        call Wait4VSync
nowait4vsync:
        comment/
        mov ssrect.RX,190
        mov ssrect.RY,100
        mov ssrect.RW,200
        mov ssrect.RH,200
        mov dsrect.RX,0
        mov dsrect.RY,0
        mov dsrect.RW,640
        mov dsrect.RH,480
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset backscrsurface
        mov edi,offset scrsurface
        call DrawSurface8S/

        call BackScr2Scr

        cmp quit,1
        je quit_program

        mov eax,offset framet
        call GetTime
        fstp fps

        jmp main_loop

quit_program:

;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        call CloseKB
        invoke FreeHeapMem
        SetTXTMode
        mov ax,4c00h
        int 21h

stub32 ends

end
