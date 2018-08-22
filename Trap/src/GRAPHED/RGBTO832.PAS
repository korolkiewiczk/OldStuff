program rgbto832;
uses crt,scr;
const
cl = 1.8;
type
bmpb = array [0..$35] of byte;
bmpl = array [0..$36 div 4-1] of longint;
ar = array [0..65534] of byte;
var
f : file;
fn,fn2 : string;
c : char;
p2 : byte;
bmp : ^bmpb;
bmp2 : ^bmpl;
x,y,s : longint;
ec : ^ar;
i,j,k,o,p : word;
r,g,b,e : byte;
tx : ttex;
a : word;
begin
     if paramcount < 1 then readln (fn)
     else
     fn := paramstr (1);
     c := fn[1];
     p := 1;
     while (c <> '.') and (p < length (fn)) do
     begin
          inc (p);
          c := fn[p];
     end;
     fn2 := copy (fn,1,p);
     fn2 := fn2+'832';
     assign (f,fn);
     {$I-}
     reset (f,1);
     {$I+}
     if ioresult <> 0 then exit;
     getmem (buf,64000);
     getmem (bmp,sizeof (bmpb));
     blockread (f,bmp^,sizeof (bmpb));
     if bmp^[$1c] <> 24 then begin close (f); exit; end;
     bmp2 := ptr (seg(bmp^),ofs(bmp^)+$12);
     x := bmp2^[0];
     y := bmp2^[1];
     s := x*y;
     a := x mod 4;
     if s > 65535 then begin close (f); exit; end;
     getmem (ec,s);
     {writeln ('s',s);
     writeln ('s*3',s*3);
     writeln ('x',x);
     writeln ('y',y);
     writeln (filesize(f));}
     tx.sizex := x;
     tx.sizey := y;
     p := 0;
     for i := 0 to s-1 do
     begin
          blockread (f,b,1);
          blockread (f,g,1);
          blockread (f,r,1);
          b := b div 8;
          g := g div 8;
          r := r div 8;
          if a <> 0 then if (i mod x=x-1) and (i <> 0) then begin for j := 1 to a do blockread (f,p2,1); end;
          if (r = 0) and (g = 0) and (b = 0) then
          begin
               ec^[i] := 0;
          end;
          if r = 0 then r := 1;
          if g = 0 then g := 1;
          if b = 0 then b := 1;
          if (r >= g) and (r >= b) then
          begin
               if (r / g <= cl) and (r / b <= cl) then
               begin
                    e := 3;
                    o := (r+g+b) div 3;
               end
               else
               if r / b <= cl then
               begin
               e := 5;
               o := (r+b) div 2;
               end
               else
               if (r / g >= cl*0.75) and (r / g <= cl*1.25) then
               begin
                    e := 7;
                    o := (r+round(g * cl*0.75)) div 2;
               end
               else
               if r / g < cl*0.75 then
               begin
                    e := 4;
                    o := (r+g) div 2;
               end
               else
               begin
                    e := 0;
                    o := r;
               end;
          end
          else
          if (g >= r) and (g >= b) then
          begin
               if (g / r <= cl) and (g / b <= cl) then
               begin
                    e := 3;
                    o := (r+g+b) div 3;
               end
               else
               if g / r <= cl then
               begin
                    e := 4;
                    o := (g+r) div 2;
               end
               else
               if g / b <= cl then
               begin
                    e := 6;
                    o := (g+b) div 2;
               end
               else
               begin
                    e := 1;
                    o := g;
               end;
          end
          else
          if (b >= r) and (b >= g) then
          begin
               if (b / r <= cl) and (b / g <= cl) then
               begin
                    e := 3;
                    o := (r+g+b) div 3;
               end
               else
               if b / r <= cl then
               begin
                    e := 5;
                    o := (b+r) div 2;
               end
               else
               if b / g <= cl then
               begin
                    e := 6;
                    o := (b+g) div 2;
               end
               else
               begin
                    e := 2;
                    o := b;
               end;
          end
          else
          begin
               e := 3;
               o := (r+g+b) div 3;
          end;
          ec^[i] := e*32+o;
     end;
     close (f);
     tx.texmemory := ec;
     initvgascr;
     set_8_32_pal;
     setpalette;
     buftoclr;
     scrtexnf (0,0,tx);
     buftoscr;
     readkey;
     inittxtscr;
     assign (f,fn2);
     rewrite (f,1);
     blockwrite (f,x,2);
     blockwrite (f,y,2);
     for i := y-1 downto 0 do
     blockwrite (f,mem [seg (ec^):ofs (ec^)+i*x],x);
     close (f);
end.