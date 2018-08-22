uses strings;
function copystrbpl (var dest : pchar; var source : string) : word;
var
l,i : byte;
pc : char;
begin
     l := ord (source[0]);
     fillchar (dest^,strlen(dest)+1,0);
     for i := 0 to l-2 do
     begin
          pc := source[i+2];
          if (pc <> ' ') and (pc <> '=') and (pc <> '(') and (pc <> ')') and (pc <> ',') then
          dest[i] := pc
          else
          break;
     end;
     dest[i+1] := #0;
     while (pc = ' ') or (pc = '=') or (pc = '(') or (pc = ')') or (pc = ',')  do
     begin
          pc := source[i+2];
          inc (i);
     end;
     copystrbpl := i;
end;

function copystrl (var dest,source : pchar; n : word) : word;
var
l,i : byte;
pc : char;
begin
     l := strlen (source);
     fillchar (dest^,strlen(dest)+1,0);
     for i := n to l-1 do
     begin
          pc := source[i];
          if (pc <> ' ') and (pc <> '=') and (pc <> '(') and (pc <> ')') and (pc <> ',') then
          dest[i-n] := pc
          else
          break;
     end;
     dest[i+1-n] := #0;
     while (pc = ' ') or (pc = '=') or (pc = '(') or (pc = ')') or (pc = ',')  do
     begin
          pc := source[i];
          inc (i);
     end;
     copystrl := i-1;
end;

procedure copystrn (var dest,source : pchar; n : word);
var
i : word;
c : char;
begin
     i := n;
     c := ' ';
     while c <> #0 do
     begin
          c := source[i];
          dest[i-n] := c;
          inc (i);
     end;
end;

var
pc,pc2,pc3 : pchar;
s : string;
a : word;
begin
     s := '/script (45,22)';
     getmem (pc,255);
     getmem (pc2,255);
     getmem (pc3,255);
     strpcopy (pc3,s);
     a := copystrbpl (pc,s);
     a := copystrl (pc2,pc3,a);
     writeln (s);
     writeln (pc);
     writeln (strlen (pc));
     writeln (a);
     writeln ('pc2:',pc2);
     writeln (pc3[a-1]);
     {}
     a := copystrl (pc2,pc3,a);
     writeln (s);
     writeln (pc);
     writeln (strlen (pc));
     writeln (a);
     writeln ('pc2:',pc2);
     writeln (pc3[a-1]);
     writeln;
     {}
     {strcopy (pc2,'script,123');
     a := copystrl (pc,pc2);
     copystrn (pc3,pc2,a);
     writeln (pc2);
     writeln (pc);
     writeln (strlen (pc));
     writeln (a);
     writeln (pc3);
     writeln (pc2[a-1]);}
end.