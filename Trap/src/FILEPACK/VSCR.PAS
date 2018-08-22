{
  TRAP (r) Copyright (c) Kamil Korolkiewicz 2003
  Darmowa biblioteka graficzna do obsˆugi wysokich rozdzielczo˜ci przy 256 kolorach stworzona na potzeby gry TRAP (r)
  Wszelkie prawa autorskie zastrze¾one
}

(*                      UWAGA!!!!!!!!!!!!!!!!!!!!!UWAGA
   _______________________________________________________________________________________________________
   |WSZYSTKIE PROCEDURY, FUNKCJE, TRYBY I TABLICE Z NIMI ODNOSZA SIE DO 256 KOLOROWYCH TRYBOW GRAFICZNYCH|
   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
*)

unit vscr;

interface

uses scr;


{deklaracje stalych}

const

vesainfget : boolean = false; {czy zostaly juz pobrane informacje o karcie}
modeinfget : boolean = false; {{czy zostaly juz pobrane informacje o trybie}
vesaset : boolean = false; {czy jest ustawiony tryb graficzny}

{deklaracje typ¢w}

type

vesainformation = record {record w ktorym przechowywane sa informacje o karcie}
                vesa_sygnature : array [0..3] of char; {sygnaturka vesy 'vesa'}
                vesa_version : word; {werja vesy}
                pointer_to_oem_name : pointer; {wskaznik do nazwy karty}
                capabilites_flags : longint;
                list_of_all_modes : pointer; {wskaznik do listy wszystkich trybow vesy}
                total_video_banks : word; {liczba bankow karty}
                OEM_soft_ver : word; {oem'owa wersja karty}
                pointer_to_vend_name : pointer; {nazwa firmy}
                pointer_to_product_name : pointer; {nazwa produktu}
                point_to_prod_rev_str : pointer; {nazwa produktu 2}
                reservedall : array [0..221] of byte; {zarezerwowane}
                OEM_scratchpad : array [0..255] of byte; {dla vbe 2.0> notatki}
end;

modeinformation = record {record w ktorym sa przechowywane informacje o danym trybie graficzym}
                mode_attributes : word;
                window_a_attributes : byte;
                window_b_attributes : byte;
                win_granularity_in_kb : word;
                win_size_in_kb : word;
                start_seg_of_win_a : word;
                start_seg_of_win_b : word;
                FAR_win_positioning_fun : pointer;
                bytes_per_scan_line : word;
                width_in_pixels : word;
                height_in_pixels : word;
                width_of_char_cell_in_pixels : byte;
                height_of_char_cell_in_pixels : byte;
                num_of_mem_plan : byte;
                num_of_bpp : byte;
                num_of_bank : byte;
                mem_model_t : byte;
                size_of_bank_in_kb : byte;
                num_of_img_pag : byte;
                reserved1 : byte;
                R_mask_size : byte;
                R_field_position : byte;
                G_mask_size : byte;
                G_field_position : byte;
                B_mask_size : byte;
                B_field_position : byte;
                reserv_mask_size : byte;
                reserv_mask_pos : byte;
                direct_col_mode_inf : byte;
                physical_addr_of_linear_vid_buf : pointer;
                pointer_to_start_of_offscr_mem : pointer;
                kb_of_offscr_mem : word;
                reservedall : array [0..205] of byte;
end;


{deklaracje zmiennych}

var

vi : ^vesainformation; {record w ktorym przechowywane sa informacje o karcie}
mi : ^modeinformation;
vscrwidth{x},vscrheight{y} : word; {dlugosc i szerokosc ekranu}
num_of_used_vid_banks : byte;
actual_bank : byte;
actual_drw_bank : byte;
page_2_bank_start : byte;
page_2_x_s,page_2_y_s : word;
act_x,act_y : word;
us_flip : boolean;
is_error : boolean;
actual_error : string;
endpixinendline,endline : array [0..30] of word;
vesa_modes : array [0..8] of record
                                   mode_width,mode_height : word;
                                   mode_bmode : word;
                                end;

{deklaracje procedur i funkcji}

procedure initvesavscr (width,height : word); {vesa on}
procedure closevesavscr; {vesa off}
procedure getallmodesvscr; {pobiera wszyskie informacje o karcie i trybach graficznych}
procedure getvesavscr (var version : word; var totalmemoryinkilobytes : longint; var oemsygnature : string); {pob inf o karcie}
procedure getmodevscr (mode : word; var width,height : word); {pobiera inf. o podanym trybie graf.}
procedure setactivevscr (no : byte); {ustawia jeden z dwoch ekranow na aktywny po ktorym mozemy rysowac}
procedure setvisualvscr (no : byte); {ustawia jeden z dwoch ekranow na do wyswietlania}
procedure fliping; {jezeli aktywny ekr. = 0 to ekr. do rys. = 1 i tak na zmiane (sluzy do plynnych animacji}
procedure vputpixel (x,y : word; c : byte); {pixel}
procedure vputpixeltranspar (x,y : word; c : byte); {rysuje pixa o prze«roczysto˜ci}
function vgetpixel (x,y : word) : byte; {bierze go}
procedure vhline (x1,y1,x2 : word; c : byte); {linia pozioma}
procedure vvline (x1,y1,y2 : word; c : byte); {linia pionowa}
procedure vcls;{czy˜ci ekran na kolor 0}
procedure vscrtexnf (x,y : integer; source : ttex); {rysuje teksture w buforze nie cieniuj¥c go}
procedure vscrtexf (x,y : integer; fadevalue : byte; source : ttex); {rysuje teksture w buforze cieniuj¥c go}
procedure vscrsprtexnf (x,y : integer; source : ttex); {rysuje sprajta w buforze nie cieniuj¥c go}
procedure vscrsprtexf (x,y : integer; fadevalue : byte; source : ttex); {rysuje sprajta w buforze cieniuj¥c go}
procedure vscrtextf (x,y : integer; fadevalue : byte; source : ttex); {rysuje cieniowan¥ texture z prze«roczysto˜ci¥}
procedure vscrsprtextf (x,y : integer; fadevalue : byte; source : ttex); {rysuje cieniowanego sprajta z prze«roczyst.}
{procedure vscrtranspwin (x1,y1,x2,y2 : integer; c : byte); {rysuje okno o kolorze c pod pewn¥ prze«roczyso˜ci¥}


implementation {implementacje procedur i funkcji}


procedure initvesavscr;
var
i : byte;
error : boolean;
mode : word;
w,h,v : word;
m : longint;
sig : string;
begin
     getallmodesvscr;
     vscrwidth := 0;
     vscrheight := 0;
     is_error := false;
     actual_error := 'No errors';
     mode := $ffff;
     us_flip := true;
     for i := 0 to 8 do
     begin
          if (vesa_modes[i].mode_width = width) and (vesa_modes[i].mode_height = height) then
          mode := vesa_modes[i].mode_bmode;
     end;
     if mode = $ffff then
     begin
          is_error := true;
          actual_error := 'No mode supported';
          exit;
     end;
     getmodevscr (mode,w,h);
     if (w = 0) or (h = 0) then
     begin
          is_error := true;
          actual_error := 'No mode supported(?)';
          exit;
     end;
     getvesavscr (v,m,sig);
     if v = $ffff then
     begin
          is_error := true;
          actual_error := 'VESA is not detected';
          exit;
     end;
     if (hi(v) = 1) and (lo(v) = 0) then us_flip := false;
     if (width = 320) and (height = 200) then
     begin
     num_of_used_vid_banks := 0;
     if m <= 3 then
     begin
          us_flip := false;
          if m <= num_of_used_vid_banks then
          begin
               is_error := true;
               actual_error := 'Too small video memory for mode';
               exit;
          end;
     end;
     endpixinendline [0] := 0;
     endline [0] := 203;
     end
     else
     begin
     num_of_used_vid_banks := (longint (width)*longint (height) div 65535);
     if m <= ((num_of_used_vid_banks+1)*2+1) then
     begin
          us_flip := false;
          if m <= num_of_used_vid_banks then
          begin
               is_error := true;
               actual_error := 'Too small video memory for mode';
               exit;
          end;
     end;
     for i := 0 to num_of_used_vid_banks do endpixinendline [i] := (longint (i+1)*65536) mod width;
     for i := 0 to num_of_used_vid_banks do endline [i] := (longint (i+1)*65536) div width;
     end;
     page_2_bank_start := num_of_used_vid_banks+1;
     page_2_x_s := (num_of_used_vid_banks+1)*((65535-(65535 div width)*width)+1);
     page_2_y_s := (num_of_used_vid_banks+1)*(65535 div width);
     asm
        mov ax,4f02h
        mov bx,mode
        int 10h
        mov error,ah
     end;
     if error then
     begin
          is_error := true;
          actual_error := 'Mode can''t init(?)'; {Mode can't init}
          exit;
     end;
     vscrwidth := width;
     vscrheight := height;
     buf := ptr ($a000,0);
     setactivevscr (1);
     setvisualvscr (0);
     actual_drw_bank := 0;
end;

procedure closevesavscr;
begin
     if vesainfget then
     freemem (vi,512);
     if modeinfget then
     freemem (mi,256);
     buf := nil;
     vesainfget := false;
     modeinfget := false;
     vesaset := false;
     vscrwidth := 0;
     vscrheight := 0;
     page_2_bank_start := 0;
     page_2_x_s := 0;
     page_2_y_s := 0;
     us_flip := false;
     num_of_used_vid_banks := 0;
     actual_drw_bank := 0;
     fillchar (endpixinendline [0],num_of_used_vid_banks,0);
     fillchar (endline [0],num_of_used_vid_banks,0);
     asm
        mov ax,3h
        int 10h
     end;
end;

procedure getallmodesvscr;
var
v : word;
memor : longint;
namm : string;
i,j,m,w,h : word;
begin
     getvesavscr (v,memor,namm);
     if v = $ffff then exit;
     with vi^ do
     begin
     m := memw [seg (list_of_all_modes^):ofs (list_of_all_modes^)];
     i := 1;
     j := 0;
     while m <> $ffff do
     begin
          if m > $146 then
          begin
               inc (i);
               m := memw [seg (list_of_all_modes^):ofs (list_of_all_modes^)+i];
               continue;
          end;
          getmodevscr (m,w,h);
          if mi^.num_of_bpp = 8 then
          begin
               vesa_modes [j].mode_bmode := m;
               vesa_modes [j].mode_width := w;
               vesa_modes [j].mode_height := h;
               inc (j);
          end;
          inc (i);
          m := memw [seg (list_of_all_modes^):ofs (list_of_all_modes^)+i];
     end;
     end;
     if vesainfget then
     freemem (vi,512);
     vesainfget := false;
     if modeinfget then
     freemem (mi,256);
     modeinfget := false;
end;

procedure getvesavscr;
var
error : boolean;
oc : char;
i : byte;
begin
     if not vesainfget then
     getmem (vi,512);
     fillchar (vi^,512,0);
     asm
        mov ax,4f00h
        les di,vi
        int 10h
        mov error,ah
     end;
     if error then
     begin
          freemem (vi,512);
          version := $ffff;
          totalmemoryinkilobytes := 0;
          vesainfget := false;
          exit;
     end;
     with vi^ do
     begin
          version := vesa_version;
          totalmemoryinkilobytes := total_video_banks*64;
          fillchar (oemsygnature,255,0);
          oc := chr (mem [seg (pointer_to_oem_name^):ofs (pointer_to_oem_name^)]);
          i := 1;
          while (oc <> #0) do
          begin
               oemsygnature [i] := oc;
               inc (i);
               oc := chr (mem [seg (pointer_to_oem_name^):ofs (pointer_to_oem_name^)+i-1]);
          end;
          oemsygnature [0] := chr (i-1);
     end;
     vesainfget := true;
end;

procedure getmodevscr;
var
error : boolean;
begin
     if not modeinfget then
     getmem (mi,256);
     fillchar (mi^,256,0);
     asm
        mov ax,4f01h
        mov cx,mode
        les di,mi
        int 10h
        mov error,ah
     end;
     if error then
     begin
          width := 0;
          height := 0;
     end;
     with mi^ do
     begin
          width := width_in_pixels;
          height := height_in_pixels;
          modeinfget := false;
          freemem (mi,256);
          exit;
     end;
     modeinfget := true;
end;

procedure setactivevscr;
begin
     if not us_flip then
     begin
          actual_bank := 0;
          exit;
     end;
     if no = 0 then actual_bank := 0
     else
     if no = 1 then actual_bank := page_2_bank_start;
end;

procedure setvisualvscr;
begin
     if not us_flip then exit;
     if no = 0 then
     begin
          act_x := 0;
          act_y := 0;
     end
     else
     if no = 1 then
     begin
          act_x := page_2_x_s;
          act_y := page_2_y_s;
     end;
     asm
        mov ax,4f07h
        mov bx,0
        mov cx,act_x
        mov dx,act_y
        int 10h
     end;
end;

procedure fliping;
begin
     if act_x = 0 then
     begin
          setvisualvscr (1);
          setactivevscr (0);
     end
     else
     begin
          setvisualvscr (0);
          setactivevscr (1);
     end;
end;

procedure vputpixel; assembler;
asm
   mov ax,y
   cmp ax,vscrheight
   ja @q
   mov bx,x
   cmp bx,vscrwidth
   ja @q
   push es
   les di,buf
   mov dx,vscrwidth
   mul dx
   add ax,bx
   adc dl,actual_bank
   mov di,ax
   cmp actual_drw_bank,dl
   je @noo
   mov ax,4f05h
   mov bx,0
   int 10h
   mov actual_drw_bank,dl
   @noo:
   mov al,c
   mov es:[di],al
   pop es
   @q:
end;

procedure vputpixeltranspar; assembler;
asm
   mov ax,y
   cmp ax,vscrheight
   ja @q
   mov bx,x
   cmp bx,vscrwidth
   ja @q
   push es
   les di,buf
   mov dx,vscrwidth
   mul dx
   add ax,bx
   adc dl,actual_bank
   mov di,ax
   cmp actual_drw_bank,dl
   je @noo
   mov ax,4f05h
   mov bx,0
   int 10h
   mov actual_drw_bank,dl
   @noo:
   mov di,ax
   mov al,es:[di]
   mov ah,al
   shr al,5
   shl al,5
   sub ah,al
   mov al,c
   mov bl,al
   shr al,5
   shl al,5
   sub bl,al
   add ah,bl
   shr ah,1
   add al,ah
   mov es:[di],al
   pop es
   @q:
end;

function vgetpixel;
begin
     asm
        mov ax,y
        cmp ax,vscrheight
        ja @q
        mov bx,x
        cmp bx,vscrwidth
        ja @q
        push es
        les di,buf
        mov dx,vscrwidth
        mul dx
        add ax,bx
        adc dl,actual_bank
        mov di,ax
        cmp actual_drw_bank,dl
        je @noo
        mov ax,4f05h
        mov bx,0
        int 10h
        mov actual_drw_bank,dl
        @noo:
        mov al,es:[di]
        mov @result,al
        pop es
        @q:
     end;
end;

procedure vhline; assembler;
asm
   push es
   mov ax,y1
   cmp ax,vscrheight
   ja @q
   mov bx,x1
   cmp bx,vscrwidth
   ja @q
   mov cx,x2
   cmp cx,bx
   jbe @q
   cmp cx,vscrwidth
   ja @q
   les di,buf
   mov dx,vscrwidth
   mul dx
   add ax,bx
   adc dl,actual_bank
   mov di,ax
   cmp actual_drw_bank,dl
   je @noo
   mov ax,4f05h
   mov bx,0
   int 10h
   mov actual_drw_bank,dl
   @noo:
   sub cx,x1
   mov ax,di
   add ax,cx
   cmp ax,di
   ja @secondln
   push ax
   mov cx,0ffffh
   sub cx,di
   inc cx
   mov bx,cx
   shr cx,1
   shl cx,1
   sub bx,cx
   shr cx,1
   mov al,c
   mov ah,c
   cld
   rep stosw
   cmp bx,0
   jz @opusc
   stosb
   @opusc:
   mov ax,4f05h
   inc dl
   mov bx,0
   int 10h
   xor di,di
   pop cx
   @secondln:
   mov bx,cx
   shr cx,1
   mov dx,cx
   shl cx,1
   sub bx,cx
   mov cx,dx
   mov al,c
   mov ah,c
   cld
   rep stosw
   cmp bx,0
   je @q
   mov es:[di],al
   @q:
   pop es
end;

procedure vvline; assembler;
asm
   push es
   mov ax,y1
   cmp ax,vscrheight
   ja @q
   mov cx,y2
   cmp cx,ax
   jb @q
   mov bx,x1
   cmp bx,vscrwidth
   ja @q
   les di,buf
   mul vscrwidth
   mov ax,di
   add di,bx
   adc dl,actual_bank
   sub cx,ax
   cmp dl,actual_drw_bank
   jne @call_bank2
   jmp @ncb2
   @call_bank1:
   inc dl
   @call_bank2:
   mov ax,4f05h
   push bx
   mov bx,0
   int 10h
   pop bx
   mov al,c
   jmp @1
   @ncb2:
   mov al,c
   @s:
      mov es:[di],al
      mov bx,di
      add di,vscrwidth
      cmp di,bx
      jb @call_bank1
      @1:
      loop @s
   mov actual_drw_bank,dl
   @q:
   pop es
end;

procedure vcls; assembler;
asm
   push es
   les di,buf
   mov dl,actual_bank
   xor bx,bx
   @a:
      mov ax,4f05h
      xor dh,dh
      int 10h
      mov cx,65536/4
      xor ax,ax
      rep; db 66h; stosw;
      inc dl
      mov dh,dl
      sub dh,actual_bank
      cmp dh,num_of_used_vid_banks
      jbe @a
   mov actual_drw_bank,dl
   pop es
end;

function vcalculate_tex_start_poz_x_y (
x,y : integer;
var starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex : word;
tex : ttex
) : word;
var
sizeh,sizev : byte;
endxo,endyo : integer;
i,licznik : byte;
j : word;
begin
     j := 0;
     vcalculate_tex_start_poz_x_y := 0;
     starttexx := 0;
     starttexy := 0;
     if (x >= vscrwidth) or (y >= vscrheight) then exit;
     sizeh := tex.sizex;
     sizev := tex.sizey;
     endxo := x+sizeh;
     endyo := y+sizev;
     if (endxo <= 0) or (endyo <= 0) then exit;
     if x < 0 then
     begin
          startxo := 0;
          starttexx := abs (x);
     end;
     if y < 0 then
     begin
          startyo := 0;
          starttexy := abs (y);
     end;
     if endxo > vscrwidth then endxo := vscrwidth;
     if endyo > vscrheight then endyo := vscrheight;
     if x >= 0 then startxo := x;
     if y >= 0 then startyo := y;
     shf := vscrwidth - endxo + startxo;
     shftex := sizeh - endxo + startxo;
     _endxo := endxo;
     _endyo := endyo;
     {j:
        -dolna czesc:1 bank
        -gorna czesc:0000 0000
                    / \------/
                  /     \  /
                \/       \/
     rodzaj rysow.         \-------> liczba powt.
     dalej jest:
     vcalculate_tex_start_poz_x_y := j;
     }
     j := (longint (startyo)*longint (vscrwidth)) shr 16;
     if _endyo <= endline [j] then
     begin
          j := j or 256;
          vcalculate_tex_start_poz_x_y := j;
          exit;
     end;
     if _endyo > endline [j] then
     begin
          licznik := 0;
          i := j;
          while (_endyo > endline [i]) do
          begin
               inc (licznik,2);
               inc (i);
          end;
          if startyo = endline [j] then dec (licznik);
          if _endyo = endline [i-1]+1 then dec (licznik);
          j := j or (word (licznik) shl 8);
          j := j or 32768;
          vcalculate_tex_start_poz_x_y := j;
     end;
end;

{procedury do rysow.}

procedure texnf (startb,startt,shf,shftex : word; width,height : word; source : ttex);
var
b,t : pointer;
begin
     {je˜li warto˜† funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     b := buf;
     t := source.texmemory;
         asm
            {pocz¥tkowa faza obliczeä}
            push es
            push ds
            cld {adresy rosn¥ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo˜† cykli p©tli dla y}
            mov ax,shf {przesuni©cie dla buforu}
            mov dx,shftex {przesuni©cie dla tekstury}
            @for_y: {p©tla dla y}
                    mov cx,width {ilo˜† cykli p©tli dla x}
                    rep movsb {przesyˆaj dane z ds:[si] do es:[di] dopuki cx = 0 (2 bajty dla lepszej szybko˜ci)}
                    add si,dx
                    add di,ax
                    dec bx
                    jnz @for_y
            pop ds
            pop es
         end;
end;

procedure texf (startb,startt,shf,shftex : word; width,height : word; fadevalue : byte; source : ttex);
var
b,t : pointer;
begin
     {je˜li warto˜† funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     b := buf;
     t := source.texmemory;
     fadevalue := 31-fadevalue;
         asm
            {pocz¥tkowa faza obliczeä}
            push es
            push ds
            cld {adresy rosn¥ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo˜† cykli p©tli dla y}
            mov dl,fadevalue {przesuni©cie dla tekstury}
            @for_y: {p©tla dla y}
                    mov cx,width {ilo˜† cykli p©tli dla x}
                    @for_x: {petla dla x}
                           mov al,ds:[si] {pobranie pixela do al}
                           mov ah,al {al do ah}
                           mov dh,al {al(ah) do dh}
                           shr ah,5 {obliczenie pierwszego odcienia (kol.czarny) koloru}
                           shl ah,5
                           sub dh,ah {dh - ah = kolor pixela - pierwszy kolor odcienia pixela}
                           cmp dh,dl {sprawdzenie czy instrukcja sub al,dl > ah}
                           jbe @null {jesli tak do et. null (zero)}
                           sub al,dl {jesli nie to oblicz nowy odcien (co daje efekt przyciemniania obrazka)}
                           stosb {wklejenie pixela do es:[di]}
                           inc si
                           dec cx
                           jnz @for_x
                           jz @quit
                           @null: {wklejamy pierwszy odcien koloru {kolor czarny}
                                 mov es:[di],ah
                                 inc si
                                 inc di
                                 dec cx
                                 jnz @for_x
                           @quit:
                    add si,shftex
                    add di,shf
                    dec bx
                    jnz @for_y
            pop ds
            pop es
         end;
end;

procedure sprtexnf (startb,startt,shf,shftex : word; width,height : word; source : ttex);
var
b,t : pointer;
begin
     {je˜li warto˜† funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     b := buf;
     t := source.texmemory;
         asm
            {pocz¥tkowa faza obliczeä}
            push es
            push ds
            cld {adresy rosn¥ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo˜† cykli p©tli dla y}
            mov dx,shftex {przesuni©cie dla tekstury}
            @for_y: {p©tla dla y}
                    mov cx,width {ilo˜† cykli p©tli dla x}
                    @for_x: {p©tla dla x}
                            mov al,ds:[si] {pixel z ds:[si] do al}
                            cmp al,0 {poruwnanie al z 0 dla ponizszej instrukcji}
                            jz @notputpixel {jesli nie zero idz do et. putpix}
                            mov es:[di],al {rysuje pixel w es:[di]}
                            @notputpixel:
                            inc si
                            inc di
                            dec cx
                            jnz @for_x
                    add si,dx
                    add di,shf
                    dec bx
                    jnz @for_y
            pop ds
            pop es
         end;
end;

procedure sprtexf (startb,startt,shf,shftex : word; width,height : word; fadevalue : byte; source : ttex);
var
b,t : pointer;
begin
     {je˜li warto˜† funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     b := buf;
     t := source.texmemory;
     fadevalue := 31-fadevalue;
         asm
            {pocz¥tkowa faza obliczeä}
            push es
            push ds
            cld {adresy rosn¥ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo˜† cykli p©tli dla y}
            mov dl,fadevalue {przesuni©cie dla tekstury}
            @for_y: {p©tla dla y}
                    mov cx,width {ilo˜† cykli p©tli dla x}
                    @for_x: {petla dla x}
                           mov al,ds:[si] {pobranie pixela do al}
                           cmp al,0
                           jz @notputpixel
                           mov ah,al {al do ah}
                           mov dh,al {al(ah) do dh}
                           shr ah,5 {obliczenie pierwszego odcienia (kol.czarny) koloru}
                           shl ah,5
                           sub dh,ah {dh - ah = kolor pixela - pierwszy kolor odcienia pixela}
                           cmp dh,dl {sprawdzenie czy instrukcja sub al,dl > ah}
                           jbe @null {jesli tak do et. null (zero)}
                           sub al,dl {jesli nie to oblicz nowy odcien (co daje efekt przyciemniania obrazka)}
                           mov es:[di],al {wklejenie pixela do es:[di]}
                           @notputpixel:
                           inc si
                           inc di
                           dec cx
                           jnz @for_x
                           jz @quit
                           @null: {wklejamy pierwszy odcien koloru {kolor czarny}
                                 mov es:[di],ah
                                 inc si
                                 inc di
                                 dec cx
                                 jnz @for_x
                           @quit:
                    add si,shftex
                    add di,shf
                    dec bx
                    jnz @for_y
            pop ds
            pop es
         end;
end;

procedure textf (startb,startt,shf,shftex : word; width,height : word; fadevalue : byte; source : ttex);
var
b,t : pointer;
begin
     {je˜li warto˜† funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     b := buf;
     t := source.texmemory;
     fadevalue := 31-fadevalue;
         asm
            {pocz¥tkowa faza obliczeä}
            push es
            push ds
            cld {adresy rosn¥ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo˜† cykli p©tli dla y}
            @for_y: {p©tla dla y}
                    mov cx,width {ilo˜† cykli p©tli dla x}
                    @for_x: {petla dla x}
                           mov al,ds:[si] {pobranie pixela do al}
                           mov ah,al {al do ah}
                           mov dh,al {al(ah) do dh}
                           shr ah,5 {obliczenie pierwszego odcienia (kol.czarny) koloru}
                           shl ah,5
                           sub dh,ah {dh - ah = kolor pixela - pierwszy kolor odcienia pixela}
                           cmp dh,fadevalue {sprawdzenie czy instrukcja sub al,dl > ah}
                           jbe @null {jesli tak do et. null (zero)}
                           sub dh,fadevalue {jesli nie to oblicz nowy odcien (co daje efekt przyciemniania obrazka)}
                           mov dl,es:[di]
                           mov al,dl
                           shr dl,5
                           shl dl,5
                           sub al,dl
                           add al,dh
                           shr al,1
                           add al,ah
                           stosb {wklejenie pixela do es:[di]}
                           inc si
                           dec cx
                           jnz @for_x
                           jz @quit
                           @null: {wklejamy pierwszy odcien koloru {kolor czarny}
                                  mov dl,es:[di]
                                  mov al,dl
                                  shr dl,5
                                  shl dl,5
                                  sub al,dl
                                  add al,0
                                  shr al,1
                                  add al,ah
                                  stosb
                                  inc si
                                  dec cx
                                  jnz @for_x
                           @quit:
                    add si,shftex
                    add di,shf
                    dec bx
                    jnz @for_y
            pop ds
            pop es
         end;
end;

procedure sprtextf (startb,startt,shf,shftex : word; width,height : word; fadevalue : byte; source : ttex);
var
b,t : pointer;
begin
     {je˜li warto˜† funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     b := buf;
     t := source.texmemory;
     fadevalue := 31-fadevalue;
         asm
            {pocz¥tkowa faza obliczeä}
            push es
            push ds
            cld {adresy rosn¥ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo˜† cykli p©tli dla y}
            @for_y: {p©tla dla y}
                    mov cx,width {ilo˜† cykli p©tli dla x}
                    @for_x: {petla dla x}
                           mov al,ds:[si] {pobranie pixela do al}
                           cmp al,0
                           jz @notputpixel
                           mov ah,al {al do ah}
                           mov dh,al {al(ah) do dh}
                           shr ah,5 {obliczenie pierwszego odcienia (kol.czarny) koloru}
                           shl ah,5
                           sub dh,ah {dh - ah = kolor pixela - pierwszy kolor odcienia pixela}
                           cmp dh,fadevalue {sprawdzenie czy instrukcja sub al,dl > ah}
                           jbe @null {jesli tak do et. null (zero)}
                           sub dh,fadevalue {jesli nie to oblicz nowy odcien (co daje efekt przyciemniania obrazka)}
                           mov dl,es:[di]
                           mov al,dl
                           shr dl,5
                           shl dl,5
                           sub al,dl
                           add al,dh
                           shr al,1
                           add al,ah
                           mov es:[di],al {wklejenie pixela do es:[di]}
                           @notputpixel:
                           inc si
                           inc di
                           dec cx
                           jnz @for_x
                           jz @quit
                           @null: {wklejamy pierwszy odcien koloru {kolor czarny}
                                  mov dl,es:[di]
                                  mov al,dl
                                  shr dl,5
                                  shl dl,5
                                  sub al,dl
                                  add al,0
                                  shr al,1
                                  add al,ah
                                  mov es:[di],al
                                  inc si
                                  inc di
                                  dec cx
                                  jnz @for_x
                           @quit:
                    add si,shftex
                    add di,shf
                    dec bx
                    jnz @for_y
            pop ds
            pop es
         end;
end;

(*procedure transpwin (startb,startt,shf,shftex : word; width,height,c : byte; source : ttex);
var
t : ttex;
b : pointer;
begin
     t.sizex := x2-x1;
     t.sizey := y2-y1;
     b := buf;
         asm
            {pocz¥tkowa faza obliczeä}
            push es
            push ds
            cld {adresy rosn¥ce}
            les di,b {ladowanie wsk. buforu}
            add di,startb {start buforu}
            mov bx,height {ilo˜† cykli p©tli dla y}
            mov dh,c
            mov ah,dh
            shr dh,5
            shl dh,5
            sub ah,dh
            @for_y: {p©tla dla y}
                    mov cx,width {ilo˜† cykli p©tli dla x}
                    @for_x: {petla dla x}
                           mov dl,es:[di]
                           mov al,dl
                           shr dl,5
                           shl dl,5
                           sub al,dl
                           add al,ah
                           shr al,1
                           add al,dh
                           stosb {wklejenie pixela do es:[di]}
                           dec cx
                           jnz @for_x
                    add di,shf
                    dec bx
                    jnz @for_y
            pop ds
            pop es
         end;
end;*)

{wlasciwe proc.}
procedure vscrtexnf;
var
starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,startb,starttex,wh,ht,mode : word;
endb,endxo2,endyo2,startyo2,liczbpowtorz : word;
bank,i,j : byte;
begin
     mode := vcalculate_tex_start_poz_x_y (x,y,starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,source);
     if mode = 0 then exit;
     if hi (mode) = 1 then
     begin
          wh := _endxo-startxo;
          ht := _endyo-startyo;
          bank := lo (mode);
          startb := startyo*vscrwidth+startxo;
          starttex := starttexy*source.sizex+starttexx;
          asm
             mov dl,bank
             add dl,actual_bank
             cmp dl,actual_drw_bank
             je @q
             mov ax,4f05h
             mov bx,0
             xor dh,dh
             int 10h
             mov actual_drw_bank,dl
             @q:
          end;
          texnf (startb,starttex,shf,shftex,wh,ht,source)
     end
     else
     if (mode and 32768) = 32768 then
     begin
          liczbpowtorz := hi (mode) and 127;
          bank := lo (mode);
          endyo2 := _endyo;
          startyo2 := startyo;
          if startyo = endline [bank] then j := 0
          else
          begin
               j := 1;
               _endyo := endline [bank];
          end;
          for i := 0 to liczbpowtorz do
          begin
               startb := startyo*vscrwidth+startxo;
               starttex := starttexy*source.sizex+starttexx;
               if j = 0 then
               begin
                    endb := startb-startxo+_endxo;
                    if endb < startb then
                    begin
                         endxo2 := endpixinendline [bank];
                         wh := endxo2-startxo;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                         end;
                         texnf (startb,starttex,shf,shftex,wh,1,source);
                         startb := 0;
                         starttex := starttex+wh;
                         wh := _endxo-endxo2;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            inc dl
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                            mov actual_drw_bank,dl
                         end;
                         texnf (startb,starttex,shf,shftex,wh,1,source);
                    end
                    else
                    begin
                         wh := _endxo-startxo;
                         if startb < vscrwidth then
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 inc dl
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              texnf (startb,starttex,shf,shftex,wh,1,source);
                         end
                         else
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              texnf (startb,starttex,shf,shftex,wh,1,source);
                         end;
                    end;
                    startyo := endline[bank]+1;
                    inc (bank);
                    _endyo := endline[bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
                    if _endyo > endyo2 then _endyo := endyo2;
                    if _endyo = startyo then inc (_endyo);
               end
               else
               begin
                    wh := _endxo-startxo;
                    ht := _endyo-startyo;
                    asm
                       mov dl,bank
                       add dl,actual_bank
                       mov ax,4f05h
                       mov bx,0
                       xor dh,dh
                       int 10h
                       mov actual_drw_bank,dl
                    end;
                    texnf (startb,starttex,shf,shftex,wh,ht,source);
                    startyo := endline [bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
               end;
               j := 1-j;
          end;
     end;
end;

procedure vscrtexf;
var
starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,startb,starttex,wh,ht,mode : word;
endb,endxo2,endyo2,startyo2,liczbpowtorz : word;
bank,i,j : byte;
begin
     mode := vcalculate_tex_start_poz_x_y (x,y,starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,source);
     if mode = 0 then exit;
     if hi (mode) = 1 then
     begin
          wh := _endxo-startxo;
          ht := _endyo-startyo;
          bank := lo (mode);
          startb := startyo*vscrwidth+startxo;
          starttex := starttexy*source.sizex+starttexx;
          asm
             mov dl,bank
             add dl,actual_bank
             cmp dl,actual_drw_bank
             je @q
             mov ax,4f05h
             mov bx,0
             xor dh,dh
             int 10h
             mov actual_drw_bank,dl
             @q:
          end;
          texf (startb,starttex,shf,shftex,wh,ht,fadevalue,source)
     end
     else
     if (mode and 32768) = 32768 then
     begin
          liczbpowtorz := hi (mode) and 127;
          bank := lo (mode);
          endyo2 := _endyo;
          startyo2 := startyo;
          if startyo = endline [bank] then j := 0
          else
          begin
               j := 1;
               _endyo := endline [bank];
          end;
          for i := 0 to liczbpowtorz do
          begin
               startb := startyo*vscrwidth+startxo;
               starttex := starttexy*source.sizex+starttexx;
               if j = 0 then
               begin
                    endb := startb-startxo+_endxo;
                    if endb < startb then
                    begin
                         endxo2 := endpixinendline [bank];
                         wh := endxo2-startxo;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                         end;
                         texf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                         startb := 0;
                         starttex := starttex+wh;
                         wh := _endxo-endxo2;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            inc dl
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                            mov actual_drw_bank,dl
                         end;
                         texf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                    end
                    else
                    begin
                         wh := _endxo-startxo;
                         if startb < vscrwidth then
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 inc dl
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              texf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                         end
                         else
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              texf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                         end;
                    end;
                    startyo := endline[bank]+1;
                    inc (bank);
                    _endyo := endline[bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
                    if _endyo > endyo2 then _endyo := endyo2;
                    if _endyo = startyo then inc (_endyo);
               end
               else
               begin
                    wh := _endxo-startxo;
                    ht := _endyo-startyo;
                    asm
                       mov dl,bank
                       add dl,actual_bank
                       mov ax,4f05h
                       mov bx,0
                       xor dh,dh
                       int 10h
                       mov actual_drw_bank,dl
                    end;
                    texf (startb,starttex,shf,shftex,wh,ht,fadevalue,source);
                    startyo := endline [bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
               end;
               j := 1-j;
          end;
     end;
end;

procedure vscrsprtexnf;
var
starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,startb,starttex,wh,ht,mode : word;
endb,endxo2,endyo2,startyo2,liczbpowtorz : word;
bank,i,j : byte;
begin
     mode := vcalculate_tex_start_poz_x_y (x,y,starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,source);
     if mode = 0 then exit;
     if hi (mode) = 1 then
     begin
          wh := _endxo-startxo;
          ht := _endyo-startyo;
          bank := lo (mode);
          startb := startyo*vscrwidth+startxo;
          starttex := starttexy*source.sizex+starttexx;
          asm
             mov dl,bank
             add dl,actual_bank
             cmp dl,actual_drw_bank
             je @q
             mov ax,4f05h
             mov bx,0
             xor dh,dh
             int 10h
             mov actual_drw_bank,dl
             @q:
          end;
          sprtexnf (startb,starttex,shf,shftex,wh,ht,source)
     end
     else
     if (mode and 32768) = 32768 then
     begin
          liczbpowtorz := hi (mode) and 127;
          bank := lo (mode);
          endyo2 := _endyo;
          startyo2 := startyo;
          if startyo = endline [bank] then j := 0
          else
          begin
               j := 1;
               _endyo := endline [bank];
          end;
          for i := 0 to liczbpowtorz do
          begin
               startb := startyo*vscrwidth+startxo;
               starttex := starttexy*source.sizex+starttexx;
               if j = 0 then
               begin
                    endb := startb-startxo+_endxo;
                    if endb < startb then
                    begin
                         endxo2 := endpixinendline [bank];
                         wh := endxo2-startxo;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                         end;
                         sprtexnf (startb,starttex,shf,shftex,wh,1,source);
                         startb := 0;
                         starttex := starttex+wh;
                         wh := _endxo-endxo2;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            inc dl
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                            mov actual_drw_bank,dl
                         end;
                         sprtexnf (startb,starttex,shf,shftex,wh,1,source);
                    end
                    else
                    begin
                         wh := _endxo-startxo;
                         if startb < vscrwidth then
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 inc dl
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              sprtexnf (startb,starttex,shf,shftex,wh,1,source);
                         end
                         else
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              sprtexnf (startb,starttex,shf,shftex,wh,1,source);
                         end;
                    end;
                    startyo := endline[bank]+1;
                    inc (bank);
                    _endyo := endline[bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
                    if _endyo > endyo2 then _endyo := endyo2;
                    if _endyo = startyo then inc (_endyo);
               end
               else
               begin
                    wh := _endxo-startxo;
                    ht := _endyo-startyo;
                    asm
                       mov dl,bank
                       add dl,actual_bank
                       mov ax,4f05h
                       mov bx,0
                       xor dh,dh
                       int 10h
                       mov actual_drw_bank,dl
                    end;
                    sprtexnf (startb,starttex,shf,shftex,wh,ht,source);
                    startyo := endline [bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
               end;
               j := 1-j;
          end;
     end;
end;

procedure vscrsprtexf;
var
starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,startb,starttex,wh,ht,mode : word;
endb,endxo2,endyo2,startyo2,liczbpowtorz : word;
bank,i,j : byte;
begin
     mode := vcalculate_tex_start_poz_x_y (x,y,starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,source);
     if mode = 0 then exit;
     if hi (mode) = 1 then
     begin
          wh := _endxo-startxo;
          ht := _endyo-startyo;
          bank := lo (mode);
          startb := startyo*vscrwidth+startxo;
          starttex := starttexy*source.sizex+starttexx;
          asm
             mov dl,bank
             add dl,actual_bank
             cmp dl,actual_drw_bank
             je @q
             mov ax,4f05h
             mov bx,0
             xor dh,dh
             int 10h
             mov actual_drw_bank,dl
             @q:
          end;
          sprtexf (startb,starttex,shf,shftex,wh,ht,fadevalue,source)
     end
     else
     if (mode and 32768) = 32768 then
     begin
          liczbpowtorz := hi (mode) and 127;
          bank := lo (mode);
          endyo2 := _endyo;
          startyo2 := startyo;
          if startyo = endline [bank] then j := 0
          else
          begin
               j := 1;
               _endyo := endline [bank];
          end;
          for i := 0 to liczbpowtorz do
          begin
               startb := startyo*vscrwidth+startxo;
               starttex := starttexy*source.sizex+starttexx;
               if j = 0 then
               begin
                    endb := startb-startxo+_endxo;
                    if endb < startb then
                    begin
                         endxo2 := endpixinendline [bank];
                         wh := endxo2-startxo;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                         end;
                         sprtexf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                         startb := 0;
                         starttex := starttex+wh;
                         wh := _endxo-endxo2;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            inc dl
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                            mov actual_drw_bank,dl
                         end;
                         sprtexf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                    end
                    else
                    begin
                         wh := _endxo-startxo;
                         if startb < vscrwidth then
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 inc dl
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              sprtexf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                         end
                         else
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              sprtexf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                         end;
                    end;
                    startyo := endline[bank]+1;
                    inc (bank);
                    _endyo := endline[bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
                    if _endyo > endyo2 then _endyo := endyo2;
                    if _endyo = startyo then inc (_endyo);
               end
               else
               begin
                    wh := _endxo-startxo;
                    ht := _endyo-startyo;
                    asm
                       mov dl,bank
                       add dl,actual_bank
                       mov ax,4f05h
                       mov bx,0
                       xor dh,dh
                       int 10h
                       mov actual_drw_bank,dl
                    end;
                    sprtexf (startb,starttex,shf,shftex,wh,ht,fadevalue,source);
                    startyo := endline [bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
               end;
               j := 1-j;
          end;
     end;
end;

procedure vscrtextf;
var
starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,startb,starttex,wh,ht,mode : word;
endb,endxo2,endyo2,startyo2,liczbpowtorz : word;
bank,i,j : byte;
begin
     mode := vcalculate_tex_start_poz_x_y (x,y,starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,source);
     if mode = 0 then exit;
     if hi (mode) = 1 then
     begin
          wh := _endxo-startxo;
          ht := _endyo-startyo;
          bank := lo (mode);
          startb := startyo*vscrwidth+startxo;
          starttex := starttexy*source.sizex+starttexx;
          asm
             mov dl,bank
             add dl,actual_bank
             cmp dl,actual_drw_bank
             je @q
             mov ax,4f05h
             mov bx,0
             xor dh,dh
             int 10h
             mov actual_drw_bank,dl
             @q:
          end;
          textf (startb,starttex,shf,shftex,wh,ht,fadevalue,source)
     end
     else
     if (mode and 32768) = 32768 then
     begin
          liczbpowtorz := hi (mode) and 127;
          bank := lo (mode);
          endyo2 := _endyo;
          startyo2 := startyo;
          if startyo = endline [bank] then j := 0
          else
          begin
               j := 1;
               _endyo := endline [bank];
          end;
          for i := 0 to liczbpowtorz do
          begin
               startb := startyo*vscrwidth+startxo;
               starttex := starttexy*source.sizex+starttexx;
               if j = 0 then
               begin
                    endb := startb-startxo+_endxo;
                    if endb < startb then
                    begin
                         endxo2 := endpixinendline [bank];
                         wh := endxo2-startxo;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                         end;
                         textf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                         startb := 0;
                         starttex := starttex+wh;
                         wh := _endxo-endxo2;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            inc dl
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                            mov actual_drw_bank,dl
                         end;
                         textf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                    end
                    else
                    begin
                         wh := _endxo-startxo;
                         if startb < vscrwidth then
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 inc dl
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              textf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                         end
                         else
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              textf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                         end;
                    end;
                    startyo := endline[bank]+1;
                    inc (bank);
                    _endyo := endline[bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
                    if _endyo > endyo2 then _endyo := endyo2;
                    if _endyo = startyo then inc (_endyo);
               end
               else
               begin
                    wh := _endxo-startxo;
                    ht := _endyo-startyo;
                    asm
                       mov dl,bank
                       add dl,actual_bank
                       mov ax,4f05h
                       mov bx,0
                       xor dh,dh
                       int 10h
                       mov actual_drw_bank,dl
                    end;
                    textf (startb,starttex,shf,shftex,wh,ht,fadevalue,source);
                    startyo := endline [bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
               end;
               j := 1-j;
          end;
     end;
end;

procedure vscrsprtextf;
var
starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,startb,starttex,wh,ht,mode : word;
endb,endxo2,endyo2,startyo2,liczbpowtorz : word;
bank,i,j : byte;
begin
     mode := vcalculate_tex_start_poz_x_y (x,y,starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,source);
     if mode = 0 then exit;
     if hi (mode) = 1 then
     begin
          wh := _endxo-startxo;
          ht := _endyo-startyo;
          bank := lo (mode);
          startb := startyo*vscrwidth+startxo;
          starttex := starttexy*source.sizex+starttexx;
          asm
             mov dl,bank
             add dl,actual_bank
             cmp dl,actual_drw_bank
             je @q
             mov ax,4f05h
             mov bx,0
             xor dh,dh
             int 10h
             mov actual_drw_bank,dl
             @q:
          end;
          sprtextf (startb,starttex,shf,shftex,wh,ht,fadevalue,source)
     end
     else
     if (mode and 32768) = 32768 then
     begin
          liczbpowtorz := hi (mode) and 127;
          bank := lo (mode);
          endyo2 := _endyo;
          startyo2 := startyo;
          if startyo = endline [bank] then j := 0
          else
          begin
               j := 1;
               _endyo := endline [bank];
          end;
          for i := 0 to liczbpowtorz do
          begin
               startb := startyo*vscrwidth+startxo;
               starttex := starttexy*source.sizex+starttexx;
               if j = 0 then
               begin
                    endb := startb-startxo+_endxo;
                    if endb < startb then
                    begin
                         endxo2 := endpixinendline [bank];
                         wh := endxo2-startxo;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                         end;
                         sprtextf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                         startb := 0;
                         starttex := starttex+wh;
                         wh := _endxo-endxo2;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            inc dl
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                            mov actual_drw_bank,dl
                         end;
                         sprtextf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                    end
                    else
                    begin
                         wh := _endxo-startxo;
                         if startb < vscrwidth then
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 inc dl
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              sprtextf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                         end
                         else
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              sprtextf (startb,starttex,shf,shftex,wh,1,fadevalue,source);
                         end;
                    end;
                    startyo := endline[bank]+1;
                    inc (bank);
                    _endyo := endline[bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
                    if _endyo > endyo2 then _endyo := endyo2;
                    if _endyo = startyo then inc (_endyo);
               end
               else
               begin
                    wh := _endxo-startxo;
                    ht := _endyo-startyo;
                    asm
                       mov dl,bank
                       add dl,actual_bank
                       mov ax,4f05h
                       mov bx,0
                       xor dh,dh
                       int 10h
                       mov actual_drw_bank,dl
                    end;
                    sprtextf (startb,starttex,shf,shftex,wh,ht,fadevalue,source);
                    startyo := endline [bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
               end;
               j := 1-j;
          end;
     end;
end;

{procedure vscrtranspwin;
var
starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,startb,starttex,wh,ht,mode : word;
endb,endxo2,endyo2,startyo2,liczbpowtorz : word;
bank,i,j : byte;
begin
     mode := vcalculate_tex_start_poz_x_y (x,y,starttexx,starttexy,startxo,startyo,_endxo,_endyo,shf,shftex,source);
     if mode = 0 then exit;
     if hi (mode) = 1 then
     begin
          wh := _endxo-startxo;
          ht := _endyo-startyo;
          bank := lo (mode);
          startb := startyo*vscrwidth+startxo;
          starttex := starttexy*source.sizex+starttexx;
          asm
             mov dl,bank
             add dl,actual_bank
             cmp dl,actual_drw_bank
             je @q
             mov ax,4f05h
             mov bx,0
             xor dh,dh
             int 10h
             mov actual_drw_bank,dl
             @q:
          end;
          transpwin (startb,starttex,shf,shftex,wh,ht,source)
     end
     else
     if (mode and 32768) = 32768 then
     begin
          liczbpowtorz := hi (mode) and 127;
          bank := lo (mode);
          endyo2 := _endyo;
          startyo2 := startyo;
          if startyo = endline [bank] then j := 0
          else
          begin
               j := 1;
               _endyo := endline [bank];
          end;
          for i := 0 to liczbpowtorz do
          begin
               startb := startyo*vscrwidth+startxo;
               starttex := starttexy*source.sizex+starttexx;
               if j = 0 then
               begin
                    endb := startb-startxo+_endxo;
                    if endb < startb then
                    begin
                         endxo2 := endpixinendline [bank];
                         wh := endxo2-startxo;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                         end;
                         transpwin (startb,starttex,shf,shftex,wh,1,source);
                         startb := 0;
                         starttex := starttex+wh;
                         wh := _endxo-endxo2;
                         if wh > 255 then wh := 0;
                         asm
                            mov dl,bank
                            add dl,actual_bank
                            inc dl
                            mov ax,4f05h
                            mov bx,0
                            xor dh,dh
                            int 10h
                            mov actual_drw_bank,dl
                         end;
                         transpwin (startb,starttex,shf,shftex,wh,1,source);
                    end
                    else
                    begin
                         wh := _endxo-startxo;
                         if startb < vscrwidth then
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 inc dl
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              transpwin (startb,starttex,shf,shftex,wh,1,source);
                         end
                         else
                         begin
                              asm
                                 mov dl,bank
                                 add dl,actual_bank
                                 mov ax,4f05h
                                 mov bx,0
                                 xor dh,dh
                                 int 10h
                                 mov actual_drw_bank,dl
                              end;
                              transpwin (startb,starttex,shf,shftex,wh,1,source);
                         end;
                    end;
                    startyo := endline[bank]+1;
                    inc (bank);
                    _endyo := endline[bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
                    if _endyo > endyo2 then _endyo := endyo2;
                    if _endyo = startyo then inc (_endyo);
               end
               else
               begin
                    wh := _endxo-startxo;
                    ht := _endyo-startyo;
                    asm
                       mov dl,bank
                       add dl,actual_bank
                       mov ax,4f05h
                       mov bx,0
                       xor dh,dh
                       int 10h
                       mov actual_drw_bank,dl
                    end;
                    transpwin (startb,starttex,shf,shftex,wh,ht,source);
                    startyo := endline [bank];
                    if startyo2 > 0 then
                    starttexy := startyo - startyo2
                    else
                    starttexy := startyo + abs (y);
               end;
               j := 1-j;
          end;
     end;
end;
}

end.