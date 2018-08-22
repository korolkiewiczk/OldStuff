program ssss;
uses crt;
procedure Swygrana;
var
i : integer;
begin
     for i := 60 to 80 do
     begin
          sound (i * 10);
          delay (i div 3);
     end;
     nosound;
     delay (30);
     for i := 50 to 60 do
     begin
          sound (i*10);
          delay (i);
     end;
     nosound;
     delay (10);
     sound (800);
     delay (50);
     nosound;
end;
procedure Sstartgame;
var
i : integer;
begin
     sound (800);
     delay (100);
     sound (700);
     delay (90);
     sound (780);
     delay (40);
     for i := 78 downto 10 do
     begin
          sound (i*10);
          delay (10);
     end;
     nosound;
end;
procedure Sendgame;
begin
     sound (500);
     delay (100);
     sound (300);
     delay (100);
     sound (100);
     delay (100);
     nosound;
end;
procedure Swybuch;
begin
     sound (random (100)+100);
     delay (random (20) + 70);
     nosound;
end;
procedure Strafiono;
begin
     sound (900);
     delay (70);
     nosound;
     sound (900);
     delay (50);
     nosound;
     sound (800);
     delay (68);
     nosound;
     sound (740);
     delay (20);
     nosound;
end;
procedure Sprzegrana;
var
i : integer;
begin
     for i := 30 downto 20 do
     begin
          sound (i*10);
          delay (i*3);
     end;
     nosound;
end;
begin
     writeln ('swybuch');
     Swybuch;
     readln;
     writeln ('sstartgame');
     sstartgame;
          readln;
     writeln ('swygrana');
     swygrana;
          readln;
     writeln ('sprzegrana');
     sprzegrana;
          readln;
     writeln ('strafiono');
     strafiono;
          readln;
     writeln ('sendgame');
     sendgame;
          readln;
               readln;
end.