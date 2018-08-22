uses pfile;
var
a : pfiler;
fi : fileor;
blka : array [1..60000] of char;
i : word;
z : char;
begin
     if not uspack (a,'pack.pak') then exit;
     assignpfile (fi,a,'vscr.pas');
     if pfileior <> 0 then
     begin
          writeln (getpfior);
          exit;
     end;
     resetpfile (fi);
     writeln (filepos (f));
     {blockreadpfile (fi,blka,pfilesize (fi));}
     while not pfileeof (fi) do
     begin
          blockreadpfile (fi,z,1);
          write (z);
     end;
     {for i := 1 to pfilesize (fi) do write (blka[i]);}
     writeln (pfilesize (fi));
     writeln (pfilepos (fi));
     seekpfile (fi,0);
     writeln (pfilepos (fi));
     writeln;
     writeln (a^[0].pfname);
     writeln;
     unuspack (a);
end.