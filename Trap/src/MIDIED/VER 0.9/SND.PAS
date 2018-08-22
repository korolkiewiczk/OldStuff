{
  TRAP (r) Copyright (c) Kamil Korolkiewicz 2003
  Darmowa biblioteka do odtwarzania d«wiek¢w stworzona na potzeby gry TRAP (r)
  Wszelkie prawa autorskie zastrze¾one
}

{$S-}
unit snd;

interface

{doˆ¥czone unity}

uses crt,dos;

{deklaracje staˆych}

const

freq = 8000;
tconst = 256 - (1000000 div freq);
bufsize = freq div 8; {0.125 s.}
nchannels = 4;
dmatab1 : array [0..11] of byte = ($0,$1,$87,$2,$3,$83,$4,$5,$81,$6,$7,$82);
irqs : array [0..4] of byte = ($f7,$ef,$df,$bf,$7f); {3..7}
snddis : boolean = false;
musicplayed : boolean = false;

{deklarcje typ¢w}

type

tsample = record {pojedyäczy sampel}
                size_of_sample : word;
                sme_bf : pointer; {bufor sampla}
                no : byte;
end;

tsb_rec = record {record w kt¢rym s¥ przechowywane informacje o karcie sndb}
                port_2x0 : word; {np. 220}
                port_2x6 : word; {np. 226}
                port_2xa : word; {np. 22a}
                port_2xc : word; {np. 22c}
                port_2xe : word; {np. 22e}
                midi_port : word; {np. 330}
                dma : byte; {dma}
                irq : byte; {irq}
                sb_ver : word; {wersja karty sb}
end;

tchannel = record
                 size,poz : word;
                 gl : byte;
                 used : boolean;
                 sndmem : pointer;
                 no : byte;
end;

tmidimusic = record
               msize : word;
               mpoz : word;
               mbuf : pointer;
end;

{deklaracje zmiennych}

var

sbinfo : tsb_rec;
sndbuf : pointer; {bufor}
oldsndint : pointer;
prs : byte;
oldirqp : byte;
oldsbirq : pointer;
channels : array [1..nchannels] of tchannel;
bpg : byte;
midimusic : tmidimusic;
pz : word;

{deklaracje procedur i funkcji}

procedure outsb (b : byte);
procedure insb (var b : byte);
function  sbreset (p : word) : boolean;
function  initsnd (portmain : word; ndma,nirq : byte) : boolean;
procedure closesnd;
procedure initmus (midiport : word; pmus : pointer; pmsize : word); {mp-midi port,pmus-wskaznik do scieszki muz.}
procedure closemus;
procedure addsample (smpl : tsample; gl : byte); {w gl glosnosc}
procedure speakeron;
procedure speakeroff;
procedure stopoutput;
procedure getsbver (var ver : word);
procedure getsbenv (var p : word; var d,i : byte; var m : word); {port sb,dma,irq,midi}

implementation

{definicje procedur i funkcji}

procedure outsb;
begin
asm
   mov dx,sbinfo.port_2xc
   @a:
     in al,dx
     test al,128
     jnz @a
   mov al,b
   out dx,al
end;
end;

procedure insb;
begin
asm
   mov dx,sbinfo.port_2xe
   @a:
      in ax,dx
      test al,128
      jz @a
   mov dx,sbinfo.port_2xa
   in al,dx
   les di,b
   mov es:[di],al
end;
end;

function sbreset;
begin
     asm
        mov dx,p
        add dx,6
        mov al,1
        out dx,al
        mov cx,100 {opuznienie}
        @a:
        in al,dx
        loop @a
        mov dx,p
        add dx,6
        mov al,0
        out dx,al
        mov cx,100 {opuznienie}
        @a2:
        in al,dx
        loop @a2
        mov dx,p
        add dx,0eh
        in al,dx
        test al,128
        jz @q
        mov dx,p
        add dx,0ah
        in al,dx
        cmp al,0aah
        jnz @q
        mov @result,1
        jmp @q2
        @q:
        mov @result,0
        @q2:
     end;
end;

procedure useirq; assembler;
asm
   in al,21h
   mov oldirqp,al
   mov bl,sbinfo.irq
   sub bl,3
   xor bh,bh
   add bx,offset irqs
   and al,[bx]
   out 21h,al
end;

procedure unuseirq; assembler;
asm
   mov al,oldirqp
   out 21h,al
end;

procedure sbirq; interrupt; assembler;
asm
   {sekcja dla odtwarzania muzuki midi}
   cmp musicplayed,0
   je @me1
   les di,midimusic.mbuf
   mov bx,di
   add di,midimusic.mpoz
   @mp:
       mov al,es:[di]
       inc di
       mov dx,di
       sub dx,bx
       cmp dx,midimusic.msize
       jbe @md1
       mov musicplayed,0
       mov di,bx
       jmp @me1
       @md1:
       cmp al,0ffh
       je @me2
       mov dx,sbinfo.midi_port
       out dx,al
       jmp @mp
   @me2:
   sub di,bx
   mov midimusic.mpoz,di
   inc pz
   @me1:
   mov dx,sbinfo.port_2xe
   in al,dx
   mov al,20h
   out 20h,al
end;

function initsnd;
var
ad : word;
pg : byte;
i : byte;
begin
     initsnd := false;
     {sprawdzenie obecno˜ci sb}
     if not sbreset (portmain) then exit;
     {inicjacja tablic i zmiennych}
     for i := 1 to nchannels do channels [i].used := false;
     sbinfo.port_2x0 := portmain;
     sbinfo.port_2x6 := portmain+$6;
     sbinfo.port_2xa := portmain+$a;
     sbinfo.port_2xc := portmain+$c;
     sbinfo.port_2xe := portmain+$e;
     sbinfo.dma := ndma;
     sbinfo.irq := nirq;
     getsbver (sbinfo.sb_ver);
     bpg := 0;
     fillchar (sndbuf^,bufsize,128);
     {inicjacja przerwania (irq) sb}
     i := nirq+8;
     getintvec (i,oldsbirq);
     setintvec (i,@sbirq);
     useirq;
     {inicjacja dma}
     port [$0a] := 4+ndma;
     port [$0b] := $58+ndma;
     ad := (seg (sndbuf^) shl 4)+ofs (sndbuf^);
     pg := ((seg (sndbuf^))+ofs (sndbuf^) shr 4) shr 12;
     port [dmatab1 [ndma*3+2]] := pg;
     port [$0c] := 0;
     port [dmatab1 [ndma*3]] := lo (ad);
     port [dmatab1 [ndma*3]] := hi (ad);
     port [$0c] := 0;
     port [dmatab1 [ndma*3+1]] := lo (bufsize-1);
     port [dmatab1 [ndma*3+1]] := hi (bufsize-1);
     port [$0a] := ndma;
     {inicjacja sb}
     outsb ($40);
     outsb (tconst);
     outsb ($c6);
     outsb (0);
     outsb (lo (bufsize div 2-1));
     outsb (hi (bufsize div 2-1));
     speakeron;
     initsnd := true;
     snddis := true;
end;

procedure closesnd;
begin
     stopoutput;
     speakeroff;
     unuseirq;
     setintvec (sbinfo.irq+8,oldsbirq);
     snddis := false;
     musicplayed := false; {muzyka midi jest wylaczona}
end;

procedure initmus;
begin
     sbinfo.midi_port := midiport;
     midimusic.msize := pmsize;
     midimusic.mbuf := pmus;
     midimusic.mpoz := 0;
     pz := 0;
     asm {inicjacja syntezatora mpu-401}
        mov dx,sbinfo.midi_port
        inc dx
        mov al,3fh
        out dx,al
     end;
     musicplayed := true;
end;

procedure closemus;
begin
     midimusic.msize := 0;
     midimusic.mbuf := nil;
     midimusic.mpoz := 0;
     musicplayed := false;
     asm {deinicjacja syntezatora mpu-401}
        mov dx,sbinfo.midi_port
        inc dx
        mov al,0ffh
        out dx,al
     end;
end;

procedure addsample;
var
i : byte;
begin
     if smpl.no <> 0 then
     for i := 1 to nchannels do if channels [i].no = smpl.no then
     begin
          channels [i].gl := gl;
          exit;
     end;
     for i := 1 to nchannels do
     begin
          if not channels [i].used then
          begin
               channels [i].size := smpl.size_of_sample;
               channels [i].poz := 0;
               channels [i].gl := gl;
               channels [i].used := true;
               channels [i].sndmem := smpl.sme_bf;
               channels [i].no := smpl.no;
               exit;
          end;
     end;
end;

procedure speakeron;
begin
     outsb ($d1);
end;

procedure speakeroff;
begin
     outsb ($d3);
end;

procedure stopoutput;
begin
     outsb ($da);
end;

procedure getsbver;
var
l,h : byte;
begin
     outsb ($e1);
     insb (l);
     insb (h);
     ver := h*256+l;
end;

procedure getsbenv;
var
s : string [23];
s1,s4 : string [3];
s2,s3 : string [1];
a : integer;
begin
     s := getenv ('BLASTER');
     if s = '' then exit;
     s1 := copy (s,2,3);
     s2 := copy (s,7,1);
     s3 := copy (s,10,1);
     s4 := copy (s,16,3);
     val ('$'+s1,p,a);
     val ('$'+s2,i,a);
     val ('$'+s3,d,a);
     val ('$'+s4,m,a);
end;

end.