var
i:longint;
f:file of longint;
begin
     assign (f,'t.com');
     rewrite(f);
     for i:=0 to 65536*32 do write(f,i);
     close(f);
end.