.386
.model flat,C

.data

;xstub32.asm

        public stub32zerosel,stub32zerodesc, \
        cpu_name,cpu_ver,cpu_brand,cpu_feat,cpu_efeat

        ;selektory
        stub32zerosel dw 0      ;selektor zerowy (adres pocz.:00000000h) dla danych
        stub32zerodesc db 0ffh,0ffh,0,0,0,0f2h,0cfh,0
        ;informacje o procesorze
        cpu_name db 13 dup (0)
        cpu_ver dd 0
        cpu_brand dd 0
        cpu_feat dd 0
        cpu_efeat dd 0


;mcon.inc

public MCON_CUR_X,MCON_CUR_Y,MCON_TEXT_ATTR

MCON_CUR_X db 0          ;wsp. x kursora
MCON_CUR_Y db 0          ;wsp. y kursora
MCON_TEXT_ATTR db 7      ;atrybuty (kolor,tlo)


;prc.inc

MAXPAR=31

public prc_parlist

prc_parlist dd MAXPAR dup (0)


;vga.inc

;vesa.inc

;surface8.inc

;zerocpy.inc

;text8x8.inc

;fnt.inc

;kb.inc

key_scantokeytab db 128 dup(0)
key_keytab db 128 dup (0)
key_isshiftp db 0

public key_scantokeytab,key_keytab,key_isshiftp


;time.inc

__cpu__tsc__freq dq 0           ;czestotliwosc taktowania procesora
Time_Get dd 0
__float__time__0_001 real4 0.001
__float__time__1_0 real4 1.0
__float__time__60_0 real4 60.0
__float__time__3600_0 real4 3600.0
__float__time__0_018 real4 0.018
__float__time__18_2 real4 18.2
__float__time__1092 real4 1092.0
__float__time__65520 real4 65520.0
__float__time__temp real4 0.0


public __cpu__tsc__freq,Time_Get
public __float__time__0_001,\
__float__time__1_0,\
__float__time__60_0,\
__float__time__3600_0,\
__float__time__0_018,\
__float__time__18_2,\
__float__time__1092,\
__float__time__65520,\
__float__time__temp


;fade8.inc

FADE8Tab dd 0
FADE8TabShift dd 0
FADE8TabSize dd 0

public FADE8Tab,FADE8TabShift,FADE8TabSize


;transp8.inc

;[scol,dcol,fade]
TRANSP8Tab dd 0
TRANSP8TabShift dd 0
TRANSP8TabSize dd 0

public TRANSP8Tab,TRANSP8TabShift,TRANSP8TabSize


;snd.inc

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

end
