type

tphysicobj = record
                   physicobjpozx,physicobjpozy : floatint;
                   physicobjxvelocity,physicobjyvelocity : floatint;
                   physicobjmaxxvel,physicobjmaxyvel : floatint;
                   physicobjxaccel,physicobjyaccel : floatint;
end;

procedure update_physicobj (var physicobj : tphysicobj; step : floatint);
var
pvel : floatint;
begin
     with physicobj do
     begin
          pvel := physicobjxvelocity;
          floatint_mul (pvel,pvel,step);
          floatint_mul (pvel,pvel,1);
          floatint_add (physicobjpozx,physicobjpozx,pvel);
          {}
          pvel := physicobjyvelocity;
          floatint_mul (pvel,pvel,step);
          floatint_mul (pvel,pvel,1);
          floatint_add (physicobjpozy,physicobjpozy,pvel);
     end;
end;

procedure update_velocity (var physicobj : tphysicobj; xaccel,yaccel : floatint; step : floatint);
var
pacc : floatint;
begin
     with physicobj do
     begin
          pacc := xaccel;
          floatint_mul (pacc,pacc,step);
          floatint_mul (pacc,pacc,1);
          floatint_add (physicobjxvelocity,physicobjxvelocity,pacc);
          {}
          pacc := yaccel;
          floatint_mul (pacc,pacc,step);
          floatint_mul (pacc,pacc,1);
          floatint_add (physicobjyvelocity,physicobjyvelocity,pacc);
          {}
          if abs (physicobjxvelocity) > physicobjmaxxvel then
          if physicobjxvelocity >= 0 then
          physicobjxvelocity := physicobjmaxxvel
          else
          physicobjxvelocity := -physicobjmaxxvel;
          {}
          if abs (physicobjyvelocity) > physicobjmaxyvel then
          if physicobjyvelocity >= 0 then
          physicobjyvelocity := physicobjmaxyvel
          else
          physicobjyvelocity := -physicobjmaxyvel;
     end;
end;

procedure rnd_physicobj (var rx,ry : integer; var physicobj : tphysicobj);
begin
     with physicobj do
     begin
          floatint_rnd (rx,physicobjpozx);
          floatint_rnd (ry,physicobjpozy);
     end;
end;