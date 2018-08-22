;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
;MLAB copyright (c) Kamil Korolkiewicz 2005
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

;funkcje

GetKeyState proc stdcall uses eax
        IsKeyPressed KEY_F1+9
        jz d1
        mov quit,1
d1:
        cmp menuon,1
        je menuonk

        IsKeyPressed KEY_UP
        jz d2
        dec playerpozy
d2:
        IsKeyPressed KEY_DOWN
        jz d3
        inc playerpozy
d3:
        IsKeyPressed KEY_LEFT
        jz d4
        dec playerpozx
d4:
        IsKeyPressed KEY_RIGHT
        jz d5
        inc playerpozx
d5:
        IsKeyPressed KEY_F1+1
        jz d6
        call SaveGame
        mov byte ptr __KBKeyTab+KEY_F1+1,0
d6:
        IsKeyPressed KEY_F1+2
        jz d7
        call LoadGame
        mov byte ptr __KBKeyTab+KEY_F1+2,0
d7:
        jmp kesc
menuonk:
        IsKeyPressed KEY_UP
        jz md1
        dec menupoz
        mov byte ptr __KBKeyTab+KEY_UP,0
md1:
        IsKeyPressed KEY_DOWN
        jz md2
        inc menupoz
        mov byte ptr __KBKeyTab+KEY_DOWN,0
md2:
        mov eax,menupoz
        mov ebx,4
        xor edx,edx
        div ebx
        mov menupoz,edx

        IsKeyPressed KEY_ENTER
        jz md3
        call EnterMenuFunc
        mov byte ptr __KBKeyTab+KEY_ENTER,0
md3:
kesc:
        IsKeyPressed KEY_ESC
        jz endp1
        mov eax,1
        sub eax,menuon
        mov menuon,eax
        mov eax,offset statetxt0
        mov actualstatetxt,eax
        mov byte ptr __KBKeyTab+KEY_ESC,0
endp1:
        ret
GetKeyState endp

GetLabParams proc stdcall
        call GetParamCount
        or eax,eax
        jz pendp
        mov esi,offset tbuf1
gploop:
        push eax
        call GetParam
        invoke StrCmp,addr tbuf1,addr paramname1
        or eax,eax
        jnz d1
        mov iswait4vsync,0
        jmp dqp
d1:
        invoke StrCmp,addr tbuf1,addr paramname2
        or eax,eax
        jnz d2
        mov dfps,1
        jmp dqp
d2:
dqp:
        pop eax
        dec eax
        jnz gploop
pendp:
        ret
GetLabParams endp

EnterMenuFunc proc stdcall uses eax
        cmp menupoz,0
        jne d1
        call CloseMap
        mov nmap,1
        mov pkt,0
        mov pktatstart,0
        mov playerkeys,0
        mov atime,0
        fldz
        fstp ftime
        mov eax,offset timerstruct
        mov ebx,TIME_FLAGS_SEC_TIME
        call InitTimeStruct
        push nmap
        call LoadMap
        jmp emfendp
d1:
        cmp menupoz,1
        jne d2
        call LoadGame
        or eax,eax
        jz emfendpm1
        jmp emfendp
d2:
        cmp menupoz,2
        jne d3
        call SaveGame
        or eax,eax
        jz emfendpm1
        jmp emfendp
d3:
        mov quit,1
emfendp:
        mov menuon,0
emfendpm1:
        ret
EnterMenuFunc endp

NextLevel proc stdcall
        inc nmap
        call CloseMap
        mov eax,3600
        sub eax,atime
        cmp eax,0
        jl noextrapkt
        mul eax
        add pkt,eax
noextrapkt:
        mov playerkeys,0
        mov atime,0
        mov eax,pkt
        mov pktatstart,eax
        fldz
        fstp ftime
        mov eax,offset timerstruct
        mov ebx,TIME_FLAGS_SEC_TIME
        call InitTimeStruct
        push nmap
        call LoadMap
        cmp maploaded,1
        je nlendp
        mov eax,offset statetxt2
        mov actualstatetxt,eax
nlendp:
        ret
NextLevel endp

LoadMap proc stdcall uses eax ebx ecx edx,lmnmap:dword
        invoke UToA,lmnmap,addr tbuf1
        invoke StrCopy,addr tbuf2,addr mapname1
        invoke StrCat,addr tbuf2,addr tbuf1
        invoke StrCat,addr tbuf2,addr mapname2

        invoke FOpen,addr tbuf2,OPEN_ONLY,READ_ONLY
        jc end_error
        mov bx,ax

        invoke FRead,bx,28,addr LabMap
        mov eax,LabMap.opp_n
        or eax,eax
        jz lmd1
        mov ecx,eax
        imul eax,sizeof (TOpp)
        invoke AllocFAMem,eax
        mov opp_tab_famem,eax
        mov edx,fs:[eax]
        mov LabMap.opp_tab,edx
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
        mov es:[edx].opp_byte,0
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
        invoke AllocFAMem,eax
        mov map_cells_famem,eax
        mov eax,fs:[eax]
        mov LabMap.map_cells,eax
        invoke EFRead,bx,ecx,eax

        invoke FClose,bx

        ;eax=scrsx/cell_size/2-LabMap.map_width/2
        mov ebx,LabMap.map_width
        mov eax,scrsx/cell_size/2
        shr ebx,1
        sub eax,ebx
        imul eax,cell_size
        ;ebx=scrsy/cell_size/2-LabMap.map_height/2
        mov ecx,LabMap.map_height
        mov ebx,scrsy/cell_size/2
        shr ecx,1
        sub ebx,ecx
        imul ebx,cell_size
        mov ecx,LabMap.map_start_x
        imul ecx,cell_size
        add ecx,eax
        add ecx,2
        mov playerpozx,ecx

        mov ecx,LabMap.map_start_y
        imul ecx,cell_size
        add ecx,ebx
        add ecx,2
        mov playerpozy,ecx

        mov endgame,0
        mov maploaded,1
        ret
end_error_c:
        invoke FClose,bx
end_error:
        mov endgame,1
        mov maploaded,0
        call CloseKB
        ret
LoadMap endp

CloseMap proc stdcall
        invoke FreeFAMem,map_cells_famem
        invoke FreeFAMem,opp_tab_famem
        mov maploaded,0
        ret
CloseMap endp

LoadGame proc stdcall uses ebx
        invoke FOpen,addr savename,OPEN_ONLY,READ_ONLY
        jc end_error
        mov bx,ax

        invoke FRead,bx,4,addr nmap
        invoke FRead,bx,4,addr pkt

        invoke FClose,bx

        call CloseMap
        mov eax,pkt
        mov pktatstart,eax
        mov playerkeys,0
        mov atime,0
        fldz
        fstp ftime
        mov eax,offset timerstruct
        mov ebx,TIME_FLAGS_SEC_TIME
        call InitTimeStruct
        push nmap
        call LoadMap
        mov eax,offset statetxt0
        mov actualstatetxt,eax
        mov eax,1
        ret
end_error:
        xor eax,eax
        ret
LoadGame endp

SaveGame proc stdcall uses ebx
        cmp maploaded,0
        jz end_error
        invoke FOpen,addr savename,CREATE_ONLY,WRITE_ONLY
        jc end_error
        mov bx,ax

        invoke FWrite,bx,4,addr nmap
        invoke FWrite,bx,4,addr pktatstart

        invoke FClose,bx
        mov eax,1
        ret
end_error:
        xor eax,eax
        ret
SaveGame endp

AddPlayerToList proc stdcall uses eax ebx ecx edx
        dec nmap
        mov actplp,-1
        cmp nplayerlist,0
        jnz apld1
        mov eax,dword ptr pplayername
        mov dword ptr PlayerList[0].player_name,eax
        mov eax,pkt
        mov PlayerList[0].player_pkt,eax
        mov eax,nmap
        mov PlayerList[0].player_nmap,eax
        mov actplp,0
        inc nplayerlist
        jmp aplendp
apld1:
        mov ecx,nplayerlist
        xor ebx,ebx
        mov eax,pkt
apll1:
        cmp PlayerList[ebx].player_pkt,eax
        jae apld2
        push ebx
        mov ebx,nplayerlist
        dec ebx
        imul ebx,sizeof (TPlayerList)
        push ecx
apll2:
        mov dl,PlayerList[ebx].player_name[0]
        mov PlayerList[ebx+sizeof (TPlayerList)].player_name[0],dl
        mov dl,PlayerList[ebx].player_name[1]
        mov PlayerList[ebx+sizeof (TPlayerList)].player_name[1],dl
        mov dl,PlayerList[ebx].player_name[2]
        mov PlayerList[ebx+sizeof (TPlayerList)].player_name[2],dl
        mov edx,PlayerList[ebx].player_pkt
        mov PlayerList[ebx+sizeof (TPlayerList)].player_pkt,edx
        mov edx,PlayerList[ebx].player_nmap
        mov PlayerList[ebx+sizeof (TPlayerList)].player_nmap,edx
        sub ebx,sizeof (TPlayerList)
        loop apll2
        pop ecx
        pop ebx
        mov eax,dword ptr pplayername
        mov dword ptr PlayerList[ebx].player_name,eax
        mov eax,pkt
        mov PlayerList[ebx].player_pkt,eax
        mov eax,nmap
        mov PlayerList[ebx].player_nmap,eax
        mov eax,nplayerlist
        sub eax,ecx
        mov actplp,eax
        cmp nplayerlist,maxplayerlist
        jae aplendp
        inc nplayerlist
        jmp aplendp

apld2:
        add ebx,sizeof (TPlayerList)
        dec ecx
        jnz apll1

        cmp nplayerlist,maxplayerlist
        je aplendp
        mov eax,dword ptr pplayername
        mov dword ptr PlayerList[ebx].player_name,eax
        mov eax,pkt
        mov PlayerList[ebx].player_pkt,eax
        mov eax,nmap
        mov PlayerList[ebx].player_nmap,eax
        mov eax,nplayerlist
        mov actplp,eax
        inc nplayerlist
aplendp:
        call SavePlayerList
        mov nmap,1
        ret
AddPlayerToList endp

LoadPlayerList proc stdcall uses eax ebx esi
        invoke FOpen,addr playerlistname,OPEN_ONLY,READ_ONLY
        jc end_error
        mov bx,ax

        invoke FRead,bx,4,addr nplayerlist
        mov ecx,nplayerlist
        jecxz end_error2
        mov esi,offset PlayerList
lpll1:
        invoke FRead,bx,sizeof (TPlayerList),esi
        add esi,sizeof (TPlayerList)
        loop lpll1

end_error2:
        invoke FClose,bx

end_error:
        ret
LoadPlayerList endp

SavePlayerList proc stdcall uses eax ebx esi
        cmp nplayerlist,0
        je end_error

        invoke FOpen,addr playerlistname,CREATE_ONLY,WRITE_ONLY
        jc end_error
        mov bx,ax

        invoke FWrite,bx,4,addr nplayerlist
        mov ecx,nplayerlist
        mov esi,offset PlayerList
lpll1:
        invoke FWrite,bx,sizeof (TPlayerList),esi
        add esi,sizeof (TPlayerList)
        loop lpll1

        invoke FClose,bx

end_error:
        ret
SavePlayerList endp

GetPlayerName proc stdcall
        GetKey
        mov ax,stub32inthandrec.__flg
        and ax,64
        jnz gpnendp
        ReadKey
        cmp al,13
        jne gpnd1
        call InitKB
        mov endgame,2
        mov eax,offset statetxt0
        mov actualstatetxt,eax
        mov ppnnpoz,0
        call AddPlayerToList
        ret
gpnd1:
        cmp al,33
        jb gpnendp
        cmp al,'z'
        ja gpnendp
        mov ebx,ppnnpoz
        mov pplayername[ebx],al
        inc ppnnpoz
        cmp ppnnpoz,3
        jb gpnendp
        mov ppnnpoz,0
gpnendp:
        ret
GetPlayerName endp

HandleEv proc stdcall uses ecx
        cmp al,245
        jb hebluecn
        cmp al,253
        ja hebluecn
        xor dl,dl
        ret
hebluecn:
        push edx
        cmp al,228
        je heend
        or dl,dl
        jnz heend
        xor ecx,ecx
        cmp eax,doorcoltab
        jne hedctn1
        mov cl,1
        jmp hedcto
hedctn1:
        cmp eax,doorcoltab+4
        jne hedctn2
        mov cl,2
        jmp hedcto
hedctn2:
        cmp eax,doorcoltab+8
        jne hedctn3
        mov cl,3
        jmp hedcto
hedctn3:
        dec eax
        cmp eax,doorcoltab
        jne hedctn4
        mov cl,1
        mov ch,1
        jmp hedcto
hedctn4:
        cmp eax,doorcoltab+4
        jne hedctn5
        mov cl,2
        mov ch,1
        jmp hedcto
hedctn5:
        cmp eax,doorcoltab+8
        jne hedctn6
        mov cl,3
        mov ch,1
        jmp hedcto
hedctn6:
        cmp eax,229
        jb heend
        cmp eax,231
        ja heend
        ;opoment
        mov playerkill,1
        pop edx
        ret
hedcto:
        push ebx
        push ecx
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
        mov ecx,eax
        mov eax,edi
        sub eax,backscrsurface.SBits
        xor edx,edx
        mov ebx,scrsx
        ;eax=edi/scrsx  eax=scry edx=scrx
        div ebx
        push edx
        mov ebx,cell_size
        xor edx,edx
        ;eax=scry/cell_size
        div ebx
        pop edx
        push eax
        mov eax,edx
        xor edx,edx
        ;eax=scrx/cellsize
        div ebx
        mov edx,eax
        pop eax
        pop ebx

        ;eax=y edx=x
        sub eax,ebx
        sub edx,ecx
        ;eax=y*LabMap.map_width+x
        mov ecx,edx
        mov ebx,LabMap.map_width
        mul ebx
        add eax,ecx
        pop ecx
        pop ebx

        shl eax,1
        mov edx,LabMap.map_cells
        add edx,eax
        or ch,ch
        jz nodoor
dooro:
        ;jesli drzwi

        ;czy mamy klucze do drzwi
        cmp cl,1
        jne keynob
        test playerkeys,1
        jz heend
        jmp setnodoor
keynob:
        cmp cl,2
        jne keynos
        test playerkeys,2
        jz heend
        jmp setnodoor

keynos:
        cmp cl,3
        jne heend
        test playerkeys,4
        jz heend
setnodoor:
        cmp byte ptr es:[edx+1],0
        jnz setdooro
        cmp byte ptr es:[edx+ebx+1],0
        jz heend
setdooro:
        mov byte ptr es:[edx+1],0
        mov byte ptr es:[edx+ebx+1],0
        pop edx
        mov dl,1
        mov al,255
        ret

nodoor:
        mov al,es:[edx]
        shr al,4
        and al,7
        or al,al
        jz heend
        cmp al,4
        jb key
        ;a wiec braz, srebro lub zloto
        cmp pktt,1
        je nopkt
        sub al,4
        movzx eax,al
        mov eax,pkttab[eax*4]
        add pkt,eax
        and byte ptr es:[edx],15
        mov pktt,1
nopkt:
        pop edx
        mov dl,1
        mov al,255
        ret
key:
        mov al,1
        dec cl
        shl al,cl
        or playerkeys,al
        and byte ptr es:[edx],15
        pop edx
        mov dl,1
        mov al,255
        ret
heend:
        pop edx
        xor dl,dl
        mov al,228
        ret
HandleEv endp

HandleCol proc stdcall uses eax ebx ecx edx edi
        mov eax,playerpozy
        mov ebx,scrsx
        mul ebx
        add eax,playerpozx
        mov edi,backscrsurface.SBits
        add edi,eax
        push edi
        inc edi
        mov es,stub32zerosel
        mov ecx,5
        mov ebx,LabMap.map_width
        shl ebx,1
        neg ebx
        xor edx,edx
        mov pktt,0
hcsp1:
        xor eax,eax
        mov al,es:[edi]
        call HandleEv
        cmp al,229
        jb hcq1
        inc edi
        loop hcsp1
        xor al,al
hcq1:
        or al,al
        jz hcd1
        mov dh,1
        inc edi
        dec ecx
        or ecx,ecx
        jnz hcsp1

hcd1:
        or dh,dh
        jz hcdh1
        inc playerpozy
hcdh1:
        pop edi
        push edi
        add edi,scrsx+6
        mov ecx,5
        mov ebx,-2
        xor edx,edx
hcsp2:
        xor eax,eax
        mov al,es:[edi]
        call HandleEv
        cmp al,229
        jb hcq2
        add edi,scrsx
        loop hcsp2
        xor al,al
hcq2:
        or al,al
        jz hcd2
        mov dh,1
        add edi,scrsx
        dec ecx
        or ecx,ecx
        jnz hcsp2

hcd2:
        or dh,dh
        jz hcdh2
        dec playerpozx
hcdh2:
        pop edi
        push edi
        add edi,scrsx*6+5
        mov ecx,5
        mov ebx,LabMap.map_width
        shl ebx,1
        neg ebx
        xor edx,edx
hcsp3:
        xor eax,eax
        mov al,es:[edi]        
        call HandleEv
        cmp al,229
        jb hcq3
        dec edi
        loop hcsp3
        xor al,al
hcq3:
        or al,al
        jz hcd3
        mov dh,1
        dec edi
        dec ecx
        or ecx,ecx
        jnz hcsp3

hcd3:
        or dh,dh
        jz hcdh3
        dec playerpozy
hcdh3:

        pop edi
        add edi,scrsx*5
        mov ecx,5
        mov ebx,-2
        xor edx,edx
hcsp4:
        xor eax,eax
        mov al,es:[edi]
        call HandleEv
        cmp al,229
        jb hcq4
        sub edi,scrsx
        loop hcsp4
        xor al,al
hcq4:
        or al,al
        jz hcd4
        mov dh,1
        sub edi,scrsx
        dec ecx
        or ecx,ecx
        jnz hcsp4

hcd4:
        or dh,dh
        jz hcdh4
        inc playerpozx
hcdh4:
        call IsEnd
        ret
HandleCol endp

IsEnd proc stdcall
        local prx,pry:dword
        ;eax=scrsx/cell_size/2-LabMap.map_width/2
        mov ebx,LabMap.map_width
        mov eax,scrsx/cell_size/2
        shr ebx,1
        sub eax,ebx
        mov prx,eax
        ;ebx=scrsy/cell_size/2-LabMap.map_height/2
        mov ecx,LabMap.map_height
        mov ebx,scrsy/cell_size/2
        shr ecx,1
        sub ebx,ecx
        mov pry,ebx

        xor edx,edx
        mov eax,playerpozx
        dec eax
        mov ebx,cell_size
        div ebx
        sub eax,prx
        cmp eax,LabMap.map_end_x
        jne ieespr1

        xor edx,edx
        mov eax,playerpozy
        mov ebx,cell_size
        div ebx
        sub eax,pry
        cmp eax,LabMap.map_end_y
        jne ieespr1
        call NextLevel
        jmp ieespr4

ieespr1:
        xor edx,edx
        mov eax,playerpozx
        add eax,6
        mov ebx,cell_size
        div ebx
        sub eax,prx
        cmp eax,LabMap.map_end_x
        jne ieespr2

        xor edx,edx
        mov eax,playerpozy
        mov ebx,cell_size
        div ebx
        sub eax,pry
        cmp eax,LabMap.map_end_y
        jne ieespr2
        call NextLevel
        jmp ieespr4

ieespr2:
        xor edx,edx
        mov eax,playerpozx
        add eax,6
        mov ebx,cell_size
        div ebx
        sub eax,prx
        cmp eax,LabMap.map_end_x
        jne ieespr3

        xor edx,edx
        mov eax,playerpozy
        add eax,7
        mov ebx,cell_size
        div ebx
        sub eax,pry
        cmp eax,LabMap.map_end_y
        jne ieespr3
        call NextLevel
        jmp ieespr4

ieespr3:
        xor edx,edx
        mov eax,playerpozx
        dec eax
        mov ebx,cell_size
        div ebx
        sub eax,prx
        cmp eax,LabMap.map_end_x
        jne ieespr4

        xor edx,edx
        mov eax,playerpozy
        add eax,7
        mov ebx,cell_size
        div ebx
        sub eax,pry
        cmp eax,LabMap.map_end_y
        jne ieespr4
        call NextLevel
ieespr4:
        ret
IsEnd endp

SetOppByte proc stdcall uses eax ebx edx,sobpomv:dword
        assume edi:ptr TOpp
        xor eax,eax
        mov al,es:[edi].opp_dir
        inc al
        xor edx,edx
        mov ebx,4
        div ebx
        mov edx,opp_dirtabh[edx*4]
        mov ebx,sobpomv
        add ebx,edx
        mov al,es:[esi+ebx]
        mov ebx,doorcoltab+4
        cmp al,bl
        je sobendp
        cmp al,229
        jb sobo1
        mov ebx,doorcoltab
        inc bl
        cmp al,bl
        je sobo1
        mov ebx,doorcoltab+4
        inc bl
        cmp al,bl
        je sobo1
        mov ebx,doorcoltab+8
        inc bl
        cmp al,bl
        je sobo1
        cmp al,237
        jb sobendp
        cmp al,245
        jb sobo1
        jmp sobendp
sobo1:
        mov es:[edi].opp_byte,1
sobendp:
        ret
        assume edi:nothing
SetOppByte endp

OppByteTurn proc stdcall uses eax ebx ecx edx,obtpomv:dword
        assume edi:ptr TOpp
        cmp es:[edi].opp_dir,0
        je opty
        cmp es:[edi].opp_dir,2
        je opty
        mov eax,es:[edi].opp_x
        xor edx,edx
        mov ebx,10
        div ebx
        cmp edx,2
        jne obtendp
        jmp opto
opty:
        mov eax,es:[edi].opp_y
        xor edx,edx
        mov ebx,10
        div ebx
        cmp edx,2
        jne obtendp
opto:
        xor eax,eax
        mov al,es:[edi].opp_dir
        inc al
        xor edx,edx
        mov ebx,4
        div ebx
        mov edx,opp_dirtabh[edx*4]
        mov ebx,obtpomv
        add ebx,edx
        mov al,es:[esi+ebx]
        mov ebx,doorcoltab+4
        cmp al,bl
        je obtendp
        cmp al,229
        jb obtendp
        mov ebx,doorcoltab
        inc bl
        cmp al,bl
        je obtendp
        mov ebx,doorcoltab+4
        inc bl
        cmp al,bl
        je obtendp
        mov ebx,doorcoltab+8
        inc bl
        cmp al,bl
        je obtendp
        cmp al,237
        jb obto
        cmp al,245
        jb obtendp
obto:
        inc es:[edi].opp_dir
        mov es:[edi].opp_byte,0
        ret
obtendp:
        xor edx,edx
        mov dl,es:[edi].opp_dir
        mov edx,opp_dirtabx[edx*4]
        add es:[edi].opp_x,edx
        xor edx,edx
        mov dl,es:[edi].opp_dir
        mov edx,opp_dirtaby[edx*4]
        add es:[edi].opp_y,edx
        ret
        assume edi:nothing
OppByteTurn endp

HandleOpp proc stdcall uses eax ebx ecx edx esi edi
        local prx,pry:dword
        local pom:dword
        cmp maploaded,0
        jz hoendp
        cmp LabMap.opp_n,0
        jz hoendp
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
        mov esi,backscrsurface.SBits
        mov edi,LabMap.opp_tab
        mov es,stub32zerosel
        assume edi:ptr TOpp
holoop:
                mov eax,es:[edi].opp_x
                add eax,prx
                add eax,3
                mov ebx,es:[edi].opp_y
                add ebx,pry
                add ebx,3
                xor edx,edx
                mov dl,es:[edi].opp_dir
                mov edx,opp_dirtabh[edx*4]
                imul ebx,scrsx
                add ebx,eax
                mov pom,ebx
                invoke SetOppByte,pom
                add ebx,edx
                mov al,es:[esi+ebx]
                movzx eax,al
                cmp eax,doorcoltab+4
                je hod
                cmp al,229
                jb hon1
                mov edx,doorcoltab
                inc edx
                cmp al,dl
                je hon1
                mov edx,doorcoltab+4
                inc edx
                cmp al,dl
                je hon1
                mov edx,doorcoltab+8
                inc edx
                cmp al,dl
                je hon1
                cmp al,237
                jb hod
                cmp al,245
                jb hon1
                jmp hod
hon1:
                xor eax,eax
                mov al,es:[edi].opp_dir
                inc al
                xor edx,edx
                mov ebx,4
                div ebx
                mov edx,opp_dirtabh[edx*4]
                mov ebx,pom
                add ebx,edx
                mov al,es:[esi+ebx]
                movzx eax,al
                cmp eax,doorcoltab+4
                je hod2
                cmp al,229
                jb hon2
                mov edx,doorcoltab
                inc edx
                cmp al,dl
                je hon2
                mov edx,doorcoltab+4
                inc edx
                cmp al,dl
                je hon2
                mov edx,doorcoltab+8
                inc edx
                cmp al,dl
                je hon2
                cmp al,237
                jb hod2
                cmp al,245
                jb hon2
hod2:
                inc es:[edi].opp_dir
                mov es:[edi].opp_byte,0
                add edi,sizeof (TOpp)
                dec ecx
                jnz holoop
                jmp hoendp

hon2:
                xor eax,eax
                mov al,es:[edi].opp_dir
                dec al
                xor edx,edx
                mov ebx,4
                div ebx
                mov edx,opp_dirtabh[edx*4]
                mov ebx,pom
                add ebx,edx
                mov al,es:[esi+ebx]
                movzx eax,al
                cmp eax,doorcoltab+4
                je hod3
                cmp al,229
                jb hon3
                mov edx,doorcoltab
                inc edx
                cmp al,dl
                je hon3
                mov edx,doorcoltab+4
                inc edx
                cmp al,dl
                je hon3
                mov edx,doorcoltab+8
                inc edx
                cmp al,dl
                je hon3
                cmp al,237
                jb hod3
                cmp al,245
                jb hon3
hod3:
                dec es:[edi].opp_dir
                mov es:[edi].opp_byte,0
                add edi,sizeof (TOpp)
                dec ecx
                jnz holoop
                jmp hoendp

hon3:
                add es:[edi].opp_dir,2
                mov es:[edi].opp_byte,0
                add edi,sizeof (TOpp)
                dec ecx
                jnz holoop
                jmp hoendp
                
hod:
                cmp es:[edi].opp_byte,1
                jne hod1
                invoke OppByteTurn,pom
                add edi,sizeof (TOpp)
                dec ecx
                jnz holoop
                jmp hoendp
hod1:
                xor edx,edx
                mov dl,es:[edi].opp_dir
                mov edx,opp_dirtabx[edx*4]
                add es:[edi].opp_x,edx
                xor edx,edx
                mov dl,es:[edi].opp_dir
                mov edx,opp_dirtaby[edx*4]
                add es:[edi].opp_y,edx
                mov es:[edi].opp_byte,0
                add edi,sizeof (TOpp)
                dec ecx
                jnz holoop
        assume edi:nothing
hoendp:
        ret
HandleOpp endp

HandleTime proc stdcall uses eax
        local timee:dword,pomtime[1]:dword
        cmp maploaded,0
        jz htendp
        mov eax,offset timerstruct
        call GetTime
        fadd ftime
        fst ftime
        fistp atime
htendp:
        ret
HandleTime endp
