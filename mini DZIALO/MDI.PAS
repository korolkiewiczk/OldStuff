program mdinstall;
uses crt,graph;
const
pachmdso = '\in\md10.pak';
pachbgiso = '\in\egavga.pak';
pachreadmyso = '\in\readmy.pak';
pachmds = 'md.exe';
pachbgis = 'egavga.bgi';
pachwn = 'wyniki.ini';
pachreadmy = 'read.txt';
var
mds,bgis,mdso,bgiso : file of char;
klawisz : char;
wn,readmy,readmyso : text;
dir : string;
karta,mode : integer;
i : integer;
x : integer;
procedure f;
begin
     gotoxy (x,6);
end;
procedure inf;
begin
     gotoxy (6,5);
end;
procedure drww;
var
i : integer;
begin
     textbackground (4);
     textcolor (4);
     for i := 1 to 2000 do write (chr (177);
end;
begin
     clrscr;
     drww;
     textcolor (0);
     gotoxy (1,2);
     write ('Program instalacyjny gry miniDZIA�O':25);
     gotoxy (1,4);
     write ('Czy chesz zaintalowa� gr� miniDZIA�O? (t/n)':20);
     repeat
     klawisz := #0;
     klawisz := readkey;
     if klawisz := 'n' then halt;
     until klawisz := 't';
     gotoxy (1,6);
     writeln ('Oto kilka wskazuwek dotycz�cych instalacji');
     writeln ('1.Najlepiej je�eli katalogiem gdzie b�dzie si� mie�ci� gra');
     writeln ('to [c:\md]');
     writeln ('2.Mo�esz wybra� oczywi�cie inny katalog ale powinien on by�');
     writeln ('zupe�nie nowy');
     gotoxy (1,8);
     writeln ('Wci�nij dowolny klawisz aby rozpocz�� program instalacyjny');
     klawisz := readkey;
     clrscr;
     drww;
     window (20,10,70,17);
     for i := 1 to 1000 do write (' ');
     gotoxy (1,1);
     writeln ('Czy chcesz zainstalowa� gr� w katalogu [c:\md] (t\n)');
     repeat
     klawisz := #0;
     klawisz := readkey;
     if klawisz = 't' then
     begin
          write ('Podaj nazw� katalogu : ');
          read (dir);
     end;
     if klawisz = 'n' then dir := 'c:\md';
     until (klawisz = 't') or (klawisz = 'n');
     write ('Naci�nij dowolny klawisz aby rozpocz�� instalacj�');
     klawisz = readkey;
     clrscr;
     for i := 1 to 1000 do write (' ');
     detectgraph (karta,mode);
     if karta <> 9 then
     begin
          clrscr;
          write ('Niestety tw�j komputer nie ma karty VGA i gra nie mo�e
                dobrz� dzia�a�.Dlatego instalacja zosta�a przerwana.');
          readln;
          halt;
     end;
     x := 2;
     f;
     textcolor (9);
     for i := 1 to 46 do write (chr (176));
     inf;
     write ('sprawdzanie odpowiednich plik�w na dysku');
