{$I-}
uses snd,crt;
var
f : file;
s : tsample;
fn : string;
c : char;
g : byte;
begin
     if paramcount <> 1 then readln (fn)
     else
     fn := paramstr (1);
     clrscr;
     assign (f,fn);
     reset (f,1);
     if ioresult <> 0 then exit;
     blockread (f,s.size_of_sample,2);
     if s.size_of_sample <> filesize (f)-2 then
     begin
          close (f);
          exit;
     end;
     s.no := 0;
     getmem (sndbuf,bufsize);
     getmem (s.sme_bf,s.size_of_sample);
     blockread (f,s.sme_bf^,s.size_of_sample);
     initsnd ($220,1,7);
     writeln ('q-wyj˜cie p-odegraj s-gˆo˜niej a-ciszej');
     g := 128;
     repeat
           c := readkey;
           if (c = 'p') or (c = 'P') then addsample (s,g,0);
           if (c = 'a') or (c = 'A') then dec (g);
           if (c = 's') or (c = 'S') then inc (g);
           gotoxy (1,2);
           write ('   ');
           gotoxy (1,2);
           write (g);
     until (c = 'q') or (c = 'Q');
     closesnd;
end.