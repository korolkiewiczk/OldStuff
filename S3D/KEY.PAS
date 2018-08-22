{
  TRAP (r) Copyright (c) Kamil Korolkiewicz 2003
  Darmowa biblioteka do obsˆugi klawiatury stworzona na potzeby gry TRAP (r)
  Wszelkie prawa autorskie zastrze¾one
}
{$S-,G+}
unit key;

interface

{doˆ¥czone unity}

uses dos;


{deklaracje staˆych}

const

on = 1;
off = 0;
any = 0;
keyintus : boolean = false;
stdkeyint : boolean = false;


{deklaracje zmiennych}

var

keyon : array [0..127] of byte;
oldkeyint : pointer;

{procedury}

procedure keyint; interrupt;
function keyis (m : byte) : boolean;
procedure waittokeypress (k : byte);

implementation

procedure keyint; assembler;
asm
   cmp stdkeyint,0
   jz @d
   jmp dword ptr oldkeyint
   @d:
   in al,60h
   mov bl,al
   xor bh,bh
   shr al,7
   xor al,1
   and bl,127
   mov byte ptr [bx+keyon],al
   mov al,20h
   out 20h,al
end;

function keyis;
begin
     if (m = 0) and (not keyintus) or (m > 1) then
     begin
          keyis := false;
          exit;
     end;
     if (m = 1) and (keyintus) then
     begin
          keyis := false;
          exit;
     end;
     if m = 1 then
     begin
          getintvec ($9,oldkeyint);
          setintvec ($9,@keyint);
          fillchar (keyon,128,0);
          keyintus := true;
     end
     else
     if m = 0 then
     begin
          setintvec ($9,oldkeyint);
          keyintus := false;
     end;
     keyis := true;
end;

procedure waittokeypress;
var
i : byte;
p : boolean;
begin
     p := false;
     if k = 0 then
     begin
          repeat
                for i := 0 to 127 do if keyon[i] = 1 then
                begin
                     p := true;
                     break;
                end;
          until p;
     end
     else
     begin
          repeat
                for i := 0 to 127 do if (keyon[i] = 1) and (i = k) then
                begin
                     p := true;
                     break;
                end;
          until p;
     end;
end;

end.