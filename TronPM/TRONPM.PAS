{$ASMMODE INTEL}
{$S-}
{TRONPM v.1.0 copyright Kamil Korolkiewicz 2003/2004
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
Progam nale¾y skompilowa† pod Free pascal compiler
Nie wymaga dodatkowych bibliotek.
Wszystkie procedury s¥ napisane wˆasnor©cznie.
Program ma na celu da† mi troch© satysfakcji jak i
przyjemno˜ci oraz jest dobrym przykˆadem prostej gry.
Zach©cam wszysktich do programowania i nauki Pascala
(jak i innych jezyk¢w np: C/C++,Assembler).
Miˆej zabawy ¾yczy Kamil Korolkiewicz ur. 23.09.1989 roku.
                                                   01.09.2003}
program tronpm;
uses crt,go32;
type
moderec = record
                bmode : word;
                swidth,sheight : longint;
end;
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
const
modes : array [0..4] of moderec = (
                                (bmode:$100; swidth:640; sheight:400),
                                (bmode:$101; swidth:640; sheight:480),
                                (bmode:$103; swidth:800; sheight:600),
                                (bmode:$105; swidth:1024; sheight:768),
                                (bmode:$107; swidth:1280; sheight:1024));
var
nkbi,okbi : tseginfo;
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
ot,t : longint;
lodpoz : byte;
lfbp : longint;
lfbs : word;
scrwidth,scrheight,scrsiz : longint;
res : word;

function gettime : longint;
begin
     asm
        mov edi,1132
        mov eax,gs:[edi]
        mov @result,eax
     end;
end;

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

procedure textura (x,y : longint; bmpeg : bmp);
var
starx,stary : longint;
startx,starty : longint;
endx,endy : longint;
endx2,endy2 : longint;
sx,sy,arz,arc : longint;
przesz,przesc : longint;
z,c : pointer;
width : longint;
begin
     if (x > scrwidth-1) or (y > scrheight-1) then exit;
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
     if endx2 > scrwidth then endx2 := scrwidth;
     if endy2 > scrheight then endy2 := scrheight;
     z := bmpeg.pamob;
     c := buf;
     if (startx > 0) or (starty > 0) then arz := starty*sx+startx
     else
     arz := 0;
     arc := stary*scrwidth+starx;
     przesz := sx - endx2 + starx;
     przesc := scrwidth - endx2 + starx;
     width := endx2 - starx;
     asm
        mov esi,z
        mov edi,c
        add esi,arz
        add edi,arc
        mov ebx,endy2
        sub ebx,stary
        mov edx,przesc
        @s:
           mov ecx,width
           mov eax,ecx
           shr eax,2
           and ecx,3
           rep movsb
           mov ecx,eax
           rep movsd
           add esi,przesz
           add edi,edx
           dec ebx
           jnz @s
     end;
end;

procedure initvesagrph (mode : word);
var
h,l,i : word;
p : longint;
ad : longint;
r : trealregs;
pbuf : array [0..256] of byte;
begin
     p := global_dos_alloc (256);
     fillchar (r,sizeof (r),0);
     r.ax := $4f01;
     r.cx := mode;
     r.es := p shr 16;
     r.di := 0;
     realintr ($10,r);
     dosmemget (p shr 16,0,pbuf,256);
     asm
        mov eax,[pbuf+28h]
        mov ad,eax
        mov lfbs,0
        mov ax,0800h
        mov ebx,ad
        mov ecx,ebx
        shr ebx,16
        and ecx,16
        mov si,0h
        mov di,0ffffh
        int 31h
        jnc @d
        mov lfbs,0ffffh
        @d:
        mov h,bx
        mov l,cx
     end;
     global_dos_free (word(p));
     if (lfbs = $ffff) or (h=0) or (l=0) then begin writeln ('Lineal frame buffer not supprorted'); halt (1) end;
     lfbp := h*65536+l;
     lfbs := allocate_ldt_descriptors (1);
     set_segment_base_address (lfbs,lfbp);
     set_segment_limit (lfbs,$ffffff);
     asm
        mov ax,4f02h
        mov bx,mode
        int 10h
     end;
     i := 0;
     while modes[i].bmode <> mode do inc (i);
     scrwidth := modes[i].swidth;
     scrheight := modes[i].sheight;
     scrsiz := scrwidth*scrheight;
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
        cli
        xor ebx,ebx
        in al,60h
        mov bl,al
        xor bh,bh
        shr al,7
        xor al,1
        and bl,127
        mov byte ptr k[ebx],al
        mov al,20h
        out 20h,al
        sti
     end;
end;

procedure initkeyboard;
begin
     get_pm_interrupt ($9,okbi);
     nkbi.offset := @kbi;
     nkbi.segment := get_cs;
     set_pm_interrupt ($9,nkbi);
end;

procedure closekeyboard;
begin
     set_pm_interrupt ($9,okbi);
end;

procedure waittokeypress;
var
klaw : byte;
begin
     repeat
           for klaw := 0 to 127 do if k[klaw] = 1 then exit;
     until 0=2 ;
end;

procedure putpixel (x,y : longint; c : byte);
begin
     asm
        mov edi,buf
        mov ebx,y
        mov eax,scrwidth
        mul ebx
        add eax,x
        add edi,eax
        mov al,c
        mov es:[edi],al
     end;
end;

{Te dwie procedury wykorzystuj¥ instrukcje 16-bitowe(dla 80286) jak i 32-bitowe(dla 80386 i w wy¾ej)}

procedure copybuf;
begin
     asm
        push es
        mov es,lfbs
        xor edi,edi
        mov esi,buf
        mov ecx,scrsiz
        shr ecx,2
        rep movsd
        pop es
     end;
end;

procedure clearbuf;
begin
     asm
        mov edi,buf
        xor eax,eax
        mov ecx,scrsiz
        shr ecx,2
        rep stosd
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
     textura (scrwidth-8,scrheight-8,disk);
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
               putpixel ((lodpoz*35+scrwidth div 2-textures[7].szer div 2+2)+j,178,j mod 2+196);
               putpixel ((lodpoz*35+scrwidth div 2-textures[7].szer div 2+3)+j,179,j mod 2+196);
               putpixel ((lodpoz*35+scrwidth div 2-textures[7].szer div 2+2)+j,180,j mod 2+196);
               putpixel ((lodpoz*35+scrwidth div 2-textures[7].szer div 2+3)+j,181,j mod 2+196);
               putpixel ((lodpoz*35+scrwidth div 2-textures[7].szer div 2+2)+j,182,j mod 2+196);
               putpixel ((lodpoz*35+scrwidth div 2-textures[7].szer div 2+3)+j,183,j mod 2+196);
               {putpixel ((lodpoz*35+3)+j,179,j mod 2+196);
               putpixel ((lodpoz*35+2)+j,180,j mod 2+196);
               putpixel ((lodpoz*35+3)+j,181,j mod 2+196);
               putpixel ((lodpoz*35+2)+j,182,j mod 2+196);
               putpixel ((lodpoz*35+3)+j,183,j mod 2+196);}
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
               putpixel ((lodpoz*35+scrwidth div 2-textures[7].szer div 2+2)+j,178,j mod 2+196);
               putpixel ((lodpoz*35+scrwidth div 2-textures[7].szer div 2+3)+j,179,j mod 2+196);
               putpixel ((lodpoz*35+scrwidth div 2-textures[7].szer div 2+2)+j,180,j mod 2+196);
               putpixel ((lodpoz*35+scrwidth div 2-textures[7].szer div 2+3)+j,181,j mod 2+196);
               putpixel ((lodpoz*35+scrwidth div 2-textures[7].szer div 2+2)+j,182,j mod 2+196);
               putpixel ((lodpoz*35+scrwidth div 2-textures[7].szer div 2+3)+j,183,j mod 2+196);
               {putpixel ((lodpoz*35+2)+j,178,j mod 2+196);
               putpixel ((lodpoz*35+3)+j,179,j mod 2+196);
               putpixel ((lodpoz*35+2)+j,180,j mod 2+196);
               putpixel ((lodpoz*35+3)+j,181,j mod 2+196);
               putpixel ((lodpoz*35+2)+j,182,j mod 2+196);
               putpixel ((lodpoz*35+3)+j,183,j mod 2+196);}
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
     assign (f,'TRON.CFG');
     {$I-}
     reset (f);
     {$I+}
     if ioresult <> 0 then
     begin
          gl := 2000;
          p := 1;
          res := $101;
          exit;
     end;
     read (f,gl);
     read (f,p);
     read (f,res);
     close (f);
end;

procedure init;
var
i : integer;
begin
     loadconfig;
     initvesagrph (res);
     lodpoz := 0;
     loadbmp ('tex10.bmp',disk,true);
     if maxavail < 190000 then error ('ERROR 2: TOO SMALL OF MEMORY');
     initkeyboard;
     getmem (buf,scrsiz);
     clearbuf;
     loadtitles;
     for i := -100 to 10 do
     begin
          clearbuf;
          textura (scrwidth div 2-textures[6].szer div 2,i*2,textures[6]);
          wfvr;
          copybuf;
     end;
     textura (scrwidth div 2-textures[8].szer div 2,190,textures[8]);
     copybuf;
     waittokeypress;
     clearbuf;
     textura (scrwidth div 2-textures[6].szer div 2,20,textures[6]);
     textura (scrwidth div 2-textures[7].szer div 2,171,textures[7]);
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
     freemem (buf,scrsiz);
     freemem (disk.pamob,64);
     closekeyboard;
     closevgagrp;
     textcolor (7);
     writeln ('TRONPM (c) COPYRIGHT Kamil Korolkiewicz 2003/2004');
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
p1,p2 : word;
begin
     if sfx < scrwidth div 2 - 16 then lx := sfx;
     if sfy < scrheight div 2 - 16 then ly := sfy;
     if sfx > 2048-scrwidth div 2 - 16 then lx := sfx-2048+scrwidth;
     if sfy > 2048-scrheight div 2 - 16 then ly := sfy-2048+scrheight;
     dx := (sfx-lx) div 32+1;
     dy := (sfy-ly) div 32+1;
     ex := (word (2015-sfx)+(lx mod 32)) mod 32+1;
     ey := (word (2015-sfy)+(ly mod 32)) mod 32+1;
     p1 := scrheight div 32;
     p2 := scrwidth div 32;
     for j := -1 to p1 do
     for i := -1 to p2 do
     begin
     kx := i+dx;
     ky := j+dy;
     if (maps[actmp]^[ky,kx] < 2) then
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
     assign (f,'SAVEGAME.SAV');
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
     assign (f,'SAVEGAME.SAV');
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
     lx := scrwidth div 2 - 16;
     ly := scrheight div 2 - 16;
     close (f);
     k[67]:=0
end;

begin
     init;
     lx := scrwidth div 2 - 16;
     ly := scrheight div 2 - 16;
     sfx := sx;
     sfy := sy;
     rsfx := sx;
     rsfy := sy;
     w := 0;
     delta := 0;
     ot := gettime;
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
                    lx := scrwidth div 2 - 16;
                    ly := scrheight div 2 - 16;
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
     t := gettime;
     nosound;
     clearbuf;
     copybuf;
     shutdown;
end.
