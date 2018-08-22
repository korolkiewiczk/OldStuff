program mapmodv;
uses crt,strings;
{$I TRAPMAP.PAS}
const
maploaded : boolean=false;
var
trapmap : trapmapdesc;
asl : word;
ntrapinfo : byte;
midimusicname,trapmapname : pchar;

procedure resettrapmap;
var
i : word;
p : pointer;
begin
     with trapmap do
     begin
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
     fillchar (trapmapname^,256,0);
     fillchar (midimusicname^,256,0);
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
     getmem (trapmapname,256);
     getmem (midimusicname,256);
     resettrapmap;
end;


function loadmap (var mapname : pchar) : boolean;
var
trapmaphead : pchar;
i : word;
c : char;
t : pointer;
f : file;
begin
     maploaded := false;
     loadmap := false;
     if strcomp (mapname,'')=0 then exit;
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
          if ntrapinfo > maxinforecs then halt;
          t := ptr (trapinforecseg,0);
          blockread (f,t^,ntrapinfo*sizeof (trapinforec));
          blockread (f,i,2);
          if i > scryptslen then halt;
          t := ptr (trapscryptseg,0);
          blockread (f,t^,i);
          asl := i;
     end;
     close (f);
     maploaded := true;
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
end;

var
ccell : byte;
cbyte : byte;
obyte : byte;
c : char;
fnamm : pchar;

procedure getccell;
var
pv : longint;
begin
     ccell := 255;
     while (ccell > 5) do
     begin
     clrscr;
     write ('Podaj nr kom¢rki (1-6):');
     readln (pv);
     ccell := pv-1;
     end;
end;

procedure getcbyte;
var
pv : longint;
begin
     cbyte := 255;
     pv := 256;
     while (pv > 255) do
     begin
     clrscr;
     write ('Podaj bajt do zamiany (0-255):');
     readln (pv);
     cbyte := pv;
     end;
end;

procedure getobyte;
var
pv : longint;
begin
     obyte := 255;
     pv := 256;
     while (pv > 255) do
     begin
     clrscr;
     write ('Podaj now¥ warto˜† bajtu (0-255):');
     readln (pv);
     obyte := pv;
     end;
end;

procedure getfnamm;
var
pf : string;
begin
     clrscr;
     write ('Podaj nazw© mapy:');
     readln (pf);
     strpcopy (fnamm,pf);
end;

procedure processmap;
type
st = array [0..65534] of byte;
pt = ^st;
var
i,s : word;
b : byte;
t : pt;
begin
     t := pointer (trapmap.trapmap);
     clrscr;
     writeln ('Przetwa¾am map©...');
     s := trapmap.trapmapsizex*trapmap.trapmapsizey-1;
     for i := 0 to s do
     begin
          b := t^[i*6+ccell];
          if b = cbyte then t^[i*6+ccell] := obyte;
     end;
end;

procedure copyparam;
var
ps : string;
begin
     ps := paramstr (1);
     strpcopy (fnamm,ps);
end;

begin
     getmem (fnamm,256);
     fillchar (fnamm^,256,0);
     inittrapmap;
     if paramcount > 0 then copyparam;
     while 1=1 do
     begin
     clrscr;
     writeln ('MAPMODV copyright (c) Kamil Korolkiewicz 2004');
     writeln ('Program sˆu¾y do zamiany odpowiednich kom¢rek w mapie.');
     writeln ('1-Podaj nr kom¢rki do zamiany (Aktualna kom¢rka:',ccell+1,')');
     writeln ('2-Podaj bajt do zamiany (Aktualny bajt:',cbyte,')');
     writeln ('3-Podaj now¥ warto˜† bajtu (Aktualny bajt:',obyte,')');
     writeln ('4-Podaj nazw© mapy (Aktualna nazwa:',fnamm,')');
     writeln ('5-Otw¢rz plik mapy');
     writeln ('6-Zamieä w mapie');
     writeln ('7-Zapisz map©');
     writeln ('8-Wyjd« z programu');
     if maploaded then
     writeln ('Mapa wczytana')
     else
     writeln ('Mapa nie wczytana');
     c := readkey;
     case c of
     '1':getccell;
     '2':getcbyte;
     '3':getobyte;
     '4':getfnamm;
     '5':loadmap (fnamm);
     '6':if maploaded then processmap;
     '7':if maploaded then savemap (fnamm);
     '8':break;
     end;
     end;
end.