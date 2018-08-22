{$G+}
uses crt,key;
const
maxtex=4;
objtex=maxtex-1;
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
     if (sx+1=0) or (sy+1=0) then exit;
     w:=width[tn];
     h:=height[tn];
     if (x<0) then x1:=0 else x1:=x;
     if (x+sx>319) then x2:=319 else x2:=x+sx;
     if (x+sx<0) then exit;
     if (y<0) then y1:=0 else y1:=y;
     if (y+sy>199) then y2:=199 else y2:=y+sy;
     if (y+sy<0) then exit;
     dlx:=(w shl SH) div (sx+1);
     dly:=(h shl SH) div (sy+1);
     v:=(y1-y)*dly;
     u0:=(x1-x)*dlx;
     if (x2-x1<=0) then exit;
     for j:=y1 to y2 do
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

procedure clrscr(c1,c2:byte); assembler;
asm
   mov ax,seg buf
   les di,buf
   mov cx,64000/8
   mov al,c1
   mov ah,al
   mov dx,ax
   db 66h; shl ax,16
   mov ax,dx
   cld
   rep; db 66h; stosw;
   mov cx,64000/8
   mov al,c2
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
i:integer;
fn:string;
const
maxobj=64;
MAXDIST=500;
MINDIST=2;
DELTAP=8;
DELTAA=6;
PR=256;
OBJWIDTH=10;
OBJHEIGHT=16;
type
tpoint=record x,y:real; t,tag:integer; dx,dy:real; end;
var
objtab:array[0..maxobj-1] of tpoint;
px,py,angle:real;
isshot:byte;
shotx,shoty,sangle:real;
score:integer;

procedure movep(p,a:real);
begin
     px:=px+p*cos((angle+90)/180*3.14);
     py:=py+p*sin((angle+90)/180*3.14);
     angle:=angle+a;
end;

procedure puttex3d(tn:word; x,y,w,h,s,c:real);
var
xt,yt:real;
xp1,xp2,yp1,yp2:integer;
begin
     xt:=x-px;
     yt:=y-py;
     x:=xt*c-yt*s;
     y:=yt*c+xt*s;
     if (y<MINDIST) then exit;
     xp1:=160+round((x-OBJWIDTH/2)*PR/y);
     xp2:=160+round((x+OBJWIDTH/2)*PR/y);
     yp1:=100+round((-OBJHEIGHT/2)*PR/y);
     yp2:=100+round((OBJHEIGHT/2)*PR/y);
     puttex(tn,xp1,yp1,xp2-xp1,yp2-yp1);
end;

procedure drawobj;
var
mark:array[0..maxobj-1] of longint;
dist:array[0..maxobj] of longint;
i,j,maxi:word;
a2:real;
s,c:real;
xd,yd:longint;
begin
     a2:=-(angle)*3.14/180;
     s:=sin(a2);
     c:=cos(a2);
     for i:=0 to maxobj-1 do
     begin
          if objtab[i].t<>-1 then
          begin
               xd:=round(objtab[i].x-px);
               yd:=round(objtab[i].y-py);
               dist[i]:=xd*xd+yd*yd;
          end
          else
          dist[i]:=0;
     end;
     dist[maxobj]:=-1;
     for j:=1 to maxobj do
     begin
          maxi:=maxobj;
          for i:=0 to maxobj-1 do
          begin
               if dist[i]>0 then
               begin
                    if (dist[i]>dist[maxi]) then maxi:=i;
               end;
          end;
          if maxi<maxobj then
          puttex3d(objtab[maxi].t,objtab[maxi].x,objtab[maxi].y,OBJWIDTH,OBJHEIGHT,s,c);
          dist[maxi]:=0;
     end;
end;

function distp(a,b,c:real):boolean;
begin
     distp:=a*a+b*b<c*c;
end;

procedure shot;
const
dshot=40;
maxsdist=1000;
minsdist=20;
var
j:word;
s,c:real;
begin
     if isshot=1 then
     begin
          shotx:=px;
          shoty:=py;
          sangle:=angle;
          isshot:=2;
     end;
     s:=sin(-(angle)/180*3.14);
     c:=cos(-(angle)/180*3.14);
     for j:=0 to maxobj-1 do
     begin
          if objtab[j].t=-1 then continue;
          if distp(objtab[j].x-shotx,objtab[j].y-shoty,minsdist) then
          begin
               objtab[j].t:=-1;
               if objtab[j].tag=-1 then
               inc(score)
               else
               inc(score,5);
          end;
     end;
     puttex3d(objtex+1-1,shotx,shoty,5,5,s,c);
     s:=dshot*sin((sangle+90)/180*3.14);
     c:=dshot*cos((sangle+90)/180*3.14);
     shotx:=shotx+c;
     shoty:=shoty+s;
     if not distp(shotx-px,shoty-py,maxsdist) then begin isshot:=0 end;
end;

function moveo:boolean;
const
dm=10;
maxiter=40;
var
i:word;
ok:boolean;
begin
     ok:=false;
     for i:=0 to maxobj-1 do
     begin
          if objtab[i].t<>-1 then ok:=true;
          if objtab[i].tag=0 then
          begin
               if random(50)>45 then objtab[i].tag:=1;
               objtab[i].dx:=(integer(random(1024))-512)/1024*dm;
               objtab[i].dy:=(integer(random(1024))-512)/1024*dm;
          end
          else
          if objtab[i].tag>0 then
          begin
               objtab[i].x:=objtab[i].x+objtab[i].dx;
               objtab[i].y:=objtab[i].y+objtab[i].dy;
               inc(objtab[i].tag,1);
               if (objtab[i].tag>maxiter) then objtab[i].tag:=0;
          end;
     end;
     moveo:=ok;
end;

const
numtab:array[0..9,0..4,0..2] of byte=
(
((1,1,1),
 (1,0,1),
 (1,0,1),
 (1,0,1),
 (1,1,1)),
((0,1,0),
 (0,1,0),
 (0,1,0),
 (0,1,0),
 (0,1,0)),
((1,1,1),
 (0,0,1),
 (1,1,1),
 (1,0,0),
 (1,1,1)),
((1,1,1),
 (0,0,1),
 (1,1,1),
 (0,0,1),
 (1,1,1)),
((1,0,1),
 (1,0,1),
 (1,1,1),
 (0,0,1),
 (0,0,1)),
((1,1,1),
 (1,0,0),
 (1,1,1),
 (0,0,1),
 (1,1,1)),
((1,1,1),
 (1,0,0),
 (1,1,1),
 (1,0,1),
 (1,1,1)),
((1,1,1),
 (0,0,1),
 (0,0,1),
 (0,0,1),
 (0,0,1)),
((1,1,1),
 (1,0,1),
 (1,1,1),
 (1,0,1),
 (1,1,1)),
((1,1,1),
 (1,0,1),
 (1,1,1),
 (0,0,1),
 (1,1,1))
);

procedure drawdig(x,y:integer; c,a:byte);
var
i,j:integer;
begin
     for j:=0 to 4 do
     for i:=0 to 2 do if numtab[a mod 10,j,i]=1 then putpix(x+i,y+j,c);
end;

procedure drawnum(x,y:integer; c:byte; n:word);
var
i:integer;
k,l,m:word;
begin
     i:=x;
     k:=10000;
     m:=0;
     while k>0 do
     begin
          l:=(n div k) mod 10;
          if (m=0) and (l>0) then m:=1;
          if (m>0) or (l>0) or (k=1) then
          begin
               drawdig(i,y,c,l);
               inc(i,4);
          end;
          k:=k div 10;
     end;
end;

begin
     randomize;
     getmem(buf,64000);
     fn:=' .tex';
     for i:=0 to objtex-1 do
     begin
          fn[1]:=chr(i+ord('0'));
          load(i,fn);
     end;
     fn:='a.tex';
     load(objtex+1-1,fn);
     px:=0;
     py:=0;
     angle:=0;
     isshot:=0;
     for i:=0 to maxobj-1 do
     begin
          objtab[i].x:=integer(random(2*MAXDIST))-MAXDIST;
          objtab[i].y:=integer(random(2*MAXDIST))-MAXDIST;
          objtab[i].t:=random(objtex);
          if objtab[i].t=2 then
          objtab[i].tag:=0
          else
          objtab[i].tag:=-1;
          {writeln (objtab[i].x:0:0,',',objtab[i].y:0:0,',',objtab[i].t);}
     end;
     {readln;}
     {keyis(on);
     i:=0;
     fillchar(keyon,sizeof(keyon),0);
     while 1=1 do
     begin
          if keyon[i mod sizeof(keyon)]=1 then begin writeln (i mod sizeof(keyon)); break; end;
          inc(i);
     end;
     keyis(off);
     readln;}
     keyis(on);
     asm
        mov ax,13h
        int 10h
     end;
     while 1=1 do
     begin
          clrscr(9,14);
          if keyon[1]=1 then break;
          if keyon[72]=1 then movep(DELTAP,0);
          if keyon[80]=1 then movep(-DELTAP,0);
          if keyon[75]=1 then movep(0,DELTAA);
          if keyon[77]=1 then movep(0,-DELTAA);
          if keyon[57]=1 then begin dec(score); if score<0 then score:=0; isshot:=1; end;
          if not moveo then break;
          drawobj;
          drawnum(0,0,15,score);
          if isshot<>0 then shot;
          wfvr;
          cpybuf;
     end;
     keyis(off);
     asm
        mov ax,3
        int 10h
     end;
     writeln ('SCORE:',score);
end.