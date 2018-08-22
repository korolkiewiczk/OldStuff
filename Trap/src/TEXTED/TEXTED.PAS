uses crt,scr,text;
var
i,j,k,l : byte;
x,y : byte;
f : file;
c : char;
asc : char;
tabpom : array [0..9,0..9] of byte;
opt : string;
tabsize : byte;
procedure put;
begin
     tabpom [y,x] := 127;
end;
procedure unput;
begin
     tabpom [y,x] := 0;
end;
procedure kw;
begin
     hline (x*tabsize,y*tabsize,x*tabsize+tabsize,255);
     hline (x*tabsize,y*tabsize+tabsize,x*tabsize+tabsize,255);
     vline (x*tabsize,y*tabsize,y*tabsize+tabsize,255);
     vline (x*tabsize+tabsize,y*tabsize,y*tabsize+tabsize+1,255);
end;
procedure kwp (x1,y1,c : byte);
var
pi : byte;
begin
     for pi := 0 to tabsize-2 do hline (x1*tabsize+1,y1*tabsize+pi+1,x1*tabsize+tabsize,c);
end;
procedure cleartab;
begin
     fillchar (tabpom,100,0);
end;
procedure incchr;
begin
     if l < 111 then inc (l);
     cleartab;
     move (txttbl^[l*100],tabpom,100);
end;
procedure decchr;
begin
     if l > 0 then dec (l);
     cleartab;
     move (txttbl^[l*100],tabpom,100);
end;
begin
     clrscr;
     writeln ('Geting memory...');
     getmem (buf,64000);
     getmem (txttbl,11200);
     writeln ('Freing bufers...');
     buftoclr;
     fillchar (txttbl^,11200,0);
     fillchar (tabpom,100,0);
     writeln ('Loading/Creating files...');
     assign (f,'txttbl.asc');
     if paramcount = 0 then
     begin
          {$I-}
          reset (f,1);
          {$I+}
          if ioresult <> 0 then rewrite (f,1)
          else
          blockread (f,txttbl^,11200);
     end
     else
     begin
          opt := paramstr (1);
          if (opt = '/d') or (opt = '/D') then rewrite (f,1);
     end;
     move (txttbl^[l*100],tabpom,100);
     writeln ('Initializing graph...');
     set_8_32_pal;
     initvgascr;
     setpalette;
     x := 0;
     y := 0;
     k := 0;
     l := 0;
     directvideo := false;
     tabsize := 15;
     repeat
           buftoclr;
           if l < 94 then
           asc := chr (l+33)
           else
           case l of
               94  :  asc := #164; {¤}
               95  :  asc := #143; {}
               96  :  asc := #168; {¨}
               97  :  asc := #157; {}
               98  :  asc := #227; {ã}
               99  :  asc := #224; {à}
               100 :  asc := #151; {—}
               101 :  asc := #189; {½}
               102 :  asc := #141; {}
               103 :  asc := #165; {¥}
               104 :  asc := #134; {†}
               105 :  asc := #169; {©}
               106 :  asc := #136; {ˆ}
               107 :  asc := #228; {ä}
               108 :  asc := #162; {¢}
               109 :  asc := #152; {˜}
               110 :  asc := #190; {¾}
               111 :  asc := #171; {«}
           end;
           for i := 0 to 10 do
           hline (0,i*tabsize,tabsize*10,245);
           for i := 0 to 10 do
           vline (i*tabsize,0,tabsize*10+1,245);
           kw;
           for j := 0 to 9 do
               for i := 0 to 9 do
                   if tabpom[j,i] = 127 then kwp (i,j,127)
                   else
                   kwp (i,j,68);
           textcolor (15);
           gotoxy (30,10);
           write (asc);
           if keypressed then
           c := readkey
           else
           c := #2;
           case c of
           #0 : begin
                     c := readkey;
                     case c of
                     #72 : if y > 0 then dec (y);{g}
                     #80 : if y < 9 then inc (y);{d}
                     #75 : if x > 0 then dec (x);{l}
                     #77 : if x < 9 then inc (x);{p}
                     end;
                end;
           #27 : k := 1;
           ' ' : put;
           '1' : put;
           '0' : unput;
           '+' : incchr;
           '-' : decchr;
           'c' : unput;
           'C' : cleartab;
           ']' : if tabsize < 19 then inc (tabsize);
           '[' : if tabsize > 2 then dec (tabsize);
           end;
           move (tabpom,txttbl^[l*100],100);
           textprint (240,69,0,0,asc);
           textprint (251,74,0,1,asc);
           wfvr;
           buftoscr;
     until k = 1;
     inittxtscr;
     textcolor (7);
     writeln ('Saving file...');
     rewrite (f,1);
     blockwrite (f,txttbl^,11200);
     close (f);
     writeln ('Freeing memory...');
     freemem (buf,64000);
     freemem (txttbl,11200);
end.