{TRAPED v. 0.5 copyright (c) Kamil Korolkiewicz 2004}
{$G+}
{$M 2048,0,655360}
program traped;
uses scr,trapcrt,pfile,text,strings,time;
{$I ANIPROCS.PAS}
{$I GUNS.PAS}
{$I TRAPMAP.PAS}
{$I PLAYER.PAS}
const

pakname = 'TRAP.PAK';
maxtextures = 128;
maxsprites = 64;
maxanimations = 64;
maxmonstersanimations = 10;
maxitemssprites = 32;
maxgunssprites = 32;
maxanimtexts = 80;
playeranimtab : array [0..7] of byte = (0,1,2,4,6,7,8,10);
helptxt : pchar = 'ABY UZYSKAC POMOC PRZECZYTAJ README.TXT';
watertab : array [1..3] of byte = (3*32+16,4*32+16,0*32+16);
maxscripts = 256;

type

tscript = record
                name : pchar;
                sa : word;
end;

var
ks : byte;
kc : char;
{}
trapmap : trapmapdesc;
trapmapfname : pchar;
{}
traptextures : array [0..maxtextures-1] of ttex;
trapsprites : array [0..maxsprites-1] of ttex;
trapanimations : array [0..maxanimations-1] of tanimation;
trapmonstersanimations : array [0..maxmonstersanimations-1] of tanimation;
playeranimations : tanimation;
handanimations : tanimation;
trapitemssprites : array [0..maxitemssprites-1] of ttex;
trapgunssprites : array [0..maxgunssprites-1] of tanimation;
animtexts : array [0..maxanimtexts-1] of ttex;
ntraptextures : byte;
ntrapsprites : byte;
ntrapanimations : byte;
ntrapmonstersanimations : byte;
ntrapitemssprites : byte;
ntrapgunssprites : byte;
ianimtexts : word;
ntrapinfo : byte;
{}
currtraptexture : byte;
currtrapsprite : byte;
currtrapanimation : byte;
currtrapmonsteranimation : byte;
currtrapitemssprites : byte;
currtrapgunssprites : byte;
currfade : byte;
currwater : byte;
curref : byte;
currinfo : byte;
texstype : byte;
currtrapinforec : trapinforec;
trapmapname : pchar;
midimusicname : pchar;
{}
trappfrec : pfiler;
trapfilehandle : fileor;
{}
l : boolean;
{}
oldexitproc : pointer;
{}
frames : longint;
{}
ednet : boolean;
edfade : boolean;
edmfade : boolean;
edinfo : boolean;
edainfo : boolean;
edani : boolean;
edobj : boolean;
edcur : boolean;
edall : boolean;
ed32 : boolean;
edmov : word;
edpozx,edpozy : word;
edepozx,edepozy : word;
edmaxpozx,edmaxpozy : word;
edcell : byte;
inftype : byte;
{}
ef1,ef2,ef3 : byte;
def1 : shortint;
{}
cl1,cl2 : longint;
fps : real;
{}
hon : boolean;
{}
scripts : array [0..maxscripts-1] of tscript;
sl : word;
asl : word;
curspt,curspta : word;
{}
trapguns : tguns;

procedure renderall; forward;

procedure val2pchar (vall : longint; var p : pchar);
var
s : string [9];
begin
     str (vall,s);
     strpcopy (p,s);
end;

procedure word2hex (wordd : word; var p : pchar);
const
hextbl : array [0..15] of char = ('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F');
var
s : string[4];
begin
     s[0] := #4;
     s[4] := hextbl [lo (wordd) and $f];
     s[3] := hextbl [lo (wordd) shr 4];
     s[2] := hextbl [hi (wordd) and $f];
     s[1] := hextbl [hi (wordd) shr 4];
     strpcopy (p,s);
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
     hline (x1,y2,x2,pc+20);
     vline (x1,y1,y2,pc+30);
     vline (x2,y1,y2,pc+20);
     end
     else
     if a = 1 then
     begin
     hline (x1,y1,x2,pc+20);
     hline (x1,y2,x2,pc+30);
     vline (x1,y1,y2,pc+20);
     vline (x2,y1,y2,pc+30);
     end
     else
     begin
     hline (x1,y1,x2,pc+24);
     hline (x1,y2,x2,pc+24);
     vline (x1,y1,y2,pc+24);
     vline (x2,y1,y2,pc+24);
     end;
end;

procedure syserror (fnamm,enamm : pchar); far;
var
ccseg,ipr : word;
ppch,ph : pchar;
begin
     getmem (ppch,140);
     getmem (ph,5);
     asm
        mov ax,[bp+4]
        mov bx,[bp+2]
        mov ccseg,ax
        mov ipr,bx
     end;
     if getscr = $13 then begin inittxtscr; clrscr; dec (trapcrtcy); end;
     strcopy (ppch,'Error at ');
     word2hex (cseg,ph);
     strcat (ppch,ph);
     strcat (ppch,':');
     word2hex (ipr,ph);
     strcat (ppch,ph);
     strcat (ppch,' in function ');
     strcat (ppch,fnamm);
     strcat (ppch,': ');
     strcat (ppch,enamm);
     trapcrtprint (0,trapcrtcy+1,7,ppch);
     halt(1);
end;

procedure trapexitproc; far;
begin
     exitproc := oldexitproc;
     if getscr <> $3 then inittxtscr;
     if l then
     unuspack (trappfrec);
end;

procedure loadanim (fn : pchar; var oa : tanimation);
var
ispr,time,noftexs : byte;
i : byte;
parr : array [0..49] of longint;
sx,sy : word;
begin
     assignpfile (trapfilehandle,trappfrec,fn);
     if pfileior <> 0 then syserror ('loadanim()',getpfior);
     resetpfile (trapfilehandle);
     blockreadpfile (trapfilehandle,ispr,1);
     blockreadpfile (trapfilehandle,time,2);
     blockreadpfile (trapfilehandle,noftexs,1);
     createanimation (oa,ispr,noftexs,time);
     blockreadpfile (trapfilehandle,parr,noftexs*4);
     for i := 0 to noftexs-1 do
     begin
          seekpfile (trapfilehandle,parr[i]);
          blockreadpfile (trapfilehandle,animtexts[ianimtexts].sizex,4);
          getmem (animtexts[ianimtexts].texmemory,animtexts[ianimtexts].sizex*animtexts[ianimtexts].sizey);
          blockreadpfile (trapfilehandle,animtexts[ianimtexts].texmemory^,animtexts[ianimtexts].sizex*
          animtexts[ianimtexts].sizey);
          addtextoanim (animtexts[ianimtexts],i,oa);
          inc (ianimtexts);
     end;
     closepfile (trapfilehandle);
end;

procedure load832 (fnamm : pchar; var t : ttex);
var
pbufer : pointer;
pages,modpages : word;
i : word;
begin
     assignpfile (trapfilehandle,trappfrec,fnamm);
     if pfileior <> 0 then syserror ('load832()',getpfior);
     resetpfile (trapfilehandle);
     blockreadpfile (trapfilehandle,t.sizex,4);
     getmem (t.texmemory,t.sizex*t.sizey);
     blockreadpfile (trapfilehandle,t.texmemory^,t.sizex*t.sizey);
     closepfile (trapfilehandle);
end;

procedure resettrapmap;
var
i : word;
p : pointer;
begin
     with trapmap do
     begin
          {fillchar (trapmap^,trapmapsize*6,0);}
          for i := 0 to trapmapsize do
          begin
               trapmap^[i].backtex := 0;
               trapmap^[i].fronttex1 := 0;
               trapmap^[i].fronttex2 := 0;
               trapmap^[i].fadeandwater := 31;
               trapmap^[i].trapinforec1 := 0;
               trapmap^[i].trapinforec2 := 0;
          end;
          p := ptr (trapinforecseg,0);
          fillchar (p^,sizeof (trapinforec)*maxinforecs,0);
          p := ptr (trapscryptseg,0);
          fillchar (p^,scryptslen,0);
          trapmapsizex := 1;
          trapmapsizey := 1;
          nplayers := 0;
          nmonsters := 0;
     end;
     currfade := 31;
     currwater := 0;
     curref := 0;
     currinfo := 0;
     ednet := true;
     edfade := true;
     edmfade := false;
     edinfo := false;
     edainfo := false;
     edani := false;
     edobj := false;
     edcur := true;
     edall := true;
     ed32 := false;
     edmov := 1;
     edpozx := 0;
     edpozy := 0;
     edcell := 0;
     ntrapinfo := 0;
     inftype := 0;
     fillchar (currtrapinforec,sizeof (trapinforec),0);
     fillchar (trapmapfname^,256,0);
     fillchar (trapmapname^,256,0);
     fillchar (midimusicname^,256,0);
     ef1 := 31;
     ef2 := 1;
     ef3 := 1;
     def1 := -1;
end;

function readpcharfromfile (var f : file; pc : pchar) : word;
var
c : char;
i : word;
begin
     c := #255;
     i := 0;
     while c <> #0 do
     begin
          blockread (f,c,1);
          pc [i] := c;
          inc (i);
     end;
     readpcharfromfile := i;
end;

function loadscript (var sname : pchar) : boolean;
const
pc : pchar = 'TRAP SCRIPT';
type
st = array [0..65534] of byte;
pt = ^st;
var
f : file;
pn : pchar;
sn : word;
i,l : word;
code : byte;
ppch : array [0..255] of char;
t : pt;
begin
     loadscript := false;
     assign (f,sname);
     {$I-}
     reset (f,1);
     {$I+}
     if ioresult <> 0 then exit;
     {}
     getmem (pn,strlen (pc)+1);
     blockread (f,pn^,strlen (pc)+1);
     if strcomp (pn,pc) <> 0 then begin freemem (pn,strlen (pc)+1); close (f); exit; end;
     freemem (pn,strlen (pc)+1);
     {}
     blockread (f,sn,2);
     if sn > maxscripts then begin close (f); exit; end;;
     sl := sn;
     if sl > 0 then
     for i := 0 to sl-1 do
     begin
          l := readpcharfromfile (f,ppch);
          getmem (scripts[i].name,l);
          strcopy (scripts[i].name,ppch);
          blockread (f,scripts[i].sa,2);
     end;
     i := 0;
     t := ptr (trapmap.trapscryptseg,0);
     while not eof (f) do
     begin
          blockread (f,code,1);
          t^[i] := code;
          inc (i);
          if i > scryptslen then begin close (f); exit; end;
     end;
     close (f);
     asl := i;
     loadscript := true;
     curspt := 0;
     curspta := scryptslen+1;
end;

function loadmap (var mapname : pchar) : boolean;
var
trapmaphead : pchar;
i : word;
c : char;
t : pointer;
f : file;
begin
     loadmap := false;
     {}
     assign (f,mapname);
     {$I-}
     reset (f,1);
     {$I+}
     if ioresult <> 0 then exit;
     {}
     getmem (trapmaphead,8);
     {$I-}
     blockread (f,trapmaphead^,8);
     {$I+}
     if ioresult <> 0 then exit;
     if strcomp (trapmaphead,'TRAPMAP') <> 0 then exit;
     freemem (trapmaphead,8);
     {}
     resettrapmap;
     fillchar (trapmapname^,256,0);
     fillchar (midimusicname^,256,0);
     i := 0;
     c := #255;
     while (c <> #0) do
     begin
          blockread (f,c,1);
          midimusicname[i] := c;
          inc (i);
     end;
     {}
     i := 0;
     c := #255;
     while (c <> #0) do
     begin
          blockread (f,c,1);
          trapmapname[i] := c;
          inc (i);
     end;
     {}
     with trapmap do
     begin
          blockread (f,trapmapsizex,2);
          blockread (f,trapmapsizey,2);
          {}
          blockread (f,nplayers,1);
          for i := 1 to nplayers do
          begin
               blockread (f,players[i-1].playerpozx,2);
               blockread (f,players[i-1].playerpozy,2);
               blockread (f,players[i-1].state,1);
          end;
          {}
          blockread (f,nmonsters,1);
          for i := 1 to nmonsters do
          begin
               blockread (f,monsters[i-1].monsterpozx,2);
               blockread (f,monsters[i-1].monsterpozy,2);
               blockread (f,monsters[i-1].state,1);
               blockread (f,monsters[i-1].monstertype,1);
          end;
          {}
          {wczytywanie caˆej mapy (poziomu)}
          blockread (f,trapmap^,trapmapsizex*trapmapsizey*6);
          blockread (f,ntrapinfo,1);
          if ntrapinfo > maxinforecs then syserror ('loadmap()','Unable to load map');
          t := ptr (trapinforecseg,0);
          blockread (f,t^,ntrapinfo*sizeof (trapinforec));
          blockread (f,i,2);
          if i > scryptslen then syserror ('loadmap()','Unable to load map');
          t := ptr (trapscryptseg,0);
          blockread (f,t^,i);
          asl := i;
     end;
     curspt := 0;
     curspta := scryptslen+1;
     sl := 0;
     close (f);
     edmaxpozx := trapmap.trapmapsizex*32-1;
     edmaxpozy := trapmap.trapmapsizey*24-1;
     strcopy (trapmapfname,mapname);
end;

procedure readline (var s : pchar; dl : word);
var
pc : char;
i : word;
begin
     i := 0;
     pc := #0;
     while (not pfileeof (trapfilehandle)) and (pc <> #13) and (pc <> #10) and (i < dl) do
     begin
          blockreadpfile (trapfilehandle,pc,1);
          if pfileior <> 0 then syserror ('readline()',getpfior);
          if (pc = #13) or (pc = #10) or (i+1 = dl) then s[i] := #0
          else
          s[i] := pc;
          inc (i);
     end;
     s[i] := #0;
     blockreadpfile (trapfilehandle,pc,1);
end;

procedure loaditems;
var
ppchar : pchar;
ppval : pchar;
i,j,k : byte;
ppfilepos : longint;
pffile,npffile : fileor;
begin
     ntrapitemssprites := 0;
     currtrapitemssprites := 0;
     getmem (ppchar,256);
     getmem (ppval,14);
     assignpfile (trapfilehandle,trappfrec,'ITEMS.TXT');
     if pfileior <> 0 then syserror ('loaditems()',getpfior);
     resetpfile (trapfilehandle);
     i := 0;
     while not pfileeof (trapfilehandle) do
     begin
          readline (ppchar,256);
          k := 0;
          j := 0;
          fillchar (ppval^,14,0);
          while ppchar[k] <> ' ' do
          begin
               inc (k);
          end;
          {}
          inc (k);
          while ppchar[k] <> ' ' do
          begin
               inc (k);
          end;
          {}
          inc (k);
          while ppchar[k] <> ' ' do
          begin
               inc (k);
          end;
          {}
          inc (k);
          while ppchar[k] <> #0 do
          begin
               ppval[j] := ppchar[k];
               inc (j);
               inc (k);
          end;
          ppfilepos := filepos (pf);
          pffile := trapfilehandle;
          npffile := nextfile;
          load832 (ppval,trapitemssprites[i]);
          trapfilehandle := pffile;
          nextfile := npffile;
          pfus := true;
          seek (pf,ppfilepos);
          inc (i);
     end;
     ntrapitemssprites := i;
     freemem (ppval,14);
     freemem (ppchar,256);
     closepfile (trapfilehandle);
end;

procedure loadall832;
var
i : word;
ppfilepos : longint;
pffile,npffile : fileor;
__ppchar : array [0..49] of char;
ppchar : pchar;
begin
     ppchar := @__ppchar;
     assignpfile (trapfilehandle,trappfrec,'FILES832.TXT');
     if pfileior <> 0 then syserror ('loadall832()',getpfior);
     resetpfile (trapfilehandle);
     i := 0;
     ntraptextures := 0;
     ntrapsprites := 0;
     ntrapanimations := 0;
     ntrapmonstersanimations := 0;
     currtraptexture := 0;
     currtrapsprite := 0;
     currtrapanimation := 0;
     currtrapmonsteranimation := 0;
     texstype := 0;
     while not pfileeof (trapfilehandle) do
     begin
          readline (ppchar,256);
          writeln (ppchar);
          if strcomp (ppchar,'[TEXTURES]') = 0 then i := 0
          else
          if strcomp (ppchar,'[SPRITES]') = 0 then i := 1
          else
          if strcomp (ppchar,'[ANIMS]') = 0 then i := 2
          else
          begin
               ppfilepos := filepos (pf);
               pffile := trapfilehandle;
               npffile := nextfile;
               if i = 0 then
               begin
                    load832 (ppchar,traptextures[ntraptextures]);
                    inc (ntraptextures);
               end;
               if i = 1 then
               begin
                    load832 (ppchar,trapsprites[ntrapsprites]);
                    inc (ntrapsprites);
               end;
               if i = 2 then
               begin
                    loadanim (ppchar,trapanimations[ntrapanimations]);
                    inc (ntrapanimations);
               end;
               trapfilehandle := pffile;
               nextfile := npffile;
               pfus := true;
               seek (pf,ppfilepos);
          end;
     end;
end;

procedure loadplayer;
var
i,j,k,l : word;
c : integer;
ppfilepos : longint;
pffile,npffile : fileor;
__ppchar : array [0..255] of char;
ppchar : pchar;
ppchar2 : pchar;
begin
     ppchar := @__ppchar;
     assignpfile (trapfilehandle,trappfrec,'PLAYER.TXT');
     if pfileior <> 0 then syserror ('loadplayer()',getpfior);
     resetpfile (trapfilehandle);
     i := 0;
     j := 0;
     while not pfileeof (trapfilehandle) do
     begin
          readline (ppchar,256);
          if strcomp (ppchar,'[ANIM]') = 0 then i := 0
          else
          if strcomp (ppchar,'[HAND]') = 0 then i := 1
          else
          if strcomp (ppchar,'[LINK]') = 0 then i := 2
          else
          if strcomp (ppchar,'[PHYSICS]') = 0 then i := 3
          else
          if (i = 0) or (i = 1) then
          begin
               ppfilepos := filepos (pf);
               pffile := trapfilehandle;
               npffile := nextfile;
               if i = 0 then loadanim (ppchar,playeranimations);
               if i = 1 then loadanim (ppchar,handanimations);
               trapfilehandle := pffile;
               nextfile := npffile;
               pfus := true;
               seek (pf,ppfilepos);
          end
          else
          if i = 2 then
          begin
               k := 0;
               l := 0;
               getmem (ppchar2,10);
               fillchar (ppchar2^,10,0);
               while ppchar[l] <> ' ' do
               begin
                    ppchar2[k] := ppchar[l];
                    inc (k);
                    inc (l);
                    if k > 10 then syserror ('loadplayer()','Bad PLAYER.TXT file');
               end;
               val (ppchar2,playerlinks[j].mx,c);
               if c <> 0 then syserror ('loadplayer()','Bad PLAYER.TXT file');
               {}
               fillchar (ppchar2^,10,0);
               k := 0;
               while ppchar[l] <> #0 do
               begin
                    ppchar2[k] := ppchar[l];
                    inc (k);
                    inc (l);
                    if k > 10 then syserror ('loadplayer()','Bad PLAYER.TXT file');
               end;
               val (ppchar2,playerlinks[j].my,c);
               if c <> 0 then syserror ('loadplayer()','Bad PLAYER.TXT file');
               freemem (ppchar2,10);
               inc (j);
               if j > nplayerlinks then syserror ('loadplayer()','j > nplayerlinks');
          end
          else
          if i = 3 then
          begin
               k := 0;
               l := 0;
               getmem (ppchar2,10);
               fillchar (ppchar2^,10,0);
               while ppchar[l] <> ' ' do
               begin
                    ppchar2[k] := ppchar[l];
                    inc (k);
                    inc (l);
                    if k > 10 then syserror ('loadplayer()','Bad PLAYER.TXT file');
               end;
               {val (ppchar2,playerphysicobj.physicobjmaxxvel,c);
               if c <> 0 then syserror ('loadplayer()','Bad PLAYER.TXT file');
               {}
               fillchar (ppchar2^,10,0);
               k := 0;
               while ppchar[l] <> #0 do
               begin
                    ppchar2[k] := ppchar[l];
                    inc (k);
                    inc (l);
                    if k > 10 then syserror ('loadplayer()','Bad PLAYER.TXT file');
               end;
               {val (ppchar2,playerphysicobj.physicobjxaccel,c);
               if c <> 0 then syserror ('loadplayer()','Bad PLAYER.TXT file');}
               freemem (ppchar2,10);
               {
               playerphysicobj.physicobjmaxyvel := worldphysicobj.physicobjmaxyvel;
               playerphysicobj.physicobjyaccel := worldphysicobj.physicobjyaccel;}
          end;
     end;
     closepfile (trapfilehandle);
end;

procedure loadguns;
var
ppchar : pchar;
ppval : pchar;
i,j,k : byte;
co : integer;
ppfilepos : longint;
pffile,npffile : fileor;
begin
     getmem (ppchar,256);
     getmem (ppval,14);
     assignpfile (trapfilehandle,trappfrec,'GUNS.TXT');
     if pfileior <> 0 then syserror ('loadguns()',getpfior);
     resetpfile (trapfilehandle);
     i := 0;
     while not pfileeof (trapfilehandle) do
     begin
          readline (ppchar,256);
          k := 0;
          j := 0;
          fillchar (ppval^,14,0);
          while ppchar[k] <> ' ' do
          begin
               ppval[j] := ppchar[k];
               inc (j);
               inc (k);
          end;
          val (ppval,trapguns[i].gapower,co);
          if co <> 0 then syserror ('loadguns()','Bad GUNS.TXT file');
          {}
          inc (k);
          j := 0;
          fillchar (ppval^,14,0);
          while ppchar[k] <> ' ' do
          begin
               ppval[j] := ppchar[k];
               inc (j);
               inc (k);
          end;
          val (ppval,trapguns[i].gnammo,co);
          if co <> 0 then syserror ('loadguns()','Bad GUNS.TXT file');
          {}
          inc (k);
          j := 0;
          fillchar (ppval^,14,0);
          while ppchar[k] <> ' ' do
          begin
               ppval[j] := ppchar[k];
               inc (j);
               inc (k);
          end;
          val (ppval,trapguns[i].gtime,co);
          if co <> 0 then syserror ('loadguns()','Bad GUNS.TXT file');
          {}
          inc (k);
          j := 0;
          fillchar (ppval^,14,0);
          while ppchar[k] <> ' ' do
          begin
               ppval[j] := ppchar[k];
               inc (j);
               inc (k);
          end;
          val (ppval,trapguns[i].gtype,co);
          if co <> 0 then syserror ('loadguns()','Bad GUNS.TXT file');
          {}
          inc (k);
          j := 0;
          fillchar (ppval^,14,0);
          while ppchar[k] <> ' ' do
          begin
               ppval[j] := ppchar[k];
               inc (j);
               inc (k);
          end;
          val (ppval,trapguns[i].gatype,co);
          if co <> 0 then syserror ('loadguns()','Bad GUNS.TXT file');
          {}
          inc (k);
          j := 0;
          fillchar (ppval^,14,0);
          while ppchar[k] <> ' ' do
          begin
               ppval[j] := ppchar[k];
               inc (j);
               inc (k);
          end;
          val (ppval,trapguns[i].gextra,co);
          if co <> 0 then syserror ('loadguns()','Bad GUNS.TXT file');
          {}
          inc (k);
          j := 0;
          fillchar (ppval^,14,0);
          while ppchar[k] <> #0 do
          begin
               ppval[j] := ppchar[k];
               inc (j);
               inc (k);
          end;
          ppfilepos := filepos (pf);
          pffile := trapfilehandle;
          npffile := nextfile;
          loadanim (ppval,trapgunssprites[i]);
          trapfilehandle := pffile;
          nextfile := npffile;
          pfus := true;
          seek (pf,ppfilepos);
          inc (i);
     end;
     if i < maxguns then syserror ('loadguns()','i < maxguns');
     ntrapgunssprites := i;
     freemem (ppval,14);
     freemem (ppchar,256);
     closepfile (trapfilehandle);
end;


procedure loadfilesfrompak;
begin
     writeln ('LOADING...');
     if not uspack (trappfrec,pakname) then halt (1);
     getmem (txttbl,11200);
     assignpfile (trapfilehandle,trappfrec,'TXTTBL.ASC');
     if pfileior <> 0 then syserror ('main()',getpfior);
     resetpfile (trapfilehandle);
     blockreadpfile (trapfilehandle,txttbl^,11200);
     closepfile (trapfilehandle);
     textdefaults;
     ianimtexts := 0;
     loaditems;
     loadguns;
     loadplayer;
     loadall832;
     {}
     unuspack (trappfrec);
end;

procedure inittrapmap;
var
p : pointer;
begin
     with trapmap do
     begin
          getmem (trapmap,trapmapsize*6);
          getmem (p,sizeof (trapinforec)*maxinforecs);
          trapinforecseg := seg (p^);
          getmem (p,scryptslen);
          trapscryptseg := seg (p^);
     end;
     getmem (trapmapfname,256);
     getmem (trapmapname,256);
     getmem (midimusicname,256);
     resettrapmap;
end;

procedure initialize;
var
pp : pointer;
begin
     clrscr;
     oldexitproc := exitproc;
     exitproc := @trapexitproc;
     l := true;
     loadfilesfrompak;
     l := false;
     getmem (buf,65535);
     if ofs (buf^) <> 0 then
     begin
          getmem (pp,16);
          buf := ptr (seg (buf^)+1,0);
     end;
     inittrapmap;
     initvgascr;
     set_8_32_pal;
     setpalette;
     pscx := 320;
     pscy := 200;
     hon := false;
     randomize;
end;

procedure savemap (fnamm : pchar);
const
trapmaphead : pchar = 'TRAPMAP';
type
at = array [0..maxinforecs-1] of trapinforec;
pt = ^at;
var
f : file;
i : word;
t : pt;
begin
     assign (f,fnamm);
     rewrite (f,1);
     blockwrite (f,trapmaphead^,strlen (trapmaphead)+1);
     blockwrite (f,midimusicname^,strlen (midimusicname)+1);
     blockwrite (f,trapmapname^,strlen (trapmapname)+1);
     with trapmap do
     begin
          blockwrite (f,trapmapsizex,2);
          blockwrite (f,trapmapsizey,2);
          blockwrite (f,nplayers,1);
          for i := 1 to nplayers do
          begin
               blockwrite (f,players[i-1].playerpozx,2);
               blockwrite (f,players[i-1].playerpozy,2);
               blockwrite (f,players[i-1].state,1);
          end;
          blockwrite (f,nmonsters,1);
          for i := 1 to nmonsters do
          begin
               blockwrite (f,monsters[i-1].monsterpozx,2);
               blockwrite (f,monsters[i-1].monsterpozx,2);
               blockwrite (f,monsters[i-1].state,1);
               blockwrite (f,monsters[i-1].monstertype,1);
          end;
          blockwrite (f,trapmap^,trapmapsizex*trapmapsizey*6);
          blockwrite (f,ntrapinfo,1);
          t := ptr (trapinforecseg,0);
          blockwrite (f,t^,ntrapinfo*sizeof (trapinforec));
          i := asl;
          blockwrite (f,i,2);
          t := ptr (trapscryptseg,0);
          blockwrite (f,t^,i);
     end;
     strcopy (trapmapfname,fnamm);
end;

procedure setani;
var
i : byte;
begin
     edani := not edani;
     for i := 0 to ntrapanimations-1 do setcuranim (trapanimations[i],0);
end;

procedure processani;
var
i : byte;
begin
     if edani then
     for i := 0 to ntrapanimations-1 do
     if frames mod trapanimations[i]^.animtime = trapanimations[i]^.animtime-1 then
     setcuranim (trapanimations[i],trapanimations[i]^.curanim+1);
end;

procedure idval (var n : byte; l : byte; k : shortint);
begin
     if l = 0 then exit;
     if ((n = 0) and (k < 0)) then
     begin
          n := l;
          exit;
     end;
     n := n+k;
     if n > l then n := 0;
end;

procedure widval (var n : word; l : word; k : shortint);
begin
     if l = 0 then exit;
     if ((n = 0) and (k < 0)) then
     begin
          n := l;
          exit;
     end;
     n := n+k;
     if n > l then n := 0;
end;


procedure idvalbit (var n : byte; l : byte; k : shortint; b1,b2 : byte);
var
pv,pv2,pv3,i : byte;
begin
     if l = 0 then exit;
     pv3 := 1;
     for i := 1 to b2 do
     begin
          pv3 := pv3*2;
     end;
     dec (pv3);
     pv := (n shr b1) and pv3;
     if ((n = 0) and (k < 0)) then
     begin
          pv := l;
     end
     else
     begin
          pv := pv+k;
          if pv > l then pv := 0;
     end;
     pv2 := not (pv3 shl b1);
     n := n and pv2;
     n := n or ((pv and pv3) shl b1);
end;

procedure idedpoz (x,y : integer);
var
px,py : longint;
begin
     px := edpozx+x;
     py := edpozy+y;
     if px < 0 then px := 0;
     if px > edmaxpozx then px := edmaxpozx;
     if py < 0 then py := 0;
     if py > edmaxpozy then py := edmaxpozy;
     edpozx := px;
     edpozy := py;
     px := longint (edpozx)-160;
     py := longint (edpozy)-72;
     if px < 0 then px := 0;
     if py < 0 then py := 0;
     if px > longint (edmaxpozx)-319 then px := longint (edmaxpozx)-319;
     if py > longint (edmaxpozy)-141 then py := longint (edmaxpozy)-141;
     if px < 0 then px := 0;
     if py < 0 then py := 0;
     edepozx := px;
     edepozy := py;
end;

procedure setcell (x,y : word; n,v : byte);
type
st = array [0..65534] of byte;
pt = ^st;
var
p : pt;
pv : word;
begin
     if x >= trapmap.trapmapsizex then exit;
     if y >= trapmap.trapmapsizey then exit;
     p := pointer (trapmap.trapmap);
     pv := (y*trapmap.trapmapsizex+x)*6+n;
     if pv < trapmap.trapmapsizex*trapmap.trapmapsizey*6 then
     p^[pv] := v;
end;

function getcell (x,y : word; n : byte) : byte;
type
st = array [0..65534] of byte;
pt = ^st;
var
p : pt;
pv : word;
begin
     getcell := 0;
     if x >= trapmap.trapmapsizex then exit;
     if y >= trapmap.trapmapsizey then exit;
     p := pointer (trapmap.trapmap);
     pv := (y*trapmap.trapmapsizex+x)*6+n;
     if pv < trapmap.trapmapsizex*trapmap.trapmapsizey*6 then
     getcell := p^[pv];
end;

procedure setall (n,v : byte);
var
i,j : word;
begin
     for j := 0 to trapmap.trapmapsizey-1 do
     for i := 0 to trapmap.trapmapsizex-1 do setcell (i,j,n,v);
end;

procedure drawcur;
begin
     putpixeltranspar (edpozx-edepozx,edpozy-edepozy-1,31);
     putpixeltranspar (edpozx-edepozx+1,edpozy-edepozy,31);
     putpixeltranspar (edpozx-edepozx,edpozy-edepozy+1,31);
     putpixeltranspar (edpozx-edepozx-1,edpozy-edepozy,31);
end;

procedure addinfo (x,y : word; pozx,pozy : byte; sc : word; s1,s2,c,d : byte);
type
at = array [0..maxinforecs-1] of trapinforec;
pt = ^at;
var
pc : byte;
cell,pcell : byte;
t : pt;
i,j : word;
begin
     t := ptr (trapmap.trapinforecseg,0);
     if c = 1 then pc := 4
     else
     if c = 2 then pc := 5
     else
     exit;
     if currinfo > maxinforecs then
     begin
          if d = 0 then setcell (x,y,pc,currinfo)
          else
          if d = 1 then setcell (x,y,pc,0);
          exit;
     end;
     cell := getcell (x,y,pc);
     if d = 0 then
     begin
     if (cell = 0) then
     begin
          if ntrapinfo >= maxinforecs then exit;
          inc (ntrapinfo);
          setcell (x,y,pc,ntrapinfo);
          with t^[ntrapinfo-1] do
          begin
               objectpozx := pozx;
               objectpozy := pozy;
               objectscrypt := sc;
               objectstate := s1;
               objectextra := s2;
          end;
     end
     else
     begin
          with t^[cell-1] do
          begin
               objectpozx := pozx;
               objectpozy := pozy;
               objectscrypt := sc;
               objectstate := s1;
               objectextra := s2;
          end;
     end;
     end
     else
     begin
     if currinfo > maxinforecs then setcell (x,y,pc,0);
     if (cell <= maxinforecs) and (ntrapinfo > 0) and (d = 1) then
     begin
          if cell > ntrapinfo then syserror ('addinfo()','cell > ntrapinfo');
          if cell < ntrapinfo then
          for i := cell to ntrapinfo-1 do move (t^[i],t^[i-1],sizeof (trapinforec));
          for j := 0 to trapmap.trapmapsizey-1 do
          for i := 0 to trapmap.trapmapsizex-1 do
          begin
               pcell := getcell (i,j,4);
               if (pcell > cell) and (pcell <= maxinforecs) then setcell (i,j,4,pcell-1);
               pcell := getcell (i,j,5);
               if (pcell > cell) and (pcell <= maxinforecs) then setcell (i,j,5,pcell-1);
          end;
          dec (ntrapinfo);
          setcell (x,y,pc,0);
     end;
     end;
end;

procedure setinfo (n,pc : byte);
type
at = array [0..maxinforecs-1] of trapinforec;
pt = ^at;
var
t : pt;
begin
     setcell (edpozx div 32,edpozy div 24,pc,n);
     if (n = 0) or (n > maxinforecs) then exit;
     t := ptr (trapmap.trapinforecseg,0);
     with t^[n-1] do
     begin
          objectpozx := currtrapinforec.objectpozx;
          objectpozy := currtrapinforec.objectpozy;
          objectscrypt := currtrapinforec.objectscrypt;
          objectstate := currtrapinforec.objectstate;
          objectextra := currtrapinforec.objectextra;
     end;
end;

procedure addtex (x,y : word; c,t,typee : byte);
var
i,j,sx,sy : byte;
begin
     if c = 0 then
     begin
          if getcell (x,y,c) and 127 <> 127 then setcell (x,y,c,getcell (x,y,c) and 128+t) else exit;
     end
     else
     begin
          if getcell (x,y,c) <> 255 then setcell (x,y,c,t) else exit;
     end;
     if (t = 0) or (t = 64) or (t = 128) or (t = 192) or (t = 224) then exit;
     sx := 0;
     sy := 0;
     if typee = 0 then
     begin
          sx := traptextures[t-1].sizex div 33;
          sy := traptextures[t-1].sizey div 25;
     end
     else
     if typee = 1 then
     begin
          sx := trapsprites[t-65].sizex div 33;
          sy := trapsprites[t-65].sizey div 25;
     end
     else
     if typee = 2 then
     begin
          sx := getanimation (trapanimations[t-129])^.sizex div 33;
          sy := getanimation (trapanimations[t-129])^.sizey div 25;
     end;
     if (sx > 0) or (sy > 0) then
     begin
          for j := 0 to sy do
          for i := 0 to sx do if (i+j <> 0) then if c = 0 then
          setcell (i+x,j+y,c,127+getcell (i+x,j+y,c) and 128)
          else
          setcell (i+x,j+y,c,255)
     end;
     if edinfo then addinfo (x,y,edpozx mod 32,edpozy mod 24,
     curspta,currtrapinforec.objectstate,currtrapinforec.objectextra,c,0);
end;

procedure deltex (x,y : word; c : byte);
var
cc,i,j,sx,sy,inf : byte;
begin
     if c = 0 then
     cc := getcell (x,y,c) and 127
     else
     cc := getcell (x,y,c);
     if c = 0 then if (cc = 0) or (cc = 127) then exit;
     if (cc = 0) or (cc = 64) or (cc = 128) or (cc = 255) then exit;
     sx := 0;
     sy := 0;
     if cc < 64 then
     begin
          sx := traptextures[cc-1].sizex div 33;
          sy := traptextures[cc-1].sizey div 25;
     end
     else
     if cc < 128 then
     begin
          sx := trapsprites[cc-65].sizex div 33;
          sy := trapsprites[cc-65].sizey div 25;
     end
     else
     if cc < 192 then
     begin
          sx := getanimation (trapanimations[cc-129])^.sizex div 33;
          sy := getanimation (trapanimations[cc-129])^.sizey div 25;
     end;
     for j := 0 to sy do
     for i := 0 to sx do if c = 0 then
     setcell (i+x,j+y,c,getcell (i+x,j+y,c) and 128)
     else
     setcell (i+x,j+y,c,0);
     inf := 0;
     if (c = 1) or (c = 2) then
     begin
          inf := getcell (x,y,c+3);
          if (inf > 0) and (inf <= maxinforecs)then addinfo (edpozx div 32,edpozy div 24,edpozx mod 32,edpozy mod 24,0,0,0,c,1)
          else
          setcell (x,y,c+3,0);
     end;
end;

procedure getcellval (x,y : word; c : byte);
var
gc : byte;
begin
     gc := getcell (x,y,c);
     if c = 0 then
     begin
          if gc <> 127 then currtraptexture := gc and 127;
     end
     else
     if (c = 1) or (c = 2) then
     begin
          if gc < 64 then currtraptexture := gc
          else
          if gc < 128 then currtrapsprite := gc-64
          else
          if gc < 192 then currtrapanimation := gc-128
          else
          if gc < 224 then currtrapitemssprites := gc-192
          else
          if gc < 255 then currtrapgunssprites := gc-224
     end
     else
     if (c = 3) then
     begin
          currfade := gc and 31;
          currwater := (gc shr 5) and 3;
          curref := getcell (x,y,0) shr 7+(gc shr 7)*2;
     end;
end;

procedure setalltex (n,v,t : byte);
var
i,j : word;
begin
     for j := 0 to trapmap.trapmapsizey-1 do
     for i := 0 to trapmap.trapmapsizex-1 do deltex (i,j,n);
     if v = 0 then exit;
     for j := 0 to trapmap.trapmapsizey-1 do
     for i := 0 to trapmap.trapmapsizex-1 do addtex (i,j,n,v,t);
end;

procedure setalle1;
var
i,j : word;
begin
     for j := 0 to trapmap.trapmapsizey-1 do
     for i := 0 to trapmap.trapmapsizex-1 do setcell (i,j,0,getcell (i,j,0) and 127+(curref and 1) shl 7);
end;

procedure addplayer;
var
px,py : word;
begin
     if ed32 = false then
     begin
          trapmap.players[0].playerpozx := edpozx;
          trapmap.players[0].playerpozy := edpozy;
          trapmap.nplayers := 1;
     end
     else
     begin
          px := ((edpozx div 32)*32)+16;
          py := ((edpozy div 24)*24)+24;
          setcuranim (playeranimations,playeranimtab[trapmap.players[0].state]);
          trapmap.players[0].playerpozx := px-(getanimation (playeranimations)^.sizex div 2);
          trapmap.players[0].playerpozy := py-getanimation (playeranimations)^.sizey;
          trapmap.nplayers := 1;
     end;
end;

procedure drawplayer (x,y : integer; a : byte);
var
hdirection : shortint;
begin
     setcuranim (playeranimations,a);
     hdirection := directiontab [a];
     if hdirection = 1 then setcuranim (handanimations,1)
     else
     if hdirection = -1 then setcuranim (handanimations,2);
     scrsprtexnf (x+playerlinks[a*2+1].mx,y+playerlinks[a*2+1].my,getanimation (handanimations)^);
     scrsprtexnf (x,y,getanimation (playeranimations)^);
     if hdirection = 1 then setcuranim (handanimations,0)
     else
     if hdirection = -1 then setcuranim (handanimations,3);
     scrsprtexnf (x+playerlinks[a*2].mx,y+playerlinks[a*2].my,getanimation (handanimations)^);
end;

procedure drawmap;
type
at = array [0..maxinforecs-1] of trapinforec;
pt = ^at;
var
i,j : integer;
p1,p2,p3,p4 : word;
c,f,ef,wat,info,px,py : byte;
p : boolean;
t : pt;
st : byte;
begin
     pscx := 320;
     pscy := 142;
     p1 := edepozx mod 32;
     p2 := edepozy mod 24;
     p3 := trapmap.trapmapsizey-1;
     p4 := trapmap.trapmapsizex-1;
     if p3 > 6 then p3 := 6;
     if p4 > 10 then p4 := 10;
     t := ptr (trapmap.trapinforecseg,0);
     {0}
     p := true;
     if not edall then if edcell <> 0 then p := false;
     if p then
     for j := -1 to p3 do
     for i := -1 to p4 do
     begin
          c := getcell (i+edepozx div 32,j+edepozy div 24,0) and 127;
          if edfade then
          begin
               f := getcell (i+edepozx div 32,j+edepozy div 24,3) and 31;
               ef:=getcell (i+edepozx div 32,j+edepozy div 24,0) shr 7+(getcell (i+edepozx div 32,j+edepozy div 24,3) shr 7)*2;
               if ef = 1 then f := ef1-(31-f);
               if ef = 2 then if ef2 = 0 then f := 0;
               if ef = 3 then f := f-ef3;
          end
          else f := 31;
          if (c <> 0) and (c <> 255) then
          scrtexf (i*32-p1,j*24-p2,f,traptextures[c-1]);
     end;
     {1}
     p := true;
     if not edall then if edcell <> 1 then p := false;
     if p then
     for j := -1 to p3 do
     for i := -1 to p4 do
     begin
          c := getcell (i+edepozx div 32,j+edepozy div 24,1);
          if edfade then
          begin
               f := getcell (i+edepozx div 32,j+edepozy div 24,3) and 31;
               ef:=getcell (i+edepozx div 32,j+edepozy div 24,0) shr 7+(getcell (i+edepozx div 32,j+edepozy div 24,3) shr 7)*2;
               if ef = 1 then f := ef1-(31-f);
               if ef = 2 then if ef2 = 0 then f := 0;
               if ef = 3 then f := f-ef3;
          end
          else f := 31;
          info := getcell (i+edepozx div 32,j+edepozy div 24,4);
          if (info > 0) and (info <= maxinforecs) then
          begin
               px := t^[info-1].objectpozx;
               py := t^[info-1].objectpozy;
               st := t^[info-1].objectstate;
               if (st shr 1) and 1 = 1 then
               begin
                    f := (st shr 3) and 31;
               end;
          end
          else
          begin
               px := 0;
               py := 0;
               st := 0;
          end;
          if (st shr 2) and 1 = 1 then
          begin
               if (c < 64) and (c <> 0) and (c <> 255) then
               scrtextf (i*32-p1+px,j*24-p2+py,f,traptextures[c-1])
               else
               if (c < 128) and (c <> 64) and (c <> 255) then
               scrsprtextf (i*32-p1+px,j*24-p2+py,f,trapsprites[c-65])
               else
               if (c < 192) and (c <> 128) and (c <> 255) then
               scrsprtextf (i*32-p1+px,j*24-p2+py,f,getanimation (trapanimations[c-129])^)
               else
               if (c < 224) and (c <> 192) and (c <> 255) then
               scrsprtextf (i*32-p1+(16-trapitemssprites[c-193].sizex div 2),j*24-p2+(24-trapitemssprites[c-193].sizey)
               ,f,trapitemssprites[c-193])
               else
               if (c <> 224) and (c < 255) then
               scrsprtextf (i*32-p1+(16-getanimation (trapgunssprites[c-225])^.sizex div 2),
               j*24-p2+(24-getanimation (trapgunssprites[c-225])^.sizey),f,getanimation (trapgunssprites[c-225])^);
          end
          else
          begin
               if (c < 64) and (c <> 0) and (c <> 255) then
               scrtexf (i*32-p1+px,j*24-p2+py,f,traptextures[c-1])
               else
               if (c < 128) and (c <> 64) and (c <> 255) then
               scrsprtexf (i*32-p1+px,j*24-p2+py,f,trapsprites[c-65])
               else
               if (c < 192) and (c <> 128) and (c <> 255) then
               scrsprtexf (i*32-p1+px,j*24-p2+py,f,getanimation (trapanimations[c-129])^)
               else
               if (c < 224) and (c <> 192) and (c <> 255) then
               scrsprtexf (i*32-p1+(16-trapitemssprites[c-193].sizex div 2),j*24-p2+(24-trapitemssprites[c-193].sizey)
               ,f,trapitemssprites[c-193])
               else
               if (c <> 224) and (c < 255) then
               scrsprtexf (i*32-p1+(16-getanimation (trapgunssprites[c-225])^.sizex div 2),
               j*24-p2+(24-getanimation (trapgunssprites[c-225])^.sizey),f,getanimation (trapgunssprites[c-225])^);
          end;
     end;
     {2}
     p := true;
     if not edall then if edcell <> 2 then p := false;
     if p then
     for j := -1 to p3 do
     for i := -1 to p4 do
     begin
          c := getcell (i+edepozx div 32,j+edepozy div 24,2);
          if edfade then
          begin
               f := getcell (i+edepozx div 32,j+edepozy div 24,3) and 31;
               ef:=getcell (i+edepozx div 32,j+edepozy div 24,0) shr 7+(getcell (i+edepozx div 32,j+edepozy div 24,3) shr 7)*2;
               if ef = 1 then f := ef1-(31-f);
               if ef = 2 then if ef2 = 0 then f := 0;
               if ef = 3 then f := f-ef3;
          end
          else f := 31;
          info := getcell (i+edepozx div 32,j+edepozy div 24,5);
          if (info > 0) and (info <= maxinforecs) then
          begin
               px := t^[info-1].objectpozx;
               py := t^[info-1].objectpozy;
               st := t^[info-1].objectstate;
               if (st shr 1) and 1 = 1 then
               begin
                    f := (st shr 3) and 31;
               end;
          end
          else
          begin
               px := 0;
               py := 0;
               st := 0;
          end;
          if (st shr 2) and 1 = 1 then
          begin
               if (c < 64) and (c <> 0) and (c <> 255) then
               scrtextf (i*32-p1+px,j*24-p2+py,f,traptextures[c-1])
               else
               if (c < 128) and (c <> 64) and (c <> 255) then
               scrsprtextf (i*32-p1+px,j*24-p2+py,f,trapsprites[c-65])
               else
               if (c < 192) and (c <> 128) and (c <> 255) then
               scrsprtextf (i*32-p1+px,j*24-p2+py,f,getanimation (trapanimations[c-129])^)
               else
               if (c < 224) and (c <> 192) and (c <> 255) then
               scrsprtextf (i*32-p1+(16-trapitemssprites[c-193].sizex div 2),j*24-p2+(24-trapitemssprites[c-193].sizey)
               ,f,trapitemssprites[c-193])
               else
               if (c <> 224) and (c < 255) then
               scrsprtextf (i*32-p1+(16-getanimation (trapgunssprites[c-225])^.sizex div 2),
               j*24-p2+(24-getanimation (trapgunssprites[c-225])^.sizey),f,getanimation (trapgunssprites[c-225])^);
          end
          else
          begin
               if (c < 64) and (c <> 0) and (c <> 255) then
               scrtexf (i*32-p1+px,j*24-p2+py,f,traptextures[c-1])
               else
               if (c < 128) and (c <> 64) and (c <> 255) then
               scrsprtexf (i*32-p1+px,j*24-p2+py,f,trapsprites[c-65])
               else
               if (c < 192) and (c <> 128) and (c <> 255) then
               scrsprtexf (i*32-p1+px,j*24-p2+py,f,getanimation (trapanimations[c-129])^)
               else
               if (c < 224) and (c <> 192) and (c <> 255) then
               scrsprtexf (i*32-p1+(16-trapitemssprites[c-193].sizex div 2),j*24-p2+(24-trapitemssprites[c-193].sizey)
               ,f,trapitemssprites[c-193])
               else
               if (c <> 224) and (c < 255) then
               scrsprtexf (i*32-p1+(16-getanimation (trapgunssprites[c-225])^.sizex div 2),
               j*24-p2+(24-getanimation (trapgunssprites[c-225])^.sizey),f,getanimation (trapgunssprites[c-225])^);
          end;
     end;
     for i := 1 to trapmap.nplayers do
     with trapmap.players[i-1] do
     begin
          drawplayer (playerpozx-edepozx,playerpozy-edepozy,playeranimtab[state]);
     end;
     for j := 0 to p3 do
     for i := 0 to p4 do
     begin
          wat := (getcell (i+edepozx div 32,j+edepozy div 24,3) shr 5) and 3;
          if wat <> 0 then
          scrtranspwin (i*32-p1,j*24-p2,i*32+32-p1,j*24+24-p2,watertab [wat]);
     end;
     pscx := 320;
     pscy := 200;
end;

procedure showmap;
type
at = array [0..maxinforecs-1] of trapinforec;
pt = ^at;
var
i,j : word;
p1,p2,p3,p4 : word;
c,f,wat,info,st : byte;
t : pt;
begin
     t := ptr (trapmap.trapinforecseg,0);
     buftoclr;
     p3 := trapmap.trapmapsizex-1;
     p4 := trapmap.trapmapsizey-1;
     {}
     if p3 < 319 then p1 := 0
     else
     begin
          p1 := p3-319;
          p3 := 319;
     end;
     {}
     if p4 < 199 then p2 := 0
     else
     begin
          p2 := p4-199;
          p4 := 199;
     end;
     if edmfade then
     begin
     for j := 0 to p4 do
     for i := 0 to p3 do
     begin
          f := getcell (i+p1,j+p2,3) and 31+96;
          putpixel (i,j,f);
     end;
     end
     else
     begin
     {0}
     for j := 0 to p4 do
     for i := 0 to p3 do
     begin
          c := getcell (i+p1,j+p2,0) and 127;
          if c <> 0 then
          putpixel (i,j,110);
     end;
     {1}
     for j := 0 to p4 do
     for i := 0 to p3 do
     begin
          c := getcell (i+p1,j+p2,1);
          if (c <> 0) and (c <> 64) and (c <> 128) and (c <> 192) and (c <> 224) then
          begin
          info := getcell (i+p1,j+p2,4);
          if (info > 0) and (info <= maxinforecs) then
          begin
               st := t^[info-1].objectstate;
               if st and 1 = 0 then f := 112 else f := 120;
          end
          else
          if info = 255 then f := 112
          else
          if info = 254 then f := 114
          else
          if info = 253 then f := 116
          else
          if info = 252 then f := 31
          else
          if info = 251 then f := 95
          else
          if info = 250 then f := 159
          else
          f := 120;
          putpixel (i,j,f);
          end;
     end;
     {2}
     for j := 0 to p4 do
     for i := 0 to p3 do
     begin
          c := getcell (i+p1,j+p2,2);
          if (c <> 0) and (c <> 64) and (c <> 128) and (c <> 192) and (c <> 224) then
          begin
          info := getcell (i+p1,j+p2,5);
          if (info > 0) and (info <= maxinforecs) then
          begin
               st := t^[info-1].objectstate;
               if st and 1 = 0 then f := 113 else f := 121;
          end
          else
          if info = 255 then f := 113
          else
          if info = 254 then f := 115
          else
          if info = 253 then f := 116
          else
          if info = 252 then f := 31
          else
          if info = 251 then f := 95
          else
          if info = 250 then f := 159
          else
          f := 121;
          putpixel (i,j,f);
          end;
     end;
     for j := 0 to p4 do
     for i := 0 to p3 do
     begin
          wat := (getcell (i,j,3) shr 5) and 3;
          if wat <> 0 then
          putpixeltranspar (i,j,watertab [wat]-4);
     end;
     end;
     if edcur then
     putpixel (edpozx div 32,edpozy div 24,31);
     buftoscr;
     while not readkey (kc,ks) do;
end;

procedure inputbox (x1,y1,x2,y2 : integer; var out : pchar; tit : pchar);
var
txnchars : word;
txpos : word;
txppos,txepos : word;
p : pchar;
i,j : word;
begin
     txnchars := (x2-x1-4) div 6-1;
     txpos := 0;
     txppos := 0;
     txepos := 0;
     getmem (p,256);
     repeat
           buftoclr;
           if readkey (kc,ks) then
           begin
                if (kc <> #0) and (kc <> #27) and (kc <> #8) and (kc <> #13) then
                begin
                     if txpos < 256 then
                     if txpos = strlen (out) then
                     out[txpos] := upcase (kc)
                     else
                     begin
                          j := strlen (out)-1;
                          for i := j downto txpos do out [i+1] := out [i];
                          out [j+2] := #0;
                          out[txpos] := upcase (kc)
                     end;
                     inc (txpos);
                     if txpos > 255 then txpos := 255
                     else
                     begin
                          if txppos >= txnchars then inc (txepos)
                          else
                          inc (txppos);
                     end;
                end
                else
                begin
                     if ks = 75 then
                     begin
                          if txpos > 0 then
                          begin
                               if (txpos > txepos) then dec (txppos)
                               else dec (txepos);
                               dec (txpos);
                          end;
                     end;
                     if ks = 77 then
                     begin
                          if txpos < strlen (out) then
                          begin
                               if (txppos >= txnchars) then inc (txepos)
                               else inc (txppos);
                               inc (txpos);
                          end;
                     end;
                     if ks = 71 then
                     begin
                          txpos := 0;
                          txppos := 0;
                          txepos := 0;
                     end;
                     if ks = 79 then
                     begin
                          txpos := strlen (out);
                          if txpos >= txnchars then txppos := txnchars
                          else
                          txppos := txpos;
                          txepos := txpos-txppos;
                     end;
                     if (ks = 83) and (txpos < strlen (out)) then
                     begin
                          j := strlen (out);
                          for i := txpos to j do out [i] := out [i+1];
                     end;
                     if (ks = 14) and (txpos > 0) then
                     begin
                          if txpos = strlen (out) then
                          out [txpos-1] := #0
                          else
                          begin
                               j := strlen (out);
                               for i := txpos-1 to j do out [i] := out [i+1];
                          end;
                          if (txpos > txepos) then dec (txppos)
                          else dec (txepos);
                          dec (txpos);
                     end;
                end;
           end;
           renderall;
           textsetcolor (96);
           win3db (x1,y1,x2,y2,3,0);
           i := y1+(y2-y1-8) div 2;
           textprint (x1+((x2-x1) div 2)-(strlen (tit)*3),y1+2,0,1,tit);
           win3db (x1+2,i,x2-2,i+8,3,1);
           strcopy (p,@out[txepos]);
           p[txnchars] := #0;
           textprint (x1+4,i+2,0,1,p);
           if frames mod 20 > 10 then vline (((txppos)*6)+x1+4,i+2,i+7,0);
           wfvr;
           buftoscr;
           textsetcolor (127);
           inc (frames);
     until (ks = 1) or (ks = 28);
     if ks = 1 then fillchar (out^,strlen (out),0);
     freemem (p,256);
end;

procedure animefects;
begin
     ef1 := ef1+def1;
     if ef1 = 0 then def1 := 1;
     if ef1 = 31 then def1 := -1;
     if frames mod 4 = 2 then ef2 := random (2);
     if frames mod 4 = 2 then ef3 := 3-(ef2*3);
end;

procedure newmap;
var
ppchar1,ppchar2 : pchar;
ok : boolean;
px,py : word;
c1,c2 : integer;
begin
     getmem (ppchar1,256);
     getmem (ppchar2,256);
     resettrapmap;
     ok := false;
     repeat
           fillchar (ppchar1^,256,0);
           inputbox (96,60,224,100,ppchar1,'PODAJ SZEROKOSC MAPY');
           val (ppchar1,px,c1);
           if (c1 = 0) and (px < 1000) and (px > 0) then ok := true;
     until ok;
     ok := false;
     repeat
           fillchar (ppchar2^,256,0);
           inputbox (96,60,224,100,ppchar2,'PODAJ WYSOKOSC MAPY');
           val (ppchar2,py,c2);
           if (c2 = 0) and (longint (px)*longint (py)*6 < 65535) and (py < 1000) and (py > 0) then ok := true;
     until ok;
     trapmap.trapmapsizex := px;
     trapmap.trapmapsizey := py;
     edmaxpozx := px*32-1;
     edmaxpozy := py*24-1;
     freemem (ppchar2,256);
     freemem (ppchar1,256);
end;

procedure saveloadmap (n : byte);
var
fnamm : pchar;
begin
     getmem (fnamm,256);
     fillchar (fnamm^,256,0);
     inputbox (66,60,254,100,fnamm,'PODAJ NAZWE PLIKU');
     if fnamm[0] <> #0 then
     begin
          if n = 0 then savemap (fnamm);
          if n = 1 then loadmap (fnamm);
     end;
     freemem (fnamm,256);
end;

procedure loadscriptib;
var
fnamm : pchar;
ok : boolean;
begin
     getmem (fnamm,256);
     ok := false;
     fillchar (fnamm^,256,0);
     while ok = false do
     begin
     inputbox (66,60,254,100,fnamm,'PODAJ NAZWE PLIKU ZE SKRYPTEM');
     if fnamm[0] <> #0 then
     begin
          if loadscript (fnamm) then ok := true else begin ok := false; fillchar (fnamm^,256,0); end;
     end
     else
     ok := true;
     end;
     freemem (fnamm,256);
end;

procedure renderall;
var
ppchar1,ppchar2 : pchar;
i,j,p1,p2 : word;
ginfr : trapinforec;
pp : pointer;
begin
     ppchar1 := nil;
     ppchar2 := nil;
     getmem (ppchar1,128);
     getmem (ppchar2,9);
     rect (0,142,319,199,120);
     {}
     if texstype = 0 then
     begin
     vline (67,142,199,120);
     strcopy (ppchar1,'TEX:');
     val2pchar (currtraptexture,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar (ntraptextures,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (2,144,0,1,ppchar1);
     if (ntraptextures <> 0) and (currtraptexture <> 0) then scrtexnf (2,150,traptextures[currtraptexture-1]);
     {}
     vline (67+67,142,199,120);
     strcopy (ppchar1,'SPR:');
     val2pchar (currtrapsprite,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar (ntrapsprites,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (2+67,144,0,1,ppchar1);
     if (ntrapsprites <> 0) and (currtrapsprite <> 0) then scrtexnf (2+67,150,trapsprites[currtrapsprite-1]);
     {}
     vline (67+67+67,142,199,120);
     strcopy (ppchar1,'ANI:');
     val2pchar (currtrapanimation,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar (ntrapanimations,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (2+67+67,144,0,1,ppchar1);
     if (ntrapanimations <> 0) and (currtrapanimation <> 0) then
     scrtexnf (2+67+67,150,getanimation (trapanimations[currtrapanimation-1])^);
     end
     else
     begin
     vline (67,142,199,120);
     strcopy (ppchar1,'MON:');
     val2pchar (currtrapmonsteranimation,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar (ntrapmonstersanimations,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (2,144,0,1,ppchar1);
     if (ntrapmonstersanimations <> 0) and (currtrapmonsteranimation <> 0) then
     scrtexnf (2,150,getanimation (trapmonstersanimations[currtrapmonsteranimation-1])^);
     {}
     vline (67+67,142,199,120);
     strcopy (ppchar1,'ITE:');
     val2pchar (currtrapitemssprites,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar (ntrapitemssprites,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (2+67,144,0,1,ppchar1);
     if (ntrapitemssprites <> 0) and (currtrapitemssprites <> 0) then
     scrtexnf (2+67,150,trapitemssprites [currtrapitemssprites-1]);
     {}
     vline (67+67+67,142,199,120);
     strcopy (ppchar1,'GUN:');
     val2pchar (currtrapgunssprites,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar (ntrapgunssprites,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (2+67+67,144,0,1,ppchar1);
     if (ntrapgunssprites <> 0) and (currtrapgunssprites <> 0) then
     begin
          setcuranim (trapgunssprites [currtrapgunssprites-1],0);
          scrtexnf (2+67+67,150,getanimation (trapgunssprites [currtrapgunssprites-1])^);
     end;
     end;
     {---}
     if inftype = 0 then
     begin
     vline (67+67+67+62,142,199,120);
     strcopy (ppchar1,'FADE:');
     val2pchar (currfade,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,144,0,1,ppchar1);
     {}
     strcopy (ppchar1,'WATER:');
     val2pchar (currwater,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,150,0,1,ppchar1);
     {}
     strcopy (ppchar1,'EFEKT:');
     val2pchar (curref,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,156,0,1,ppchar1);
     {}
     strcopy (ppchar1,'CELL:');
     val2pchar (edcell+1,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,162,0,1,ppchar1);
     {}
     strcopy (ppchar1,'INF:');
     val2pchar (currinfo,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar (ntrapinfo,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,168,0,1,ppchar1);
     {}
     strcopy (ppchar1,'EDFADE:');
     val2pchar (byte (edfade),ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,174,0,1,ppchar1);
     {}
     strcopy (ppchar1,'EDMFADE:');
     val2pchar (byte (edmfade),ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,180,0,1,ppchar1);
     {}
     strcopy (ppchar1,'EDALL:');
     val2pchar (byte (edall),ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,186,0,1,ppchar1);
     {}
     strcopy (ppchar1,'EDINFO:');
     val2pchar (byte (edinfo),ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar (byte (edainfo),ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,192,0,1,ppchar1);
     {---}
     strcopy (ppchar1,'X:');
     val2pchar (edpozx,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (265,144,0,1,ppchar1);
     {}
     strcopy (ppchar1,'Y:');
     val2pchar (edpozy,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (265,150,0,1,ppchar1);
     {}
     strcopy (ppchar1,'XC:');
     val2pchar (edpozx div 32,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (265,156,0,1,ppchar1);
     {}
     strcopy (ppchar1,'YC:');
     val2pchar (edpozy div 24,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (265,162,0,1,ppchar1);
     {}
     strcopy (ppchar1,'C1:');
     val2pchar (getcell (edpozx div 32,edpozy div 24,0) and 127,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (265,168,0,1,ppchar1);
     {}
     strcopy (ppchar1,'C2:');
     val2pchar (getcell (edpozx div 32,edpozy div 24,1),ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (265,174,0,1,ppchar1);
     {}
     strcopy (ppchar1,'C3:');
     val2pchar (getcell (edpozx div 32,edpozy div 24,2),ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (265,180,0,1,ppchar1);
     {}
     strcopy (ppchar1,'F:');
     val2pchar (getcell (edpozx div 32,edpozy div 24,3) and 31,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (265,186,0,1,ppchar1);
     {}
     strcopy (ppchar1,'I:');
     val2pchar (getcell (edpozx div 32,edpozy div 24,4),ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar (getcell (edpozx div 32,edpozy div 24,5),ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (265,192,0,1,ppchar1);
     end
     else
     begin
     with currtrapinforec do
     begin
     {vline (67+67+67+82,142,199,120);}
     hline (202,168,320,120);
     {}
     strcopy (ppchar1,'SPT:');
     if (sl > 0) and (curspt > 0) then
     begin
          strcat (ppchar1,strupper (scripts[curspt-1].name));
     end;
     textprint (203,144,0,1,ppchar1);
     {}
     strcopy (ppchar1,'STA1:');
     val2pchar (objectstate and 1,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar ((objectstate shr 1) and 1,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar ((objectstate shr 2) and 1,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar ((objectstate shr 3) and 31,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,150,0,1,ppchar1);
     {}
     strcopy (ppchar1,'STA2:');
     val2pchar (objectextra,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,156,0,1,ppchar1);
     {}
     strcopy (ppchar1,'SPTN:');
     val2pchar (curspta,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,162,0,1,ppchar1);
     end;
     {---}
     if edcell = 2 then i := 5 else i := 4;
     j := getcell (edpozx div 32,edpozy div 24,i);
     if (j > 0) and (j <= maxinforecs) then
     begin
     pp := ptr (trapmap.trapinforecseg,(j*sizeof (trapinforec))-sizeof (trapinforec));
     move (pp^,ginfr,sizeof (trapinforec));
     with ginfr do
     begin
     strcopy (ppchar1,'OX:');
     val2pchar (objectpozx,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,170,0,1,ppchar1);
     {}
     strcopy (ppchar1,'OY:');
     val2pchar (objectpozy,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (265,170,0,1,ppchar1);
     {}
     strcopy (ppchar1,'SP:');
     val2pchar (objectscrypt,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,176,0,1,ppchar1);
     {}
     strcopy (ppchar1,'S1:');
     val2pchar (objectstate and 1,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar ((objectstate shr 1) and 1,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar ((objectstate shr 2) and 1,ppchar2);
     strcat (ppchar1,ppchar2);
     strcat (ppchar1,'/');
     val2pchar ((objectstate shr 3) and 31,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,182,0,1,ppchar1);
     {}
     strcopy (ppchar1,'S2:');
     val2pchar (objectextra,ppchar2);
     strcat (ppchar1,ppchar2);
     textprint (203,188,0,1,ppchar1);
     end;
     end;
     end;
     {rysowanie mapy}
     drawmap;
     if ednet then
     begin
          p1 := edepozx mod 32;
          p2 := edepozy mod 24;
          for j := 0 to 6 do if j*24-p2 < 142 then hline (0-p1,j*24-p2,319,105);
          for i := 0 to 10 do vline (i*32-p1,0-p2,141,105);
     end;
     if edcur then drawcur;
     if hon then textprint (0,0,0,1,helptxt);
     {}
     processani;
     {}
     freemem (ppchar2,9);
     freemem (ppchar1,128);
end;

procedure printfps (x,y : integer);
var
ppchar : pchar;
s : string[15];
begin
     getmem (ppchar,15);
     str (fps:0:2,s);
     strpcopy (ppchar,s);
     textprint (x,y,0,1,ppchar);
     freemem (ppchar,15);
end;

procedure trapedloop;
begin
     {cl1 := clock;}
     frames := 0;
     while (1=1) do
     begin
          buftoclr;
          if readkey (kc,ks) then
          begin
               if ks = 1 then halt (1);
               {}
               if ks = 30 then idval (currtraptexture,ntraptextures,1); {a}
               if ks = 44 then idval (currtraptexture,ntraptextures,-1); {z}
               {}
               if ks = 31 then idval (currtrapsprite,ntrapsprites,1); {s}
               if ks = 45 then idval (currtrapsprite,ntrapsprites,-1); {x}
               {}
               if ks = 32 then idval (currtrapanimation,ntrapanimations,1); {d}
               if ks = 46 then idval (currtrapanimation,ntrapanimations,-1); {c}
               {}
               if ks = 33 then idval (currtrapmonsteranimation,ntrapmonstersanimations,1); {f}
               if ks = 47 then idval (currtrapmonsteranimation,ntrapmonstersanimations,-1); {v}
               {}
               if ks = 34 then idval (currtrapitemssprites,ntrapitemssprites,1); {g}
               if ks = 48 then idval (currtrapitemssprites,ntrapitemssprites,-1); {b}
               {}
               if ks = 35 then idval (currtrapgunssprites,ntrapgunssprites,1); {h}
               if ks = 49 then idval (currtrapgunssprites,ntrapgunssprites,-1); {n}
               {}
               if ks = 79 then idval (trapmap.players[0].state,7,1);
               if biosextkeys1 and LSHIFT_PRESSED <> LSHIFT_PRESSED then
               begin
               if ks = 12 then idval (currfade,31,-1); {-}
               if ks = 13 then idval (currfade,31,1); {+}
               end
               else
               begin
               if ks = 12 then widval (curspt,sl,-1); {-}
               if ks = 13 then widval (curspt,sl,1); {+}
               if curspt > 0 then
               curspta := scripts[curspt-1].sa
               else
               curspta := scryptslen+1;
               end;
               if ks = 24 then idval (currwater,3,-1); {o}
               if ks = 25 then idval (currwater,3,1); {p}
               if ks = 26 then idval (curref,3,-1); {[}
               if ks = 27 then idval (curref,3,1); {]}
               if ks = 51 then idval (currinfo,255,-1); {,}
               if ks = 52 then idval (currinfo,255,1); {.}
               {}
               if biosextkeys1 and LSHIFT_PRESSED = LSHIFT_PRESSED then
               begin
                    if ks = 36 then idvalbit (currtrapinforec.objectstate,31,-1,3,5); {j}
                    if ks = 22 then idvalbit (currtrapinforec.objectstate,31,1,3,5); {u}
                    if ks = 37 then idval (currtrapinforec.objectextra,255,-1); {k}
                    if ks = 23 then idval (currtrapinforec.objectextra,255,1); {i}
               end;
               if ks = 43 then {\}
               begin
                    if (biosextkeys1 and CTRL_PRESSED <> CTRL_PRESSED)
                    and (biosextkeys1 and LSHIFT_PRESSED <> LSHIFT_PRESSED) then
                    idvalbit (currtrapinforec.objectstate,1,1,0,1)
                    else
                    if biosextkeys1 and CTRL_PRESSED = CTRL_PRESSED then
                    idvalbit (currtrapinforec.objectstate,1,1,1,1)
                    else
                    if biosextkeys1 and LSHIFT_PRESSED = LSHIFT_PRESSED then
                    idvalbit (currtrapinforec.objectstate,1,1,2,1)
               end;
               {}
               if ks = 71 then setani;
               if ks = 15 then {tab}
               if biosextkeys1 and LSHIFT_PRESSED <> LSHIFT_PRESSED then
               texstype := 1-texstype
               else
               inftype := 1-inftype;
               if (biosextkeys1 and LSHIFT_PRESSED <> LSHIFT_PRESSED) and (biosextkeys1 and CTRL_PRESSED <> CTRL_PRESSED) then
               begin
               if ks = 37 then edcur := not edcur; {k}
               if ks = 38 then edfade := not edfade; {l}
               end;
               if ks = 39 then edmfade := not edmfade; {;}
               if ks = 40 then edall := not edall; {'}
               if ks = 50 then {m}
               if biosextkeys1 and CTRL_PRESSED <> CTRL_PRESSED then
               edinfo := not edinfo
               else
               edainfo := not edainfo;
               {}
               if ks = 67 then ednet := not ednet;
               {}
               if ks in [2..7] then edcell := ks-2;
               {}
               if ks = 83 then {del}
               begin
               if biosextkeys1 and RSHIFT_PRESSED = RSHIFT_PRESSED then setall (edcell,0)
               else
               if biosextkeys1 and LSHIFT_PRESSED <> LSHIFT_PRESSED then
               begin
                    if edcell < 3 then deltex (edpozx div 32,edpozy div 24,edcell)
                    else setcell (edpozx div 32,edpozy div 24,edcell,0);
               end
               else
               begin
                    if edcell < 3 then setalltex (edcell,0,0)
                    else setall (edcell,0);
                    if edcell >= 4 then ntrapinfo := 0;
               end;
               end;
               if biosextkeys1 and CTRL_PRESSED <> CTRL_PRESSED then
               begin
                    if ks = 16 then {q}
                    if edcell < 3 then
                    addtex (edpozx div 32,edpozy div 24,edcell,currtraptexture,0);
                    {}
                    if ks = 17 then {w}
                    if (edcell > 0) and (edcell < 3) then
                    addtex (edpozx div 32,edpozy div 24,edcell,currtrapsprite+64,1);
                    {}
                    if ks = 18 then {e}
                    if (edcell > 0) and (edcell < 3) then
                    addtex (edpozx div 32,edpozy div 24,edcell,currtrapanimation+128,2);
                    {}
                    if ks = 20 then {t}
                    if (edcell > 0) and (edcell < 3) then
                    addtex (edpozx div 32,edpozy div 24,edcell,currtrapitemssprites+192,3);
                    {}
                    if ks = 21 then {y}
                    if (edcell > 0) and (edcell < 3) then
                    addtex (edpozx div 32,edpozy div 24,edcell,currtrapgunssprites+224,4);
                    if ks = 28 then {enter}
                    begin
                         setcell (edpozx div 32,edpozy div 24,3,currfade+(currwater shl 5)+((curref shr 1) shl 7));
                         setcell (edpozx div 32,edpozy div 24,0,getcell (edpozx div 32,edpozy div 24,0) and 127+
                         (curref and 1) shl 7);
                    end;
                    if ks = 53 then {/}
                    if edainfo = false then
                    begin
                         if (edcell > 3) and (edcell < 6) and ((currinfo = 0) or (currinfo > maxinforecs)) then
                         setcell (edpozx div 32,edpozy div 24,edcell,currinfo);
                    end
                    else
                    begin
                         if (edcell > 3) and (edcell < 6) then
                         setinfo (currinfo,edcell);
                    end;
               end
               else
               begin
                    if ks = 16 then if edcell < 3 then setalltex (edcell,currtraptexture,0);
                    if ks = 17 then if (edcell > 0) and (edcell < 3) then setalltex (edcell,currtrapsprite+64,1);
                    if ks = 18 then if (edcell > 0) and (edcell < 3) then setalltex (edcell,currtrapanimation+128,2);
                    if ks = 20 then if (edcell > 0) and (edcell < 3) then setalltex (edcell,currtrapitemssprites+192,3);
                    if ks = 21 then if (edcell > 0) and (edcell < 3) then setalltex (edcell,currtrapgunssprites+224,4);
                    if ks = 28 then {enter}
                    begin
                         setall (3,currfade+(currwater shl 5)+((curref shr 1) shl 7));
                         setalle1;
                    end;
               end;
               if ed32 = false then
               begin
                    if ks = 72 then idedpoz (0,-1);
                    if ks = 80 then idedpoz (0,1);
                    if ks = 75 then idedpoz (-1,0);
                    if ks = 77 then idedpoz (1,0);
               end
               else
               begin
                    if ks = 72 then idedpoz (0,-24);
                    if ks = 80 then idedpoz (0,24);
                    if ks = 75 then idedpoz (-32,0);
                    if ks = 77 then idedpoz (32,0);
               end;
               if ks = 57 then showmap;
               if ks = 73 then ed32 := true;
               if ks = 81 then ed32 := false;
               if ks = 41 then addplayer;
               {}
               if ks = 59 then hon := not hon; {F1}
               if ks = 60 then if trapmapfname[0] <> #0 then savemap (trapmapfname) else saveloadmap (0);{F2}
               if ks = 61 then saveloadmap (1);{F3}
               if ks = 62 then newmap;         {F4}
               if ks = 63 then saveloadmap (0);{F5}
               if ks = 64 then loadscriptib;   {F6}
               {}
               if ks = 14 then getcellval (edpozx div 32,edpozy div 24,edcell);
          end;
          renderall;
          animefects;
          {printfps (0,0);}
          wfvr;
          buftoscr;
          inc (frames);
          {cl2 := clock;
          if cl2 <> cl1 then
          fps := frames/((cl2-cl1)/18.2);}
     end;
end;

begin
     initialize;
     newmap;
     trapedloop;
end.