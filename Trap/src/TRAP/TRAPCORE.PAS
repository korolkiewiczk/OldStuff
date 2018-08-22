{Trap copyright (c) Kamil Korolkiewicz 2003/2004}
{-----------------------------------------------}

{Dolanczane moduly}

uses trapcrt,trapdos,scr,xscr,text,xmem,time,snd,key,pfile,strings,tcu;

{deklaracje stalych}

const

{stale zmienne}
processornames : array [0..1] of pchar = ('Intel(TM) 386DX','Intel(TM) 486DX');
MENU832N = 8;
MENUTWVN = 2;
menu832names : array [0..MENU832N-1] of pchar = ('OKCHAR.832','NOCHAR.832','LARROW.832','RARROW.832',
'GAMMATRK.832','GAMMATRC.832','NOMMX.832','HEALTHI.832');
menutwvnames : array [0..MENUTWVN-1] of pchar = ('MENUKEYP.TWV','MENUEKEY.TWV');
ussndt : boolean = true;
trappfrec : pfiler=nil;
levelmenu : word = 200;
listkeysmenu : word = 0;
watercolors : array [1..3] of byte = (32*3+16,32*4+16,32*0+16);
deltatefect1 : shortint = 1;

rsplayershifts : array [0..3] of shortint = (-6,0,-6,0);
lsplayershifts : array [0..3] of shortint = (-4,0,-4,0);
hrsplayershifts : array [0..3] of shortint = (-6,0,-6,0);
hlsplayershifts : array [0..3] of shortint = (-2,0,-2,0);


{stale}
ver = '0.5';
midimusicmaxsize = 40000;

FIRSTKEY=0;
SECONDKEY=1;

MOVERIGHT=0;
MOVELEFT=1;
JUMP=2;
CROUCH=3;
USE=4;
SHOT=5;

PLAYERHEALTH = 100;

KEY_UP=72;
KEY_DOWN=80;
KEY_LEFT=75;
KEY_RIGHT=77;
KEY_SHIFT=42;
KEY_CTRL=29;
KEY_ALT=56;

DOSMEM = 0;
XMSMEM = 1;

GAMESTATE_GAME=0;
GAMESTATE_TITLE=1;
GAMESTATE_DEMO=2;

SDOOR=3;
RDOOR=2;
BDOOR=1;
YDOOR=0;

TIMETITLE=800; {8}

STDWORLDGRAVITY=235 shl 8+51; {235.2}
STDW=10;
STDO=250;
STDOT=50;
STDTRAPSTEP=936; {1/70}
UNDERWATERMAXTIME=1000;
UNDERWATERHDT=18;
UNDERWATERHD=5;

LU=1;
RU=2;
LD=4;
RD=8;
N=16;
E=32;
S=64;
W=128;

HEALTH_ITEMS = 0;
KEYS_ITEMS = 1;
AMMO_ITEMS = 2;

REDKEY=0;
BLUEKEY=1;
YELLOWKEY=2;

trapstep : longint = STDTRAPSTEP;

efect_1_fps = 56 shl 8;
efect_2_3_fps = 17 shl 8;

pjmpp = -(70 shl 8);
pjmpv = 18;

pusft = 300;

stdplkilledt = 5;

maxtextures = 128;
maxsprites = 64;
maxanimations = 64;
maxitemssprites = 32;
maxgunssprites = 32;
maxanimtexts = 80;

maxmonstersounds = 0;
maxothersounds = 0;
maxplayersounds = 5;
maxgunssounds = 0;

maxtypedmonsters = 1;
maxmonstersonmap = 30;

HDELOBJV=-50;

trapxmssndbufsize = 65536 div nchannels;

gammostd = 50;


{$I PARAMS.PAS}

{deklaracje typow}

type

tsbinforec = record
                   p : word;
                   d,i : byte;
                   m : word;
end;

tsysinf = record
                procname : pchar;
                proccaps : longint;
                procclk : word;
                dosver : word;
                winver : word;
                dosmemfree : longint;
                xmsmemfree : longint;
                vsyncfreq : word;
end;

tconfigrec = record
                   xscrsx,xscrsy : word;
                   scrmode : byte;
                   ussnd,usmus,usmmx : byte;
                   xsbx1,xsby1,xsbx2,xsby2 : word;
                   gamma : integer;
end;

titem = record
              id,idval1,idval2 : word;
end;

tinforecxyc = record
                   x,y : word;
                   c : byte;
end;

{$I FLOATINT.PAS}
{$I PHYSICS.PAS}
{$I PART.PAS}
{$I GUNS.PAS}
{$I TRAPMENU.PAS}
{$I ANIPROCS.PAS}
{$I KEYCONF.PAS}
{$I TRAPMAP.PAS}
{$I PLAYER.PAS}
{$I CONSOLE.PAS}

type

tpart = record
               used : boolean;
               n : byte;
               sx,sy : floatint;
               particles : tparticles;
end;

tparts = array [0..maxparts-1] of tpart;

{deklaracje zmiennych}

var

{}
trapfilehandle : fileor;
{}
sbinforec : tsbinforec;
sysinf : tsysinf;
configrec : tconfigrec;
{}
vgaonly : boolean;
xscrscrsx,xscrscrsy : word;
scrox,scroy : word;
{}
oldexitproc : pointer;
{}
trapmainmenu : pmenu;
trapcurrmenu : pmenu;
{}
texttex1,texttex2 : ttex;
{}
traptextures : array [0..maxtextures-1] of ttex;
trapsprites : array [0..maxsprites-1] of ttex;
trapanimations : array [0..maxanimations-1] of tanimation;
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
ianimtexts : byte;
{}
monstersounds : array [0..maxplayersounds-1] of tsample;
othersounds : array [0..maxplayersounds-1] of tsample;
playersounds : array [0..maxplayersounds-1] of tsample; {0-skok 1-spadek 2-uderzenie 3-mocne uderzenie 4-˜mier†}
gunssounds : array [0..maxplayersounds-1] of tsample;
{}
items : array [0..maxitemssprites-1] of titem;
skeyitems,sammoitems : byte;
{}
rkscan : byte;
rkchar : char;
{}
trapframes,trapbframes : longint;
trapfps : floatint;
trapfpstimer : stimer;
drawtrapfps : boolean;
iswait4vsync : boolean;
{}
trapmenuon : boolean;
keypoz : shortint;
setingkeys : byte;
traptitle : ttex;
trapcur : tanimation;
{}
keyactionstab : array [0..1,0..5] of byte;
menu832texs : array [0..MENU832N-1] of ttex;
menutwvs : array [0..MENUTWVN-1] of tsample;
actionnames : array [0..5,0..29] of char;
sysinfotxts : array [0..8,0..59] of char;
sysinfotxt : array [0..299] of char;
modesnames : array [0..8,0..10] of char;
traphelp : array [0..499] of char;
params : word;
{}
curepizod : byte;
gamestate : byte;
{}
imodes,mmodeg,amodeg : byte;
{}
titletimer : stimer;
{}
trapmidimusic : pointer;
trapmidimusicsize : word;
trapsampleno : byte;
trapxmssndbuf : array [0..nchannels-1] of tsample;
{}
trapcurrmap : trapmapdesc;
trapinforecs : byte;
trapscriptssize : word;
trapcurrmapname : pchar;
trapcurrmapmusic : pchar;
maploaded : boolean;
trapmapviewshfx,trapmapviewshfy : word;
mtrapmapsizex,mtrapmapsizey : word;
{}
trapefects : array [1..3] of byte;
curof : byte;
{}
trapconsole : tconsole;
editconsole : boolean;
editconsolepchar,editconsolepcharl : pchar;
editconsolepchars : word;
{}
worldphysicobj : tphysicobj;
watervel : floatint;
{}
playerashf : integer;
playerhshf : integer;
playerjumps : boolean;
playerisongrab : boolean;
pusf : stimer;
pauset : stimer;
plrlmovd,pludmovd : boolean;
pcrouch : boolean;
pdir : byte;
pwaterw : floatint;
maxpvel : floatint;
playerphysicobj : tphysicobj;
playermov : byte;
playersteps : longint;
playerdeltayvel : floatint;
playerdeltahealth,playerdeltahealthf : integer;
playerunderwater : boolean;
playeruwptime : longint;
playerkeys : array [0..2] of boolean;
playerdwaterhd : word;
playeriskilled : boolean;
plkilledt : longint;
playeronmap : boolean;
playerisuse : boolean;
playershoting : boolean;
gamepaused : boolean;
{}
godmode : boolean;
noclipmode : boolean;
{}
inforecxyctab : array [0..maxinforecs-1] of tinforecxyc;
{}
trapguns : tguns;
plguntimer : stimer;
plguna : byte;
trapsammo : tsammo;
isupdateshot : boolean;
{}
trapparts : tparts;
usedtrapparts : byte;
{}
trapmonsters : ttmonsters;


{deklaracje procedor i funkcji}

procedure render; forward;

{-----------------------------------------------}

{konczy prace programu}

procedure getframescnt (nps : floatint; var framescnt,framesskip : integer);
var
out,modv : floatint;
begin
     floatint_div (out,modv,trapfps,nps);
     if out >= 256 then
     begin
          floatint_rnd (framescnt,out);
          framesskip := 0;
     end
     else
     begin
          if out = 0 then inc (out);
          floatint_div (out,modv,256,out);
          floatint_rnd (framesskip,out);
          framescnt := 1;
     end;
end;

procedure animanimation (var anim : tanimation);
var
fcnt,fskip : integer;
i : integer;
begin
     getframescnt (anim^.animtime shl 8,fcnt,fskip);
     if trapframes mod fcnt = fcnt-1 then
     for i := 0 to fskip do setcuranim (anim,anim^.curanim+1);
end;

{procedure animval (var v : word; nv1,nv2 : word; t : floatint);
var
fcnt,fskip : integer;
begin
     getframescnt (t,fcnt,fskip);
     if trapframes mod fcnt < fcnt shr 1 then v := nv1 else v := nv2;
end;}

procedure animval (var v : byte; nvtab : pointer; nvc : byte; t : floatint; frames : word);
type
st = array [0..255] of byte;
pt = ^st;
var
fcnt,fskip : integer;
pom : word;
i : byte;
ppt : pt;
begin
     ppt := nvtab;
     getframescnt (t,fcnt,fskip);
     pom := fcnt div nvc;
     if pom = 0 then pom := 1;
     for i := 1 to nvc do
     begin
          if frames mod fcnt < pom*i then
          begin
               v := ppt^[i-1];
               break;
          end;
     end;
end;

procedure getplayersxsy (var sx,sy : word);
begin
     if trapcurrmap.players[0].state >= 12 then
     begin
          setcuranim (playeranimations,trapcurrmap.players[0].state);
          sx := getanimation (playeranimations)^.sizex;
          sy := getanimation (playeranimations)^.sizey;
          exit;
     end;
     if not pcrouch then
     setcuranim (playeranimations,0)
     else
     setcuranim (playeranimations,6);
     sx := getanimation (playeranimations)^.sizex;
     sy := getanimation (playeranimations)^.sizey;
end;

{procedury do obsˆugi skrypt¢w}

function iscolision (p1x1,p1y1,p1x2,p1y2,p2x1,p2y1,p2x2,p2y2 : word) : byte; forward;

{$I SCRIPTS.PAS}

function s_isplayeroncell (x,y : word) : boolean;
var
i,j,px1,py1,px2,py2 : word;
sx,sy : word;
begin
     {px1 := trapcurrmap.players[0].playerpozx;
     py1 := trapcurrmap.players[0].playerpozy;
     getplayersxsy (sx,sy);
     px2 := trapcurrmap.players[0].playerpozx+sx-1;
     py2 := trapcurrmap.players[0].playerpozy+sy-1;
     if iscolision (px1,py1,px2,py2,x*32,y*24,x*32+31,y*24+23)<>0 then s_isplayeroncell := true
     else
     begin
          if iscolision (x*32,y*24,x*32+31,y*24+23,px1,py1,px2,py2)<>0 then s_isplayeroncell := true
          else s_isplayeroncell := false;
     end;}
     s_isplayeroncell := true;
     px1 := (trapcurrmap.players[0].playerpozx) shr 5;
     py1 := (trapcurrmap.players[0].playerpozy) div 24;
     getplayersxsy (sx,sy);
     px2 := (trapcurrmap.players[0].playerpozx+sx) shr 5;
     py2 := (trapcurrmap.players[0].playerpozy+sy) div 24;
     for j := py1 to py2 do
     for i := px1 to px2 do
     begin
          if (i = x) and (j = y) then exit;
     end;
     s_isplayeroncell := false;
end;


procedure s_init;
begin
     with trapcurrmap do
     begin
     scriptinforec := ptr (trapinforecseg,0);
     scriptcode := ptr (trapscriptseg,0);
     scriptcodeip := scriptslen+1;
     scriptmap := @(trapmap^);
     end;
end;

procedure s_processscript (i : byte);
var
callproc : pointer;
begin
     scriptownxyc := 0;
     scriptcurrinforec := i;
     with trapcurrmap do
     begin
     scriptcodeip := scriptinforec^[i].objectscript;
     if scriptcodeip > trapscriptssize then exit;
     {wykonuj kod skryptu}
     while (1=1) do
     begin
          scriptbcode := scriptcode^[scriptcodeip];
          if scriptbcode = 0 then exit
          else
          if scriptbcode = 1 then if scriptplayerisuse = false then exit
          else
          begin asm inc scriptcodeip end; continue; end;
          {}
          if scriptbcode >= scriptprocedures then syserror ('s_processscript()','Invalid script opcode');
          callproc := scriptprocs [scriptbcode];
          if callproc = nil then syserror ('s_processscript()','Script procedure is nil');
          asm
             call callproc
             inc scriptcodeip
          end;
     end;
     end;
end;

procedure s_processallmap;
var
i : byte;
begin
     with trapcurrmap do
     begin
     if trapscriptssize > 0 then
     if trapinforecs > 0 then
     for i := 0 to trapinforecs-1 do
     begin
          scriptplayerisuse := false;
          if playerisuse then
          begin
               if s_isplayeroncell (inforecxyctab[i].x,inforecxyctab[i].y)
               then scriptplayerisuse := true;
          end;
          s_processscript (i);
     end;
     end;
end;

{procedury do obsˆugi kolizji}

function isplayerongrab : boolean;
const
gv1 = 12;
gv2 = 4;
gv3 = 12;
gv4 = 16;
var
i,j,px1,py1,px2,py2 : word;
inf : byte;
sx,sy : word;
begin
     isplayerongrab := true;
     px1 := (trapcurrmap.players[0].playerpozx+gv1) shr 5;
     py1 := (trapcurrmap.players[0].playerpozy+gv2) div 24;
     getplayersxsy (sx,sy);
     px2 := (trapcurrmap.players[0].playerpozx+sx-gv3) shr 5;
     py2 := (trapcurrmap.players[0].playerpozy+sy-gv4) div 24;
     for j := py1 to py2 do
     for i := px1 to px2 do
     begin
          inf := trapcurrmap.trapmap^[j*trapcurrmap.trapmapsizex+i].trapinforec1;
          if inf = 254 then exit
          else
          begin
               inf := trapcurrmap.trapmap^[j*trapcurrmap.trapmapsizex+i].trapinforec2;
               if inf = 254 then exit;
          end;
     end;
     isplayerongrab := false;
end;

{zwraca wypor}
function isplayerinwater : floatint;
const
pwy = 12;
punderwy = 6;
kwashd = 5;
lawahd = 50;
var
i,j,px1,py1,px2,py2 : word;
ppy1,ppy2 : longint;
water,pw : byte;
sx,sy : word;
t : boolean;
pfi,pmv : floatint;
begin
     isplayerinwater := $fffffff;
     px1 := (trapcurrmap.players[0].playerpozx) shr 5;
     py1 := (trapcurrmap.players[0].playerpozy) div 24;
     getplayersxsy (sx,sy);
     px2 := (trapcurrmap.players[0].playerpozx+sx) shr 5;
     py2 := (trapcurrmap.players[0].playerpozy+sy) div 24;
     t := false;
     for j := py1 to py2 do
     begin
     for i := px1 to px2 do
     begin
          water := (trapcurrmap.trapmap^[j*trapcurrmap.trapmapsizex+i].fadeandwater shr 5) and 3;
          if water <> 0 then
          begin
               t := true;
               pw := water;
          end;
     end;
     if t then break;
     end;
     if t = false then
     begin
          playerunderwater := false;
          playerdwaterhd := 0;
          exit;
     end;
     ppy1 := longint (j)*24-longint (trapcurrmap.players[0].playerpozy);
     if ppy1 < punderwy then
     begin
          if playerunderwater = false then
          begin
               trapcurrmap.players[0].watertime := clock;
               playerunderwater := true;
          end;
     end
     else
     playerunderwater := false;
     if ppy1 > sy-pwy then
     begin
          playerdwaterhd := 0;
          exit;
     end
     else
     begin
          if pw = 1 then playerdwaterhd := 0
          else
          if pw = 2 then playerdwaterhd := kwashd
          else
          if pw = 3 then playerdwaterhd := lawahd;
     end;
     if ppy1 < 0 then ppy1 := 0;
     ppy2 := sy;
     floatint_div (pfi,pmv,ppy1 shl 8,ppy2 shl 8);
     floatint_sub (pfi,256,pfi);
     floatint_mul (pfi,pfi,watervel);
     isplayerinwater := pfi;
end;

function ispixinrect (x1,y1,x2,y2,x,y : integer) : boolean;
begin
     if ((x >= x1) and (x <= x2)) and ((y >= y1) and (y <= y2)) then
     begin
          ispixinrect := true;
          exit;
     end
     else
     ispixinrect := false;
end;

function iscolision (p1x1,p1y1,p1x2,p1y2,p2x1,p2y1,p2x2,p2y2 : word) : byte;
var
ov : byte;
begin
     ov := 0;
     {}
     if ispixinrect (p1x1,p1y1,p1x2,p1y2,p2x1,p2y1) then ov := ov or LU;
     if ispixinrect (p1x1,p1y1,p1x2,p1y2,p2x2,p2y1) then ov := ov or RU;
     if ispixinrect (p1x1,p1y1,p1x2,p1y2,p2x1,p2y2) then ov := ov or LD;
     if ispixinrect (p1x1,p1y1,p1x2,p1y2,p2x2,p2y2) then ov := ov or RD;
     {}
     if (ov and LU=LU) and (ov and RU=RU) then ov := ov or N;
     if (ov and LD=LD) and (ov and RD=RD) then ov := ov or S;
     if (ov and RU=RU) and (ov and RD=RD) then ov := ov or E;
     if (ov and LU=LU) and (ov and LD=LD) then ov := ov or W;
     {}
     iscolision := ov;
end;

function iscellfull (x,y : word) : boolean;
var
cell : byte;
s : word;
inf : byte;
st : byte;
inforecs : pinforecs;
begin
     inforecs := ptr (trapcurrmap.trapinforecseg,0);
     iscellfull := false;
     s := y*trapcurrmap.trapmapsizex+x;
     cell := trapcurrmap.trapmap^[s].fronttex1;
     inf := trapcurrmap.trapmap^[s].trapinforec1;
     if (inf > 0) and (inf <= maxinforecs) then
     st := inforecs^[inf-1].objectstate and 1
     else
     begin
          if (inf = 255) or (inf = 254) then st := 0
          else st := 1;
     end;
     if (cell < 64) and (cell <> 0) and (st = 1) then
     begin
          iscellfull := true;
          exit;
     end;
     if (cell = 0) or (cell = 64) or (cell = 128) or (cell = 192) or (cell = 224) or (st = 0) then
     begin
          cell := trapcurrmap.trapmap^[s].fronttex2;
          inf := trapcurrmap.trapmap^[s].trapinforec2;
          if (inf > 0) and (inf <= maxinforecs) then
          st := inforecs^[inf-1].objectstate and 1
          else
          begin
               if (inf = 255) or (inf = 254) then st := 0
               else st := 1;
          end;
          if (cell = 0) or (cell = 64) or (cell = 128) or (cell = 192) or (cell = 224) or (st = 0) then exit;
     end;
     iscellfull := true;
end;

function iscellcolision (x1,y1,x2,y2 : word) : byte;
var
ov : byte;
px1,py1,px2,py2 : word;
i,j : word;
begin
     ov := 0;
     px1 := x1 shr 5;
     py1 := y1 div 24;
     px2 := x2 shr 5;
     py2 := y2 div 24;
     for j := py1 to py2 do
     for i := px1 to px2 do
     begin
          if iscellfull (i,j) then
          begin
               if (i = px1) or (i = px2) then
               begin
               if i = px1 then
               begin
                    if (j = py1) or (j = py2) then
                    begin
                         if j = py1 then ov := ov or LU;
                         if j = py2 then ov := ov or LD;
                    end
                    else
                    ov := ov or W;
               end;
               if i = px2 then
               begin
                    if (j = py1) or (j = py2) then
                    begin
                         if j = py1 then ov := ov or RU;
                         if j = py2 then ov := ov or RD;
                    end
                    else
                    ov := ov or E;
               end;
               end
               else
               begin
                    if j = py1 then ov := ov or N;
                    if j = py2 then ov := ov or S;
               end;
          end;
     end;
     if (ov and LU=LU) and (ov and RU=RU) then ov := ov or N;
     if (ov and LD=LD) and (ov and RD=RD) then ov := ov or S;
     if (ov and RU=RU) and (ov and RD=RD) then ov := ov or E;
     if (ov and LU=LU) and (ov and LD=LD) then ov := ov or W;
     iscellcolision := ov;
end;

function handlecellcol (var po : tphysicobj; sx,sy : word; j : boolean) : byte;
var
cellcol : byte;
px,py : integer;
ppx,ppy,ppv : word;
p : byte;
begin
     handlecellcol := 0;
     playerjumps := true;
     if playerisongrab then if isplayerongrab = false then playerisongrab := false;
     floatint_rnd (px,po.physicobjpozx);
     floatint_rnd (py,po.physicobjpozy);
     cellcol := iscellcolision (px,py,px+sx,py+sy);
     handlecellcol := cellcol;
     if cellcol = 0 then exit;
     ppx := (sx-1) shr 5;
     ppy := (sy-1) div 24;
     px := px shr 5;
     py := py div 24;
     p := 0;
     ppv := (po.physicobjpozy shr 8) mod 24;
     {}
     if cellcol and (S)=(S) then
     begin
          po.physicobjpozy := longint ((py+ppy+1) * 24-sy) shl 8;
          po.physicobjyvelocity := 0;
          playerjumps := false;
          p := p or 2;
     end;
     {}
     if cellcol and (N)=(N) then
     begin
          po.physicobjpozy := longint ((py+1) * 24) shl 8;
          po.physicobjyvelocity := 0;
          p := p or 2;
     end;
     {}
     if cellcol and (E)=(E) then
     begin
          po.physicobjpozx := longint ((px+ppx+1) shl 5-sx) shl 8;
          po.physicobjxvelocity := 0;
          p := p or 4;
     end;
     {}
     if cellcol and (W)=(W) then
     begin
          po.physicobjpozx := longint ((px+1) shl 5) shl 8;
          po.physicobjxvelocity := 0;
          p := p or 4;
     end;
     if ((cellcol and (LU)=(LU)) or (cellcol and (RU)=(RU))) and (p and 2<>2) {and (p and 4<>4)} then
     begin
          if (cellcol and (RU)=(RU)) then
          begin
               if (((po.physicobjpozx shr 8)+sx) mod 32) < 4 then
               begin
                    po.physicobjpozx := longint ((px+ppx+1) shl 5-sx) shl 8;
                    po.physicobjxvelocity := 0;
               end
               else
               begin
                    po.physicobjpozy := longint ((py+1) * 24) shl 8;
                    po.physicobjyvelocity := 0;
               end;
          end
          else
          if (cellcol and (LU)=(LU)) then
          begin
               if (((po.physicobjpozx shr 8)+sx) mod 32) >= sx-4 then
               begin
                    po.physicobjpozx := longint ((px+1) shl 5) shl 8;
                    po.physicobjxvelocity := 0;
               end
               else
               begin
                    po.physicobjpozy := longint ((py+1) * 24) shl 8;
                    po.physicobjyvelocity := 0;
               end;
          end;
          p := p or 8;
     end;
     {}
     if ((cellcol and (LD)=(LD)) or (cellcol and (RD)=(RD))) and (p and 2<>2) then
     begin
          if ((cellcol and (E)=(E)) and (cellcol and (RD)<>(RD))) then
          begin
               po.physicobjpozy := longint ((py+ppy+1) * 24-sy) shl 8;
               po.physicobjyvelocity := 0;
               playerjumps := false;
               po.physicobjpozx := longint ((px+ppx+1) shl 5-sx) shl 8;
               po.physicobjxvelocity := 0;
          end
          else
          if ((cellcol and (W)=(W)) and (cellcol and (LD)<>(LD))) then
          begin
               po.physicobjpozy := longint ((py+ppy+1) * 24-sy) shl 8;
               po.physicobjyvelocity := 0;
               playerjumps := false;
               po.physicobjpozx := longint ((px+1) shl 5) shl 8;
               po.physicobjxvelocity := 0;
          end
          else
          if (cellcol and (E)<>(E)) and (cellcol and (W)<>(W)) then
          begin
          if (cellcol and (RD)=(RD)) then
          begin
               if (((po.physicobjpozy shr 8)+sy) mod 24) < pjmpv then
               begin
                    if (((po.physicobjpozy shr 8)+sy) mod 24) > 2 then
                    begin
                         po.physicobjpozx := longint ((px+ppx+1) shl 5-sx) shl 8;
                         po.physicobjxvelocity := 0;
                    end;
                    {po.physicobjpozy := longint ((py+ppy+1) * 24-sy) shl 8;}
                    if po.physicobjpozy shr 8 <= (py+ppy+1) * 24-sy+2
                    then
                    begin
                         po.physicobjpozy := longint ((py+ppy+1) * 24-sy) shl 8;
                         po.physicobjyvelocity := 0;
                         playerjumps := false;
                    end
                    else
                    if (playerjumps) and (po.physicobjyvelocity < 25600) and (j) then
                    po.physicobjyvelocity := pjmpp;
                    {po.physicobjpozy := longint ((py+ppy+1) * 24-sy) shl 8;
                    po.physicobjyvelocity := 0;}
                    {playerjumps := false;}
               end
               else
               begin
                    po.physicobjpozx := longint ((px+ppx+1) shl 5-sx) shl 8;
                    po.physicobjxvelocity := 0;
               end;
          end
          else
          if (cellcol and (LD)=(LD)) then
          begin
               if (((po.physicobjpozy shr 8)+sy) mod 24) < pjmpv then
               begin
                    if (((po.physicobjpozy shr 8)+sy) mod 24) > 2 then
                    begin
                         po.physicobjpozx := longint ((px+1) shl 5) shl 8;
                         po.physicobjxvelocity := 0;
                    end;
                    if po.physicobjpozy shr 8 <= (py+ppy+1) * 24-sy+2
                    then
                    begin
                         po.physicobjpozy := longint ((py+ppy+1) * 24-sy) shl 8;
                         po.physicobjyvelocity := 0;
                         playerjumps := false;
                    end
                    else
                    if (playerjumps) and (po.physicobjyvelocity < 25600) and (j) then
                    po.physicobjyvelocity := pjmpp;
               end
               else
               begin
                    po.physicobjpozx := longint ((px+1) shl 5) shl 8;
                    po.physicobjxvelocity := 0;
               end;
          end;
          end;
     end;
     if ((cellcol and (LU+RD)=(LU+RD)) or (cellcol and (RU+LD)=(RU+LD))) and (p and 2<>2) and (p and 4<>4)
     and (ppv > 20) then
     begin
          if (cellcol and (RU+LD)=(RU+LD)) then
          begin
               po.physicobjpozx := longint ((px+1) shl 5) shl 8;
               po.physicobjxvelocity := 0;
               po.physicobjpozy := longint ((py+1) * 24) shl 8;
               po.physicobjyvelocity := 0;
          end;
          if (cellcol and (LU+RD)=(LU+RD)) then
          begin
               po.physicobjpozx := longint ((px+ppx+1) shl 5-sx) shl 8;
               po.physicobjxvelocity := 0;
               po.physicobjpozy := longint ((py+1) * 24) shl 8;
               po.physicobjyvelocity := 0;
          end;
     end;
end;

{procedury do obsˆugi d«wi©ku xms}

procedure xmssnd_addsample (smpl : tsample; gl : byte);
var
i : byte;
begin
     if snddis = false then exit;
     for i := 1 to nchannels do if channels [i].used = false then break;
     {}
     trapxmssndbuf[i-1].size_of_sample := smpl.size_of_sample;
     trapxmssndbuf[i-1].no := smpl.no;
     if smpl.size_of_sample > trapxmssndbufsize then syserror ('xmssnd_addsample()','Sample is too big');
     xmemmove (smpl.sme_bf,trapxmssndbuf[i-1].sme_bf,smpl.size_of_sample,xms2dosmem);
     {}
     addsample (trapxmssndbuf[i-1],gl,0);
end;

{laduje domyslne ustawienia}

procedure loaddefaults;
begin
     configrec.ussnd := byte (ussndt);
     configrec.usmus := byte (ussndt);
     if sysinf.proccaps and $800000 = $800000 then configrec.usmmx := 1 else configrec.usmmx := 0;
     trapsampleno := 1;
     configrec.scrmode := 0;
     configrec.xscrsx := 320;
     configrec.xscrsy := 200;
     configrec.gamma := 0;
     scrox := 160;
     scroy := 100;
     keypoz := 0;
     setingkeys := 255;
     gamestate := GAMESTATE_TITLE;
     drawtrapfps := true;
     trapframes := 0;
     startstimer (pusf);
     trapbframes := 0;
     iswait4vsync := true;
     {standardowe sterowanie}
     keyactionstab [FIRSTKEY,MOVERIGHT] := KEY_RIGHT;
     keyactionstab [SECONDKEY,MOVERIGHT] := 32; {D}
     keyactionstab [FIRSTKEY,MOVELEFT] := KEY_LEFT;
     keyactionstab [SECONDKEY,MOVELEFT] := 30; {A}
     keyactionstab [FIRSTKEY,JUMP] := KEY_UP;
     keyactionstab [SECONDKEY,JUMP] := KEY_SHIFT;
     keyactionstab [FIRSTKEY,CROUCH] := KEY_DOWN;
     keyactionstab [SECONDKEY,CROUCH] := 46; {C}
     keyactionstab [FIRSTKEY,USE] := KEY_ALT;
     keyactionstab [SECONDKEY,USE] := 18; {U}
     keyactionstab [FIRSTKEY,SHOT] := KEY_CTRL;
     keyactionstab [SECONDKEY,SHOT] := 31; {S}
     {}
     if parameq (params,SAFE_MODE) then
     begin
          vgaonly := true;
          ussndt := false;
          configrec.ussnd := 0;
          configrec.usmus := 0;
          configrec.usmmx := 0;
          trapcrtprint (trapcrtcx+1,trapcrtcy,7,'SAVE_MODE');
     end;
     if parameq (params,NOSOUND_MODE) then
     begin
          ussndt := false;
          trapcrtprint (trapcrtcx+1,trapcrtcy,7,'NOSOUND_MODE');
     end;
     if not parameq (params,NOMUSIC_MODE) then
     begin
          configrec.usmus := 0;
          trapcrtprint (trapcrtcx+1,trapcrtcy,7,'NOMUSIC_MODE');
     end;
end;

{procedury ladowania i zapisywania konfiguracji}

procedure loadconfig;
const
confignames : array [0..5] of pchar = ('MODE','MODET','SOUND','MUSIC','MMX','GAMMA');
actionsnames : array [0..5] of pchar = ('MOVERIGHT','MOVELEFT','JUMP','CROUCH','USE','SHOT');
var
f : file of char;
t1,t2,t3 : pchar;
i,j,k : byte;
c : char;
cd : integer;
begin
     loaddefaults;
     if not parameq (params,DEFAULTS_MODE) then
     begin
          trapcrtprint (trapcrtcx+1,trapcrtcy,7,'DEFAULTS_MODE');
          exit;
     end;
     assign (f,'TRAP.CFG');
     reset (f);
     if ioresult<>0 then begin loaddefaults; exit; end;
     getmem (t1,30);
     getmem (t2,30);
     while not eof (f) do
     begin
          i := 0;
          j := 0;
          read (f,c);
          while (c<>'=') and (c<>#13) do
          begin
               t1[i] := c;
               inc (i);
               if eof (f) then break;
               read (f,c);
          end;
          if c = #13 then read (f,c);
          read (f,c);
          while (c<>#13) do
          begin
               t2[j] := c;
               inc (j);
               if eof (f) then break;
               read (f,c);
          end;
          if (c = #13) and (not eof (f)) then read (f,c);
          t1[i] := #0;
          t2[j] := #0;
          for i := 0 to 6 do begin if i=6 then break; if strcomp (t1,confignames[i])=0 then break; end;
          if i <> 6 then
          begin
          case i of
          0:begin
                 j := 0;
                 c := t2[0];
                 while c<>'X' do begin inc (j); c:=t2[j]; if c=#0 then break; end;
                 if c = #0 then continue;
                 strcopy (t1,t2);
                 t1[j] := #0;
                 val (t1,configrec.xscrsx,cd);
                 if cd <> 0 then continue;
                 inc (j);
                 c := t2[j];
                 k := j;
                 while c<>#0 do begin inc (j); c:=t2[j]; end;
                 strcopy (t1,t2);
                 t3 := t1;
                 t1[j] := #0;
                 t1 := ptr (seg (t1^),ofs (t1^)+k);
                 val (t1,configrec.xscrsy,cd);
                 t1 := t3;
                 if cd <> 0 then continue;
            end;
          1:if strcomp (t2,'VGA')=0 then configrec.scrmode := 0 else if strcomp (t2,'VESA')=0 then configrec.scrmode := 1
          else configrec.scrmode := 0;
          2:if strcomp (t2,'FALSE')=0 then configrec.ussnd := 0 else if strcomp (t2,'TRUE')=0 then
          if configrec.ussnd = 1 then configrec.ussnd := 1 else configrec.ussnd := 0;
          3:if strcomp (t2,'FALSE')=0 then configrec.usmus := 0 else if strcomp (t2,'TRUE')=0 then
          if configrec.usmus = 1 then configrec.usmus := 1 else configrec.usmus := 0;
          4:if strcomp (t2,'FALSE')=0 then configrec.usmmx := 0 else if strcomp (t2,'TRUE')=0 then
          if configrec.usmmx = 1 then configrec.usmmx := 1 else configrec.usmmx := 0;
          5:begin
                 val (t2,configrec.gamma,cd);
                 configrec.gamma := configrec.gamma;
          end;
          end;
          continue;
          end;
          if i = 6 then for i := 0 to 6 do begin if i=6 then break; if strcomp (t1,actionsnames[i])=0 then break; end;
          if i = 6 then continue;
          c := t2[0];
          j := 0;
          while (c <> ',') and (c <> #0) do begin c := t2[j]; inc (j); end;
          strcopy (t1,t2);
          t1[j-1] := #0;
          val (t1,k,cd);
          if cd <> 0 then continue;
          keyactionstab [FIRSTKEY,i] := k;
          k := j;
          c := t2[j];
          while (c <> #0) do begin c := t2[j]; inc (j); end;
          strcopy (t1,t2);
          t3 := t1;
          t1[j] := #0;
          t1 := ptr (seg (t1^),ofs (t1^)+k);
          val (t1,k,cd);
          t1 := t3;
          if cd <> 0 then continue;
          keyactionstab [SECONDKEY,i] := k;
     end;
     freemem (t2,30);
     freemem (t1,30);
     close (f);
end;

procedure saveconfig;
const
confignames : array [0..5] of pchar = ('MODE','MODET','SOUND','MUSIC','MMX','GAMMA');
typenames : array [0..7] of pchar = ('=','X',#13#10,'TRUE','FALSE','VGA','VESA',',');
actionsnames : array [0..5] of pchar = ('MOVERIGHT','MOVELEFT','JUMP','CROUCH','USE','SHOT');
var
f : file;
t : pchar;
i : byte;
begin
     getmem (t,7);
     assign (f,'TRAP.CFG');
     rewrite (f,1);
     {---}
     blockwrite (f,confignames[0]^,4);
     blockwrite (f,typenames [0]^,1);
     val2pchar (configrec.xscrsx,t);
     blockwrite (f,t^,strlen (t));
     blockwrite (f,typenames [1]^,1);
     val2pchar (configrec.xscrsy,t);
     blockwrite (f,t^,strlen (t));
     blockwrite (f,typenames [2]^,2);
     {---}
     blockwrite (f,confignames[1]^,5);
     blockwrite (f,typenames [0]^,1);
     if configrec.scrmode = 0 then
     blockwrite (f,typenames [5]^,3)
     else
     blockwrite (f,typenames [6]^,4);
     blockwrite (f,typenames [2]^,2);
     {---}
     blockwrite (f,confignames[2]^,5);
     blockwrite (f,typenames [0]^,1);
     if configrec.ussnd = 0 then
     blockwrite (f,typenames [4]^,5)
     else
     blockwrite (f,typenames [3]^,4);
     blockwrite (f,typenames [2]^,2);
     {---}
     blockwrite (f,confignames[3]^,5);
     blockwrite (f,typenames [0]^,1);
     if configrec.usmus = 0 then
     blockwrite (f,typenames [4]^,5)
     else
     blockwrite (f,typenames [3]^,4);
     blockwrite (f,typenames [2]^,2);
     {---}
     blockwrite (f,confignames[4]^,3);
     blockwrite (f,typenames [0]^,1);
     if configrec.usmmx = 0 then
     blockwrite (f,typenames [4]^,5)
     else
     blockwrite (f,typenames [3]^,4);
     blockwrite (f,typenames [2]^,2);
     {---}
     blockwrite (f,confignames[5]^,5);
     blockwrite (f,typenames [0]^,1);
     val2pchar (configrec.gamma,t);
     blockwrite (f,t^,strlen (t));
     blockwrite (f,typenames [2]^,2);
     {---}
     for i := 0 to 5 do
     begin
          blockwrite (f,actionsnames[i]^,strlen (actionsnames[i]));
          blockwrite (f,typenames [0]^,1);
          val2pchar (keyactionstab [FIRSTKEY,i],t);
          blockwrite (f,t^,strlen (t));
          blockwrite (f,typenames [7]^,1);
          val2pchar (keyactionstab [SECONDKEY,i],t);
          blockwrite (f,t^,strlen (t));
          blockwrite (f,typenames [2]^,2);
     end;
     close (f);
end;

{procedura wyjscia}

procedure trapexitproc; far;
begin
     saveconfig;
     exitproc := oldexitproc;
     if getscr <> $3 then inittxtscr;
     if configrec.scrmode <> 0 then closevesamode;
     keyis (off);
     if snddis then closesnd;
     if musicplayed then closemus;
     unuspack (trappfrec);
     reallocxmsheap (0);
     freexmsheap;
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
          animtexts[ianimtexts].texmemory := allocmem (animtexts[ianimtexts].sizex*animtexts[ianimtexts].sizey,0);
          blockreadpfile (trapfilehandle,animtexts[ianimtexts].texmemory^,animtexts[ianimtexts].sizex*
          animtexts[ianimtexts].sizey);
          addtextoanim (animtexts[ianimtexts],i,oa);
          inc (ianimtexts);
     end;
     closepfile (trapfilehandle);
end;

procedure load832 (fnamm : pchar; var t : ttex; memt : byte);
var
pbufer : pointer;
pages,modpages : word;
i : word;
begin
     assignpfile (trapfilehandle,trappfrec,fnamm);
     if pfileior <> 0 then syserror ('load832()',getpfior);
     resetpfile (trapfilehandle);
     blockreadpfile (trapfilehandle,t.sizex,4);
     if memt = DOSMEM then
     begin
          t.texmemory := allocmem (t.sizex*t.sizey,0);
          blockreadpfile (trapfilehandle,t.texmemory^,t.sizex*t.sizey);
     end;
     if memt = XMSMEM then
     begin
          getmem (pbufer,256);
          xmemget (t.texmemory,t.sizex*t.sizey);
          pages := (t.sizex*t.sizey) shr 8;
          modpages := (t.sizex*t.sizey) mod 256;
          for i := 0 to pages-1 do
          begin
               blockreadpfile (trapfilehandle,pbufer^,256);
               xmemmove (pbufer,pointer (longint (t.texmemory)+i*256),256,dosmem2xms);
          end;
          if modpages <> 0 then
          begin
               blockreadpfile (trapfilehandle,pbufer^,modpages);
               xmemmove (pbufer,pointer (longint (t.texmemory)+(i+1)*256+modpages),modpages,dosmem2xms);
          end;
          freemem (pbufer,256);
     end;
     closepfile (trapfilehandle);
end;

procedure loadwave (fnamm : pchar; var s : tsample; no,memt : byte);
var
pbufer : pointer;
pages,modpages : word;
i : word;
begin
     assignpfile (trapfilehandle,trappfrec,fnamm);
     if pfileior <> 0 then syserror ('loadwave()',getpfior);
     resetpfile (trapfilehandle);
     blockreadpfile (trapfilehandle,s.size_of_sample,2);
     if no = 1 then
     begin
          s.no := trapsampleno;
          inc (trapsampleno);
     end;
     if memt = DOSMEM then
     begin
          s.sme_bf := allocmem (s.size_of_sample,0);
          blockreadpfile (trapfilehandle,s.sme_bf^,s.size_of_sample);
     end;
     if memt = XMSMEM then
     begin
          getmem (pbufer,256);
          xmemget (s.sme_bf,s.size_of_sample);
          pages := s.size_of_sample shr 8;
          modpages := s.size_of_sample mod 256;
          for i := 0 to pages-1 do
          begin
               blockreadpfile (trapfilehandle,pbufer^,256);
               xmemmove (pbufer,pointer (longint (s.sme_bf)+i*256),256,dosmem2xms);
          end;
          if modpages <> 0 then
          begin
               blockreadpfile (trapfilehandle,pbufer^,modpages);
               xmemmove (pbufer,pointer (longint (s.sme_bf)+(i+1)*256),modpages,dosmem2xms);
          end;
          freemem (pbufer,256);
     end;
     closepfile (trapfilehandle);
end;

procedure loadmidi (fnamm : pchar; m : pointer; var s : word);
begin
     _32fill (trapmidimusic,midimusicmaxsize,255);
     assignpfile (trapfilehandle,trappfrec,fnamm);
     if pfileior <> 0 then syserror ('loadmidi()',getpfior);
     resetpfile (trapfilehandle);
     blockreadpfile (trapfilehandle,s,2);
     if s > midimusicmaxsize then syserror ('loadmidi()','Midi file is to big');
     blockreadpfile (trapfilehandle,m^,s);
     closepfile (trapfilehandle);
end;

procedure getamodeg;
var
i : word;
begin
     amodeg := 0;
     if configrec.scrmode = 0 then begin amodeg := 255; exit; end;
     for i := 0 to imodes do
     begin
          if (vesacaps.modes[i].width = configrec.xscrsx) and (vesacaps.modes[i].height = configrec.xscrsy) then break;
     end;
     if i <> imodes then amodeg := i;
end;

procedure makemodesnames;
var
i,j : word;
ppchar1 : pchar;
begin
     if vgaonly then
     begin
          imodes := 0;
          mmodeg := 255;
          amodeg := 255;
          exit;
     end;
     j := 0;
     while vesacaps.modes[j].width <> 0 do inc (j);
     for i := 0 to j-1 do
     begin
          strcopy (modesnames[i],'');
     end;
     getmem (ppchar1,5);
     for i := 0 to j-1 do
     begin
          val2pchar (vesacaps.modes[i].width,ppchar1);
          strcat (modesnames[i],ppchar1);
          strcat (modesnames[i],'X');
          val2pchar (vesacaps.modes[i].height,ppchar1);
          strcat (modesnames[i],ppchar1);
     end;
     freemem (ppchar1,5);
     imodes := j-1;
     getamodeg;
     mmodeg := amodeg;
end;

procedure initmode (mode : byte);
begin
     if (mode > imodes) and (mode <> 255) then
     begin
          con_addstr (trapconsole,'CAN''T SET MODE');
          exit;
     end;
     if mode = 255 then
     begin
          initvgascr;
          configrec.scrmode := 0;
          setgammapal (configrec.gamma);
          configrec.xscrsx := 320;
          configrec.xscrsy := 200;
          scrox := 160;
          scroy := 100;
          mmodeg := mode;
          amodeg := mode;
     end
     else
     begin
          if not setvesamode (vesacaps.modes[mode].width,vesacaps.modes[mode].height) then
          syserror ('execmenucommand()','Can''t set VESA mode');
          configrec.scrmode := 1;
          setgammapal (configrec.gamma);
          configrec.xscrsx := vesacaps.modes[mode].width;
          configrec.xscrsy := vesacaps.modes[mode].height;
          configrec.xsbx1 := configrec.xscrsx div 2-160;
          configrec.xsbx2 := configrec.xscrsx div 2+159;
          configrec.xsby1 := configrec.xscrsy div 2-100;
          configrec.xsby2 := configrec.xscrsy div 2+99;
          scrox := configrec.xscrsx div 2;
          scroy := configrec.xscrsy div 2;
          xscrscrsx := (configrec.xscrsx-1) div 320;
          xscrscrsy := (configrec.xscrsy-1) div 200;
          mmodeg := mode;
          amodeg := mode;
     end;
end;

procedure createsysinfotxt;
var
ppchar2 : pchar;
begin
     getmem (ppchar2,9);
     strcopy (sysinfotxt,sysinfotxts[0]);
     strcat (sysinfotxt,sysinf.procname);
     strcat (sysinfotxt,#13);
     {}
     strcat (sysinfotxt,sysinfotxts[1]);
     val2pchar (sysinf.procclk,ppchar2);
     strcat (sysinfotxt,ppchar2);
     strcat (sysinfotxt,#13);
     {}
     strcat (sysinfotxt,sysinfotxts[2]);
     val2pchar (lo (sysinf.dosver),ppchar2);
     strcat (sysinfotxt,ppchar2);
     strcat (sysinfotxt,'.');
     val2pchar (hi (sysinf.dosver),ppchar2);
     strcat (sysinfotxt,ppchar2);
     strcat (sysinfotxt,#13);
     {}
     strcat (sysinfotxt,sysinfotxts[3]);
     val2pchar (hi (sysinf.winver),ppchar2);
     strcat (sysinfotxt,ppchar2);
     strcat (sysinfotxt,'.');
     val2pchar (lo (sysinf.winver),ppchar2);
     strcat (sysinfotxt,ppchar2);
     strcat (sysinfotxt,#13);
     {}
     strcat (sysinfotxt,sysinfotxts[4]);
     val2pchar (sysinf.dosmemfree,ppchar2);
     strcat (sysinfotxt,ppchar2);
     strcat (sysinfotxt,#13);
     {}
     strcat (sysinfotxt,sysinfotxts[5]);
     val2pchar (sysinf.xmsmemfree,ppchar2);
     strcat (sysinfotxt,ppchar2);
     strcat (sysinfotxt,#13);
     {}
     if vgaonly = false then
     begin
     strcat (sysinfotxt,sysinfotxts[6]);
     strcat (sysinfotxt,vesacaps.name);
     strcat (sysinfotxt,#13);
     {}
     strcat (sysinfotxt,sysinfotxts[7]);
     val2pchar (vesacaps.mem div 16,ppchar2);
     strcat (sysinfotxt,ppchar2);
     strcat (sysinfotxt,#13);
     {}
     strcat (sysinfotxt,sysinfotxts[8]);
     val2pchar (hi (vesacaps.ver),ppchar2);
     strcat (sysinfotxt,ppchar2);
     strcat (sysinfotxt,'.');
     val2pchar (lo (vesacaps.ver),ppchar2);
     strcat (sysinfotxt,ppchar2);
     end;
     {}
     freemem (ppchar2,9);
end;

procedure gettrapcurrmap;
var
p : pointer;
begin
     with trapcurrmap do
     begin
          trapmap := allocmem (trapmapsize*6,0);
          _32fill (trapmap,trapmapsize*6,0);
          trapmapsizex := 0;
          trapmapsizey := 0;
          getmem (p,maxinforecs*sizeof (trapinforec));
          trapinforecseg := seg (p^);
          getmem (p,scriptslen);
          trapscriptseg := seg (p^);
     end;
end;

procedure inittrapxmssndbuf;
var
i : byte;
begin
     for i := 0 to nchannels-1 do
     begin
          trapxmssndbuf [i].sme_bf := allocmem (trapxmssndbufsize,0);
          _32fill (trapxmssndbuf [i].sme_bf,trapxmssndbufsize,80);
     end;
end;

procedure inittexttex;
type
at = array [0..99] of byte;
pt = ^at;
var
i : byte;
c : byte;
begin
     texttex1.sizex := 10;
     texttex1.sizey := 10;
     texttex2.sizex := 10;
     texttex2.sizey := 10;
     getmem (texttex1.texmemory,100);
     getmem (texttex2.texmemory,100);
     for i := 0 to 99 do
     begin
          c := random (7);
          pt (texttex1.texmemory)^[i] := c+(32*0)+25;
          pt (texttex2.texmemory)^[i] := c+(32*3)+25;
     end;
end;

procedure inittrapconsole;
begin
     getmem (editconsolepchar,consolestrlen);
     getmem (editconsolepcharl,consolestrlen);
     _32fill (editconsolepcharl,consolestrlen,0);
     editconsole := false;
     editconsolepchars := 0;
     _32fill (@trapconsole,sizeof (trapconsole),0);
end;

procedure inittrapparts;
var
i : byte;
begin
     for i := 0 to maxparts-1 do
     begin
          trapparts[i].used := false;
     end;
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

procedure loadtxt (fnamm : pchar; arr : pointer; s : word; n : word);
var
p : pchar;
ppchar : pchar;
i : byte;
begin
     getmem (ppchar,256);
     p := arr;
     assignpfile (trapfilehandle,trappfrec,fnamm);
     if pfileior <> 0 then syserror ('loadtxt()',getpfior);
     resetpfile (trapfilehandle);
     for i := 0 to n-1 do
     begin
          readline (ppchar,256);
          strcopy (p,ppchar);
          inc (p,s);
     end;
     closepfile (trapfilehandle);
     freemem (ppchar,256);
end;

procedure load2pchar (fnamm : pchar; b : pchar; s : word);
var
c : char;
i : word;
begin
     assignpfile (trapfilehandle,trappfrec,fnamm);
     if pfileior <> 0 then syserror ('load2buf()',getpfior);
     resetpfile (trapfilehandle);
     i := 0;
     while not pfileeof (trapfilehandle) do
     begin
          blockreadpfile (trapfilehandle,c,1);
          if c <> #13 then
          begin
               b[i] := c;
               inc (i);
          end;
          if i >= s then break;
     end;
     b[i] := #0;
     closepfile (trapfilehandle);
end;

{Zarzadzanie mapa}

procedure resetworldphysicobj;
begin
     worldphysicobj.physicobjmaxxvel := $7fffffff;
     worldphysicobj.physicobjxaccel := 0;
     {}
     worldphysicobj.physicobjmaxyvel := $7fffffff;
     worldphysicobj.physicobjyaccel := STDWORLDGRAVITY;
     floatint_mul (watervel,worldphysicobj.physicobjyaccel,256+STDW);
end;

procedure resetplayerphysicobj;
begin
     with trapcurrmap.players[0] do
     begin
          playerphysicobj.physicobjpozx := longint (playerpozx) shl 8;
          playerphysicobj.physicobjpozy := longint (playerpozy) shl 8;
          playerphysicobj.physicobjxvelocity := 0;
          playerphysicobj.physicobjyvelocity := 0;
          playerphysicobj.physicobjyaccel := worldphysicobj.physicobjyaccel;
     end;
     playersteps := 2;
     playerisongrab := false;
     playerdeltayvel := 0;
     playeriskilled := false;
     plkilledt := 0;
     godmode := false;
     noclipmode := false;
     playerisuse := false;
     playeronmap := true;
     playershoting := false;
     {_32fill (@trapsammo,sizeof (tsammo)*maxshotammo,0);}
end;

procedure resettrapcurrmap;
var
i : byte;
begin
     with trapcurrmap do
     begin
          trapmapsizex := 0;
          trapmapsizey := 0;
          nplayers := 0;
          nmonsters := 0;
          _32fill (trapmap,trapmapsize*6,0);
     end;
     {}
     with trapcurrmap.players[0] do
     begin
          playerpozx := 0;
          playerpozy := 0;
          health := PLAYERHEALTH;
          state := 0;
          igun := 1;
          _32fill (@playerguns,maxguns,0);
          for i := 0 to maxguns-1 do playerguns[i] := false;
          playerguns[0] := true;
          playerguns[1] := true;
          _32fill (@playerammo,maxammotypes*2,0);
          playerammo[0] := gammostd;
          playerammo[1] := 0;
          playerammo[2] := 0;
          playerammo[3] := 0;
          watertime := 0;
     end;
     resetworldphysicobj;
     mtrapmapsizex := 0;
     mtrapmapsizey := 0;
     trapmapviewshfx := 0;
     trapmapviewshfy := 0;
     maploaded := false;
     playerkeys[0] := false;
     playerkeys[1] := false;
     playerkeys[2] := false;
     gamepaused := false;
     for i := 0 to maxshotammo-1 do
     begin
          trapsammo[0].used := false;
     end;
end;

function loadmap (var mapname : pchar) : boolean;
var
trapmaphead : pchar;
i : word;
c : char;
t : pointer;
cell,x,y : byte;
maps : word;
begin
     maploaded := false;
     loadmap := false;
     {}
     assignpfile (trapfilehandle,trappfrec,mapname);
     if pfileior <> 0 then exit;
     resetpfile (trapfilehandle);
     {}
     getmem (trapmaphead,8);
     blockreadpfile (trapfilehandle,trapmaphead^,8);
     if pfileior <> 0 then exit;
     if strcomp (trapmaphead,'TRAPMAP') <> 0 then exit;
     freemem (trapmaphead,8);
     {}
     _32fill (trapcurrmapname,256,0);
     _32fill (trapcurrmapmusic,256,0);
     i := 0;
     c := #255;
     while (c <> #0) do
     begin
          blockreadpfile (trapfilehandle,c,1);
          trapcurrmapmusic[i] := c;
          inc (i);
     end;
     {}
     i := 0;
     c := #255;
     while (c <> #0) do
     begin
          blockreadpfile (trapfilehandle,c,1);
          trapcurrmapname[i] := c;
          inc (i);
     end;
     {}
     with trapcurrmap do
     begin
          blockreadpfile (trapfilehandle,trapmapsizex,2);
          blockreadpfile (trapfilehandle,trapmapsizey,2);
          {}
          blockreadpfile (trapfilehandle,nplayers,1);
          if nplayers = 0 then syserror ('loadmap()','No players in map');
          for i := 1 to nplayers do
          begin
               blockreadpfile (trapfilehandle,players[i-1].playerpozx,2);
               blockreadpfile (trapfilehandle,players[i-1].playerpozy,2);
               blockreadpfile (trapfilehandle,players[i-1].state,1);
          end;
          {}
          blockreadpfile (trapfilehandle,nmonsters,1);
          if nmonsters <> 0 then
          for i := 1 to nmonsters do
          begin
               blockreadpfile (trapfilehandle,monsters[i-1].pozx,2);
               blockreadpfile (trapfilehandle,monsters[i-1].pozy,2);
               blockreadpfile (trapfilehandle,monsters[i-1].state,1);
               blockreadpfile (trapfilehandle,monsters[i-1].mtype,1);
          end;
          {}
          {wczytywanie caˆej mapy (poziomu)}
          blockreadpfile (trapfilehandle,trapmap^,trapmapsizex*trapmapsizey*6);
          if pfileior <> 0 then syserror ('loadmap()',getpfior);
          blockreadpfile (trapfilehandle,i,1);
          trapinforecs := i;
          if i > maxinforecs then syserror ('loadmap()','Too many inforecs');
          t := ptr (trapinforecseg,0);
          blockreadpfile (trapfilehandle,t^,i*sizeof (trapinforec));
          blockreadpfile (trapfilehandle,i,2);
          if i > scriptslen then syserror ('loadmap()','Too big script');
          t := ptr (trapscriptseg,0);
          blockreadpfile (trapfilehandle,t^,i);
          trapscriptssize := i;
          {}
          mtrapmapsizex := trapmapsizex*32;
          mtrapmapsizey := trapmapsizey*24;
          if players[0].state >= 6 then pcrouch := true
          else
          pcrouch := false;
          {}
          {ustawiamy tablice inforecxyctab}
          maps := trapmapsizex*trapmapsizey-1;
          x := 0;
          y := 0;
          for i := 0 to maps do
          begin
               cell := trapmap^[i].trapinforec1;
               if (cell > 0) and (cell <= maxinforecs) then
               begin
                    inforecxyctab[cell-1].x := x;
                    inforecxyctab[cell-1].y := y;
                    inforecxyctab[cell-1].c := 0;
               end
               else
               begin
                    cell := trapmap^[i].trapinforec2;
                    if (cell > 0) and (cell <= maxinforecs) then
                    begin
                         inforecxyctab[cell-1].x := x;
                         inforecxyctab[cell-1].y := y;
                         inforecxyctab[cell-1].c := 1;
                    end;
               end;
               inc (x);
               if x >= trapmapsizex then
               begin
                    x := 0;
                    inc (y);
               end;
          end;
     end;
     {}
     closepfile (trapfilehandle);
     maploaded := true;
     loadmap := true;
end;

procedure initmap (mapname : pchar);
var
ppchar : pchar;
begin
     resettrapcurrmap;
     if not loadmap (mapname) then
     begin
          getmem (ppchar,100);
          strcopy (ppchar,'MAP ');
          strcat (ppchar,mapname);
          strcat (ppchar,' NOT FOUND!');
          con_addstr (trapconsole,ppchar);
          freemem (ppchar,100);
          gamestate := GAMESTATE_TITLE;
     end
     else
     begin
          gamestate := GAMESTATE_GAME;
          resetplayerphysicobj;
          if trapcurrmapname[0] <> #0 then con_addstr (trapconsole,trapcurrmapname);
     end;
     startstimer (trapfpstimer);
     trapframes := 0;
     trapbframes := 0;
     startstimer (pusf);
     startstimer (pauset);
     playerdeltahealth := 0;
     playerdeltahealthf := 0;
end;

{Wczytywanie menu}
procedure loadmenu;
var
pdmenu : tmenu;
pdmenuptr : pchar;
pdcmenuptr : pchar;
pdhmenuptr : pchar;
pdtmenuptr : pchar;
_ppchar : array [0..255] of char;
_ppcharn : array [0..127] of char;
ptbl1,ptbl2 : array [0..49] of pchar;
ppchar,ppcharn : pchar;
prevm,currm : pmenu;
i,j,k,valr : word;
pcode : integer;
pwsk1,pwsk2 : ptxts;
begin
     _32fill (@pdmenu,sizeof (tmenu),0);
     pdmenu.menuhelp := '';
     assignpfile (trapfilehandle,trappfrec,'MENU.TXT');
     if pfileior <> 0 then syserror ('loadmenu()',getpfior);
     resetpfile (trapfilehandle);
     {pdmenuptr := ptr ($b800,0);
     pdcmenuptr := ptr ($b860,0);
     pdhmenuptr := ptr ($b8c0,0);
     pdtmenuptr := ptr ($b8d0,0);
     _32fill (ptr ($b800,0),$1000,0);}
     getmem (pdmenuptr,1000);
     getmem (pdcmenuptr,1000);
     getmem (pdhmenuptr,400);
     getmem (pdtmenuptr,400);
     prevm := nil;
     currm := nil;
     ppchar := _ppchar;
     ppcharn := _ppcharn;
     valr := 0;
     j := 0;
     k := 0;
     {}
     while not pfileeof (trapfilehandle) do
     begin
          readline (ppchar,256);
          if ppchar[0] = '$' then
          begin
               if (ppchar[1] in ['0'..'9']) then
               begin
                    if valr <> 0 then
                    begin
                         pdmenu.numoftxts := j;
                         pdmenu.numofcoms := k;
                         getmem (pwsk1,j*4);
                         getmem (pwsk2,k*4);
                         _32move (@ptbl1,pwsk1,j*4);
                         _32move (@ptbl2,pwsk2,k*4);
                         pdmenu.menutxts := pwsk1;
                         pdmenu.menucoms := pwsk2;
                         createmenu (prevm,currm,pdmenu);
                         prevm := currm;
                         if valr=1 then trapmainmenu := currm;
                         j := 0;
                         k := 0;
                         pdmenu.menuhelp := '';
                         pdmenu.menutitle := '';
                    end;
                    valr := 0;
                    i := 1;
                    while (ppchar[i] in ['0'..'9']) do
                    begin
                         ppcharn[i-1] := ppchar[i];
                         inc (i);
                    end;
                    ppcharn[i-1] := #0;
                    val (ppcharn,valr,pcode);
                    if pcode <> 0 then syserror ('loadmenu()','Bad menu.txt file!');
                    pdmenu.menuid := valr;
                    getstr (ppchar,ppcharn,i);
                    strcopy (pdtmenuptr,ppcharn);
                    pdmenu.menutitle := pdtmenuptr;
                    inc (pdtmenuptr,strlen (ppcharn)+2);
               end;
               if ppchar[1] = '"' then
               begin
                    getstr (ppchar,ppcharn,1);
                    strcopy (pdhmenuptr,ppcharn);
                    pdmenu.menuhelp := pdhmenuptr;
                    inc (pdhmenuptr,strlen (ppcharn)+2);
               end;
          end;
          if ppchar[0] = '"' then
          begin
               getstr (ppchar,ppcharn,0);
               strcopy (pdmenuptr,ppcharn);
               ptbl1[j] := pdmenuptr;
               inc (pdmenuptr,strlen(ppcharn)+2);
               inc (j);
               if (ppchar[strlen (ppcharn)+2] = '$') then
               begin
                    getcom (ppchar,ppcharn,strlen (ppcharn)+3);
                    strcopy (pdcmenuptr,ppcharn);
                    ptbl2[k] := pdcmenuptr;
                    if strcomp (ppcharn,'LISTKEYS') = 0 then listkeysmenu := valr;
                    inc (pdcmenuptr,strlen(ppcharn)+2);
                    inc (k);
               end;
          end;
     end;
     closepfile (trapfilehandle);
     trapcurrmenu := trapmainmenu;
     if valr <> 0 then
     begin
          pdmenu.numoftxts := j;
          pdmenu.numofcoms := k;
          getmem (pwsk1,j*4);
          getmem (pwsk2,k*4);
          _32move (@ptbl1,pwsk1,j*4);
          _32move (@ptbl2,pwsk2,k*4);
          pdmenu.menutxts := pwsk1;
          pdmenu.menucoms := pwsk2;
          createmenu (prevm,currm,pdmenu);
          prevm := currm;
          if valr=1 then trapmainmenu := currm;
          j := 0;
          k := 0;
     end;
     closepfile (trapfilehandle);
end;

procedure loaditems;
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
     assignpfile (trapfilehandle,trappfrec,'ITEMS.TXT');
     if pfileior <> 0 then syserror ('loaditems()',getpfior);
     resetpfile (trapfilehandle);
     skeyitems := 255;
     sammoitems := 255;
     i := 0;
     while not pfileeof (trapfilehandle) do
     begin
          readline (ppchar,256);
          k := 0;
          j := 0;
          _32fill (ppval,14,0);
          while ppchar[k] <> ' ' do
          begin
               ppval[j] := ppchar[k];
               inc (j);
               inc (k);
          end;
          val (ppval,items[i].id,co);
          if co <> 0 then syserror ('loaditems()','Bad ITEMS.TXT file');
          if items[i].id = KEYS_ITEMS then
          if skeyitems = 255 then skeyitems := i;
          {}
          if items[i].id = AMMO_ITEMS then
          if sammoitems = 255 then sammoitems := i;
          {}
          inc (k);
          j := 0;
          _32fill (ppval,14,0);
          while ppchar[k] <> ' ' do
          begin
               ppval[j] := ppchar[k];
               inc (j);
               inc (k);
          end;
          val (ppval,items[i].idval1,co);
          if co <> 0 then syserror ('loaditems()','Bad ITEMS.TXT file');
          {}
          inc (k);
          j := 0;
          _32fill (ppval,14,0);
          while ppchar[k] <> ' ' do
          begin
               ppval[j] := ppchar[k];
               inc (j);
               inc (k);
          end;
          val (ppval,items[i].idval2,co);
          if co <> 0 then syserror ('loaditems()','Bad ITEMS.TXT file');
          {}
          inc (k);
          j := 0;
          _32fill (ppval,14,0);
          while ppchar[k] <> #0 do
          begin
               ppval[j] := ppchar[k];
               inc (j);
               inc (k);
          end;
          ppfilepos := filepos (pf);
          pffile := trapfilehandle;
          npffile := nextfile;
          load832 (ppval,trapitemssprites[i],DOSMEM);
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

procedure loadmonsters;
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
     assignpfile (trapfilehandle,trappfrec,'MONSTERS.TXT');
     if pfileior <> 0 then syserror ('loadmonsters()',getpfior);
     resetpfile (trapfilehandle);
     i := 0;
     while not pfileeof (trapfilehandle) do
     begin
          readline (ppchar,256);
     end;
end;

procedure loadall832;
var
i : word;
ppfilepos : longint;
pffile,npffile : fileor;
__ppchar : array [0..255] of char;
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
     while not pfileeof (trapfilehandle) do
     begin
          readline (ppchar,256);
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
                    load832 (ppchar,traptextures[ntraptextures],DOSMEM);
                    inc (ntraptextures);
               end;
               if i = 1 then
               begin
                    load832 (ppchar,trapsprites[ntrapsprites],DOSMEM);
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
     closepfile (trapfilehandle);
end;

procedure loadallsounds;
var
i : word;
m,o,p,g : byte;
ppfilepos : longint;
pffile,npffile : fileor;
__ppchar : array [0..255] of char;
ppchar : pchar;
begin
     ppchar := @__ppchar;
     assignpfile (trapfilehandle,trappfrec,'FILESSND.TXT');
     if pfileior <> 0 then syserror ('loadallsounds()',getpfior);
     resetpfile (trapfilehandle);
     i := 0;
     m := 0;
     o := 0;
     p := 0;
     g := 0;
     while not pfileeof (trapfilehandle) do
     begin
          readline (ppchar,256);
          if strcomp (ppchar,'[MONSTERS]') = 0 then i := 0
          else
          if strcomp (ppchar,'[OTHER]') = 0 then i := 1
          else
          if strcomp (ppchar,'[PLAYER]') = 0 then i := 2
          else
          if strcomp (ppchar,'[GUNS]') = 0 then i := 3
          else
          begin
               ppfilepos := filepos (pf);
               pffile := trapfilehandle;
               npffile := nextfile;
               if i = 0 then
               begin
                    loadwave (ppchar,monstersounds[m],XMSMEM,1);
                    inc (m);
               end;
               if i = 1 then
               begin
                    loadwave (ppchar,othersounds[o],XMSMEM,1);
                    inc (o);
               end;
               if i = 2 then
               begin
                    loadwave (ppchar,playersounds[p],XMSMEM,1);
                    inc (p);
               end;
               if i = 3 then
               begin
                    loadwave (ppchar,gunssounds[g],XMSMEM,1);
                    inc (g);
               end;
               trapfilehandle := pffile;
               nextfile := npffile;
               pfus := true;
               seek (pf,ppfilepos);
          end;
     end;
     if m <> maxmonstersounds then syserror ('loadallsounds()','Can''t load sounds');
     if o <> maxothersounds then syserror ('loadallsounds()','Can''t load sounds');
     if p <> maxplayersounds then syserror ('loadallsounds()','Can''t load sounds');
     if g <> maxgunssounds then syserror ('loadallsounds()','Can''t load sounds');
     closepfile (trapfilehandle);
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
               _32fill (ppchar2,10,0);
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
               val (ppchar2,playerphysicobj.physicobjmaxxvel,c);
               maxpvel := playerphysicobj.physicobjmaxxvel;
               if c <> 0 then syserror ('loadplayer()','Bad PLAYER.TXT file');
               {}
               _32fill (ppchar2,10,0);
               k := 0;
               while ppchar[l] <> #0 do
               begin
                    ppchar2[k] := ppchar[l];
                    inc (k);
                    inc (l);
                    if k > 10 then syserror ('loadplayer()','Bad PLAYER.TXT file');
               end;
               val (ppchar2,playerphysicobj.physicobjxaccel,c);
               if c <> 0 then syserror ('loadplayer()','Bad PLAYER.TXT file');
               freemem (ppchar2,10);
               {}
               playerphysicobj.physicobjmaxyvel := worldphysicobj.physicobjmaxyvel;
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
          _32fill (ppval,14,0);
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
          _32fill (ppval,14,0);
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
          _32fill (ppval,14,0);
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
          _32fill (ppval,14,0);
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
          _32fill (ppval,14,0);
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
          _32fill (ppval,14,0);
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
          _32fill (ppval,14,0);
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
     freemem (ppval,14);
     freemem (ppchar,256);
     closepfile (trapfilehandle);
end;

{laduje wszystkie pliki}

procedure loadallfiles;
const
loadstr : pchar = 'LOADING';
col = 118;
var
i : word;
begin
     if configrec.scrmode <> 0 then clearvesascr (col);
     _32fill (buf,64000,col);
     textsetcolor (127);
     textprint (161-(strlen (loadstr)*11) div 2,101-5,0,0,loadstr);
     textsetcolor (110);
     textprint (159-(strlen (loadstr)*11) div 2,99-5,0,0,loadstr);
     textsetcolor (116);
     textprint (160-(strlen (loadstr)*11) div 2,100-5,0,0,loadstr);
     if configrec.scrmode = 0 then
     buftoscr
     else
     begin
          xbuftoscr (configrec.xsbx1,configrec.xsby1,configrec.xsbx2,configrec.xsby2);
          flipscr;
     end;
     {}
     resetworldphysicobj;
     inittrapconsole;
     inittrapparts;
     inittexttex;
     inittrapxmssndbuf;
     gettrapcurrmap;
     s_init;
     {Wczytywanie menu}
     loadmenu;
     loaditems;
     loadguns;
     {wczytywanie plikow graficznych}
     loadplayer;
     loadall832;
     loadallsounds;
     {}
     loadtxt ('ACTNAMES.TXT',@actionnames,30,6);
     loadtxt ('SYSINFO.TXT',@sysinfotxts,60,9);
     load2pchar ('HELP.TXT',@traphelp,500);
     {}
     loadanim ('TRAPCUR.ANI',trapcur);
     for i := 0 to MENU832N-1 do load832 (menu832names[i],menu832texs[i],DOSMEM);
     for i := 0 to MENUTWVN-1 do loadwave (menutwvnames[i],menutwvs[i],1,DOSMEM);
     createsysinfotxt;
     load832 ('TRAPTITL.832',traptitle,XMSMEM);
     if configrec.usmus = 1 then
     loadmidi ('TRAPTITL.MMM',trapmidimusic,trapmidimusicsize);
     makemodesnames;
     if configrec.usmus = 1 then
     initmus ($330,trapmidimusic,trapmidimusicsize,0);
     startstimer (titletimer);
     if configrec.scrmode <> 0 then clearvesascr (0);
     startstimer (trapfpstimer);
     trapfps := sysinf.vsyncfreq shl 8;
end;

procedure tex320x200 (t : ttex; m : byte);
begin
     if m = DOSMEM then
     _32move (t.texmemory,buf,64000);
     if m = XMSMEM then
     xmemmove (t.texmemory,buf,64000,xms2dosmem);
end;

procedure execmenucommand (command : pchar; p : shortint);
var
pval : word;
e : integer;
scancode1,scancode2 : byte;
c : char;
begin
     if p = 0 then
     begin
     if strcomp (command,'UP') = 0 then
     begin
          addsample (menutwvs[0],128,0);
          if trapcurrmenu^.menuid = listkeysmenu then
          begin
               if strcomp (trapcurrmenu^.menucoms^[trapcurrmenu^.menupoz],'LISTKEYS') = 0 then
               begin
                     dec (keypoz);
                     if keypoz = -1 then
                     begin
                          dec (trapcurrmenu^.menupoz);
                          keypoz := 6;
                     end;
               end
               else
               begin
                    dec (trapcurrmenu^.menupoz);
                    keypoz := 5;
               end;
          end
          else
          begin
               dec (trapcurrmenu^.menupoz);
          end;
          exit;
     end;
     {}
     if strcomp (command,'DOWN') = 0 then
     begin
          addsample (menutwvs[0],128,0);
          if trapcurrmenu^.menuid = listkeysmenu then
          begin
               if strcomp (trapcurrmenu^.menucoms^[trapcurrmenu^.menupoz],'LISTKEYS') = 0 then
               begin
                    inc (keypoz);
                    if keypoz = 6 then inc (trapcurrmenu^.menupoz);
                    if keypoz = 7 then keypoz := 0;
               end
               else
               begin
                    inc (trapcurrmenu^.menupoz);
                    keypoz := 0;
               end;
          end
          else
          begin
               inc (trapcurrmenu^.menupoz);
          end;
          exit;
     end;
     val (command,pval,e);
     if pval = levelmenu then curepizod := trapcurrmenu^.menupoz;
     if e = 0 then gotoitem (trapcurrmenu,pval);
     {}
     addsample (menutwvs[1],128,0);
     {}
     if strcomp (command,'RETURN') = 0 then
     begin
          trapcurrmenu := trapcurrmenu^.prevmenu;
          if trapcurrmenu = nil then
          begin
               trapcurrmenu := trapmainmenu;
               trapmenuon := false;
          end;
          mmodeg := amodeg;
     end;
     {}
     if strcomp (command,'EXIT') = 0 then halt (0);
     if strcomp (command,'SOUNDONOFF') = 0 then if ussndt then
     begin
          configrec.ussnd := 1-configrec.ussnd;
          snddis := boolean (configrec.ussnd);
     end;
     if strcomp (command,'MUSICONOFF') = 0 then if ussndt then
     begin
          configrec.usmus := 1-configrec.usmus;
          musicplayed := boolean (configrec.usmus);
          pausemus;
          {initmus ($330,trapmidimusic,trapmidimusicsize,0);}
     end;
     if strcomp (command,'MMXONOFF')   = 0 then if sysinf.proccaps and $800000 = $800000 then
     configrec.usmmx := 1-configrec.usmmx;
     {}
     if strcomp (command,'LISTMODES')  = 0 then
     begin
          initmode (mmodeg);
     end;
     if strcomp (command,'LISTKEYS') = 0 then
     begin
          setingkeys := 0;
          stdkeyint := false;
          setnumlock (on);
          if numlockp = 1 then pressnumlock;
     end;
     end;
     {}
     if p = 1 then
     begin
     pval := 0;
     if strcomp (command,'LISTMODES') = 0 then
     begin
          if not vgaonly then
          inc (mmodeg);
          if mmodeg > imodes then mmodeg := 255;
          pval := 1;
     end;
     {}
     if strcomp (command,'GAMMATRACK') = 0 then
     begin
          inc (configrec.gamma,2);
          if configrec.gamma > 8 then configrec.gamma := 8;
          setgammapal (configrec.gamma);
          pval := 1;
     end;
     if pval = 1 then addsample (menutwvs[0],128,0);
     end;
     {}
     if p = -1 then
     begin
     pval := 0;
     if strcomp (command,'LISTMODES') = 0 then
     begin
          if not vgaonly then
          if mmodeg = 255 then mmodeg := imodes
          else
          dec (mmodeg);
          pval := 1;
     end;
     {}
     if strcomp (command,'GAMMATRACK') = 0 then
     begin
          dec (configrec.gamma,2);
          if configrec.gamma < -8 then configrec.gamma := -8;
          setgammapal (configrec.gamma);
          pval := 1;
     end;
     if pval = 1 then addsample (menutwvs[0],128,0);
     end;
end;

procedure execconsolecommand (command : pchar);
const
maxparams = 4;
onoff : array [0..1] of pchar = ('OFF','ON');
var
ppchar,ppchar2 : pchar;
params : array [0..maxparams-1] of pchar;
i,j,k : word;
l : integer;
c,bc : char;
err : integer;
begin
     getmem (ppchar,consolestrlen);
     getmem (ppchar2,256);
     _32fill (ppchar,consolestrlen,0);
     for i := 0 to maxparams-1 do
     begin
          getmem (params[i],consolestrlen);
          _32fill (params[i],consolestrlen,0);
     end;
     {pobieranie komendy}
     i := 0;
     c := #255;
     while (c <> #0) and (c <> ' ') do
     begin
          c := command [i];
          if c <> ' ' then
          ppchar[i] := c else ppchar[i] := #0;
          inc (i);
          if i > consolestrlen then syserror ('execconsolecommand()','i > consolestrlen');
     end;
     c := command [i];
     while (c = ' ') do
     begin
          inc (i);
          c := command [i];
     end;
     {pobieranie parametrow}
     k := 0;
     j := 0;
     while (c <> #0) do
     begin
          c := command [i];
          bc := command [i+1];
          if c <> ' ' then
          params[j][k] := c else
          begin
          if (bc <> ' ') then
          begin
               params[j][k] := #0;
               inc (j);
               if j > maxparams-1 then
               begin
                    con_addstr (trapconsole,'TOO MANY PARAMETERS');
                    _32fill (ppchar,consolestrlen,0);
                    break;
               end;
               k := 65535;
          end
          else
          dec (k);
          end;
          inc (i);
          inc (k);
          if i > consolestrlen then syserror ('execconsolecommand()','i > consolestrlen');
     end;
     {}
     if ppchar[0] <> #0 then
     begin
     if strcomp (ppchar,'QUIT') = 0 then halt {wyjscie}
     else
     if strcomp (ppchar,'LOADMAP') = 0 then
     begin
     if params[0][0] <> #0 then
     begin
          strcopy (ppchar2,'LOADING ');
          strcat (ppchar2,params[0]);
          strcat (ppchar2,'...');
          con_addstr (trapconsole,ppchar2);
          render;
          initmap (params[0]);
          if maploaded then
          con_addstr (trapconsole,'MAP LOADED');
          startstimer (titletimer);
     end
     else
     con_addstr (trapconsole,'USAGE: LOADMAP <MAPFILE>');
     end
     else
     if strcomp (ppchar,'FREEDOSMEM') = 0 then
     begin
          val2pchar (longint ($a000-dossegcnt) shl 4,ppchar2);
          con_addstr (trapconsole,ppchar2);
     end
     else
     if strcomp (ppchar,'FREEXMSMEM') = 0 then
     begin
          val2pchar (xmemmaxavail,ppchar2);
          con_addstr (trapconsole,ppchar2);
     end
     else
     if strcomp (ppchar,'SETGAMMA') = 0 then
     begin
          val (params[0],i,err);
          if err <> 0 then con_addstr (trapconsole,'BAD PARAMETER')
          else
          begin
               setgammapal (i);
          end;
     end
     else
     if strcomp (ppchar,'SETMODE') = 0 then
     begin
          val (params[0],l,err);
          if err <> 0 then con_addstr (trapconsole,'BAD PARAMETER')
          else
          begin
               initmode (l-1);
               if l <> 0 then
               begin
               if l-1 <= imodes then
               begin
                    strcopy (ppchar2,'MODE:');
                    strcat (ppchar2,modesnames [l-1]);
                    con_addstr (trapconsole,ppchar2);
               end;
               end
               else
               con_addstr (trapconsole,'MODE:320X200');
          end;
     end
     else
     if strcomp (ppchar,'W4VSYNC') = 0 then
     begin
          iswait4vsync := not iswait4vsync;
          strcopy (ppchar2,'WAIT4VSYNC IS ');
          strcat (ppchar2,onoff[byte (iswait4vsync)]);
          con_addstr (trapconsole,ppchar2);
     end
     else
     if strcomp (ppchar,'GOD') = 0 then
     begin
          godmode := not godmode;
          strcopy (ppchar2,'GODMODE IS ');
          strcat (ppchar2,onoff[byte (godmode)]);
          con_addstr (trapconsole,ppchar2);
     end
     else
     if strcomp (ppchar,'NOCLIP') = 0 then
     begin
          noclipmode := not noclipmode;
          strcopy (ppchar2,'NOCLIPMODE IS ');
          strcat (ppchar2,onoff[byte (noclipmode)]);
          con_addstr (trapconsole,ppchar2);
     end
     else
     if strcomp (ppchar,'GIVEALL') = 0 then
     begin
          for i := 0 to 3 do trapcurrmap.players[0].playerammo[i] := 999;
          for i := 0 to maxguns-1 do trapcurrmap.players[0].playerguns[i] := true;
          strcopy (ppchar2,'AMMO AND WEAPON UPDATED');
          con_addstr (trapconsole,ppchar2);
     end
     else
     if strcomp (ppchar,'SETG') = 0 then
     begin
          val (params[0],l,err);
          if err <> 0 then con_addstr (trapconsole,'BAD PARAMETER')
          else
          begin
               worldphysicobj.physicobjyaccel := longint (l) shl 8;
          end;
     end
     else
     con_addstr (trapconsole,'UNKNOWN COMMAND');
     end;
     for i := maxparams-1 downto 0 do freemem (params[i],consolestrlen);
     freemem (ppchar2,256);
     freemem (ppchar,consolestrlen);
end;

procedure drawcommand (i : word);
var
ti : word;
drw : boolean;
p : word;
ppchar,ppchar2 : pchar;
pc : byte;
begin
     drw := false;
     ppchar := trapcurrmenu^.menucoms^[i];
     p := 100-(trapcurrmenu^.numoftxts*14) div 2+i*14;
     if strcomp (ppchar,'SOUNDONOFF') = 0 then
     begin
          if configrec.ussnd = 1 then ti := 0 else ti := 1;
          drw := true;
     end;
     if strcomp (ppchar,'MUSICONOFF') = 0 then
     begin
          if configrec.usmus = 1 then ti := 0 else ti := 1;
          drw := true;
     end;
     if strcomp (ppchar,'MMXONOFF') = 0 then
     begin
          if sysinf.proccaps and $800000 = $800000 then
          begin
               if configrec.usmmx = 1 then ti := 0 else ti := 1;
          end
          else ti := 6;
          drw := true;
     end;
     if drw then
     scrsprtexnf (260,100-(trapcurrmenu^.numoftxts*14) div 2+i*14,menu832texs[ti]);
     if strcomp (ppchar,'GAMMATRACK') = 0 then
     begin
          scrtexnf (230,p,menu832texs[4]);
          scrsprtexnf (configrec.gamma*3+230+27,p+1,menu832texs[5]);
     end;
     if strcomp (ppchar,'LISTMODES') = 0 then
     begin
          scrsprtexnf (50,p,menu832texs[2]);
          if mmodeg = amodeg then textsetfade (22) else textsetfade (86);
          if mmodeg = 255 then
          begin
               textprint (65,p,0,0,'VGA 320X200');
               scrsprtexnf (225,p,menu832texs[3]);
          end
          else
          begin
               textprint (65,p,0,0,'VESA');
               textprint (120,p,0,0,modesnames[mmodeg]);
               scrsprtexnf (225,p,menu832texs[3]);
          end;
          textsettex (0,0,texttex1);
     end;
     if strcomp (ppchar,'LISTKEYS') = 0 then
     begin
          getmem (ppchar,30);
          vline (138,p-1,p+36,31);
          vline (208,p-1,p+36,31);
          for ti := 0 to 5 do
          begin
               textprint (50,p+ti*6,0,1,actionnames[ti]);
               convertscan2pchar (keyactionstab[0,ti],ppchar);
               textprint (140,p+ti*6,0,1,ppchar);
               convertscan2pchar (keyactionstab[1,ti],ppchar);
               textprint (210,p+ti*6,0,1,ppchar);
          end;
          if setingkeys <> 255 then
          pc := 54
          else
          pc := 28;
          if keypoz <> 6 then scrtranspwin (50,p+keypoz*6-1,278,p+keypoz*6+6,pc);
          freemem (ppchar,30);
     end;
     if strcomp (ppchar,'LISTSYSINFO') = 0 then textprint (50,p-20,0,1,sysinfotxt);
     if strcomp (ppchar,'LISTHELP') = 0 then textprint (50,50,0,1,traphelp);
end;

procedure addtrappart (particle : tparticle; n : byte; vxmod,vymod,cmod,tmod : word);
var
i : byte;
begin
     for i := 0 to maxparts-1 do
     begin
          if trapparts[i].used = false then
          begin
               trapparts[i].used := true;
               trapparts[i].n := n;
               trapparts[i].sx := particle.x;
               trapparts[i].sy := particle.y;
               initparticles (trapparts[i].particles,particle.x,particle.y,particle.xvel,particle.yvel,
               particle.color,particle.ecolor,particle.etime,n,vxmod,vymod,cmod,tmod);
               exit;
          end;
     end;
end;

procedure updatetrapparts;
var
i,j : byte;
begin
     usedtrapparts := 0;
     for i := 0 to maxparts-1 do
     begin
          if trapparts[i].used then
          begin
               inc (usedtrapparts);
               updateparticles (trapparts[i].particles,trapparts[i].n,trapstep);
               for j := 0 to trapparts[i].n do
               begin
                    trapparts[i].used := false;
                    if trapparts[i].particles[j].onlife = true then
                    begin
                         trapparts[i].used := true;
                         break;
                    end;
               end;
          end;
     end;
end;

procedure drawtrapparts (x1,y1,x2,y2 : word);
var
i : byte;
begin
     for i := 0 to maxparts-1 do
     begin
          if trapparts[i].used then
          begin
               if (ispixinrect (x1-10,y1-10,x2+10,y2+10,trapparts[i].sx shr 8,trapparts[i].sy shr 8)) then
               begin
                    drawparticles (x1,y1,trapparts[i].particles,trapparts[i].n);
               end;
          end
     end;
end;

procedure playershot;
label
q;
var
apo : tphysicobj;
hdirection : shortint;
icf : boolean;
j,pv : byte;
i : word;
pa : integer;
px,py : word;
pv2 : floatint;
begin
     if playeriskilled then exit;
     if trapcurrmap.players[0].igun <> 0 then
     begin
          if trapcurrmap.players[0].playerammo [gun2ammotab[trapcurrmap.players[0].igun]]
          < (-ammo2ntab[trapcurrmap.players[0].igun]) then exit;
          pa := trapcurrmap.players[0].playerammo [gun2ammotab[trapcurrmap.players[0].igun]];
          int_idval (pa,ammo2ntab[trapcurrmap.players[0].igun],0,999);
          trapcurrmap.players[0].playerammo [gun2ammotab[trapcurrmap.players[0].igun]] := pa;
     end;
     startstimer (plguntimer);
     plguna := 2;
     playershoting := true;
     hdirection := directiontab [trapcurrmap.players[0].state];
     if hdirection = -1 then
     begin
          setcuranim (handanimations,3);
          setcuranim (trapgunssprites[trapcurrmap.players[0].igun],1);
          {}
          px := trapcurrmap.players[0].playerpozx+playerlinks[trapcurrmap.players[0].state*2].mx+
          playerhshf-getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizex+6;
          {}
          py := trapcurrmap.players[0].playerpozy+playerlinks[trapcurrmap.players[0].state*2].my+
          getanimation (handanimations)^.sizey-getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizey;
     end;
     if hdirection = 1 then
     begin
          setcuranim (handanimations,0);
          setcuranim (trapgunssprites[trapcurrmap.players[0].igun],0);
          {}
          px := trapcurrmap.players[0].playerpozx+playerlinks[trapcurrmap.players[0].state*2].mx+
          getanimation (handanimations)^.sizex+
          playerhshf+getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizex-4;
          {}
          py := trapcurrmap.players[0].playerpozy+playerlinks[trapcurrmap.players[0].state*2].my+
          getanimation (handanimations)^.sizey-getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizey;
     end;
     if trapguns [trapcurrmap.players[0].igun].gtype = 0 then
     begin
          inc (py);
          if iscellfull (px shr 5,py div 24) = false then
          begin
               inc (py,getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizey-3);
               if iscellfull (px shr 5,py div 24) = false then goto q;
               gunsparts1.y := longint (py) shl 8;
          end
          else
          gunsparts1.y := longint (py+getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizey shr 1) shl 8;
          if hdirection = 1 then
          gunsparts1.x := (longint (px) shr 5) shl 13
          else
          gunsparts1.x := (longint (px) shr 5+1) shl 13;
          addtrappart (gunsparts1,gparts1n,gparts1mx,gparts1my,gparts1mc,gparts1mt);
     end;
     if trapguns [trapcurrmap.players[0].igun].gtype = 1 then
     begin
          pv := trapguns [trapcurrmap.players[0].igun].gnammo-1;
          if pv > 128 then syserror ('playershot()','trapguns [trapcurrmap.players[0].igun].gnammo > 128');
          apo.physicobjmaxxvel := $ffffff;
          apo.physicobjmaxyvel := $ffffff;
          apo.physicobjxaccel := 0;
          apo.physicobjyaccel := 0;
          for j := 0 to pv do
          begin
          apo.physicobjpozx := longint (px) shl 8;
          apo.physicobjpozy := longint (py) shl 8+128;
          pv2 := random (pv+2)*(256 div 6);
          pv2 := pv2-((word (pv+1)*(256 div 6)) shr 1);
          apo.physicobjxvelocity := hdirection*(32 shl 8);
          apo.physicobjyvelocity := pv2;
          icf := false;
          i := 0;
          while icf = false do
          begin
               icf := iscellfull (apo.physicobjpozx shr 13,apo.physicobjpozy div 6144);
               update_physicobj (apo,65536);
               inc (i);
               if i > 1000 then break;
          end;
          if hdirection = 1 then
          gunsparts1.x := (apo.physicobjpozx shr 13-1) shl 13
          else
          gunsparts1.x := (apo.physicobjpozx shr 13+2) shl 13;
          gunsparts1.y := apo.physicobjpozy;
          addtrappart (gunsparts1,gparts1n,gparts1mx,gparts1my,gparts1mc,gparts1mt);
          end;
     end;
     if (trapguns [trapcurrmap.players[0].igun].gtype = 2) or (trapguns [trapcurrmap.players[0].igun].gtype = 3)
     or (trapguns [trapcurrmap.players[0].igun].gtype = 4) then
     begin
          for i := 0 to maxshotammo-1 do
          begin
               if trapsammo[i].used = false then
               begin
                    trapsammo[i].used := true;
                    trapsammo[i].atype := trapguns [trapcurrmap.players[0].igun].gatype;
                    trapsammo[i].gtype := trapguns [trapcurrmap.players[0].igun].gtype;
                    trapsammo[i].igun := trapcurrmap.players[0].igun;
                    trapsammo[i].tsammoobj.physicobjmaxxvel := $ffffff;
                    trapsammo[i].tsammoobj.physicobjmaxyvel := $ffffff;
                    trapsammo[i].tsammoobj.physicobjxaccel := 0;
                    trapsammo[i].tsammoobj.physicobjyaccel := 0;
                    if trapguns [trapcurrmap.players[0].igun].gtype = 3 then
                    begin
                         trapsammo[i].tsammoobj.physicobjyaccel := worldphysicobj.physicobjyaccel;
                    end;
                    trapsammo[i].tsammoobj.physicobjpozx := longint (px) shl 8;
                    {}
                    if trapguns [trapcurrmap.players[0].igun].gtype = 2 then
                    trapsammo[i].tsammoobj.physicobjpozy := longint (py) shl 8+256
                    else
                    if trapguns [trapcurrmap.players[0].igun].gtype = 3 then
                    trapsammo[i].tsammoobj.physicobjpozy := longint (py) shl 8+768
                    else
                    trapsammo[i].tsammoobj.physicobjpozy := longint (py) shl 8+256;
                    {}
                    trapsammo[i].tsammoobj.physicobjxvelocity := hdirection*
                    (longint (trapguns [trapcurrmap.players[0].igun].gextra) shl 8);
                    trapsammo[i].tsammoobj.physicobjyvelocity := 0;
                    break;
               end;
          end;
     end;
     q:
end;

procedure playerupdateshot;
const
rockedbum = 60*256;
var
pt : longint;
st : longint;
i,j : word;
icf : boolean;
py : longint;
fcnt,fskip : integer;
odl,odly,sc : floatint;
begin
     if playershoting then
     begin
     stopstimer (plguntimer,timer_in_milli_seconds,pt);
     st := trapguns[trapcurrmap.players[0].igun].gtime;
     if pt > st then playershoting := false;
     if pt > st shr 1 then plguna := 0;
     end;
     for i := 0 to maxshotammo-1 do
     begin
          if trapsammo[i].used then
          begin
               icf := iscellfull (trapsammo[i].tsammoobj.physicobjpozx shr 13,trapsammo[i].tsammoobj.physicobjpozy div 6144);
               if icf then
               begin
                    trapsammo[i].used := false;
                    {}
                    if trapsammo[i].tsammoobj.physicobjxvelocity > 0 then
                    begin
                         gunsparts2.xvel := -6400;
                         trapsammo[i].tsammoobj.physicobjpozx := (trapsammo[i].tsammoobj.physicobjpozx shr 13) shl 13;
                    end
                    else
                    begin
                         gunsparts2.xvel := 6400;
                         trapsammo[i].tsammoobj.physicobjpozx := (trapsammo[i].tsammoobj.physicobjpozx shr 13+1) shl 13;
                    end;
                    gunsparts2.yvel := 0;
                    gunsparts2.x := trapsammo[i].tsammoobj.physicobjpozx;
                    gunsparts2.y := trapsammo[i].tsammoobj.physicobjpozy;
                    if trapsammo[i].atype = 3 then
                    begin
                         gunsparts2.color := 95;
                         gunsparts2.ecolor := 80;
                         addtrappart (gunsparts2,gparts2n,gparts2mx,gparts2my,gparts2mc,gparts2mt);
                    end
                    else
                    if trapsammo[i].atype = 4 then
                    begin
                         odl := trapsammo[i].tsammoobj.physicobjpozx-playerphysicobj.physicobjpozx;
                         if abs (odl) < rockedbum then
                         begin
                              odly := trapsammo[i].tsammoobj.physicobjpozy-playerphysicobj.physicobjpozy;
                              if abs (odly) < rockedbum then
                              begin
                                   sc := (longint (trapguns[trapsammo[i].igun].gapower) shl 16) div rockedbum;
                                   if odl = 0 then odl := 1;
                                   odl := abs (odl);
                                   sc := (odl*sc) shr 8;
                                   trapcurrmap.players[0].health :=
                                   trapcurrmap.players[0].health-(trapguns[trapsammo[i].igun].gapower-sc shr 8);
                              end;
                         end;
                         for j := 0 to 5 do
                         begin
                              if j mod 2 = 0 then
                              begin
                                   gunsparts2.color := 31;
                                   gunsparts2.ecolor := 15;
                              end
                              else
                              begin
                                   gunsparts2.color := 159;
                                   gunsparts2.ecolor := 145;
                              end;
                              addtrappart (gunsparts2,gparts2n,gparts2mx,gparts2my,gparts2mc,gparts2mt);
                         end;
                    end;
                    continue;
               end
               else
               begin
                    py := trapitemssprites[trapsammo[i].gtype*2+sammoitems].sizey shl 8;
                    icf := iscellfull (trapsammo[i].tsammoobj.physicobjpozx shr 13,(
                    trapsammo[i].tsammoobj.physicobjpozy+py) div 6144);
                    if icf then
                    begin
                         trapsammo[i].used := false;
                         gunsparts2.x := trapsammo[i].tsammoobj.physicobjpozx;
                         gunsparts2.y := trapsammo[i].tsammoobj.physicobjpozy;
                         if trapsammo[i].atype = 3 then
                         begin
                              if trapsammo[i].tsammoobj.physicobjxvelocity > 0 then
                              gunsparts2.xvel := -6400
                              else
                              gunsparts2.xvel := 6400;
                              gunsparts2.color := 95;
                              gunsparts2.ecolor := 80;
                              addtrappart (gunsparts2,gparts2n,gparts2mx,gparts2my,gparts2mc,gparts2mt);
                         end
                         else
                         if trapsammo[i].atype = 4 then
                         begin
                              odl := trapsammo[i].tsammoobj.physicobjpozx-playerphysicobj.physicobjpozx;
                              if abs (odl) < rockedbum then
                              begin
                                   odly := trapsammo[i].tsammoobj.physicobjpozy-playerphysicobj.physicobjpozy;
                                   if abs (odly) < rockedbum then
                                   begin
                                        sc := (longint (trapguns[trapsammo[i].igun].gapower) shl 16) div rockedbum;
                                        if odl = 0 then odl := 1;
                                        odl := abs (odl);
                                        sc := (odl*sc) shr 8;
                                        trapcurrmap.players[0].health :=
                                        trapcurrmap.players[0].health-(trapguns[trapsammo[i].igun].gapower-sc shr 8);
                                   end;
                              end;
                              gunsparts2.xvel := 0;
                              gunsparts2.yvel := -6400;
                              for j := 0 to 5 do
                              begin
                                   if j mod 2 = 0 then
                                   begin
                                        gunsparts2.color := 31;
                                        gunsparts2.ecolor := 15;
                                   end
                                   else
                                   begin
                                        gunsparts2.color := 159;
                                        gunsparts2.ecolor := 145;
                                   end;
                                   addtrappart (gunsparts2,gparts2n,gparts2mx,gparts2my-6400,gparts2mc,gparts2mt);
                              end;
                         end;
                         continue;
                    end
               end;
               scrsprtextf (trapsammo[i].tsammoobj.physicobjpozx shr 8-trapmapviewshfx,
               trapsammo[i].tsammoobj.physicobjpozy shr 8-trapmapviewshfy,31,
               trapitemssprites[(trapsammo[i].atype-1)*2+sammoitems]);
               {}
               if trapsammo[i].gtype = 4 then if isupdateshot then
               begin
                    getframescnt (20*256,fcnt,fskip);
                    if trapframes mod fcnt = fcnt-1 then
                    begin
                    if directiontab [trapcurrmap.players[0].state] = 1 then
                    gunsparts3.x := trapsammo[i].tsammoobj.physicobjpozx+256
                    else
                    gunsparts3.x := trapsammo[i].tsammoobj.physicobjpozx+768;
                    gunsparts3.y := trapsammo[i].tsammoobj.physicobjpozy+256;
                    addtrappart (gunsparts3,gparts3n,gparts3mx,gparts3my,gparts3mc,gparts3mt);
                    end;
               end;
               {}
               if isupdateshot then
               begin
                    if trapsammo[i].tsammoobj.physicobjyaccel > 0 then update_velocity (trapsammo[i].tsammoobj,0,
                    trapsammo[i].tsammoobj.physicobjyaccel,trapstep);
                    update_physicobj (trapsammo[i].tsammoobj,trapstep);
               end;
          end;
     end;
     isupdateshot := false;
end;

procedure drawplayer (x,y : integer; a : byte; f : byte);
var
hdirection : shortint;
px,py : integer;
begin
     setcuranim (playeranimations,a);
     hdirection := directiontab [a];
     {}
     if hdirection = 1 then setcuranim (handanimations,1)
     else
     if hdirection = -1 then setcuranim (handanimations,2);
     scrsprtexf (x+playerlinks[a*2+1].mx+playerhshf,y+playerlinks[a*2+1].my,f,getanimation (handanimations)^);
     {}
     if hdirection = -1 then
     begin
          setcuranim (handanimations,3);
          setcuranim (trapgunssprites[trapcurrmap.players[0].igun],1+plguna);
          px := -getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizex+7;
          py := getanimation (handanimations)^.sizey-getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizey;
          scrsprtexf (x+playerlinks[a*2].mx+playerhshf+px,y+playerlinks[a*2].my+py,f,
          getanimation (trapgunssprites[trapcurrmap.players[0].igun])^);
     end;
     {}
     scrsprtexf (x+playerashf,y,f,getanimation (playeranimations)^);
     if abs (playerashf) > 10 then syserror ('drawplayer()','abs (playerashf) > 10');
     {}
     if hdirection = 1 then
     begin
          setcuranim (handanimations,0);
          setcuranim (trapgunssprites[trapcurrmap.players[0].igun],0+plguna);
          px := getanimation (handanimations)^.sizex-4;
          py := getanimation (handanimations)^.sizey-getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizey;
          scrsprtexf (x+playerlinks[a*2].mx+playerhshf+px,y+playerlinks[a*2].my+py,f,
          getanimation (trapgunssprites[trapcurrmap.players[0].igun])^);
     end;
     {}
     if hdirection = 1 then setcuranim (handanimations,0)
     else
     if hdirection = -1 then setcuranim (handanimations,3);
     scrsprtexf (x+playerlinks[a*2].mx+playerhshf,y+playerlinks[a*2].my,f,getanimation (handanimations)^);
end;

procedure playercrouch (a : byte);
var
pv1,pv2,pv4,pv6 : word;
pv3,pv5 : floatint;
o : byte;
begin
     if a = 1 then
     begin
          pcrouch := true;
          if pdir = 0 then trapcurrmap.players[0].state := 6
          else
          trapcurrmap.players[0].state := 7;
          if playerjumps = false then
          begin
               setcuranim (playeranimations,0);
               pv1 := getanimation (playeranimations)^.sizey;
               setcuranim (playeranimations,6);
               pv2 := getanimation (playeranimations)^.sizey;
               floatint_add (playerphysicobj.physicobjpozy,playerphysicobj.physicobjpozy,(pv1-pv2) shl 8);
          end;
          pv3 := pv2;
          setcuranim (playeranimations,0);
          pv1 := getanimation (playeranimations)^.sizex shr 1;
          setcuranim (playeranimations,6);
          pv4 := getanimation (playeranimations)^.sizex;
          pv2 := pv4 shr 1;
          floatint_sub (playerphysicobj.physicobjpozx,playerphysicobj.physicobjpozx,(pv2-pv1) shl 8);
          {}
          o := iscellcolision (playerphysicobj.physicobjpozx shr 8,playerphysicobj.physicobjpozy shr 8,
          playerphysicobj.physicobjpozx shr 8+pv4,playerphysicobj.physicobjpozy shr 8+pv3);
          if (o and (LU)=(LU)) then
          begin
               {playerphysicobj.physicobjpozy := longint ((playerphysicobj.physicobjpozy shr 16+pv3 div 24+1) * 24-pv3) shl 8;
               playerphysicobj.physicobjyvelocity := 0;
               playerjumps := false;}
               playerphysicobj.physicobjpozx := longint ((playerphysicobj.physicobjpozx shr 13+1) shl 5) shl 8;
               playerphysicobj.physicobjxvelocity := 0;
          end
          else
          if (o and (RU)=(RU)) then
          begin
               {playerphysicobj.physicobjpozy := longint ((playerphysicobj.physicobjpozy shr 16+pv3 shr 8+1) * 24-pv3) shl 8;
               playerphysicobj.physicobjyvelocity := 0;
               playerjumps := false;}
               playerphysicobj.physicobjpozx := longint ((playerphysicobj.physicobjpozx shr 13+pv4 shr 5+1) shl 5-pv4) shl 8;
               playerphysicobj.physicobjxvelocity := 0;
          end;
     end
     else
     begin
          setcuranim (playeranimations,0);
          pv1 := getanimation (playeranimations)^.sizey;
          pv6 := pv1;
          pv4 := getanimation (playeranimations)^.sizex;
          setcuranim (playeranimations,6);
          pv2 := getanimation (playeranimations)^.sizey;
          pv3 := playerphysicobj.physicobjpozy;
          floatint_sub (pv3,pv3,(pv1-pv2) shl 8);
          {}
          setcuranim (playeranimations,0);
          pv1 := getanimation (playeranimations)^.sizex div 2;
          setcuranim (playeranimations,6);
          pv2 := getanimation (playeranimations)^.sizex div 2;
          pv5 := playerphysicobj.physicobjpozx;
          floatint_add (pv5,pv5,(pv2-pv1) shl 8);
          {}
          o := iscellcolision (pv5 shr 8,pv3 shr 8,pv5 shr 8+pv4,pv3 shr 8+pv6);
          if (o and (N)=(N)) or (o and (RU)=(RU)) or (o and (LU)=(LU)) then
          exit
          else
          if playerjumps = false then
          begin
               playerphysicobj.physicobjpozy := pv3
          end;
          pcrouch := false;
          if pdir = 0 then trapcurrmap.players[0].state := 0
          else
          trapcurrmap.players[0].state := 1;
          playerphysicobj.physicobjpozx := pv5;
          o := iscellcolision (pv5 shr 8,playerphysicobj.physicobjpozy shr 8,
          pv5 shr 8+pv4,playerphysicobj.physicobjpozy shr 8+pv6);
          if (o and (S)=(S)) or (o and (RD)=(RD)) or (o and (LD)=(LD)) or (o and (E)=(E)) or (o and (W)=(W)) then
          begin
               playerphysicobj.physicobjpozy := longint ((((playerphysicobj.physicobjpozy-256) div 6144)
               +pv6 div 24+1) * 24-pv6) shl 8;
               playerphysicobj.physicobjyvelocity := 0;
               playerjumps := false;
          end;
     end;
end;

procedure killplayer;
var
sx,sy,sx2,sy2 : word;
pv1,pv2 : word;
o : byte;
begin
     if trapcurrmap.players[0].health < HDELOBJV then
     begin
          playeriskilled := true;
          playeronmap := false;
          exit;
     end;
     if playeriskilled = false then
     begin
          playeriskilled := true;
          if pcrouch then
          plkilledt := clock-stdplkilledt
          else
          plkilledt := clock;
          playerashf := 0;
          playerhshf := 0;
          playerisongrab := false;
          {}
          getplayersxsy (sx,sy);
          if pcrouch = false then
          setcuranim (playeranimations,12)
          else
          setcuranim (playeranimations,13);
          pcrouch := false;
          sx := sx shr 1;
          sx2 := getanimation (playeranimations)^.sizex shr 1;
          sy2 := getanimation (playeranimations)^.sizey;
          floatint_add (playerphysicobj.physicobjpozy,playerphysicobj.physicobjpozy,integer (sy-sy2) shl 8);
          floatint_sub (playerphysicobj.physicobjpozx,playerphysicobj.physicobjpozx,integer (sx2-sx) shl 8);
          (*pv1 := getanimation (playeranimations)^.sizey;
          floatint_add (playerphysicobj.physicobjpozy,playerphysicobj.physicobjpozy,(sy-pv1) shl 8);
          pv2 := getanimation (playeranimations)^.sizex;
          floatint_sub (playerphysicobj.physicobjpozx,playerphysicobj.physicobjpozx,(sy-pv2) shl 8);
          {}
          o := iscellcolision (playerphysicobj.physicobjpozx shr 8,playerphysicobj.physicobjpozy shr 8,
          playerphysicobj.physicobjpozx shr 8+pv2,playerphysicobj.physicobjpozy shr 8+pv1);
          if (o and (LU)=(LU)) then
          begin
               playerphysicobj.physicobjpozx := longint ((playerphysicobj.physicobjpozx shr 13+1) shl 5) shl 8;
               playerphysicobj.physicobjxvelocity := 0;
          end
          else
          if (o and (RU)=(RU)) then
          begin
               playerphysicobj.physicobjpozx := longint ((playerphysicobj.physicobjpozx shr 13+pv2 shr 5+1) shl 5-pv2) shl 8;
               playerphysicobj.physicobjxvelocity := 0;
          end;*)
          xmssnd_addsample (playersounds[4],128);
     end;
     if playeriskilled then
     begin
          if clock-plkilledt < stdplkilledt then trapcurrmap.players[0].state := 12
          else
          trapcurrmap.players[0].state := 13;
     end;
end;

{0-prawo
 1-lewo
 2-gora
 3-dol}
procedure moveplayer (dir : byte);
const
playeranimtab1 : array [0..3] of byte = (2,0,3,0);
playeranimtab2 : array [0..3] of byte = (4,1,5,1);
playeranimtab3 : array [0..3] of byte = (8,6,9,6);
playeranimtab4 : array [0..3] of byte = (10,7,11,7);
ptime = 550;
var
paccelx : floatint;
p : pchar;
pv : byte;
pptime : floatint;
begin
     {with trapcurrmap.players[0] do
     begin
     case dir of
     0:word_idval (playerpozx,1,0,mtrapmapsizex);
     1:word_idval (playerpozx,-1,0,mtrapmapsizex);
     2:word_idval (playerpozy,-1,0,mtrapmapsizey);
     3:word_idval (playerpozy,1,0,mtrapmapsizey);
     end;}
     if playeriskilled then
     begin
          playermov := 0;
          exit;
     end;
     paccelx := 0;
     with playerphysicobj do
     begin
     case dir of
     0:paccelx := physicobjxaccel;
     1:paccelx := -physicobjxaccel;
     end;
     end;
     update_velocity (playerphysicobj,paccelx,0,trapstep);
     if playerisongrab then
     begin
          if dir = 2 then
          begin
               update_velocity (playerphysicobj,0,-playerphysicobj.physicobjxaccel,trapstep);
               playermov := 2;
               pludmovd := true;
          end;
          if dir = 3 then
          begin
               update_velocity (playerphysicobj,0,playerphysicobj.physicobjxaccel,trapstep);
               playermov := 3;
               pludmovd := true;
          end;
     end
     else
     begin
     if dir = 2 then
     begin
     if pwaterw <> $fffffff then
     begin
          update_velocity (playerphysicobj,0,-(playerphysicobj.physicobjxaccel shr 2),trapstep);
          playermov := 2;
          pludmovd := true;
     end
     else
     if (not playerjumps) and (not pcrouch) then
     begin
          playerphysicobj.physicobjyvelocity := pjmpp;
          playerjumps := true;
          playermov := 2;
          pludmovd := true;
          xmssnd_addsample (playersounds[0],128);
     end;
     end;
     if dir = 3 then
     begin
     if shiftkp then
     begin
     if pwaterw <> $fffffff then
     begin
          update_velocity (playerphysicobj,0,(playerphysicobj.physicobjxaccel shr 2),trapstep);
          playermov := 3;
          pludmovd := true;
     end
     else
     begin
          if pcrouch = false then playercrouch (1);
          playermov := 3;
          pludmovd := true;
     end;
     end
     else
     begin
          if pcrouch = false then playercrouch (1);
          playermov := 3;
          pludmovd := true;
     end;
     end;
     end;
     {}
     if shiftkp then
     floatint_mul (pptime,ptime,160)
     else
     pptime := ptime;
     if dir = 0 then
     begin
          if pcrouch = false then
          begin
               animval (trapcurrmap.players[0].state,@playeranimtab1,4,pptime,playersteps);
               {}
               pv := 0;
               animval (pv,@rsplayershifts,4,pptime,playersteps);
               playerashf := shortint (pv);
               {}
               pv := 0;
               animval (pv,@hrsplayershifts,4,pptime,playersteps);
               playerhshf := shortint (pv);
          end
          else
          begin
               animval (trapcurrmap.players[0].state,@playeranimtab3,4,pptime,playersteps);
               playerashf := 0;
               playerhshf := 0;
          end;
          plrlmovd := true;
          pdir := 0;
          playermov := 1;
     end
     else
     if dir = 1 then
     begin
          if pcrouch = false then
          begin
               animval (trapcurrmap.players[0].state,@playeranimtab2,4,pptime,playersteps);
               {}
               pv := 0;
               animval (pv,@lsplayershifts,4,pptime,playersteps);
               playerashf := shortint (pv);
               {}
               pv := 0;
               animval (pv,@hlsplayershifts,4,pptime,playersteps);
               playerhshf := shortint (pv);
          end
          else
          begin
               animval (trapcurrmap.players[0].state,@playeranimtab4,4,pptime,playersteps);
               playerashf := 0;
               playerhshf := 0;
          end;
          plrlmovd := true;
          pdir := 1;
          playermov := 1;
     end;
     {}
     {}
     {if playerpozx >= 160 then
     trapmapviewshfx := playerpozx-160;
     if playerpozy >= 100 then
     trapmapviewshfy := playerpozy-100;
     {
     if trapmapviewshfx > mtrapmapsizex-320 then trapmapviewshfx := mtrapmapsizex-320;
     if trapmapviewshfy > mtrapmapsizey-200 then trapmapviewshfy := mtrapmapsizey-200;
     end;}
end;

procedure isitem (x1,y1,sx,sy : word);
var
cell : byte;
i,j,px1,py1,px2,py2,pphe : word;
pv,v : byte;
label
fex,nta,ntg;
begin
     px1 := x1 shr 5;
     py1 := y1 div 24;
     px2 := (x1+sx-1) shr 5;
     py2 := (y1+sy-1) div 24;
     for j := py1 to py2 do
     for i := px1 to px2 do
     begin
          cell := trapcurrmap.trapmap^[j*trapcurrmap.trapmapsizex+i].fronttex1;
          pv := 1;
          if (cell <= 192) or (cell = 224) or (cell = 255) then
          begin
               pv := 2;
               cell := trapcurrmap.trapmap^[j*trapcurrmap.trapmapsizex+i].fronttex2;
               if (cell > 192) and (cell <> 224) and (cell <> 255) then goto fex;
          end
          else
          goto fex;
     end;
     fex:
     if (cell <= 192) or (cell = 224) or (cell = 255) then exit;
     if cell < 224 then
     begin
          px1 := ((i+1) shl 5)-16-trapitemssprites[cell-193].sizex shr 1;
          py1 := ((j+1) * 24)-trapitemssprites[cell-193].sizey;
          px2 := px1+trapitemssprites[cell-193].sizex;
          py2 := py1+trapitemssprites[cell-193].sizey;
     end
     else
     begin
          px1 := ((i+1) shl 5)-16-getanimation (trapgunssprites[cell-225])^.sizex shr 1;
          py1 := ((j+1) * 24)-getanimation (trapgunssprites[cell-225])^.sizey;
          px2 := px1+getanimation (trapgunssprites[cell-225])^.sizex;
          py2 := py1+getanimation (trapgunssprites[cell-225])^.sizey;
     end;
     v := iscolision (x1,y1,x1+sx,y1+sy,px1,py1,px2,py2);
     if v = 0 then v := iscolision (px1,py1,px2,py2,x1,y1,x1+sx,y1+sy);
     if v <> 0 then
     begin
          if cell < 224 then
          begin
               cell := cell-193;
               if items[cell].id = HEALTH_ITEMS then
               begin
                    pphe := trapcurrmap.players[0].health;
                    if trapcurrmap.players[0].health < items[cell].idval2 then
                    int_idval (trapcurrmap.players[0].health,items[cell].idval1,0,items[cell].idval2);
                    if pphe = trapcurrmap.players[0].health then pv := 0;
               end
               else
               if items[cell].id = KEYS_ITEMS then
               begin
                    playerkeys[items[cell].idval1] := true;
               end
               else
               if items[cell].id >= AMMO_ITEMS then
               begin
                    if trapcurrmap.players[0].playerammo[items[cell].id-AMMO_ITEMS] < items[cell].idval2 then
                    word_idval (trapcurrmap.players[0].playerammo[items[cell].id-AMMO_ITEMS],
                    items[cell].idval1,0,items[cell].idval2)
                    else goto nta;
               end;
               if pv = 1 then trapcurrmap.trapmap^[j*trapcurrmap.trapmapsizex+i].fronttex1 := 0
               else if pv = 2 then
               trapcurrmap.trapmap^[j*trapcurrmap.trapmapsizex+i].fronttex2 := 0;
               nta:
          end
          else
          begin
               trapcurrmap.players[0].playerguns [cell-225] := true;
               trapcurrmap.players[0].igun := cell-225;
               v := cell-225;
               if v <> 0 then
               begin
                    v := gun2ammotab[v];
                    if trapcurrmap.players[0].playerammo[v] < items[sammoitems+(v*2)].idval2 then
                    word_idval (trapcurrmap.players[0].playerammo[v],
                    items[sammoitems+(v*2)].idval1,0,items[sammoitems+(v*2)].idval2)
                    else goto ntg;
               end;
               if pv = 1 then trapcurrmap.trapmap^[j*trapcurrmap.trapmapsizex+i].fronttex1 := 0
               else if pv = 2 then
               trapcurrmap.trapmap^[j*trapcurrmap.trapmapsizex+i].fronttex2 := 0;
               ntg:
          end;
     end;
end;

function isdoor (x1,y1,sx,sy : word; doortype : byte) : shortint;
var
inft : byte;
px,py,ps : word;
i : word;
begin
     ps := (sy-1) div 24;
     {}
     isdoor := 1;
     inft := doortype+250;
     px := (x1 shr 5)+1;
     py := (y1 div 24);
     for i := 0 to ps do
     begin
          if trapcurrmap.trapmap^[(py+i)*trapcurrmap.trapmapsizex+px].trapinforec1 = inft then exit
          else
          if trapcurrmap.trapmap^[(py+i)*trapcurrmap.trapmapsizex+px].trapinforec2 = inft then exit;
     end;
     {}
     isdoor := -1;
     px := ((x1+sx) shr 5)-1;
     for i := 0 to ps do
     begin
          if trapcurrmap.trapmap^[(py+i)*trapcurrmap.trapmapsizex+px].trapinforec1 = inft then exit
          else
          if trapcurrmap.trapmap^[(py+i)*trapcurrmap.trapmapsizex+px].trapinforec2 = inft then exit;
     end;
     isdoor := 0;
end;

procedure opendoor (x1,y1,sy : word; doortype : byte; dir : shortint);
var
inft : byte;
px : word;
i : word;
ov : byte;
skip : word;
begin
     skip := (sy-1) div 24;
     inft := doortype+250;
     px := x1+dir;
     i := y1-1;
     while (1=1) do
     begin
          ov := trapcurrmap.trapmap^[i*trapcurrmap.trapmapsizex+px].trapinforec1;
          if ov = inft then
          begin
               trapcurrmap.trapmap^[i*trapcurrmap.trapmapsizex+px].trapinforec1 := 0;
               trapcurrmap.trapmap^[i*trapcurrmap.trapmapsizex+px].fronttex1 := 0;
          end
          else
          begin
               ov := trapcurrmap.trapmap^[i*trapcurrmap.trapmapsizex+px].trapinforec2;
               if ov = inft then
               begin
                    trapcurrmap.trapmap^[i*trapcurrmap.trapmapsizex+px].trapinforec2 := 0;
                    trapcurrmap.trapmap^[i*trapcurrmap.trapmapsizex+px].fronttex2 := 0;
               end
          end;
          dec (i);
          if inft <> ov then break;
     end;
     {}
     i := y1;
     while (1=1) do
     begin
          ov := trapcurrmap.trapmap^[i*trapcurrmap.trapmapsizex+px].trapinforec1;
          if ov = inft then
          begin
               trapcurrmap.trapmap^[i*trapcurrmap.trapmapsizex+px].trapinforec1 := 0;
               trapcurrmap.trapmap^[i*trapcurrmap.trapmapsizex+px].fronttex1 := 0;
          end
          else
          begin
               ov := trapcurrmap.trapmap^[i*trapcurrmap.trapmapsizex+px].trapinforec2;
               if ov = inft then
               begin
                    trapcurrmap.trapmap^[i*trapcurrmap.trapmapsizex+px].trapinforec2 := 0;
                    trapcurrmap.trapmap^[i*trapcurrmap.trapmapsizex+px].fronttex2 := 0;
               end
          end;
          inc (i);
          if (i-y1-1 < skip) then continue;
          if inft <> ov then break;
     end;
end;

procedure playeruse;
var
pv : longint;
sx,sy : word;
ov : shortint;
label
q;
begin
     if playeriskilled then exit;
     stopstimer (pusf,timer_in_milli_seconds,pv);
     if pv < pusft then exit;
     getplayersxsy (sx,sy);
     {}
     if shiftkp then
     begin
     ov := isdoor (trapcurrmap.players[0].playerpozx,trapcurrmap.players[0].playerpozy,sx,sy,SDOOR);
     if ov <> 0 then
     begin
          opendoor (trapcurrmap.players[0].playerpozx shr 5,trapcurrmap.players[0].playerpozy div 24,sy,SDOOR,ov);
          goto q;
     end;
     if playerkeys[REDKEY] then
     begin
          ov := isdoor (trapcurrmap.players[0].playerpozx,trapcurrmap.players[0].playerpozy,sx,sy,RDOOR);
          if ov <> 0 then
          begin
               opendoor (trapcurrmap.players[0].playerpozx shr 5,trapcurrmap.players[0].playerpozy div 24,sy,RDOOR,ov);
               goto q;
          end;
     end;
     if playerkeys[BLUEKEY] then
     begin
          ov := isdoor (trapcurrmap.players[0].playerpozx,trapcurrmap.players[0].playerpozy,sx,sy,BDOOR);
          if ov <> 0 then
          begin
               opendoor (trapcurrmap.players[0].playerpozx shr 5,trapcurrmap.players[0].playerpozy div 24,sy,BDOOR,ov);
               goto q;
          end;
     end;
     if playerkeys[YELLOWKEY] then
     begin
          ov := isdoor (trapcurrmap.players[0].playerpozx,trapcurrmap.players[0].playerpozy,sx,sy,YDOOR);
          if ov <> 0 then
          begin
               opendoor (trapcurrmap.players[0].playerpozx shr 5,trapcurrmap.players[0].playerpozy div 24,sy,YDOOR,ov);
          end;
     end;
     end
     else
     begin
          if isplayerongrab then playerisongrab := not playerisongrab;
     end;
     q:
     {}
     startstimer (pusf);
     playerisuse := true;
end;

procedure handleplayerbyvel;
const
playerfallvel = 150 shl 8;
playerbyvels = 184 shl 8;
playerbyvele = 330 shl 8;
var
p,modv : floatint;
begin
     floatint_sub (p,-playerdeltayvel,playerbyvels);
     if p <= 0 then
     begin
          if -playerdeltayvel > playerfallvel then xmssnd_addsample (playersounds[1],128);
          exit;
     end;
     floatint_div (p,modv,p,playerbyvele-playerbyvels);
     floatint_mul (p,p,25600);
     floatint_sub (p,longint (trapcurrmap.players[0].health) shl 8,p);
     floatint_rnd (trapcurrmap.players[0].health,p);
end;

function cell2tex (var otex : ttex; cell : byte) : byte;
begin
     cell2tex := 0;
     if (cell <> 0) and (cell <> 64) and (cell <> 128) and (cell <> 192)
     and (cell <> 224) and (cell <> 255) then
     begin
          cell2tex := 2;
          if cell < 64 then
          begin
               otex := traptextures [cell-1];
               cell2tex := 1;
          end
          else
          if cell < 128 then otex := trapsprites [cell-65]
          else
          if cell < 192 then otex := getanimation (trapanimations [cell-129])^
          else
          begin
               cell2tex := 3;
               if cell < 224 then otex := trapitemssprites [cell-193]
               else
               begin
                    setcuranim (trapgunssprites [cell-225],0);
                    otex := getanimation (trapgunssprites [cell-225])^;
               end;
          end;
     end;
end;

procedure getcurof (x1,y1,x2,y2 : word);
var
bcell,cell,efect : byte;
px1,py1,px2,py2 : word;
fade,pfade : integer;
i,j : word;
begin
     px1 := x1 shr 5;
     py1 := y1 div 24;
     px2 := x2 shr 5;
     py2 := y2 div 24;
     fade := 0;
     for j := py1 to py2 do
     for i := px1 to px2 do
     begin
          bcell := trapcurrmap.trapmap^[j*trapcurrmap.trapmapsizex+i].backtex;
          cell := trapcurrmap.trapmap^[j*trapcurrmap.trapmapsizex+i].fadeandwater;
          pfade := cell and 31;
          efect := (bcell shr 7)+(cell shr 7)*2;
          if efect <> 0 then efect := trapefects [efect];
          pfade := pfade-efect;
          fade := fade+pfade;
     end;
     fade := fade div ((py2-py1+1)*(px2-px1+1));
     curof := fade;
end;

procedure drawmapbuf (x1,y1,x2,y2 : word; dx,dy : byte; drwplayer : boolean);
var
s,k,pr : word;
sx,sy,px,py,px0,py0,ppx0,ppy0 : integer;
dtx,dty : shortint;
state : byte;
i : word;
bcell,cell,fade,water,efect,inf1,inf2 : byte;
otexture : ttex;
inforecs : pinforecs;
c2tval : byte;
plsx,plsy : word;
begin
     with trapcurrmap do
     begin
     if x2 >= trapmapsizex then x2 := trapmapsizex-1;
     if y2 >= trapmapsizey then y2 := trapmapsizey-1;
     inforecs := ptr (trapinforecseg,0);
     sx := x1-2;
     if sx < 0 then sx := x1-1;
     if sx < 0 then sx := x1;
     if sx < 0 then syserror ('drawmapbuf()','sx < 0');
     sy := y1-2;
     if sy < 0 then sy := y1-1;
     if sy < 0 then sy := y1;
     if sx < 0 then syserror ('drawmapbuf()','sy < 0');
     s := sy*trapmapsizex+sx;
     k := y2*trapmapsizex+x2;
     pr := trapmapsizex-x2+sx;
     ppx0 := (sx-x1)*32;
     ppy0 := (sy-y1)*24;
     i := s;
     px := sx;
     py := sy;
     px0 := ppx0;
     py0 := ppy0;
     while (i <= k) do
     begin
          bcell := trapmap^[i].backtex;
          cell := trapmap^[i].fadeandwater;
          fade := cell and 31;
          water := (cell shr 5) and 3;
          efect := (bcell shr 7)+(cell shr 7)*2;
          bcell := bcell and 127;
          if bcell = 127 then bcell := 0;
          if efect <> 0 then efect := trapefects [efect];
          fade := fade-efect;
          if bcell <> 0 then
          begin
          if ((px0 < 0) or (py0 < 0)) and ((px0 >= -32) and (py0 >= -24)) then
          begin
               if (traptextures [bcell-1].sizex > 32) or (traptextures [bcell-1].sizey > 24) then
               scrtexf (px0-dx,py0-dy,fade,traptextures[bcell-1]);
          end;
          if (px0 >= 0) and (py0 >= 0) then
          begin
               cell := trapmap^[i].fronttex1;
               if (cell > 64) or (cell = 0) then
               begin
                    scrtexf (px0-dx,py0-dy,fade,traptextures[bcell-1])
               end
               else
               begin
                    inf1 := trapmap^[i].trapinforec1;
                    if (inf1 > 0) and (inf1 <= maxinforecs) then
                    begin
                         scrtexf (px0-dx,py0-dy,fade,traptextures[bcell-1]);
                    end
                    else
                    begin
                    inf2 := trapmap^[i].trapinforec2;
                    if (inf2 > 0) and (inf2 <= maxinforecs) then
                    begin
                         scrtexf (px0-dx,py0-dy,fade,traptextures[bcell-1]);
                    end
                    else
                    if (traptextures[bcell-1].sizex > 32)
                    and (traptextures[cell-1].sizex <= 32) then
                    scrtexf (px0-dx,py0-dy,fade,traptextures[bcell-1]);
                    end;
               end;
          end;
          end;
          inc (px);
          inc (px0,32);
          if px > x2 then
          begin
               px := sx;
               px0 := (sx-x1)*32;
               inc (py);
               inc (py0,24);
               inc (i,pr);
          end
          else
          inc (i);
     end;
     {}
     i := s;
     px := sx;
     py := sy;
     px0 := ppx0;
     py0 := ppy0;
     while (i <= k) do
     begin
          bcell := trapmap^[i].backtex;
          cell := trapmap^[i].fadeandwater;
          fade := cell and 31;
          water := (cell shr 5) and 3;
          efect := (bcell shr 7)+(cell shr 7)*2;
          if efect <> 0 then efect := trapefects [efect];
          fade := fade-efect;
          {}
          if (px0 < -32) or (py0 < -24) then
          begin
               inf1 := trapmap^[i].trapinforec1;
               if (inf1 <> 0) and (inf1 <= maxinforecs) then
               begin
                    c2tval := cell2tex (otexture,trapmap^[i].fronttex1);
                    if c2tval <> 0 then
                    if (otexture.sizex > 32) or (otexture.sizey > 24) then
                    begin
                         dtx := inforecs^[inf1-1].objectpozx-dx;
                         dty := inforecs^[inf1-1].objectpozy-dy;
                         state := inforecs^[inf1-1].objectstate;
                         if (state shr 1) and 1 = 1 then fade := state shr 3;
                         if (state shr 2) and 1 = 0 then
                         begin
                              if c2tval = 1 then scrtexf (px0+dtx,py0+dty,fade,otexture)
                              else scrsprtexf (px0+dtx,py0+dty,fade,otexture);
                         end
                         else
                         begin
                              if c2tval = 1 then scrtextf (px0+dtx,py0+dty,fade,otexture)
                              else scrsprtextf (px0+dtx,py0+dty,fade,otexture);
                         end;
                    end;
               end;
          end;
          {}
          if ((px0 < 0) or (py0 < 0)) and ((px0 >= -32) and (py0 >= -24)) then
          begin
               c2tval := cell2tex (otexture,trapmap^[i].fronttex1);
               if c2tval <> 0 then
               begin
               if (otexture.sizex > 32) or (otexture.sizey > 24) then
               begin
                    inf1 := trapmap^[i].trapinforec1;
                    if (inf1 <> 0) and (inf1 <= maxinforecs) then
                    begin
                         dtx := inforecs^[inf1-1].objectpozx-dx;
                         dty := inforecs^[inf1-1].objectpozy-dy;
                         state := inforecs^[inf1-1].objectstate;
                         if (state shr 1) and 1 = 1 then fade := state shr 3;
                    end
                    else
                    begin
                         dtx := -dx;
                         dty := -dy;
                         state := 0;
                    end;
                    if (state shr 2) and 1 = 0 then
                    begin
                         if c2tval = 1 then scrtexf (px0+dtx,py0+dty,fade,otexture)
                         else scrsprtexf (px0+dtx,py0+dty,fade,otexture);
                    end
                    else
                    begin
                         if c2tval = 1 then scrtextf (px0+dtx,py0+dty,fade,otexture)
                         else scrsprtextf (px0+dtx,py0+dty,fade,otexture);
                    end;
               end
               else
               begin
                    inf1 := trapmap^[i].trapinforec1;
                    if (inf1 <> 0) and (inf1 <= maxinforecs) then
                    begin
                         dtx := inforecs^[inf1-1].objectpozx-dx;
                         dty := inforecs^[inf1-1].objectpozy-dy;
                         state := inforecs^[inf1-1].objectstate;
                         if (state shr 1) and 1 = 1 then fade := state shr 3;
                         if (state shr 2) and 1 = 0 then
                         begin
                              if c2tval = 1 then scrtexf (px0+dtx,py0+dty,fade,otexture)
                              else scrsprtexf (px0+dtx,py0+dty,fade,otexture);
                         end
                         else
                         begin
                              if c2tval = 1 then scrtextf (px0+dtx,py0+dty,fade,otexture)
                              else scrsprtextf (px0+dtx,py0+dty,fade,otexture);
                         end;
                    end;
               end;
               end;
               {}
          end;
          {}
          if (px0 >= 0) and (py0 >= 0) then
          begin
               c2tval := cell2tex (otexture,trapmap^[i].fronttex1);
               if c2tval <> 0 then
               begin
                    inf1 := trapmap^[i].trapinforec1;
                    if c2tval = 3 then
                    begin
                         dtx := 16-otexture.sizex shr 1-dx;
                         dty := 24-otexture.sizey-dy;
                         state := 0;
                    end
                    else
                    if (inf1 <> 0) and (inf1 <= maxinforecs) then
                    begin
                         dtx := inforecs^[inf1-1].objectpozx-dx;
                         dty := inforecs^[inf1-1].objectpozy-dy;
                         state := inforecs^[inf1-1].objectstate;
                         if (state shr 1) and 1 = 1 then fade := (state shr 3) and 31;
                    end
                    else
                    begin
                         dtx := -dx;
                         dty := -dy;
                         state := 0;
                    end;
                    if (state shr 2) and 1 = 0 then
                    begin
                         if c2tval = 1 then scrtexf (px0+dtx,py0+dty,fade,otexture)
                         else scrsprtexf (px0+dtx,py0+dty,fade,otexture);
                    end
                    {}
                    else
                    begin
                         if c2tval = 1 then scrtextf (px0+dtx,py0+dty,fade,otexture)
                         else scrsprtextf (px0+dtx,py0+dty,fade,otexture);
                    end;
               end;
          end;
          inc (px);
          inc (px0,32);
          if px > x2 then
          begin
               px := sx;
               px0 := (sx-x1)*32;
               inc (py);
               inc (py0,24);
               inc (i,pr);
          end
          else
          inc (i);
     end;
     {}
     i := s;
     px := sx;
     py := sy;
     px0 := ppx0;
     py0 := ppy0;
     while (i <= k) do
     begin
          bcell := trapmap^[i].backtex;
          cell := trapmap^[i].fadeandwater;
          fade := cell and 31;
          water := (cell shr 5) and 3;
          efect := (bcell shr 7)+(cell shr 7)*2;
          if efect <> 0 then efect := trapefects [efect];
          fade := fade-efect;
          {}
          if (px0 < -32) or (py0 < -24) then
          begin
               inf2 := trapmap^[i].trapinforec2;
               if (inf2 <> 0) and (inf2 <= maxinforecs) then
               begin
                    c2tval := cell2tex (otexture,trapmap^[i].fronttex2);
                    if c2tval <> 0 then
                    if (otexture.sizex > 32) or (otexture.sizey > 24) then
                    begin
                         dtx := inforecs^[inf2-1].objectpozx-dx;
                         dty := inforecs^[inf2-1].objectpozy-dy;
                         state := inforecs^[inf2-1].objectstate;
                         if (state shr 1) and 1 = 1 then fade := state shr 3;
                         if (state shr 2) and 1 = 0 then
                         begin
                              if c2tval = 1 then scrtexf (px0+dtx,py0+dty,fade,otexture)
                              else scrsprtexf (px0+dtx,py0+dty,fade,otexture);
                         end
                         else
                         begin
                              if c2tval = 1 then scrtextf (px0+dtx,py0+dty,fade,otexture)
                              else scrsprtextf (px0+dtx,py0+dty,fade,otexture);
                         end;
                    end;
               end;
          end;
          {}
          if ((px0 < 0) or (py0 < 0)) and ((px0 >= -32) and (py0 >= -24)) then
          begin
               {}
               c2tval := cell2tex (otexture,trapmap^[i].fronttex2);
               if c2tval <> 0 then
               if (otexture.sizex > 32) or (otexture.sizey > 24) then
               begin
                    inf2 := trapmap^[i].trapinforec2;
                    if (inf2 <> 0) and (inf2 <= maxinforecs) then
                    begin
                         dtx := inforecs^[inf2-1].objectpozx-dx;
                         dty := inforecs^[inf2-1].objectpozy-dy;
                         state := inforecs^[inf2-1].objectstate;
                         if (state shr 1) and 1 = 1 then fade := state shr 3;
                    end
                    else
                    begin
                         dtx := -dx;
                         dty := -dy;
                         state := 0;
                    end;
                    if (state shr 2) and 1 = 0 then
                    begin
                         if c2tval = 1 then scrtexf (px0+dtx,py0+dty,fade,otexture)
                         else scrsprtexf (px0+dtx,py0+dty,fade,otexture);
                    end
                    else
                    begin
                         if c2tval = 1 then scrtextf (px0+dtx,py0+dty,fade,otexture)
                         else scrsprtextf (px0+dtx,py0+dty,fade,otexture);
                    end;
               end
               else
               begin
                    inf2 := trapmap^[i].trapinforec2;
                    if (inf2 <> 0) and (inf2 <= maxinforecs) then
                    begin
                         dtx := inforecs^[inf2-1].objectpozx-dx;
                         dty := inforecs^[inf2-1].objectpozy-dy;
                         state := inforecs^[inf2-1].objectstate;
                         if (state shr 1) and 1 = 1 then fade := state shr 3;
                         if (state shr 2) and 1 = 0 then
                         begin
                              if c2tval = 1 then scrtexf (px0+dtx,py0+dty,fade,otexture)
                              else scrsprtexf (px0+dtx,py0+dty,fade,otexture);
                         end
                         else
                         begin
                              if c2tval = 1 then scrtextf (px0+dtx,py0+dty,fade,otexture)
                              else scrsprtextf (px0+dtx,py0+dty,fade,otexture)
                         end;
                    end;
               end;
          end;
          {}
          if (px0 >= 0) and (py0 >= 0) then
          begin
               c2tval := cell2tex (otexture,trapmap^[i].fronttex2);
               if c2tval <> 0 then
               begin
                    inf2 := trapmap^[i].trapinforec2;
                    if c2tval = 3 then
                    begin
                         dtx := 16-otexture.sizex shr 1-dx;
                         dty := 24-otexture.sizey-dy;
                    end
                    else
                    if (inf2 <> 0) and (inf2 <= maxinforecs) then
                    begin
                         dtx := inforecs^[inf2-1].objectpozx-dx;
                         dty := inforecs^[inf2-1].objectpozy-dy;
                         state := inforecs^[inf2-1].objectstate;
                         if (state shr 1) and 1 = 1 then fade := state shr 3;
                    end
                    else
                    begin
                         dtx := -dx;
                         dty := -dy;
                         state := 0;
                    end;
                    if (state shr 2) and 1 = 0 then
                    begin
                         if c2tval = 1 then scrtexf (px0+dtx,py0+dty,fade,otexture)
                         else scrsprtexf (px0+dtx,py0+dty,fade,otexture)
                    end
                    else
                    begin
                         if c2tval = 1 then scrtextf (px0+dtx,py0+dty,fade,otexture)
                         else scrsprtextf (px0+dtx,py0+dty,fade,otexture)
                    end;
               end;
          end;
          inc (px);
          inc (px0,32);
          if px > x2 then
          begin
               px := sx;
               px0 := (sx-x1)*32;
               inc (py);
               inc (py0,24);
               inc (i,pr);
          end
          else
          inc (i);
     end;
     {}
     {i := (players[0].playerpozy div 24)*trapmapsizex+(players[0].playerpozx div 32);
     bcell := trapmap^[i].backtex;
     cell := trapmap^[i].fadeandwater;
     fade := cell and 31;
     efect := (bcell shr 7)+(cell shr 7)*2;
     if efect <> 0 then efect := trapefects [efect];
     fade := fade-efect;}
     if drwplayer then
     begin
     getplayersxsy (plsx,plsy);
     getcurof (
     players[0].playerpozx,
     players[0].playerpozy,
     players[0].playerpozx+plsx-1,
     players[0].playerpozy+plsy-1);
     drawplayer (players[0].playerpozx-trapmapviewshfx,players[0].playerpozy-trapmapviewshfy,
     trapcurrmap.players[0].state,curof);
     end;
     {}
     i := s;
     px := sx;
     py := sy;
     px0 := ppx0;
     py0 := ppy0;
     while (i <= k) do
     begin
          cell := trapmap^[i].fadeandwater;
          water := (cell shr 5) and 3;
          if water <> 0 then scrtranspwin (px0-dx,py0-dy,px0+32-dx,py0+24-dy,watercolors [water]);
          inc (px);
          inc (px0,32);
          if px > x2 then
          begin
               px := sx;
               px0 := (sx-x1)*32;
               inc (py);
               inc (py0,24);
               inc (i,pr);
          end
          else
          inc (i);
     end;
     {}
     end;
end;

procedure updatetrapefects;
var
p : byte;
fcnt,fskip : integer;
begin
     getframescnt (efect_1_fps,fcnt,fskip);
     if trapframes mod fcnt = fcnt-1 then
     for p := 0 to fskip do
     begin
          trapefects[1] := trapefects[1]+deltatefect1;
          if trapefects[1] = 31 then deltatefect1 := -1;
          if trapefects[1] = 0 then deltatefect1 := 1;
     end;
     getframescnt (efect_2_3_fps,fcnt,fskip);
     if trapframes mod fcnt = fcnt-1 then
     begin
          p := random (2);
          trapefects[2] := p*31;
          trapefects[3] := (p*3);
     end;
end;

procedure drawtrapconsole;
var
i : byte;
begin
     with trapconsole do
     begin
     if currconstr = 0 then exit;
     textsetcolor (176);
     for i := 1 to currconstr do
     begin
          textprint (1,(i-1)*6+1,0,1,@constack[i-1]);
     end;
     end;
end;

procedure draweditconsole;
begin
     textsetcolor (176);
     textprint (1,(configrec.xscrsy-6) mod 200,0,1,':>');
     textprint (13,(configrec.xscrsy-6) mod 200,0,1,editconsolepchar);
     hline (13+strlen (editconsolepchar)*6,(configrec.xscrsy-2) mod 200,17+strlen (editconsolepchar)*6,random (11)+179)
end;

procedure drawlosthealth;
var
fcnt,fskip : integer;
begin
     playerdeltahealthf := playerdeltahealthf+playerdeltahealth;
     if playerdeltahealthf <= 0 then exit;
     scrtranspwin (0,0,320,200,22);
     getframescnt (50 shl 8,fcnt,fskip);
     if trapframes mod fcnt = fcnt-1 then
     dec (playerdeltahealthf,fskip+1);
end;

procedure drawplayeruwbar;
const
puwbsx = 60;
puwbsy = 10;
var
pv,modv : floatint;
px : word;
ppx : integer;
csw : longint;
begin
     px := 160-puwbsx shr 1;
     csw := clock-trapcurrmap.players[0].watertime;
     rect (px-1,(configrec.xscrsy-2-puwbsy-1) mod 200,px+puwbsx,(configrec.xscrsy-2) mod 200,127);
     if csw > UNDERWATERMAXTIME then
     begin
          scrtranspwin (px,(configrec.xscrsy-2-puwbsy) mod 200,px+puwbsx,(configrec.xscrsy-2) mod 200,31);
          exit;
     end;
     floatint_div (pv,modv,csw shl 8,UNDERWATERMAXTIME shl 8);
     floatint_sub (pv,256,pv);
     floatint_mul (pv,pv,puwbsx shl 8);
     floatint_rnd (ppx,pv);
     scrtranspwin (px,(configrec.xscrsy-2-puwbsy) mod 200,px+ppx,(configrec.xscrsy-2) mod 200,95);
end;

procedure processtimers;
var
v : longint;
begin
     if gamestate = GAMESTATE_TITLE then
     begin
          stopstimer (titletimer,timer_in_seconds,v);
          if v >= TIMETITLE then
          begin
               gamestate := GAMESTATE_GAME;
               if configrec.usmus = 1 then
               musicplayed := false;
          end;
     end;
     con_update (trapconsole);
     updatetrapefects;
end;

procedure gettrapfps;
var
msec : longint;
modv : floatint;
begin
     msec := 0;
     stopstimer (trapfpstimer,timer_in_milli_seconds,msec);
     if msec >= 945 then
     begin
          trapfps := (trapframes-trapbframes) shl 8;
          trapbframes := trapframes;
          startstimer (trapfpstimer);
          trapstep := 65536;
          if trapfps <> 0 then
          begin
               floatint_div (trapstep,modv,trapstep,trapfps);
               {floatint_mul (trapstep,trapstep,65536);}
          end
          else
          trapstep := STDTRAPSTEP;
     end;
     {trapfps := ((trapframes shl 8) shl floatintshift) div (sec shl 8);}
end;

procedure drawfps;
var
ppchar : pchar;
rndi : integer;
begin
     getmem (ppchar,9);
     floatint_rnd (rndi,trapfps);
     val2pchar (rndi,ppchar);
     textsetcolor (176);
     textprint ((configrec.xscrsx-2) mod 320-strlen (ppchar)*6,1,0,1,ppchar);
     freemem (ppchar,9);
end;

procedure drawplayerstatush;
var
ppchar : pchar;
i : byte;
begin
     getmem (ppchar,10);
     {}
     textsettex (0,0,texttex1);
     scrtextf (3,(configrec.xscrsy-13) mod 200,31,menu832texs[7]);
     val2pchar (trapcurrmap.players[0].health,ppchar);
     textprint (13,(configrec.xscrsy-13) mod 200,1,0,ppchar);
     {}
     for i := 0 to 2 do
     if playerkeys[i] then scrsprtextf (i*(trapitemssprites[i+skeyitems].sizex+1)+54,
     (configrec.xscrsy-4-trapitemssprites[i+skeyitems].sizey) mod 200,
     trapefects[1],trapitemssprites[i+skeyitems]);
     {}
     freemem (ppchar,10);
end;

procedure drawplayerstatusa;
var
ppchar : pchar;
begin
     getmem (ppchar,10);
     {}
     textsettex (0,0,texttex1);
     if trapcurrmap.players[0].igun <> 0 then
     begin
     scrsprtextf ((configrec.xscrsx-40) mod 320,
     (configrec.xscrsy-4-trapitemssprites[gun2ammotab[trapcurrmap.players[0].igun]*2+
     sammoitems].sizey) mod 200,31,trapitemssprites[gun2ammotab[trapcurrmap.players[0].igun]*2+sammoitems]);
     {}
     val2pchar (trapcurrmap.players[0].playerammo[gun2ammotab[trapcurrmap.players[0].igun]],ppchar);
     textprint ((configrec.xscrsx-33) mod 320,(configrec.xscrsy-13) mod 200,1,0,ppchar);
     end;
     {}
     freemem (ppchar,10);
end;

procedure startframe;
begin
     playerdeltayvel := playerphysicobj.physicobjyvelocity;
     playerisuse := false;
end;

procedure updateallanimations;
var
i : byte;
begin
     for i := 0 to ntrapanimations do animanimation (trapanimations [i]);
end;

procedure updateplayer;
var
pacc : floatint;
rx,ry : integer;
paccel : floatint;
hcol : byte;
playersx,playersy : word;
p : pchar;
pw : floatint;
fcnt,fskip : integer;
bpopx,bpopy : floatint;
ppobj : tphysicobj;
gsx,gsy : word;
hdirection : shortint;
begin
     if noclipmode then playerisongrab := true;
     pwaterw := isplayerinwater;
     if playermov <> 1 then
     begin
          if not pcrouch then
          begin
               if (trapcurrmap.players[0].state < 4) and (trapcurrmap.players[0].state <> 1) then
               trapcurrmap.players[0].state := 0
               else
               if (trapcurrmap.players[0].state < 6) and (trapcurrmap.players[0].state <> 0) then
               trapcurrmap.players[0].state := 1;
          end
          else
          begin
               if (trapcurrmap.players[0].state < 10) and (trapcurrmap.players[0].state <> 7) then
               trapcurrmap.players[0].state := 6
               else
               if (trapcurrmap.players[0].state < 12) and (trapcurrmap.players[0].state <> 6) then
               trapcurrmap.players[0].state := 7;
          end;
          if playerphysicobj.physicobjxvelocity <> 0 then
          begin
          if (trapcurrmap.players[0].state = 0) or (trapcurrmap.players[0].state = 6) then
          begin
               paccel := -playerphysicobj.physicobjxaccel;
               update_velocity (playerphysicobj,paccel,0,trapstep);
               if playerphysicobj.physicobjxvelocity < 0 then playerphysicobj.physicobjxvelocity := 0;
          end
          else
          begin
               paccel := playerphysicobj.physicobjxaccel;
               update_velocity (playerphysicobj,paccel,0,trapstep);
               if playerphysicobj.physicobjxvelocity > 0 then playerphysicobj.physicobjxvelocity := 0;
          end
          end;
          playerashf := 0;
          playerhshf := 0;
          playersteps := 2;
     end
     else
     inc (playersteps);
     {}
     if (pcrouch) and (pludmovd = false) then
     playercrouch (0);
     if pwaterw <> $fffffff then
     if (pcrouch) and (not shiftkp) and (pludmovd = false) then
     playercrouch (0);
     {}
     if playerisongrab then
     begin
          if pludmovd = false then playerphysicobj.physicobjyvelocity := 0;
          if plrlmovd = false then playerphysicobj.physicobjxvelocity := 0;
     end;
     {}
     if pcrouch then playerphysicobj.physicobjmaxxvel := maxpvel shr 1
     else
     playerphysicobj.physicobjmaxxvel := maxpvel;
     if shiftkp then playerphysicobj.physicobjmaxxvel := playerphysicobj.physicobjmaxxvel shr 1;
     if playerisongrab then playerphysicobj.physicobjmaxyvel := playerphysicobj.physicobjmaxxvel
     else
     playerphysicobj.physicobjmaxyvel := worldphysicobj.physicobjmaxyvel;
     if not playerisongrab then
     begin
          if pwaterw = $fffffff then
          update_velocity (playerphysicobj,0,worldphysicobj.physicobjyaccel,trapstep)
          else
          begin
               floatint_sub (pw,worldphysicobj.physicobjyaccel,pwaterw);
               update_velocity (playerphysicobj,0,pw,trapstep);
               getframescnt (STDOT shl 8,fcnt,fskip);
               if trapframes mod fcnt = fcnt-1 then
               for fcnt := 0 to fskip do
               begin
                    floatint_mul (playerphysicobj.physicobjxvelocity,playerphysicobj.physicobjxvelocity,STDO - (STDO div 8));
                    floatint_mul (playerphysicobj.physicobjyvelocity,playerphysicobj.physicobjyvelocity,STDO);
               end;
          end;
     end;
     if playeronmap then
     update_physicobj (playerphysicobj,trapstep);
     with trapcurrmap.players[0] do
     begin
          getplayersxsy (playersx,playersy);
          playerdeltahealth := health;
          {}
          hcol := 0;
          if playeronmap then
          if noclipmode = false then
          begin
               bpopx := playerphysicobj.physicobjpozx;
               bpopy := playerphysicobj.physicobjpozy;
               {}
               (*hdirection := directiontab [trapcurrmap.players[0].state];
               if hdirection = -1 then
               begin
                    setcuranim (handanimations,3);
                    setcuranim (trapgunssprites[trapcurrmap.players[0].igun],1+plguna);
                    gsx := getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizex;
                    gsy := getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizey;
                    {}
                    ppobj.physicobjpozx := playerphysicobj.physicobjpozx+longint (
                    playerlinks[trapcurrmap.players[0].state*2].mx+
                    playerhshf-gsx+6) shl 8;
                    {}
                    ppobj.physicobjpozy := playerphysicobj.physicobjpozy+longint (
                    playerlinks[trapcurrmap.players[0].state*2].my+
                    getanimation (handanimations)^.sizey-gsy) shl 8;
               end;
               if hdirection = 1 then
               begin
                    setcuranim (handanimations,0);
                    setcuranim (trapgunssprites[trapcurrmap.players[0].igun],0+plguna);
                    gsx := getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizex;
                    gsy := getanimation (trapgunssprites[trapcurrmap.players[0].igun])^.sizey;
                    {}
                    ppobj.physicobjpozx := playerphysicobj.physicobjpozx+longint (
                    playerlinks[trapcurrmap.players[0].state*2].mx+
                    getanimation (handanimations)^.sizex+
                    playerhshf+gsx-4) shl 8;
                    {}
                    ppobj.physicobjpozy := playerphysicobj.physicobjpozy+longint (
                    playerlinks[trapcurrmap.players[0].state*2].my+
                    getanimation (handanimations)^.sizey-gsy) shl 8;
               end;
               hcol := handlecellcol (ppobj,gsx,gsy,false);
               if hcol <> 0 then
               begin
                    if hdirection = -1 then
                    begin
                         playerphysicobj.physicobjpozx := ppobj.physicobjpozx-
                         longint (playerlinks[trapcurrmap.players[0].state*2].mx+
                         playerhshf-gsx+7) shl 8;
                         playerphysicobj.physicobjpozy := ppobj.physicobjpozy-
                         longint (playerlinks[trapcurrmap.players[0].state*2].my+
                         getanimation (handanimations)^.sizey-gsy) shl 8;
                    end;
                    if hdirection = 1 then
                    begin
                         playerphysicobj.physicobjpozx := ppobj.physicobjpozx-
                         longint (playerlinks[trapcurrmap.players[0].state*2].mx+
                         getanimation (handanimations)^.sizex+
                         playerhshf+gsx-4) shl 8;
                         playerphysicobj.physicobjpozy := ppobj.physicobjpozy-
                         longint (playerlinks[trapcurrmap.players[0].state*2].my+
                         getanimation (handanimations)^.sizey-gsy) shl 8;
                    end;
               end;*)
               {}
               hcol := handlecellcol (playerphysicobj,playersx,playersy,true);
               {}
               if abs (playerphysicobj.physicobjpozx-bpopx) > (playersx shr 1)*256 then hcol := LU+RU+LD+RD;
               if abs (playerphysicobj.physicobjpozy-bpopy) > (playersy shr 1)*256 then hcol := LU+RU+LD+RD;
          end;
          {}
          if hcol and (LU+RU+LD+RD)=(LU+RU+LD+RD) then health := HDELOBJV-1;
          {}
          rnd_physicobj (rx,ry,playerphysicobj);
          {}
          floatint_sub (playerdeltayvel,playerphysicobj.physicobjyvelocity,playerdeltayvel);
          if playeronmap then
          handleplayerbyvel;
          {}
          if playerdwaterhd = 0 then
          begin
          if playerunderwater then
          begin
               if clock-watertime > UNDERWATERMAXTIME then
               begin
                    if (clock mod UNDERWATERHDT = UNDERWATERHDT-1) and (clock <> playeruwptime) then
                    begin
                         int_idval (health,-UNDERWATERHD,0,200);
                         playeruwptime := clock;
                    end;
               end;
          end;
          end
          {}
          else
          begin
               if (clock mod UNDERWATERHDT = UNDERWATERHDT-1) and (clock <> playeruwptime) then
               begin
                    int_idval (health,-playerdwaterhd,0,200);
                    playeruwptime := clock;
               end;
          end;
          playerdeltahealth := playerdeltahealth-health;
          if playeriskilled = false then
          if godmode then trapcurrmap.players[0].health := 200;
          if health <= 0 then
          begin
               killplayer;
          end;
          if playerdeltahealth > 0 then
          begin
               if playeriskilled = false then
               if playerdeltahealth <= 50 then xmssnd_addsample (playersounds[2],128)
               else
               xmssnd_addsample (playersounds[3],128);
               playerdeltahealth := playerdeltahealth shr 4+3;
          end;
          if rx < 0 then
          begin
               rx := 0;
               playerphysicobj.physicobjpozx := 0;
          end;
          if ry < 0 then
          begin
               ry := 0;
               playerphysicobj.physicobjpozy := 0;
          end;
          if rx > mtrapmapsizex-playersx then
          begin
               rx := mtrapmapsizex-playersx;
               playerphysicobj.physicobjpozx := longint (rx) shl 8;
          end;
          if ry > mtrapmapsizey-playersy then
          begin
               ry := mtrapmapsizey-playersy;
               playerphysicobj.physicobjpozy := longint (ry) shl 8;
          end;
          if playerphysicobj.physicobjxvelocity = 0 then
          begin
               playerashf := 0;
               playerhshf := 0;
               if not pcrouch then
               begin
               if (trapcurrmap.players[0].state < 4) and (trapcurrmap.players[0].state <> 1) then
               trapcurrmap.players[0].state := 0
               else
               if (trapcurrmap.players[0].state < 6) and (trapcurrmap.players[0].state <> 0) then
               trapcurrmap.players[0].state := 1;
               end
               else
               begin
               if (trapcurrmap.players[0].state < 10) and (trapcurrmap.players[0].state <> 7) then
               trapcurrmap.players[0].state := 6
               else
               if (trapcurrmap.players[0].state < 12) and (trapcurrmap.players[0].state <> 6) then
               trapcurrmap.players[0].state := 7;
               end;
          end;
          playerpozx := rx;
          playerpozy := ry;
          isitem (playerpozx,playerpozy,playersx,playersy);
          {}
          if trapcurrmap.trapmapsizex shl 5 > configrec.xscrsx then
          rx := playerpozx-scrox+playersx shr 1
          else
          rx := 0;
          if trapcurrmap.trapmapsizey*24 > configrec.xscrsy then
          ry := playerpozy-scroy+playersy shr 1
          else
          ry := 0;
          if rx < 0 then rx := 0;
          if ry < 0 then ry := 0;
          trapmapviewshfx := rx;
          trapmapviewshfy := ry;
          if trapmapviewshfx > mtrapmapsizex-configrec.xscrsx then trapmapviewshfx := mtrapmapsizex-configrec.xscrsx;
          if trapmapviewshfy > mtrapmapsizey-configrec.xscrsy then trapmapviewshfy := mtrapmapsizey-configrec.xscrsy;
          {}
          if playeriskilled then playershoting := true;
          {getmem (p,9);
          val2pchar (usedtrapparts,p);
          con_addstr (trapconsole,p);
          freemem (p,9);}
     end;
     playermov := 0;
     plrlmovd := false;
     pludmovd := false;
end;

procedure update;
begin
     if gamepaused = false then
     if gamestate = GAMESTATE_GAME then
     begin
          updateplayer;
          updateallanimations;
          updatetrapparts;
          s_processallmap;
     end;
end;

procedure render;
var
i,j,p,k : word;
scrx,scry : word;
ptmx,ptmy : word;
begin
     isupdateshot := true;
     if configrec.scrmode = 0 then
     begin
     buftoclr;
     if (gamestate = GAMESTATE_TITLE){ or (gamestate = GAMESTATE_GAME)} then
     begin
          tex320x200 (traptitle,XMSMEM);
          textsetfade (96);
          textprint (290,192,0,1,'V.');
          textprint (290+12,192,0,1,ver);
          textsetfade (118);
          textprint (291,193,0,1,'V.');
          textprint (291+12,193,0,1,ver);
     end;
     if gamestate = GAMESTATE_GAME then
     begin
          if maploaded then
          begin
               {drawmapbuf (0,0,10,8{trapcurrmap.trapmapsizex,trapcurrmap.trapmapsizey}{,0,0);}
               drawmapbuf (trapmapviewshfx div 32,trapmapviewshfy div 24,
               trapmapviewshfx div 32+10,trapmapviewshfy div 24+9,trapmapviewshfx mod 32,trapmapviewshfy mod 24,playeronmap);
               drawtrapparts (trapmapviewshfx,trapmapviewshfy,trapmapviewshfx+320,trapmapviewshfy+200);
               drawplayerstatush;
               drawplayerstatusa;
               drawlosthealth;
               if playerunderwater then drawplayeruwbar;
               if gamepaused = false then playerupdateshot;
          end;
     end;
     {rysowanie menu}
     if trapmenuon then
     begin
          {for k := 0 to 19 do
          for i := 0 to 31 do
          scrtextf (i*10,k*10,10,texttex1);
          {for i := 0 to 4 do rect (20+i,20+i,300-i,180-i,127-(i*5));
          {scrfadewin (25,25,296,176,10);}
          scrtranspwin (0,0,320,200,64);
          {tytul menu}
          textsettex (0,0,texttex2);
          textprint (160-strlen (trapcurrmenu^.menutitle)*11 div 2,28,0,0,trapcurrmenu^.menutitle);
          {tresc menu}
          textsettex (0,0,texttex1);
          p := 0;
          for i := 0 to trapcurrmenu^.numoftxts-1 do
          begin
               if trapcurrmenu^.menutxts^[i][0] <> #0 then
               textprint (50,100-(trapcurrmenu^.numoftxts*14) div 2+i*14+p,0,0,trapcurrmenu^.menutxts^[i])
               else
               begin
                    if strcomp (trapcurrmenu^.menucoms^[i],'LISTKEYS') = 0 then p := 6*5 else
                    if strcomp (trapcurrmenu^.menucoms^[i],'LISTSYSINFO') = 0 then p := 6*5 else
                    if strcomp (trapcurrmenu^.menucoms^[i],'LISTHELP') = 0 then p := 6*5;
               end;
               drawcommand (i);
          end;
          {pomoc menu}
          if trapcurrmenu^.menuhelp[0] <> #0 then
          textprint (160-strlen (trapcurrmenu^.menuhelp)*6 div 2,170,0,1,trapcurrmenu^.menuhelp);
          if strcomp (trapcurrmenu^.menucoms^[trapcurrmenu^.menupoz],'LISTSYSINFO') = 0 then inc (trapcurrmenu^.menupoz);
          if strcomp (trapcurrmenu^.menucoms^[trapcurrmenu^.menupoz],'LISTHELP') = 0 then inc (trapcurrmenu^.menupoz);
          if strcomp (trapcurrmenu^.menucoms^[trapcurrmenu^.menupoz],'LISTKEYS') <> 0 then
          scrsprtexnf (50-17-3,trapcurrmenu^.menupoz*14+100-(trapcurrmenu^.numoftxts*14) div 2-3+p,getanimation (trapcur)^);
          animanimation (trapcur);
     end;
     {}
     {rysowanie konsoli}
     drawtrapconsole;
     if drawtrapfps then drawfps;
     if editconsole then draweditconsole;
     {}
     {skopiowanie bufora na ekran}
     if iswait4vsync then {for i := 0 to 10 do} wfvr;
     buftoscr
     end
     {-----------------------------------------}
     {je˜li vesa}
     else
     begin                                      {}
     ptmx := trapmapviewshfx;
     ptmy := trapmapviewshfy;
     {}
     for scry := 0 to xscrscrsy do
     begin
     for scrx := 0 to xscrscrsx do
     begin
     buftoclr;
     if (gamestate = GAMESTATE_TITLE) then
     begin
          tex320x200 (traptitle,XMSMEM);
          if (scrx = xscrscrsx) and (scry = xscrscrsy) then
          begin
               textsetfade (96);
               textprint (290,192,0,1,'V.');
               textprint (290+12,192,0,1,ver);
               textsetfade (118);
               textprint (291,193,0,1,'V.');
               textprint (291+12,193,0,1,ver);
          end;
     end;
     if gamestate = GAMESTATE_GAME then
     begin
          if maploaded then
          begin
               {drawmapbuf (0,0,10,8{trapcurrmap.trapmapsizex,trapcurrmap.trapmapsizey}{,0,0);}
               drawmapbuf (trapmapviewshfx div 32,trapmapviewshfy div 24,
               trapmapviewshfx div 32+10,trapmapviewshfy div 24+9,trapmapviewshfx mod 32,trapmapviewshfy mod 24,playeronmap);
               drawtrapparts (trapmapviewshfx,trapmapviewshfy,trapmapviewshfx+320,trapmapviewshfy+200);
               if (scrx = 0) and (scry = xscrscrsy) then
               drawplayerstatush;
               if (scrx = xscrscrsx) and (scry = xscrscrsy) then
               drawplayerstatusa;
               drawlosthealth;
               if playerunderwater then if (scrx = 0) and (scry = xscrscrsy) then drawplayeruwbar;
               if gamepaused = false then playerupdateshot;
          end;
     end;
     {rysowanie menu}
     if trapmenuon then
     begin
          scrtranspwin (0,0,320,200,64);
          {tytul menu}
          if (scrx = 0) and (scry = 0) then {}
          begin
          textsettex (0,0,texttex2);
          textprint (160-strlen (trapcurrmenu^.menutitle)*11 div 2,28,0,0,trapcurrmenu^.menutitle);
          {tresc menu}
          textsettex (0,0,texttex1);
          p := 0;
          for i := 0 to trapcurrmenu^.numoftxts-1 do
          begin
               if trapcurrmenu^.menutxts^[i][0] <> #0 then
               textprint (50,100-(trapcurrmenu^.numoftxts*14) div 2+i*14+p,0,0,trapcurrmenu^.menutxts^[i])
               else
               begin
                    if strcomp (trapcurrmenu^.menucoms^[i],'LISTKEYS') = 0 then p := 6*5 else
                    if strcomp (trapcurrmenu^.menucoms^[i],'LISTSYSINFO') = 0 then p := 6*5 else
                    if strcomp (trapcurrmenu^.menucoms^[i],'LISTHELP') = 0 then p := 6*5;
               end;
               drawcommand (i);
          end;
          {pomoc menu}
          if trapcurrmenu^.menuhelp[0] <> #0 then
          textprint (160-strlen (trapcurrmenu^.menuhelp)*6 div 2,170,0,1,trapcurrmenu^.menuhelp);
          if strcomp (trapcurrmenu^.menucoms^[trapcurrmenu^.menupoz],'LISTSYSINFO') = 0 then inc (trapcurrmenu^.menupoz);
          if strcomp (trapcurrmenu^.menucoms^[trapcurrmenu^.menupoz],'LISTHELP') = 0 then inc (trapcurrmenu^.menupoz);
          if strcomp (trapcurrmenu^.menucoms^[trapcurrmenu^.menupoz],'LISTKEYS') <> 0 then
          scrsprtexnf (50-17-3,trapcurrmenu^.menupoz*14+100-(trapcurrmenu^.numoftxts*14) div 2-3+p,getanimation (trapcur)^);
          animanimation (trapcur);
          end;                                {}
     end;
     {}
     {rysowanie konsoli}
     if (scrx = 0) and (scry = 0) then
     drawtrapconsole;
     if drawtrapfps then if (scrx = xscrscrsx) and (scry = 0) then drawfps;
     if editconsole then if (scrx = 0) and (scry = xscrscrsy) then draweditconsole;
     {}
     {skopiowanie bufora na ekran}
     xbuftoscr (scrx*320,scry*200,scrx*320+319,scry*200+199);
     {}
     inc (trapmapviewshfx,320);
     end;
     trapmapviewshfx := ptmx;
     inc (trapmapviewshfy,200);
     end;
     trapmapviewshfx := ptmx;
     trapmapviewshfy := ptmy;
     flipscr;
     if iswait4vsync then wfvr;
     end;                                         {}
end;

procedure setmenu (ms : boolean);
begin
     if ms = false then
     begin
          execmenucommand ('RETURN',0);
          if trapmenuon = false then
          begin
               _32fill (@keyon,128,0);
               stdkeyint := false;
               setnumlock (on);
               gamepaused := false;
          end;
     end
     else
     begin
          shiftkp := false;
          stdkeyint := true;
          trapmenuon := true;
          setnumlock (off);
          addsample (menutwvs[1],128,0);
          gamepaused := true;
     end;
end;

procedure seteditconsole (ms : boolean);
begin
     if ms then
     begin
          shiftkp := false;
          stdkeyint := true;
          editconsole := true;
          setnumlock (off);
          _32fill (editconsolepchar,consolestrlen,0);
          editconsolepchars := 0;
     end
     else
     begin
          _32fill (@keyon,128,0);
          stdkeyint := false;
          editconsole := false;
          setnumlock (on);
     end;
end;

procedure verifykeys;
var
i : byte;
pt : longint;
begin
     {w przypadku gdy zmieniamy ustawienia klawiatury}
     if setingkeys <> 255 then
     begin
          rkscan := getkey;
          if rkscan = 255 then exit;
          _32fill (@keyon,128,0);
          if not (rkscan in [59..68]) and not (rkscan in [87..88]) and (rkscan <> 1) and (rkscan <> 42) and (rkscan <> 54)
          then {nie f1..f12 shift esc}
          begin
          keyactionstab [setingkeys,keypoz] := rkscan;
          if setingkeys = 1 then
          begin
               setingkeys := 255;
               stdkeyint := true;
               _32fill (@keyon,128,0);
               setnumlock (off);
          end
          else setingkeys := 1;
          end;
          exit;
     end;
     {w przypadku gdy jest wlaczone menu}
     if trapmenuon then
     begin
     if readkey (rkchar,rkscan) then
     begin
          if rkscan = 1 then setmenu (false);
          {}
          if rkscan = KEY_UP then execmenucommand ('UP',0);
          if rkscan = KEY_DOWN then execmenucommand ('DOWN',0);
          if trapcurrmenu^.menupoz < 0 then trapcurrmenu^.menupoz := trapcurrmenu^.numoftxts-1;
          if trapcurrmenu^.menupoz > trapcurrmenu^.numoftxts-1 then trapcurrmenu^.menupoz := 0;
          {}
          if rkscan = 28 then execmenucommand (trapcurrmenu^.menucoms^[trapcurrmenu^.menupoz],0);
          if rkscan = KEY_LEFT then execmenucommand (trapcurrmenu^.menucoms^[trapcurrmenu^.menupoz],-1);
          if rkscan = KEY_RIGHT then execmenucommand (trapcurrmenu^.menucoms^[trapcurrmenu^.menupoz],1);
     end;
     end
     else
     if editconsole then
     begin
     if readkey (rkchar,rkscan) then
     begin
          if rkscan = 1 then seteditconsole (false);
          if rkscan = 41 then seteditconsole (false);
          {}
          if (rkchar >= #32) and (rkchar < #127) then
          begin
               if editconsolepchars < consolestrlen-1 then
               begin
                    editconsolepchar[editconsolepchars] := upcase (rkchar);
                    inc (editconsolepchars);
               end;
          end;
          if rkscan = 14 then
          begin
               if editconsolepchars > 0 then
               begin
                    dec (editconsolepchars);
                    editconsolepchar[editconsolepchars] := #0;
               end;
          end;
          if rkscan = KEY_UP then
          begin
               _32move (editconsolepcharl,editconsolepchar,consolestrlen);
               editconsolepchars := strlen (editconsolepcharl);
          end;
          if rkscan = 28 then
          begin
               execconsolecommand (editconsolepchar);
               _32fill (editconsolepcharl,consolestrlen,0);
               _32move (editconsolepchar,editconsolepcharl,consolestrlen);
               _32fill (editconsolepchar,consolestrlen,0);
               editconsolepchars := 0;
          end;
     end;
     end
     else
     begin
          if gamestate = GAMESTATE_TITLE then
          begin
               if keyon[41] = 1 then seteditconsole (true)
               else
               if anykeypressed then setmenu (true)
          end
          else
          begin
               if keyon[1] = 1 then setmenu (true);
               if keyon[41] = 1 then seteditconsole (true);
               if keyon[88] = 1 then
               begin
                    stopstimer (pauset,timer_in_milli_seconds,pt);
                    if pt > 200 then
                    begin
                         startstimer (pauset);
                         gamepaused := not gamepaused;
                    end;
               end;
               {}
               if playeriskilled = false then
               for i := 0 to maxguns-1 do if keyon[i+2]=1 then if trapcurrmap.players[0].playerguns[i] then
               trapcurrmap.players[0].igun := i;
               {if keyon [keyactionstab [FIRSTKEY,MOVERIGHT]] = 1 then
               word_idval (trapmapviewshfx,1,0,trapcurrmap.trapmapsizex*32-320);
               if keyon [keyactionstab [FIRSTKEY,MOVELEFT]] = 1 then
               word_idval (trapmapviewshfx,-1,0,trapcurrmap.trapmapsizex*32-320);
               if keyon [keyactionstab [FIRSTKEY,JUMP]] = 1 then
               word_idval (trapmapviewshfy,-1,0,trapcurrmap.trapmapsizey*24-200);
               if keyon [keyactionstab [FIRSTKEY,CROUCH]] = 1 then
               word_idval (trapmapviewshfy,1,0,trapcurrmap.trapmapsizey*24-200);
               if keyon [keyactionstab [FIRSTKEY,SHOT]] = 1 then
               word_idval (trapcurrmap.players[0].playerpozx,1,0,trapcurrmap.trapmapsizex*32);
               if keyon [keyactionstab [FIRSTKEY,USE]] = 1 then
               word_idval (trapcurrmap.players[0].playerpozy,1,0,trapcurrmap.trapmapsizey*24);}
               if keyon [keyactionstab [FIRSTKEY,JUMP]] = 1 then moveplayer (2);
               if keyon [keyactionstab [FIRSTKEY,CROUCH]] = 1 then moveplayer (3);
               if keyon [keyactionstab [FIRSTKEY,MOVERIGHT]] = 1 then moveplayer (0)
               else
               if keyon [keyactionstab [FIRSTKEY,MOVELEFT]] = 1 then moveplayer (1);
               if keyon [keyactionstab [FIRSTKEY,USE]] = 1 then playeruse;
               if keyon [keyactionstab [FIRSTKEY,SHOT]] = 1 then if playershoting = false then playershot;
               {if trapframes mod 10 = 0 then
               addtrappart (pparticle,random (31),15,10,0,10);}
          end;
          if numlockp = 1 then pressnumlock;
     end;
end;