uses crt;
var
__cs,__ds,__es,__si,__di : word;
function inhex (a : word) : string;
const
hexx : array [0..15] of char=('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F');
begin
     inhex [0] := #4;
     inhex [1] := hexx[hi(a) shr 4];
     inhex [2] := hexx[hi(a) and $f];
     inhex [3] := hexx[lo(a) shr 4];
     inhex [4] := hexx[lo(a) and $f];
end;
begin
     asm
        mov ax,cs
        mov __cs,ax
        mov ax,ds
        mov __ds,ax
        mov ax,es
        mov __es,ax
        mov __si,si
        mov __di,di
     end;
     clrscr;
     write ('cs=',inhex(__cs));
     write (' ds=',inhex(__ds));
     write (' es=',inhex(__es));
     write (' di=',inhex(__di));
     write (' si=',inhex(__si));
end.