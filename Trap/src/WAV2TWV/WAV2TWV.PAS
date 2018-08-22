{$I-}
const
rozsz : string [3] = 'TWV';
bufsize = 8000 div 8;
var
f : file;
fnamm,fnamm2 : string;
buf : pointer;
s : longint;
i,j : byte;
c : char;
wave : string[4];
begin
     if paramcount <> 1 then readln (fnamm) else
     fnamm := paramstr (1);
     getmem (buf,65535);
     fillchar (buf^,65535,128);
     assign (f,fnamm);
     reset (f,1);
     if ioresult <> 0 then halt (1);
     seek (f,$8);
     for i := 1 to 4 do blockread (f,wave[i],1);
     wave[0] := #4;
     if wave <> 'WAVE' then begin writeln ('No wave-type file'); close (f); halt (1) end;
     seek (f,$36);
     blockread (f,s,4);
     if s > 65535 then begin writeln ('s=',s,' > 65535'); close (f); halt (1); end;
     seek (f,$3a);
     blockread (f,buf^,s);
     if ioresult <> 0 then begin writeln ('Error while reading file'); close (f); halt (1); end;
     close (f);
     i := 1;
     c := fnamm[1];
     while (c <> '.') and (i <= length (fnamm)) do
     begin
          c := fnamm[i];
          fnamm2[i] := c;
          inc (i);
     end;
     for j := i to i+2 do fnamm2 [j] := rozsz[j-i+1];
     fnamm2[0] := chr (length (fnamm));
     assign (f,fnamm2);
     rewrite (f,1);
     if s mod (bufsize div 2) <> 0 then
     s := ((s div (bufsize div 2))+1)*(bufsize div 2);
     blockwrite (f,s,2);
     blockwrite (f,buf^,s);
     close (f);
     writeln ('s=',s);
     writeln ('name=',fnamm2);
end.