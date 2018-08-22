const
tabf:array[0..7] of string[3]=('ROL','ROR','RCL','RCR','SHL','SHR','0','SAR');
tabn:array[0..7] of string[5]=('"rol"','"ror"','"rcl"','"rcr"','"shl"','"shr"','0','"sar"');

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
t1,p1:longint;
begin
     p1:=rm and 7;
     if rm<$40 then t1:=16
     else
     if rm<$80 then t1:=17
     else
     if rm<$C0 then t1:=18
     else
     t1:=0;
     rm2p:=t1+p1 shl 8;
end;



var
i:byte;
begin
     for i:=0 to 255 do
     begin
          writeln(' /*',i,'(0x',tohex(i),')*/ {',tabn[(i shr 3) and 7],',0,',rm2p(i),',',tabf[(i shr 3) and 7],',0},');
     end;
end.