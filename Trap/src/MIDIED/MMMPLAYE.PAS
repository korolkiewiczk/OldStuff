uses midadd,snd,crt,scr;
var
f : file;
s : word;
b : array [0..63000] of byte;
c : char;
scan : byte;
fn : string;
begin
     getmem (sndbuf,bufsize);
     if paramcount = 1 then fn := paramstr (1)
     else
     readln (fn);
     clrscr;
     assign (f,fn);
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
     initmus ($330,@b,0,s);
     repeat
           getkey (c,scan);
           wfvr;
           gotoxy (1,2);
           write ('pozycja:',pz,'     czas:',pz div 16,'s');
     until (c = 'p') or (c = 'P');
     closemus;
     closesnd;
     close (f);
end.