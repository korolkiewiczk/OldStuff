function tohex(a:byte):string;
const
xltab:array[0..15] of char = ('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F');
var
s:string[2];
begin
     s[0]:=#2;
     if a<16 then begin s[1]:='0'; s[2]:=xltab[a and 15]; end
     else
     begin
          s[1]:=xltab[a shr 4];
          s[2]:=xltab[a and 15];
     end;
     tohex:=s;
end;
var
i:byte;
begin
     for i:=0 to 255 do writeln (' /*',i,'(0x',tohex(i),')*/',' {0,0,0,0,0},');
end.