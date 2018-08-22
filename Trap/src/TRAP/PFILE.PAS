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
('File ','Error while reading file','File can''t be closed','File is not reset');

{deklaracje typ¢w}

type

fileor = record
               pfpos : longint;
               pfname : array [0..11] of char;
end;

pfilers = array [0..255] of fileor;

pfiler = ^pfilers;

{deklaracje zmiennych}

var

pfileior : byte;
pfileiorn : array [0..11] of char;
pfileiorpch : array [0..28] of char;
pfus : boolean;
pf : file;
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
     assign (pf,nm);
     {$I-}
     reset (pf,1);
     {$I+}
     if ioresult <> 0 then exit;
     blockread (pf,stri,2);
     for i := 0 to 1 do if stri[i] <> charss[i] then exit;
     blockread (pf,j,1);
     getmem (zm,sizeof (fileor)*j+sizeof (fileor));
     for i := 0 to strlen (nm)-1 do pstr [i] := nm[i];
     for i := strlen (nm) to 11 do pstr [i] := #0;
     zm^[0].pfpos := j;
     move (pstr,zm^[0].pfname,12);
     for i := 1 to j do blockread (pf,zm^[i],sizeof (fileor));
     reset (pf,1);
     pfileior := 0;
     pfus := false;
     uspack := true;
end;

function unuspack;
begin
     unuspack := false;
     if zm = nil then exit;
     freemem (zm,zm^[0].pfpos);
     zm := nil;
     close (pf);
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
     if j > 12 then j := 12;
     for i := 0 to j-1 do fn[i] := nm[i];
     for i := j to 11 do fn[i] := #0;
     for i := 1 to zm^[0].pfpos+1 do
     begin
          if i > zm^[0].pfpos then
          begin
               pfileior := 1;
               strcopy (pfileiorn,nm);
               exit;
          end;
          if zm^[i].pfname = fn then break;
     end;
     upf.pfpos := zm^[i].pfpos;
     move (zm^[i].pfname,upf.pfname,12);
     if i < zm^[0].pfpos then
     nextfile.pfpos := zm^[i+1].pfpos
     else
     nextfile.pfpos := filesize (pf);
     pfileior := 0;
end;

procedure resetpfile;
begin
     seek (pf,upf.pfpos);
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
     blockread (pf,bf,count);
     {$I+}
     if ioresult <> 0 then
     begin
          seek (pf,upf.pfpos);
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
     reset (pf,1);
     nextfile.pfpos := 0;
     fillchar (nextfile.pfname,12,0);
     upf.pfpos := 0;
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
     pfilepos := filepos (pf)-upf.pfpos;
end;

procedure seekpfile;
begin
     if pfus = false then
     begin
          pfileior := 4;
          exit;
     end;
     seek (pf,upf.pfpos+poz);
end;

function pfilesize;
begin
     if pfus = false then
     begin
          pfileior := 4;
          exit;
     end;
     pfilesize := nextfile.pfpos-upf.pfpos;
end;

function pfileeof;
begin
     if filepos (pf) >= nextfile.pfpos then pfileeof := true
     else
     pfileeof := false;
end;

function getpfior;
begin
     if pfileior = 1 then
     begin
          strcopy (pfileiorpch,pfileioerrors [pfileior]);
          strcat (pfileiorpch,pfileiorn);
          strcat (pfileiorpch,' not found');
          getpfior := pfileiorpch;
     end
     else
     getpfior := pfileioerrors [pfileior];
     pfileior := 0;
end;

end.