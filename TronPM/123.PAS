uses crt;
var
tab : array [0..63,0..63] of byte;
f : file;
i,j,k : byte;
begin
     clrscr;
     assign (f,'map1.map');
     reset (f,1);
     blockread (f,tab,4096);
     for i := 0 to 63 do
     begin
         for j := 0 to 63 do
         begin
              if (j = 25) and (i = 59) then
              begin
                   textbackground (2);
                   write (tab[i,j]);
                   textbackground (0);
              end
              else
              write (tab[i,j]);
              end;
         writeln;
     end;
     close (f);
end.