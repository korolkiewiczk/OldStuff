uses trapcrt,key;
var
s,i : byte;
c : char;
t : byte;
begin
     clrscr;
     readln (t);
     keyis (on);
     if t=0 then
     begin
     stdkeyint := true;
     repeat
           if readkey (c,s) then
           writeln (s,' #',ord (c),' ',c,' ',biosextkeys1,' ',biosextkeys2);
           if biosextkeys1 and CTRL_PRESSED = CTRL_PRESSED then writeln ('CTRL_PRESSED');
           if biosextkeys1 and ALT_PRESSED = ALT_PRESSED then writeln ('ALT_PRESSED');
     until s=1;
     end
     else
     begin
     stdkeyint := false;
     pressnumlock;
     repeat
           for s := 0 to 127 do if keyon[s]=1 then begin i:=s; writeln (s,' ',numlockp,' ',shiftkp); end;
           if numlockp = 1 then pressnumlock;
     until i=1;
     end;
     keyis (off);
end.