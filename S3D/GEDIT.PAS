{$G+}
uses crt;
const
maxtex=1;
SCALE=4;
var
tex:array [0..maxtex-1] of ^byte;
width,height:array[0..maxtex] of word;
buf:pointer;

procedure putpix(x,y,c:integer);
begin
     if (x<0) or (y<0) or (x>319) or (y>199) then exit;
     asm
        mov ax,y
        mov dx,ax
        shl ax,6
        shl dx,8
        add ax,dx
        les di,buf
        add di,x
        add di,ax
        mov ax,c
        mov es:[di],al
     end;
end;

procedure puttex(tn:word; x,y,sx,sy:integer);
const
SH=8;
var
dlx,dly,i,j:integer;
x1,y1,x2,y2:integer;
v,u0:integer;
w,h:word;
ll:word;
begin
     w:=width[tn];
     h:=height[tn];
     if (x<0) then x1:=0 else x1:=x;
     if (x+sx>319) then x2:=319 else x2:=x+sx;
     if (y<0) then y1:=0 else y1:=y;
     if (y+sy>199) then y2:=199 else y2:=y+sy;
     dlx:=(w shl SH) div sx;
     dly:=(h shl SH) div sy;
     v:=(y1-y)*dly;
     u0:=(x1-x)*dlx;
     if (x2-x1<0) then exit;
     for j:=y1 to y2-1 do
     begin
          asm
             push ds
             mov ax,tn
             shl ax,2
             mov si,offset tex
             add si,ax
             les di,buf
             lds si,[si]
             mov ax,j
             mov dx,ax
             shl ax,8
             shl dx,6
             add ax,dx
             add ax,x1
             add di,ax
             mov ax,x2
             sub ax,x1
             inc ax
             mov ll,ax
             mov ax,v
             shr ax,SH
             mul w
             mov dx,u0
             mov bx,ax
@l:
                mov cx,dx
                shr cx,SH
                add bx,cx
                mov al,[bx+si]
                or al,al
                jz @d
                mov es:[di],al
@d:
                add dx,dlx
                sub bx,cx
                inc di
                dec ll
                jnz @l
             pop ds
          end;
          inc(v,dly);
     end;
end;

procedure clrscr(c:byte); assembler;
asm
   mov ax,seg buf
   les di,buf
   mov cx,64000/4
   mov al,c
   mov ah,al
   mov dx,ax
   db 66h; shl ax,16
   mov ax,dx
   cld
   rep; db 66h; stosw;
end;

procedure cpybuf; assembler;
asm
   push ds
   lds si,buf
   mov ax,0a000h
   mov es,ax
   xor di,di
   mov cx,64000/4
   cld
   rep; db 66h; movsw;
   pop ds
end;

procedure wfvr; assembler;
asm
   mov dx,3dah
   @tp:
       in al,dx
       test al,00001000b
       jnz @tp
   @np:
       in al,dx
       test al,00001000b
       jz @np
end;

procedure save(tn:word; fn:string);
var
f:file;
begin
     assign(f,fn);
     rewrite(f,1);
     blockwrite(f,width[tn],1);
     blockwrite(f,height[tn],1);
     blockwrite(f,tex[tn]^,width[tn]*height[tn]);
     close(f);
end;

procedure load(tn:word; fn:string);
var
f:file;
begin
     assign(f,fn);
     reset(f,1);
     blockread(f,width[tn],1);
     blockread(f,height[tn],1);
     getmem(tex[tn],width[tn]*height[tn]);
     blockread(f,tex[tn]^,width[tn]*height[tn]);
     close(f);
end;

var
fn:string;
i,x,y:integer;
sx,sy:byte;
col:byte;
c:char;
begin
     getmem(buf,64000);
     write('S/L?');
     readln(c);
     c:=upcase(c);
     write('f:');
     readln(fn);
     if c='L' then
     begin
          load(0,fn);
          sx:=width[0];
          sy:=height[0];
     end
     else
     begin
          write('sx:');
          readln (sx);
          write('sy:');
          readln(sy);
          width[0]:=sx;
          height[0]:=sy;
          getmem(tex[0],sx*sy);
          asm
             mov di,offset tex
             les di,[di]
             xor ax,ax
             mov al,sx
             mul sy
             mov cx,ax
             xor al,al
             rep stosb
          end;
     end;
     col:=1;
     asm
        mov ax,13h
        int 10h
     end;
     repeat
           clrscr(0);
           if keypressed then
           begin
           c:=readkey;
           if c=#0 then
           begin
                c:=readkey;
                if c=#72 then dec(y);
                if c=#80 then inc(y);
                if c=#75 then dec(x);
                if c=#77 then inc(x);
                if c=#73 then dec(col);
                if c=#81 then inc(col);
           end;
           if x<0 then x:=0;
           if y<0 then y:=0;
           if x>=sx then x:=sx-1;
           if y>=sy then y:=sy-1;
           if c=#32 then
           asm
              mov ax,y
              mul sx
              add ax,x
              mov di,offset tex
              les di,[di]
              add di,ax
              mov al,col
              mov es:[di],al
           end;
           end;
           puttex(0,0,0,SCALE*sx,SCALE*sy);
           for i:=0 to SCALE-1 do putpix(SCALE*x+i,SCALE*y,col);
           for i:=0 to SCALE-1 do putpix(SCALE*x+i,SCALE*(y+1),col);
           for i:=0 to SCALE-1 do putpix(SCALE*x,SCALE*y+i,col);
           for i:=0 to SCALE do putpix(SCALE*(x+1),SCALE*y+i,col);
           wfvr;
           cpybuf;
     until c=#27;
     asm
        mov ax,3
        int 10h
     end;
     save(0,fn);
end.