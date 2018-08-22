{
  TRAP (r) Copyright (c) Kamil Korolkiewicz 2004
  Darmowa biblioteka do obsˆugi pami©si xms stworzona na potzeby gry TRAP (r)
  Wszelkie prawa autorskie zastrze¾one
}

{$S-,G+}
unit xmem;

interface

{deklaracje staˆych}

const

xms2dosmem = 0; {tryb 0: z pami©ci xms do konwencjonalnej}
xms2xms    = 1; {tryb 1: z pami©ci xms do xms}
dosmem2xms = 2; {tryb 2: z pami©ci konwencjonalnej do xms}
unkown     = $ff; {tryb niewiadomy (pami©† nie jest kopiowana}

{deklaracje typ¢w}

type

temm = record
             btocopy : longint;
             shandle : word;
             sofs : longint;
             dhandle : word;
             dofs : longint;
end;

{deklaracje zmiennych}

var

handle : word;
xmsver : word;
xmsentr : pointer;
xmemhsize : longint; {kb}
emm : temm;
up : longint; {b}
iserr : byte;

{deklaracje procedur i funckcji}

function  detectxms : boolean; {sprawdza czy jest zainstalowany sterownik xms}
procedure initxmsheap (size : longint); {przydziela [size] kb pamieci xms dla programu}
procedure freexmsheap; {zwalnia t¥ pami©†}
procedure reallocxmsheap (newsize : longint); {przydziela now¥ ilo˜† pami©ci programowi (size] kb pamieci xms)}
function  checkxmsver : word; {pobiera wersje drivera xms}
function  xmsmemavail : longint; {zwraca ilo˜† caˆej pami©ci xms w kb}
function  xmsmaxavail : longint; {zwraca nawi©kszy wolny blok pami©ci xms w kb}
procedure xmemget (var np : pointer; size : longint); {pobiera okre˜lon¥ ilo˜† pami©ci xms (odpowiednik pascalowego getmem)}
procedure xmemfree (var np : pointer; size : longint); {zwalnia okre˜lon¥ ilo˜† pami©ci xms a np = ffffffff(pascalowy freemem)}
procedure xmemmove (s,d : pointer; size : longint; t : byte); {kopiuje [size] b z [s] do [d]}
function  xmemavail : longint; {zwraca caˆ¥ ilo˜† pami©ci heap}
function  xmemmaxavail : longint; {zwraca najwi©kszy wony blok pami©ci heap}

{implementacje procedur i funkcji}

implementation

function detectxms; assembler;
asm
   mov ax,4300h
   int 2fh
   cmp al,80h
   jnz @q
   mov ax,4310h
   int 2fh
   mov ax,seg xmsentr
   {mov fs,ax} db 8eh; db 0e0h;
   mov di,offset xmsentr
   db 64h; {fs:} mov [di],bx
   db 64h; {fs:} mov [di+2],es
   call checkxmsver
   mov xmsver,ax
   mov iserr,0
   mov ax,1
   jmp @q2
   @q:
   xor ax,ax
   @q2:
end;

procedure initxmsheap;
begin
asm
   mov ax,xmsver
   cmp ah,3
   jb @d
   mov ax,8900h
   jmp @d2
   @d:
   mov ax,0900h
   @d2:
   {mov edx,[bp+6]}     db 66h; db 8bh; db 56h; db 06h;
   call xmsentr
   cmp ax,0
   jz @err
   mov handle,dx
   {mov up,0}           db 66h; db 0c7h; db 06h; dw offset up; dd 0;
   {mov eax,[bp+6]}     db 66h; db 8bh; db 46h; db 06h;
   {mov xmshsize,eax}   db 66h; db 0a3h; dw offset xmemhsize;
   mov iserr,0
   jmp @q
   @err:
   mov iserr,bl
   @q:
end;
end;

procedure freexmsheap; assembler;
asm
   mov ax,0ah
   mov dx,handle
   call xmsentr
   cmp ax,0
   jz @err
   mov iserr,0
   jmp @q
   @err:
   mov iserr,bl
   @q:
end;

procedure reallocxmsheap;
begin
asm
   mov ax,xmsver
   cmp ah,3
   jb @d
   mov ax,8f00h
   jmp @d2
   @d:
   mov ax,0f00h
   @d2:
   mov dx,handle
   {mov ebx,[bp+6]}     db 66h; db 8bh; db 5eh; db 06h
   call xmsentr
   cmp ax,0
   jz @err
   {mov eax,[bp+6]}     db 66h; db 8bh; db 46h; db 06h
   {mov xmshsize,eax}   db 66h; db 0a3h; dw offset xmemhsize;
   mov iserr,0
   jmp @q
   @err:
   mov iserr,bl
   @q:
end;
end;

function checkxmsver; assembler;
asm
   mov ax,0
   call xmsentr
end;

function xmsmemavail; assembler;
asm
   mov ax,xmsver
   cmp ax,3
   jb @d
   mov ax,8800h
   call xmsentr
   cmp bl,0
   jnz @err
   {mov eax,edx} db 66h; db 8bh; db 0cah;
   {shr edx,16}  db 66h; db 0c1h; db 0eah; db 16;
   mov iserr,0
   jmp @q
   @d:
   mov ax,0800h
   call xmsentr
   mov ax,dx
   xor dx,dx
   mov iserr,0
   jmp @q
   @err:
   mov iserr,bl
   @q:
end;

function xmsmaxavail; assembler;
asm
   mov ax,xmsver
   cmp ax,3
   jb @d
   mov ax,8800h
   call xmsentr
   cmp bl,0
   jnz @err
   {mov edx,eax} db 66h; db 8bh; db 0d0h;
   {shr edx,16}  db 66h; db 0c1h; db 0eah; db 16;
   mov iserr,0
   jmp @q
   @d:
   mov ax,0800h
   call xmsentr
   xor dx,dx
   mov iserr,0
   jmp @q
   @err:
   mov iserr,bl
   @q:
end;

procedure xmemget;
begin
asm
   {mov eax,xmshsize}     db 66h; db 0a1h; dw offset xmemhsize;
   {shl eax,10}           db 66h; db 0c1h; db 0e0h; db 10;
   {sub eax,up}           db 66h; db 2bh; db 06; dw offset up;
   {cmp eax,[bp+6](size)} db 66h; db 3bh; db 46h; db 06h;
   jb @error
   les di,np
   {mov eax,up}           db 66h; db 0a1h; dw offset up;
   {mov es:[di],eax}      db 66h; db 26h; db 89h; db 05h;
   {mov eax,[bp+6](size)} db 66h; db 8bh; db 46h; db 6;
   {add up,eax}           db 66h; db 01h; db 06h; dw offset up
   jmp @q
   @error:
   les di,np
   {xor eax,eax}          db 66h; xor ax,ax;
   {dec eax}              db 66h; db 48; {eax = 0ffffffffh}
   {mov es:[di],eax}      db 66h; db 26h; db 89h; db 05h;
   @q:
end;
end;

procedure xmemfree;
begin
asm
   les di,np
   {cmp es:[di],0ffffffffh}    db 66h; db 26h; db 83h; db 3dh; db 0ffh;
   je @q
   {mov eax,up}                db 66h; db 0a1h; dw offset up;
   {sub eax,[bp+6] (size)}     db 66h; db 2bh; db 46h; db 6;
   {cmp eax,es:[di]}           db 66h; db 26h; db 3bh; db 5h;
   jnz @q2
   {mov eax,[bp+6] (size)}     db 66h; db 8bh; db 46h; db 6;
   {sub up,eax}                db 66h; db 29h; db 06h; dw offset up;
   @q2:
   {mov es:[di],0ffffffffh}    db 66h; db 26h; db 0c7h; db 05h; dd 0ffffffffh;
   @q:
end;
end;

{dla t=
0.Z pami©ci xms do pami©ci dolnej
1.Z pami©ci xms do pami©ci xms
2.Z pami©ci dolnej do pami©ci xms}
procedure xmemmove;
begin
asm
   mov al,[bp+6] {t}
   cmp al,0
   ja @d1
   {mov eax,[bp+12] (d)}     db 66h; db 8bh; db 46h; db 12;
   {mov emm.dofs,eax}        db 66h; db 0a3h; dw offset emm.dofs;
   {mov eax,[bp+16] (s)}     db 66h; db 8bh; db 46h; db 16;
   {mov emm.sofs,eax}        db 66h; db 0a3h; dw offset emm.sofs;
   mov emm.dhandle,0
   mov ax,handle
   mov emm.shandle,ax
   jmp @go
   @d1:
   cmp al,1
   ja @d2
   {mov eax,[bp+12] (d)}     db 66h; db 8bh; db 46h; db 12;
   {mov emm.dofs,eax}        db 66h; db 0a3h; dw offset emm.dofs;
   {mov eax,[bp+16] (s)}     db 66h; db 8bh; db 46h; db 16;
   {mov emm.sofs,eax}        db 66h; db 0a3h; dw offset emm.sofs;
   mov ax,handle
   mov emm.dhandle,ax
   mov emm.shandle,ax
   jmp @go
   @d2:
   cmp al,2
   ja @q
   {mov eax,[bp+12] (d)}     db 66h; db 8bh; db 46h; db 12;
   {mov emm.dofs,eax}        db 66h; db 0a3h; dw offset emm.dofs;
   {mov eax,[bp+16] (s)}     db 66h; db 8bh; db 46h; db 16;
   {mov emm.sofs,eax}        db 66h; db 0a3h; dw offset emm.sofs;
   mov emm.shandle,0
   mov ax,handle
   mov emm.dhandle,ax
   @go:
   {mov eax,[bp+8] (size)}   db 66h; db 8bh; db 46h; db 8;
   {mov emm.btocopy,eax}     db 66h; db 0a3h; dw offset emm.btocopy;
   mov ax,0b00h
   mov si,offset emm
   call xmsentr
   cmp al,1
   je @noerr
   mov iserr,bl
   jmp @q
   @noerr:
   mov iserr,0
   @q:
end;
end;

function xmemavail; assembler;
asm
   {mov eax,xmemhsize}    db 66h; db 0a1h; dw offset xmemhsize;
   {shl eax,10}           db 66h; db 0c1h; db 0e0h; db 10;
   {mov edx,eax}          db 66h; db 8bh; db 0d0h;
   {shr edx,16}           db 66h; db 0c1h; db 0eah; db 16;
end;

function xmemmaxavail; assembler;
asm
   {mov eax,xmemhsize}    db 66h; db 0a1h; dw offset xmemhsize;
   {shl eax,10}           db 66h; db 0c1h; db 0e0h; db 10;
   {sub eax,up}           db 66h; db 2bh; db 06h; dw offset up;
   {mov edx,eax}          db 66h; db 8bh; db 0d0h;
   {shr edx,16}           db 66h; db 0c1h; db 0eah; db 16;
end;

end.