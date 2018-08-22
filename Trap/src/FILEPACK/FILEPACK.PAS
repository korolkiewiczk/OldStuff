{$M 4096,0,655350}
{$G+}
program filepack;
uses crt,dos;
const
chrsig : string [2] = 'PF';
filelistname = 'FILELIST';
var
i,k,poz,pom : longint;
j,l : byte;
s : array [0..255] of string[12];
s2 : array [0..11] of char;
pn : string[12];
f,fpo : file;
sign : array [0..1] of char;
txt : text;
dosver : word;
prc,m : word;
buf : pointer;
p : longint;

procedure checkproc (var proc,mode : word);
var
md : word;
begin
     proc := test8086;
     asm
        smsw ax
        mov md,ax
     end;
     if md = 16 then mode := 0
     else
     mode := 1;
end;

procedure checkparams;
begin
     if paramcount = 0 then
     begin
          writeln (#179+' Usage: FILEPACK.EXE [PACKNAME.XXX] (Default PACK.PAK)                       '+#179);
          pn := 'PACK.PAK';
          writeln ('-------------------------------------------------------------------------------');
     end
     else
     pn := paramstr (1);
end;

begin
     clrscr;
     writeln ('FILEPACK v 1.0 copyright (c) Kamil Korolkiewicz 2003/2004');
     writeln ('-------------------------------------------------------------------------------');
     checkparams;
     dosver := dosversion;
     checkproc (prc,m);
     writeln ('Dos version: ',lo (dosver),'.',hi (dosver));
     if m = 0 then
     begin
          case prc of
          0 : write ('8086 processor. ');
          1 : write ('286 processor. ');
          2 : write ('386 or higher processor. ');
          end;
          writeln ('Real mode addresing.');
     end
     else
     begin
          write ('386 or higher processor in V86 mode. ');
          if lo (dosver) >= 7 then writeln ('Windows detected')
          else
          writeln;
     end;
     writeln ('Lo memory available: ',maxavail div 1024,'Kb');
     writeln ('Creating file buffer...');
     getmem (buf,65535);
     writeln ('Lo memory available: ',maxavail div 1024,'Kb');
     writeln ('-------------------------------------------------------------------------------');
     write ('Opening '+filelistname+'...');
     assign (txt,filelistname);
     {$I-}
     reset (txt);
     {$I+}
     if ioresult <> 0 then
     begin
          writeln (' Error: '+filelistname+' not found. Program terminated.');
          halt (1);
     end
     else
     writeln;
     writeln ('Reading '+filelistname+'...');
     j := 0;
     while not eof (txt) do
     begin
          readln (txt,s[j]);
          inc (j);
     end;
     writeln ('Closing '+filelistname+'...');
     close (txt);
     writeln ('-------------------------------------------------------------------------------');
     writeln ('Opening '+pn+'...');
     assign (f,pn);
     rewrite (f,1);
     writeln ('Writing header to '+pn+'...');
     for l := 0 to 1 do sign [l] := chrsig[l+1];
     blockwrite (f,sign,2);
     blockwrite (f,j,1);
     poz := j*16+3;
     for i := 0 to j-1 do
     begin
          assign (fpo,s[i]);
          {$I-}
          reset (fpo,1);
          {$I+}
          if ioresult <> 0 then
          begin
               close (f);
               writeln ('Error: File '+s[i]+' not found. Program must be terminated');
               halt (1);
          end;
          k := filesize (fpo);
          blockwrite (f,poz,4);
          for l := 1 to length (s[i]) do s2 [l-1] := s[i][l];
          for l := length (s[i])+1 to 12 do s2[l-1] := #0;
          blockwrite (f,s2,12);
          inc (poz,k);
          close (fpo);
     end;
     writeln ('Writing files to '+pn+'...');
     for i := 0 to j-1 do
     begin
          assign (fpo,s[i]);
          reset (fpo,1);
          while not eof (fpo) do
          begin
               p := filesize(fpo)-filepos(fpo);
               if p < 65535 then
               begin
                    blockread (fpo,buf^,p);
                    blockwrite (f,buf^,p);
               end
               else
               begin
                    blockread (fpo,buf^,65535);
                    blockwrite (f,buf^,65535);
               end;
          end;
          close (fpo);
     end;
     writeln ('Closing '+pn+'...');
     close (f);
end.