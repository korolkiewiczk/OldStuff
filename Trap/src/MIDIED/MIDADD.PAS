unit midadd;
interface
uses scr,strings,snd;

type

tabt = array [0..65534] of byte;
tabp = ^tabt;

var
np : boolean;
a,b,ch : byte;
li : word;

function getkey (var c : char; var s : byte) : boolean;
procedure win3dnb (x1,y1,x2,y2 : integer; c,a : byte);
procedure win3db (x1,y1,x2,y2 : integer; c,a : byte);
procedure strpchar (i : longint; var p : pchar);
function blo (b : byte) : byte; {dla $95 zwraca 5}
function shfp : boolean;
function rsqr (a : byte) : word; {podnosi liczbe 2 do a potegi}
procedure resetmidi (p : word);
procedure stopmidi (p : word);
procedure playnote (c,i,n,g1,g2,lr : byte);
procedure stopnote;
function pb2m (a : word) : word;

implementation

function getkey (var c : char; var s : byte)  : boolean;
begin
asm
   mov ax,0100h
   int 16h
   jz @d1
   xor ax,ax
   int 16h
   les di,c
   mov es:[di],al
   les di,s
   mov es:[di],ah
   mov al,1
   jmp @d2
   @d1:
   xor al,al
   @d2:
   mov [bp-1],al
end;
end;

procedure win3dnb;
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

procedure win3db;
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

procedure strpchar;
var
ps : string [11];
begin
     str (i,ps);
     strpcopy (p,ps);
end;

function blo (b : byte) : byte;
begin
     blo := b and $f;
end;

function shfp : boolean; assembler;
asm
   mov ax,40h
   mov es,ax
   mov di,17h
   mov al,es:[di]
   test al,2
   jz @d
   mov al,1
   ret
   @d:
   test al,1
   jz @d2
   mov al,1
   ret
   @d2:
   mov al,0
end;

function rsqr (a : byte) : word; assembler;
asm
   mov ax,2
   mov bx,2
   mov cl,a
   xor ch,ch
   or cl,cl
   jz @q1
   dec cl
   jz @q2
   @a:
      mul bx
   loop @a
   jmp @q3
   @q1:
   mov ax,1
   jmp @q3
   @q2:
   mov ax,2
   @q3:
end;

procedure resetmidi (p : word);
begin
     sbinfo.midi_port := p;
     port [p+1] := $3f;
end;

procedure stopmidi (p : word);
var
i : byte;
begin
     musicplayed := false;
     for i := 0 to 15 do
     begin
          port [p] := $b0+i;
          port [p] := $7b;
          port [p] := 1;
     end;
end;

procedure playnote (c,i,n,g1,g2,lr : byte);
begin
     if musicplayed = false then
     begin
          port [sbinfo.midi_port] := $c0+c;
          port [sbinfo.midi_port] := i;
          port [sbinfo.midi_port] := $b0+c;
          port [sbinfo.midi_port] := $7;
          port [sbinfo.midi_port] := g1;
          port [sbinfo.midi_port] := $b0+c;
          port [sbinfo.midi_port] := $a;
          port [sbinfo.midi_port] := lr;
          port [sbinfo.midi_port] := $90+c;
          port [sbinfo.midi_port] := n;
          port [sbinfo.midi_port] := g2;
          a := n;
          b := g2;
          ch := c;
          np := true;
          li := 0;
     end;
end;

procedure stopnote;
begin
     if (musicplayed = false) and (np = true) then
     begin
          port [sbinfo.midi_port] := $80+ch;
          port [sbinfo.midi_port] := a;
          port [sbinfo.midi_port] := b;
     end;
end;

function pb2m (a : word) : word;
begin
     pb2m := (a and $7f)+256*(a shr 7);
end;

end.