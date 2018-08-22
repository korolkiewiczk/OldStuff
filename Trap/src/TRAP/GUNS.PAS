const
maxguns = 10;
maxammotypes = 4;
maxshotammo = 32;
gunsparts1 : tparticle = (onlife:true;x:0;y:0;xvel:0;yvel:0;color:127;ecolor:120;atime:0;etime:5);
gparts1n = 4;
gparts1mx = 520;
gparts1my = 500;
gparts1mc = 0;
gparts1mt = 5;
{}
gunsparts2 : tparticle = (onlife:true;x:0;y:0;xvel:0;yvel:0;color:95;ecolor:80;atime:0;etime:5);
gparts2n = 5;
gparts2mx = 6400;
gparts2my = 12800;
gparts2mc = 0;
gparts2mt = 3;
{}
gunsparts3 : tparticle = (onlife:true;x:0;y:0;xvel:0;yvel:0;color:127;ecolor:112;atime:0;etime:10);
gparts3n = 5;
gparts3mx = 20*256;
gparts3my = 10*256;
gparts3mc = 0;
gparts3mt = 0;

gun2ammotab : array [0..maxguns-1] of byte = (255,0,1,1,1,0,0,2,3,3);
ammo2ntab : array [0..maxguns-1] of shortint = (0,-1,-1,-2,-4,-1,-1,-1,-1,-1);


type

tguninfo = record
                 gapower : word;
                 gnammo : byte;
                 gtime : longint;
                 gtype : byte;
                 gatype : byte;
                 gextra : word;
end;

tguns = array [0..maxguns-1] of tguninfo;

tsammorec = record
               used : boolean;
               tsammoobj : tphysicobj;
               atype,gtype,igun : byte;
end;

tsammo = array [0..maxshotammo-1] of tsammorec;
