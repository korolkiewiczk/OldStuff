{
  TRAP (r) Copyright (c) Kamil Korolkiewicz 2003
  Darmowa biblioteka do obsˆugi klawiatury stworzona na potzeby gry TRAP (r)
  Wszelkie prawa autorskie zastrze¾one
}
{$S-,G+}
unit key;

interface

{doˆ¥czone unity}

uses trapdos;


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
numlockp : byte;
shiftkp : boolean;
_224k : byte;

{procedury}

procedure keyint; interrupt;
procedure setnumlock (m : byte);
function keyis (m : byte) : boolean;
procedure waittokeypress (k : byte);
function anykeypressed : boolean;
function getkey : byte;
procedure pressnumlock;

implementation

procedure keyint; assembler;
asm
   cmp stdkeyint,0
   jz @d
   mov di,offset oldkeyint
   pushf
   call dword ptr [di]
   jmp @q
   @d:
   in al,60h
   cmp al,224
   jne @_224d
   mov _224k,1
   jmp @q224
   @_224d:
   cmp al,197
   jne @dn
   mov ah,1
   sub ah,numlockp
   mov numlockp,ah
   @dn:
   cmp al,42
   jne @ds
   mov shiftkp,1
   @ds:
   cmp al,42+128
   jne @dps
   cmp _224k,1
   je @dps
   mov shiftkp,0
   @dps:
   cmp _224k,1
   jne @d224
   mov _224k,0
   @d224:
   mov bl,al
   xor bh,bh
   shr al,7
   xor al,1
   and bl,127
   mov byte ptr [bx+keyon],al
   @q224:
   mov al,20h
   out 20h,al
   @q:
end;

procedure setnumlock;
begin
     if m=on then
     begin
          if mem [$0040:$0017] and $20 = $20 then numlockp := 1 else numlockp := 0;
     end
     else
     begin
          if numlockp = 1 then mem [$0040:$0017] := mem [$0040:$0017] or $20
          else
          mem [$0040:$0017] := mem [$0040:$0017] and (not $20);
     end;
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
          setnumlock (m);
     end
     else
     if m = 0 then
     begin
          setintvec ($9,oldkeyint);
          keyintus := false;
          setnumlock (m);
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

function anykeypressed : boolean;
var
i : byte;
begin
     anykeypressed := false;
     for i := 0 to 127 do if keyon [i] = 1 then anykeypressed := true;
end;

function getkey : byte;
var
i : byte;
{p : byte;}
begin
     getkey := 255;
     {p := 255;}
     for i := 0 to 127 do if keyon[i]=1 then
     begin
          {if i <> 42 then
          begin}
               getkey := i;
               exit;
          {end
          else p := i;}
     end;
     {if p = 42 then getkey := 42;}
end;

procedure pressnumlock; assembler;
asm
   cmp numlockp,0
   jz @q
   {}
   @d1:
   in al,64h
   test al,2h
   jnz @d1
   mov al,0d2h
   out 64h,al
   {}
   @d2:
   in al,64h
   test al,2h
   jnz @d2
   mov al,69
   out 60h,al
   {}
   @d3:
   in al,64h
   test al,2h
   jnz @d3
   mov al,0d2h
   out 64h,al
   {}
   @d4:
   in al,64h
   test al,2h
   jnz @d4
   mov al,197
   out 60h,al
   {}
   mov ax,40h
   mov es,ax
   mov bx,17h
   mov ah,es:[bx]
   shr ah,4
   {}
   mov al,0edh
   out 60h,al
   @d5:
   in al,64h
   test al,2h
   jnz @d5
   mov al,ah
   and al,5
   out 60h,al
   @q:
end;

end.