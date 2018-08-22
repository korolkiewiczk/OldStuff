uses midadd;
var
i : byte;
c : char;
begin
     while c <> #27 do
     begin
          if getkey (c,i) then writeln (i);
     end;
end.