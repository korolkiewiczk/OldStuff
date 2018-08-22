uses midadd,snd;
var
f : file;
s : word;
b : array [0..63000] of byte;
c : char;
scan : byte;
begin
     getmem (sndbuf,bufsize);
     if paramcount <> 1 then exit;
     assign (f,paramstr (1));
     {$I-}
     reset (f,1);
     {$I+}
     if ioresult <> 0 then exit;
     blockread (f,s,2);
     if s <> filesize (f)-2 then begin writeln ('b'); close (f); exit; end;
     if s > 64000 then begin writeln ('c'); close (f); exit; end;
     blockread (f,b,s);
     writeln ('P aby przerwa†');
     initsnd ($220,1,7);
     initmus ($330,@b,s);
     repeat
           getkey (c,scan);
     until (c = 'p') or (c = 'P');
     closemus;
     closesnd;
     close (f);
end.