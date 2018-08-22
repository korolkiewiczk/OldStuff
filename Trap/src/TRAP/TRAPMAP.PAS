{stale}
const
trapmapsize = 65535 div 6;
maxmonsters = 256;
maxplayers = 1;
maxinforecs = 249;
scriptslen = 10000;

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
ttmonsterrec = record
                 maxhealth : word;
                 maxvel : floatint;
                 gun : word;
                 monsteranimations : tanimation;
end;

tmonsterrec = record
                    pozx,pozy : floatint;
                    vel : floatint;
                    health : word;
                    mtype : byte;
                    state : byte;
                    xstate : word;
end;

ttmonsters = array [0..maxtypedmonsters-1] of ttmonsterrec;
tmonsters = array [0..maxmonstersonmap-1] of tmonsterrec;
{-----------------------------------------------}
{gracz}
playerrec = record
                  playerpozx,playerpozy : word;
                  health : integer;
                  state : byte;
                  igun : byte;
                  playerguns : array [0..maxguns-1] of boolean;
                  playerammo : array [0..maxammotypes-1] of word;
                  watertime : longint;
end;
tplayers = array [0..maxplayers-1] of playerrec;
{-----------------------------------------------}
{poziom}
trapmapdesc = record
                    trapmapsizex,trapmapsizey : word;
                    trapinforecseg : word;
                    trapscriptseg : word;
                    nplayers : word;
                    players : tplayers; {1}
                    nmonsters : word;
                    monsters : tmonsters; {256}
                    trapmap : ptrapmap;
end;
{-----------------------------------------------}
trapinforec = record
                    objectpozx,objectpozy : byte;
                    objectscript : word;
                    objectstate : byte;
                    objectextra : byte;
end;
{-----------------------------------------------}
strapscripts = array [0..scriptslen-1] of byte;
ptrapscripts = ^strapscripts;

tinforecs = array [0..maxinforecs-1] of trapinforec;
pinforecs = ^tinforecs;