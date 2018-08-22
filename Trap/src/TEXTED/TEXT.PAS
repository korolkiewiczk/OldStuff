{
  TRAP (r) Copyright (c) Kamil Korolkiewicz 2003
  Darmowa biblioteka do wy˜wietlania tekst¢w na potzeby gry TRAP (r)
  Wszelkie prawa autorskie zastrze¾one
}

unit text;

interface

{doˆ¥czone moduˆy}

uses scr;

{deklaracje staˆych}

const

normaltext = 0;
transpartext = 1;

{deklaracje typ¢w}

type

characters = array [0..11199] of byte;

{deklaracje zmiennych}

var

txttbl : ^characters;
pomchr : array [0..24] of byte;
pchr : ttex;
_spr : boolean;
fc : byte;

{deklaracje procedur i funkcji}

procedure textprint (x,y : word; pc,sc : byte; stri : string); {pisze normalnego stringa}
procedure textscale; {skaluje bitmape (pchr) litery do dwa razy mniejszej}
procedure textsetcolor (c : byte); {ustawia kolor pisanego tekstu}
procedure textsetfadecolor (c : byte); {ustawia kolor przezroczysto˜ci}
procedure textsettex (x,y : byte; tex : ttex); {ustawia teksture tex jako tˆo tekstu zaczynaj¥c od x i y}
procedure textunspr; {powoduje ¾e tekst ma czarne tˆo}
procedure textspr; {powoduje ¾e tekst nie ma tˆa}
procedure textdefaults; {ustawia wszystko domu˜lnie}

implementation

{definicje procedury i funkcji}

procedure textprint;
var
i,j,x_,y_ : word;
l : byte;
c : char;
przes : byte;
begin
     if sc = 0 then
     begin
          przes := 11;
          pchr.sizex := 10;
          pchr.sizey := 10;
     end
     else
     if sc = 1 then
     begin
          przes := 6;
          pchr.sizex := 5;
          pchr.sizey := 5;
     end;
     l := length (stri);
     x_ := x;
     y_ := y;
     for i := 1 to l do
     begin
          c := stri[i];
          if c = #13 then
          begin
               x_ := x;
               y_ := y+przes;
               continue;
          end;
          if c = #32 then
          begin
               x_ := x_+przes;
               continue;
          end;
          if c < #32 then continue;
          if c < #127 then j := ord (c)-33
          else
          case c of
               #164: j := 94;  {¤}
               #143: j := 95;  {}
               #168: j := 96;  {¨}
               #157: j := 97;  {}
               #227: j := 98;  {ã}
               #224: j := 99;  {à}
               #151: j := 100; {—}
               #189: j := 101; {½}
               #141: j := 102; {}
               #165: j := 103; {¥}
               #134: j := 104; {†}
               #169: j := 105; {©}
               #136: j := 106; {ˆ}
               #228: j := 107; {ä}
               #162: j := 108; {¢}
               #152: j := 109; {˜}
               #190: j := 110; {¾}
               #171: j := 111; {«}
               else
               continue;
          end;
          pchr.texmemory := ptr (seg(txttbl^[j*100]),ofs(txttbl^[j*100]));
          if sc = 1 then textscale;
          if pc = normaltext then
          begin
               if _spr then
               scrsprtexnf (x_,y_,pchr)
               else
               scrtexnf (x_,y_,pchr);
          end
          else
          if pc = transpartext then
          begin
               if _spr then
               scrsprtextf (x_,y_,fc,pchr)
               else
               scrtextf (x_,y_,fc,pchr);
          end;
          x_ := x_+przes;
     end;
end;

procedure textscale;
var
i,j : byte;
a : byte;
begin
     asm
        push es
        mov di,offset pomchr
        mov ax,seg pomchr
        mov es,ax
        mov ax,0
        mov cx,25/2
        stosb
        rep stosw
        push ds
        mov di,offset pomchr
        lds si,pchr.texmemory
        mov cx,5
        add si,11
        @s1:
            push cx
            mov cx,5
            @s2:
                mov al,[si]
                stosb
                inc si
                inc si
                loop @s2
            pop cx
            add si,10
            loop @s1
        pop ds
        pop es
     end;
     pchr.texmemory := addr (pomchr);
end;

procedure textsetcolor;
begin
     asm
        push es
        les di,txttbl
        mov al,c
        xor ah,ah
        mov cx,11200
        @start:
               cmp byte ptr es:[di],0
               je @dpom
               mov es:[di],al
               @dpom:
               inc di
               loop @start
        pop es
     end;
end;

procedure textsetfadecolor;
begin
     fc := c;
end;

procedure textsettex;
var
i,j : byte;
s : word;
begin
     i := tex.sizex;
     j := tex.sizey;
     if (i < 10) or (j < 10) then exit;
     s := y*j+x;
     asm
        push es
        push ds
        les di,txttbl
        lds si,tex.texmemory
        mov dl,j
        xor dh,dh
        sub dx,10
        mov bx,112
        @start:
               push si
               add si,s
               mov cx,10
               @d1:
                   push cx
                   mov cx,10
                   @d2:
                       cmp byte ptr es:[di],0
                       je @dpom
                       mov al,[si]
                       mov es:[di],al
                       @dpom:
                       inc si
                       inc di
                       loop @d2
                   pop cx
                   add si,dx
                   loop @d1
               dec bx
               pop si
               jnz @start
        pop ds
        pop es
     end;
end;
procedure textunspr;
begin
     _spr := false;
end;

procedure textspr;
begin
     _spr := true;
end;

procedure textdefaults;
begin
     textspr;
     textsetcolor (127);
     textsetfadecolor (31);
end;

end.