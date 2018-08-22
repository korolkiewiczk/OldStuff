uses key;
var
i : byte;
begin
     keyis (on);
     while keyon[1] = 0 do
     for i := 0 to 127 do if keyon [i] = 1 then writeln (i);
     keyis (off);
end.