.386

public main

stub32 segment public
assume cs:stub32,ds:stub32

;dane

;kod

include dpmi.inc
include main.inc
include mouse.inc
include mlab_game.asm
include mlab_g.asm
include mstdlib\mmemmak.inc

mst TMouseState <>

scell db 0
sspec db 0
sdoor db 0
sdoorspec db 0
odir db 0
aot dd 0

DrawCell2 proc stdcall uses eax ebx ecx edx esi edi,cellpx:dword,cellpy:dword,cellv1:dword,cellv2:dword,dccol:dword
        mov eax,cellpx
        mov ebx,cell_size
        mul ebx
        ;ecx=cellpx*cell_size
        mov ecx,eax
        ;eax=cellpy*cell_size
        mov eax,cellpy
        mul ebx
        ;rysowanie sciany
        mov ebx,cellv1
        mov esi,ebx
        and ebx,15
        shr esi,4
        cmp ebx,15
        je cell_full
        mov dl,celltypes[ebx*4]
        or dl,dl
        jz cell_d1
        invoke vline,ecx,eax,dccol
cell_d1:
        mov dl,celltypes[ebx*4]+1
        or dl,dl
        jz cell_d2
        invoke hline,ecx,eax,dccol
cell_d2:
        mov dl,celltypes[ebx*4]+2
        or dl,dl
        jz cell_d3
        push ecx
        add ecx,cell_size
        invoke vline,ecx,eax,dccol
        pop ecx
cell_d3:
        mov dl,celltypes[ebx*4]+3
        or dl,dl
        jz cell_d4
        push eax
        add eax,cell_size
        invoke hline,ecx,eax,dccol
        pop eax
cell_d4:
        jmp door
cell_full:
        push eax
        push ecx
        mov ssrect.RX,10
        mov ssrect.RY,0
        mov ssrect.RW,10
        mov ssrect.RH,10
        mov dsrect.RX,ecx
        mov dsrect.RY,eax
        mov dsrect.RW,10
        mov dsrect.RH,10
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset gfxsurface
        mov edi,offset backscrsurface
        call DrawSurface8
        pop ecx
        pop eax
door:        
        ;rysowanie drzwi
        mov ebx,cellv2
        mov edi,ebx
        and ebx,15
        shr edi,4
        mov edx,doorcoltab[edi*4]
        cmp byte ptr celltypes[ebx*4],0
        jz door_d1
        invoke vline,ecx,eax,edx
door_d1:
        mov edx,doorcoltab[edi*4]
        cmp byte ptr celltypes[ebx*4]+1,0
        jz door_d2
        invoke hline,ecx,eax,edx
door_d2:
        mov edx,doorcoltab[edi*4]
        cmp byte ptr celltypes[ebx*4]+2,0
        jz door_d3
        push ecx
        add ecx,cell_size
        invoke vline,ecx,eax,edx
        pop ecx
door_d3:
        mov edx,doorcoltab[edi*4]
        cmp byte ptr celltypes[ebx*4]+3,0
        jz door_d4
        push eax
        add eax,cell_size
        invoke hline,ecx,eax,edx
        pop eax
door_d4:
        or esi,esi
        jz no_draw_a
        dec esi
        imul esi,10
        add esi,40
        mov ssrect.RX,esi
        mov ssrect.RY,0
        mov ssrect.RW,10
        mov ssrect.RH,10
        mov dsrect.RX,ecx
        mov dsrect.RY,eax
        mov dsrect.RW,10
        mov dsrect.RH,10
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset gfxsurface
        mov edi,offset backscrsurface
        call DrawSurface8C
no_draw_a:
        ret
DrawCell2 endp

LoadMap2 proc stdcall uses eax ebx ecx,lmnmap:dword
        invoke UToA,lmnmap,addr tbuf1
        invoke StrCopy,addr tbuf2,addr mapname1
        invoke StrCat,addr tbuf2,addr tbuf1
        invoke StrCat,addr tbuf2,addr mapname2

        invoke FOpen,addr tbuf2,OPEN_ONLY,READ_ONLY
        jc end_error
        mov bx,ax

        invoke FRead,bx,28,addr LabMap
        mov ecx,LabMap.opp_n
        invoke AllocMem,64*48*9
        mov LabMap.opp_tab,eax
        mov edx,eax
        jecxz lmd1

        mov es,stub32zerosel
        assume edx:ptr TOpp
loadopp_loop:
        invoke FRead,bx,4,addr opp_loaddword
        mov eax,opp_loaddword
        mov es:[edx].opp_x,eax
        invoke FRead,bx,4,addr opp_loaddword
        mov eax,opp_loaddword
        mov es:[edx].opp_y,eax
        invoke FRead,bx,1,addr opp_loaddword
        mov al,byte ptr opp_loaddword
        mov es:[edx].opp_dir,al
        add edx,sizeof (TOpp)
        loop loadopp_loop
        assume edx:nothing

lmd1:
        mov eax,LabMap.map_width
        mov ecx,LabMap.map_height
        mul ecx
        or eax,eax
        jz end_error_c
        shl eax,1
        mov ecx,eax
        invoke AllocMem,eax
        mov LabMap.map_cells,eax
        invoke EFRead,bx,ecx,eax

        invoke FClose,bx

        mov maploaded,1
        ret
end_error_c:
        invoke FClose,bx
end_error:
        mov maploaded,0
        ret
LoadMap2 endp

SaveMap proc stdcall uses eax ebx ecx,lmnmap:dword
        invoke UToA,lmnmap,addr tbuf1
        invoke StrCopy,addr tbuf2,addr mapname1
        invoke StrCat,addr tbuf2,addr tbuf1
        invoke StrCat,addr tbuf2,addr mapname2

        invoke FOpen,addr tbuf2,CREATE_ONLY,WRITE_ONLY
        mov bx,ax

        invoke FWrite,bx,28,addr LabMap
        mov ecx,LabMap.opp_n
        mov edx,LabMap.opp_tab
        jecxz lmd1
        mov es,stub32zerosel
        assume edx:ptr TOpp
loadopp_loop:
        mov eax,es:[edx].opp_x
        mov opp_loaddword,eax
        invoke FWrite,bx,4,addr opp_loaddword
        mov eax,es:[edx].opp_y
        mov opp_loaddword,eax
        invoke FWrite,bx,4,addr opp_loaddword
        mov al,es:[edx].opp_dir
        mov byte ptr opp_loaddword,al
        invoke FWrite,bx,1,addr opp_loaddword
        add edx,sizeof (TOpp)
        loop loadopp_loop
        assume edx:nothing

lmd1:
        mov eax,LabMap.map_width
        mov ecx,LabMap.map_height
        mul ecx
        shl eax,1
        mov ecx,eax
        mov eax,LabMap.map_cells
        invoke EFWrite,bx,ecx,eax

        invoke FClose,bx

        ret
SaveMap endp

drawcur:
        movzx eax,mst.mouse_y
        imul eax,640
        movzx edi,mst.mouse_x
        add eax,edi
        mov edi,backscrsurface.SBits
        add edi,eax
        mov es,stub32zerosel
        mov byte ptr es:[edi-1],228
        mov byte ptr es:[edi+1],228
        mov byte ptr es:[edi-640],228
        mov byte ptr es:[edi+640],228
        ret

drawn:
        mov edi,backscrsurface.SBits
        mov es,stub32zerosel
        mov eax,64
        mov ecx,48
drl1:
        push ecx
        mov ecx,640
        rep stosb
        pop ecx
        add edi,640*9
        loop drl1
        mov edi,backscrsurface.SBits
        mov ecx,64
drl2:
        push ecx
        mov ecx,480
drl3:
        mov es:[edi],al
        add edi,640
        loop drl3
        pop ecx
        sub edi,640*480-10
        loop drl2
        ret

drawm:
        movzx eax,mst.mouse_x
        xor edx,edx
        mov ebx,10
        div ebx
        mov ecx,eax
        xor edx,edx
        movzx eax,mst.mouse_y
        div ebx
        xchg eax,ecx
        mov ebx,ecx
        movzx ecx,scell
        movzx edx,sspec
        shl edx,4
        add ecx,edx
        movzx edx,sdoor
        movzx esi,sdoorspec
        shl esi,4
        add edx,esi
        call DrawMap
        call DrawOpp
        invoke DrawCell2,eax,ebx,ecx,edx,128
        ;eax=scrsx/cell_size/2-LabMap.map_width/2
        mov ebx,LabMap.map_width
        mov eax,scrsx/cell_size/2
        shr ebx,1
        sub eax,ebx
        ;ebx=scrsy/cell_size/2-LabMap.map_height/2
        mov ecx,LabMap.map_height
        mov ebx,scrsy/cell_size/2
        shr ecx,1
        sub ebx,ecx
        mov ecx,LabMap.map_start_x
        imul ecx,cell_size
        mov edx,LabMap.map_start_y
        imul edx,cell_size
        imul eax,cell_size
        imul ebx,cell_size
        add ecx,eax
        add edx,ebx
        mov ssrect.RX,20
        mov ssrect.RY,0
        mov ssrect.RW,9
        mov ssrect.RH,9
        add ecx,2
        add edx,2
        mov dsrect.RX,ecx
        mov dsrect.RY,edx
        mov dsrect.RW,9
        mov dsrect.RH,9
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset gfxsurface
        mov edi,offset backscrsurface
        call DrawSurface8C
        ret

put:
        ;eax=scrsx/cell_size/2-LabMap.map_width/2
        mov ebx,LabMap.map_width
        mov eax,scrsx/cell_size/2
        shr ebx,1
        sub eax,ebx
        ;ebx=scrsy/cell_size/2-LabMap.map_height/2
        mov ecx,LabMap.map_height
        mov ebx,scrsy/cell_size/2
        shr ecx,1
        sub ebx,ecx
        push eax
        push ebx
        movzx eax,mst.mouse_x
        xor edx,edx
        mov ebx,10
        div ebx
        mov ecx,eax
        xor edx,edx
        movzx eax,mst.mouse_y
        div ebx
        xchg eax,ecx
        mov ebx,ecx
        pop edx
        pop ecx
        sub eax,ecx
        sub ebx,edx
        imul ebx,LabMap.map_width
        add ebx,eax
        movzx ecx,scell
        movzx edx,sspec
        shl edx,4
        add ecx,edx
        movzx edx,sdoor
        movzx esi,sdoorspec
        shl esi,4
        add edx,esi
        mov edi,LabMap.map_cells
        shl ebx,1
        add edi,ebx
        mov es:[edi],cl
        mov es:[edi+1],dl
        ret

addopp:
        ;eax=scrsx/cell_size/2-LabMap.map_width/2
        mov ebx,LabMap.map_width
        mov eax,scrsx/cell_size/2
        shr ebx,1
        sub eax,ebx
        ;ebx=scrsy/cell_size/2-LabMap.map_height/2
        mov ecx,LabMap.map_height
        mov ebx,scrsy/cell_size/2
        shr ecx,1
        sub ebx,ecx
        push eax
        push ebx
        movzx eax,mst.mouse_x
        xor edx,edx
        mov ebx,10
        div ebx
        mov ecx,eax
        xor edx,edx
        movzx eax,mst.mouse_y
        div ebx
        xchg eax,ecx
        mov ebx,ecx
        pop edx
        pop ecx
        sub eax,ecx
        sub ebx,edx
        imul eax,cell_size
        imul ebx,cell_size
        add eax,2
        add ebx,2
        mov edx,LabMap.opp_tab
        assume edx:ptr TOpp
        mov es,stub32zerosel
        mov ecx,LabMap.opp_n
        jecxz addoppo
        xor edi,edi
addoppspr:
        cmp es:[edx].opp_x,eax
        jne addoppld
        cmp es:[edx].opp_y,ebx
        je addoppd
addoppld:
        add edx,sizeof (TOpp)
        inc edi
        loop addoppspr
addoppo:
        
        mov es:[edx].opp_x,eax
        mov es:[edx].opp_y,ebx
        mov al,odir
        mov es:[edx].opp_dir,al
        inc LabMap.opp_n
        mov aot,0
        ret
addoppd:
        mov ecx,LabMap.opp_n
        imul ecx,sizeof (TOpp)
        sub ecx,edi
        push ds
        push es
        pop ds
        mov edi,edx
        mov esi,edx
        add esi,sizeof (TOpp)
        cld
        rep movsb
        pop ds
        dec LabMap.opp_n
        mov aot,0
        ret
        assume edx:nothing

main:

        call Cls

        invoke GetParamCount
        cmp eax,1
        jb pq

        ;inicjacja pamieci
        invoke InitHeapMem,1024*1024*4          ;4MB
        mov __MMEM_is_generate_error,1

        mov al,1
        mov esi,offset tbuf1
        invoke GetParam
        invoke AToU,addr tbuf1
        mov nmap,eax
        invoke LoadMap2,nmap
        cmp maploaded,1
        je d1

        invoke GetParamCount
        cmp eax,3
        jne pq

        mov al,2
        mov esi,offset tbuf1
        invoke GetParam
        invoke AToU,addr tbuf1
        mov LabMap.map_width,eax

        mov al,3
        mov esi,offset tbuf1
        invoke GetParam
        invoke AToU,addr tbuf1
        mov LabMap.map_height,eax
        mov LabMap.map_start_x,0
        mov LabMap.map_start_y,0
        mov LabMap.map_end_x,0
        mov LabMap.map_end_y,0
        mov LabMap.opp_n,0
        mov maploaded,1

        mov eax,LabMap.map_width
        imul eax,LabMap.map_height
        shl eax,1
        mov ecx,eax
        invoke AllocMem,eax
        mov LabMap.map_cells,eax
        mov es,stub32zerosel
        FillMemB LabMap.map_cells,ecx,0
        invoke AllocMem,64*48*9
        mov LabMap.opp_tab,eax
        mov es,stub32zerosel
        FillMemB LabMap.opp_tab,64*48*9,0

d1:
        
        call InitMouse
        or eax,eax
        jz pq

        ;inicjacja klawiatury
        call InitKB

        ;ladowanie pliku
        invoke LoadBMP8,addr gfxfname,addr gfxsurface

        invoke AllocMem,768
        mov pal,eax
        invoke LoadPalFromBMP,addr gfxfname,pal

        ;pobieranie LFB
        mov cx,scrmode
        call GetLFB
        mov scrsurface.SBits,eax

        mov eax,offset backscrsurface
        call AllocSurface

        SetVESAMode scrmode,0100000000000000b
        ;SetVGAMode

        invoke SetPalette,pal

        call InitMouse
        invoke SetMouseCursorPoz,320,240
        ;eax=scrsx/cell_size/2-LabMap.map_width/2
        mov ebx,LabMap.map_width
        mov eax,scrsx/cell_size/2
        shr ebx,1
        sub eax,ebx
        imul eax,10
        ;ebx=scrsy/cell_size/2-LabMap.map_height/2
        mov ecx,LabMap.map_height
        mov ebx,scrsy/cell_size/2
        shr ecx,1
        sub ebx,ecx
        imul ebx,10
        mov ecx,eax
        mov esi,LabMap.map_width
        imul esi,10
        add ecx,esi
        mov edx,ebx
        mov esi,LabMap.map_height
        imul esi,10
        add edx,esi
        dec ecx
        dec edx
        invoke SetMouseCursorRect,eax,ebx,ecx,edx

main_loop:
        invoke GetMouseState,addr mst
        IsKeyPressed KEY_F1+9
        jnz pq

        IsKeyPressed KEY_F1+1
        jz spn
        invoke SaveMap,nmap
        mov byte ptr __KBKeyTab+KEY_F1+1,0
spn:

        IsKeyPressed KEY_UP
        jz ddd1
        inc scell
        mov byte ptr __KBKeyTab+KEY_UP,0
ddd1:
        IsKeyPressed KEY_DOWN
        jz ddd2
        dec scell
        mov byte ptr __KBKeyTab+KEY_DOWN,0
ddd2:
        IsKeyPressed KEY_LEFT
        jz ddd3
        dec sspec
        mov byte ptr __KBKeyTab+KEY_LEFT,0
ddd3:
        IsKeyPressed KEY_RIGHT
        jz ddd4
        inc sspec
        mov byte ptr __KBKeyTab+KEY_RIGHT,0
ddd4:
        IsKeyPressed KEY_PAGEUP
        jz ddd5
        inc sdoor
        mov byte ptr __KBKeyTab+KEY_PAGEUP,0
ddd5:
        IsKeyPressed KEY_PAGEDOWN
        jz ddd6
        dec sdoor
        mov byte ptr __KBKeyTab+KEY_PAGEDOWN,0
ddd6:
        IsKeyPressed KEY_HOME
        jz ddd7
        inc sdoorspec
        mov byte ptr __KBKeyTab+KEY_HOME,0
ddd7:
        IsKeyPressed KEY_END
        jz ddd8
        dec sdoorspec
        mov byte ptr __KBKeyTab+KEY_END,0
ddd8:
        IsKeyPressed KEY_INSERT
        jz ddd9
        inc odir
        mov byte ptr __KBKeyTab+KEY_INSERT,0
ddd9:
        IsKeyPressed KEY_DELETE
        jz ddd10
        dec odir
        mov byte ptr __KBKeyTab+KEY_DELETE,0
ddd10:

        mov ebx,16
        xor edx,edx
        movzx eax,scell
        div ebx
        mov scell,dl

        cmp sspec,255
        jne dds1
        mov sspec,6
dds1:

        mov ebx,7
        xor edx,edx
        movzx eax,sspec
        div ebx
        mov sspec,dl

        mov ebx,16
        xor edx,edx
        movzx eax,sdoor
        div ebx
        mov sdoor,dl

        cmp sdoorspec,255
        jne ddsd
        mov sdoorspec,2

ddsd:

        mov ebx,3
        xor edx,edx
        movzx eax,sdoorspec
        div ebx
        mov sdoorspec,dl

        mov ebx,4
        xor edx,edx
        movzx eax,odir
        div ebx
        mov odir,dl

        inc aot

        IsKeyPressed KEY_ALT
        jz ddnalt
        cmp mst.mouse_key,MOUSE_KEY_LEFT
        jne ddnalt
        cmp aot,30
        jb ddnalt1
        call addopp
ddnalt1:
        jmp md2
ddnalt:

        IsKeyPressed KEY_CTRL
        jz ddsd1        
        ;eax=scrsx/cell_size/2-LabMap.map_width/2
        mov ebx,LabMap.map_width
        mov eax,scrsx/cell_size/2
        shr ebx,1
        sub eax,ebx
        ;ebx=scrsy/cell_size/2-LabMap.map_height/2
        mov ecx,LabMap.map_height
        mov ebx,scrsy/cell_size/2
        shr ecx,1
        sub ebx,ecx
        push eax
        push ebx
        movzx eax,mst.mouse_x
        xor edx,edx
        mov ebx,10
        div ebx
        mov ecx,eax
        xor edx,edx
        movzx eax,mst.mouse_y
        div ebx
        xchg eax,ecx
        mov ebx,ecx
        pop edx
        pop ecx
        sub eax,ecx
        sub ebx,edx
        cmp mst.mouse_key,MOUSE_KEY_LEFT
        jne ddc11
        mov LabMap.map_start_x,eax
        mov LabMap.map_start_y,ebx
        jmp md2
ddc11:
        cmp mst.mouse_key,MOUSE_KEY_RIGHT
        jne ddsd1
        mov LabMap.map_end_x,eax
        mov LabMap.map_end_y,ebx
        jmp md2

ddsd1:
        cmp mst.mouse_key,MOUSE_KEY_LEFT
        jne md1
        call put
md1:
        cmp mst.mouse_key,MOUSE_KEY_RIGHT
        jne md2
        movzx eax,scell
        push eax
        movzx eax,sspec
        push eax
        movzx eax,sdoor
        push eax
        movzx eax,sdoorspec
        push eax
        mov scell,0
        mov sspec,0
        mov sdoor,0
        mov sdoorspec,0
        call put
        pop eax
        mov sdoorspec,al
        pop eax
        mov sdoor,al
        pop eax
        mov sspec,al
        pop eax
        mov scell,al
md2:

        mov eax,0
        mov esi,offset backscrsurface
        call FillSurface

        call drawn
        call drawm
        call drawcur

        mov esi,offset backscrsurface
        mov edi,offset scrsurface
        call CopySurface

        jmp main_loop

pq:

        call CloseKB
        invoke FreeHeapMem
        SetTXTMode
        mov ax,4c00h
        int 21h

stub32 ends

end
