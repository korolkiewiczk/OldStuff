program setup;
uses crt;
const
texts : array [0..10] of string =
('Tron Setup Program (c) Copyright Kamil Korolkiewicz 2003',
 'Cz©stotliwoòÜ d´wi©ku (w hz):',
 'Odst©py mi©dzy d´wi©kami:',
 'Generuj nowe mapy',
 'Zako‰cz i nie zapisuj',
 'Zako‰cz i zapisz',
 'Bardzo àatwy',
 'ùatwy',
 'óredni',
 'Trudny',
 'Bardzo trudny');
var
i,j,dl,gl,p,pz : word;
f : file of word;
ch : char;

procedure saveconfig;
begin
     assign (f,'tron.cfg');
     rewrite (f);
     write (f,gl);
     write (f,p);
     close (f);
end;

procedure loadconfig;
begin
     assign (f,'tron.cfg');
     {$I-}
     reset (f);
     {$I+}
     if ioresult <> 0 then
     begin
          gl := 2000;
          p := 1;
          exit;
     end;
     read (f,gl);
     read (f,p);
     close (f);
end;

procedure wfvr;
begin
     asm
        mov dx,3dah
        @tp:
            in al,dx
            test al,00001000b
            jnz @tp
        @np:
            in al,dx
            test al,00001000b
            jz @np
     end;
end;

procedure dc; assembler;
asm
   mov ah,1
   mov ch,16
   mov cl,0
   int 10h
end;

procedure gennewmaps;
var
fe : file of byte;
ii : word;
_1,_2,jj,k : byte;
_3,_4 : boolean;
stri : string;
pt : byte;
begin
     textbackground (3);
     for i := 7 to 13 do
     begin
         for j := 20 to 60 do
         begin
              mem [$b800:(i*80+j)*2] := 32;
              mem [$b800:(i*80+j)*2+1] := textattr;
         end;
     end;
     pt := 4;
     gotoxy (22,9);
     writeln ('Wybierz poziom trudnoòci:');
     repeat
           wfvr;
           gotoxy (47,9);
           write ('               ');
           gotoxy (47,9);
           write (texts[pt+4]);
           ch := readkey;
           if ch = #27 then exit;
           case ch of
           #0: begin
                    ch := readkey;
                    case ch of
                    #75: if pt > 2 then dec (pt);
                    #77: if pt < 6 then inc (pt);
                    end;
               end;
           end;
     until ch = #13;
     pt := 9-pt;
     for k := 1 to 3 do
     begin
     _1 := 0;
     _2 := 1;
     _3 := false;
     _4 := false;
     str (k,stri);
     assign (fe,'map'+stri+'.map');
     rewrite (fe);
     for ii := 0 to 4095 do
     begin
          if (ii > random (20)+10) and (ii < random (20)+31) then
          jj := random (3)
          else
          if (ii > random (70)+3900) and (ii < random (70)+3971) then
          jj := random (4)
          else
          jj := random (2);
          if jj = 2 then
          begin
          if _3 then write (fe,_1)
          else
          begin
               write (fe,jj);
               _3 := true;
          end;
          end
          else
          if jj = 3 then
          begin
          if _4 then write (fe,_1)
          else
          begin
               write (fe,jj);
               _4 := true;
          end;
          end
          else
          if (random (100) mod pt = 0) then
          write (fe,_2)
          else
          write (fe,_1)
     end;
     close (fe);
     end;
end;

begin
     randomize;
     dc;
     clrscr;
     textbackground (7);
     textcolor (0);
     for i := 0 to 79 do write (' ');
     gotoxy (1,1);
     writeln (texts[0]);
     textbackground (0);
     textcolor (7);
     for i := 0 to 1918 do write (#178);
     mem [$b800:3998] := 178;
     mem [$b800:3999] := textattr;
     textbackground (0);
     textcolor (8);
     for j := 21 to 61 do
     begin
          mem [$b800:(14*80+j)*2] := 178;
          mem [$b800:(14*80+j)*2+1] := textattr;
     end;
     for i := 8 to 13 do
     begin
          mem [$b800:(i*80+61)*2] := 178;
          mem [$b800:(i*80+61)*2+1] := textattr;
     end;
     textbackground (3);
     for i := 7 to 13 do
     begin
         for j := 20 to 60 do
         begin
              mem [$b800:(i*80+j)*2] := 32;
              mem [$b800:(i*80+j)*2+1] := textattr;
         end;
     end;
     loadconfig;
     pz := 0;
     textcolor (0);
     repeat
           for i := 7 to 13 do
           begin
                for j := 20 to 60 do
                begin
                     mem [$b800:(i*80+j)*2] := 32;
                     mem [$b800:(i*80+j)*2+1] := textattr;
                end;
           end;
           for i := 1 to 5 do
           begin
                if i = pz+1 then
                begin
                     gotoxy (23,i+8);
                     textbackground (7);
                     for j := 0 to 36 do write (' ');
                end;
                dl := length (texts[i]);
                gotoxy (41-(dl div 2),i+8);
                write (texts[i]);
                if i = 1 then write (gl);
                if i = 2 then write (p);
                textbackground (3);
           end;
           ch := readkey;
           case ch of
           #0:begin
                  ch := readkey;
                  case ch of
                  #72: if pz > 0 then pz := pz-1
                       else
                       pz := 4;
                  #80: if pz < 4 then pz := pz+1
                       else
                       pz := 0;
                  #75: begin
                            if pz=0 then if gl > 100 then dec (gl,25);
                            if pz=1 then if p > 1 then dec (p);
                       end;
                  #77: begin
                            if pz=0 then if gl < 9000 then inc (gl,25);
                            if pz=1 then if p < 100 then inc (p);
                       end;
                  end;
              end;
           #13: begin
                     if pz=2 then gennewmaps;
                     if pz=3 then begin textcolor (7); textbackground (0); clrscr; halt; end;
                     if pz=4 then begin saveconfig; textcolor (7); textbackground (0); clrscr; halt; end;
                end;
           end;
           wfvr;
     until 1=2;
end.