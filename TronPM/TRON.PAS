{TRON v.0.99 copyright Kamil Korolkiewicz 2003
Ten program jest freeware jak i open source.
Moze by† dowolnie modyfikowany pod warunkiem
¾e, bedzie on nadal posiadal t¥ sam¥ nazw©.
Nie mo¾na r¢wnie¾ si© podpisywa† jako jego tw¢rca.
Ten kod «r¢dlowy programu nie posiada ¾adnych komentarzy
ze wzgl©du na jego bardzo prost¥ budowe.
Wszyskie zawarte procedury i funkcje mog¥ by† dowolnie
wykorzystywane w innych programach.
Przed rozpocz©ciem gry warto uruchomi† program setup
i przeczyta† plik readme.txt.
Progam nale¾y skompilowa† pod Turbo/Borland Pascal 7.0/7.1
(w Borland Pascal w trybie real).
Nie wymaga dodatkowych bibliotek.
Wszystkie procedury s¥ napisane wˆasnor©cznie.
Program ma na celu da† mi troch© satysfakcji jak i
przyjemno˜ci oraz jest dobrym przykˆadem prostej gry.
Zach©cam wszysktich do programowania i nauki Pascala
(jak i innych jezyk¢w np: C/C++,Assembler).
Miˆej zabawy ¾yczy Kamil Korolkiewicz ur. 23.09.1989 roku.
                                                   01.09.2003}
program tron;
uses crt,dos;
type
arr1 = array [0..65534] of byte;
arr2 = ^arr1;
bmp = record
            zarez1 : array [0..17] of byte;
            szer,wys : longint;
            zarez2 : array [0..27] of byte;
            pal : array [0..255] of record
                                          b,g,r,c : byte;
            end;
            pamob : arr2;
end;
mapa = array [0..63,0..63] of byte;
var
okbi : pointer;
k : array [0..127] of byte;
buf : pointer;
textures : array [0..8] of bmp;
disk : bmp;
maps : array [0..2] of ^mapa;
x,y,sx,sy,kx,ky,sfx,sfy,i,j,lx,ly : integer;
actmp : byte;
kie : byte;
w : longint;
dl : word;
delta : real;
rsfx,rsfy : real;
gl,p : word;
use386 : byte;
ot,t : longint;
lodpoz : byte;

procedure spp (c : byte); assembler;
asm
   mov dx,3c8h
   mov al,c
   out dx,al
end;

procedure setcol (r,g,b : byte); assembler;
asm
   mov dx,3c9h
   mov al,r
   out dx,al
   mov al,g
   out dx,al
   mov al,b
   out dx,al
end;

procedure getcol (c : byte; var r,g,b : byte);
var
_r,_g,_b : byte;
begin
     asm
        mov dx,3c7h
        mov al,c
        out dx,al
        mov dx,3c9h
        in al,dx
        mov _r,al
        in al,dx
        mov _g,al
        in al,dx
        mov _b,al
     end;
     r := _r;
     g := _g;
     b := _b;
end;

procedure wfvr;
begin
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
end;

procedure textura (x,y : integer; bmpeg : bmp);
var
starx,stary : word;
startx,starty : word;
endx,endy : integer;
endx2,endy2 : word;
sx,sy,arz,arc : word;
przesz,przesc : word;
z,c : pointer;
width : word;
begin
     if (x > 319) or (y > 199) then exit;
     sx := bmpeg.szer;
     sy := bmpeg.wys;
     starx := x;
     stary := y;
     startx := 0;
     starty := 0;
     endx := x+sx;
     endy := y+sy;
     if (endx <= 0) or (endy <= 0) then exit;
     if x < 0 then
     begin
          starx := 0;
          startx := abs (x);
     end;
     if y < 0 then
     begin
          stary := 0;
          starty := abs (y);
     end;
     endx2 := endx;
     endy2 := endy;
     if endx2 > 320 then endx2 := 320;
     if endy2 > 200 then endy2 := 200;
     z := bmpeg.pamob;
     c := buf;
     if (startx > 0) or (starty > 0) then arz := starty*sx+startx
     else
     arz := 0;
     arc := stary*320+starx;
     przesz := sx - endx2 + starx;
     przesc := 320 - endx2 + starx;
     width := endx2 - starx;
     asm
        push ds
        push es
        lds si,z
        les di,c
        add si,arz
        add di,arc
        mov bx,endy2
        sub bx,stary
        mov ax,przesz
        mov dx,przesc
        @s:
           mov cx,width
           test cx,1
           jz @noo
           movsb
           @noo:
           shr cx,1
           rep movsw
           add si,ax
           add di,dx
           dec bx
           jnz @s
        pop es
        pop ds
     end;
end;

procedure initvgagrph;
begin
     asm
        mov ax,13h
        int 10h
     end;
end;

procedure closevgagrp;
begin
     asm
        mov ax,3h
        int 10h
     end;
end;

procedure kbi; interrupt;
begin
     asm
        in al,60h
        mov bl,al
        xor bh,bh
        shr al,7
        xor al,1
        and bl,127
        mov byte ptr k[bx],al
        mov al,20h
        out 20h,al
     end;
end;

procedure initkeyboard;
begin
     getintvec ($9,okbi);
     setintvec ($9,@kbi);
end;

procedure closekeyboard;
begin
     setintvec ($9,okbi);
end;

procedure waittokeypress;
var
klaw : byte;
begin
     repeat
           for klaw := 0 to 127 do if k[klaw] = 1 then exit;
     until 0=2 ;
end;

procedure putpixel (x,y : word; c : byte);
begin
     asm
        push es
        les di,buf
        mov ax,y
        mov di,ax
        shl ax,8
        shl di,6
        add di,ax
        add di,x
        mov al,c
        mov es:[di],al
        pop es
     end;
end;

{Te dwie procedury wykorzystuj¥ instrukcje 16-bitowe(dla 80286) jak i 32-bitowe(dla 80386 i w wy¾ej)}

procedure copybuf;
begin
     asm
        push ds
        push es
        lds si,buf
        mov bx,0a000h
        mov es,bx
        xor di,di
        mov al,use386
        test al,1
        jnz @no
        mov cx,16000 {ecx=00003e80}
        cld
        rep     {-|}
        db 66h  {-|-------------rep stosd}
        movsw   {-|}
        jmp @q
        @no:
        mov cx,32000 {cx=7d00}
        cld
        rep movsw
        @q:
        pop es
        pop ds
     end;
end;

procedure clearbuf;
begin
     asm
        push es
        les di,buf
        xor di,di
        mov al,use386
        test al,1
        jnz @no
        db 66h     {-| --------xor eax,eax}
        xor ax,ax  {-|}
        mov cx,16000 {eax=00000000 ecx=00003e80}
        cld
        rep    {-|}
        db 66h {-|-------------rep stosd}
        stosw  {-|}
        jmp @q
        @no:
        xor ax,ax
        mov cx,32000 {ax=0000 cx=7d00}
        cld
        rep stosw
        @q:
        pop es
     end;
end;

procedure negatyw (t : byte);
var
color : byte;
begin
     spp (0);
     for color := 0 to 255 do
     begin
          setcol (255-textures[0].pal[color].r shr 2,
                  255-textures[0].pal[color].g shr 2,
                  255-textures[0].pal[color].b shr 2);
     end;
     for color := 0 to t do
     wfvr;
     spp (0);
     for color := 0 to 255 do
     begin
          setcol (textures[0].pal[color].r shr 2,
                  textures[0].pal[color].g shr 2,
                  textures[0].pal[color].b shr 2);
     end;
end;

procedure sciemnij;
var
color,i,red,green,blue : byte;
begin
     for i := 0 to 63 do
     begin
          for color := 0 to 255 do
          begin
               getcol (color,red,green,blue);
               if red > 0 then dec (red);
               if green > 0 then dec (green);
               if blue > 0 then dec (blue);
               spp (color);
               setcol (red,green,blue);
          end;
          wfvr;
     end;
     clearbuf;
     copybuf;
     spp (0);
     for color := 0 to 255 do
     begin
          setcol (textures[0].pal[color].r shr 2,
                  textures[0].pal[color].g shr 2,
                  textures[0].pal[color].b shr 2);
     end;
end;

function loadbmp (s : string; var bmpeg : bmp; sp : boolean) : boolean;
var
f : file;
i : word;
siz : word;
begin
     assign (f,s);
     {$I-}
     reset (f,1);
     {$I+}
     if ioresult <> 0 then
     begin
          loadbmp := false;
          exit;
     end;
     blockread (f,bmpeg,1078);
     siz := bmpeg.szer * bmpeg.wys;
     getmem (bmpeg.pamob,siz);
     seek (f,1078);
     for i := 0 to bmpeg.wys-1 do
     blockread (f,bmpeg.pamob^[(bmpeg.wys-1-i)*bmpeg.szer],bmpeg.szer);
     asm
        mov dx,3c8h
        mov al,0
        out dx,al
     end;
     if sp then
     for i := 0 to 255 do setcol (bmpeg.pal [i].r shr 2,bmpeg.pal [i].g shr 2,bmpeg.pal [i].b shr 2);
     close (f);
     loadbmp := true;
end;

function loadmap (s : string; no : byte) : boolean;
var
f : file;
begin
     assign (f,s);
     {$I-}
     reset (f,1);
     {$I+}
     if ioresult <> 0 then
     begin
          loadmap := false;
          exit;
     end;
     getmem (maps[no],4096);
     blockread (f,maps[no]^,4096);
     close (f);
end;

procedure l;
begin
     textura (312,192,disk);
     copybuf;
end;


procedure error (s : string);
var
i : byte;
begin
     closevgagrp;
     textbackground (1);
     textcolor (7);
     for i := 0 to 79 do write (' ');
     gotoxy (40-(length (s) div 2),1);
     write (s);
     halt;
end;

procedure loadtextures;
var
i,j : byte;
stri : string;
begin
     spp (196);
     setcol (60,0,0);
     setcol (63,0,0);
     for i := 0 to 5 do
     begin
          str (i+1,stri);
          if not loadbmp ('tex'+stri+'.bmp',textures[i],false)
          then
          error ('ERROR 1: FILE NOT FOUND');
          for j := 0 to 33 do
          begin
               putpixel ((lodpoz*35+2)+j,178,j mod 2+196);
               putpixel ((lodpoz*35+3)+j,179,j mod 2+196);
               putpixel ((lodpoz*35+2)+j,180,j mod 2+196);
               putpixel ((lodpoz*35+3)+j,181,j mod 2+196);
               putpixel ((lodpoz*35+2)+j,182,j mod 2+196);
               putpixel ((lodpoz*35+3)+j,183,j mod 2+196);
          end;
          copybuf;
          inc (lodpoz);
     end;
end;

procedure loadtitles;
var
i : byte;
stri : string;
begin
     l;
     for i := 6 to 8 do
     begin
          str (i+1,stri);
          loadbmp ('tex'+stri+'.bmp',textures[i],false);
     end;
end;

procedure freetitles;
var
i : byte;
begin
     for i := 6 to 8 do freemem (textures[i].pamob,textures[i].szer*textures[i].wys);
end;

procedure loadmaps;
var
i : byte;
stri : string;
begin
     for i := 0 to 2 do
     begin
          str (i+1,stri);
          if not loadmap ('map'+stri+'.map',i)
          then
          error ('ERROR 1: FILE NOT FOUND');
          for j := 0 to 33 do
          begin
               putpixel ((lodpoz*35+2)+j,178,j mod 2+196);
               putpixel ((lodpoz*35+3)+j,179,j mod 2+196);
               putpixel ((lodpoz*35+2)+j,180,j mod 2+196);
               putpixel ((lodpoz*35+3)+j,181,j mod 2+196);
               putpixel ((lodpoz*35+2)+j,182,j mod 2+196);
               putpixel ((lodpoz*35+3)+j,183,j mod 2+196);
          end;
          copybuf;
          inc (lodpoz);
     end;
end;

procedure gp (var x1,y1,x2,y2 : integer; no : byte);
var
i,j : byte;
begin
     for i := 0 to 63 do
     for j := 0 to 63 do
     begin
          if maps[no]^[i,j] = 2 then
          begin
               x1 := j*32;
               y1 := i*32;
          end
          else
          if maps[no]^[i,j] = 3 then
          begin
               x2 := j*32;
               y2 := i*32;
          end;
     end;
end;

procedure loadconfig;
var
f : file of word;
begin
     l;
     assign (f,'tron.cfg');
     {$I-}
     reset (f);
     {$I+}
     if ioresult <> 0 then
     begin
          gl := 2000;
          p := 1;
          exit;
     end;
     read (f,gl);
     read (f,p);
     close (f);
end;

procedure testproc;
begin
     if test8086 < 2 then use386 := 0
     else
     use386 := 1;
end;

procedure init;
var
i : integer;
begin
     initvgagrph;
     directvideo := false;
     lodpoz := 0;
     loadbmp ('tex10.bmp',disk,true);
     if maxavail < 190000 then error ('ERROR 2: TOO SMALL OF MEMORY');
     initkeyboard;
     testproc;
     getmem (buf,64000);
     clearbuf;
     loadconfig;
     loadtitles;
     for i := -100 to 10 do
     begin
          clearbuf;
          textura (18,i*2,textures[6]);
          wfvr;
          copybuf;
     end;
     textura (65,190,textures[8]);
     copybuf;
     waittokeypress;
     clearbuf;
     textura (18,20,textures[6]);
     textura (0,171,textures[7]);
     freetitles;
     copybuf;
     loadtextures;
     loadmaps;
     wfvr;
     sciemnij;
     actmp := 0;
     gp (sx,sy,kx,ky,actmp);
end;

procedure shutdown;
var
i : byte;
pomo : longint;
begin
     nosound;
     for i := 0 to 2 do freemem (maps [i],4096);
     for i := 0 to 5 do freemem (textures [i].pamob,1024);
     freemem (buf,64000);
     freemem (disk.pamob,64);
     closekeyboard;
     closevgagrp;
     textcolor (7);
     writeln ('TRON (c) COPYRIGHT KaMiL KoRoLkIeWiCz 2003');
     writeln (w,' frames');
     if t < ot then
     begin
          pomo := t;
          t := ot;
          ot := pomo;
     end;
     if trunc ((t-ot)*0.054945054) > 59 then
     writeln ('You played ',trunc ((t-ot)*0.00091575) mod 60,' minutes and ',trunc ((t-ot)*0.054945054) mod 60,' seconds.')
     else
     writeln ('You played ',trunc ((t-ot)*0.054945054),' seconds.');
     writeln (w/((t-ot)*0.054945054):0:0,' fps');
     halt;
end;

procedure wysw;
var
i,j : integer;
dx,dy : shortint;
ex,ey : shortint;
kx,ky : integer;
begin
     if sfx < 144 then lx := sfx;
     if sfy < 84 then ly := sfy;
     if sfx > 1872 then lx := sfx-1728;
     if sfy > 1932 then ly := sfy-1848;
     dx := (sfx-lx) div 32+1;
     dy := (sfy-ly) div 32+1;
     ex := (word (2015-sfx)+(lx mod 32)) mod 32+1;
     ey := (word (2015-sfy)+(ly mod 32)) mod 32+1;
     for j := -1 to 6 do
     for i := -1 to 9 do
     begin
     kx := i+dx;
     ky := j+dy;
     if (maps[actmp]^[ky,kx] <> 2) and (maps[actmp]^[ky,kx] <> 3) then
     textura (i*32+ex,j*32+ey,textures[maps[actmp]^[ky,kx]])
     else
     textura (i*32+ex,j*32+ey,textures[0])
     end;
     textura (lx,ly,textures [kie+2]);
     wfvr;
     copybuf;
end;

procedure beep;
begin
     sound (gl);
end;

procedure savegame;
var
f : file of byte;
bsfx,bsfy : byte;
begin
     l;
     assign (f,'savegame.sav');
     rewrite (f);
     write (f,actmp);
     bsfx := sfx div 32;
     write (f,bsfx);
     bsfy := sfy div 32;
     write (f,bsfy);
     close (f);
     k[64]:=0;
end;

procedure loadgame;
var
f : file of byte;
bsfx,bsfy : byte;
begin
     l;
     assign (f,'savegame.sav');
     {$I-}
     reset (f);
     {$I+}
     if ioresult <> 0 then exit;
     read (f,actmp);
     read (f,bsfx);
     read (f,bsfy);
     gp (sx,sy,kx,ky,actmp);
     rsfx := bsfx*32;
     rsfy := bsfy*32;
     sfx := trunc (rsfx);
     sfy := trunc (rsfy);
     delta := 0;
     lx := 144;
     ly := 84;
     close (f);
     k[67]:=0
end;

begin
     init;
     lx := 144;
     ly := 84;
     sfx := sx;
     sfy := sy;
     rsfx := sx;
     rsfy := sy;
     w := 0;
     delta := 0;
     ot := meml [0000:1132];
     while (0=0) do
     begin
          dl := round (sqrt (abs (((kx div 32)-(sfx div 32))
          *((kx div 32)-(sfx div 32))+((ky div 32)-(sfy div 32))*((ky div 32)-(sfy div 32))))) * p;
          if dl > 0 then
          if (w mod dl = 0) then beep;
          if k[72] = 1 then
          begin
               if (maps [actmp]^[(sfy-1) div 32,sfx div 32] <> 1)
               and
               (maps [actmp]^[(sfy-1) div 32,(sfx+31) div 32] <> 1) then
               begin
               if rsfy > 0 then
               rsfy := rsfy-delta;
               if delta < 1.0 then
               delta := delta+0.05;
               end
               else
               delta := 0;
               kie := 0;
          end
          else
          if k[80] = 1 then
          begin
               if (maps [actmp]^[(sfy+32) div 32,sfx div 32] <> 1)
               and
               (maps [actmp]^[(sfy+32) div 32,(sfx+31) div 32] <> 1) then
               begin
               if rsfy < 2016 then
               rsfy := rsfy+delta;
               if delta < 1.0 then
               delta := delta+0.05;
               end
               else
               delta := 0;
               kie := 2;
          end
          else
          if k[75] = 1 then
          begin
               if (maps [actmp]^[sfy div 32,(sfx-1) div 32] <> 1)
               and
               (maps [actmp]^[(sfy+31) div 32,(sfx-1) div 32] <> 1) then
               begin
               if rsfx > 0 then
               rsfx := rsfx-delta;
               if delta < 1.0 then
               delta := delta+0.05;
               end
               else
               delta := 0;
               kie := 3;
          end
          else
          if k[77] = 1 then
          begin
               if (maps [actmp]^[sfy div 32,(sfx+32) div 32] <> 1)
               and
               (maps [actmp]^[(sfy+31) div 32,(sfx+32) div 32] <> 1) then
               begin
               if rsfx < 2016 then
               rsfx := rsfx+delta;
               if delta < 1.0 then
               delta := delta+0.05;
               end
               else
               delta := 0;
               kie := 1;
          end
          else
          begin
               if delta > 0 then
               delta := delta-0.05
               else
               delta := 0;
               case kie of
               0:if (maps [actmp]^[(sfy-1) div 32,sfx div 32] <> 1)
                    and
                       (maps [actmp]^[(sfy-1) div 32,(sfx+31) div 32] <> 1)
                       and
                          (rsfy > 0)
                                then
                                    rsfy := rsfy-delta
                                         else delta := 0;
               1:if (maps [actmp]^[sfy div 32,(sfx+32) div 32] <> 1)
                    and
                       (maps [actmp]^[(sfy+31) div 32,(sfx+32) div 32] <> 1)
                       and
                          (rsfx < 2016)
                                then
                                    rsfx := rsfx+delta
                                         else delta := 0;
               2:if (maps [actmp]^[(sfy+32) div 32,sfx div 32] <> 1)
                    and
                       (maps [actmp]^[(sfy+32) div 32,(sfx+31) div 32] <> 1)
                       and
                          (rsfy < 2016)
                                then
                                    rsfy := rsfy+delta
                                         else delta := 0;
               3:if (maps [actmp]^[sfy div 32,(sfx-1) div 32] <> 1)
                    and
                       (maps [actmp]^[(sfy+31) div 32,(sfx-1) div 32] <> 1)
                       and
                          (rsfx > 0)
                                then
                                    rsfx := rsfx-delta
                                         else delta := 0;
               end;
          end;
          sfx := trunc (rsfx);
          sfy := trunc (rsfy);
          if ((sfx div 32) = (kx div 32)) and ((sfy div 32) = (ky div 32)) then
          begin
               nosound;
               if actmp < 2 then
               begin
                    inc (actmp);
                    delta := 0;
                    gp (sfx,sfy,kx,ky,actmp);
                    rsfx := sfx;
                    rsfy := sfy;
                    negatyw (9);
                    sciemnij;
               end
               else
               begin
                    nosound;
                    t := meml [0000:1132];
                    negatyw (9);
                    sciemnij;
                    clearbuf;
                    copybuf;
                    gotoxy (17,12);
                    spp (7);
                    setcol (0,0,60);
                    writeln ('YOU WIN!!!');
                    delay (500);
                    shutdown;
               end;
          end;
          if k[1]=1 then break;
          if k[61]=1 then
          begin
               actmp := 0;
               gp (sx,sy,kx,ky,actmp);
               rsfx := sx;
               rsfy := sy;
               continue;
          end;
          if k[64]=1 then savegame
          else
          if k[67]=1 then loadgame;
          wysw;
          nosound;
          inc (w);
     end;
     t := meml [0000:1132];
     nosound;
     clearbuf;
     copybuf;
     gotoxy (16,12);
     spp (7);
     setcol (60,0,0);
     textcolor (7);
     writeln ('YOU LOSE!!!');
     delay (500);
     shutdown;
end.