program standart;
uses crt,dos;
type
tst = record
    gs,ms,ss,tss,gk,mk,sk,tsk : word;
end;
var
klawisz : char;
pau,paupr,pky,am,sam : word;
stgw,stpr : tst;
gw : array [1..79,3..23] of byte;
br,st,ols,colo : byte;
zyje : boolean;
strp,strl : boolean;
xk,oldxk,yk,oldyk : byte;
xkp,oldxkp,ykp,oldykp : byte;
xm,oldxm,xg,oldxg : byte;

procedure _write (x,y : byte; a : string);
begin
     gotoxy (x,y);
     write (a);
end;

procedure stoper_r (a : tst);
begin
     with a do
     begin
          gs := 0;
          ms := 0;
          ss := 0;
          tss := 0;
          gk := 0;
          mk := 0;
          sk := 0;
          tsk := 0;
     end;
end;

procedure stoper_on (a : tst);
begin
     with a do gettime (gs,ms,ss,tss);
end;

procedure stoper_off (a : tst; var gt : word);
begin
     with a do
     begin
          gettime (gk,mk,sk,tsk);
          gt := ((gk-gs)*360000)+((mk-ms)*6000)+((sk-ss)*10)+((tsk-tss));
     end;
end;

procedure mkgw;
var
i,j : integer;
begin
     for j := 3 to 23 do
         for i := 1 to 79 do
             gw[i,j] := random (160);
end;

procedure skgw;
var
i,j : integer;
begin
     for j := 3 to 23 do
         for i := 1 to 79 do
             if gw[i,j] = 1 then _write (i,j,#15)
                                 else
                                 _write (i,j,' ');
end;

procedure off;
begin
asm
   mov ah,  1
   mov ch, 16
   mov cl,  0
   int 10h
end;
end;

function keyread : char;
begin
     keyread := #0;
     if keypressed then keyread := readkey;
end;

procedure koniec;
var
i : integer;
begin
     clrscr;
     for i := 1 to 2000 do write (#219);
     _write (20,11,'standart v.1.0 (c) copyright Kamil Korolkiewicz');
     halt;
end;

procedure opcje;
var
ssam : string;
w,ow : byte;
begin
     clrscr;
     w := 1;
     repeat
     _write (38,3,'Opcje');
     _write (38,11,'Kolory');
     _write (37,12,'Amunicja');
     _write (38,13,'Powr¢t');
     _write (35,ow+10,' ');
     _write (35,w+10,#9);
     _write (46,ow+10,' ');
     _write (46,w+10,#9);
     repeat
           ow := w;
           klawisz := upcase (readkey);
           case klawisz of
                #72 : dec (w);
                #80 : inc (w);
           end;
           if w = 0 then w := 3;
           if w = 4 then w := 1;
           sound (900);
           delay (3);
           nosound;
           _write (35,ow+10,' ');
           _write (35,w+10,#9);
           _write (46,ow+10,' ');
           _write (46,w+10,#9);
     until klawisz = #13;
     sound (1000);
     delay (10);
     nosound;
     case w of
     1 : begin
              clrscr;
              _write (37,2,'Kolory');
              _write (20,12,'Podaj numer nowego koloru gry(0-15): ');
              readln (colo);
              if colo > 15 then colo := 15;
              textcolor (colo);
              clrscr;
         end;
     2 : begin
              clrscr;
              str (sam,ssam);
              _write (36,2,'Amunicja');
              _write (15,11,'Obecna ilo˜† amunicji na pocz¥tku gry to : '+ssam);
              _write (17,12,'Podaj now¥ ilo˜† amunicji na pocz¥tku : ');
              readln (sam);
              if sam > 500 then sam := 500;
              clrscr;
         end;
     end;
     until w = 3;
     clrscr;
end;

procedure oautorze;
begin
     clrscr;
     _write (37,3,'O autorze');
     _write (1,5,'Standart jest szczelanin¥ kosmiczn¥ czasu rzeczywistego');
     _write (1,7,'Tw¢rca gry standart :');
     _write (1,8,#9+' Pomysˆ : Kamil Korolkiewicz');
     _write (1,9,#9+' Engine gry : Kamil Korolkiewicz');
     _write (1,10,#9+' Grafika : Kamil Korolkiewicz');
     _write (1,12,'Standart jest gr¥ freeware');
     readkey;
     clrscr;
end;

procedure czytajto;
begin
     clrscr;
     _write (37,3,'Czytaj to');
     gotoxy (1,5);
     writeln ('Cel gry to zdobycie jak najwi©ksze liczby puntkt¢w.Gra koäczy');
     writeln ('si© wtedy kiedy zginiemy,stracimu ju¾ caˆa amunicje lub naci˜niemy');
     writeln ('klawisz escape.Mo¾emy sobie r¢wnie¾ zmieni† liczb© amunicji zmieniaj¥c');
     writeln ('jej pocz¥tkow¥ warto˜† w opcjach');
     writeln;
     writeln ('Bronie s¥ trzy i charakteryzuj¥ si© inn¥ szybko˜ci¥ lotu kuli.Im mamy lepsz¥');
     writeln ('broä tym mo¾emy szybciej strzela† do przeciwnika i jednocze˜nie przeciwnik ma');
     writeln ('mniejsze szanse na unik');
     writeln;
     writeln ('Za zabicie przeciwnika otrzymujemy 1 punkt.Kiedy zdob©dziemy 10,20 lub');
     writeln ('30 punkt¢w otrzymujemy lepsz¥ broä.');
     writeln;
     writeln ('Przeciwnik jes praktycznie jedyn¥ rzecz¥ przez kt¢r¥ mo¾emy straci† ¾ycie i');
     writeln ('jednocze˜nie skoäczy† gr©.');
     writeln;
     writeln ('Pami©taj ¾e gra si© toczy w czasie rzeczywistym i nigdy');
     writeln ('nie wiadomo kiedy przeciwnik odda strzaˆ.');
     writeln;
     textcolor (colo+16);
     writeln ('Naci˜nij dowolny klawisz');
     textcolor (colo);
     readkey;
     clrscr;
     writeln ('Klawiszologia : ');
     _write (1,3,'q - wy˜cie z rozgrywki');
     _write (1,4,'ENTER - potwierdzenie wyboru');
     _write (1,5,#16+' - przesuä statek kosmiczny w prawo');
     _write (1,6,#17+' - przesuä statek kosmiczny w lewo');
     _write (1,7,'SPACE - strzaˆ');
     _write (1,8,'kursory - poruszanie si© po menu');
     readkey;
     clrscr;
end;

procedure strz;
begin
     strl := true;
     yk := 23;
     xk := xm;
     oldyk := yk;
     oldxk := xm;
     dec (am);
end;

procedure strzp;
begin
     strp := true;
     ykp := 3;
     xkp := xg;
     oldykp := yk;
     oldxkp := xg;
end;

procedure gra;
var
time1,time2 : word;
begin
     paupr := 20;
     pau := 10;
     zyje := true;
     pky := 0;
     br := 1;
     am := sam;
     xm := 40;
     xg := random (78)+1;
     zyje := true;
     clrscr;
     mkgw;
     skgw;
     stoper_r (stgw);
     _write (xm,24,#219);
     repeat
           stoper_on (stpr);
           stoper_on (stgw);
           oldxm := xm;
           oldxg := xg;
           if keypressed then
           begin
           klawisz := readkey;
           case klawisz of
           #0 :
                begin
                     klawisz := readkey;
                     case klawisz of
                     #75 : dec (xm);
                     #77 : inc (xm);
                     end;
                end;
           #32 : if am > 0 then strz;
           'q' : break;
           end;
           if xm <= 0 then xm := 1;
           if xm >= 79 then xm := 78;
           _write (oldxm,24,' ');
           _write (xm,24,#219);
           end;
           if (strl) and (am > 0) then
           begin
                oldyk := yk;
                dec (yk);
                sound (yk*10);
                delay (pau);
                _write (xk,oldyk,' ');
                _write (xk,yk,#24);
                if yk = 2 then
                begin
                     nosound;
                     strl := false;
                     _write (xk,2,' ');
                end;
                if (xk = xg) and (yk = 2) then
                begin
                     inc (am);
                     inc (pky);
                     stoper_r (stpr);
                     sound (1000);
                     delay (200);
                     nosound;
                     _write (xg,2,' ');
                     xg := random (78)+1;
                     if pky mod 10 = 0 then inc (br);
                     if pky mod 10 = 0 then dec (paupr);
                     if paupr <= 0 then paupr := 1;
                     case br of
                          1 : pau := 10;
                          2 : pau := 5;
                          3 : pau := 1;
                          else pau := 1;
                     end
                end;
                if br > 3 then br := 3;
           end;
           stoper_off (stgw,time2);
           if (time2 mod 10 = 0) then
           begin
                mkgw;
                skgw;
           end;
           stoper_off (stpr,time1);
           if time1 mod 10 = 0 then
           begin
                oldxg := xg;
                if random (2) = 0 then inc (xg)
                else
                dec (xg);
                if xg <= 0 then xg := 1;
                if xg >= 79 then xg := 78;
                _write (oldxg,2,' ');
                _write (xg,2,#219);
           end;
           if time1 mod 50 = 0 then strzp;
           if strp then
           begin
                oldykp := ykp;
                inc (ykp);
                sound (ykp*10);
                delay (paupr);
                _write (xkp,oldykp,' ');
                _write (xkp,ykp,#25);
                if ykp = 24 then
                begin
                     nosound;
                     strp := false;
                     _write (xkp,24,' ');
                end;
                if (xkp = xm) and (ykp = 24) then
                begin
                     zyje := false;
                     sound (700);
                     delay (200);
                     sound (450);
                     delay (200);
                     sound (260);
                     delay (200);
                     nosound;
                     _write (xm,24,' ');
                     delay (1000);
                     clrscr;
                     while keypressed do readkey;
                     _write (35,12,'Koniec gry');
                     readkey;
                end;
           end;
           gotoxy (1,1);
           write ('Punkty : ',pky,'           Broä : ',br,'          Amunicja : ',am);
     until (klawisz = 'q') or (not zyje) or (am < 1);
     clrscr;
     gotoxy (19,12);
     if (pky > 4) and (pky < 200) then
     write ('Nie«le.W tej rozgrywce zdobyˆe˜ : ',pky,' punkt¢w');
     if pky < 5 then
     write ('Sˆabo.W tej rozgrywce zdobyˆe˜ zaledwie : ',pky,' punkt¢w');
     if pky > 199 then
     begin
     gotoxy (11,12);
     write ('Nowy mistrzu gry standart!W tej rozgrywce zdobyˆe˜ a¾ : ',pky,' punkt¢w');
     end;
     readkey;
     clrscr;
     nosound;
end;

begin
     colo := 7;
     textcolor (colo);
     randomize;
     sam := 500;
     am := sam;
     clrscr;
     off;
     st := 1;
     repeat
     _write (38,3,'Menu');
     _write (39,9,'Gra');
     _write (36,10,'Czytaj to');
     _write (36,11,'O autorze');
     _write (38,12,'Opcje');
     _write (37,13,'Koniec');
     repeat
           _write (34,ols+8,' ');
           _write (34,st+8,#9);
           _write (46,ols+8,' ');
           _write (46,st+8,#9);
           ols := st;
           klawisz := upcase (readkey);
           case klawisz of
                #72 : dec (st);
                #80 : inc (st);
           end;
           if (klawisz = #72) or (klawisz = #80) then
           begin
                sound (900);
                delay (3);
                nosound;
           end;
           if st = 0 then st := 5;
           if st = 6 then st := 1;
     until klawisz = #13;
     sound (1000);
     delay (10);
     nosound;
     case st of
          1 : gra;
          2 : czytajto;
          3 : oautorze;
          4 : opcje;
          5 : koniec;
     end;
     until st=5;
end.