{$S-,G+}
unit trapcrt;

interface

{stale}

const


{biosextkeys1}
INSERT_MODE_ON      = $80;
CAPS_LOCK_MODE_ON   = $40;
NUM_LOCK_MODE_ON    = $20;
SCROLL_LOCK_MODE_ON = $10;
ALT_PRESSED         = $8;
CTRL_PRESSED        = $4;
LSHIFT_PRESSED      = $2;
RSHIFT_PRESSED      = $1;

{biosextkeys2}
INSERT_PRESSED      = $80;
CAPS_LOCK_PRESSED   = $40;
NUM_LOCK_PRESSED    = $20;
SCROLL_LOCK_PRESSED = $10;
PAUSE_MODE_ON       = $8;
SYSREQ_PRESSED      = $4;
LALT_PRESSED        = $2;
LCTRL_PRESSED       = $1;

{deklaracje zmiennych}

var

trapcrtcx,trapcrtcy : word;
biosextkeys1 : byte absolute $0040:$0017;
biosextkeys2 : byte absolute $0040:$0018;


{deklaracje procedur i funckji}

function readkey (var kchr : char; var kscan : byte) : boolean;
procedure wait (ms : longint);
procedure clrscr;
procedure setcur (crs : byte);
procedure gotoxy (x,y : byte);
procedure trapcrtprint (x,y : word; a : byte; txt : pchar);

implementation

function readkey (var kchr : char; var kscan : byte) : boolean;
begin
asm
   mov ax,0100h
   int 16h
   jz @d1
   xor ax,ax
   int 16h
   les di,kchr
   mov es:[di],al
   les di,kscan
   mov es:[di],ah
   mov al,1
   jmp @d2
   @d1:
   xor al,al
   @d2:
   mov [bp-1],al
end;
end;

procedure wait;
var
a,b : longint;
begin
     a := meml [0:1132];
     b := a;
     while b-a < (ms shl 8) div 14065 do b := meml [0:1132];
end;

procedure clrscr; assembler;
asm
   mov ax,0b800h
   mov es,ax
   mov di,0
   mov cx,2000
   (*{xor eax,eax}        db 66h; xor ax,ax
   {rep stosd}          db 66h; rep stosw*)
   mov ax,0720h
   rep stosw
   mov trapcrtcx,0
   mov trapcrtcy,0
   xor ax,ax
   push ax
   push ax
   call gotoxy
end;

procedure setcur;
begin
asm
   cmp crs,0
   je @d
   mov ch,14
   mov cl,15
   jmp @a
   @d:
   mov ch,16
   mov cl,0
   @a:
   mov ax,0100h
   int 10h
end;
end;

procedure gotoxy;
begin
asm
   mov ax,0200h
   mov bx,0
   mov dh,y
   mov dl,x
   int 10h
end;
end;

procedure trapcrtprint;
begin
asm
   push ds
   mov ax,0b800h
   mov es,ax
   mov ax,y
   mov trapcrtcy,ax
   mov bx,160
   mul bx
   mov di,x
   shl di,1
   add di,ax
   push di
   lds si,txt
   mov ah,a
   @pentl:
   lodsb
   or al,al
   jz @q
   stosw
   jmp @pentl
   @q:
   mov bx,di
   pop di
   pop ds
   mov ax,bx
   sub ax,di
   shr ax,1
   add ax,x
   mov trapcrtcx,ax
   push ax
   mov ax,trapcrtcy
   push ax
   call gotoxy
end;
end;

end.