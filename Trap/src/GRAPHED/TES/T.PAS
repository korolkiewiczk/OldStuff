uses scr,crt;
var
f : file;
t1 : ttex;
i,j : word;
k : integer;
begin
     initvgascr;
     set_8_32_pal;
     setpalette;
     assign (f,'b.832');
     reset (f,1);
     blockread (f,t1.sizex,4);
     getmem (buf,64000);
     getmem (t1.texmemory,t1.sizex*t1.sizey);
     blockread (f,t1.texmemory^,t1.sizex*t1.sizey);
     close (f);
     buftoclr;
     k := 0;
     repeat
     for j := 0 to 200 div t1.sizey do
         for i := 0 to 320 div t1.sizex do
             scrtexnf (i*t1.sizex,j*t1.sizey{(abs (shortint(i-j)*4))+4},t1);
     buftoscr;
     until keypressed;
     inittxtscr;
end.