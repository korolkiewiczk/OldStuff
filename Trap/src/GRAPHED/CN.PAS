var
f1 : file;
f2 : text;
p,p2 : pointer;
i,j : word;
s : string [3];
begin
     assign (f1,'txttbl.asc');
     reset (f1,1);
     getmem (p,11200);
     getmem (p2,1400);
     blockread (f1,p^,11200);
     close (f1);
     asm
        push ds
        mov cx,1400
        les di,p2
        lds si,p
        @p1:
            xor ah,ah
            mov dl,8
            mov bl,1
            @p22:
            lodsb
            or al,al
            jz @d1
            or ah,bl
            @d1:
            shl bl,1
            dec dl
            jnz @p22
            mov al,ah
            stosb
        loop @p1
        pop ds
     end;
     assign (f2,'txttbl.pas');
     rewrite (f2);
     for j := 0 to 174 do
     begin
     for i := 0 to 7 do
     begin
          str (mem [seg (p2^):ofs (p2^)+j*8+i],s);
          write (f2,s+',');
     end;
     write (f2,#13);
     end;
     close (f2);
end.