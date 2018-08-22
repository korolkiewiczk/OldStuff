{podstawowe procedury i typy danych dotyczace animacji}
type

tptrttex = ^ttex;
taptrttex = array [0..255] of tptrttex;

ranimation = record
                   isspr : byte;
                   numofanims : byte;
                   curanim : byte;
                   animtime : word;
                   animationstex : ^taptrttex;
end;

tanimation = ^ranimation;

function getanimation (anim : tanimation) : tptrttex;
begin
     getanimation := anim^.animationstex^[anim^.curanim];
end;

procedure createanimation (var newanim : tanimation; spr : byte; numofanimsp : byte; animtm : word);
begin
     getmem (newanim,sizeof (ranimation));
     getmem (newanim^.animationstex,numofanimsp*4);
     newanim^.isspr := spr;
     newanim^.numofanims := numofanimsp;
     newanim^.curanim := 0;
     newanim^.animtime := animtm;
end;

procedure addtextoanim (var t : ttex; n : byte; var anim : tanimation);
begin
     anim^.animationstex^[n] := @t;
end;

procedure setcuranim (var anim : tanimation; curr : byte);
begin
     if curr >= anim^.numofanims then curr := 0;
     anim^.curanim := curr;
end;