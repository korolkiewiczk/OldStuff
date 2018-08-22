const
tabf:array[0..7] of string[5]=('FILD','0','FIST','FISTP','0','FLD','0','FSTP');
tabn:array[0..7] of string[7]=('"fild"','0','"fist"','"fistp"','0','"fld"','0','"fstp"');

tabf2:array[0..3] of string[8]=('FCMOVNB','FCMOVNE','FCMOVNBE','FCMOVNU');
tabn2:array[0..3] of string[10]=('"fcomvnb"','"fcmovne"','"fcmovnbe"','"fcmovnu"');

tabf3:array[0..1] of string[6]=('FUCOMI','FCOMI');
tabn3:array[0..1] of string[8]=('"fucomi"','"fcomi"');

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
               if (i shr 3) and 7 <5 then
               writeln(' /*',i,'(0x',tohex(i),')*/ {',
               tabn[(i shr 3) and 7],',F_PAR+F_DW+F_D,',rm2p(i),',',tabf[(i shr 3) and 7],',0},')
               else
               writeln(' /*',i,'(0x',tohex(i),')*/ {',
               tabn[(i shr 3) and 7],',F_PAR+F_TW+F_D+F_DP,',rm2p(i),',',tabf[(i shr 3) and 7],',0},')
          end
          else
          if i=$e2 then
          writeln (' /*',i,'(0x',tohex(i),')*/ {"fclex",0,0,FCLEX,0},')
          else
          if i=$e3 then
          writeln (' /*',i,'(0x',tohex(i),')*/ {"finit",0,0,FINIT,0},')
          else
          if (i>=$c0) and (i<$e0) then
          writeln (' /*',i,'(0x',tohex(i),')*/ {',
          tabn2[(i shr 3) and 3],',F_PAR,',rm2p(i),',',tabf2[(i shr 3) and 3],',0},')
          else
          if (i>=$e8) and (i<$f8) then
          writeln (' /*',i,'(0x',tohex(i),')*/ {',
          tabn3[(i shr 3) and 3-1],',F_PAR,',rm2p(i),',',tabf3[(i shr 3) and 3-1],',0},')
          else
          writeln (' /*',i,'(0x',tohex(i),')*/ {0,0,0,0,0},');
     end;
end.