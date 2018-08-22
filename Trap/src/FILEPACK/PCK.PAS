program pck;
uses crt;
const
chrsig : string [2] = 'PF';
var
i,k,poz,pom : longint;
j,l : byte;
s : array [0..255] of string[12];
s2 : array [0..11] of char;
pn : string[12];
f,fpo : file;
sign : array [0..1] of char;
begin
     clrscr;
     writeln ('PCK copyright (c) Kamil Korolkiewicz 2003');
     for j := 0 to 1 do sign [j] := chrsig[j+1];
     write ('Podaj liczbe plikow:'); readln (j);
     writeln ('Podaj nazwy plikow');
     for i := 0 to j-1 do
     begin
          readln (s[i]);
          assign (f,s[i]);
          {$I-}
          reset (f);
          {$I+}
          if ioresult <> 0 then
          begin
               writeln ('Plik nie odnaleziony!!');
               exit;
          end;
          close (f);
     end;
     write ('Podaj nazwe paka:'); readln (pn);
     writeln ('-----------------------------------------');
     write ('Pakuje pliki...');
     assign (f,pn);
     rewrite (f,1);
     blockwrite (f,sign,2);
     blockwrite (f,j,1);
     poz := j*16+3;
     for i := 0 to j-1 do
     begin
          assign (fpo,s[i]);
          reset (fpo,1);
          k := filesize (fpo);
          blockwrite (f,poz,4);
          for l := 1 to length (s[i]) do s2 [l-1] := s[i][l];
          for l := length (s[i])+1 to 12 do s2[l-1] := #0;
          blockwrite (f,s2,12);
          inc (poz,k);
          close (fpo);
          write ('.');
     end;
     for i := 0 to j-1 do
     begin
          assign (fpo,s[i]);
          reset (fpo,1);
          while not eof (fpo) do
          begin
               blockread (fpo,pom,1);
               blockwrite (f,pom,1);
          end;
          close (fpo);
          write ('.');
     end;
     close (f);
end.