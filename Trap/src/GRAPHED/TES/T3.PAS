uses scr,crt;
var
f : file;
t1,t2 : ttex;
i,j : word;
k : integer;
begin
     initvgascr;
     set_8_32_pal;
     setpalette;
     assign (f,'4.bm');
     reset (f,1);
     blockread (f,t1.sizex,4);
     getmem (buf,64000);
     getmem (t1.texmemory,t1.sizex*t1.sizey);
     blockread (f,t1.texmemory^,t1.sizex*t1.sizey);
     close (f);
     assign (f,'l.832');
     reset (f,1);
     blockread (f,t2.sizex,4);
     getmem (t2.texmemory,t2.sizex*t1.sizey);
     blockread (f,t2.texmemory^,t2.sizex*t2.sizey);
     close (f);
     buftoclr;
     k := 0;
     repeat
     for j := 0 to 200 div 48 do
         for i := 0 to 320 div 64 do
             scrtexf (i*64,j*48,(abs (shortint(i-j)*4))+4,t1);
     scrsprtexnf (k,7,t2);
     for i := 0 to 10 do
     wfvr;
     buftoscr;
     inc (k);
     if k > 320 then k := 0;
     until keypressed;
     inittxtscr;
end.