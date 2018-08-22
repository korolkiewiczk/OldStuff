{
  TRAP (r) Copyright (c) Kamil Korolkiewicz 2003
  Darmowa biblioteka do wy˜wietlania tekst¢w na potzeby gry TRAP (r)
  Wszelkie prawa autorskie zastrze¾one
}

{$S-,G+}
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

procedure textprint (x,y : word; pc,sc : byte; stri : pchar); {pisze normalnego stringa}
procedure textscale; {skaluje bitmape (pchr) litery do dwa razy mniejszej}
procedure textsetcolor (c : byte); {ustawia kolor pisanego tekstu}
procedure textsetfadecolor (c : byte); {ustawia kolor przezroczysto˜ci}
procedure textsettex (x,y : byte; tex : ttex); {ustawia teksture tex jako tˆo tekstu zaczynaj¥c od x i y}
procedure textsetfade (c : byte); {ustawia przyciemnienie tekstu przy klorze c}
procedure textunspr; {powoduje ¾e tekst ma czarne tˆo}
procedure textspr; {powoduje ¾e tekst nie ma tˆa}
procedure textdefaults; {ustawia wszystko domu˜lnie}

implementation

{definicje procedury i funkcji}

procedure textprint;
var
i,j,j2,x_,y_ : word;
l : byte;
c,c2 : char;
przes,przes2 : byte;
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
     przes2 := przes;
     x_ := x;
     y_ := y;
     i := 0;
     c := stri[i];
     c2 := stri[i+1];
     dec (i);
     while c <> #0 do
     begin
          inc (i);
          przes := przes2;
          c := stri[i];
          c2 := stri[i+1];
          if (c = #13) or (c = #10) then
          begin
               x_ := x;
               y_ := y_+przes2;
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
          if c2 < #127 then j2 := ord (c2)-33
          else
          case c2 of
               #164: j2 := 94;  {¤}
               #143: j2 := 95;  {}
               #168: j2 := 96;  {¨}
               #157: j2 := 97;  {}
               #227: j2 := 98;  {ã}
               #224: j2 := 99;  {à}
               #151: j2 := 100; {—}
               #189: j2 := 101; {½}
               #141: j2 := 102; {}
               #165: j2 := 103; {¥}
               #134: j2 := 104; {†}
               #169: j2 := 105; {©}
               #136: j2 := 106; {ˆ}
               #228: j2 := 107; {ä}
               #162: j2 := 108; {¢}
               #152: j2 := 109; {˜}
               #190: j2 := 110; {¾}
               #171: j2 := 111; {«}
               else
               continue;
          end;
          if sc = 0 then
          begin
               if ((c >= #97) and (c <= #122)) or ((c2 >= #97) and (c2 <= #122)) then przes := 8; {dla a..z}
               if ((j >= 103) and (j <= 111)) or ((j2 >= 103) and (j2 <= 111)) then przes := 8; {dla ¥..«}
          end;
          if sc = 1 then
          begin
               if ((c >= #97) and (c <= #122)) or ((c2 >= #97) and (c2 <= #122)) then przes := 4; {dla a..z}
               if ((j >= 103) and (j <= 111)) or ((j2 >= 103) and (j2 <= 111)) then przes := 4; {dla ¥..«}
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

procedure textscale; assembler;
asm
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
   mov ax,offset pomchr
   mov dx,seg pomchr
   mov bx,offset pchr.texmemory
   mov [bx],ax
   mov [bx+2],dx
   {pchr.texmemory := addr (pomchr);}
end;

procedure textsetcolor;
begin
asm
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
     s := y*i+x;
     asm
        push ds
        les di,txttbl
        lds si,tex.texmemory
        mov dl,i
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
     end;
end;

procedure textsetfade;
begin
asm
   les di,txttbl
   mov bx,112
   mov al,c
   @start:
          push ax
          mov cx,10
          @d1:
              push cx
              mov cx,10
              @d2:
              cmp byte ptr es:[di],0
              je @dpom
              mov es:[di],al
              @dpom:
                    inc di
                    loop @d2
              pop cx
              inc al
              loop @d1
          dec bx
          pop ax
          jnz @start
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