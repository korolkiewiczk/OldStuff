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

function rm2p(rm:byte):longint;
var
t1,t2:longint;
p1,p2:longint;
begin
     p1:=rm and 7;
     if rm<$40 then t1:=16
     else
     if rm<$80 then t1:=17
     else
     if rm<$C0 then t1:=18
     else
     t1:=0;
     t2:=0;
     p2:=(rm shr 3) and 7;
     rm2p:=t1+p1 shl 8+t2 shl 16+p2 shl 24;
end;

var
i:byte;
begin
     for i:=0 to 255 do
     begin
          writeln(' /*',i,'(0x',tohex(i),')*/ {0,F_PAR,',rm2p(i),',0,0},');
     end;
end.