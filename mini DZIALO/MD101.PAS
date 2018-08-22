                        {miniDZIAùO v 1.01}
                {gra w kt¢rej sterujemy k•tem i sià• strzaàu czoàgu}
                                 {Uwagi i pomoc}
{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
{
oto wyjaònienia do niekt¢rych zmiennych i inne waæne uwagi dotycz•ce nimiejszego kodu
´r¢dàowego

zmienne:
pau - pauza

procedury
sng - sygnaturka tw¢rcy gry (a raczej po prostu autor)
zwpam - zwolnienie pami©ci przez komputer i wyà•czenie trybu graf
r - losowe wybieranie pozycji strartowej:
gracza (dm)
przeciwnika (dp)
i wiatru kt¢ry nam troch© utrudnia gr©
{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
{awin - okno kt¢re widzmy na pocz•tku
mainmenu - menu gà
pt - wybieramy ptr¢d.
opr¢cz tego jest jeszcze oczywiòcie wiele innych waæniejszycz procedur
UWAGA przed rozpocz©ciem gry zalecam trzeczytaÜ plik READ.txt
æycze miàej zabawy
mini DZIAùO v.1.01 - ulepszenia*
     -"wait for vertical retrace" - procedura niweluj•ca efekt skaka‰ obrazu}
program minidzialo;
uses crt,graph,ssound;
const
maxx = 640;
maxy = 480;
pach = '';
maxwyni = 100;
graw = 0.4;
var
wyniki : text;
dx,dy : real;
wiatr : integer;
dxi,dyi : integer;
dp,dm : integer;
karta,mode : integer;
poziomtr,i,j,start,pora,tury,mozliwosci : integer;
lk,ls,sila,kat : integer;
vy,vx,vs,t,c,s : real;
koniec,konieccaly,trafiles,przegrana,trafiono : boolean;
klawisz : char;
tankm,tankp,kulaa,wybucha : pointer;
memory : word;
wynik : integer;
wyniks : array [1..maxwyni] of string;
wynikinteger : array [1..maxwyni] of integer;
wynikst : string;
procedure mainmenu;
forward;

procedure WFR;
begin
repeat until (Port[$3DA] and 8<>0);
repeat until (Port[$3DA] and 8=0);
end;

procedure sng;
begin
     window (1,1,80,25);
     textbackground (0);
     writeln;
     writeln;
     writeln;
     textcolor (15);
     for i := 1 to 2000 do write (chr (177));
     window (20,10,70,18);
     textcolor (0);
     i := 7;
     textbackground (i);
     write (chr (218)); for i := 1 to 41 do write (chr(196)); writeln (chr(191));
     write (chr (179),'           mini DZIAùO v. 1.01           ',chr (179));
     writeln (chr(177));
     write (chr (179),'  (c) Copyright Kamil Korolkiewicz 2002  ',chr (179));
     writeln (chr(177));
     write (chr (179),'             DZI®KUJ® ZA GR®             ',chr (179));
     writeln (chr(177));
     write (chr (192)); for i := 1 to 41 do write (chr(196)); write (chr(217)); writeln (chr(177));
     gotoxy (2,wherey); for i := 1 to 43 do write (chr(177));
end;
procedure zwpam;
begin
     memory := imagesize (1,1,15,10);
     freemem (tankm,memory);
     freemem (tankp,memory);
     memory := imagesize (1,1,4,4);
     freemem (kulaa,memory);
     memory := imagesize (1,1,10,10);
     freemem (wybucha,memory);
     closegraph;
end;
procedure error (zwomemory,e : integer);
begin
     case zwomemory of
          1 : begin
              zwpam;
              write ('bà•d nr. ');
              case e of
                   1 : write ('1:brak karty graficznej');
                   2 : write ('2:bà•d oblicze‰');
                   3 : write ('3:???????????????');
                   4 : write ('4:brak pami©ci');
                   5 : write ('5:dzielenie przez zero');
              end;
              halt;
              end;
          0 : begin
          write ('bà•d nr. ');
          case e of
               1 : write ('1:brak karty graficznej');
               2 : write ('2:bà•d oblicze‰');
               3 : write ('3:???????????????');
               4 : write ('4:brak pami©ci');
               5 : write ('5:dzielenie przez zero');
          end;
          halt;
     end;
     end;
end;
procedure tank;
begin
     setcolor (2);
     rectangle (1,1,15,10);
     setfillstyle (1,2);
     floodfill (3,3,2);
end;
procedure tanka;
begin
     setcolor (red);
     rectangle (1,1,15,10);
     setfillstyle (1,red);
     floodfill (3,3,red);
end;
procedure kula;
begin
     setcolor (15);
     circle (11,11,1);
end;
procedure wybuch;
begin
     setcolor (12);
     circle (5,5,3);
     setfillstyle (1,12);
     floodfill (5,5,12);
end;
procedure initializuj;
begin
     clrscr;
     karta := detect;
     initgraph (karta,mode,pach);
     if graphresult <> grok then error (0,1);
end;
procedure laduj;
begin
     moveto (1,maxy-10);
     setcolor (15);
     outtext ('àaduje'+chr(219));
     memory := imagesize (1,1,15,10);
     getmem (tankm,memory);
     tank;
     getimage (1,1,15,10,tankm^);
     putimage (1,1,tankm^,xorput);
     setcolor (15);
     outtext (chr (219));
     getmem (tankp,memory);
     tanka;
     getimage (1,1,15,10,tankp^);
     putimage (1,1,tankp^,xorput);
     setcolor (15);
     outtext (chr(219));
     memory := imagesize (9,9,13,13);
     getmem (kulaa,memory);
     kula;
     getimage (9,9,13,13,kulaa^);
     putimage (9,9,kulaa^,xorput);
     setcolor (15);
     outtext (chr(219));
     memory := imagesize (1,1,10,10);
     getmem (wybucha,memory);
     setcolor (15);
     outtext (chr(219));
     wybuch;
     getimage (1,1,10,10,wybucha^);
     putimage (1,1,wybucha,xorput);
     outtext (chr(219));
     cleardevice;
     moveto (0,0);
     setcolor (15);
     setfillstyle (1,15);
end;
procedure savewyniki;
begin
     assign (wyniki,'wyniki.ini');
     append (wyniki);
     writeln (wyniki,wynik);
end;
procedure showwyniki;
var
i,pom : integer;
zamiana : boolean;
istring : string;
code : integer;
begin
     cleardevice;
     j := 1;
     i := 1;
     settextstyle (0,0,3);
     outtextxy (maxx div 3,2,'Wyniki');
     settextstyle (0,0,2);
     reset (wyniki);
     while not eof (wyniki) do
     begin
          readln (wyniki,wyniks[i]);
          inc (i);
     end;
     for i := 1 to maxwyni do val (wyniks[i],wynikinteger[i],code);
     while zamiana do
     begin
          zamiana := false;
          for i := 1 to maxwyni do
              if wynikinteger [i] > wynikinteger [i+1] then
              begin
                   pom := wynikinteger[i];
                   wynikinteger[i] :=wynikinteger[i+1];
                   wynikinteger[i+1] := pom;
                   zamiana := true;
              end;
     end;
     for i := maxwyni downto 1 do
     begin
          str(wynikinteger[i],wyniks[j]);
          inc(j);
     end;
     for i := 1 to maxwyni do
     begin
          str (i,istring);
          outtextxy (maxx div 2 - 100,i*45,istring+' wynik : '+wyniks[i]);
     end;
     close (wyniki);
     klawisz := readkey;
     repeat until keypressed;
     mainmenu;
end;
procedure awin;
var
x,y,x1,y1,a : integer;
begin
     x1 := maxx-15;
     x := 1;
     setcolor (2);
     setbkcolor (9);
     settextstyle (0,0,5);
     outtextxy (maxx div 2 - 230,maxy div 2,'mini DZIAùO');
     sstartgame;
     repeat
           y := random (465);
           x := 1;
           x1 := maxx-15;
           y1 := y;
           for a := 1 to maxx-1 do begin
                 dec (x1);
                 inc (x);
                 putimage (x,y,tankm^,xorput);
                 putimage (x1,y1,tankp^,xorput);
                 wfr;
                 putimage (x,y,tankm^,xorput);
                 putimage (x1,y1,tankp^,xorput);
                 if x > maxx-15 then x := 1;
                 if x1 < 1 then x1 := maxx-15;
                 if x = x1 then
                 begin
                      putimage (x,y,wybucha^,xorput);
                      swybuch;
                      delay (100);
                      putimage (x,y,wybucha^,xorput);
                 end;
                 if keypressed then break;
           end;
     until keypressed;
     for i := 240 downto 1 do begin
         setcolor (random (16));
         rectangle (1+i,1+i,maxx-i,maxy-i);
     end;
     settextstyle (0,0,1);
     setbkcolor (red);
     setcolor (15);
end;
procedure r;
begin
     dm := random (170)+1;
     dp := random (170)+350;
     wiatr := random (400)-200;
end;
procedure poradn;
begin
case pora of
          1: begin
                  setbkcolor (9);
                  setfillstyle (1,14);
                  circle (40,40,10);
                  floodfill (40,40,15);
          end;
          2 : begin
                   setbkcolor (0);
                   setfillstyle (1,7);
                   circle (40,40,10);
                   floodfill (40,40,15);
                   for i := 1 to 150 do putpixel (random (maxx),random (maxy-5),15);
          end;
     else error (1,3);
     end;
end;
procedure move;
begin
     vs :=sila*s/40;       {skladowa pionowa predkosci poczatkowej=sila}
     gotoxy (1,1);
     dy :=maxy-16-(vs*t-graw*t*t/2); {wsp¢àrzedna pionowa "y"}
     vx :=sila*c/40+(wiatr/100);       {skladowa pozioma predkosci poczatkowej=const}
     dx :=dm+6+vx*t;        {wsp¢àrz©dna pozioma w danej chwili}
end;
procedure movep;
begin
     vs :=sila*s/40;
     gotoxy (1,1);
     dy :=maxy-16-(vs*t-graw*t*t/2);
     vx :=sila*c/40-(wiatr/100);
     dx :=dp+3-vx*t;
end;
procedure drwm (pomoc : integer);
var
c,s,dlm : real;
wys,szer,rysy,rysx : integer;
begin
     setwritemode (xorput);
     dlm := pi/180*kat;
     s := sin (dlm);
     c := cos (dlm);
     wys := round (15*s);
     szer := round (15*c);
     rysy := maxy-16-wys;
     rysx := dm + szer;
     case pomoc of
     0 : line (dm,maxy-16,rysx,rysy);
     1 :
     begin
          dlm := pi/180*(kat-1);
          s := sin (dlm);
          c := cos (dlm);
          wys := round (15*s);
          szer := round (15*c);
          rysy := maxy-16-wys;
          rysx := dm + szer;
          line (dm,maxy-16,rysx,rysy);
          dlm := pi/180*kat;
          s := sin (dlm);
          c := cos (dlm);
          wys := round (15*s);
          szer := round (15*c);
          rysy := maxy-16-wys;
          rysx := dm + szer;
          line (dm,maxy-16,rysx,rysy);
     end;
     -1 :
     begin
          dlm := pi/180*(kat+1);
          s := sin (dlm);
          c := cos (dlm);
          wys := round (15*s);
          szer := round (15*c);
          rysy := maxy-16-wys;
          rysx := dm + szer;
          line (dm,maxy-16,rysx,rysy);
          dlm := pi/180*kat;
          s := sin (dlm);
          c := cos (dlm);
          wys := round (15*s);
          szer := round (15*c);
          rysy := maxy-16-wys;
          rysx := dm + szer;
          line (dm,maxy-16,rysx,rysy);
     end;
end;
end;
procedure drwp (pomoc : integer);
var
c,s,dlm : real;
wys,szer,rysy,rysx : integer;
begin
     setcolor (red);
     setwritemode (xorput);
     dlm := pi/180*lk;
     s := sin (dlm);
     c := cos (dlm);
     wys := round (15*s);
     szer := round (15*c);
     rysy := maxy-16-wys;
     rysx := dp+15 - szer;
     case pomoc of
     0 : line (rysx,rysy,dp+15,maxy-16);
     1 :
     begin
          dlm := pi/180*(lk-1);
          s := sin (dlm);
          c := cos (dlm);
          wys := round (15*s);
          szer := round (15*c);
          rysy := maxy-16-wys;
          rysx := dp + 15 - szer;
          line (rysx,rysy,dp+15,maxy-16);
          dlm := pi/180*lk;
          s := sin (dlm);
          c := cos (dlm);
          wys := round (15*s);
          szer := round (15*c);
          rysy := maxy-16-wys;
          rysx := dp + 15 - szer;
          line (rysx,rysy,dp+15,maxy-16);
     end;
     -1 :
     begin
          dlm := pi/180*(lk+1);
          s := sin (dlm);
          c := cos (dlm);
          wys := round (15*s);
          szer := round (15*c);
          rysy := maxy-16-wys;
          rysx := dp + 15 - szer;
          line (rysx,rysy,dp+15,maxy-16);
          dlm := pi/180*lk;
          s := sin (dlm);
          c := cos (dlm);
          wys := round (15*s);
          szer := round (15*c);
          rysy := maxy-16-wys;
          rysx := dp + 15 - szer;
          line (rysx,rysy,dp+15,maxy-16);
     end;
end;
end;
procedure inputkatandsilap;
begin
     lk := 45;
     drwp (0);
     ls := 500;
     textcolor (4);
     gotoxy (1,1);
     write ('k•t :');
     gotoxy (6,1);
     write (lk);
     gotoxy (15,1);
     write ('siàa :');
     gotoxy (21,1);
     write (ls);
     repeat
           if kat < lk then begin dec (lk); drwp (-1); end
           else begin inc (lk); drwp (1); end;
           delay (10);
           dzwiek (lk*9);
           gotoxy (6,1);
           write ('   ');
           gotoxy (6,1);
           write (lk);
     until lk = kat;
     repeat
           if sila < ls then dec (ls)
           else inc (ls);
           delay (2);
           dzwiek (ls);
           gotoxy (21,1);
           write ('    ');
           gotoxy (21,1);
           write (ls);
     until ls = sila;
     setcolor (15);
end;
procedure inputkatandsila;
begin
     kat := 40;
     sila := 500;
     textcolor (2);
     gotoxy (1,1);
     write ('k•t :');
     gotoxy (6,1);
     write (kat);
     gotoxy (15,1);
     write ('siàa :');
     gotoxy (21,1);
     write (sila);
     gotoxy (59,1);
     textcolor (15);
     write ('wiatr: ',wiatr);
     if wiatr < 0 then
     begin
          gotoxy (69,1);
          write ('<');
          for i := 1 to 10 do write (chr(196));
     end
     else
     begin
          gotoxy (79,1);
          write ('>');
          gotoxy (69,1);
          for i := 1 to 10 do write (chr(196));
     end;
     textcolor (2);
     repeat
           if (kat >= 0) and (kat <= 90) and (sila >= 0) and (sila <= 1000) then
           begin
           klawisz  := #0;
           klawisz := readkey;
           case klawisz of
                #0:
                  begin
                       klawisz := readkey;
                       case klawisz of
                            #72 : inc (kat);
                            #80 : dec (kat);
                            #75 : dec (sila);
                            #77 : inc (sila);
                            #73 : sila := sila + 30;
                            #81 : sila := sila - 30;
                       end;
                  end;
           end;
           if klawisz = #72 then begin drwm (1); dzwiek (kat*9); end;
           if klawisz = #80 then begin drwm (-1); dzwiek (kat*9); end;
           if (klawisz = #75) or (klawisz = #77) or (klawisz = #73) or (klawisz = #81)
           then dzwiek (sila);
           gotoxy (6,1);
           write ('   ');
           gotoxy (6,1);
           if (kat < 0) then write (kat+1)
           else if (kat > 90) then write (kat-1) else write (kat);
           gotoxy (21,1);
           write ('    ');
           gotoxy (21,1);
           if (sila < 0) then write (sila+1)
           else if (sila > 1000) then write (sila-1) else write (sila);
           end
           else
           begin
           if kat <= 0 then kat := 0
           else if kat >= 90 then kat := 90;
           if sila <= 0 then sila := 0
           else if sila >= 1000 then sila := 1000;
           end;
     until klawisz = #32;
     setcolor (15);
end;
procedure strzal;
begin
     dx := dm + 6;
     dy := maxy - 16;
     trafiono := false;
     trafiles := false;
     t := 0;
     repeat
     t := t+1;
     move;
     dxi := round (dx);
     dyi := round (dy);
     putimage (dxi,dyi,kulaa^,xorput);
     wfr;
     putimage (dxi,dyi,kulaa^,xorput);
     if dyi > maxy-10 then
     begin
          putimage (dxi,dyi-5,wybucha^,normalput);
          swybuch;
          delay (100);
          putimage (dxi,dyi-5,wybucha^,xorput);
          trafiono := true;
     end;
     if dxi > maxx-1 then
     begin
          putimage (dxi,dyi,wybucha^,xorput);
          swybuch;
          delay (100);
          putimage (dxi,dyi,wybucha^,xorput);
          trafiono := true;
     end;
     if dyi < 1 then
     begin
          putimage (dxi,dyi,wybucha^,xorput);
          swybuch;
          delay (100);
          putimage (dxi,dyi,wybucha^,xorput);
          trafiono := true;
     end;
     if dxi < 1 then
     begin
          putimage (dxi,dyi,wybucha^,xorput);
          swybuch;
          delay (100);
          putimage (dxi,dyi,wybucha^,xorput);
          trafiono := true;
      end;
     if (dxi >= dp-6) and (dxi <= dp + 17) and (dyi < maxy-5) and (dyi >= maxy-15) then
     begin
          putimage (dxi,dyi,wybucha^,xorput);
          strafiles;
          delay (100);
          putimage (dxi,dyi,wybucha^,xorput);
          delay (100);
          putimage (dp,maxy-16,tankp^,xorput);
          trafiles := true;
          inc (wynik);
     end;
     until (trafiono) or (trafiles);
     setwritemode (0);
end;
procedure strzalp;
begin
     dx := dp + 3;
     dy := maxy - 16;
     trafiono := false;
     t := 0;
     if trafiles then exit;
     repeat
     t := t+1;
     movep;
     dxi := round (dx);
     dyi := round (dy);
     putimage (dxi,dyi,kulaa^,xorput);
     wfr;
     putimage (dxi,dyi,kulaa^,xorput);
     if dyi > maxy-10 then
     begin
          putimage (dxi,dyi-5,wybucha^,normalput);
          swybuch;
          delay (100);
          putimage (dxi,dyi-5,wybucha^,xorput);
          trafiono := true;
     end;
     if dxi > maxx-1 then
     begin
          putimage (dxi,dyi,wybucha^,xorput);
          swybuch;
          delay (100);
          putimage (dxi,dyi,wybucha^,xorput);
          trafiono := true;
     end;
     if dyi < 1 then
     begin
          putimage (dxi,dyi,wybucha^,xorput);
          swybuch;
          delay (100);
          putimage (dxi,dyi,wybucha^,xorput);
          trafiono := true;
     end;
     if dxi < 1 then
     begin
          putimage (dxi,dyi,wybucha^,xorput);
          swybuch;
          delay (100);
          putimage (dxi,dyi,wybucha^,xorput);
          trafiono := true;
      end;
     if (dxi >= dm) and (dxi <= dm + 16) and (dyi < maxy-5) and (dyi >= maxy-15) then
     begin
          putimage (dxi,dyi,wybucha^,xorput);
          strafiles;
          delay (100);
          putimage (dxi,dyi,wybucha^,xorput);
          delay (70);
          putimage (dm,maxy-16,tankm^,xorput);
          trafiles := true;
          trafiono := true;
          przegrana := true;
     end;
     until (trafiono) or (trafiles);
     setwritemode (0);
end;
procedure pt;
begin
     cleardevice;
     settextstyle (0,0,3);
     outtextxy (maxx div 15,10,'Wybierz poziom trudnoòci');
     settextstyle (0,0,2);
     outtextxy (maxx div 2 - 100,maxy div 2-60,'à(A)twy');
     outtextxy (maxx div 2 - 100,maxy div 2-40,'ò(R)edni');
     outtextxy (maxx div 2 - 100,maxy div 2-20,'(T)rudny');
     repeat
           klawisz := readkey;
           case klawisz of
                'a' : poziomtr := 1;
                'r' : poziomtr := 2;
                't' : poziomtr := 3;
           end;
     until (klawisz = 'a') or (klawisz = 'r') or (klawisz = 't');
     dzwiek (poziomtr*400);
end;
procedure gra;
var
pom : string;
begin
wynik := 0;
tury := 0;
pt;
repeat
     koniec := false;
     pora := random (2) + 1;
     inc (tury);
     cleardevice;
     setcolor (15);
     mozliwosci := 0;
     r;
     repeat
     setcolor (15);
     sila := 500;
     kat := 40;
     cleardevice;
     koniec := false;
     przegrana := false;
     poradn;
     inc (mozliwosci);
     setcolor (2);
     line (0,maxy-5,maxx,maxy-5);
     setcolor (15);
     putimage (dm,maxy-16,tankm^,normalput);
     putimage (dp,maxy-16,tankp^,normalput);
     line (0,16,maxx,16);
     setfillstyle (1,7);
     floodfill (20,4,15);
     setfillstyle (1,15);
     setcolor (2);
     drwm (0);
     inputkatandsila;
     s :=sin(kat*pi/180);
     c :=cos(kat*pi/180);
     dzwiek (900);
     strzal;
     {komputer strzela}
     if not trafiles then begin
     case poziomtr of
     1 :
     begin kat := random (91);
     sila := random (1000)+1;
     end;
     2 :
     begin kat := random (52);
     sila := random (1000)+1;
     end;
     3 :
     begin kat := random (20)+7;
     sila := random (690)+300;
     end
     else error (1,2);
     end;
     inputkatandsilap;
     s :=sin(kat*pi/180);
     c :=cos(kat*pi/180);
     end;
     dzwiek (900);
     strzalp;
     klawisz := readkey;
     dzwiek (600);
     until (mozliwosci >= 2) or (przegrana);
     setcolor (15);
     cleardevice;
     str (tury,pom);
     str (wynik,wynikst);
     settextstyle (0,0,2);
     setbkcolor (red);
     if przegrana then
     begin
          outtextxy (maxx div 3,maxy div 2,'Przegrana :(');
          sprzegrana;
          delay (200);
          wynik := 0;
          koniec := true;
     end
     else
     if tury < 20 then
     begin
          settextstyle (0,0,3);
          outtextxy (maxx div 3,maxy div 6 - 60,'tura nr. '+pom);
          settextstyle (0,0,2);
          outtextxy (maxx div 9,maxy div 4 - 10,'Zdobyàeò w tej grze '+wynikst+' punkt¢w');
          outtextxy (maxx div 2 - 100,maxy div 2 - 60,'(N)ast©pna tura');
          outtextxy (maxx div 2 - 100,maxy div 2 - 40,'(K)oniec gry');
          outtextxy (maxx div 2 - 100,maxy div 2 - 20,'(W)yjdz do DOSU');
          repeat
                klawisz := #0;
                klawisz := readkey;
                case klawisz of
                     'n' : koniec := false;
                     'k' : koniec := true;
                     'w' : begin
                          zwpam;
                          sendgame;
                          j := random (14)+1;
                          textcolor (j+1);
                          sng;
                          halt;
                     end;
                end; {case}
          until (klawisz = 'n') or (klawisz = 'k') or (klawisz = 'w');
     dzwiek (600);
     end
     else if tury >= 20 then
     begin
          settextstyle (0,0,3);
          outtextxy (maxx div 3 - 50,maxy div 11,'KONIEC GRY!!!');
          swygrana;
          settextstyle (0,0,2);
          outtextxy (maxx div 9,maxy div 4 - 10,'Zdobyàeò w grze '+wynikst+' punkt¢w');
          outtextxy (maxx div 2 - 100,maxy div 2 - 40,'(M)enu');
          outtextxy (maxx div 2 - 100,maxy div 2 - 16,'(K)oniec');
          repeat
                klawisz := #0;
                klawisz := readkey;
                case klawisz of
                     'm' : begin
                                koniec := true;
                     end;
                     'k' : begin
                                zwpam;
                                sendgame;
                                j := random (14)+1;
                                textcolor (j+1);
                                sng;
                                halt;
                     end;
                end; {case}
          until (klawisz = 'm') or (klawisz = 'k');
     end;
until (koniec) or (przegrana);
if not przegrana then begin
   savewyniki;
   showwyniki;
end
else
mainmenu;
end;
procedure mainmenu;
begin
     if koniec = true then koniec := false;
     cleardevice;
     settextstyle (0,0,3);
     outtextxy (maxx div 2 - 80,maxy div 20,'MENU');
     settextstyle (0,0,2);
     outtextxy (maxx div 2 - 100,maxy div 2 - 40,'(S)tart');
     outtextxy (maxx div 2 - 100,maxy div 2,'(K)oniec');
     repeat
           klawisz := #0;
           klawisz := readkey;
                case klawisz of
                     's' : begin dzwiek (600); gra; end;
                     'k' : begin
                                konieccaly := true;
                                zwpam;
                     end;
           end;
     until (klawisz = 's') or (klawisz = 'k')
end;
begin
     konieccaly := false;
     initializuj;
     laduj;
     awin;
     textcolor (15);
     directvideo := false;
     repeat
           mainmenu;
     until konieccaly;
     sendgame;
     sng;
end.