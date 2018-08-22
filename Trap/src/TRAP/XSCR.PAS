{
  TRAP (r) Copyright (c) Kamil Korolkiewicz 2004
  Darmowa biblioteka graficzna vesa stworzona na potzeby gry TRAP (r)
  Wszelkie prawa autorskie zastrze¾one
}

{$S-,G+}
unit xscr;

interface

{doˆ¥czone moduˆy}

uses scr;

{deklaracje typ¢w}

type

t1mode = record
               width,height,bmode : word;
end;

tvesacaps = record
                 mem : word;
                 name : pchar;
                 ver : word;
                 iactmode : byte;
                 end_line : array [0..30] of word;
                 end_pixel_in_end_line : array [0..30] of word;
                 used_vid_banks : word;
                 p2_bank_start : word;
                 p2_bank_start_x : word;
                 p2_bank_start_y : word;
                 scrwidth,scrheight : word;
                 actual_bank : byte;
                 modes : array [0..8] of t1mode;
end;

tvesarec = record
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

tmoderec = record
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

{deklaracje staˆych i zmiennych}

const

vesarec : ^tvesarec = nil;
moderec : ^tmoderec = nil;

{deklaracje zmiennych}

var

vesacaps : tvesacaps;

function setvesamode (width,height : word) : boolean;
function getvesainfo : boolean;
function getmodeinfo (m : word) : boolean;
procedure closevesamode;
procedure xbuftoscr (x1,y1,x2,y2 : integer);
procedure setactivepage (p : byte);
procedure setvisualpage (p : byte);
procedure flipscr;
procedure clearvesascr (c : byte);
procedure mmxscrsprtexnf (x,y : integer; source : ttex);
procedure mmxscrtexf (x,y : integer; fadevalue : byte; source : ttex);
procedure mmxscrtextf (x,y : integer; fadevalue : byte; source : ttex);

implementation

function setvesamode (width,height : word) : boolean;
var
mode : word;
i : byte;
error : byte;
begin
     setvesamode := false;
     if getvesainfo = false then exit;
     mode := 0;
     for i := 0 to 8 do
     begin
          if (vesacaps.modes[i].width = width) and (vesacaps.modes[i].height = height) then
          mode := vesacaps.modes[i].bmode;
     end;
     if mode = 0 then exit;
     getmodeinfo (mode);
     vesacaps.used_vid_banks := longint (width)*longint (height) div 65535;
     for i := 0 to vesacaps.used_vid_banks do vesacaps.end_line[i] := (longint (i+1)*65536) div width;
     for i := 0 to vesacaps.used_vid_banks do vesacaps.end_pixel_in_end_line[i] := (longint (i+1)*65536) mod width;
     vesacaps.p2_bank_start := vesacaps.used_vid_banks+1;
     vesacaps.p2_bank_start_x := (vesacaps.used_vid_banks+1)*((65535-(65535 div width)*width)+1);;
     vesacaps.p2_bank_start_y := (vesacaps.used_vid_banks+1)*(65535 div width);
     asm
        mov ax,4f02h
        mov bx,mode
        int 10h
        mov error,ah
     end;
     if error <> 0 then exit;
     vesacaps.scrwidth := width;
     vesacaps.scrheight := height;
     setvesamode := true;
     setactivepage (1);
     setvisualpage (0);
end;

function getvesainfo : boolean;
type
smodesinfo = array [0..1000] of word;
tmodesinfo = ^smodesinfo;
var
error : byte;
pptr : tmodesinfo;
p : word;
i,j : byte;
begin
     getvesainfo := false;
     if vesarec = nil then getmem (vesarec,512);
     asm
        mov ax,4f00h
        les di,vesarec
        int 10h
        mov error,ah
     end;
     if error <> 0 then
     begin
          freemem (vesarec,512);
          vesarec := nil;
          exit;
     end;
     vesacaps.mem := vesarec^.total_video_banks;
     vesacaps.name := vesarec^.pointer_to_oem_name;
     vesacaps.ver := vesarec^.vesa_version;
     pptr := vesarec^.list_of_all_modes;
     p := pptr^[0];
     i := 0;
     j := 0;
     while p <> $ffff do
     begin
          getmodeinfo (p);
          if moderec^.num_of_bpp = 8 then
          begin
               if (longint (moderec^.width_in_pixels)*longint (moderec^.height_in_pixels)*2+65536) <
               (longint (vesacaps.mem)*65536) then
               begin
                    vesacaps.modes[j].width  :=   moderec^.width_in_pixels;
                    vesacaps.modes[j].height :=   moderec^.height_in_pixels;
                    vesacaps.modes[j].bmode  :=   p;
                    inc (j);
                    if j > 8 then break;
               end;
          end;
          inc (i);
          p := pptr^[i];
     end;
     getvesainfo := true;
end;

function getmodeinfo (m : word) : boolean;
var
error : byte;
begin
     getmodeinfo := false;
     if moderec = nil then getmem (moderec,256);
     asm
        mov ax,4f01h
        mov cx,m
        les di,moderec
        int 10h
        mov error,ah
     end;
     if error <> 0 then
     begin
          freemem (moderec,256);
          moderec := nil;
          exit;
     end;
     getmodeinfo := true;
end;

procedure closevesamode;
begin
     if moderec <> nil then freemem (moderec,256);
     if vesarec <> nil then freemem (vesarec,512);
     moderec := nil;
     vesarec := nil;
     fillchar (vesacaps,sizeof (vesacaps),0);
end;

procedure xbuftoscr;
var
px,py,sx,sy,kx : integer;
ad,adlbf : word;
bn,bn2 : word;
y,i,l,lbf : word;
buf2,buf3 : pointer;
begin
     buf2 := buf;
     buf3 := ptr ($a000,0);
     if x1 > x2 then asm mov ax,x2; xchg ax,x1; mov x2,ax end;
     if y1 > y2 then asm mov ax,y2; xchg ax,y1; mov y2,ax end;
     if x1 > vesacaps.scrwidth then exit;
     if y1 > vesacaps.scrheight then exit;
     if x2 < 0 then exit;
     if y2 < 0 then exit;
     if x2 > vesacaps.scrwidth then x2 := vesacaps.scrwidth-1;
     if y2 > vesacaps.scrheight then y2 := vesacaps.scrheight-1;
     sx := x2-x1+1;
     if sx > 320 then exit;
     sy := y2-y1;
     if sy >= 200 then exit;
     lbf := 0;
     if x1 < 0 then begin px := 0; lbf := -x1; sx := x2-px+1; end else px := x1;
     if y1 < 0 then begin py := 0; if x1 < 0 then lbf := -x1+(-y1)*320 else lbf := (-y1)*320; sy := y2-py
     end else py := y1;
     ad := vesacaps.scrwidth-sx;
     adlbf := 320-sx;
     y := py;
     l := py*vesacaps.scrwidth+px;
     bn2 := (longint (py)*vesacaps.scrwidth+longint (px)) div 65535;
     bn := bn2+vesacaps.actual_bank;
     kx := px+sx;
     asm
        mov ax,4f05h
        mov bx,0
        mov dx,bn
        int 10h
     end;
     for i := 0 to sy do
     begin
          if y <> vesacaps.end_line[bn2] then
          begin
               asm
                  push ds
                  les di,buf3
                  lds si,buf2
                  add di,l
                  add si,lbf
                  mov cx,sx
                  mov ax,cx
                  shr ax,2
                  and cx,3
                  rep movsb
                  mov cx,ax
                  {rep movsd}   db 66h; rep movsw;
                  mov l,di
                  mov lbf,si
                  pop ds
               end;
          end
          else
          begin
               asm
                  push ds
                  mov bx,offset vesacaps.end_pixel_in_end_line
                  mov di,bn2
                  shl di,1
                  mov ax,[bx+di]
                  push ax
                  les di,buf3
                  lds si,buf2
                  add di,l
                  add si,lbf
                  cmp ax,px
                  jb @op1
                  mov cx,ax
                  sub cx,px
                  cmp ax,kx
                  jb @op2
                  mov cx,sx
                  @op2:
                  mov ax,cx
                  shr ax,2
                  and cx,3
                  rep movsb
                  mov cx,ax
                  {rep movsd}   db 66h; rep movsw;
                  @op1:
                  inc bn
                  inc bn2
                  mov ax,4f05h
                  mov bx,0
                  mov dx,bn
                  int 10h
                  mov cx,kx
                  pop ax
                  cmp cx,ax
                  jb @k
                  sub cx,ax
                  cmp ax,px
                  ja @opk
                  mov cx,sx
                  @opk:
                  mov ax,cx
                  shr ax,2
                  and cx,3
                  rep movsb
                  mov cx,ax
                  {rep movsd}   db 66h; rep movsw;
                  @k:
                  mov l,di
                  mov lbf,si
                  pop ds
               end;
          end;
          inc (l,ad);
          inc (lbf,adlbf);
          inc (y);
     end;
end;

procedure setactivepage;
begin
     if p = 0 then vesacaps.actual_bank := 0
     else
     if p = 1 then vesacaps.actual_bank := vesacaps.p2_bank_start
end;

procedure setvisualpage;
var
a,b : word;
begin
     if p = 0 then
     begin
          a := 0;
          b := 0;
     end
     else
     if p = 1 then
     begin
          a := vesacaps.p2_bank_start_x;
          b := vesacaps.p2_bank_start_y;
     end;
     asm
        mov ax,4f07h
        mov bx,0
        mov cx,a
        mov dx,b
        int 10h
     end;
end;

procedure flipscr;
begin
     if vesacaps.actual_bank = 0 then
     begin
          setactivepage (1);
          setvisualpage (0);
     end
     else
     begin
          setactivepage (0);
          setvisualpage (1);
     end;
end;

procedure clearvesascr;
begin
asm
   mov ax,0a000h
   mov es,ax
   mov dx,0
   mov si,vesacaps.p2_bank_start
   add si,vesacaps.used_vid_banks
   @p:
   mov ax,4f05h
   mov bx,0
   int 10h
   mov di,0
   mov cx,16384
   {xor eax,eax}        db 66h; xor ax,ax;
   mov al,c
   mov ah,c
   mov bx,ax
   {shl eax,16}         db 66h; shl ax,16
   mov ax,bx
   {rep stosd}          db 66h; rep stosw;
   inc dx
   cmp dx,si
   jbe @p
end;
end;

procedure mmxscrsprtexnf;
var
startb,startt,width,height,shf,shftex : word;
b,t : pointer;
zero : array [0..7] of byte;
one : array [0..7] of byte;
begin
     b := buf;
     t := source.texmemory;
     {je˜li warto˜† funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     if calculate_tex_start_poz_x_y (x,y,startb,startt,width,height,shf,shftex,source)
     then
         asm {pocz¥tkowa faza obliczeä}
            push ds
            {emms} db 0fh; db 77h;
            cld {adresy rosn¥ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo˜† cykli p©tli dla y}
            xor ax,ax
            mov word ptr zero[0],ax
            mov word ptr zero[2],ax
            mov word ptr zero[4],ax
            mov word ptr zero[6],ax
            dec ax
            mov word ptr one[0],ax
            mov word ptr one[2],ax
            mov word ptr one[4],ax
            mov word ptr one[6],ax
            {movq mm1,zero}                     db 0fh; db 6fh; db 4eh; db 0dch;
            {movq mm3,one}                      db 0fh; db 6fh; db 5eh; db 0d4h;
            @for_y: {p©tla dla y}
                    mov cx,width {ilo˜† cykli p©tli dla x}
                    mov dx,cx
                    shr cx,3
                    and dx,7
                    jz @for_x
                    push cx
                    mov cx,dx
                    @for_x2: {p©tla dla x}
                            mov al,ds:[si]
                            cmp al,0
                            jz @notputpixel
                            mov es:[di],al
                            @notputpixel:
                            inc si
                            inc di
                            dec cx
                            jnz @for_x2
                    pop cx
                    or cx,cx
                    jz @next
                    @for_x: {p©tla dla x}
                            {movq mm0,[si]}     db 0fh; db 6fh; db 4h;
                            {movq mm2,mm0}      db 0fh; db 6fh; db 0d0h;
                            {pcmpeqb mm0,mm1}   db 0fh; db 74h; db 0c1h;
                            {pxor mm0,mm3}      db 0fh; db 0efh; db 0c3h;
                            {psubb mm2,es:[di]} db 26h; db 0fh; db 0f8h; db 15h;
                            {pand mm0,mm2}      db 0fh; db 0dbh; db 0c2h;
                            {paddb mm0,es:[di]} db 26h; db 0fh; db 0fch; db 5h;
                            {movq es:[di],mm0}  db 26h; db 0fh; db 7fh; db 05h;
                            add si,8
                            add di,8
                            dec cx
                            jnz @for_x
                    @next:
                    add si,shftex
                    add di,shf
                    dec bx
                    jnz @for_y
            {emms} db 0fh; db 77h;
            pop ds
         end;
end;

procedure mmxscrtexf;
var
startb,startt,width,height,shf,shftex : word;
b,t : pointer;
fadet : array [0..7] of byte;
pandt : array [0..7] of byte;
begin
     if fadevalue = 31 then
     begin
          scrtexnf (x,y,source);
          exit;
     end;
     b := buf;
     t := source.texmemory;
     fadevalue := 31-fadevalue;
     {je˜li warto˜† funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     if calculate_tex_start_poz_x_y (x,y,startb,startt,width,height,shf,shftex,source)
     then
         asm
            {pocz¥tkowa faza obliczeä}
            push ds
            {emms} db 0fh; db 77h;
            cld {adresy rosn¥ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo˜† cykli p©tli dla y}
            mov dl,fadevalue {przesuni©cie dla tekstury}
            mov dh,dl
            mov word ptr fadet[0],dx
            mov word ptr fadet[2],dx
            mov word ptr fadet[4],dx
            mov word ptr fadet[6],dx
            xor dh,dh
            mov dl,255
            mov word ptr pandt[0],dx
            mov word ptr pandt[2],dx
            mov word ptr pandt[4],dx
            mov word ptr pandt[6],dx
            {movq mm3,fadet} db 0fh; db 6fh; db 5eh; db 0dch;
            @for_y: {p©tla dla y}
                    mov cx,width {ilo˜† cykli p©tli dla x}
                    mov dx,cx
                    shr cx,3
                    and dx,7
                    jz @for_x
                    push cx
                    mov cx,dx
                    mov dl,fadevalue
                    @for_x2: {petla dla x}
                           mov al,ds:[si]
                           mov ah,al
                           mov dh,al
                           shr ah,5
                           shl ah,5
                           sub dh,ah
                           cmp dh,dl
                           jbe @null
                           sub al,dl
                           stosb
                           inc si
                           dec cx
                           jnz @for_x2
                           jz @quit
                           @null:
                                 mov es:[di],ah
                                 inc si
                                 inc di
                                 dec cx
                                 jnz @for_x2
                           @quit:
                    pop cx
                    or cx,cx
                    jz @next
                    @for_x: {petla dla x}
                           {movq mm0,[si]}      db 0fh; db 6fh; db 04h;
                           {movq mm1,mm0}       db 0fh; db 6fh; db 0c8h;
                           {movq mm2,[si+1]}    db 0fh; db 6fh; db 54h; db 01h;
                           {pand mm1,pandt}     db 0fh; db 0dbh; db 4eh; db 0d4h;
                           {pand mm2,pandt}     db 0fh; db 0dbh; db 56h; db 0d4h;
                           {psrlw mm1,5}        db 0fh; db 71h; db 0d1h; db 05h;
                           {psllw mm1,5}        db 0fh; db 71h; db 0f1h; db 05h;
                           {psrlw mm2,5}        db 0fh; db 71h; db 0d2h; db 05h;
                           {psllw mm2,5}        db 0fh; db 71h; db 0f2h; db 05h;
                           {psllq mm2,8}        db 0fh; db 73h; db 0f2h; db 08h;
                           {por mm1,mm2}        db 0fh; db 0ebh; db 0cah;
                           {psubb mm0,mm1}      db 0fh; db 0f8h; db 0c1h;
                           {psubusb mm0,mm3}    db 0fh; db 0d8h; db 0c3h;
                           {paddb mm0,mm1}      db 0fh; db 0fch; db 0c1h;
                           {movq es:[di],mm0}   db 26h; db 0fh; db 7fh; db 05h;
                           add si,8
                           add di,8
                           dec cx
                           jnz @for_x
                    @next:
                    add si,shftex
                    add di,shf
                    dec bx
                    jnz @for_y
            {emms} db 0fh; db 77h;
            pop ds
         end;
end;

procedure mmxscrtextf;
var
startb,startt,width,height,shf,shftex : word;
b,t : pointer;
fadet : array [0..7] of byte;
pandt : array [0..7] of byte;
begin
     b := buf;
     t := source.texmemory;
     fadevalue := 31-fadevalue;
     {je˜li warto˜† funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     if calculate_tex_start_poz_x_y (x,y,startb,startt,width,height,shf,shftex,source)
     then
         asm
            {pocz¥tkowa faza obliczeä}
            push ds
            {emms} db 0fh; db 77h;
            cld {adresy rosn¥ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo˜† cykli p©tli dla y}
            mov dl,fadevalue {przesuni©cie dla tekstury}
            mov dh,dl
            mov word ptr fadet[0],dx
            mov word ptr fadet[2],dx
            mov word ptr fadet[4],dx
            mov word ptr fadet[6],dx
            xor dh,dh
            mov dl,255
            mov word ptr pandt[0],dx
            mov word ptr pandt[2],dx
            mov word ptr pandt[4],dx
            mov word ptr pandt[6],dx
            {movq mm3,fadet} db 0fh; db 6fh; db 5eh; db 0dch;
            @for_y: {p©tla dla y}
                    mov cx,width {ilo˜† cykli p©tli dla x}
                    mov dx,cx
                    shr cx,3
                    and dx,7
                    jz @for_x
                    push cx
                    mov cx,dx
                    mov dl,fadevalue
                    @for_x2:  {petla dla x}
                           mov ah,ds:[si] {pobranie pixela do al}
                           mov dh,ah {ah do dh}
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
                           jnz @for_x2
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
                                  jnz @for_x2
                           @quit:
                    pop cx
                    or cx,cx
                    jz @next
                    @for_x: {petla dla x}
                           {movq mm0,[si]}      db 0fh; db 6fh; db 04h;
                           {movq mm1,mm0}       db 0fh; db 6fh; db 0c8h;
                           {movq mm2,[si+1]}    db 0fh; db 6fh; db 54h; db 01h;
                           {pand mm1,pandt}     db 0fh; db 0dbh; db 4eh; db 0d4h;
                           {pand mm2,pandt}     db 0fh; db 0dbh; db 56h; db 0d4h;
                           {psrlw mm1,5}        db 0fh; db 71h; db 0d1h; db 05h;
                           {psllw mm1,5}        db 0fh; db 71h; db 0f1h; db 05h;
                           {psrlw mm2,5}        db 0fh; db 71h; db 0d2h; db 05h;
                           {psllw mm2,5}        db 0fh; db 71h; db 0f2h; db 05h;
                           {psllq mm2,8}        db 0fh; db 73h; db 0f2h; db 08h;
                           {por mm1,mm2}        db 0fh; db 0ebh; db 0cah;
                           {psubb mm0,mm1}      db 0fh; db 0f8h; db 0c1h;
                           {psubusb mm0,mm3}    db 0fh; db 0d8h; db 0c3h;
                           {movq mm5,mm1}       db 0fh; db 6fh; db 0e9h;
                           {movq mm4,es:[di]}   db 26h; db 0fh; db 06fh; db 25h;
                           {movq mm1,mm4}       db 0fh; db 06fh; db 0cch;
                           {movq mm2,es:[di+1]} db 26h; db 0fh; db 6fh; db 55h; db 01h;
                           {pand mm1,pandt}     db 0fh; db 0dbh; db 4eh; db 0d4h;
                           {pand mm2,pandt}     db 0fh; db 0dbh; db 56h; db 0d4h;
                           {psrlw mm1,5}        db 0fh; db 71h; db 0d1h; db 05h;
                           {psllw mm1,5}        db 0fh; db 71h; db 0f1h; db 05h;
                           {psrlw mm2,5}        db 0fh; db 71h; db 0d2h; db 05h;
                           {psllw mm2,5}        db 0fh; db 71h; db 0f2h; db 05h;
                           {psllq mm2,8}        db 0fh; db 73h; db 0f2h; db 08h;
                           {por mm1,mm2}        db 0fh; db 0ebh; db 0cah;
                           {psubb mm4,mm1}      db 0fh; db 0f8h; db 0e1h;
                           {paddb mm4,mm0}      db 0fh; db 0fch; db 0e0h;
                           {movq mm1,mm4}       db 0fh; db 06fh; db 0cch;
                           {movq mm2,mm1}       db 0fh; db 6fh; db 0d1h;
                           {psrlq mm2,8}        db 0fh; db 73h; db 0d2h; db 08h;
                           {pand mm1,pandt}     db 0fh; db 0dbh; db 4eh; db 0d4h;
                           {pand mm2,pandt}     db 0fh; db 0dbh; db 56h; db 0d4h;
                           {psrlw mm1,1}        db 0fh; db 71h; db 0d1h; db 01h;
                           {psrlw mm2,1}        db 0fh; db 71h; db 0d2h; db 01h;
                           {psllq mm2,8}        db 0fh; db 73h; db 0f2h; db 08h;
                           {por mm1,mm2}        db 0fh; db 0ebh; db 0cah;
                           {paddb mm1,mm5}      db 0fh; db 0fch; db 0cdh;
                           {movq es:[di],mm1}   db 26h; db 0fh; db 7fh; db 0dh;
                           add si,8
                           add di,8
                           dec cx
                           jnz @for_x
                    @next:
                    add si,shftex
                    add di,shf
                    dec bx
                    jnz @for_y
            {emms} db 0fh; db 77h;
            pop ds
         end;
end;

end.