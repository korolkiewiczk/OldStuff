{Graphed v 0.99 - edytor bitmap 8_32 copyright (c) Kamil Korolkiewicz 2004}
program graphed;

uses crt,strings,scr,text,key,gadd;
const
memm = 295000;
pmenu = 4;
maxsizex = 296;
maxsizey = 152;
names : array [0..pmenu] of pchar = ('PLIK','EDYCJA','PISAKI','OPCJE','POMOC');
namesc : array [0..pmenu] of byte = (7,9,3,4,2);
namesi : array [0..29] of pchar =
       ('NOWY','-','OTWORZ','ZAPISZ','ZAPISZ JAKO...','ZAMKNIJ PLIK','-','ZAMKNIJ PROGRAM',
        'ZAZNACZ','ZAZNACZ WSZYSTKO','-','KOPIUJ','WYTNIJ','WKLEJ','-','ODWROC','-','NOWY KOLOR',
        'PIKSEL','PEDZEL','LINIA','TEKSTURA','KOLOR TLA','-','SKALA...','-','WKLEJANIE SPRITE''OW',
        'POMOC PROGRAMU','-','O PROGRAMIE');
messages : array [0..pmenu] of pchar = ('OTWIERANIE,ZAPISYWANIE,TWORZENIE PLIKOW','EDYCJA BITMAPY',
                                        'WYBOR SPOSOBU RYSOWANIA','WYBOR KOLORU TLA,SKALI,RODZAJU WKLEJANIA','POMOC PROGRAMU');
checkedico : array [0..4*3-1] of byte =
(0,0,0,96,
 96,0,96,0,
 0,96,0,0);

type

pimenu = ^timenu;
timenu = record
               down : pimenu;
               active : boolean;
               checked : boolean;
               text : pchar;
end;
pmainmenu = ^tmainmenu;
tmainmenu = record
                  next : pmainmenu;
                  list : pimenu;
                  active : boolean;
                  text : pchar;
end;

var
buft,bufc,bufc2 : pointer;
tex : ttex;
mainc : byte;
mainmenu : pmainmenu;
act,mact : word;
actm : byte;
actmi : byte;
i : byte;
key_d_p,key_u_p,key_r_p,key_l_p,key_ent_p,key_esc_p,key_sp_p,key_alt_p,key_shf_p,key_ctr_p,key_f1_p,key_tab_p,
key_nl_p,key_nr_p,key_del_p,key_any_p : boolean;
apprun : boolean;
pt : ttex;
li : word;
titlet : ttex;
pow : byte;
msg : pchar;
msgxy : pchar;
ppc,ppc2,ppc3,ppc4 : pchar;
floaded : boolean;
fname : string [12];
actgate : boolean;
scale : word;
actc : byte;
dx,dy : integer;
pis : byte;
pzx,pzy : integer;
pixx,pixy : integer;
poczx,poczy,poczpx,poczpy : integer;
pdx,pdy : word;
lact : byte;
cpx,cpy,cpx2,cpy2,scpx,scpy : integer;
oncopy : boolean;
sprct : boolean;
s1,s2 : word;
modf,saved : boolean;
ptx,pty : word;
rc : byte;

procedure messagebox (x,y : integer; s,s2 : pchar); forward;
procedure title; forward;
procedure help; forward;
procedure colorset; forward;
procedure newfile; forward;
procedure scales; forward;
procedure opensavedialog (t : byte); forward;

procedure win3dnb (x1,y1,x2,y2 : integer; c,a : byte);
var
pc : byte;
begin
     pc := c*32;
     if a = 0 then
     begin
     hline (x1,y1,x2,pc+30);
     hline (x1,y2,x2+1,pc+20);
     vline (x1,y1,y2,pc+30);
     vline (x2,y1,y2,pc+20);
     end
     else
     if a = 1 then
     begin
     hline (x1,y1,x2,pc+20);
     hline (x1,y2,x2+1,pc+30);
     vline (x1,y1,y2,pc+20);
     vline (x2,y1,y2,pc+30);
     end
     else
     begin
     hline (x1,y1,x2,pc+24);
     hline (x1,y2,x2+1,pc+24);
     vline (x1,y1,y2,pc+24);
     vline (x2,y1,y2,pc+24);
     end;
end;

procedure win3db (x1,y1,x2,y2 : integer; c,a : byte);
var
pc : byte;
begin
     pc := c*32;
     scrwin (x1,y1,x2,y2,pc+24);
     if a = 0 then
     begin
     hline (x1,y1,x2,pc+30);
     hline (x1,y2,x2+1,pc+20);
     vline (x1,y1,y2,pc+30);
     vline (x2,y1,y2,pc+20);
     end
     else
     if a = 1 then
     begin
     hline (x1,y1,x2,pc+20);
     hline (x1,y2,x2+1,pc+30);
     vline (x1,y1,y2,pc+20);
     vline (x2,y1,y2,pc+30);
     end
     else
     begin
     hline (x1,y1,x2,pc+24);
     hline (x1,y2,x2+1,pc+24);
     vline (x1,y1,y2,pc+24);
     vline (x2,y1,y2,pc+24);
     end;
end;

procedure win3df (x1,y1,x2,y2 : integer; c,a,f : byte);
var
pc : byte;
begin
     pc := c*32;
     scrwin (x1,y1,x2,y2,pc+f);
     if a = 0 then
     begin
     hline (x1,y1,x2,pc+30);
     hline (x1,y2,x2+1,pc+20);
     vline (x1,y1,y2,pc+30);
     vline (x2,y1,y2,pc+20);
     end
     else
     if a = 1 then
     begin
     hline (x1,y1,x2,pc+20);
     hline (x1,y2,x2+1,pc+30);
     vline (x1,y1,y2,pc+20);
     vline (x2,y1,y2,pc+30);
     end
     else
     begin
     hline (x1,y1,x2,pc+24);
     hline (x1,y2,x2+1,pc+24);
     vline (x1,y1,y2,pc+24);
     vline (x2,y1,y2,pc+24);
     end;
end;

procedure hline3d (x1,y1,x2 : integer; c : byte);
begin
     hline (x1,y1,x2,c*32+20);
     hline (x1,y1+1,x2,c*32+30);
end;

procedure putptex;
var
i,j,ex,ey : word;
c,c2 : integer;
begin
     c2 := (actc shr 5) shl 5;
     ex := ptx-1;
     ey := pty-1;
     if ex+dx > tex.sizex-1 then ex := tex.sizex-dx-1;
     if ey+dy > tex.sizey-1 then ey := tex.sizey-dy-1;
     for j := 0 to ey do
         for i := 0 to ex do
         begin
              c := actc-random (rc);
              if c < c2 then c := c2;
              mem [seg (buft^):(j+dy)*tex.sizex+(i+dx)] := c;
         end;
end;

procedure settexpix (c : byte);
begin
     if pis = 1 then
     mem [seg (buft^):dy*tex.sizex+dx] := c;
     if pis = 2 then
     begin
          mem [seg (buft^):dy*tex.sizex+dx] := c;
          mem [seg (buft^):dy*tex.sizex+dx+1] := c;
          mem [seg (buft^):(dy+1)*tex.sizex+dx] := c;
          mem [seg (buft^):(dy+1)*tex.sizex+dx+1] := c;
     end;
     if pis = 4 then putptex;
end;

procedure cpdr;
var
a,b : integer;
begin
     if oncopy then
     begin
          a := (scpx-pzx)*scale;
          if a < 0 then a := 0;
          if a > maxsizex then a := maxsizex;
          b := (scpy-pzy)*scale;
          if b < 0 then b := 0;
          if b > maxsizey then b := maxsizey;
          scrtranspwin (8+a,16+b,pixx+8+scale,pixy+16+scale,mainc*32+31);
     end;
end;

function gettcol : byte;
begin
     gettcol := mem [seg (buft^):dy*tex.sizex+dx];
end;

procedure drawpix;
var
a,b : byte;
begin
     if (pis = 1) or (pis = 4) then
     begin
          for a := 0 to scale-1 do
          for b := 0 to scale-1 do
          putpixel (pixx+8+b,pixy+16+a,actc);
          cpdr;
     end;
     if pis = 2 then
     begin
          for a := 0 to scale*2-1 do
          for b := 0 to scale*2-1 do
          putpixel (pixx+8+b,pixy+16+a,actc);
          cpdr;
     end;
     if pis = 3 then
     begin
          if (lact = 1) or (lact = 2) then
          begin
               for a := 0 to scale-1 do
               for b := 0 to scale-1 do
               begin
                    if (poczpx+8+b < maxsizex) and (poczpy+16+a < maxsizey) then
                    putpixel (poczpx+8+b,poczpy+16+a,actc);
               end;
               line (poczpx+8,poczpy+16,pixx+8,pixy+16,actc);
               for a := 0 to scale-1 do
               for b := 0 to scale-1 do
               putpixel (pixx+8+b,pixy+16+a,actc);
          end
          else
          for a := 0 to scale-1 do
          for b := 0 to scale-1 do
          putpixel (pixx+8+b,pixy+16+a,actc);
          cpdr;
     end;
end;

procedure drwtex;
var
pt : ttex;
begin
     pt := tex;
     if (pzx > 0) or (pzy > 0) then
     pt.texmemory := ptr (seg (pt.texmemory^),pzy*tex.sizex+pzx);
     texnfs (8,16,pt,scale);
end;

procedure startcopy;
begin
     scpx := dx;
     scpy := dy;
     cpx := 0;
     cpy := 0;
     oncopy := true;
end;

procedure stopcopy;
begin
     cpx := dx-scpx;
     cpy := dy-scpy;
end;

procedure cpm (t : byte);
var
i,j : word;
pcpy,pcpx : word;
begin
     if ((t = 0) or (t = 1)) and (oncopy) then
     begin
          if cpy < 0 then pcpy := scpy+cpy
          else
          pcpy := scpy;
          if cpx < 0 then pcpx := scpx+cpx
          else
          pcpx := scpx;
          for i := 0 to abs (cpy) do
          move (mem [seg(buft^):(pcpy+i)*tex.sizex+pcpx],mem [seg (bufc^):i*(abs (cpx)+1)],abs (cpx)+1);
          if t = 1 then
          for i := 0 to abs (cpy) do
          fillchar (mem [seg(buft^):(pcpy+i)*tex.sizex+pcpx],abs (cpx)+1,0);
          cpx2 := abs (cpx);
          cpy2 := abs (cpy);
     end;
     if t = 2 then
     begin
          if not modf then if floaded then modf := true;
          pcpy := cpy2;
          pcpx := cpx2;
          if pcpx+dx >= tex.sizex then pcpx := tex.sizex-dx-1;
          if pcpy+dy >= tex.sizey then pcpy := tex.sizey-dy-1;
          if sprct then
          begin
          for i := 0 to pcpy do
          for j := 0 to pcpx do
          if mem [seg (bufc^):i*(cpx2+1)+j] <> 0 then mem [seg(buft^):(dy+i)*tex.sizex+dx+j] := mem [seg (bufc^):i*(cpx2+1)+j];
          end
          else
          for i := 0 to pcpy do
          move (mem [seg (bufc^):i*(cpx2+1)],mem [seg(buft^):(dy+i)*tex.sizex+dx],pcpx+1);
     end;
     if t = 3 then
     begin
          if cpy < 0 then pcpy := scpy+cpy
          else
          pcpy := scpy;
          if cpx < 0 then pcpx := scpx+cpx
          else
          pcpx := scpx;
          for i := 0 to abs (cpy) do
          fillchar (mem [seg(buft^):(pcpy+i)*tex.sizex+pcpx],abs (cpx)+1,0);
     end;
end;

procedure createmenu;
var
p,p2,pp : pmainmenu;
l,l2,l3 : pimenu;
i,j,k : byte;
begin
     pt.texmemory := addr (checkedico);
     pt.sizex := 4;
     pt.sizey := 3;
     getmem (ppc,3);
     getmem (ppc2,3);
     getmem (ppc3,3);
     getmem (ppc4,13);
     new (mainmenu);
     new (p);
     new (l);
     pp := p;
     k := 0;
     for i := 0 to pmenu do
     begin
          new (p2);
          new (l2);
          p^.text := names [i];
          p^.next := p2;
          p^.list := l;
          p^.active := true;
          for j := 0 to namesc [i] do
          begin
               new (l3);
               l^.text := namesi [k];
               l^.down := l3;
               l^.active := true;
               l^.checked := false;
               l := l3;
               inc (k);
          end;
          l^.down := nil;
          p := p2;
          l := l2;
     end;
     p^.next := nil;
     mainmenu := pp;
end;

procedure setmenuit (namm,inamm : pchar; a,c : boolean);
var
pm : pmainmenu;
pmi : pimenu;
d : integer;
begin
     pm := mainmenu;
     while pm <> nil do
     begin
          d := strcomp (namm,pm^.text);
          if d = 0 then
          begin
               pmi := pm^.list;
               while pmi <> nil do
               begin
                    d := strcomp (inamm,pmi^.text);
                    if d=0 then
                    begin
                         pmi^.active := a;
                         pmi^.checked := c;
                         exit;
                    end;
                    pmi := pmi^.down;
               end;
          end;
          pm := pm^.next;
     end;
end;

procedure setnomenui;
begin
     setmenuit ('PLIK','ZAPISZ',false,false);
     setmenuit ('PLIK','ZAPISZ JAKO...',false,false);
     setmenuit ('PLIK','ZAMKNIJ PLIK',false,false);
     setmenuit ('EDYCJA','ZAZNACZ',false,false);
     setmenuit ('EDYCJA','ZAZNACZ WSZYSTKO',false,false);
     setmenuit ('EDYCJA','KOPIUJ',false,false);
     setmenuit ('EDYCJA','WYTNIJ',false,false);
     setmenuit ('EDYCJA','WKLEJ',false,false);
     setmenuit ('EDYCJA','ODWROC',false,false);
     setmenuit ('EDYCJA','NOWY KOLOR',false,false);
     setmenuit ('OPCJE','SKALA...',false,false);
end;

procedure setyesmenui;
begin
     setmenuit ('PLIK','ZAPISZ',true,false);
     setmenuit ('PLIK','ZAPISZ JAKO...',true,false);
     setmenuit ('PLIK','ZAMKNIJ PLIK',true,false);
     setmenuit ('EDYCJA','ZAZNACZ',true,false);
     setmenuit ('EDYCJA','ZAZNACZ WSZYSTKO',true,false);
     setmenuit ('EDYCJA','KOPIUJ',true,false);
     setmenuit ('EDYCJA','WYTNIJ',true,false);
     setmenuit ('EDYCJA','WKLEJ',true,false);
     setmenuit ('EDYCJA','ODWROC',true,false);
     setmenuit ('EDYCJA','NOWY KOLOR',true,false);
     setmenuit ('OPCJE','SKALA...',true,false);
end;

procedure makemsg (x,y : word);
var
ps : string;
i : byte;
begin
     ps := '';
     fillchar (ppc^,3,0);
     fillchar (ppc2^,3,0);
     if pis = 4 then
     begin
          if modf then
          msgxy := 'X/Y=   C=  M'
          else
          msgxy := 'X/Y=   C=   ';
          str (ptx,ps);
          strpcopy (ppc,ps);
          for i := 4 to 6 do if ppc [i-4] <> #0 then msgxy [i] := ppc [i-4] else msgxy[i] := ' ';
          str (rc,ps);
          strpcopy (ppc2,ps);
          for i := 10 to 11 do if ppc2 [i-10] <> #0 then msgxy [i] := ppc2 [i-10] else msgxy[i] := ' ';
     end
     else
     begin
          if modf then
          msgxy := 'X=    Y=   M'
          else
          msgxy := 'X=    Y=    ';
          str (x,ps);
          strpcopy (ppc,ps);
          for i := 2 to 4 do if ppc [i-2] <> #0 then msgxy [i] := ppc [i-2] else msgxy[i] := ' ';
          str (y,ps);
          strpcopy (ppc2,ps);
          for i := 8 to 10 do if ppc2 [i-8] <> #0 then msgxy [i] := ppc2 [i-8] else msgxy[i] := ' ';
     end;
end;

procedure createtex (sx,sy : word);
begin
     tex.sizex := sx;
     tex.sizey := sy;
     tex.texmemory := buft;
     floaded := true;
     fillchar (buft^,65535,0);
     dx := 0;
     dy := 0;
     pzx := 0;
     pzy := 0;
     pixx := dx;
     pixy := dy;
     oncopy := false;
     rc := 5;
     ptx := 5;
     pty := 5;
     lact := 0;
     makemsg (0,0);
     setyesmenui;
     act := 0;
     saved := false;
     modf := false;
end;

procedure disposetex;
begin
     tex.sizex := 0;
     tex.sizey := 0;
     floaded := false;
     actc := 0;
     setnomenui;
end;

function loadf (s : string) : byte;
var
f : file;
begin
     loadf := 1;
     assign (f,s);
     {$I-}
     reset (f,1);
     {$I+}
     if ioresult <> 0 then exit;
     blockread (f,s1,4);
     if (filesize (f) < longint (s1)*longint (s2)+4) or (filesize (f) > longint (s1)*longint (s2)+4) then
     begin
          close (f);
          loadf := 2;
          exit;
     end;
     createtex (s1,s2);
     blockread (f,buft^,s1*s2);
     close (f);
     loadf := 0;
     saved := true;
end;

procedure savef (s : string);
var
f : file;
begin
     assign (f,s);
     rewrite (f,1);
     blockwrite (f,tex.sizex,2);
     blockwrite (f,tex.sizey,2);
     blockwrite (f,buft^,tex.sizex*tex.sizey);
     saved := true;
     modf := false;
     close (f);
end;

procedure makenewc;
var
j,i : word;
c1,c2,c3 : byte;
begin
     c2 := actc shr 5;
     c2 := c2 shl 5;
     for j := 0 to tex.sizey-1 do
         for i := 0 to tex.sizex-1 do
         begin
              c3 := mem [seg (buft^):j*tex.sizex+i];
              c1 := c3 shr 5;
              c1 := c1 shl 5;
              c3 := c3-c1;
              c3 := c3+c2;
              mem [seg (buft^):j*tex.sizex+i] := c3;
         end;
end;

procedure setpis (ps : byte);
begin
     if ps = 1 then
     begin
          setmenuit ('PISAKI','PIKSEL',true,true);
          setmenuit ('PISAKI','PEDZEL',true,false);
          setmenuit ('PISAKI','LINIA',true,false);
          setmenuit ('PISAKI','TEKSTURA',true,false);
          pis := 1;
     end;
     if ps = 2 then
     begin
          setmenuit ('PISAKI','PIKSEL',true,false);
          setmenuit ('PISAKI','PEDZEL',true,true);
          setmenuit ('PISAKI','LINIA',true,false);
          setmenuit ('PISAKI','TEKSTURA',true,false);
          pis := 2;
     end;
     if ps = 3 then
     begin
          setmenuit ('PISAKI','PIKSEL',true,false);
          setmenuit ('PISAKI','PEDZEL',true,false);
          setmenuit ('PISAKI','LINIA',true,true);
          setmenuit ('PISAKI','TEKSTURA',true,false);
          pis := 3;
     end;
     if ps = 4 then
     begin
          setmenuit ('PISAKI','PIKSEL',true,false);
          setmenuit ('PISAKI','PEDZEL',true,false);
          setmenuit ('PISAKI','LINIA',true,false);
          setmenuit ('PISAKI','TEKSTURA',true,true);
          pis := 4;
     end;
end;

procedure initialize;
var
s : longint;
begin
     clrscr;
     textbackground (1);
     textcolor (0);
     writeln ('       Graphed - 8_32 bitmap editor copyright (c) Kamil Korolkiewicz 2004       ');
     textbackground (0);
     textcolor (7);
     writeln ('Geting memory...');
     s := memavail;
     if s < memm then
     begin
          writeln ('Error: Memory = ',s,'.Requied = ',memm);
          halt (1);
     end;
     getmem (buf,64000);
     getmem (buft,65535);
     getmem (bufc,65535);
     getmem (bufc2,65535);
     getmem (txttbl,11200);
     getmem (titlet.texmemory,190*75);
     createmenu;
     titlet.sizex := 190;
     titlet.sizey := 75;
     writeln ('Geting params...');
     if paramcount > 0 then
     begin
          if loadf (paramstr (1)) = 0 then fname := paramstr (1);
          setyesmenui;
     end
     else
     begin
          disposetex;
          fillchar (buft^,65535,0);
          setnomenui;
     end;
     writeln ('Using keyboard...');
     keyis (on);
     writeln ('Freeing buffers...');
     buftoclr;
     unpack;
     writeln ('Setting 8-32 palette...');
     set_8_32_pal;
     initvgascr;
     setpalette;
     textspr;
     textsetcolor (96);
     randomize;
     mainc := 3;
     act := 0;
     actm := 0;
     apprun := true;
     mact := 0;
     li := 0;
     pow := 5;
     scale := 1;
     pis := 1;
     pzx := 0;
     pzy := 0;
     minx := 7;
     miny := 15;
     maxx := 304;
     maxy := 168;
     sprct := false;
     setmenuit ('PISAKI','PIKSEL',true,true);
end;

procedure shutdown;
begin
     inittxtscr;
     keyis (off);
     dispose (mainmenu);
     freemem (ppc4,13);
     freemem (ppc3,3);
     freemem (ppc2,3);
     freemem (ppc,3);
     {freemem (txttbl,11200);}
     freemem (titlet.texmemory,190*75);
     freemem (bufc2,65535);
     freemem (bufc,65535);
     freemem (buft,65535);
     freemem (buf,64000);
end;

procedure copyo;
var
i,j : word;
pcpy,pcpx : word;
begin
     if oncopy then
     begin
     if cpy < 0 then pcpy := scpy+cpy
     else
     pcpy := scpy;
     if cpx < 0 then pcpx := scpx+cpx
     else
     pcpx := scpx;
     for j := 0 to abs (cpy) do
         for i := 0 to abs (cpx) do
             mem [seg (bufc2^):j*(abs (cpx)+1)+i] := mem [seg (buft^):(j+pcpy)*tex.sizex+(abs (cpx)-i+pcpx)];
     for j := 0 to abs (cpy) do
     move (mem [seg (bufc2^):(abs (cpx)+1)*j],mem [seg (buft^):(j+pcpy)*tex.sizex+pcpx],abs (cpx)+1);
     end
     else
     begin
     for j := 0 to tex.sizey-1 do
         for i := 0 to tex.sizex-1 do
             mem [seg (bufc2^):j*tex.sizex+i] := mem [seg (buft^):j*tex.sizex+(tex.sizex-1-i)];
     move (bufc2^,buft^,tex.sizex*tex.sizey);
     end;
end;

procedure entrprc;
begin
     if actm = 0 then if actmi = 7 then apprun := false;
     if actm = 0 then if actmi = 0 then
     begin
          if modf then
          begin
               messagebox (14,30,'PLIK ZOSTAL ZMODYFIKOWANY.MOZESZ GO ZAPISAC.','NOWY PLIK');
               opensavedialog (1);
          end;
          newfile;
     end;
     if actm = 0 then if actmi = 2 then
     begin
          if modf then
          begin
               messagebox (14,30,'PLIK ZOSTAL ZMODYFIKOWANY.MOZESZ GO ZAPISAC.','OTWORZ PLIK');
               opensavedialog (1);
          end;
          opensavedialog (0);
     end;
     if actm = 0 then if actmi = 3 then
     begin
          if saved then savef (fname)
          else
          opensavedialog (1);
     end;
     if actm = 0 then if actmi = 4 then opensavedialog (1);
     if actm = 0 then if actmi = 5 then
     begin
          if modf then
          begin
               messagebox (14,30,'PLIK ZOSTAL ZMODYFIKOWANY.MOZESZ GO ZAPISAC.','ZAMKNIJ PLIK');
               opensavedialog (1);
          end;
          disposetex;
     end;
     if actm = 1 then if actmi = 0 then startcopy;
     if actm = 1 then if actmi = 1 then
     begin
          oncopy := true;
          scpx := 0;
          scpy := 0;
          cpx := tex.sizex-1;
          cpy := tex.sizey-1;
          dx := cpx;
          dy := cpy;
          pixx := dx*scale;
          pixy := dy*scale;
     end;
     if actm = 1 then if actmi = 3 then cpm (0);
     if actm = 1 then if actmi = 4 then cpm (1);
     if actm = 1 then if actmi = 5 then cpm (2);
     if actm = 1 then if actmi = 7 then copyo;
     if actm = 1 then if actmi = 9 then makenewc;
     if actm = 4 then if actmi = 0 then help;
     if actm = 4 then if actmi = 2 then title;
     if actm = 3 then if actmi = 0 then colorset;
     if actm = 3 then if actmi = 2 then scales;
     if actm = 3 then if actmi = 4 then
     begin
          if sprct = false then begin sprct := true; setmenuit ('OPCJE','WKLEJANIE SPRITE''OW',true,true) end
          else begin sprct := false; setmenuit ('OPCJE','WKLEJANIE SPRITE''OW',true,false) end;
     end;
     if actm = 2 then
     begin
          if actmi = 0 then
          setpis (1)
          else
          if actmi = 1 then
          setpis (2)
          else
          if actmi = 2 then
          setpis (3)
          else
          if actmi = 3 then
          setpis (4);
     end;
end;

procedure getmessages;
var
i : byte;
begin
     if li mod pow = 0 then
     begin
     if keyon [1] = 1 then key_esc_p := true else key_esc_p := false;
     if keyon [28] = 1 then key_ent_p := true else key_ent_p := false;
     if keyon [57] = 1 then key_sp_p := true else key_sp_p := false;
     if keyon [80] = 1 then key_d_p := true else key_d_p := false;
     if keyon [77] = 1 then key_r_p := true else key_r_p := false;
     if keyon [72] = 1 then key_u_p := true else key_u_p := false;
     if keyon [75] = 1 then key_l_p := true else key_l_p := false;
     if keyon [29] = 1 then key_ctr_p := true else key_ctr_p := false;
     if keyon [56] = 1 then key_alt_p := true else key_alt_p := false;
     if keyon [42] = 1 then key_shf_p := true else key_shf_p := false;
     if keyon [59] = 1 then key_f1_p := true else key_f1_p := false;
     if keyon [15] = 1 then key_tab_p := true else key_tab_p := false;
     if keyon [26] = 1 then key_nl_p := true else key_nl_p := false;
     if keyon [27] = 1 then key_nr_p := true else key_nr_p := false;
     if keyon [83] = 1 then key_del_p := true else key_del_p := false;
     for i := 0 to 127 do if keyon[i] = 1 then
     begin
          key_any_p := true;
          break;
     end;
     end
     else
     begin
          key_esc_p := false;
          key_ent_p := false;
          key_sp_p := false;
          key_d_p := false;
          key_r_p := false;
          key_u_p := false;
          key_l_p := false;
          key_ctr_p := false;
          key_alt_p := false;
          key_shf_p := false;
          key_f1_p := false;
          key_tab_p := false;
          key_nl_p := false;
          key_nr_p := false;
          key_del_p := false;
          key_any_p := false;
     end;
     inc (li);
end;

procedure renderall;
var
p,p2 : pmainmenu;
p3 : pimenu;
k : byte;
i,j,sx,l,m,n : word;
t : pchar;
pdl,pdl2,dl1,dl2,epdl1,epdl2 : word;
begin
     scrwin (0,0,320,200,mainc*32+24);
     hline3d (0,9,320,mainc);
     win3dnb (0,191,240,199,mainc,1);
     win3dnb (242,191,319,199,mainc,1);
     textprint (2,193,0,1,msg);
     win3df (7,15,304,168,mainc,1,10);
     if floaded then
     begin
          dl1 := tex.sizex*scale;
          dl2 := tex.sizey*scale;
          if dl1 > maxsizex then
          begin
               win3df (7,168,304,176,mainc,1,21);
               pdl := ((maxsizex shl 7) div dl1)*maxsizex;
               pdl2 := ((pzx*scale shl 7) div (tex.sizex*scale-maxsizex))*(maxsizex-(pdl shr 7));
               epdl1 := pdl shr 7+8+pdl2 shr 7;
               if epdl1 > 303 then epdl1 := 303;
               win3db (8+pdl2 shr 7,168,epdl1,175,mainc,0);
          end;
          if dl2 > maxsizey then
          begin
               win3df (304,15,315,168,mainc,1,21);
               pdl := ((maxsizey shl 8) div dl2)*maxsizey;
               pdl2 := ((pzy*scale shl 8) div (tex.sizey*scale-maxsizey))*(maxsizey-(pdl shr 8));
               epdl2 := pdl shr 8+15+pdl2 shr 8;
               if epdl2 > 167 then epdl2 := 167;
               win3db (305,16+pdl2 shr 8,314,epdl2,mainc,0)
          end;
          if oncopy then
          makemsg (abs (cpx),abs (cpy))
          else
          makemsg (dx,dy);
          textprint (244,193,0,1,msgxy);
          drwtex;
          drawpix;
     end
     else
     modf := false;
     win3dnb (27,177,284,187,mainc,1);
     win3dnb (11,177,24,187,mainc,1);
     scrwin (12,178,24,187,actc);
     for i := 0 to 255 do vline (i+28,178,187,i);
     putpixel (actc+28,188,10);
     putpixel (actc+27,189,10);
     putpixel (actc+28,189,10);
     putpixel (actc+29,189,10);
     putpixel (actc+26,190,10);
     putpixel (actc+27,190,10);
     putpixel (actc+28,190,10);
     putpixel (actc+29,190,10);
     putpixel (actc+30,190,10);
     if mact <> 1 then
     begin
     if key_esc_p then
     begin
          if act = 0 then
          begin
               if oncopy then oncopy := false
               else
               lact := 0;
          end;
          if act = 1 then act := 0;
          if act = 2 then act := 1;
     end;
     if key_ent_p then
     begin
          if act = 0 then
          begin
               if not modf then if floaded then modf := true;
               settexpix (actc);
               if pis = 3 then
               begin
               if lact = 0 then
               begin
                    poczx := dx;
                    poczy := dy;
                    poczpx := pixx;
                    poczpy := pixy;
                    lact := 1;
               end;
               if lact = 1 then
               begin
                    bline (buft,poczx,poczy,dx,dy,tex.sizex,actc);
                    lact := 2;
               end;
               if lact = 2 then
               begin
                    poczx := dx;
                    poczy := dy;
                    pdx := dx;
                    pdy := dy;
                    poczpx := pixx;
                    poczpy := pixy;
                    lact := 1;
               end;
               end;
          end;
          if act = 1 then
          begin
               act := 2;
               key_ent_p := false;
          end
          else
          if act = 2 then if actgate then entrprc;
     end;
     if key_d_p then
     begin
     if act = 0 then
     begin
          if floaded then
          if mact = 0 then
          begin
               inc (dy);
               if (pis = 1) or (pis = 3) or (pis = 4) then
               begin
                    if dy > tex.sizey-1 then dy := tex.sizey-1
               end
               else
               if pis = 2 then if dy > tex.sizey-2 then dy := tex.sizey-2;
               pixy := (dy-pzy)*scale;
               if pixy < 0 then
               begin
                    pixy := 0;
                    pzy := dy;
                    poczpy := (pdy*scale)-(pzy*scale);
               end;
          end;
     end;
     if act = 1 then act := 2
     else
     if act = 2 then
     begin
          if actmi < namesc [actm] then inc (actmi)
          else
          actmi := 0;
     end;
     end;
     if key_r_p then
     begin
     if act = 0 then
     begin
          if mact = 2 then if key_ctr_p then inc (actc,16) else inc (actc);
          if floaded then
          if mact = 0 then
          begin
               inc (dx);
               if (pis = 1) or (pis = 3) or (pis = 4) then
               begin
                    if dx > tex.sizex-1 then dx := tex.sizex-1;
               end
               else
               if pis = 2 then if dx > tex.sizex-2 then dx := tex.sizex-2;
               pixx := (dx-pzx)*scale;
               if pixx < 0 then
               begin
                    pixx := 0;
                    pzx := dx;
                    poczpx := (pdx*scale)-(pzx*scale);
               end;
          end;
     end;
     if (act = 1) or (act = 2) then
     begin
          if actm < pmenu then inc (actm)
          else actm := 0;
          actmi := 0;
     end;
     end;
     if key_l_p then
     begin
     if act = 0 then
     begin
          if mact = 2 then if key_ctr_p then dec (actc,16) else dec (actc);
          if floaded then
          if mact = 0 then
          begin
               dec (dx);
               if dx < 0 then dx := 0;
               pixx := (dx-pzx)*scale;
               if pixx < 0 then
               begin
                    pixx := 0;
                    pzx := dx;
                    poczpx := (pdx*scale)-(pzx*scale);
               end;
          end;
     end;
     if (act = 1) or (act = 2) then
     begin
          if actm > 0 then dec (actm)
          else actm := pmenu;
          actmi := 0;
     end;
     end;
     if key_u_p then
     begin
     if act = 0 then
     begin
          if floaded then
          if mact = 0 then
          begin
               dec (dy);
               if dy < 0 then dy := 0;
               pixy := (dy-pzy)*scale;
               if pixy < 0 then
               begin
                    pixy := 0;
                    pzy := dy;
                    poczpy := (pdy*scale)-(pzy*scale);
               end;
          end;
     end;
     if act = 1 then act := 2
     else
     if act = 2 then
     begin
          if actmi > 0 then dec (actmi)
          else
          actmi := namesc [actm];
     end;
     end;
     if key_alt_p then
     begin
          if (act = 1) or (act = 2) then act := 0
          else
          act := 1;
     end;
     if (pis = 1) or (pis = 3) or (pis = 4) then
     begin
          if pixy >= maxsizey-scale then
          begin
               pixy := maxsizey-scale;
               pzy := dy-(pixy div scale);
               poczpy := (pdy*scale)-(pzy*scale);
          end;
          if pixx >= maxsizex-scale then
          begin
               pixx := maxsizex-scale;
               pzx := dx-(pixx div scale);
               poczpx := (pdx*scale)-(pzx*scale);
          end;
     end;
     if pis = 2 then
     begin
          if pixy >= maxsizey-2*scale then
          begin
               pixy := maxsizey-2*scale;
               pzy := dy-(pixy div scale);
          end;
          if pixy > tex.sizey*scale-2*scale then pixy := tex.sizey*scale-2*scale;
          if pixx >= maxsizex-2*scale then
          begin
               pixx := maxsizex-2*scale;
               pzx := dx-(pixx div scale);
          end;
          if pixx > tex.sizex*scale-2*scale then pixx := tex.sizex*scale-2*scale;
     end;
     if oncopy then stopcopy;
     if key_f1_p then help;
     if key_tab_p then if mact = 0 then mact := 2 else mact := 0;
     if key_sp_p then
     begin
          if act = 0 then
          begin
          if not modf then if floaded then modf := true;
          settexpix (actc);
          if pis = 3 then
          begin
               if lact = 0 then
               begin
                    poczx := dx;
                    poczy := dy;
                    poczpx := pixx;
                    poczpy := pixy;
                    lact := 1;
               end;
               if lact = 1 then
               begin
                    bline (buft,poczx,poczy,dx,dy,tex.sizex,actc);
                    lact := 2;
               end;
               if lact = 2 then
               begin
                    poczx := dx;
                    poczy := dy;
                    pdx := dx;
                    pdy := dy;
                    poczpx := pixx;
                    poczpy := pixy;
                    lact := 1;
               end;
          end;
          end;
     end;
     if key_nl_p then dec (actc);
     if key_nr_p then inc (actc);
     if key_ctr_p then if act = 0 then li := pow-1;
     if key_del_p then if act = 0 then
     begin
          if oncopy then cpm (3)
          else
          settexpix (0);
     end;
     if key_any_p then
     begin
          for i := 2 to 9 do if keyon[i] = 1 then
          begin
               if act = 0 then
               actc := (i-1)*32-1
               else
               actm := (i-2) mod (pmenu+1);
          end;
          if keyon[10] = 1 then actc := 96;
          if keyon[11] = 1 then actc := 0;
          if keyon[14] = 1 then actc := gettcol;
          if keyon[44] = 1 then startcopy; {z}
          if keyon[45] = 1 then cpm (1); {x}
          if keyon[46] = 1 then cpm (0); {c}
          if keyon[47] = 1 then cpm (2); {v}
          if keyon[30] = 1 then setpis (1); {a}
          if keyon[31] = 1 then setpis (2); {s}
          if keyon[32] = 1 then setpis (3); {d}
          if keyon[33] = 1 then setpis (4); {f}
          if keyon[16] = 1 then if rc > 0 then dec (rc); {q}
          if keyon[17] = 1 then if rc < 31 then inc (rc); {w}
          if keyon[18] = 1 then {e}
          begin
               if ptx > 1 then dec (ptx);
               if pty > 1 then dec (pty);
          end;
          if keyon[19] = 1 then {r}
          begin
               if ptx < 999 then inc (ptx);
               if pty < 999 then inc (pty);
          end;
          if keyon[24] = 1 then copyo; {o}
     end;
     end;
     p := mainmenu;
     i := 2;
     k := 0;
     while p^.next <> nil do
     begin
          if p^.active = true then textsetcolor (96)
          else
          textsetcolor (116);
          textprint (i,2,0,1,p^.text);
          l := strlen (p^.text)*6+3;
          if actm = k then
          begin
               j := i;
               sx := i+l;
               p2 := p;
               if actmi > namesc [k] then actmi := actmi mod (namesc [k]+1);
               msg := messages [actm];
          end;
          inc (k);
          inc (i,l);
          p := p^.next;
     end;
     if act = 0 then
     begin
          if mact = 0 then
          begin
               if floaded then
               msg := 'TRYB EDYCJI | F1-POMOC | ALT-MENU'
               else
               msg := 'F1-POMOC | ALT-MENU';
          end
          else
          if mact = 1 then msg := 'OKNO DIALOGOWE'
          else
          if mact = 2 then msg := 'WYBOR KOLORU | F1-POMOC | ALT-MENU';
     end;
     if act = 1 then win3dnb (j-2,0,sx-2,8,mainc,0)
     else
     if act = 2 then
     begin
          win3dnb (j-2,0,sx-2,8,mainc,1);
          p3 := p2^.list;
          m := 0;
          n := 0;
          while p3^.down <> nil do
          begin
               l := strlen (p3^.text)*6+15;
               if l > m then m := l;
               p3 := p3^.down;
               inc (n);
          end;
          win3db (j-2,9,m+j,n*8+12,mainc,0);
          p3 := p2^.list;
          n := 12;
          i := 0;
          while p3^.down <> nil do
          begin
               if p3^.active = true then textsetcolor (96)
               else
               textsetcolor (116);
               t := p3^.text;
               if t[0] <> '-' then
               begin
                    if actmi = i then
                    begin
                         scrwin (j+9,n-1,j+m-2,n+7,83);
                         if p3^.active = true then
                         begin
                              textsetcolor (127);
                              actgate := true;
                         end
                         else
                         begin
                              textsetcolor (108);
                              actgate := false;
                         end;
                    end;
                    textprint (j+10,n,0,1,t);
                    if p3^.checked then scrsprtexnf (j+2,n+2,pt);
               end
               else
               begin
                    hline3d (j+1,n+2,j+m-2,mainc);
                    if actmi = i then
                    begin
                         if key_d_p then inc (actmi);
                         if key_u_p then
                         begin
                              dec (actmi);
                              p3 := p2^.list;
                              n := 12;
                              i := 0;
                              continue;
                         end;
                    end;
               end;
               inc (n,8);
               p3 := p3^.down;
               inc (i);
     end;
     end;
     textsetcolor (96);
end;

procedure title;
var
s : word;
i : word;
kier,pcolor : shortint;
om : byte;
begin
     act := 0;
     om := mact;
     mact := 1;
     i := 0;
     pow := 1;
     kier := 1;
     pcolor := 1;
     while 1=1 do
     begin
          getmessages;
          renderall;
          win3db (40,30,280,39,2,0);
          textprint (42,32,0,1,'O PROGRAMIE');
          win3db (40,40,280,160,mainc,0);
          textsetcolor (18);
          textprint (120,50,0,0,'GRAPHED');
          textprint (197,55,0,1,'v.0.99');
          if i mod 50 = 0 then
          for s := 0 to 190*75 do mem [seg (titlet.texmemory^):s] := random (4)+2*32+26;
          scrtexnf (65,65,titlet);
          textprint (67,70,1,0,'Program do tworzenia'+#13+'grafiki typu 8 kolo-'+#13+'r¢w przy 32 odcie-'+#13+'niach.');
          textsetcolor (pcolor+2*32);
          textprint (50,151,0,1,'COPYRIGHT (C) KAMIL KOROLKIEWICZ 2004');
          textsetcolor (96);
          wfvr;
          buftoscr;
          if i > 20 then
          begin
          if key_ent_p then break;
          if key_esc_p then
          begin
               key_esc_p := false;
               break;
          end;
          end;
          inc (pcolor,kier);
          if pcolor = 31 then kier := -1;
          if pcolor = 1 then kier := 1;
          inc (i);
     end;
     mact := om;
     pow := 5;
     key_ent_p := false;
     key_esc_p := false;
     delay (50);
end;

procedure help;
var
i : word;
om : byte;
begin
     act := 0;
     om := mact;
     mact := 1;
     pow := 1;
     i := 0;
     while 1=1 do
     begin
          getmessages;
          renderall;
          win3db (60,50,260,59,2,0);
          win3db (60,60,260,147,mainc,0);
          textprint (62,52,0,1,'POMOC');
          textprint (70,65,0,0,'F1-Pomoc'+#13+'ALT-Menu'+#13+'Kusory-poruszanie si©'+#13+'po menu i polu edycji'+
                                #13+'Spacja/Enter-Postaw'+#13+'punkt'+#13+'ESC-Anuluj');
          wfvr;
          buftoscr;
          if i > 20 then
          begin
          if key_ent_p then break;
          if key_esc_p then break;
          end;
          inc (i);
     end;
     mact := om;
     pow := 5;
     key_ent_p := false;
     key_esc_p := false;
     delay (50);
end;

procedure colorset;
var
i : word;
c : byte;
om : byte;
begin
     act := 0;
     om := mact;
     mact := 1;
     i := 0;
     c := mainc;
     while 1=1 do
     begin
          getmessages;
          renderall;
          win3db (90,40,210,49,2,0);
          win3db (90,50,210,104,mainc,0);
          win3dnb (100,92,149,102,mainc,0);
          win3dnb (151,92,200,102,mainc,0);
          textprint (92,42,0,1,'WYBOR KOLORU TLA');
          textprint (100,52,0,1,'WYBIERZ KOLOR TLA'+#13+'W CALYM PROGRAMIE');
          textprint (120,95,0,1,'OK');
          textprint (158,95,0,1,'ANULUJ');
          if key_l_p then if c > 0 then dec (c) else c := 7;
          if key_r_p then if c < 7 then inc (c) else c := 0;
          scrwin (100,68,200,90,c*32+23);
          if i > 20 then
          begin
          if key_ent_p then break;
          if key_esc_p then begin c := mainc; break; end;
          end;
          wfvr;
          buftoscr;
          inc (i);
     end;
     if key_ent_p then
     begin
          win3dnb (100,92,149,102,mainc,1);
          buftoscr;
          for i := 0 to 5 do wfvr;
          win3dnb (100,92,149,102,mainc,0);
          buftoscr;
          for i := 0 to 5 do wfvr;
     end
     else
     begin
          win3dnb (151,92,200,102,mainc,1);
          buftoscr;
          for i := 0 to 5 do wfvr;
          win3dnb (151,92,200,102,mainc,0);
          buftoscr;
          for i := 0 to 5 do wfvr;
     end;
     mainc := c;
     key_ent_p := false;
     key_esc_p := false;
     mact := om;
     delay (100);
end;

procedure newfile;
var
i,j : word;
om : byte;
psx,psy : word;
actmp : byte;
s1,s2,sp : string[3];
sp2 : string[1];
licz1,licz2,liczp,l : byte;
begin
     act := 0;
     om := mact;
     mact := 1;
     i := 0;
     j := 0;
     actmp := 0;
     psx := tex.sizex;
     psy := tex.sizey;
     fillchar (ppc3^,3,0);
     fillchar (ppc4^,3,0);
     str (psx,s1);
     str (psy,s2);
     licz1 := length (s1);
     licz2 := length (s2);
     if (licz1 = 1) and (s1[1] = '0') then licz1 := 0;
     if (licz2 = 1) and (s2[1] = '0') then licz2 := 0;
     strpcopy (ppc3,s1);
     strpcopy (ppc4,s2);
     while 1=1 do
     begin
          getmessages;
          renderall;
          win3db (65,30,255,39,2,0);
          win3db (65,40,255,90,mainc,0);
          textprint (67,32,0,1,'NOWY PLIK');
          textprint (81,43,0,1,'WPISZ WYMIARY NOWEJ BITMAPY');
          if actmp = 0 then scrwin (73,53,212,62,79)
                       else scrwin (73,63,212,72,79);
          if actmp = 0 then textsetcolor (126) else textsetcolor (96);
          textprint (75,55,0,1,'SZEROKOSC [X]');
          if actmp = 1 then textsetcolor (126) else textsetcolor (96);
          textprint (75,65,0,1,'WYSOKOSC  [Y]');
          textsetcolor (96);
          if licz1 = 0 then textprint (240-licz1*7,55,0,1,ppc3)
          else
          textprint (240-(licz1-1)*6,55,0,1,ppc3);
          if licz2 = 0 then textprint (240-licz2*7,65,0,1,ppc4)
          else
          textprint (240-(licz2-1)*6,65,0,1,ppc4);
          win3dnb (215,53,245,61,mainc,1);
          win3dnb (215,63,245,71,mainc,1);
          win3dnb (75,75,159,85,mainc,0);
          win3dnb (161,75,245,85,mainc,0);
          textprint (112,78,0,1,'OK');
          textprint (186,78,0,1,'ANULUJ');
          if i > 20 then
          begin
               if key_ent_p then
               begin
                    win3dnb (75,75,159,85,mainc,1);
                    buftoscr;
                    for i := 0 to 5 do wfvr;
                    win3dnb (75,75,159,85,mainc,0);
                    buftoscr;
                    for i := 0 to 5 do wfvr;
                    val (s1,psx,j);
                    val (s2,psy,j);
                    if (psx = 0) or (psy = 0) then
                    begin
                         messagebox (25,50,'WYMIARY BITMAPY NIE MOGA BYC ROWNE ZERU!','ZLE PARAMETRY');
                         continue;
                    end;
                    if longint (psx)*longint (psy) > 65535 then
                    begin
                         messagebox (65,50,'WYMIARY BITMAPY SA ZA DUZE!','ZLE PARAMETRY');
                         continue;
                    end;
                    createtex (psx,psy);
                    break;
               end;
               if key_esc_p then break;
               if key_tab_p then actmp := 1-actmp;
               if key_u_p then actmp := 1-actmp;
               if key_d_p then actmp := 1-actmp;
               if key_any_p then
               begin
                    for j := 2 to 11 do if keyon[j] = 1 then
                    begin
                         if j = 11 then l := 0
                         else
                         l := j-1;
                         if actmp = 0 then
                         begin
                              sp := s1;
                              liczp := licz1;
                         end
                         else
                         begin
                              sp := s2;
                              liczp := licz2;
                         end;
                         if liczp = 0 then
                         begin
                              str (l,sp);
                              if sp <> '0' then
                              inc (liczp)
                              else
                              break;
                         end
                         else
                         if liczp = 1 then
                         begin
                              str (l,sp2);
                              sp[2] := sp2[1];
                              inc (liczp);
                         end
                         else
                         if liczp = 2 then
                         begin
                              str (l,sp2);
                              sp[3] := sp2[1];
                              inc (liczp);
                         end
                         else
                         begin
                              str (l,sp2);
                              sp [3] := sp2[1];
                         end;
                         sp [0] := chr (liczp);
                         if actmp = 0 then
                         begin
                              s1 := sp;
                              licz1 := liczp;
                         end
                         else
                         begin
                              s2 := sp;
                              licz2 := liczp;
                         end;
                    end;
               end;
               if key_del_p then
               begin
                    if actmp = 0 then
                    begin
                         sp := s1;
                         liczp := licz1;
                    end
                    else
                    begin
                         sp := s2;
                         liczp := licz2;
                    end;
                    if liczp = 0 then
                    else
                    if liczp = 1 then
                    begin
                         sp[1] := '0';
                         dec (liczp);
                    end
                    else
                    dec (liczp);
                    if liczp = 0 then sp [0] := #1
                    else
                    sp [0] := chr (liczp);
                    if actmp = 0 then
                    begin
                         s1 := sp;
                         licz1 := liczp;
                    end
                    else
                    begin
                         s2 := sp;
                         licz2 := liczp;
                    end;
               end;
               strpcopy (ppc3,s1);
               strpcopy (ppc4,s2);
          end;
          wfvr;
          buftoscr;
          inc (i);
     end;
     if key_esc_p then
     begin
          win3dnb (161,75,245,85,mainc,1);
          buftoscr;
          for i := 0 to 5 do wfvr;
          win3dnb (161,75,245,85,mainc,0);
          buftoscr;
          for i := 0 to 5 do wfvr;
     end;
     mact := om;
     key_ent_p := false;
     key_esc_p := false;
     delay (100);
end;

procedure scales;
var
om : byte;
i : word;
wybo,wyb : byte;
begin
     om := mact;
     mact := 1;
     act := 0;
     i := 0;
     wybo := 0;
     case scale of
     1:wyb := 0;
     2:wyb := 1;
     4:wyb := 2;
     8:wyb := 3;
     end;
     while 1=1 do
     begin
          getmessages;
          renderall;
          win3db (85,40,235,49,2,0);
          win3db (85,50,235,105,mainc,0);
          textprint (87,42,0,1,'WYBOR SKALI RYSOWANIA');
          textprint (91,52,0,1,'WYBIERZ SKALE RYSOWANIA');
          win3dnb (90,92,159,100,mainc,0);
          win3dnb (161,92,230,100,mainc,0);
          textprint (120,94,0,1,'OK');
          textprint (179,94,0,1,'ANULUJ');
          {}
          scrwin (95,wybo*6+62,230,wybo*6+67,84);
          textprint (96,62,0,1,'1X'+#13+'2X'+#13+'4X'+#13+'8X');
          scrsprtexnf (90,wyb*6+63,pt);
          if i > 20 then
          begin
               if key_esc_p then break;
               if key_ent_p then
               begin
                    case wybo of
                    0:scale := 1;
                    1:scale := 2;
                    2:scale := 4;
                    3:scale := 8;
                    end;
                    pixx := dx*scale;
                    pixy := dy*scale;
                    if pixx > maxsizex then
                    begin
                         pzx := dx-pixx div scale;
                         pixx := maxsizex;
                    end
                    else
                    pzx := 0;
                    if pixy > maxsizey then
                    begin
                         pzy := dy-pixy div scale;
                         pixy := maxsizey;
                    end
                    else
                    pzy := 0;
                    poczpx := (pdx*scale)-(pzx*scale);
                    poczpy := (pdy*scale)-(pzy*scale);
                    break;
               end;
               if key_d_p then inc (wybo);
               if key_u_p then dec (wybo);
               wybo := wybo mod 4;
          end;
          wfvr;
          buftoscr;
          inc (i);
     end;
     if key_ent_p then
     begin
          win3dnb (90,92,159,100,mainc,1);
          buftoscr;
          for i := 0 to 5 do wfvr;
          win3dnb (90,92,159,100,mainc,0);
          buftoscr;
          for i := 0 to 5 do wfvr;
     end;
     if key_esc_p then
     begin
          win3dnb (161,92,230,100,mainc,1);
          buftoscr;
          for i := 0 to 5 do wfvr;
          win3dnb (161,92,230,100,mainc,0);
          buftoscr;
          for i := 0 to 5 do wfvr;
     end;
     mact := om;
end;

procedure opensavedialog (t : byte);
var
i,j,k : word;
om,cnt,ed,p : byte;
msg1,msg2,msg3 : pchar;
dirr : string;
c : char;
pfname : string [12];
begin
     om := mact;
     mact := 1;
     act := 0;
     i := 0;
     getdir (0,dirr);
     if length (dirr) > 28 then delete (dirr,28,length (dirr)-27);
     strpcopy (msg3,dirr);
     fillchar (ppc4^,13,0);
     cnt := 0;
     ed := 0;
     if t = 0 then
     begin
          msg1 := 'OTWORZ PLIK';
          msg2 := 'WPISZ NAZWE PLIKU DO OTWARCIA';
     end
     else
     if t = 1 then
     begin
          msg1 := 'ZAPISZ PLIK';
          msg2 := 'WPISZ NAZWE PLIKU DO ZAPISU';
     end;
     while 1=1 do
     begin
          getmessages;
          renderall;
          win3db (70,30,250,39,2,0);
          win3db (70,40,250,110,mainc,0);
          textprint (72,32,0,1,msg1);
          textprint (160-strlen(msg2)*3,42,0,1,msg2);
          win3dnb (80,95,159,105,mainc,0);
          win3dnb (161,95,240,105,mainc,0);
          textprint (115,98,0,1,'OK');
          textprint (183,98,0,1,'ANULUJ');
          win3dnb (74,48,246,64,mainc,1);
          textprint (76,51,0,1,'AKTUALNY KATALOG:');
          textprint (76,57,0,1,msg3);
          textprint (74,70,0,1,'NAZWA PLIKU:');
          win3dnb (145,68,246,77,mainc,1);
          if i > 20 then
          begin
               if key_ent_p then
               begin
                    if ed = 0 then
                    begin
                         messagebox (80,50,'MUSISZ PODAC NAZWE PLIKU','BLAD');
                         continue;
                    end;
                    pfname := fname;
                    for k := 0 to ed-1 do fname[k+1] := ppc4[k];
                    fname [0] := chr (ed);
                    if t = 0 then
                    begin
                         j := loadf (fname);
                         if j=1 then
                         begin
                              fname := pfname;
                              messagebox (70,50,'PLIK NIE ZOSTAL ZNALEZIONY!','BLAD ODCZYTU PLIKU');
                              continue;
                         end;
                         if j=2 then
                         begin
                              fname := pfname;
                              messagebox (41,50,'ODCZYTANY PLIK NIE JEST PRAWIDLOWY!','BLAD ODCZYTU PLIKU');
                              continue;
                         end;
                    end;
                    if t = 1 then savef (fname);
                    break;
               end;
               if key_esc_p then break;
               if key_l_p then if cnt > 0 then dec (cnt);
               if key_r_p then if cnt < ed then inc (cnt);
               if key_del_p then
               if cnt < ed then
               begin
                    p := ed-cnt-1;
                    for k := 0 to p do ppc4[cnt+k] := ppc4 [cnt+k+1];
                    if ed > 0 then dec (ed);
               end;
               if key_any_p then
               begin
                    if keyon[14] = 1 then
                    begin
                         if cnt < ed then
                         begin
                              p := ed-cnt;
                              for k := 0 to p do ppc4[cnt+k-1] := ppc4 [cnt+k];
                              if ed > 0 then dec (ed);
                              if cnt > 0 then dec (cnt);
                         end
                         else
                         begin
                              if ed > 0 then dec (ed);
                              if cnt > 0 then dec (cnt);
                              ppc4 [cnt] := #0;
                         end;
                    end;
                    for j := 0 to 127 do if keyon [j]=1 then
                    begin
                         c := #0;
                         convert (j,c);
                         if c = #0 then
                         begin
                              if j = 52 then c:='.';
                              if j in [2..10] then c := chr (j+47);
                              if j = 11 then c := '0';
                         end;
                         if (c <> #0) and (cnt < 12) then
                         begin
                              if cnt < 12 then inc (cnt);
                              if ed < 12 then inc (ed);
                              if cnt < ed then
                              begin
                                   p := ed - cnt-1;
                                   for k := p downto 0 do ppc4 [cnt+k] := ppc4 [cnt+k-1];
                              end;
                              ppc4 [cnt-1] := c;
                         end;
                    end;
               end;
          end;
          textprint (147,70,0,1,ppc4);
          if i mod 24 > 12 then vline (147+cnt*6,70,76,96);
          wfvr;
          buftoscr;
          inc (i);
     end;
     textprint (147,70,0,1,ppc4);
     if key_ent_p then
     begin
          win3dnb (80,95,159,105,mainc,1);
          buftoscr;
          for i := 0 to 5 do wfvr;
          win3dnb (80,95,159,105,mainc,0);
          buftoscr;
          for i := 0 to 5 do wfvr;
     end;
     if key_esc_p then
     begin
          win3dnb (161,95,240,105,mainc,1);
          buftoscr;
          for i := 0 to 5 do wfvr;
          win3dnb (161,95,240,105,mainc,0);
          buftoscr;
          for i := 0 to 5 do wfvr;
     end;
     mact := om;
     key_ent_p := false;
     key_esc_p := false;
end;

procedure messagebox (x,y : integer; s,s2 : pchar);
var
sx,sy : word;
sl : word;
i : word;
pact : byte;
om : byte;
begin
     sy := y+50;
     sl := strlen (s);
     sx := x+sl*6+20;
     if sx < 47 then sx := 47;
     om := mact;
     mact := 1;
     i := 0;
     while 1=1 do
     begin
          getmessages;
          renderall;
          win3db (x,y,sx,y+9,2,0);
          textprint (x+2,y+2,0,1,s2);
          win3db (x,y+10,sx,sy,mainc,0);
          textprint (x+10,y+20,0,1,s);
          win3dnb (x+(sx-x) div 2-20,y+37,x+(sx-x) div 2+20,y+45,mainc,0);
          textprint (x+(sx-x) div 2-5,y+39,0,1,'OK');
          if i > 20 then
          if key_ent_p then break;
          wfvr;
          buftoscr;
          inc (i);
     end;
     win3dnb (x+(sx-x) div 2-20,y+37,x+(sx-x) div 2+20,y+45,mainc,1);
     buftoscr;
     for i := 0 to 6 do
     wfvr;
     win3dnb (x+(sx-x) div 2-20,y+37,x+(sx-x) div 2+20,y+45,mainc,0);
     buftoscr;
     for i := 0 to 6 do
     wfvr;
     mact := om;
     key_ent_p := false;
end;

procedure mainloop;
begin
     while apprun do
     begin
          getmessages;
          renderall;
          wfvr;
          buftoscr;
     end;
end;

begin
     initialize;
     title;
     mainloop;
     shutdown;
end.