{
  TRAP (r) Copyright (c) Kamil Korolkiewicz 2003
  Darmowa biblioteka graficzna stworzona na potzeby gry TRAP (r)
  Wszelkie prawa autorskie zastrze�one
}

{$S-}
unit scr;

interface

{deklaracje typ�w}

type

ttex = record
             sizex,sizey : word;
             texmemory : pointer;
end;

{deklaracje zmiennych}

var

buf : pointer;
pal : array[0..768] of byte;

{deklaracje procedur i funkcji}

procedure initvgascr; {vga on}
procedure inittxtscr; {vga off}
function  getscr : byte; {vga lub text}
procedure putpixel (x,y : word; c : byte); {pixel}
procedure putpixeltranspar (x,y : word; c : byte); {rysuje pixa o prze�roczysto�ci}
function  getpixel (x,y : word) : byte; {bierze go}
procedure bufor (ofset : word; c : byte); {definiuje bufor w ofset}
procedure hline (x1,y1,x2 : word; c : byte); {linia pozioma}
procedure vline (x1,y1,y2 : word; c : byte); {linia pionowa}
procedure setpalcol (c,r,g,b : byte); {definiuje kolor w pablicy palety}
procedure getpalcol (c : byte; var r,g,b : byte); {bierze kolor w pablicy palety}
procedure setcol (c,r,g,b : byte); {definiuje 1 kolor}
procedure getcol (c : byte; var r,g,b : byte); {bierze 1 kolor}
procedure setpalette; {ustanawia palete z tablicy pal}
procedure getpalette; {bierze w�a�ciw� palet� do pal}
procedure setbackgroundcolor (r,g,b : byte); {ustawia kolor t�a na warto�ci r,g,b}
procedure buftoscr; {rysuje to co jest w buforze na ekranie}
procedure buftoclr; {czy�ci go}
procedure cls (c : byte); {czy�ci ekran na kolor wyra�ony w c}
procedure set_8_32_pal; {ustanawia palet� 8:32 (w tablicy pal)}
procedure scrtexnf (x,y : integer; source : ttex); {rysuje teksture w buforze nie cieniuj�c go}
procedure scrtexf (x,y : integer; fadevalue : byte; source : ttex); {rysuje teksture w buforze cieniuj�c go}
procedure scrsprtexnf (x,y : integer; source : ttex); {rysuje sprajta w buforze nie cieniuj�c go}
procedure scrsprtexf (x,y : integer; fadevalue : byte; source : ttex); {rysuje sprajta w buforze cieniuj�c go}
procedure scrtextf (x,y : integer; fadevalue : byte; source : ttex); {rysuje cieniowan� texture z przezroczysto�ci�}
procedure scrsprtextf (x,y : integer; fadevalue : byte; source : ttex); {rysuje cieniowanego sprajta z przezroczyst.}
procedure scrtranspwin (x1,y1,x2,y2 : integer; c : byte); {rysuje okno o kolorze c pod pewn� przezroczyso�ci�}
procedure scrfadewin (x1,y1,x2,y2 : integer; fadevalue : byte); {rysuje przezroczyste przyciemnione okno}
procedure scrwin (x1,y1,x2,y2 : integer; c : byte); {rysuje okno o kolorze c}
procedure wfvr; {czekaj na powr�t pionowy}

implementation {implementacje procedur i funkcji}


procedure initvgascr; assembler;
asm
   mov ax,13h
   int 10h
end;

procedure inittxtscr; assembler;
asm
   mov ax,3h
   int 10h
end;

function getscr : byte; assembler;
asm
   mov ax,0f00h
   int 10h
   xor ah,ah
end;


procedure putpixel;
begin
asm
   mov ax,y
   cmp ax,199
   ja @q
   mov bx,x
   cmp bx,319
   ja @q
   les di,buf
   mov di,ax
   shl ax,8
   shl di,6
   add di,ax
   add di,bx
   mov al,c
   mov es:[di],al
   @q:
end;
end;

procedure putpixeltranspar;
begin
asm
   mov ax,y
   cmp ax,199
   ja @q
   mov bx,x
   cmp bx,319
   ja @q
   les di,buf
   mov di,ax
   shl ax,8
   shl di,6
   add di,ax
   add di,bx
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
   @q:
end;
end;

function getpixel;
begin
asm
   mov ax,y
   mov bx,x
   les di,buf
   mov di,ax
   shl ax,8
   shl di,6
   add di,ax
   add di,bx
   mov al,es:[di]
   mov [bp-1],al
end;
end;

procedure bufor;
begin
asm
   les di,buf
   mov di,ofset
   mov al,c
   mov es:[di],al
end;
end;

procedure hline;
begin
asm
   mov ax,y1
   cmp ax,200
   jae @q
   mov bx,x1
   cmp bx,320
   ja @q
   mov cx,x2
   cmp cx,bx
   jb @q
   cmp cx,320
   ja @q
   les di,buf
   mov di,ax
   shl ax,8
   shl di,6
   add di,ax
   add di,bx
   sub cx,x1
   mov al,c
   mov ah,c
   mov bx,ax
   {shl eax,16} db 66h; shl ax,16;
   mov ax,bx
   mov bx,cx
   and cx,3
   rep stosb
   mov cx,bx
   shr cx,2
   cld
   {rep stosd}  db 66h; rep stosw;
   @q:
end;
end;

procedure vline;
begin
asm
   mov ax,y1
   cmp ax,200
   ja @q
   mov cx,y2
   cmp cx,ax
   jb @q
   mov bx,x1
   cmp bx,320
   jae @q
   les di,buf
   sub cx,ax
   mov di,ax
   shl ax,8
   shl di,6
   add di,ax
   add di,bx
   mov al,c
   @s:
      mov es:[di],al
      add di,320
      loop @s
   @q:
end;
end;

procedure setpalcol;
var
ofs : word;
begin
     ofs := c*3;
     pal[ofs] := r;
     pal[ofs+1] := g;
     pal[ofs+2] := b;
end;

procedure getpalcol;
var
ofs : word;
begin
     ofs := c*3;
     r := pal[ofs];
     g := pal[ofs+1];
     b := pal[ofs+2];
end;

procedure setcol;
begin
asm
   mov dx,3c8h
   mov al,c
   out dx,al
   inc dx
   mov al,r
   out dx,al
   mov al,g
   out dx,al
   mov al,b
   out dx,al
end;
end;

procedure getcol;
var
red,green,blue : byte;
begin
     asm
        mov dx,3c7h
        mov al,c
        out dx,al
        mov dx,3c9h
        in al,dx
        mov red,al
        in al,dx
        mov green,al
        in al,dx
        mov blue,al
     end;
     r := red;
     g := green;
     b := blue;
end;

procedure setpalette;
var
j : integer;
i,r,g,b : byte;
begin
     j := 0;
     for i := 0 to 255 do
     begin
          r := pal[j];
          inc(j);
          g := pal[j];
          inc(j);
          b := pal[j];
          inc(j);
          setcol(i,r,g,b);
     end;
end;

procedure getpalette;
var
j : integer;
i,r,g,b : byte;
begin
     j := 0;
     for i := 0 to 255 do
     begin
          getcol(i,r,g,b);
          pal[j] := r;
          inc(j);
          pal[j] := g;
          inc(j);
          pal[j] := b;
          inc(j);
     end;
end;

procedure setbackgroundcolor;
begin
     setcol (0,r,g,b);
end;

procedure buftoscr; assembler;
asm
   push ds
   lds si,buf
   mov bx,0a000h
   mov es,bx
   xor di,di
   mov cx,16000
   cld
   {rep movsd}         db 66h; rep movsw {32 bity naraz}
   pop ds
end;

procedure buftoclr; assembler;
asm
   les di,buf
   {xor eax,eax}       db 66h; xor ax,ax
   mov cx,16000
   cld
   {rep stosd}         db 66h; rep stosw {32 bity naraz}
end;

procedure cls;
begin
asm
   mov bx,0a000h
   mov es,bx
   xor di,di
   mov al,c
   mov ah,c
   mov bx,ax
   {shl eax,16}        db 66h; shl ax,16
   mov ax,bx
   mov cx,16000
   cld
   {rep stosd}         db 66h; rep stosw {32 bity naraz}
end;
end;

procedure set_8_32_pal;
var
i : byte;
begin
     for i := 0 to 31 do setpalcol (i,i*2,0,0);        {kolor czerwony}
     for i := 32 to 63 do setpalcol (i,0,i*2,0);       {kolor zielony}
     for i := 64 to 95 do setpalcol (i,0,0,i*2);       {kolor niebieski}
     for i := 96 to 127 do setpalcol (i,i*2,i*2,i*2);  {kolor szary}
     for i := 128 to 159 do setpalcol (i,i*2,i*2,0);   {kolor ���ty}
     for i := 160 to 191 do setpalcol (i,i*2,0,i*2);   {kolor fioletowy}
     for i := 192 to 223 do setpalcol (i,0,i*2,i*2);   {kolor turkusowy}
     for i := 224 to 255 do setpalcol (i,i*2,i-31,0);  {kolor br�zowy}
end;

function calculate_tex_start_poz_x_y (
x,y : integer;
var startb,startt,width,height,shf,shftex : word;
tex : ttex
) : boolean;
var
sizeh,sizev,starttexx,starttexy,startxo,startyo : word;
endxo,endyo : integer;
begin
     starttexx := 0;
     starttexy := 0;
     if (x > 319) or (y > 199) then
     begin
          calculate_tex_start_poz_x_y := false;
          exit;
     end;
     calculate_tex_start_poz_x_y := true;
     sizeh := tex.sizex;
     sizev := tex.sizey;
     endxo := x+sizeh;
     endyo := y+sizev;
     if (endxo <= 0) or (endyo <= 0) then
     begin
          calculate_tex_start_poz_x_y := false;
          exit;
     end;
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
     if endxo > 320 then endxo := 320;
     if endyo > 200 then endyo := 200;
     if x >= 0 then startxo := x;
     if y >= 0 then startyo := y;
     shf := 320 - endxo + startxo;
     shftex := sizeh - endxo + startxo;
     startb := startyo*320+startxo;
     if (starttexx > 0) or (starttexy > 0) then startt := starttexy*sizeh+starttexx
     else
     startt := 0;
     width := endxo - startxo;
     if width = 0 then
     begin
          calculate_tex_start_poz_x_y := false;
          exit;
     end;
     height := endyo - startyo;
     if height = 0 then calculate_tex_start_poz_x_y := false;
end;

procedure scrtexnf;
var
startb,startt,width,height,shf,shftex : word;
b,t : pointer;
begin
     b := buf;
     t := source.texmemory;
     {je�li warto�� funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     if calculate_tex_start_poz_x_y (x,y,startb,startt,width,height,shf,shftex,source)
     then
         asm
            {pocz�tkowa faza oblicze�}
            push ds
            cld {adresy rosn�ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo�� cykli p�tli dla y}
            mov ax,shf {przesuni�cie dla buforu}
            mov dx,shftex {przesuni�cie dla tekstury}
            @for_y: {p�tla dla y}
                    mov cx,width {ilo�� cykli p�tli dla x}
                    and cx,3
                    rep movsb
                    mov cx,width
                    shr cx,2
                    {rep movsd}          db 66h; rep movsw {przesy�aj dane z ds:[si] do es:[di] a� cx = 0 po 4 bajty}
                    add si,dx
                    add di,ax
                    dec bx
                    jnz @for_y
            pop ds
         end;
end;

procedure scrtexf;
var
startb,startt,width,height,shf,shftex : word;
b,t : pointer;
begin
     if fadevalue = 31 then
     begin
          scrtexnf (x,y,source);
          exit;
     end;
     b := buf;
     t := source.texmemory;
     fadevalue := 31-fadevalue;
     {je�li warto�� funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     if calculate_tex_start_poz_x_y (x,y,startb,startt,width,height,shf,shftex,source)
     then
         asm
            {pocz�tkowa faza oblicze�}
            push ds
            cld {adresy rosn�ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo�� cykli p�tli dla y}
            mov dl,fadevalue {przesuni�cie dla tekstury}
            @for_y: {p�tla dla y}
                    mov cx,width {ilo�� cykli p�tli dla x}
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
         end;
end;

procedure scrsprtexnf;
var
startb,startt,width,height,shf,shftex : word;
b,t : pointer;
begin
     b := buf;
     t := source.texmemory;
     {je�li warto�� funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     if calculate_tex_start_poz_x_y (x,y,startb,startt,width,height,shf,shftex,source)
     then
         asm
            {pocz�tkowa faza oblicze�}
            push ds
            cld {adresy rosn�ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo�� cykli p�tli dla y}
            mov dx,shftex {przesuni�cie dla tekstury}
            @for_y: {p�tla dla y}
                    mov cx,width {ilo�� cykli p�tli dla x}
                    @for_x: {p�tla dla x}
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
         end;
end;

procedure scrsprtexf;
var
startb,startt,width,height,shf,shftex : word;
b,t : pointer;
begin
     if fadevalue = 31 then
     begin
          scrsprtexnf (x,y,source);
          exit;
     end;
     b := buf;
     t := source.texmemory;
     fadevalue := 31-fadevalue;
     {je�li warto�� funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     if calculate_tex_start_poz_x_y (x,y,startb,startt,width,height,shf,shftex,source)
     then
         asm
            {pocz�tkowa faza oblicze�}
            push ds
            cld {adresy rosn�ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo�� cykli p�tli dla y}
            mov dl,fadevalue {przesuni�cie dla tekstury}
            @for_y: {p�tla dla y}
                    mov cx,width {ilo�� cykli p�tli dla x}
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
         end;
end;

procedure scrtextf;
var
startb,startt,width,height,shf,shftex : word;
b,t : pointer;
begin
     b := buf;
     t := source.texmemory;
     fadevalue := 31-fadevalue;
     {je�li warto�� funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     if calculate_tex_start_poz_x_y (x,y,startb,startt,width,height,shf,shftex,source)
     then
         asm
            {pocz�tkowa faza oblicze�}
            push ds
            cld {adresy rosn�ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo�� cykli p�tli dla y}
            @for_y: {p�tla dla y}
                    mov cx,width {ilo�� cykli p�tli dla x}
                    @for_x: {petla dla x}
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
         end;
end;

procedure scrsprtextf;
var
startb,startt,width,height,shf,shftex : word;
b,t : pointer;
begin
     b := buf;
     t := source.texmemory;
     fadevalue := 31-fadevalue;
     {je�li warto�� funkcji wynosi true, wtedy rysujemy teksture na ekranie}
     if calculate_tex_start_poz_x_y (x,y,startb,startt,width,height,shf,shftex,source)
     then
         asm
            {pocz�tkowa faza oblicze�}
            push ds
            cld {adresy rosn�ce}
            lds si,t {ladowanie wsk. textury}
            les di,b {ladowanie wsk. buforu}
            add si,startt {start textury}
            add di,startb {start buforu}
            mov bx,height {ilo�� cykli p�tli dla y}
            @for_y: {p�tla dla y}
                    mov cx,width {ilo�� cykli p�tli dla x}
                    @for_x: {petla dla x}
                           mov ah,ds:[si] {pobranie pixela do al}
                           cmp ah,0
                           jz @notputpixel
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
         end;
end;

procedure scrtranspwin;
var
startb,startt,width,height,shf,shftex : word;
t : ttex;
sxp,syp : word;
b : pointer;
begin
     if x1 > x2 then asm mov ax,x1; xchg ax,x2; mov x1,ax end;
     if y1 > y2 then asm mov ax,y1; xchg ax,y2; mov y1,ax end;
     sxp := x2-x1;
     syp := y2-y1;
     t.sizex := sxp;
     t.sizey := syp;
     b := buf;
     {je�li warto�� funkcji wynosi true, wtedy rysujemy okno na ekranie}
     if calculate_tex_start_poz_x_y (x1,y1,startb,startt,width,height,shf,shftex,t)
     then
         asm
            {pocz�tkowa faza oblicze�}
            cld {adresy rosn�ce}
            les di,b {ladowanie wsk. buforu}
            add di,startb {start buforu}
            mov bx,height {ilo�� cykli p�tli dla y}
            mov dh,c
            mov ah,dh
            shr dh,5
            shl dh,5
            sub ah,dh
            @for_y: {p�tla dla y}
                    mov cx,width {ilo�� cykli p�tli dla x}
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
         end;
end;

procedure scrfadewin;
var
startb,startt,width,height,shf,shftex : word;
t : ttex;
sxp,syp : word;
b : pointer;
f : byte;
begin
     f := 31-fadevalue;
     if x1 > x2 then asm mov ax,x1; xchg ax,x2; mov x1,ax end;
     if y1 > y2 then asm mov ax,y1; xchg ax,y2; mov y1,ax end;
     sxp := x2-x1;
     syp := y2-y1;
     t.sizex := sxp;
     t.sizey := syp;
     b := buf;
     {je�li warto�� funkcji wynosi true, wtedy rysujemy okno na ekranie}
     if calculate_tex_start_poz_x_y (x1,y1,startb,startt,width,height,shf,shftex,t)
     then
         asm
            {pocz�tkowa faza oblicze�}
            cld {adresy rosn�ce}
            les di,b {ladowanie wsk. buforu}
            add di,startb {start buforu}
            mov bx,height {ilo�� cykli p�tli dla y}
            mov dh,f
            @for_y: {p�tla dla y}
                    mov cx,width {ilo�� cykli p�tli dla x}
                    @for_x: {petla dla x}
                           mov dl,es:[di]
                           mov ah,dl
                           mov al,ah
                           shr dl,5
                           shl dl,5
                           sub ah,dl
                           cmp ah,dh
                           jb @noput
                           sub al,dh
                           mov es:[di],al
                           inc di
                           dec cx
                           jnz @for_x
                           jz @quit
                           @noput:
                                  sub al,ah
                                  mov es:[di],al
                                  inc di
                                  dec cx
                                  jnz @for_x
                           @quit:
                    add di,shf
                    dec bx
                    jnz @for_y
         end;
end;

procedure scrwin;
var
startb,startt,width,height,shf,shftex : word;
t : ttex;
sxp,syp : word;
b : pointer;
f : byte;
begin
     if x1 > x2 then asm mov ax,x1; xchg ax,x2; mov x1,ax end;
     if y1 > y2 then asm mov ax,y1; xchg ax,y2; mov y1,ax end;
     sxp := x2-x1;
     syp := y2-y1;
     t.sizex := sxp;
     t.sizey := syp;
     b := buf;
     {je�li warto�� funkcji wynosi true, wtedy rysujemy okno na ekranie}
     if calculate_tex_start_poz_x_y (x1,y1,startb,startt,width,height,shf,shftex,t)
     then
         asm
            {pocz�tkowa faza oblicze�}
            cld {adresy rosn�ce}
            les di,b {ladowanie wsk. buforu}
            add di,startb {start buforu}
            mov bx,height {ilo�� cykli p�tli dla y}
            mov al,c
            mov ah,c
            mov cx,ax
            {shl eax,16}        db 66h; shl ax,16
            mov ax,cx
            mov dx,width
            @for_y: {p�tla dla y}
                    mov cx,dx {ilo�� cykli p�tli dla x}
                    and cx,3
                    rep stosb
                    mov cx,dx
                    shr cx,2
                    {rep stosd} db 66h; rep stosw
                    add di,shf
                    dec bx
                    jnz @for_y
         end;
end;

procedure wfvr; assembler;
asm
   mov dx,3dah
   @tp:
       in al,dx
       test al,00001000b
       jnz @tp
   @np:
       in al,dx
       test al,00001000b
       jz @np
end;

end.