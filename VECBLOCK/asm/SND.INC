;snd.inc for vb_snd.c

.data

snd_freq dd 0
snd_bits db 0
snd_stereo db 0
snd_nchannels dd 0
snd_channels dd 0
snd_sbportexit dw 0
snd_buf dd 0
snd_bufpage db 0

public snd_freq,snd_bits,snd_stereo,snd_nchannels,\
snd_channels,snd_sbportexit,snd_buf,snd_bufpage

.code

TChannel struct 
 ison dd ?
 sndref dd ?
 pos dd ?
 volume dd ?
 bal dd ?
 sndnum dd ?
 islopped dd ?
TChannel ends

TSnd struct
 bits dd ?
 ssize dd ?
 ;dane dodatkowe na temat dzwieku 
 freq dd ?
 bytesize db ?
 stereo db ?            ;mono/stereo
TSnd ends


SND_BUFSIZE=1024
SND_VOLUMEBITS=8

extern snd_freq:dword,snd_bits:byte,snd_stereo:byte,snd_nchannels:dword,snd_channels:dword,snd_sbportexit:word,snd_buf:dword,snd_bufpage:byte

__Snd_Int_Func proc C
        assume esi:ptr TChannel
        assume ebx:ptr TSnd
        local cntbuf:dword              ;licznik kanalow
        local pbufsnd:dword             ;pozycja bufora dla dzwieku(TSnd)

;Inicjacja
        pushad
        push es
        ;cli
        mov pbufsnd,0
        mov es,stub32zerosel
        mov word ptr es:[0b8000h],1111h
        mov edi,snd_buf
        mov ecx,SND_BUFSIZE/2

        mov cntbuf,ecx
        cmp snd_bufpage,0
        jz loop1_normal_mono_8
        add edi,SND_BUFSIZE/2

;normalny zapis mono 8-bitowy
loop1_normal_mono_8:
                mov esi,snd_channels
                mov ecx,snd_nchannels
                xor edx,edx
loop2_normal_mono_8:
                        cmp [esi].ison,0
                        jz no_channelused_normal_mono_8
                        mov ebx,[esi].sndref
                        mov ebx,[ebx].bits
                        add ebx,[esi].pos
                        add ebx,pbufsnd
                        mov eax,128
                        sub al,byte ptr [ebx]           ;bajt z dzwieku
                        cbw
                        cwde
                        imul eax,[esi].volume
                        sar eax,SND_VOLUMEBITS
                        add edx,eax
no_channelused_normal_mono_8:
                        add esi,sizeof(TChannel)
                        loop loop2_normal_mono_8
                cmp edx,0                               ;jesli byl dzwiek
                jnz is_snd_normal_mono_8
                mov al,128
                jmp addsbyte_normal_mono_8
is_snd_normal_mono_8:
                mov eax,edx
                cmp eax,127
                jng less_127_normal_mono_8
                mov al,127
                jmp addsbyte_normal_mono_8
less_127_normal_mono_8:
                cmp eax,-128
                jnl greater_m128_normal_mono_8
                mov al,-128
                jmp addsbyte_normal_mono_8
greater_m128_normal_mono_8:
                add al,128
addsbyte_normal_mono_8:
                stosb
                inc pbufsnd
                dec cntbuf
                jnz loop1_normal_mono_8

;...(inne tryby)

;aktualizacja tablic snd_channels
        mov ecx,snd_nchannels
        mov esi,snd_channels
upgrade_c_tab:
                cmp [esi].ison,0
                jz ug_d1
                add [esi].pos,SND_BUFSIZE/2
                mov eax,[esi].pos
                add eax,SND_BUFSIZE/2
                mov ebx,[esi].sndref                
                cmp eax,[ebx].ssize
                jbe ug_d1
                mov [esi].ison,0
ug_d1:
                add esi,sizeof(TChannel)
                loop upgrade_c_tab

        ;procedury koncowe
        ;snd_bufpage=1-snd_bufpage
        xor snd_bufpage,1
        and snd_bufpage,1
        ;dla 8-bit:2xe, dla 16-bit:2xf
        sti
        mov dx,snd_sbportexit
        in al,dx
        mov al,20h
        out 20h,al
        pop es
        popad
        iretd
        assume esi:nothing
        assume ebx:nothing
__Snd_Int_Func endp
