{$define g01}
const
{$ifndef g01}
tabf:array[0..7] of string[4]=('SLDT','STR','LLDT','LTR','VERR','VERW','0','0');
tabn:array[0..7] of string[6]=('"sldt"','"str"','"lldt"','"ltr"','"verr"','"verw"','0','0');
{$else}
tabf:array[0..7] of string[6]=('SGDT','SIDT','LGDT','LIDT','SMSW','0','LMSW','INVLPG');
tabn:array[0..7] of string[8]=('"sgdt"','"sidt"','"lgdt"','"lidt"','"smsw"','0','"lmsw"','"invlpg"');
{$endif}

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
          {$ifdef g01}
          if ((i shr 3) and 7)<4 then
          writeln(' /*',i,'(0x',tohex(i),')*/ {',tabn[(i shr 3) and 7],',F_SW,',rm2p(i),',',tabf[(i shr 3) and 7],',0},')
          else
          {$endif}
          writeln(' /*',i,'(0x',tohex(i),')*/ {',tabn[(i shr 3) and 7],',F_W,',rm2p(i),',',tabf[(i shr 3) and 7],',0},');
     end;
end.