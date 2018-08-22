;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
;MLAB copyright (c) Kamil Korolkiewicz 2005
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

;funkcje

InitBackSurface proc stdcall uses eax ebx ecx edx esi edi,bssx:dword,bssy:dword
        mov eax,bssx
        mov ssrect.RX,eax
        mov eax,bssy
        mov ssrect.RY,eax
        mov ssrect.RW,10
        mov ssrect.RH,10
        mov dsrect.RX,0
        mov dsrect.RY,0
        mov dsrect.RW,10
        mov dsrect.RH,10
        mov esi,offset gfxsurface
        mov edi,offset backsurface
        mov ecx,scrsy/10
drw_loopy:
                push ecx
                mov ecx,scrsx/10
drw_loopx:
                        mov eax,offset ssrect
                        mov ebx,offset dsrect
                        call DrawSurface8
                        add dsrect.RX,10
                        loop drw_loopx
                mov dsrect.RX,0
                add dsrect.RY,10
                pop ecx
                loop drw_loopy
        ret        
InitBackSurface endp

DrawPlayer proc stdcall uses eax ebx ecx edx esi edi
        mov ssrect.RX,20
        mov ssrect.RY,0
        mov ssrect.RW,7
        mov ssrect.RH,7
        mov eax,playerpozx
        mov dsrect.RX,eax
        mov eax,playerpozy
        mov dsrect.RY,eax
        mov dsrect.RW,7
        mov dsrect.RH,7
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset gfxsurface
        mov edi,offset backscrsurface
        call DrawSurface8C
dpeproc:
        ret
DrawPlayer endp

HLine proc stdcall uses eax ebx ecx edx edi,hlpx:dword,hlpy:dword,hlcol:dword
        mov eax,hlpy
        mov ebx,scrsx
        mul ebx
        add eax,hlpx
        mov edi,backscrsurface.SBits
        add edi,eax
        mov es,stub32zerosel
        cmp byte ptr es:[edi+cell_size/2],228
        je hlendp
        mov ecx,cell_size
        cmp hlcol,228
        jne hld1
        inc ecx
hld1:
        mov eax,hlcol
        mov ah,al
        mov bx,ax
        shl eax,16
        mov ax,bx
        cld
        mov ebx,ecx
        shr ecx,2
        and ebx,3
        rep stosd
        mov ecx,ebx
        rep stosb
hlendp:
        ret
HLine endp

VLine proc stdcall uses eax ebx ecx edx edi,vlpx:dword,vlpy:dword,vlcol:dword
        mov eax,vlpy
        mov ebx,scrsx
        mul ebx
        add eax,vlpx
        mov edi,backscrsurface.SBits
        add edi,eax
        mov es,stub32zerosel
        cmp byte ptr es:[edi+(cell_size/2)*scrsx],228
        je vlendp
        mov ecx,cell_size
        cmp vlcol,228
        jne vld1
        inc ecx
vld1:
        mov eax,vlcol
vldl:
                mov es:[edi],al
                add edi,scrsx
                loop vldl
vlendp:
        ret
VLine endp

Rectangle proc stdcall uses eax ebx ecx edx esi edi rectx1:dword,recty1:dword,rectsx:dword,rectsy:dword
        mov edi,backscrsurface.SBits
        mov eax,recty1
        mov ebx,scrsx*10
        mul ebx
        add edi,eax
        mov ebx,10
        mov eax,rectx1
        mul ebx
        add edi,eax
        mov es,stub32zerosel

        mov eax,rectsx
        mul ebx
        push eax
        mov ecx,eax
        inc ecx
        mov eax,0E4E4E4E4h
        cld
        mov edx,ecx
        shr ecx,2
        and edx,3
        rep stosd
        mov ecx,edx
        rep stosb

        mov eax,rectsy
        mul ebx
        mov ecx,eax
        inc ecx
        mov edx,eax
        mov eax,228
        dec edi
rectd1:
        mov es:[edi],al
        add edi,scrsx
        loop rectd1

        pop ecx
        inc ecx
        sub edi,scrsx+4
        std
        mov esi,ecx
        shr ecx,2
        and esi,3
        mov eax,0E4E4E4E4h
        rep stosd
        mov ecx,esi
        add edi,4
        rep stosb
        cld

        inc edi
        mov ecx,edx
rectd2:
        mov es:[edi],al
        sub edi,scrsx
        loop rectd2

        ret
Rectangle endp

DrawCell proc stdcall uses eax ebx ecx edx esi edi,cellpx:dword,cellpy:dword,cellv1:dword,cellv2:dword
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
        invoke vline,ecx,eax,228
cell_d1:
        mov dl,celltypes[ebx*4]+1
        or dl,dl
        jz cell_d2
        invoke hline,ecx,eax,228
cell_d2:
        mov dl,celltypes[ebx*4]+2
        or dl,dl
        jz cell_d3
        push ecx
        add ecx,cell_size
        invoke vline,ecx,eax,228
        pop ecx
cell_d3:
        mov dl,celltypes[ebx*4]+3
        or dl,dl
        jz cell_d4
        push eax
        add eax,cell_size
        invoke hline,ecx,eax,228
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
        inc edx
        cmp byte ptr celltypes[ebx*4],0
        jz door_d1
        invoke vline,ecx,eax,edx
door_d1:
        mov edx,doorcoltab[edi*4]
        inc edx
        cmp byte ptr celltypes[ebx*4]+1,0
        jz door_d2
        invoke hline,ecx,eax,edx
door_d2:
        mov edx,doorcoltab[edi*4]
        inc edx
        cmp byte ptr celltypes[ebx*4]+2,0
        jz door_d3
        push ecx
        add ecx,cell_size
        invoke vline,ecx,eax,edx
        pop ecx
door_d3:
        mov edx,doorcoltab[edi*4]
        inc edx
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
DrawCell endp

DrawMap proc stdcall uses eax ebx ecx edx
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
        push ebx
        mov edi,LabMap.map_cells
        mov ecx,LabMap.map_height
        mov es,stub32zerosel
dmly:
                push ecx
                mov ecx,LabMap.map_width
                push eax
dmlx:
                        movzx edx,es:byte ptr [edi]
                        movzx esi,es:byte ptr [edi+1]
                        invoke DrawCell,eax,ebx,edx,esi
                        inc eax
                        add edi,2
                        loop dmlx
                pop eax
                pop ecx
                inc ebx
                loop dmly
        pop ebx
        invoke Rectangle,eax,ebx,LabMap.map_width,LabMap.map_height
        mov ecx,LabMap.map_end_x
        imul ecx,cell_size
        mov edx,LabMap.map_end_y
        imul edx,cell_size
        imul eax,cell_size
        imul ebx,cell_size
        add ecx,eax
        add edx,ebx
        mov ssrect.RX,100
        mov ssrect.RY,0
        mov ssrect.RW,9
        mov ssrect.RH,9
        inc ecx
        mov dsrect.RX,ecx
        mov dsrect.RY,edx
        mov dsrect.RW,9
        mov dsrect.RH,9
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset gfxsurface
        mov edi,offset backscrsurface
        call DrawSurface8
end_dm:
        ret
DrawMap endp

DrawOpp proc stdcall uses eax ebx ecx edx esi edi
        local prx,pry:dword
        cmp LabMap.opp_n,0
        jz doeproc
        ;eax=scrsx/cell_size/2-LabMap.map_width/2
        mov ebx,LabMap.map_width
        mov eax,scrsx/cell_size/2
        shr ebx,1
        sub eax,ebx
        imul eax,10
        mov prx,eax
        ;ebx=scrsy/cell_size/2-LabMap.map_height/2
        mov ecx,LabMap.map_height
        mov ebx,scrsy/cell_size/2
        shr ecx,1
        sub ebx,ecx
        imul ebx,10
        mov pry,ebx
        mov ecx,LabMap.opp_n
        mov esi,LabMap.opp_tab
        mov es,stub32zerosel
        mov ssrect.RX,30
        mov ssrect.RY,0
        mov ssrect.RW,7
        mov ssrect.RH,7
        mov dsrect.RW,7
        mov dsrect.RH,7
        assume esi:ptr TOpp
doloop:
                mov edx,es:[esi].opp_x
                add edx,prx
                mov dsrect.RX,edx
                mov edx,es:[esi].opp_y
                add edx,pry
                mov dsrect.RY,edx
                push esi
                mov eax,offset ssrect
                mov ebx,offset dsrect
                mov esi,offset gfxsurface
                mov edi,offset backscrsurface
                call DrawSurface8C
                pop esi
                mov edi,dsrect.RY
                add edi,3
                imul edi,scrsx
                add edi,dsrect.RX
                add edi,3
                xor eax,eax
                mov al,es:[esi].opp_dir
                mov ebx,4
                xor edx,edx
                div ebx
                mov es:[esi].opp_dir,dl
                add edi,opp_dirtab[edx*4]
                add edi,backscrsurface.SBits
                mov byte ptr es:[edi],236
                add esi,sizeof (TOpp)
                loop doloop
        
doeproc:
        assume esi:nothing
        ret
DrawOpp endp

DrawInfo proc stdcall uses eax ebx ecx edx esi edi
        cmp dfps,0
        jz ndfps

        fld1
        fdiv fps
        fistp fpsu
        
        invoke SPrintf,addr tbuf1,addr fpstxt,fpsu
        mov eax,2
        mov ebx,2
        mov ecx,255
        mov edx,0
        mov esi,offset tbuf1
        mov edi,offset backscrsurface
        call DrawString8

ndfps:
        mov ebx,actualstatetxt
        cmp byte ptr [ebx],0
        jz nostatetxt

        mov esi,ebx
        invoke StrLen,ebx
        mov ebx,eax
        mov eax,scrsx/2
        imul ebx,4
        sub eax,ebx
        mov ebx,scrsy/2-4
        mov ecx,255
        mov edx,0
        mov edi,offset backscrsurface
        call DrawString8

nostatetxt:

        cmp endgame,1
        jne noendgame1

        mov eax,scrsx/2-sizeof pplayernametxt*4
        mov ebx,scrsy/2+10
        mov ecx,255
        mov edx,0
        mov esi,offset pplayernametxt
        mov edi,offset backscrsurface
        call DrawString8

        mov eax,scrsx/2-sizeof pplayername*4
        mov ebx,scrsy/2+18
        mov ecx,255
        mov edx,0
        mov esi,offset pplayername
        mov edi,offset backscrsurface
        call DrawString8

        mov eax,ppnnpoz
        shl eax,3
        add eax,scrsx/2-sizeof pplayername*4
        invoke HLine,eax,scrsy/2+26,255
        jmp noendgame2

noendgame1:

        cmp endgame,2
        jne noendgame2
        call DrawPlayerList
noendgame2:

        cmp maploaded,0
        jz did3

        invoke SPrintf,addr tbuf1,addr infotxt,nmap,pkt,atime
        mov eax,2
        mov ebx,scrsy-9
        mov ecx,255
        mov edx,0
        mov esi,offset tbuf1
        mov edi,offset backscrsurface
        call DrawString8
        
        mov ssrect.RY,0
        mov ssrect.RW,10
        mov ssrect.RH,10
        mov dsrect.RY,scrsy-10
        mov dsrect.RW,10
        mov dsrect.RH,10

        test playerkeys,1
        jz did1
        mov ssrect.RX,40
        mov dsrect.RX,221
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset gfxsurface
        mov edi,offset backscrsurface
        call DrawSurface8C
did1:
        test playerkeys,2
        jz did2
        mov ssrect.RX,50
        mov dsrect.RX,231
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset gfxsurface
        mov edi,offset backscrsurface
        call DrawSurface8C
did2:
        test playerkeys,4
        jz did3
        mov ssrect.RX,60
        mov dsrect.RX,241
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset gfxsurface
        mov edi,offset backscrsurface
        call DrawSurface8C
did3:
        ret
DrawInfo endp

DrawMenu proc stdcall uses eax ebx esi edi
        cmp menuon,0
        jz dmendp
        mov ssrect.RX,0
        mov ssrect.RY,10
        mov ssrect.RW,125
        mov ssrect.RH,48
        mov dsrect.RX,100
        mov dsrect.RY,216
        mov dsrect.RW,125
        mov dsrect.RH,48
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset gfxsurface
        mov edi,offset backscrsurface
        call DrawSurface8C
        mov ssrect.RX,0
        mov ssrect.RY,58
        mov ssrect.RW,143
        mov ssrect.RH,35
        mov dsrect.RX,249
        mov dsrect.RY,40
        mov dsrect.RW,143
        mov dsrect.RH,35
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset gfxsurface
        mov edi,offset backscrsurface
        call DrawSurface8C
        mov ssrect.RX,110
        mov ssrect.RY,0
        mov ssrect.RW,10
        mov ssrect.RH,10
        mov eax,menupoz
        imul eax,12
        add eax,216
        mov dsrect.RX,87
        mov dsrect.RY,eax
        mov dsrect.RW,125
        mov dsrect.RH,48
        mov eax,offset ssrect
        mov ebx,offset dsrect
        mov esi,offset gfxsurface
        mov edi,offset backscrsurface
        call DrawSurface8C
dmendp:
        ret
DrawMenu endp

DrawPlayerList proc stdcall uses eax ebx ecx edx esi edi
        local nlp:dword
        mov eax,scrsx/2-sizeof playerlisttxt*4
        mov ebx,90
        mov ecx,255
        mov edx,0
        mov esi,offset playerlisttxt
        mov edi,offset backscrsurface
        call DrawString8

        mov eax,scrsx/2-sizeof playerlisttxt1*4
        mov ebx,100
        mov ecx,255
        mov edx,0
        mov esi,offset playerlisttxt1
        mov edi,offset backscrsurface
        call DrawString8
        mov ecx,nplayerlist
        or ecx,ecx
        jz dplendp
        xor ebx,ebx
        mov nlp,0
dpll1:
        mov eax,dword ptr PlayerList[ebx].player_name
        and eax,00000000111111111111111111111111b
        mov dword ptr tbuf2,eax
        invoke SPrintf,addr tbuf1,addr playerlisttxt2,addr tbuf2,PlayerList[ebx].player_pkt,PlayerList[ebx].player_nmap
        invoke StrLen,addr tbuf1
        push ebx
        push ecx
        mov ebx,scrsx/2
        shl eax,2
        sub ebx,eax
        mov eax,ebx
        mov ebx,110
        add ebx,nlp
        mov ecx,nlp
        shr ecx,3
        cmp ecx,actplp
        je dplc2
        mov ecx,255
        jmp dplc
dplc2:
        mov ecx,236
dplc:
        mov edx,0
        mov esi,offset tbuf1
        mov edi,offset backscrsurface
        call DrawString8
        pop ecx
        pop ebx
        add ebx,sizeof (TPlayerList)
        add nlp,8
        dec ecx
        jnz dpll1
dplendp:
        ret
DrawPlayerList endp
