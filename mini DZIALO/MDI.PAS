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
     write ('Program instalacyjny gry miniDZIAùO':25);
     gotoxy (1,4);
     write ('Czy chesz zaintalowaÜ gr© miniDZIAùO? (t/n)':20);
     repeat
     klawisz := #0;
     klawisz := readkey;
     if klawisz := 'n' then halt;
     until klawisz := 't';
     gotoxy (1,6);
     writeln ('Oto kilka wskazuwek dotycz•cych instalacji');
     writeln ('1.Najlepiej jeæeli katalogiem gdzie b©dzie si© mieòciÜ gra');
     writeln ('to [c:\md]');
     writeln ('2.Moæesz wybraÜ oczywiòcie inny katalog ale powinien on byÜ');
     writeln ('zupeànie nowy');
     gotoxy (1,8);
     writeln ('Wciònij dowolny klawisz aby rozpocz•Ü program instalacyjny');
     klawisz := readkey;
     clrscr;
     drww;
     window (20,10,70,17);
     for i := 1 to 1000 do write (' ');
     gotoxy (1,1);
     writeln ('Czy chcesz zainstalowaÜ gr© w katalogu [c:\md] (t\n)');
     repeat
     klawisz := #0;
     klawisz := readkey;
     if klawisz = 't' then
     begin
          write ('Podaj nazw© katalogu : ');
          read (dir);
     end;
     if klawisz = 'n' then dir := 'c:\md';
     until (klawisz = 't') or (klawisz = 'n');
     write ('Naciònij dowolny klawisz aby rozpocz•Ü instalacj©');
     klawisz = readkey;
     clrscr;
     for i := 1 to 1000 do write (' ');
     detectgraph (karta,mode);
     if karta <> 9 then
     begin
          clrscr;
          write ('Niestety tw¢j komputer nie ma karty VGA i gra nie moæe
                dobrz© dziaàaÜ.Dlatego instalacja zostaàa przerwana.');
          readln;
          halt;
     end;
     x := 2;
     f;
     textcolor (9);
     for i := 1 to 46 do write (chr (176));
     inf;
     write ('sprawdzanie odpowiednich plik¢w na dysku');
