var
s:string;
ok:boolean;
plist:array[0..500] of string[15];
i,j:word;
begin
     ok:=false;
     i:=0;
     while not eof (input) do
     begin
          readln (s);
          if s='enum' then
          begin
               readln (s);
               ok:=true;
               continue;
          end;
          if ok then
          begin
               j:=1;
               while s[j]=' ' do inc (j);
               while (s[j]<>'=')and (j<12) do
               begin
                    if j=length (s) then
                    begin
                         dec(i);
                         break;
                    end;
                    plist[i]:=plist[i]+s[j];
                    inc (j);
               end;
               inc (i);
          end;
     end;
     for j:=1 to i do
     begin
          writeln;
          writeln ('/****************');
          writeln ('  ',plist[j-1]);
          writeln ('*****************/');
          writeln;
          writeln ('void x86_',plist[j-1],'_b()');
          writeln ('{');
          writeln ('}');
          writeln;
          writeln ('void x86_',plist[j-1],'_w()');
          writeln ('{');
          writeln ('}');
          writeln;
          writeln ('void x86_',plist[j-1],'_d()');
          writeln ('{');
          writeln ('}');
     end;
     writeln;
     writeln ('ptr x86calltab[FCNT-1][3]=');
     writeln ('{');
     for j:=1 to i do
     begin
          s:='x86_'+plist[j-1]+'_b';
          write (' {',s:16);
          s:='x86_'+plist[j-1]+'_w';
          write (',',s:16);
          s:='x86_'+plist[j-1]+'_d';
          writeln (',',s:16,'},     //',j);
     end;
     writeln ('};');
end.