const
tabf:array[0..7] of string[6]=('FLD','0','FST','FSTP','FRSTOR','0','FSAVE','FSTSW');
tabn:array[0..7] of string[8]=('"fld"','0','"fst"','"fstp"','"frstor"','0','"fsave"','"fstsw"');

tabf2:array[0..7] of string[6]=('FFREE','0','FST','FSTP','FUCOM','FUCOMP','0','0');
tabn2:array[0..7] of string[8]=('"ffree"','0','"fst"','"fstp"','"fucom"','"fucomp"','0','0');

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
t1,p1,t2,p2:longint;
begin
     p1:=rm and 7;
     t2:=0;
     p2:=0;
     if rm<$40 then t1:=16
     else
     if rm<$80 then t1:=17
     else
     if rm<$C0 then t1:=18
     else
     begin
          t1:=2;
     end;
     rm2p:=t1+p1 shl 8+t2 shl 16+p2 shl 24;
end;



var
i:byte;
begin
     for i:=0 to 255 do
     begin
          if i<$c0 then
          begin
               if (i shr 3) and 7 <4 then
               writeln(' /*',i,'(0x',tohex(i),')*/ {',
               tabn[(i shr 3) and 7],',F_PAR+F_QW+F_D,',rm2p(i),',',tabf[(i shr 3) and 7],',0},')
               else
               writeln(' /*',i,'(0x',tohex(i),')*/ {',
               tabn[(i shr 3) and 7],',F_PAR,',rm2p(i),',',tabf[(i shr 3) and 7],',0},')
          end
          else
          writeln(' /*',i,'(0x',tohex(i),')*/ {',
          tabn2[(i shr 3) and 7],',F_PAR,',rm2p(i),',',tabf2[(i shr 3) and 7],',0},')
     end;
end.