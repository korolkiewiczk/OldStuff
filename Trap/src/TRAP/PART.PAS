const

maxparticles = 6;
maxparts = 75;

type

tparticle = record
                  onlife : boolean;
                  x,y : floatint;
                  xvel,yvel : floatint;
                  color,ecolor : byte;
                  atime,etime : longint;
end;

tparticles = array [0..maxparticles-1] of tparticle;

procedure initparticles (var part : tparticles; pozx,pozy,xv,yv : floatint;
col,ecol,ptime : longint; n : byte; vxmod,vymod,cmod,tmod : word);
var
i : byte;
pv : floatint;
pc : byte;
pt : longint;
begin
     if n >= maxparticles then exit;
     for i := 0 to n do
     begin
          part[i].onlife := true;
          part[i].x := pozx;
          part[i].y := pozy;
          pv := xv+(longint (random (vxmod))-vxmod shr 1);
          part[i].xvel := pv;
          pv := yv+(longint (random (vymod))-vymod shr 1);
          part[i].yvel := pv;
          part[i].color := col;
          pc := ecol+(longint (random (cmod))-cmod shr 1);
          part[i].ecolor := pc;
          part[i].atime := clock;
          pt := part[i].atime+ptime+(longint (random (tmod))-tmod shr 1);
          part[i].etime := pt;
     end;
end;

procedure updateparticles (var part : tparticles; n : byte; step : floatint);
var
i : byte;
pvel : floatint;
begin
     if n >= maxparticles then exit;
     for i := 0 to n do
     begin
          if part[i].onlife=false then continue;
          pvel := part[i].xvel;
          floatint_mul (pvel,pvel,step);
          floatint_mul (pvel,pvel,1);
          floatint_add (part[i].x,part[i].x,pvel);
          {}
          pvel := part[i].yvel;
          floatint_mul (pvel,pvel,step);
          floatint_mul (pvel,pvel,1);
          floatint_add (part[i].y,part[i].y,pvel);
          {}
          if part[i].etime < clock then part[i].onlife := false;
     end;
end;

procedure drawparticles (px,py : integer; var part : tparticles; n : byte);
var
i : byte;
pv,pv2,dc : floatint;
begin
     if n >= maxparticles then exit;
     for i := 0 to n do
     begin
          if part[i].onlife=false then continue;
          floatint_sub (pv,part[i].etime,part[i].atime);
          floatint_sub (pv2,clock,part[i].atime);
          if pv = 0 then pv := 1;
          pv := (pv2 shl 8) div pv;
          dc := part[i].ecolor-part[i].color;
          pv := (pv*dc) div 256;
          {}
          putpixeltranspar (part[i].x shr 8-px,part[i].y shr 8-py,pv+part[i].color);
     end;
end;