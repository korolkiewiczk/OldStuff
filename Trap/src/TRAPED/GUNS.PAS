const
maxguns = 10;
maxammotypes = 4;

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