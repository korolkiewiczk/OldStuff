{$DEFINE COPR}
{$IFDEF COPR}
{$N+}
{$ENDIF}
{$G+}
unit f_x;

interface

uses graph,crt;

const

UD=32767;
maxc=64;
startc=32;

type
{$IFDEF COPR}
f_x_t=single;
{$ELSE}
f_x_t=real;
{$ENDIF}

{2d}
tf_x=function(x:f_x_t):f_x_t;

{3d}
tf_xy=function(x,y:f_x_t):f_x_t;
tpoint2d=record
             x,y:integer;
end;

var
{2d}
oxy_x,oxy_y:f_x_t;               {max. wsp. x i y}
{3d}
oxyz_x,oxyz_y,oxyz_z:f_x_t;      {max. wsp. x,y i z}
xyzscr:f_x_t;                    {plaszczyzna rzutowania (wsp. y)}
posx,posy,posz:f_x_t;            {pozycja x,y,z}
delta_xyz:f_x_t;                 {co ile ma obliczac}
{col}
gcol:word;

function Init_f_x(gmode:integer) : boolean;
procedure Close_f_x;
{2d}
procedure Draw_OXY;
procedure Draw_f_x(f:tf_x);             {rysuje funkcje f}
procedure Draw_df_x(f:tf_x);            {rysuje I pochodna funkcji f}
procedure Draw_fd_x(f:tf_x);            {rysuje I pierwotna funkcji f}
{3d}
procedure Init3df_xy;
procedure Draw_OXYZ;
procedure Draw_3df_xy(f:tf_xy);

implementation
const

maxb=8192;

var

scrx,scry:word;
sxd2,syd2:word;
oxyd:boolean;
buf_z:array[0..maxb-1] of f_x_t;

function Init_f_x(gmode:integer) : boolean;
var
gdrv:integer;
begin
     {gdrv:=Detect;}
     gdrv:=InstallUserDriver('svga256',nil);
     InitGraph(gdrv,gmode,'');
     if GraphResult<>grOK then Init_f_x:=false
     else
     begin
          scrx:=GetMaxX;
          scry:=GetMaxY;
          oxy_x:=6;
          oxy_y:=6/(scrx/scry);
          gcol:=maxc-1;
          oxyd:=false;
          sxd2:=scrx div 2;
          syd2:=scry div 2;
          Init_f_x:=true;
     end;
end;

procedure Close_f_x;
begin
     CloseGraph;
end;

procedure Draw_OXY;
var
dx,dy:f_x_t;
i:integer;
a,b:integer;
begin
     SetColor(7);

     Line(0,syd2,scrx,syd2);                            {hline}
     Line(sxd2,0,sxd2,scry);                            {vline}

     MoveTo(sxd2-4,5);
     LineTo(sxd2,0);
     LineTo(sxd2+4,5);

     MoveTo(scrx-6,syd2-4);
     LineTo(scrx-1,syd2);
     LineTo(scrx-6,syd2+4);

     OutTextXY(sxd2+7,5,'y');
     OutTextXY(scrx-9,syd2+8,'x');

     dx:=sxd2/oxy_x;
     dy:=syd2/oxy_y;

     a:=round(oxy_x);
     b:=a-1;
     for i:=0 to b do Line(round(i*dx)+sxd2,syd2-2,round(i*dx)+sxd2,syd2+2);
     for i:=0 to a do Line(-round(i*dx)+sxd2,syd2-2,-round(i*dx)+sxd2,syd2+2);
     a:=round(oxy_y);
     b:=a-1;
     for i:=0 to a do Line(sxd2-2,round(i*dy)+syd2,sxd2+2,round(i*dy)+syd2);
     for i:=0 to b do Line(sxd2-2,-round(i*dy)+syd2,sxd2+2,-round(i*dy)+syd2);
     oxyd:=true;
end;

procedure Draw_f_x(f:tf_x);
function sgn(a:f_x_t) : f_x_t;
begin
     if a>0 then sgn:=1 else sgn:=-1;
end;
var
x,y,py:f_x_t;
inty1,inty2:integer;
i:integer;
dx,dsy:f_x_t;
begin
     if not oxyd then Draw_OXY;

     dx:=oxy_x/sxd2;
     dsy:=syd2/oxy_y;

     py:=f((-sxd2-1)*dx);
     if py=UD then py:=0;
     SetColor(gcol);

     for i:=-sxd2 to sxd2 do
     begin
          y:=f(i*dx);                                   {x:=g(i)=i*dx}
          if y=UD then begin py:=UD; continue; end;

          if abs(y)>oxy_y+1 then y:=sgn(y)*(oxy_y+1);

          if abs(y-py)<=2*oxy_y then
          begin
          inty1:=round(py*dsy);                         {inty1:=h(py)=py*dsy}
          inty2:=round(y*dsy);                          {inty2:=h(y)=y*dsy}
          Line(i-1+sxd2,-inty1+syd2,i+sxd2,-inty2+syd2);
          end;

          py:=y;
     end;
end;

procedure Draw_df_x(f:tf_x);
function sgn(a:f_x_t) : f_x_t;
begin
     if a>0 then sgn:=1 else sgn:=-1;
end;
var
x,y,y2,py:f_x_t;
inty1,inty2:integer;
i:integer;
dx,dsy:f_x_t;
begin
     if not oxyd then Draw_OXY;

     dx:=oxy_x/sxd2;
     dsy:=syd2/oxy_y;

     py:=f((-sxd2-1)*dx);
     {if py=UD then py:=0;}
     py:=(f((-sxd2-1)*dx+dx)-py)/dx;
     SetColor(gcol);

     for i:=-sxd2 to sxd2 do
     begin
          y:=f(i*dx);                                   {x:=g(i)=i*dx}
          if y=UD then continue;
          y2:=f(i*dx+dx);
          if y2=UD then continue;
          y:=(y2-y)/dx;                                 {y:=f'(x)=(f(x+dx)-f(x))/dx}

          if abs(y)>oxy_y+1 then y:=sgn(y)*(oxy_y+1);

          if abs(y-py)<=2*oxy_y then
          begin
          inty1:=round(py*dsy);                         {inty1:=h(py)=py*dsy}
          inty2:=round(y*dsy);                          {inty2:=h(y)=y*dsy}
          Line(i-1+sxd2,-inty1+syd2,i+sxd2,-inty2+syd2);
          end;

          py:=y;
     end;
end;

procedure Draw_fd_x(f:tf_x);
function sgn(a:f_x_t) : f_x_t;
begin
     if a>0 then sgn:=1 else sgn:=-1;
end;
var
x,y,y2,py:f_x_t;
inty1,inty2:integer;
i:integer;
dx,dsy:f_x_t;
begin
     if not oxyd then Draw_OXY;

     dx:=oxy_x/sxd2;
     dsy:=syd2/oxy_y;

     py:=0;
     y:=0;
     SetColor(gcol);

     for i:=0 to sxd2 do
     begin
          y2:=f(i*dx);
          if y2=UD then continue;
          y:=y+y2*dx;                                   {y:=df(x)}

          if abs(y)>oxy_y+1 then y:=sgn(y)*(oxy_y+1);

          if abs(y-py)<=2*oxy_y then
          begin
          inty1:=round(py*dsy);                         {inty1:=h(py)=py*dsy}
          inty2:=round(y*dsy);                          {inty2:=h(y)=y*dsy}
          Line(i-1+sxd2,-inty1+syd2,i+sxd2,-inty2+syd2);
          end;

          py:=y;
     end;
     py:=0;
     y:=0;
     for i:=-1 downto -sxd2 do
     begin
          y2:=f(i*dx);
          if y2=UD then continue;
          y:=y-y2*dx;                                   {y:=df(x)}

          if abs(y)>oxy_y+1 then y:=sgn(y)*(oxy_y+1);

          if abs(y-py)<=2*oxy_y then
          begin
          inty1:=round(py*dsy);                         {inty1:=h(py)=py*dsy}
          inty2:=round(y*dsy);                          {inty2:=h(y)=y*dsy}
          Line(i+1+sxd2,-inty1+syd2,i+sxd2,-inty2+syd2);
          end;

          py:=y;
     end;
end;

procedure Init3df_xy;
var
i:integer;
begin
     oxyz_x:=10;
     oxyz_y:=10;
     oxyz_z:=10;
     delta_xyz:=0.5;
     posx:=0;
     posy:=-oxyz_y-10;
     posz:=2;
     xyzscr:=400;
     for i:=0 to maxc-1 do
     SetRGBPalette(i,i*(64 div maxc),i*(64 div maxc),i*(64 div maxc));
end;

procedure DoPointS(x,y,z:f_x_t; var p:tpoint2d);
var
yy:f_x_t;
begin
     yy:=y-posy;
     if yy<0.1 then begin p.x:=0; exit; end;
     p.x:=sxd2+round((x-posx)*xyzscr/yy);
     p.y:=syd2-round((z-posz)*xyzscr/yy);
end;

procedure Line3d(x1,y1,z1,x2,y2,z2:f_x_t);
var
p1,p2:tpoint2d;
begin
     DoPointS(x1,y1,z1,p1);
     DoPointS(x2,y2,z2,p2);
     if (p1.x<>0) and (p2.x<>0) then
     Line(p1.x,p1.y,p2.x,p2.y);
end;

procedure Draw_OXYZ;
const
lw=0.05;
var
i:integer;
begin
     SetColor(maxc-1);
     Line3d(0,-oxyz_y,0,0,oxyz_y,0);
     Line3d(-oxyz_x,0,0,oxyz_x,0,0);
     Line3d(0,0,-oxyz_z,0,0,oxyz_z);
     for i:=round(-oxyz_x) to round(oxyz_x) do Line3d(i,0,lw,i,0,-lw);
     for i:=round(-oxyz_y) to round(oxyz_y) do Line3d(lw,i,0,-lw,i,0);
     for i:=round(-oxyz_z) to round(oxyz_z) do Line3d(lw,0,i,-lw,0,i);
end;

(*procedure Draw_3df_xy(f:tf_xy);
function sgn(a:f_x_t) : f_x_t;
begin
     if a>0 then sgn:=1 else sgn:=-1;
end;
const
maxb=400;
var
buf_z:array[0..maxb] of f_x_t;
x,y,z,pz:f_x_t;
i:word;
col:byte;
allcl,clit:f_x_t;
oxyz_zn:f_x_t;
begin
     col:=2;
     SetColor(col);
     allcl:=2*oxyz_y / 15;
     clit:=0;
     oxyz_zn:=oxyz_z*5;
     {1}
     x:=-oxyz_x;
     y:=oxyz_y;
     i:=0;
     while (x<oxyz_x) do
     begin
          z:=f(x,y);                    {z1}
          if (abs(z)>oxyz_zn) then z:=sgn(z)*oxyz_zn;
          buf_z[i]:=z;
          inc(i);
          x:=x+delta_xyz;
     end;
     y:=y-delta_xyz;
     {2-glowna petla}
     while (y>-oxyz_y) do
     begin
          x:=-oxyz_x;
          i:=1;
          pz:=f(x,y);
          if (abs(pz)>oxyz_zn) then pz:=sgn(pz)*oxyz_zn;
          x:=x+delta_xyz;
          while (x<oxyz_x) do
          begin
               z:=f(x,y);               {z2}
               if (abs(z)>oxyz_zn) then z:=sgn(z)*oxyz_zn;
               {(x1,y2)->(x2,y2)}
               Line3d(x-delta_xyz,y-delta_xyz,pz,x,y-delta_xyz,z);
               {(x1,y1)->(x1,y2)}
               Line3d(x-delta_xyz,y,buf_z[i-1],x-delta_xyz,y-delta_xyz,pz);
               {(x1,y1)->(x2,y1)}
               Line3d(x-delta_xyz,y,buf_z[i-1],x,y,buf_z[i]);
               {(x2,y1)->(x2,y2)}
               Line3d(x,y,buf_z[i],x,y-delta_xyz,z);
               buf_z[i-1]:=pz;
               pz:=z;
               inc(i);
               x:=x+delta_xyz;
          end;
          buf_z[i-1]:=z;
          y:=y-delta_xyz;
          clit:=clit+delta_xyz;
          if clit >= allcl then
          begin
               inc(col);
               SetColor(col);
               clit:=0;
          end;
          if keypressed then begin readkey; exit; end;
     end;
end;*)

procedure Draw_3df_xy(f:tf_xy);
function sgn(a:f_x_t) : f_x_t;
begin
     if a>0 then sgn:=1 else sgn:=-1;
end;
var
x,y,z,pz:f_x_t;
i:word;
allcl,clit:f_x_t;
oxyz_zn:f_x_t;
begin
     SetColor(startc);
     allcl:=(maxc-startc-1)/(2*oxyz_y);
     clit:=0;
     oxyz_zn:=oxyz_z*5;

     {Inicjacja}
     x:=-oxyz_x;
     y:=oxyz_y;
     i:=0;
     while (x<oxyz_x) do
     begin
          z:=f(x,y);
          if (abs(z)>oxyz_zn) then z:=sgn(z)*oxyz_zn;
          buf_z[i]:=z;
          inc(i);
          x:=x+delta_xyz;
     end;

     {1 linia}
     y:=y-delta_xyz;

     {1.1}
     i:=1;
     x:=-oxyz_x;
     pz:=f(x,y);
     if (abs(pz)>oxyz_zn) then pz:=sgn(pz)*oxyz_zn;
     x:=x+delta_xyz;
     z:=f(x,y);
     if (abs(z)>oxyz_zn) then z:=sgn(z)*oxyz_zn;
     {(x1,y2)->(x2,y2)}
     Line3d(x-delta_xyz,y-delta_xyz,pz,x,y-delta_xyz,z);
     {(x1,y1)->(x1,y2)}
     Line3d(x-delta_xyz,y,buf_z[i-1],x-delta_xyz,y-delta_xyz,pz);
     {(x1,y1)->(x2,y1)}
     Line3d(x-delta_xyz,y,buf_z[i-1],x,y,buf_z[i]);
     {(x2,y1)->(x2,y2)}
     Line3d(x,y,buf_z[i],x,y-delta_xyz,z);
     buf_z[i-1]:=pz;
     pz:=z;
     inc(i);
     x:=x+delta_xyz;

     {1.i}
     while (x<oxyz_x) do
     begin
          z:=f(x,y);
          if (abs(z)>oxyz_zn) then z:=sgn(z)*oxyz_zn;
          {(x1,y2)->(x2,y2)}
          Line3d(x-delta_xyz,y-delta_xyz,pz,x,y-delta_xyz,z);
          {(x1,y1)->(x2,y1)}
          Line3d(x-delta_xyz,y,buf_z[i-1],x,y,buf_z[i]);
          {(x2,y1)->(x2,y2)}
          Line3d(x,y,buf_z[i],x,y-delta_xyz,z);
          buf_z[i-1]:=pz;
          pz:=z;
          inc(i);
          x:=x+delta_xyz;
     end;

     {i-ta linia}
     y:=y-delta_xyz;
     while (y>-oxyz_y) do
     begin
          {i.1}
          x:=-oxyz_x;
          i:=1;
          pz:=f(x,y);
          if (abs(pz)>oxyz_zn) then pz:=sgn(pz)*oxyz_zn;
          x:=x+delta_xyz;
          z:=f(x,y);
          if (abs(z)>oxyz_zn) then z:=sgn(z)*oxyz_zn;
          {(x1,y2)->(x2,y2)}
          Line3d(x-delta_xyz,y-delta_xyz,pz,x,y-delta_xyz,z);
          {(x1,y1)->(x1,y2)}
          Line3d(x-delta_xyz,y,buf_z[i-1],x-delta_xyz,y-delta_xyz,pz);
          {(x2,y1)->(x2,y2)}
          Line3d(x,y,buf_z[i],x,y-delta_xyz,z);
          buf_z[i-1]:=pz;
          pz:=z;
          inc(i);
          x:=x+delta_xyz;

          {i.i}
          while (x<oxyz_x) do
          begin
               z:=f(x,y);
               if (abs(z)>oxyz_zn) then z:=sgn(z)*oxyz_zn;
               {(x1,y2)->(x2,y2)}
               Line3d(x-delta_xyz,y-delta_xyz,pz,x,y-delta_xyz,z);
               {(x2,y1)->(x2,y2)}
               Line3d(x,y,buf_z[i],x,y-delta_xyz,z);
               buf_z[i-1]:=pz;
               pz:=z;
               inc(i);
               x:=x+delta_xyz;
          end;
          buf_z[i-1]:=z;
          y:=y-delta_xyz;
          if y-posy<0 then break;
          clit:=clit+delta_xyz;
          SetColor(round(clit*allcl)+startc);
          {if clit >= allcl then
          begin
               inc(col,round(clit/allcl));
               SetColor(col);
               clit:=0;
          end;}
          if keypressed then begin readkey; exit; end;
     end;
end;

end.