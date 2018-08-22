const
fnamm = 'note3';
var
f : file;
t : text;
buf : array [0..10000] of byte;
s1,s2,s3 : word;
i : word;
sp : string[3];
begin
     assign (f,fnamm);
     reset (f,1);
     blockread (f,s1,4);
     s3 := s1*s2;
     blockread (f,buf,s3);
     close (f);
     assign (t,fnamm+'.pas');
     rewrite (t);
     for i := 1 to s3 do
     begin
          s2 := buf [i-1];
          if s2 <> 0 then s2 := 96;
          str (s2,sp);
          write (t,sp+',');
          if (i mod s1 = 0) and (i <> 0) then writeln (t);
     end;
     close (t);
end.