const

consolelines = 4;
consolestrlen = 48;
consoletime = 5;

type

tconsolestr = array [0..consolestrlen-1] of char;
tconsole = record
                 constack : array [0..consolelines-1] of tconsolestr;
                 contimes : array [0..consolelines-1] of stimer;
                 currconstr : byte;
end;

procedure con_addstr (var con : tconsole; str : pchar);
var
i : byte;
pt : ttimer;
begin
     with con do
     begin
     if strlen (str) >= consolestrlen then exit;
     inc (currconstr);
     if currconstr > consolelines then
     begin
          for i := 1 to consolelines-1 do
          begin
               strcopy (constack[i-1],constack[i]);
               move (contimes[i],contimes[i-1],sizeof (stimer));
          end;
          dec (currconstr);
     end;
     strcopy (constack[currconstr-1],str);
     pt := @contimes[currconstr-1];
     starttimer (pt);
     end;
end;

procedure con_update (var con : tconsole);
var
i,j : byte;
pt : ttimer;
t : longint;
begin
     with con do
     begin
     if currconstr <> 0 then
     for i := 0 to currconstr-1 do
     begin
          pt := @contimes[i];
          stoptimer (pt,timer_in_seconds,t);
          if t >= consoletime then
          begin
               for j := i+1 to currconstr-1 do
               begin
                    strcopy (constack[j-1],constack[j]);
                    move (contimes[j],contimes[j-1],sizeof (stimer));
               end;
               dec (currconstr);
          end;
     end;
     end;
end;