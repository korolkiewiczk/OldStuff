const
tabf:array[0..7] of string[6]=('FIADD','FIMUL','FICOM','FICOMP','FISUB','FISUBR','FIDIV','FIDIVR');
tabn:array[0..7] of string[8]=('"fiadd"','"fimul"','"ficom"','"ficomp"','"fisub"','"fisubr"','"fidiv"','"fidivr"');

tabf2:array[0..7] of string[6]=('FADDP','FMULP','0','0','FSUBRP','FSUBP','FDIVRP','FDIVP');
tabn2:array[0..7] of string[8]=('"faddp"','"fmulp"','0','0','"fsubrp"','"fsubp"','"fdivrp"','"fdivp"');


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
          t2:=2;
     end;
     rm2p:=t1+p1 shl 8+t2 shl 16+p2 shl 24;
end;



var
i:byte;
begin
     for i:=0 to 255 do
     begin
          if i<$c0 then
          writeln(' /*',i,'(0x',tohex(i),')*/ {',tabn[(i shr 3) and 7],',F_PAR+F_DW,',rm2p(i),',',tabf[(i shr 3) and 7],',0},')
          else
          if i=$d9 then
          writeln (' /*',i,'(0x',tohex(i),')*/ {"fcompp",0,0,FCOMPP,0},')
          else
          writeln (' /*',i,'(0x',tohex(i),')*/ {'
          ,tabn2[(i shr 3) and 7],',F_PAR+F_CP,',rm2p(i),',',tabf2[(i shr 3) and 7],',0},')
     end;
end.