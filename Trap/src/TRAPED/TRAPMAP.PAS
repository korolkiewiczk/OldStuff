{stale}
const
trapmapsize = 65535 div 6;
maxmonsters = 256;
maxplayers = 1;
maxinforecs = 249;
scryptslen = 10000;

type

{record poziomu}
trapmaprecord = record
                      backtex : byte;
                      fronttex1 : byte;
                      fronttex2 : byte;
                      fadeandwater : byte;
                      trapinforec1 : byte;
                      trapinforec2 : byte;
end;
strapmap = array [0..trapmapsize-1] of trapmaprecord;
ptrapmap = ^strapmap;
{-----------------------------------------------}
{przeciwnik}
monsterrec = record
                   monsterpozx,monsterpozy : word;
                   state : byte;
                   monstertype : byte;
end;
tmonsters = array [0..maxmonsters-1] of monsterrec;
{-----------------------------------------------}
{gracz}
playerrec = record
                  playerpozx,playerpozy : word;
                  health : word;
                  state : byte;
                  igun : byte;
                  playerguns : array [0..maxguns-1] of boolean;
                  playerammo : array [0..maxguns-1] of byte;
                  watertime : word;
end;
tplayers = array [0..maxplayers-1] of playerrec;
{-----------------------------------------------}
{poziom}
trapmapdesc = record
                    trapmapsizex,trapmapsizey : word;
                    trapinforecseg : word;
                    trapscryptseg : word;
                    nplayers : word;
                    players : tplayers; {1}
                    nmonsters : word;
                    monsters : tmonsters; {256}
                    trapmap : ptrapmap;
end;
{-----------------------------------------------}
trapinforec = record
                    objectpozx,objectpozy : byte;
                    objectscrypt : word;
                    objectstate : byte;
                    objectextra : byte;
end;
{-----------------------------------------------}
strapscrypts = array [0..scryptslen-1] of byte;
ptrapscrypts = ^strapscrypts;