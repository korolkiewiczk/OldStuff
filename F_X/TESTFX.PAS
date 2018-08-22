{$N+}
uses f_x,crt;

{log}
{function fx(x:f_x_t):f_x_t; far;
begin
     if (x>0) and (x<>1) then
     fx:=1/ln(x) else fx:=UD;
end;

function gx(x:f_x_t):f_x_t; far;
begin
     if (x>0) then
     gx:=ln(x) else gx:=UD;
end;

function hx(x:f_x_t):f_x_t; far;
begin
     if x<>0 then
     hx:=exp(1/x);
end;}

function fx(x:f_x_t):f_x_t; far;
begin
     fx:=UD;
     fx:=x+2;
end;

function gx(x:f_x_t):f_x_t; far;
begin
     gx:=UD;
     if (x<>0) then
     gx:=1/x;
end;

function hx(x:f_x_t):f_x_t; far;
begin
     if cos(x)<>0 then
     hx:=sin(x)/cos(x);
end;

const

f : tf_x=fx;
g : tf_x=gx;
h : tf_x=hx;

begin
     if (not Init_f_x(2)) then begin writeln ('blad'); exit; end;
     oxy_x:=oxy_x;
     oxy_y:=oxy_y;
     gcol:=15;
     Draw_f_x(f);
     gcol:=7;
     Draw_df_x(f);
     gcol:=8;
     Draw_fd_x(f);
     {gcol:=7;
     Draw_f_x(g);
     gcol:=2;
     Draw_df_x(g);
     gcol:=3;
     Draw_f_x(h);
     gcol:=4;
     Draw_df_x(h);}
     readkey;
     Close_f_x;
end.