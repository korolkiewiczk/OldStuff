uses trapcrt;
var
s,i : byte;
c : char;
t : byte;
begin
     clrscr;
     repeat
           if readkey (c,s) then
           writeln (s,' #',ord (c),' ',c);
     until s=1;
end.