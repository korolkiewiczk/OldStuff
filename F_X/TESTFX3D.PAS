{$N+}
uses f_x,crt,graph;

function fxy(x,y:f_x_t):f_x_t; far;
const
r=7;
begin
     if abs(x*x+y*y)<=r*r then
     fxy:=sqrt(abs(r*r-x*x-y*y))
     else
     fxy:=0;
end;

function gxy(x,y:f_x_t):f_x_t;far;
begin
     x:=abs(x);
     y:=abs(y);
     if (x<>0) and (y<>0) then
     gxy:=ln(x)+ln(y)
     else
     gxy:=0;
end;

function hxy(x,y:f_x_t):f_x_t; far;
var
z:f_x_t;
begin
     if x>0 then z:=1 else z:=-1;
     hxy:=z*sqrt(abs(x))+(random(round(abs(sin(x+y))*500))/1000)
end;

function ixy(x,y:f_x_t):f_x_t; far;
begin
     {ixy:=sin(x)+cos(y);}
     ixy:=cos(sqrt(x*x+y*y));
end;

function jxy(x,y:f_x_t):f_x_t; far;
begin
     if (abs(x)>1) then
     jxy:={sin(x)+cos(y);cos(sin(sqrt(x*x+y*y)));}(1/x)
     else
     jxy:=-(y*y)+10;
end;

const

f:tf_xy=fxy;
g:tf_xy=gxy;
h:tf_xy=hxy;
i:tf_xy=ixy;
j:tf_xy=jxy;

deltap=1;
var
licz:byte;
c:char;
begin
     randomize;
     if not Init_f_x(2) then exit;
     Init3df_xy;
     {for i:=0 to maxc-1 do setcol(i,i*(64 div maxc),i*(64 div maxc),i*(64 div maxc));}
     oxyz_x:=10;
     oxyz_y:=10;
     oxyz_z:=10;
     delta_xyz:=0.5;
     posx:=5;
     posy:=-oxyz_y-10;
     posz:=4;
     Draw_OXYZ;
{     readkey;}
     while true do
     begin
          ClearDevice;
          {Draw_OXYZ;}
          Draw_3df_xy(i);   {parametr okresla nazwe funkcji}
          c:=readkey;
          case c of
               'w':posy:=posy+deltap;
               's':posy:=posy-deltap;
               'a':posx:=posx-deltap;
               'd':posx:=posx+deltap;
               'e':posz:=posz+deltap;
               'q':posz:=posz-deltap;
          end;
          if c=#27 then break;
     end;
     for licz:=0 to maxc-1 do
     begin
          SetColor(licz);
          Line(0,licz*2,10,licz*2);Line(0,licz*2+1,10,licz*2+1);
     end;
     readkey;
     Close_f_x;
end.