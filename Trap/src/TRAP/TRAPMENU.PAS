{procedury do obslugi menu}

type

atxts = array [0..99] of pchar;
ptxts = ^atxts;

pmenu = ^tmenu;

tmenu = record
              menuid : word;
              menupoz : shortint;
              numoftxts : word;
              menutxts : ptxts;
              numofcoms : word;
              menucoms : ptxts;
              menutitle : pchar;
              menuhelp : pchar;
              prevmenu : pmenu;
              nextmenu : pmenu;
end;

procedure createmenu (var prev : pmenu; var newmenu : pmenu; mdata : tmenu);
begin
     new (newmenu);
     newmenu^ := mdata;
     newmenu^.prevmenu := nil;
     newmenu^.nextmenu := nil;
     newmenu^.menupoz := 0;
     if prev <> nil then prev^.nextmenu := newmenu;
end;

procedure gotoitem (var mstart : pmenu; mid : word);
var
p : pmenu;
begin
     p := mstart;
     while p <> nil do
     begin
          if p^.menuid = mid then break;
          p := p^.nextmenu;
     end;
     if p <> nil then
     begin
          p^.prevmenu := mstart;
          mstart := p;
     end;
end;

procedure returnmenu (var cmenu : pmenu);
var
p : pmenu;
begin
     p := cmenu;
     cmenu := cmenu^.prevmenu;
     p^.prevmenu := nil;
end;