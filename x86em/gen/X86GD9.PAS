const
tabf:array[0..8] of string[6]=('FLD','0','FST','FSTP','FLDENV','FLDCW','FSTENV','FSTCW','FXCH');
tabn:array[0..8] of string[8]=('"fld"','0','"fst"','"fstp"','"fldenv"','"fldcw"','"fstenv"','"fstcw"','"fxch"');

tabf_e:array[0..15] of string[6]=('FCHS','FABS','0','0','FTST','FXAM','0','0',
'FLD1','FLDL2T','FLDL2E','FLDPI','FLDLG2','FLDLN2','FLDZ','0');
tabn_e:array[0..15] of string[8]=('"fchs"','"fabs"','0','0','"ftst"','"fxam"','0','0',
'"fld1"','"fldl2t"','"fldl2e"','"fldpi"','"fldlg2"','"fldln2"','"fldz"','0');

tabf_f:array[0..15] of string[7]=('F2XM1','FYL2X','FPTAN','FPATAN','FXTRACT','FPREM1','FDECSTP','FINCSTP',
'FPREM','FYL2XP1','FSQRT','FSINCOS','FRNDINT','FSCALE','FSIN','FCOS');
tabn_f:array[0..15] of string[9]=('"f2xm1"','"fyl2x"','"fptan"','"fpatan"','"fxtract"','"fprem1"','"fdecstp"','"fincstp"',
'"fprem"','"fyl2xp1"','"fsqrt"','"fsincos"','"frndint"','"fscale"','"fsin"','"fcos"');

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
          if ((i shr 3) and 7<4) and (i<$d0) then
          begin
               if i<$c0 then
               writeln(' /*',i,'(0x',tohex(i),')*/ {',
               tabn[(i shr 3) and 7],',F_PAR+F_DW,',rm2p(i),',',tabf[(i shr 3) and 7],',0},')
               else
               writeln(' /*',i,'(0x',tohex(i),')*/ {',
               tabn[((i and 15) shr 3)*8],',F_PAR,',rm2p(i),',',tabf[((i and 15) shr 3)*8],',0},')
          end
          else
          if i<$d0 then
          writeln(' /*',i,'(0x',tohex(i),')*/ {',tabn[(i shr 3) and 7],',F_PAR,',rm2p(i),',',tabf[(i shr 3) and 7],',0},')
          else
          if i=$d0 then
          writeln (' /*',i,'(0x',tohex(i),')*/ {"fnop",0,0,NOP,0},')
          else
          if (i>=$e0) and (i<$f0) then
          writeln (' /*',i,'(0x',tohex(i),')*/ {',tabn_e[i and 15],',0,0,',tabf_e[i and 15],',0},')
          else
          if i>$f0 then
          writeln (' /*',i,'(0x',tohex(i),')*/ {',tabn_f[i and 15],',0,0,',tabf_f[i and 15],',0},')
          else
          writeln (' /*',i,'(0x',tohex(i),')*/ {0,0,0,0,0},');
     end;
end.