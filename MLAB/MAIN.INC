;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
;MLAB copyright (c) Kamil Korolkiewicz 2005
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


;includy

;mstdlib
include mstdlib\mcon.inc
include mstdlib\mmem.inc
include mstdlib\famem.inc
include mstdlib\mtime.inc
include mstdlib\sprintf.inc
include mstdlib\strings.inc
include mstdlib\fileio.inc
include mstdlib\params.inc
include mstdlib\strings.inc

;graficzne
include surface.inc
include surface8.inc
include surface8c.inc
include bmp8.inc
include vga.inc
include vesa.inc
include vgatext.inc

;klawiatura
include kb.inc


;stale

cell_size equ 10
scrsx equ 640
scrsy equ 480
scrmode equ V640X480X256
maxplayerlist equ 10


;deklaracje struktur

TCell struct    ;jedna komorka mapy
        cell_type db ?          ;typ komurki [0..3] oraz artefakty [4..7]:
        ;0-brak
        ;1-klucz brazowy
        ;2-klucz srebny
        ;3-klucz zloty
        ;4-braz
        ;5-srebro
        ;6-zloto
        cell_door db ?          ;okresla ktore sciany sa drzwiami [0..3] oraz
        ;kolor drzwi [4..5]
        ;0-brazowe
        ;1-srebne
        ;2-zlote
TCell ends

TOpp struct     ;przeciwnik
        opp_x dd ?              ;pozycja x przeciwnika
        opp_y dd ?              ;pozycja y przeciwnika
        opp_dir db ?            ;kierunek przeciwnika
        opp_byte db ?           ;bajt pomocniczy
TOpp ends

TMap struct     ;labirynt
        map_width dd ?          ;szerokosc mapy
        map_height dd ?         ;wysokosc mapy
        map_start_x dd ?        ;poczatek mapy x
        map_start_y dd ?        ;poczatek mapy y
        map_end_x dd ?          ;koniec mapy x
        map_end_y dd ?          ;koniec mapy y
        opp_n dd ?              ;liczba przeciwnikow
        map_cells dd ?          ;wskaznik do komurek mapy (TCell)
        opp_tab dd ?            ;wskaznik do tablicy przeciwnikow
TMap ends

TPlayerList struct
        player_name db 3 dup (?)
        player_pkt dd 0
        player_nmap dd 0
TPlayerList ends

;dane

celltypes db 0,0,0,0 ;          0
                     ;
          db 1,0,0,0 ;  |       1
                     ;   _
          db 0,1,0,0 ;          2
                     ;
          db 0,0,1,0 ;    |     3
                     ;
          db 0,0,0,1 ;   _      4
                     ;   _
          db 1,1,0,0 ;  |       5
                     ;   _
          db 0,1,1,0 ;    |     6
                     ;
          db 0,0,1,1 ;   _|     7
                     ;
          db 1,0,0,1 ;  |_      8
                     ;   _
          db 1,1,1,0 ;  | |     9
                     ;   _
          db 0,1,1,1 ;   _|     10
                     ;
          db 1,0,1,1 ;  |_|     11
                     ;   _
          db 1,1,0,1 ;  |_      12
                     ;
          db 1,0,1,0 ;  | |     13
                     ;   _
          db 0,1,0,1 ;   _      14
                     ;   _
          db 1,1,1,1 ;  |_|     15

doorcoltab dd 232
dd 158,254

gfxfname db 'g.bmp',0

pal dd 0

tbuf1 db 256 dup (0)
tbuf2 db 256 dup (0)

entrystring db 'Copyright (c) Kamil Korolkiewicz 2005. Written in MASM/STUB32.',0
entry_pval dd 1
entry_pval2 db 0
entryscrtm dd 0

scrsurface TSurface <scrsx,scrsy,1,scrsx,0a0000h>
backscrsurface TSurface <scrsx,scrsy,1,scrsx,0>
backsurface TSurface <scrsx,scrsy,1,scrsx,0>
gfxsurface TSurface <>

ssrect TRect <>
dsrect TRect <>

quit db 0
endgame dd 0
maploaded dd 0

playerpozx dd 0
playerpozy dd 0

playerkeys db 0

LabMap TMap <>

mapname1 db 'Lab',0
mapname2 db '.map',0
nmap dd 1
savename db 'game.sav',0

map_cells_famem dd 0
opp_tab_famem dd 0

opp_loaddword dd 0
opp_dirtab dd -scrsx*3
dd 3,scrsx*3,-3

opp_dirtabh dd -scrsx*5
dd 5,scrsx*5,-5

opp_dirtabx dd 0
dd 1,0,-1

opp_dirtaby dd -1
dd 0,1,0

pkttab dd 250000
dd 500000,1000000

pkt dd 0
pktt db 0
pktatstart dd 0

infotxt db 'Poziom nr %02u      Klucze:         Punkty:%010u    Czas:%u',0
fpstxt db 'fps:%u',0

menuon dd 1
menupoz dd 0

atime dd 0
ftime real4 0.0
timerstruct Time <>

framet Time <>
fps real4 0.0
fps2 real4 0.0
fpsu dd 0
dfps db 0
iswait4vsync db 1

playerkill db 0

statetxt0 db 0
statetxt1 db 'Przegraˆe˜!',0
statetxt2 db 'Gratulacje! Ukoäczyˆe˜ gr©!',0
actualstatetxt dd statetxt0

paramname1 db '-nowait4vsync',0
paramname2 db '-drawfps',0

PlayerList TPlayerList maxplayerlist+1 dup (<>)
nplayerlist dd 0
playerlistname db 'player.lst',0
playerlisttxt db 'Najlepsze wyniki:',0
playerlisttxt1 db 'Nazwa        Punkty          Ilo˜† poziom¢w',0
playerlisttxt2 db '%s         ³%015u³%02u             ',0
pplayernametxt db 'Podaj swoje inicjaˆy (3 znaki) i naci˜nij [ENTER]:',0
pplayername db 'pla'
db 0
ppnnpoz dd 0
actplp dd 0

islfb db 0
