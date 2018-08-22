;TEXT8X8.INC for DJGPP by Kamil Korolkiewicz 2005/2006
;Funkcje do wyswietlania stringow na powierzchniach ze zbioru liter 8X8

.code

;funkcje

;--------------------------------
;DrawString8X8 (int,int,int,void*,char*,int,TSurf*)
;       Rysuje ciag ASCIIZ na powierzchni 8-bitowej
;--------------------------------

DrawString8X8 proc C uses ebx esi edi ,x:dword,y:dword,col:dword,charsstart:dword,txt:dword,slen:dword,surf:dword
        local shfx:dword
        local chars_height:dword
        local chars_ystart:dword
        local xl:dword,xl2:dword,xs:dword,xw:dword,clstrt:dword,sstart:dword
        ;
        mov chars_height,8
        mov chars_ystart,0
        mov xs,0
        mov xw,8
        mov edi,surf

        ;Sprawdzenie czy tekst miesci sie na ekranie
        mov eax,y
        cmp eax,-8
        jle DrawString8X8_end_proc        
        cmp eax,[edi+4]         ;SHeight
        jge DrawString8X8_end_proc
        ;
        cmp eax,0
        jge DrawString8X8_yge0
        neg eax
        mov chars_ystart,eax
        mov y,0
DrawString8X8_yge0:
        add eax,8
        cmp eax,[edi+4]         ;SHeight
        jle DrawString8X8_ylesh
        sub eax,[edi+4]
        sub chars_height,eax
DrawString8X8_ylesh:
        ;
        mov esi,txt
        mov eax,x
        cmp eax,[edi]           ;SWidth
        jge DrawString8X8_end_proc
        ;
        cmp eax,0
        jge DrawString8X8_xge0
        neg eax
        mov edx,eax
        shr eax,3
        add esi,eax
        sub slen,eax
        cmp slen,0
        jle DrawString8X8_end_proc
        ;xs=-x-p*8
        shl eax,3
        sub edx,eax
        mov xs,edx
        mov x,0
        ;
DrawString8X8_xge0:        
        mov edx,x
        sub edx,xs
        mov eax,slen
        shl eax,3               ;eax*8
        add edx,eax
        cmp edx,[edi]           ;SWidth
        jle DrawString8X8_no_sub_slen
        sub edx,[edi]           ;SWidth
        shr edx,3
        sub slen,edx
        cmp slen,0
        jle DrawString8X8_end_proc
        mov edx,x
        sub edx,xs
        mov eax,slen
        shl eax,3               ;eax*8
        add edx,eax
        sub edx,[edi]           ;SWidth
        mov ecx,8
        sub ecx,edx
        mov xw,ecx
DrawString8X8_no_sub_slen:

        ;Wyliczenie poczatku (eax=x ebx=y)
        mov eax,y
        mov ecx,[edi]           ;SWidth
        mov shfx,ecx        
        mul ecx
        add eax,x        
        mov edi,[edi+8]         ;SBits
        add edi,eax             ;y*SWidth+x+SBits
        ;
        mov sstart,esi

        ;Rysowanie: edi=Surface.SBits edx=bios_chars_ba                
DrawString8X8_draw_loop:
                movzx eax,byte ptr [esi]        ;pobranie znaku                
                mov edx,charsstart
                shl eax,3
                add edx,eax
                mov ebx,chars_ystart
                mov xl,8
                mov clstrt,8
                push edi
                cmp esi,sstart
                jne DrawString8X8_esinesstart                
                mov eax,xs
                sub xl,eax
                sub clstrt,eax
DrawString8X8_esinesstart:
                cmp slen,1
                jne DrawString8X8_loop_y
                mov eax,xw
                mov xl,eax
DrawString8X8_loop_y:
                        mov eax,xl
                        mov xl2,eax
                        mov al,[edx+ebx]
                        mov ecx,clstrt
DrawString8X8_loop_x:
                                push eax
                                shl eax,1
                                shr eax,cl
                                and eax,1
                                or al,al
                                jz DrawString8X8_no_put_pixel
                                mov eax,col
                                mov [edi],al
DrawString8X8_no_put_pixel:
                                inc edi
                                pop eax
                                dec ecx
                                dec xl2
                                jnz DrawString8X8_loop_x
                        add edi,shfx
                        sub edi,xl
                        inc ebx
                        cmp ebx,chars_height
                        jb DrawString8X8_loop_y
                pop edi
                add edi,xl
                inc esi
                dec slen
                jnz DrawString8X8_draw_loop
        mov eax,1
        jmp DrawString8X8_end_proc_ok
DrawString8X8_end_proc:
        xor eax,eax
DrawString8X8_end_proc_ok:
        ret
DrawString8X8 endp

;--------------------------------
;DrawString8X8S (int,int,TSurf*,void*,char*,int,TSurf*)
;       Rysuje ciag ASCIIZ na powierzchni 8-bitowej z powierzchnia w tle
;       Wykorzystywane sa tylko pierwsze 64 (8*8) bajty powierzchni oraz
;       powierzchnia musi byc rozmiarow 8X8.
;--------------------------------

DrawString8X8S proc C uses ebx esi edi ,x:dword,y:dword,bsurf:dword,charsstart:dword,txt:dword,slen:dword,surf:dword
        local shfx:dword
        local chars_height:dword
        local chars_ystart:dword
        local xl:dword,xl2:dword,xs:dword,xw:dword,clstrt:dword,sstart:dword
        local bsl:dword
        ;
        mov chars_height,8
        mov chars_ystart,0
        mov xs,0
        mov xw,8
        mov edi,surf

        ;Sprawdzenie czy tekst miesci sie na ekranie
        mov eax,y
        cmp eax,-8
        jle DrawString8X8S_end_proc        
        cmp eax,[edi+4]         ;SHeight
        jge DrawString8X8S_end_proc
        ;
        cmp eax,0
        jge DrawString8X8S_yge0
        neg eax
        mov chars_ystart,eax
        mov y,0
DrawString8X8S_yge0:
        add eax,8
        cmp eax,[edi+4]         ;SHeight
        jle DrawString8X8S_ylesh
        sub eax,[edi+4]
        sub chars_height,eax
DrawString8X8S_ylesh:
        ;
        mov esi,txt
        mov eax,x
        cmp eax,[edi]           ;SWidth
        jge DrawString8X8S_end_proc
        ;
        cmp eax,0
        jge DrawString8X8S_xge0
        neg eax
        mov edx,eax
        shr eax,3
        add esi,eax
        sub slen,eax
        cmp slen,0
        jle DrawString8X8S_end_proc
        ;xs=-x-p*8
        shl eax,3
        sub edx,eax
        mov xs,edx
        mov x,0
        ;
DrawString8X8S_xge0:        
        mov edx,x
        sub edx,xs
        mov eax,slen
        shl eax,3               ;eax*8
        add edx,eax
        cmp edx,[edi]           ;SWidth
        jle DrawString8X8S_no_sub_slen
        sub edx,[edi]           ;SWidth
        shr edx,3
        sub slen,edx
        cmp slen,0
        jle DrawString8X8S_end_proc
        mov edx,x
        sub edx,xs
        mov eax,slen
        shl eax,3               ;eax*8
        add edx,eax
        sub edx,[edi]           ;SWidth
        mov ecx,8
        sub ecx,edx
        mov xw,ecx
DrawString8X8S_no_sub_slen:

        ;Wyliczenie poczatku (eax=x ebx=y)
        mov eax,y
        mov ecx,[edi]           ;SWidth
        mov shfx,ecx        
        mul ecx
        add eax,x        
        mov edi,[edi+8]         ;SBits
        add edi,eax             ;y*SWidth+x+SBits
        ;
        mov sstart,esi

        ;Rysowanie: edi=Surface.SBits edx=bios_chars_ba                
DrawString8X8S_draw_loop:
                movzx eax,byte ptr [esi]        ;pobranie znaku                
                mov edx,charsstart
                shl eax,3
                add edx,eax
                mov ebx,chars_ystart
                mov xl,8
                mov clstrt,8
                mov bsl,0
                push edi
                cmp esi,sstart
                jne DrawString8X8S_esinesstart                
                mov eax,xs
                sub xl,eax
                sub clstrt,eax
DrawString8X8S_esinesstart:
                cmp slen,1
                jne DrawString8X8S_loop_y
                mov eax,xw
                mov xl,eax
DrawString8X8S_loop_y:
                        mov eax,xl
                        mov xl2,eax
                        mov al,[edx+ebx]
                        mov ecx,clstrt
DrawString8X8S_loop_x:
                                push eax
                                shl eax,1
                                shr eax,cl
                                and eax,1
                                or al,al
                                jz DrawString8X8S_no_put_pixel
                                mov eax,bsurf
                                mov eax,[eax+8]
                                add eax,bsl
                                mov al,[eax]
                                mov [edi],al
DrawString8X8S_no_put_pixel:
                                inc edi
                                inc bsl
                                pop eax
                                dec ecx
                                dec xl2
                                jnz DrawString8X8S_loop_x
                        add edi,shfx
                        sub edi,xl
                        inc ebx
                        cmp ebx,chars_height
                        jb DrawString8X8S_loop_y
                pop edi
                add edi,xl
                inc esi
                dec slen
                jnz DrawString8X8S_draw_loop
        mov eax,1
        jmp DrawString8X8S_end_proc_ok
DrawString8X8S_end_proc:
        xor eax,eax
DrawString8X8S_end_proc_ok:
        ret
DrawString8X8S endp
