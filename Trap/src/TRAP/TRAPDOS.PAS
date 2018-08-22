{$S-,G+}
unit trapdos;

interface

{deklaracje procedur i funkcji}

function getenv (env : string; var out : string) : pchar;
procedure getintvec (intn : byte; var proc : pointer);
procedure setintvec (intn : byte; proc : pointer);
function getdosver : word;
function getwinver : word;

implementation

function getenv (env : string; var out : string) : pchar;
type
tb = array [0..65534] of char;
var
tbp : ^tb;
z1,z2 : char;
i : word;
s : string;
begin
     getenv := nil;
     tbp := ptr (memw [prefixseg:$2c],0);
     z1 := tbp^[0];
     z2 := tbp^[1];
     i := 0;
     s := '';
     while (z1 <> #0) or (z2 <> #0) do
     begin
          z1 := tbp^[i];
          z2 := tbp^[i+1];
          s := s+z1;
          if (z2 = '=') and (s=env) then
          begin
               tbp := ptr (seg (tbp^),ofs (tbp^)+i+2);
               out := '';
               getenv := ptr (seg (tbp^),ofs (tbp^)+i+2);;
               i := 0;
               while tbp^[i] <> #0 do
               begin
                    out := out+tbp^[i];
                    inc (i);
               end;
               break;
          end;
          inc (i);
          if z1 = #0 then s:='';
     end;
end;

procedure getintvec;
begin
asm
   mov ah,35h
   mov al,intn
   int 21h
   mov ax,es
   les di,proc
   cld
   xchg bx,ax
   stosw
   xchg bx,ax
   stosw
end;
end;

procedure setintvec;
begin
asm
   mov ah,25h
   mov al,intn
   push ds
   lds dx,proc
   int 21h
   pop ds
end;
end;

function getdosver : word; assembler;
asm
   mov ax,3001h
   int 21h
end;

function getwinver : word; assembler;
asm
   mov ax,160ah
   int 2fh
   mov ax,bx
end;


end.