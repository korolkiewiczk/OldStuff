unit gadd;

interface

uses scr,text;

const

tab1 : array [0..9] of char = ('Q','W','E','R','T','Y','U','I','O','P');
tab2 : array [0..8] of char = ('A','S','D','F','G','H','J','K','L');
tab3 : array [0..6] of char = ('Z','X','C','V','B','N','M');
ltab1 : array [0..1] of byte = (16,25);
ltab2 : array [0..1] of byte = (30,38);
ltab3 : array [0..1] of byte = (44,50);
txttblt : array [1..1400] of byte = (
0,192,0,3,12,48,192,0,
3,0,48,192,0,0,51,204,
48,3,0,0,0,0,0,0,
0,204,48,243,255,255,204,48,
243,255,255,204,48,3,131,127,
255,13,240,143,127,128,253,247,
15,12,0,24,102,12,24,48,
96,192,128,97,131,1,0,0,
15,102,152,193,3,103,254,140,
115,140,111,0,192,0,3,6,
0,0,0,0,0,0,0,0,
0,6,12,24,48,192,0,3,
24,192,0,6,0,192,0,6,
48,128,1,6,24,48,96,192,
0,0,0,0,0,192,24,62,
254,227,195,24,0,0,0,0,
3,12,252,240,3,3,12,0,
0,0,0,0,0,0,0,0,
0,0,48,192,128,1,0,0,
0,0,0,0,240,3,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,192,0,3,0,
0,0,24,48,96,192,128,1,
3,6,8,0,0,62,140,49,
198,24,99,140,49,198,24,62,
0,0,3,14,60,192,0,3,
12,48,192,0,3,0,62,140,
49,6,12,24,48,96,192,0,
127,0,224,195,24,96,128,193,
3,24,96,140,225,3,0,112,
224,193,134,25,99,134,249,15,
24,96,0,248,103,128,1,6,
248,3,24,96,128,249,3,0,
60,24,48,96,128,31,198,24,
99,12,31,0,240,199,24,96,
192,128,1,3,12,48,192,0,
0,31,198,24,99,12,31,198,
24,99,12,31,0,224,195,24,
99,140,225,7,24,48,96,224,
0,0,0,0,192,0,3,0,
0,192,0,3,0,0,0,0,
0,12,48,0,0,0,12,48,
96,0,0,24,48,96,192,128,
1,12,96,0,3,24,0,0,
0,128,127,0,0,0,128,127,
0,0,0,0,12,96,0,3,
24,192,128,1,3,6,12,0,
224,193,12,48,192,128,1,3,
0,48,192,0,0,0,248,48,
198,30,123,236,177,199,0,30,
48,224,65,136,97,134,249,103,
152,97,134,25,6,192,31,134,
24,102,152,63,134,25,102,200,
31,0,240,97,204,0,3,12,
48,192,0,198,240,1,192,31,
198,24,102,152,97,134,25,102,
204,31,0,248,103,128,1,6,
248,99,128,1,6,248,7,128,
63,6,24,96,128,31,6,24,
96,128,1,0,240,99,216,0,
3,12,48,222,96,134,241,3,
128,97,134,25,102,152,127,134,
25,102,152,97,0,240,3,3,
12,48,192,0,3,12,48,240,
3,0,120,192,0,3,12,48,
192,48,195,12,30,0,28,103,
140,25,54,120,96,131,25,198,
28,7,0,3,12,48,192,0,
3,12,48,196,24,127,0,24,
236,184,247,118,155,108,176,193,
6,27,12,128,193,14,123,108,
179,217,198,27,110,176,193,0,
224,193,140,97,134,25,102,152,
97,204,224,1,192,63,134,25,
102,152,63,6,24,96,192,1,
0,224,193,140,97,134,25,198,
12,30,192,0,6,192,63,134,
25,102,152,63,54,152,97,204,
97,0,240,97,12,3,24,192,
0,6,48,198,240,1,192,255,
48,192,0,3,12,48,192,0,
3,12,0,24,102,152,97,134,
25,102,152,97,204,224,1,128,
193,6,27,108,48,99,140,97,
131,15,28,0,24,108,176,193,
38,219,237,189,227,6,27,12,
192,192,134,49,131,7,12,120,
48,99,216,192,0,24,102,152,
97,134,241,3,3,12,48,192,
0,128,127,134,9,3,6,12,
24,48,104,176,255,0,120,96,
128,1,6,24,96,128,1,6,
120,0,0,0,6,48,128,1,
12,96,0,3,24,192,0,128,
7,24,96,128,1,6,24,96,
128,129,7,2,28,216,48,6,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,240,3,128,1,6,48,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,240,1,
12,63,198,240,6,192,1,6,
24,96,128,31,198,24,99,204,
30,0,0,0,0,0,0,240,
97,140,1,198,240,1,0,56,
192,0,3,12,63,198,24,99,
12,111,0,0,0,0,0,124,
24,99,140,63,6,240,1,0,
15,102,24,96,192,3,6,24,
96,128,1,0,0,0,0,0,
184,49,195,12,62,192,240,1,
192,1,6,24,96,128,31,206,
24,99,140,49,0,192,0,0,
0,56,192,0,3,12,48,192,
0,0,24,0,192,1,6,24,
96,152,97,6,15,0,0,112,
128,49,102,216,224,129,15,102,
28,3,0,0,56,192,0,3,
12,48,192,0,3,30,0,0,
0,0,0,0,152,225,143,53,
198,24,3,0,0,0,0,0,
128,31,198,24,99,140,49,0,
0,0,0,0,0,224,193,12,
51,204,224,1,0,0,0,0,
96,7,51,204,240,193,128,7,
0,0,0,0,0,112,99,134,
25,124,128,1,15,0,0,0,
0,0,128,29,220,48,195,128,
7,0,0,0,0,0,0,224,
193,0,30,192,240,1,0,0,
16,96,128,1,31,24,96,128,
9,28,0,0,0,0,0,0,
48,195,12,51,204,224,13,0,
0,0,0,0,0,99,140,49,
134,13,28,0,0,0,0,0,
0,48,198,26,127,252,97,3,
0,0,0,0,0,0,51,120,
192,128,7,51,0,0,0,0,
0,204,48,195,12,62,192,240,
1,0,0,0,0,0,0,63,
100,192,128,9,63,0,192,129,
1,6,24,56,128,1,6,24,
192,1,0,0,48,192,0,3,
12,48,192,0,3,12,0,192,
1,12,48,192,0,14,12,48,
192,192,1,0,110,236,0,0,
0,0,0,0,0,0,0,120,
48,99,152,127,134,25,102,153,
109,32,0,3,6,12,124,24,
99,128,1,6,24,96,12,31,
0,248,99,128,1,126,24,96,
128,1,254,128,1,128,1,6,
216,224,129,3,6,28,96,128,
127,192,128,1,131,195,30,219,
108,182,241,134,27,12,4,24,
48,240,99,152,97,134,25,102,
24,63,64,128,1,3,31,198,
24,192,7,48,198,240,1,0,
12,48,0,224,159,97,224,224,
224,152,127,0,0,1,6,12,
254,25,6,14,14,142,249,7,
0,0,0,0,192,7,48,252,
24,195,27,224,0,0,0,0,
24,48,240,96,134,1,102,240,
0,0,0,0,0,128,7,51,
252,48,128,15,112,0,0,96,
128,9,30,56,96,192,1,6,
248,3,0,0,64,128,1,131,
29,206,24,99,140,49,0,0,
0,0,16,96,192,192,131,25,
102,240,0,0,0,0,0,3,
6,30,12,224,1,12,31,0,
0,0,0,12,48,0,192,7,
12,24,240,1,0,0,0,0,
1,6,12,124,192,128,1,31);



var

minx,miny,maxx,maxy : word;

procedure texnfs (x,y : integer; source : ttex; s : word);
procedure line (x1,y1,x2,y2 : integer; c : byte);
procedure bline (buff : pointer; x1,y1,x2,y2,s : word; c : byte);
procedure convert (s : byte; var ch : char);
procedure unpack;


implementation

procedure texnfs;
var
sizeh,sizev,starttexx,starttexy,startxo,startyo,a,c : word;
endxo,endyo : integer;
startb,startt,width,height,shf,shftex,p,p2 : word;
b,t : pointer;
begin
     starttexx := 0;
     starttexy := 0;
     if (x >= maxx) or (y >= maxy) then exit;
     sizeh := source.sizex*s;
     p2 := source.sizex;
     sizev := source.sizey*s;
     endxo := x+sizeh;
     endyo := y+sizev;
     if (endxo <= 0) or (endyo <= 0) then exit;
     if x < 0 then
     begin
          startxo := 0;
          starttexx := abs (x) div s;
     end;
     if y < 0 then
     begin
          startyo := 0;
          starttexy := abs (y) div s;
     end;
     if endxo > maxx then endxo := maxx;
     if endyo > maxy then endyo := maxy;
     if x >= 0 then startxo := x;
     if y >= 0 then startyo := y;
     shf := 320 - endxo + startxo;
     a := (endxo-startxo) mod s;
     c := (endyo-startyo) mod s;
     shftex := sizeh - endxo + startxo;
     startb := startyo*320+startxo;
     if (starttexx > 0) or (starttexy > 0) then startt := starttexy*sizeh+starttexx
     else
     startt := 0;
     width := (endxo - startxo) div s;
     if width = 0 then exit;
     height := (endyo - startyo) div s;
     if height = 0 then exit;
     b := buf;
     t := source.texmemory;
     asm
        push ds
        cld {adresy rosn¥ce}
        lds si,t {ladowanie wsk. textury}
        les di,b {ladowanie wsk. buforu}
        add si,startt {start textury}
        add di,startb {start buforu}
        mov p,si
        @f_y_1:
               mov bx,s
               @f_y_2:
                      mov si,p
                      mov dx,width
                      @f_x_1:
                             mov al,[si]
                             mov cx,s
                             rep stosb
                             inc si
                             dec dx
                             jnz @f_x_1
                      mov al,[si]
                      mov cx,a
                      rep stosb
                      add di,shf
                      dec bx
                      jnz @f_y_2
               mov ax,p2
               add p,ax
               dec height
               jnz @f_y_1
        mov bx,c
        test bx,bx
        jz @d1
        @f_y_22:
                mov si,p
                mov dx,width
                @f_x_12:
                        mov al,[si]
                        mov cx,s
                        rep stosb
                        inc si
                        dec dx
                        jnz @f_x_12
                mov cx,a
                rep stosb
                add di,shf
                dec bx
                jnz @f_y_22
        @d1:
        pop ds
     end;
end;

procedure line;
var
dx,dy : integer;
zx1,zy1,zx2,zy2 : integer;
a,b,licz : word;
begin
     dx := x2-x1;
     dy := y2-y1;
     {}
     if dx < 0 then zx1 := -1;
     if dx = 0 then zx1 := 0;
     if dx > 0 then zx1 := 1;
     if dy < 0 then zy1 := -1;
     if dy = 0 then zy1 := 0;
     if dy > 0 then zy1 := 1;
     {}
     if dx < 0 then zx2 := -1;
     if dx = 0 then zx2 := 0;
     if dx > 0 then zx2 := 1;
     zy2 := 0;
     a := abs (dx);
     b := abs (dy);
     {}
     if a<=b then
     begin
          if dy < 0 then zy2 := -1;
          if dy = 0 then zy2 := 0;
          if dy > 0 then zy2 := 1;
          zx2 := 0;
          a := abs (dy);
          b := abs (dx);
     end;
     asm
        les di,buf
        mov ax,y1
        mov di,y1
        shl ax,8
        shl di,6
        add di,ax
        add di,x1
        mov ax,a
        mov licz,ax
        inc licz
        mov bx,ax
        shr bx,1
        @p:
           mov ax,minx
           cmp x1,ax
           jbe @d2
           mov ax,maxx
           cmp x1,ax
           jae @d2
           mov ax,miny
           cmp y1,ax
           jbe @d2
           mov ax,maxy
           cmp y1,ax
           jae @d2
           mov al,c
           mov es:[di],al
           @d2:
           add bx,b
           cmp bx,a
           jb @d
           sub bx,a
           mov dx,x1
           add dx,zx1
           mov cx,y1
           add cx,zy1
           mov x1,dx
           mov y1,cx
           mov dx,cx
           shl dx,8
           shl cx,6
           add dx,cx
           add dx,x1
           mov di,dx
           dec licz
           jnz @p
           jz @q
           @d:
           mov dx,x1
           add dx,zx2
           mov cx,y1
           add cx,zy2
           mov x1,dx
           mov y1,cx
           mov dx,cx
           shl dx,8
           shl cx,6
           add dx,cx
           add dx,x1
           mov di,dx
           dec licz
           jnz @p
        @q:
     end;
end;

procedure bline;
var
dx,dy : integer;
zx1,zy1,zx2,zy2 : integer;
a,b,licz : word;
begin
     dx := x2-x1;
     dy := y2-y1;
     {}
     if dx < 0 then zx1 := -1;
     if dx = 0 then zx1 := 0;
     if dx > 0 then zx1 := 1;
     if dy < 0 then zy1 := -1;
     if dy = 0 then zy1 := 0;
     if dy > 0 then zy1 := 1;
     {}
     if dx < 0 then zx2 := -1;
     if dx = 0 then zx2 := 0;
     if dx > 0 then zx2 := 1;
     zy2 := 0;
     a := abs (dx);
     b := abs (dy);
     {}
     if a<=b then
     begin
          if dy < 0 then zy2 := -1;
          if dy = 0 then zy2 := 0;
          if dy > 0 then zy2 := 1;
          zx2 := 0;
          a := abs (dy);
          b := abs (dx);
     end;
     asm
        les di,buff
        mov ax,y1
        mov dx,s
        mul dx
        mov di,ax
        add di,x1
        mov ax,a
        mov licz,ax
        inc licz
        mov bx,a
        shr bx,1
        @p:
           mov al,c
           mov es:[di],al
           add bx,b
           cmp bx,a
           jb @d
           sub bx,a
           mov dx,x1
           add dx,zx1
           mov cx,y1
           add cx,zy1
           mov x1,dx
           mov y1,cx
           mov ax,s
           mul cx
           add ax,x1
           mov di,ax
           dec licz
           jnz @p
           jz @q
           @d:
           mov dx,x1
           add dx,zx2
           mov cx,y1
           add cx,zy2
           mov x1,dx
           mov y1,cx
           mov ax,s
           mul cx
           add ax,x1
           mov di,ax
           dec licz
           jnz @p
        @q:
     end;
end;

procedure convert;
begin
     if s in [ltab1[0]..ltab1[1]] then ch := tab1[s-ltab1[0]];
     if s in [ltab2[0]..ltab2[1]] then ch := tab2[s-ltab2[0]];
     if s in [ltab3[0]..ltab3[1]] then ch := tab3[s-ltab3[0]];
end;

procedure unpack; assembler;
asm
   push ds
   les di,txttbl
   mov ax,seg txttblt
   mov ds,ax
   mov si,offset txttblt
   mov cx,1400
   @p1:
       mov dl,8
       mov bl,1
       lodsb
       @p2:
       mov ah,al
       and ah,bl
       jz @d1
       mov ah,127
       jmp @d2
       @d1:
       xor ah,ah
       @d2:
       mov es:[di],ah
       inc di
       shl bl,1
       dec dl
       jnz @p2
       loop @p1
   pop ds
end;
end.