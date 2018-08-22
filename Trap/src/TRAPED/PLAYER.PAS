type

tplayerlink = record
                    mx : shortint;
                    my : shortint;
end;

const

nshifts1 = 10;
playershifts1 : array [0..nshifts1-1] of byte = (0,0,0,0,0,0,0,0,0,0);

nanims = 14;
directiontab : array [0..nanims-1] of shortint = (1,-1,1,1,-1,-1,1,-1,1,1,-1,-1,1,1);

nplayerlinks = 28;

var
playerlinks : array [0..nplayerlinks-1] of tplayerlink;