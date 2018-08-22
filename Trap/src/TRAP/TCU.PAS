{Trap copyright (c) Kamil Korolkiewicz 2003/2004}
{Unit pomocniczy do gry TRAP}

{$G+}
unit tcu;

interface

uses strings,time,scr,snd,key,trapcrt;

{procedury i funkcje}

procedure val2pchar (vall : longint; var p : pchar);
procedure word2hex (wordd : word; var p : pchar);
procedure startstimer (var timer : stimer);
procedure stopstimer (var timer : stimer; typeoftimer : longint; var result : longint);
function getparams : word;
function parameq (pa,flags : word) : boolean;
procedure _32fill (b : pointer; c : word; v : byte);
procedure _32move (b1,b2 : pointer; c : word);
procedure int_idval (var ival : integer; idv : integer; llimit,hlimit : integer);
procedure word_idval (var ival : word; idv : integer; llimit,hlimit : word); {hlimit < 32768}
function getvsyncfreq : word;
function allocmem (s : word; align : word) : pointer;
procedure getstr (var p1,p2 : pchar; stri : word);
procedure getcom (var p1,p2 : pchar; stri : word);
procedure syserror (fnamm,enamm : pchar);

var

dossegcnt : word;

implementation

{zamienia liczbe na string pchar}

procedure val2pchar (vall : longint; var p : pchar);
var
s : string [9];
begin
     str (vall,s);
     strpcopy (p,s);
end;

{zamienia liczbe w systemie szesnastkowym na string pchar}

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

procedure startstimer (var timer : stimer);
var
ptimer : ttimer;
begin
     ptimer := @timer;
     starttimer (ptimer);
end;

procedure stopstimer (var timer : stimer; typeoftimer : longint; var result : longint);
var
ptimer : ttimer;
begin
     ptimer := @timer;
     stoptimer (ptimer,typeoftimer,result);
end;

{pobiera parametry programu}

function getparams : word;
const
paramcnt = 3;
paramnames : array [0..paramcnt] of pchar = ('-safe','-nosound','-nomusic','-defaults');
var
i,j : byte;
s : string[128];
p : pchar;
a,b : word;
begin
     getparams := 0;
     a := 0;
     if paramcount = 0 then exit;
     getmem (p,128);
     for i := 1 to paramcount do
     begin
          s := paramstr (i);
          strpcopy (p,s);
          for j := 0 to paramcnt do
          begin
               if strcomp (p,paramnames[j])=0 then
               begin
                    b := 1 shl j;
                    a := a or b;
               end;
          end;
     end;
     freemem (p,128);
     getparams := a;
end;

function parameq (pa,flags : word) : boolean;
begin
     if pa and flags = flags then parameq := true
     else
     parameq := false;
end;

{szybie wypelnianie}

procedure _32fill (b : pointer; c : word; v : byte);
begin
asm
   les di,b
   mov al,v
   mov ah,v
   {shl eax,16} db 66h; shl ax,16
   mov al,v
   mov ah,v
   mov cx,c
   mov dx,cx
   and dx,3
   shr cx,2
   {rep stosd} rep; db 66h; stosw
   mov cx,dx
   rep stosb
end;
end;

procedure _32move (b1,b2 : pointer; c : word);
begin
asm
   push ds
   les di,b2
   lds si,b1
   mov cx,c
   mov dx,cx
   and dx,3
   shr cx,2
   {rep movsd} rep; db 66h; movsw
   mov cx,dx
   rep movsb
   pop ds
end;
end;

procedure int_idval (var ival : integer; idv : integer; llimit,hlimit : integer);
begin
asm
   les di,ival
   mov ax,idv
   add es:[di],ax
   mov ax,llimit
   cmp es:[di],ax
   jnl @d1
   mov es:[di],ax
   @d1:
   mov ax,hlimit
   cmp es:[di],ax
   jna @d2
   mov es:[di],ax
   @d2:
end;
end;

procedure word_idval (var ival : word; idv : integer; llimit,hlimit : word); {hlimit < 32768}
var
p : pchar;
begin
asm
   les di,ival
   mov ax,idv
   add es:[di],ax
   mov ax,llimit
   cmp es:[di],ax
   jnl @d1
   mov es:[di],ax
   @d1:
   mov ax,hlimit
   cmp es:[di],ax
   jng @d2
   mov es:[di],ax
   @d2:
end;
end;

{pobiera ilosc odsiezanych klatek na sekunde}

function getvsyncfreq : word;
var
i : byte;
p : ttimer;
v,vp : longint;
begin
     newtimer (p);
     starttimer (p);
     for i := 0 to 99 do wfvr;
     stoptimer (p,timer_in_milli_seconds,v);
     vp := v div 100;
     if vp <> 0 then
     getvsyncfreq := 1000 div vp
     else
     getvsyncfreq := 70;
     disposetimer (p);
end;

{funkcja pobierania pamieci}

function allocmem (s : word; align : word) : pointer;
begin
     allocmem := ptr (dossegcnt+align,0);
     if s mod 16 <> 0 then inc (align);
     inc (dossegcnt,(s shr 4)+align);
end;

procedure getstr (var p1,p2 : pchar; stri : word);
var
i,j : word;
begin
     i := stri;
     if p1[i] = '"' then
     begin
          j := 0;
          inc (i);
          while p1[i] <> '"' do
          begin
               p2[j] := p1[i];
               inc (i);
               inc (j);
          end;
          p2[j] := #0;
     end;
end;

procedure getcom (var p1,p2 : pchar; stri : word);
var
i,j : word;
begin
     i := stri;
     j := 0;
     while p1[i] <> #0 do
     begin
          p2[j] := p1[i];
          inc (i);
          inc (j);
     end;
     p2[j] := #0;
end;

procedure syserror (fnamm,enamm : pchar);
var
ccseg,ipr : word;
ppch,ph : pchar;
begin
     keyis (off);
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

end.