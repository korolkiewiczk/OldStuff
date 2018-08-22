program midied;
{dolaczone moduly}
uses scr,snd,text,strings,crt,txtrt,dos,midadd;

{deklaracje stalych oraz zmiennych z domyslnymi wartosciami}

const

noteb = 24; {nr nuty bazowej}
numoflines = 21; {liczba lini poziomych}
numoflinesv = 6; {liczba lini pionowych}
maxpoz = 42;
maxmpoz = 8000;
maxmidibufsize = 64000; {maksymalny rozmiar bufora midi}
req = 64000+10000+maxmidibufsize*2; {ilosc wymaganej pamieci}
ver = '0.9';
menuon : boolean = true;
pnr : boolean = false;
mnupoz : shortint = 0;
menuit : array [0..7] of pchar = ('Nowy','Otw¢rz','Zapisz','Zapisz jako','Odegraj','Wybierz kanaˆ','Pomoc','Wyj˜cie');
klucz : ttex = (sizex:10; sizey:31; texmemory:@kluczt);
kluczb : ttex = (sizex:12; sizey:15; texmemory:@klucztb);
krzyz : ttex = (sizex:5; sizey:5; texmemory:@krzyzt);
bemol : ttex = (sizex:4; sizey:5; texmemory:@bemolt);
note1 : ttex = (sizex:6; sizey:4; texmemory:@notet1);
notes : array [0..11] of pchar = ('C','CIS','D','ES','E','F','FIS','G','AS','A','B','H');
a1 : array [0..6] of byte = (0,2,4,5,7,9,11);
a2 : array [0..4] of byte = (1,3,6,8,10);
a3 : array [0..11] of byte = (0,0,3,5,5,8,8,10,13,13,15,15);
a4 : array [0..11] of byte = (1,1,3,6,6,8,8,11,13,13,16,16);
a5 : array [0..11] of byte = (0,1,0,2,0,0,1,0,2,0,2,0);

{deklaracje zmiennych}

var

midibuf : tabp; {pointer}
copybuf : tabp;
copystart,copyend,copybsize : word;
iscopy : boolean;
mport : word;
keychr : char;
keyscan : byte;
keystat : boolean;
ppchr1,ppchr2,ppchr3,ppchr4 : pchar;
fname : string[13];
modf : boolean;
chused : array [0..15] of boolean;
channelssize : array [0..15] of word;
channelsinfo : array [0..15] of record
                                      inst,gl,lr : byte;
end;
mcom : byte;
mpoz : word;
mbpoz : word;
eofmba : word;
eofmb : word;
actch : byte;
cn : boolean;
edpixpoz : integer;
edpoz : longint;
glo,lr2,gl2 : byte;
pb : word;
glbsiz : byte;
przes : byte;

{uzywane procedury}

procedure drawall; forward;
procedure getcmdline (mp,po1,po2 : word; var s,k,km : word); forward;
procedure geteofmb; forward;

procedure getchannelssize;
var
i : word;
c2 : byte;
begin
     fillchar (channelssize[0],32,0);
     for i := 0 to eofmb do
     begin
          c2 := midibuf^[i];
          if (c2 >= $80) and (c2 < $a0) and (c2 <> $ff) then inc (channelssize [blo (c2)]);
     end;
end;

procedure getchannelsused;
var
i : byte;
begin
     getchannelssize;
     for i := 0 to 15 do if channelssize [i] > 0 then chused[i] := true else chused[i] := false;
end;

procedure createchinf;
var
i : word;
j : byte;
begin
     j := 0;
     getchannelsused;
     if glbsiz <> 0 then
     begin
     for i := 0 to eofmb do midibuf^[i] := midibuf^[i+glbsiz];
     dec (mbpoz,glbsiz);
     dec (eofmb,glbsiz);
     end;
     for i := 0 to 15 do if chused[i] then inc (j);
     glbsiz := j*11;
     if glbsiz = 0 then exit;
     for i := eofmb downto 0 do midibuf^[i+glbsiz] := midibuf^[i];
     j := 0;
     for i := 0 to 15 do if chused[i] then
     begin
          midibuf^[(j*11)+0] := $b0+i;                       {"reset all controlers"}
          midibuf^[(j*11)+1] := $79;
          midibuf^[(j*11)+2] := 0;
          midibuf^[(j*11)+3] := $c0+i;                       {wybor instrumentu}
          midibuf^[(j*11)+4] := channelsinfo[i].inst;
          midibuf^[(j*11)+5] := $b0+i;                       {wybor glosnosci}
          midibuf^[(j*11)+6] := $7;
          midibuf^[(j*11)+7] := channelsinfo[i].gl;
          midibuf^[(j*11)+8] := $b0+i;                       {wybor l-r}
          midibuf^[(j*11)+9] := $a;
          midibuf^[(j*11)+10] := channelsinfo[i].lr;
          inc (j);
     end;
     inc (mbpoz,glbsiz);
     inc (eofmb,glbsiz);
end;

procedure newfile (namm : string);
var
i : byte;
begin
     modf := false;
     fname := namm;
     mbpoz := 0;
     mpoz := 0;
     eofmb := 0;
     edpixpoz := 0;
     edpoz := 0;
     actch := 0;
     cn := true;
     glo := 64;
     pb := 8192;
     lr2 := 64;
     gl2 := 127;
     iscopy := false;
     copystart := 0;
     {insm := false;}
     glbsiz := 0;
     przes := 3;
     stopmidi ($330);
     fillchar (midibuf^,maxmidibufsize,255);
     for i := 0 to 15 do chused[i] := false;
     for i := 0 to 15 do
     begin
          channelsinfo[i].inst := 0;
          channelsinfo[i].gl := 127;
          channelsinfo[i].lr := 64;
     end;
end;

function loadf (namm : string) : boolean;
var
f : file;
c : byte;
i,j : word;
begin
     loadf := false;
     assign (f,namm);
     {$I-}
     reset (f,1);
     {$I+}
     if ioresult <> 0 then exit;
     newfile (namm);
     blockread (f,eofmb,2);
     if eofmb <> filesize (f)-2 then
     begin
          close (f);
          exit;
     end;
     blockread (f,midibuf^,eofmb);
     if midibuf^[0] shr 4 <> $b then
     begin
          fillchar (midibuf^,eofmb,$ff);
          close (f);
          exit;
     end;
     close (f);
     c := midibuf^[0];
     i := 0;
     while (not (c in [$80..$9f])) and (c <> $ff) do
     begin
          c := midibuf^[i];
          inc (i);
     end;
     glbsiz := i-1;
     for j := 0 to (i div 11)-1 do
     begin
          c := midibuf^[(j*11)] and $f;
          channelsinfo[c].inst := midibuf^[(j*11)+4];
          channelsinfo[c].gl := midibuf^[(j*11)+7];
          channelsinfo[c].lr := midibuf^[(j*11)+10];
     end;
     getcmdline (0,0,0,i,mbpoz,j);
     {inc (mbpoz);}
     loadf := true;
     modf := false;
     fname := namm;
end;

function savef (namm : string) : boolean;
var
f : file;
begin
     savef := false;
     createchinf;
     if glbsiz = 0 then exit;
     assign (f,namm);
     {$I-}
     rewrite (f,1);
     {$I+}
     if ioresult <> 0 then exit;
     geteofmb;
     blockwrite (f,eofmb,2);
     blockwrite (f,midibuf^,eofmb);
     close (f);
     savef := true;
     modf := false;
     fname := namm;
end;

procedure opensave (t : byte);
var
pnrp : boolean;
c : char;
tdl,crs,tt : byte;
i,pii,j,p : word;
ppc : pchar;
ps : string;
przespom,cp : word;
begin
     pnrp := false;
     tdl := 0;
     crs := 0;
     fillchar (ppchr3^,255,0);
     i := 0;
     pii := 0;
     tt := 0;
     if t = 0 then
     ppc := 'OTWàRZ PLIK'
     else
     ppc := 'ZAPISZ PLIK';
     przespom := 0;
     repeat
           drawall;
           keystat := getkey (keychr,keyscan);
           win3db (90,50,230,130,3,0);
           textsetfade (1);
           textprint (101,54,0,0,ppc);
           textsetfade (21);
           textprint (100,53,0,0,ppc);
           win3dnb (94,65,226,125,3,1);
           textsetcolor (96);
           textprint (107,70,0,1,'PODAJ NAZWE PLIKU:');
           win3dnb (104,80,216,89,3,1);
           textsetfade (85);
           textprint (104,94,0,1,'[ENTER]-ZATWIERDZ'+#13+'[ESC]-ANULUJ');
           cp := crs-przespom;
           if i mod 24 > 12 then vline (106+(cp*6),82,88,0);
           textsetcolor (96);
           if keystat then
           begin
                if keyscan = 1 then pnrp := true;
                if keyscan = 28 then
                begin
                     c := ppchr3[0];
                     j := 0;
                     while c <> #0 do
                     begin
                          ps [j+1] := c;
                          inc (j);
                          c := ppchr3[j]
                     end;
                     ps [0] := chr (j);
                     if ps <> '' then
                     begin
                     if t = 0 then
                     begin
                          if loadf (ps) then break
                          else
                          begin
                               pii := i;
                               tt := 1;
                          end;
                     end
                     else
                     savef (ps);
                     end;
                end;
                if keyscan = 75 then
                begin
                     if crs > 0 then dec (crs);
                     if crs < przespom then dec (przespom);
                end;
                if keyscan = 77 then
                begin
                     if (crs < 255) and (crs < tdl) then inc (crs);
                     if crs-przespom > 18 then inc (przespom);
                end;
                if keyscan = 79 then
                begin
                     crs := tdl;
                     if tdl > 18 then przespom := crs-18 else przespom := 0;
                end;
                if keyscan = 71 then
                begin
                     crs := 0;
                     przespom := 0;
                end;
                if keyscan = 83 then
                if crs < tdl then
                begin
                     p := tdl-crs-1;
                     for j := 0 to p do ppchr3[crs+j] := ppchr3 [crs+j+1];
                     if tdl > 0 then dec (tdl);
                end;
                if keyscan = 14 then
                begin
                     if crs <> 0 then
                     begin
                     if crs < tdl then
                     begin
                          p := tdl-crs;
                          for j := 0 to p do ppchr3 [crs+j-1] := ppchr3 [crs+j];
                          if tdl > 0 then dec (tdl);
                          if crs > 0 then dec (crs);
                          if crs > 17 then dec (przespom);
                     end
                     else
                     begin
                          if tdl > 0 then dec (tdl);
                          if crs > 0 then dec (crs);
                          ppchr3 [crs] := #0;
                          if crs > 17 then dec (przespom);
                     end;
                     end;
                end;
                if keychr in [' '..'z'] then
                begin
                     c := upcase (keychr);
                     if tdl < 255 then
                     begin
                     if crs < 255 then inc (crs);
                     inc (tdl);
                     if crs < tdl then
                     begin
                          p := tdl-crs-1;
                          for j := p downto 0 do ppchr3 [crs+j] := ppchr3 [crs+j-1];
                     end;
                     ppchr3 [crs-1] := c;
                     end;
                     if crs-przespom > 18 then inc (przespom);
                end;
           end;
           fillchar (ppchr4^,255,0);
           strcopy (ppchr4,ppchr3);
           ppchr4 [przespom+18] := #0;
           textprint (106,82,0,1,ppchr4+przespom);
           if tt = 1 then
           begin
                if i-pii > 90 then tt := 0;
                textsetcolor (31);
                textprint (103,110,0,1,'PLIK NIE ZNALEZIONY'+#13+'  LUB USZKODZONY');
           end;
           wfvr;
           buftoscr;
           inc (i);
     until pnrp;
     keyscan := 0;
end;

procedure setch;
var
pnrp : boolean;
i : byte;
w : shortint;
p : word;
e : boolean;
begin
     pnrp := false;
     e := menuon;
     menuon := false;
     w := 0;
     getchannelssize;
     repeat
           drawall;
           keystat := getkey (keychr,keyscan);
           if keystat then
           begin
                if keyscan = 28 then begin actch := w; pnrp := true; end;
                if keyscan = 1 then pnrp := true;
                if keyscan = 80 then inc (w);
                if keyscan = 72 then dec (w);
                if (keyscan > 1) and (keyscan < 12) then w := keyscan-2;
                if keyscan = 12 then dec (channelsinfo[w].inst);
                if keyscan = 13 then
                if channelsinfo[w].inst < 127 then inc (channelsinfo[w].inst)
                else channelsinfo[w].inst := 0;
                if keyscan = 43 then dec (channelsinfo[w].gl);
                if keyscan = 14 then
                if channelsinfo[w].gl < 127 then inc (channelsinfo[w].gl)
                else channelsinfo[w].gl := 0;
                if keyscan = 26 then dec (channelsinfo[w].lr);
                if keyscan = 27 then
                if channelsinfo[w].lr < 127 then inc (channelsinfo[w].lr)
                else channelsinfo[w].lr := 0;
           end;
           if channelsinfo[w].inst > 128 then channelsinfo[w].inst := 127;
           if channelsinfo[w].gl > 128 then channelsinfo[w].gl := 127;
           if channelsinfo[w].lr > 128 then channelsinfo[w].lr := 127;
           if w < 0 then w := 15;
           if w > 15 then w := 0;
           win3db (79,20,241,160,3,0);
           textsetfade (1);
           textprint (95,28,0,0,'WYBàR KANAU');
           textsetfade (21);
           textprint (94,27,0,0,'WYBàR KANAU');
           textsetcolor (96);
           textprint (156,40,0,1,'S');
           textprint (182,40,0,1,'I');
           textprint (202,40,0,1,'G');
           textprint (223,40,0,1,'P');
           vline (142,40,144,0);
           vline (174,40,144,0);
           vline (194,40,144,0);
           vline (214,40,144,0);
           vline (234,40,144,0);
           for i := 1 to 16 do
           begin
                textsetcolor (i+15);
                if i > 9 then
                strcopy (ppchr4,'KANAL#')
                else
                strcopy (ppchr4,'KANAL# ');
                strpchar (i,ppchr3);
                strcat (ppchr4,ppchr3);
                textprint (85,(i*6)+41,0,1,ppchr4);
                p := channelssize [i-1];
                strpchar (p,ppchr4);
                textprint (143,(i*6)+41,0,1,ppchr4);
                p := channelsinfo[i-1].inst;
                strpchar (p,ppchr4);
                textprint (175,(i*6)+41,0,1,ppchr4);
                p := channelsinfo[i-1].gl;
                strpchar (p,ppchr4);
                textprint (195,(i*6)+41,0,1,ppchr4);
                p := channelsinfo[i-1].lr;
                strpchar (p,ppchr4);
                textprint (215,(i*6)+41,0,1,ppchr4);
           end;
           textsetfade (86);
           textprint (85,144,0,1,'[ENTER]-ZATWIERDZ'+#13+'[ESC]-ANULUJ');
           win3dnb (80,(w*6)+46,240,(w*6)+52,3,0);
           wfvr;
           buftoscr;
     until pnrp;
     menuon := e;
     keyscan := 0;
end;

procedure help;
var
pnrp : boolean;
e : boolean;
begin
     pnrp := false;
     e := menuon;
     menuon := false;
     repeat
           drawall;
           keystat := getkey (keychr,keyscan);
           if keystat then if (keyscan = 28) or (keyscan = 1) then pnrp := true;
           win3db (50,20,270,160,3,0);
           textsetfade (196);
           textprint (87,31,0,0,'POMOC PROGRAMU');
           textsetfade (213);
           textprint (86,30,0,0,'POMOC PROGRAMU');
           textsetcolor (96);
           textprint (51,50,0,0,
           '[ESC]-menu on/off'+#13+
           'Klawisze od [1] do [/]'+#13+
           '-wyb¢r nuty'+#13+
           '[TAB]-przeˆ¥czanie mi©dzy'+#13+
           'trybem ton¢w a p¢ˆton¢w'+#13+
           '[DEL]-kasuj wszystkie'+#13+
           'nuty'+#13+
           '[Shift]+klawisz nuty-'+#13+
           'wyˆ¥cz nut©');
           wfvr;
           buftoscr;
     until pnrp;
     keyscan := 0;
     menuon := e;
end;

procedure initialize;
var
n : longint;
p,m : word;
i,d : byte;
begin
     clrscr;
     textcolor (0);
     textbackground (1);
     write ('               MIDIED v '+ver+' Copyright (c) Kamil Korolkiewicz 2004               ');
     textcolor (7);
     textbackground (0);
     writeln ('Getting memory for program');
     n := maxavail;
     textcolor (15);
     writeln ('Memory available = ',n,' bytes   req = ',req,' bytes');
     if n < req then halt(1);
     textcolor (8);
     writeln (#9+' screen buffer');
     getmem (buf,64000);
     writeln (#9+' midi buffer');
     getmem (midibuf,maxmidibufsize);
     getmem (copybuf,maxmidibufsize);
     writeln (#9+' sound buffer');
     getmem (sndbuf,bufsize);
     writeln (#9+' font buffer');
     getmem (txttbl,11200);
     getmem (ppchr1,255);
     getmem (ppchr2,255);
     getmem (ppchr3,255);
     getmem (ppchr4,255);
     textcolor (7);
     writeln ('Unpacking text font');
     unpack;
     textdefaults;
     textsetcolor (96);
     writeln ('Cleaning buffers');
     textcolor (8);
     writeln (#9+' screen buffer');
     fillchar (buf^,64000,0);
     writeln (#9+' midi buffer');
     textcolor (7);
     writeln ('Initializing sound routime');
     textcolor (8);
     writeln (#9+' Geting BLASTER environment');
     if getenv ('BLASTER') = '' then
     begin
          textcolor (7);
          writeln ('BLASTER is not set. Using defaults.');
          p := $220;
          d := 1;
          i := 7;
          m := $330;
          textcolor (8);
     end;
     getsbenv (p,d,i,m);
     writeln (#9+' SB main port = ',p);
     writeln (#9+' SB DMA = ',d);
     writeln (#9+' SB IRQ = ',i);
     writeln (#9+' GENERAL MIDI (MPU-401) main port = ',m);
     writeln (#9+' Initializing sound');
     if not initsnd (p,d,i) then
     begin
          textcolor (7);
          writeln ('SB not present');
          halt (1);
     end;
     resetmidi (m);
     textcolor (7);
     mport := m;
     writeln ('Geting params');
     if paramcount <> 0 then loadf (paramstr(1)) else newfile ('NONAME.MID');
     writeln ('Setting 8-32 palette');
     set_8_32_pal;
     writeln ('Initializing VGA 320X200X8 graphics');
     initvgascr;
     setpalette;
end;

procedure shutdown;
begin
     closesnd;
     inittxtscr;
end;

procedure geteofmb;
var
i,j : word;
a : boolean;
begin
     j := 0;
     a := true;
     for i := 0 to maxmidibufsize do
     begin
          if (midibuf^[i] = $ff) and (a) then
          begin
               j := i;
               a := false;
          end;
          if midibuf^[i] <> $ff then a := true;
     end;
     eofmb := j;
end;

procedure getcmdline (mp,po1,po2 : word; var s,k,km : word);
var
i,k2,k3 : word;
j : byte;
begin
     k2 := po2;
     if k2 = 0 then po1 := glbsiz;
     for i := po1 to maxmidibufsize do
     begin
          j := midibuf^[i];
          if k2 = mp then
          begin
               k3 := i;
               s := k3;
               j := midibuf^[k3];
               if j = $ff then inc (k3) else
               while j <> $ff do
               begin
                    j := midibuf^[k3];
                    inc (k3);
                    if k3 > maxmidibufsize then break;
               end;
               if k3 > 2 then
               k := k3-1
               else
               k := 0;
               break;
          end;
          if j = $ff then inc (k2);
     end;
     km := k2;
end;

procedure incmpoz (n : word);
var
a : byte;
b,c,n2 : word;
begin
     if mpoz <= maxmpoz then
     inc (mpoz,n);
     n2 := n;
     if mpoz > maxmpoz then
     begin
          n2 := n-(mpoz-maxmpoz);
          mpoz := maxmpoz;
     end;
     edpixpoz := mpoz-edpoz;
     if edpixpoz > maxpoz then
     begin
          inc (edpoz,n);
          edpixpoz := maxpoz;
     end;
     if edpoz > mpoz-edpixpoz then edpoz := mpoz-edpixpoz;
     for b := 1 to n2 do
     begin
     c := mbpoz;
     if midibuf^[mbpoz+1] = $ff then
     begin
          if mbpoz < maxmidibufsize then inc (mbpoz)
     end
     else
     repeat
           if mbpoz > maxmidibufsize then
           begin
                mbpoz := c;
                break;
           end;
           a := midibuf^[mbpoz+1];
           inc (mbpoz);
     until a = $ff;
     end;
end;

procedure decmpoz (n : word);
var
p : longint;
a : byte;
b,c : word;
begin
     p := mpoz;
     if p >= 0 then
     dec (p,n);
     if p < 0 then p := 0;
     mpoz := p;
     edpixpoz := mpoz-edpoz;
     if edpixpoz < 0 then
     begin
          dec (edpoz,n);
          edpixpoz := 0;
     end;
     if edpoz < 0 then edpoz := 0;
     for b := 1 to n do
     begin
     c := mbpoz;
     if midibuf^[mbpoz-1] = $ff then
     begin
          if mbpoz > 0 then dec (mbpoz)
     end
     else
     repeat
           a := midibuf^[mbpoz-1];
           if mbpoz > 0 then dec (mbpoz) else
           begin
                mbpoz := c;
                break;
           end;
     until a = $ff;
     end;
end;

procedure addcmd (ct,p1,p2 : byte);
var
i : word;
s,k,e,d : word;
pm : boolean;
begin
     getcmdline (mpoz,0,0,s,k,e);
     e := 0;
     for i := s to k do
     begin
          if midibuf^[i] > $7f then if blo (midibuf^[i])=actch then inc (e);
          if e > 15 then exit;
     end;
     if eofmb+3 >= maxmidibufsize then exit;
     if ct shr 4 <> 9 then
     begin
          pm := true;
          d := mbpoz;
          mbpoz := s;
     end
     else pm := false;
     if mbpoz < eofmb then
     for i := eofmb downto mbpoz do midibuf^[i+3] := midibuf^[i];
     midibuf^[mbpoz] := ct;
     midibuf^[mbpoz+1] := p1;
     midibuf^[mbpoz+2] := p2;
     if pm then mbpoz := d;
     if mbpoz >= eofmb then eofmb := mbpoz+3
     else
     inc (eofmb,3);
     inc (mbpoz,3);
     modf := true;
end;

function delcmd (mp : word; ct,p1 : byte) : byte;
var
i : word;
j,k : byte;
p : longint;
k2,k3 : word;
begin
     k2 := 0;
     k := 0;
     for i := glbsiz to maxmidibufsize do
     begin
          j := midibuf^[i];
          if k2 = mp then
          begin
               k3 := i;
               j := midibuf^[k3];
               while j <> $ff do
               begin
                    j := midibuf^[k3];
                    if j = ct then
                    begin
                         j := midibuf^[k3+1];
                         if j = p1 then
                         begin
                              k := 1;
                              break;
                         end;
                    end;
                    inc (k3,3);
               end;
               break;
          end;
          if j = $ff then inc (k2);
     end;
     delcmd := k;
     if k = 0 then exit;
     for i := k3+3 to eofmb do midibuf^[i-3] := midibuf^[i];
     midibuf^[eofmb] := $ff;
     midibuf^[eofmb-1] := $ff;
     midibuf^[eofmb-2] := $ff;
     p := longint (mbpoz)-3;
     if p >= 0 then
     dec (mbpoz,3);
     p := longint (eofmb)-3;
     if p >= 0 then
     dec (eofmb,3);
     modf := true;
     geteofmb;
end;

procedure dell (mp : word);
var
i,l1,l2,l3,lp1 : word;
j : byte;
s,y,p : word;
pp : longint;
begin
     getcmdline (mp,0,0,s,y,p);
     if y > eofmb then exit;
     if y < s then exit;
     modf := true;
     l1 := s;
     l2 := s;
     l3 := s;
     while l1 < y do
     begin
          j := midibuf^[l1];
          while (blo (j) = actch) and (j <> $ff) do
          begin
               inc (l1,3);
               inc (l3,3);
               j := midibuf^[l1];
          end;
          lp1 := l2;
          while (blo (j) <> actch) and (j <> $ff) do
          begin
               inc (l1,3);
               inc (l2,3);
               j := midibuf^[l1];
          end;
          for i := lp1 to l2 do midibuf^[i] := midibuf^[i+(l3-s)];
     end;
     for i := l2 to eofmb do midibuf^[i] := midibuf^[i+(l3-s)];
     for i := eofmb-(l3-s) to eofmb do midibuf^[i] := $ff;
     j := (l3-s);
     pp := longint (mbpoz)-j;
     if pp >= 0 then
     dec (mbpoz,j)
     else mbpoz := 0;
     pp := longint (eofmb)-j;
     if pp >= 0 then
     dec (eofmb,j)
     else eofmb := 0;
     geteofmb;
end;

procedure dellall;
var
i,j,l1,l2,l3,l4 : word;
cm : byte;
begin
     i := glbsiz;
     l1 := i;
     l2 := i;
     l3 := 0;
     while i < eofmb do
     begin
          cm := midibuf^[l1];
          while (blo (cm) = actch) and (cm <> $ff) do
          begin
               if l1 >= eofmb then break;
               inc (l1,3);
               inc (l3,3);
               cm := midibuf^[l1];
          end;
          l4 := l2;
          while (blo (cm) <> actch) and (cm <> $ff) do
          begin
               if l1 >= eofmb then break;
               inc (l1,3);
               inc (l2,3);
               cm := midibuf^[l1];
          end;
          while (cm = $ff) do
          begin
               if l1 >= eofmb then break;
               inc (l1);
               inc (l2);
               cm := midibuf^[l1];
          end;
          if l2 <> l4 then
          for j := l4 to l2-1 do midibuf^[j] := midibuf^[j+l3];
          i := l1;
     end;
     {if l3 > eofmb then l3 := eofmb;}
     for i := eofmb-l3 to eofmb do midibuf^[i] := $ff;
     {dec (eofmb,l3);}
     geteofmb;
     getcmdline (0,0,0,i,mbpoz,j);
     {if mbpoz <> 0 then
     inc (mbpoz);}
end;

procedure copymid;
var
i,j,k,p : word;
s,e,b : word;
begin
     fillchar (copybuf^,maxmidibufsize,$ff);
     p := copystart;
     if copystart > copyend then
     begin
          k := copystart;
          copystart := copyend;
          copyend := k;
     end;
     j := 0;
     copybsize := 0;
     for k := copystart to copyend do
     begin
          getcmdline (k,0,0,s,e,b);
          for i := s to e do
          begin
               if (blo (midibuf^[i]) = actch) and (midibuf^[i] > $79) then
               begin
                    copybuf^[j] := midibuf^[i];
                    copybuf^[j+1] := midibuf^[i+1];
                    copybuf^[j+2] := midibuf^[i+2];
                    inc (j,3);
               end;
          end;
          copybuf^[j] := $ff;
          inc (j);
     end;
     copystart := p;
     copybsize := j;
end;

procedure putmid;
var
i,j,k,l,m : word;
p1 : byte;
pp : boolean;
s,e,b : word;
begin
     j := mpoz;
     k := mbpoz;
     l := edpoz;
     m := edpixpoz;
     pp := false;
     for i := 0 to copybsize do
     begin
          p1 := copybuf^[i];
          if p1 = $ff then
          begin
               incmpoz (1);
               pp := true;
               continue;
          end;
          if p1 > $79 then
          begin
               addcmd (p1,copybuf^[i+1],copybuf^[i+2]);
               if not pp then inc (k,3);
          end;
     end;
     mpoz := j;
     mbpoz := k;
     edpoz := l;
     edpixpoz := m;
end;

procedure entry (p : byte);
var
s,e,b : word;
begin
     if p = 0 then newfile ('NONAME.MID');
     if p = 1 then opensave (0);
     if p = 2 then if fname = 'NONAME.MID' then opensave (1) else savef (fname);
     if p = 3 then opensave (1);
     if p = 4 then
     begin
          stopmidi (sbinfo.midi_port);
          createchinf;
          geteofmb;
          inc (eofmb);
          s := 0;
          if mpoz <> 0 then
          getcmdline (mpoz,0,0,s,e,b);
          initmus (sbinfo.midi_port,midibuf,s,eofmb);
          pz := mpoz;
          dec (eofmb);
     end;
     if p = 5 then setch;
     if p = 6 then help;
     if p = 7 then pnr := true;
end;

procedure getkeys;
var
c,z,a,b : byte;
begin
     if menuon then
     begin
          if keyscan = 80 then inc (mnupoz); {d}
          if keyscan = 72 then dec (mnupoz); {u}
          if keyscan = 28 then entry (mnupoz);
     end
     else
     begin
          if shfp then b := 64 else b := 1;
          if keyscan = 75 then decmpoz (1*b); {r}
          if keyscan = 77 then incmpoz (1*b); {l}
          if keyscan = 72 then decmpoz (8*b); {u}
          if keyscan = 80 then incmpoz (8*b); {d}
          if not musicplayed then
          begin
               if keyscan = 83 then
               if shfp then
               begin
                    dellall;
                    {fillchar (midibuf^,maxmidibufsize,255);}
                    mpoz := 0;
                    edpixpoz := 0;
                    edpoz := 0;
               end
               else dell (mpoz);
          end;
          if not shfp then
          begin
          if not musicplayed then
          begin
               if keyscan = 13 then if delcmd (mpoz,$e0+actch,lo (pb2m (pb))) = 0
               then addcmd ($e0+actch,lo (pb2m (pb)),hi (pb2m (pb)));
               if keyscan = 43 then if delcmd (mpoz,$b0+actch,$a) = 0 then addcmd ($b0+actch,$a,lr2);
               if keyscan = 14 then if delcmd (mpoz,$b0+actch,$7) = 0 then addcmd ($b0+actch,$7,gl2);
          end;
          if keyscan = 11 then if pb > 0 then dec (pb,256) else pb := 16386;
          if keyscan = 12 then inc (pb,256);
          end
          else
          begin
          if keyscan = 12 then if lr2 > 0 then dec (lr2) else lr2 := 127;
          if keyscan = 13 then inc (lr2);
          if keyscan = 43 then if gl2 > 0 then dec (gl2) else gl2 := 127;
          if keyscan = 14 then inc (gl2);
          end;
          if pb > 16386 then pb := 0;
          if lr2 > 127 then lr2 := 0;
          if gl2 > 127 then gl2 := 0;
          {if keyscan = 82 then insm := not insm;}
          if not shfp then
          begin
          if keyscan = 71 then przes := przes+128;
          end
          else
          begin
          if keyscan = 71 then decmpoz (maxmpoz);
          if keyscan = 79 then incmpoz (maxmpoz);
          end;
          if shfp then
          begin
               if keyscan = 73 then inc (glo);
               if keyscan = 81 then if glo > 0 then dec (glo) else glo := 127;
          end
          else
          begin
          if przes and $80 = 0 then
          begin
               if keyscan = 73 then if przes < 5 then inc (przes);
               if keyscan = 81 then if przes > 0 then dec (przes);
          end;
          end;
          if glo > 127 then glo := 0;
          if keyscan = 15 then cn := not cn;
          if not musicplayed then
          if (keyscan in [44..53]) or (keyscan in [30..40]) or (keyscan in [16..27]) or (keyscan in [2..10]) then
          begin
          if shfp then b := $80 else b := $90;
          if keyscan in [44..53] then c := keyscan-44;
          if keyscan in [30..40] then c := keyscan-30+10;
          if keyscan in [16..27] then c := keyscan-16+11+10;
          if keyscan in [2..10]  then c := keyscan-2+12+11+10;
          if cn then
          begin
          a := 7;
          z := a1 [c mod 7];
          end
          else
          begin
          a := 5;
          z := a2 [c mod 5];
          end;
          if (not (keyscan in [25..27])) and (not (keyscan in [2..10])) and (not cn) then
          begin
          {if insm then insline (mpoz);}
          if delcmd (mpoz,b+actch,(c div a)*12+noteb+z) = 0 then
          addcmd (b+actch,(c div a)*12+noteb+z,glo);
          if przes and ($80) = 0 then
          begin
               incmpoz (rsqr (przes));
               if b <> $80 then
               begin
                    if delcmd (mpoz,$80+actch,(c div a)*12+noteb+z) = 0 then
                    addcmd ($80+actch,(c div a)*12+noteb+z,glo);
               end;
          end;
          stopnote;
          playnote (actch,channelsinfo[actch].inst,(c div a)*12+noteb+z,channelsinfo[actch].gl,glo,channelsinfo[actch].lr);
          end;
          if cn then
          begin
          {if insm then insline (mpoz);}
          if delcmd (mpoz,b+actch,(c div a)*12+noteb+z) = 0 then
          addcmd (b+actch,(c div a)*12+noteb+z,glo);
          if przes and ($80) = 0 then
          begin
               incmpoz (rsqr (przes));
               if b <> $80 then
               begin
                    if delcmd (mpoz,$80+actch,(c div a)*12+noteb+z) = 0 then
                    addcmd ($80+actch,(c div a)*12+noteb+z,glo);
               end;
          end;
          stopnote;
          playnote (actch,channelsinfo[actch].inst,(c div a)*12+noteb+z,channelsinfo[actch].gl,glo,channelsinfo[actch].lr);
          end;
          end;
     end;
     if keyscan = 1 then
     begin
          if iscopy then iscopy := false
          else
          menuon := not menuon;
     end;
     if (musicplayed) and ((keychr = 'p') or (keychr = 'P')) then stopmidi (sbinfo.midi_port);
     if (keyscan >= 59) and (keyscan <= 66) then entry (keyscan-59);
     if keyscan = 67 then
     begin
          copystart := mpoz;
          iscopy := not iscopy;
     end;
     if keyscan = 68 then iscopy := not iscopy;
     if keyscan = 57 then if iscopy then copymid;
     if shfp then
     begin
          if keyscan = 57 then putmid;
     end
     else
     if keyscan = 57 then if iscopy then copymid;
end;

{glowna procedura rysujaca}
procedure drawall;
var
i,j,p2,p4,d,t1,t2 : word;
pp,pp2 : longint;
p,p3,z,k : byte;
cm,pa1,pa2 : word;
a,b,c : word;
kol : array [0..47] of byte;
begin
     textsetfade (110);
     scrwin (0,0,320,152,124);
     scrwin (0,152,320,200,0);
     win3db (19,119,319,127,3,1);
     win3db (19,128,319,136,3,1);
     win3db (19,137,319,145,3,1);
     for i := 1 to numoflinesv*7 do vline (i*7+19,0,118,123);
     for i := 1 to numoflinesv*7 do vline (i*7+19,120,127,119);
     for i := 1 to numoflinesv*7 do vline (i*7+19,129,136,119);
     for i := 1 to numoflinesv*7 do vline (i*7+19,138,145,119);
     for i := 0 to numoflines-1 do
     begin
          if (i = 4) or (i = 10) or (i = 16) then
          hline (0,i*5+12,320,125)
          else
          hline (0,i*5+12,320,110);
     end;
     p := (edpoz mod 8)*7;
     for i := 1 to numoflinesv do
     begin
          vline (i*56+19-p,0,118,75);
          p2 := i+(edpoz div 8);
          strpchar (p2,ppchr1);
          p3 := strlen (ppchr1)*3;
          textprint (i*56-p3+19-p,146,0,1,ppchr1);
     end;
     vline (19,0,118,110);
     vline (edpixpoz*7+19,0,118,76);
     vline (edpixpoz*7+26,0,118,76);
     scrsprtexnf (5,34,klucz);
     scrsprtexnf (5,68,kluczb);
     textsetfade (214);
     strpchar (mpoz,ppchr1);
     strcopy (ppchr2,'POZYCJA KURSORA:');
     strcat (ppchr2,ppchr1);
     textprint (0,153,0,1,ppchr2);
     strpchar (mpoz div 16,ppchr1);
     strcopy (ppchr2,'POZYCJA KURSORA W SEC:');
     strcat (ppchr2,ppchr1);
     textprint (0,159,0,1,ppchr2);
     strpcopy (ppchr2,fname);
     strcopy (ppchr1,'NAZWA PLIKU:');
     strcat (ppchr1,ppchr2);
     textprint (160,153,0,1,ppchr1);
     {for i := 0 to 255 do if midibuf^[i] <> 255 then
     begin
     putpixel (i*2,199,midibuf^[i]+15);
     putpixel (i*2+1,199,midibuf^[i]+15);
     end;
     for i := 0 to eofmb*2 do putpixel (i,194,127);
     getcmdline (3,0,0,p2,p4,a);
     for i := p2 to p4 do if midibuf^[i] <> 255 then putpixel (i,197,midibuf^[i]+15);}
     getcmdline (edpoz,0,0,p2,p4,a);
     b := p2;
     textsetcolor (96);
     for i := edpoz to edpoz+43 do
     begin
          getcmdline (i,b,a,p2,p4,c);
          j := p2;
          d := p4;
          p := 0;
          while j < d do
          begin
               cm := midibuf^[j];
               pa1 := midibuf^[j+1];
               case cm shr 4 of
               8,9:if (pa1 >= noteb) and (blo(cm) = actch) then
                 begin
                      t1 := pa1 mod 12;
                      t2 := (pa1-noteb) div 12;
                      if t2 mod 2 = 0 then
                      z := a3 [t1]
                      else
                      z := a4 [t1];
                      k := a5 [t1];
                      t2 := 113-((t2*2240) shr 7);
                      if cm shr 4 = 9 then
                      scrsprtexnf (i*7+20-(edpoz*7),t2-z,note1)
                      else
                      vline (i*7+20-(edpoz*7)-1,t2-z,t2-z+4,31);
                      if k = 1 then scrsprtexnf (longint (i)*7+14-(edpoz*7),t2-z,krzyz);
                      if k = 2 then scrsprtexnf (longint (i)*7+14-(edpoz*7),t2-z,bemol);
               end;
               $e,$b: if (p < 3) and (blo(cm) = actch) then
                begin
                     p3 := cm shr 4;
                     if p3 = $e then textprint (i*7+21-(edpoz*7),121+p*9,0,1,'P');
                     if (p3 = $b) and (pa1=$a) then textprint (i*7+21-(edpoz*7),121+p*9,0,1,'L');
                     if (p3 = $b) and (pa1=$7) then textprint (i*7+21-(edpoz*7),121+p*9,0,1,'G');
                     inc (p);
               end;
          end;
          inc (j,3);
          end;
     end;
     getcmdline (mpoz,0,0,p2,p4,a);
     i := p2;
     j := 0;
     while i <= p4 do
     begin
          if blo (midibuf^[i]) = actch then
          begin
               kol [j] := midibuf^[i];
               kol [j+1] := midibuf^[i+1];
               kol [j+2] := midibuf^[i+2];
               inc (j,3);
               if j > 47 then break;
          end;
          inc (i,3);
     end;
     textsetfade (214);
     if musicplayed then textprint (0,179,0,1,'NACISNIJ [P] ABY PRZERWAC');
     textsetfade (86);
     strcopy (ppchr1,'G:');
     strpchar (channelsinfo[actch].gl,ppchr2);
     strcat (ppchr1,ppchr2);
     textprint (0,187,0,1,ppchr1);
     strcopy (ppchr1,'I:');
     strpchar (channelsinfo[actch].inst,ppchr2);
     strcat (ppchr1,ppchr2);
     textprint (35,187,0,1,ppchr1);
     strcopy (ppchr1,'P:');
     strpchar (channelsinfo[actch].lr,ppchr2);
     strcat (ppchr1,ppchr2);
     textprint (70,187,0,1,ppchr1);
     strcopy (ppchr1,'J:');
     strpchar (rsqr (przes),ppchr2);
     strcat (ppchr1,ppchr2);
     textprint (105,187,0,1,ppchr1);
     strcopy (ppchr1,'C:');
     strpchar (actch+1,ppchr2);
     strcat (ppchr1,ppchr2);
     textprint (134,187,0,1,ppchr1);
     strcopy (ppchr1,'PB:');
     strpchar (pb,ppchr2);
     strcat (ppchr1,ppchr2);
     textprint (160,159,0,1,ppchr1);
     strcopy (ppchr1,'LR2:');
     strpchar (lr2,ppchr2);
     strcat (ppchr1,ppchr2);
     textprint (210,159,0,1,ppchr1);
     strcopy (ppchr1,'GL2:');
     strpchar (gl2,ppchr2);
     strcat (ppchr1,ppchr2);
     textprint (260,159,0,1,ppchr1);
     strcopy (ppchr1,'GLOSNOSC NA NUTE:');
     fillchar (ppchr2^,255,0);
     strpchar (glo,ppchr2);
     strcat (ppchr1,ppchr2);
     textprint (0,194,0,1,ppchr1);
     hline (0,185,320,117);
     hline (0,165,320,117);
     vline (158,185,200,117);
     vline (158,152,165,117);
     i := 0;
     p2 := 0;
     p4 := 167;
     t1 := 160;
     t2 := 187;
     if musicplayed then if pz*7+15-(edpoz*7) > 10 then win3db (pz*7+19-(edpoz*7),146,pz*7+26-(edpoz*7),151,0,0);
     while i < j do
     begin
          if (kol[i] shr 4 <> 8) and (kol[i] shr 4 <> 9) and (kol[i] shr 4 <> $e) and (kol[i] shr 4 <> $b) then
          begin
               inc (i,3);
               continue;
          end;
          if (kol[i] shr 4 = 8) or (kol[i] shr 4 = 9) then
          begin
          if (kol[i] shr 4 = 8) then textsetfade (21) else textsetfade (117);
          p := (kol[i+1]-noteb) mod 12;
          strcopy (ppchr1,notes[p]);
          strpchar (kol[i+1],ppchr2);
          strcat (ppchr1,'(');
          strcat (ppchr1,ppchr2);
          strcat (ppchr1,')');
          strpchar (kol[i+2],ppchr2);
          textprint (p2,p4,0,1,ppchr1);
          textprint (t1,t2,0,1,ppchr2);
          end
          else
          begin
          textsetfade (53);
          if kol[i] shr 4 = $e then strcopy (ppchr1,'P');
          if (kol[i] shr 4 = $b) and (kol[i+1] = $a) then strcopy (ppchr1,'L');
          if (kol[i] shr 4 = $b) and (kol[i+1] = $7) then strcopy (ppchr1,'G');
          strpchar (kol[i+2],ppchr2);
          strcat (ppchr1,'(');
          strcat (ppchr1,ppchr2);
          strcat (ppchr1,')');
          strpchar (kol[i+2],ppchr2);
          textprint (p2,p4,0,1,ppchr1);
          textprint (t1,t2,0,1,ppchr2);
          end;
          inc (i,3);
          inc (p2,strlen(ppchr1)*7);
          inc (t1,20);
          if p2 > 260 then
          begin
               inc (p4,6);
               p2 := 0;
          end;
          if t1 > 310 then
          begin
               inc (t2,6);
               t1 := 160;
          end;
     end;
     pp := copystart*7+20-(edpoz*7);
     pp2 := copyend*7+20-(edpoz*7)+7;
     if pp2 < pp then pp2 := pp2-8;
     if pp < 20 then pp := 20;
     if iscopy then scrtranspwin (pp,0,pp2,119,31);
     if menuon then
     begin
          win3db (80,30,240,150,3,0);
          textsetcolor (96);
          textprint (130,35,0,1,'MIDIED v '+ver);
          textsetcolor (127);
          textprint (129,34,0,1,'MIDIED v '+ver);
          for i := 0 to 7 do
          begin
               if mnupoz = i then
               begin
                    win3dnb (84,i*13+44,236,i*13+44+12,3,1);
                    textsetfade (20);
               end
               else
               begin
                    win3dnb (84,i*13+44,236,i*13+44+12,3,0);
                    textsetcolor (96);
               end;
               textprint (90,i*13+45,0,0,menuit [i]);
          end;
     end;
     if np then
     begin
          inc (li);
          if li mod 26 = 0 then stopnote;
     end;
     copyend := mpoz;
end;

{glowna petla}

procedure mainloop;
begin
     repeat
           drawall;
           keystat := getkey (keychr,keyscan);
           if keystat then getkeys;
           if mnupoz > 7 then mnupoz := 0;
           if mnupoz < 0 then mnupoz := 7;
           wfvr;
           buftoscr;
     until pnr;
end;

{poczatek programu}
begin
     initialize;
     mainloop;
     shutdown;
end.