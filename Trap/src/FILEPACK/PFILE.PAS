{
  TRAP (r) Copyright (c) Kamil Korolkiewicz 2003
  Darmowa biblioteka do operowania spakowanymi plikami stworzona na potzeby gry TRAP (r)
  Wszelkie prawa autorskie zastrze¾one
}
{$S-,G+}
unit pfile;

interface

uses strings;

{deklaracje staˆych}

const

pfileioerrors : array [1..4] of pchar =
('File not found','Error while reading file','File can''t be closed','File is not reset');

{deklaracje typ¢w}

type

fileor = record
               pfpoz : longint;
               pfname : array [0..11] of char;
end;

pfilers = array [0..255] of fileor;

pfiler = ^pfilers;

{deklaracje zmiennych}

var

pfileior : byte;
pfus : boolean;
f : file;
nextfile : fileor;

{deklaracje procedur i funkcji}

function uspack (var zm : pfiler; nm : pchar) : boolean; {otwiera paka}
function unuspack (var zm : pfiler) : boolean; {zamyka paka}
procedure assignpfile (var upf : fileor; zm : pfiler; nm : pchar); {przypisuje zm. upf spakowany plik}
procedure resetpfile (upf : fileor); {otwiera spakowany plik}
procedure blockreadpfile (upf : fileor; var bf; count : word); {czyta spakowany plik}
procedure closepfile (upf : fileor); {zamyka spakowany plik}
function pfilepos (upf : fileor) : longint;
procedure seekpfile (upf : fileor; poz : longint);
function pfilesize (upf : fileor) : longint;
function pfileeof (upf : fileor) : boolean;
function getpfior : pchar; {pobiera i czy˜ci bˆedy}

{definicje procedur i funkcji}

implementation

function uspack;
const
charss : pchar = 'PF';
var
stri : array [0..1] of char;
pstr : array [0..11] of char;
i,j : byte;
begin
     uspack := false;
     assign (f,nm);
     {$I-}
     reset (f,1);
     {$I+}
     if ioresult <> 0 then exit;
     blockread (f,stri,2);
     for i := 0 to 1 do if stri[i] <> charss[i] then exit;
     blockread (f,j,1);
     getmem (zm,sizeof (fileor)*j);
     for i := 0 to strlen (nm)-1 do pstr [i] := nm[i];
     for i := strlen (nm) to 11 do pstr [i] := #0;
     zm^[0].pfpoz := j;
     move (pstr,zm^[0].pfname,12);
     for i := 1 to j do blockread (f,zm^[i],sizeof (fileor));
     reset (f,1);
     pfileior := 0;
     pfus := false;
     uspack := true;
end;

function unuspack;
begin
     unuspack := false;
     if zm = nil then exit;
     freemem (zm,zm^[0].pfpoz);
     close (f);
     pfileior := 0;
     unuspack := true;
end;

procedure assignpfile;
var
j : byte;
i : word;
fn : array [0..11] of char;
begin
     j := strlen (nm);
     for i := 1 to j do fn[i-1] := nm[i-1];
     for i := j+1 to 12 do fn[i-1] := #0;
     for i := 1 to zm^[0].pfpoz+1 do
     begin
          if i > zm^[0].pfpoz then
          begin
               pfileior := 1;
               exit;
          end;
          if zm^[i].pfname = fn then break;
     end;
     upf.pfpoz := zm^[i].pfpoz;
     move (zm^[i].pfname,upf.pfname,12);
     if i < zm^[0].pfpoz then
     nextfile.pfpoz := zm^[i+1].pfpoz
     else
     nextfile.pfpoz := filesize (f);
end;

procedure resetpfile;
begin
     seek (f,upf.pfpoz);
     pfus := true;
end;

procedure blockreadpfile;
begin
     if pfus = false then
     begin
          pfileior := 4;
          exit;
     end;
     if count > pfilesize (upf) then count := pfilesize (upf);
     {$I-}
     blockread (f,bf,count);
     {$I+}
     if ioresult <> 0 then
     begin
          seek (f,upf.pfpoz);
          pfileior := 2;
     end;
end;

procedure closepfile;
begin
     if pfus = false then
     begin
          pfileior := 3;
          exit;
     end;
     reset (f,1);
     nextfile.pfpoz := 0;
     fillchar (nextfile.pfname,12,0);
     upf.pfpoz := 0;
     fillchar (upf.pfname,12,0);
     pfus := false;
end;

function pfilepos;
begin
     if pfus = false then
     begin
          pfileior := 4;
          exit;
     end;
     pfilepos := filepos (f)-upf.pfpoz;
end;

procedure seekpfile;
begin
     if pfus = false then
     begin
          pfileior := 4;
          exit;
     end;
     seek (f,upf.pfpoz+poz);
end;

function pfilesize;
begin
     if pfus = false then
     begin
          pfileior := 4;
          exit;
     end;
     pfilesize := nextfile.pfpoz-upf.pfpoz;
end;

function pfileeof;
begin
     if filepos (f) >= nextfile.pfpoz then pfileeof := true
     else
     pfileeof := false;
end;

function getpfior;
begin
     getpfior := pfileioerrors [pfileior];
     pfileior := 0;
end;

end.