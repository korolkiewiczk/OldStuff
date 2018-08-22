{TRAP SCRIPT COMPILER copyright (c) Kamil Korolkiewicz 2004}
program tscomp;
uses crt,strings;
{$I LIST.PAS}
const
procfilename = 'PROC.DAT';
progtitle : pchar = ('TRAP SCRIPT COMPILER copyright (c) Kamil Korolkiewicz 2004');
proginfo : pchar = ('U¾ycie: TSCOMP [nazwa_pliku.tsc]');
{}
opn = 4;
opnames : array [0..opn-1] of char = ('/','$','@','#');
{}
directive = 0;
variable = 1;
jlabel = 2;
proc = 3;
{}
exitpv = 0;
{}
{$I DIRECTIV.PAS}
{$I ERRORS.PAS}
{$I PROCED.PAS}
{$I CODE.PAS}
var
fname : string[12];
f : text;
fp : text;
{}
constnames : tslist;
labelnames : tslist;
scriptnames : tslist;
{}
scriptpos : word;
scriptwordsize : word;
line : word;
{}
procedures : tprocedures;
{}
ocode : tcode;

procedure getparams;
begin
     if paramcount = 0 then
     begin
          writeln (proginfo);
          halt;
     end
     else
     begin
          fname := paramstr (1);
     end;
end;

procedure openfile;
begin
     assign (f,fname);
     {$I-}
     reset (f);
     {$I+}
     if ioresult <> 0 then
     begin
          writeln ('Plik '+fname+' nie odnaleziony!');
          halt;
     end;
end;

procedure exitprocess;
begin
     close (f);
     close (fp);
end;

procedure readf (var f : text; var s : string);
var
i : byte;
c : char;
begin
     i := 1;
     while not eof (f) do
     begin
          read (f,c);
          if (c = #13) or (c = #10) then continue;
          if c = ' ' then break;
          s[i] := c;
          inc (i);
     end;
     s[0] := chr (i-1);
end;

procedure loadprocs;
var
ps : string;
i : byte;
a,j : byte;
c : char;
begin
     i := 0;
     while not eof (fp) do
     begin
          readf (fp,ps);
          getmem (procedures[i].name,length(ps)+1);
          strpcopy (procedures[i].name,ps);
          {}
          read (fp,a);
          if a > maxarg then exitprocess;
          procedures[i].args := a;
          if a <> 0 then
          for j := 0 to a-1 do
          begin
               read (fp,c);
               read (fp,c);
               case c of
               'b':procedures[i].arg[j] := 1;
               'w':procedures[i].arg[j] := 2;
               'd':procedures[i].arg[j] := 4;
               else
               exitprocess;
               end;
          end;
          read (fp,procedures[i].code);
          inc (i);
     end;
end;

procedure initall;
begin
     assign (fp,procfilename);
     {$I-}
     reset (fp);
     {$I+}
     if ioresult <> 0 then
     begin
          close (f);
          writeln ('Plik z procedurami '+procfilename+' nie odnaleziony');
          halt;
     end;
     loadprocs;
     createlist (constnames);
     createlist (labelnames);
     createlist (scriptnames);
     getmem (ocode,clen);
     fillchar (ocode^,clen,0);
     scriptpos := 0;
end;

procedure compileerror (n : byte);
begin
     writeln ('Bˆ¥d #',n,':',errors[n],' .Linia nr ',line);
     exitprocess;
     halt;
end;

function copystrbpl (var dest : pchar; var source : string) : word;
var
l,i : byte;
pc : char;
begin
     l := ord (source[0]);
     fillchar (dest^,256,0);
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
     fillchar (dest^,256,0);
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

procedure delspace (var s : string);
var
i,j,l : byte;
c : char;
begin
     l := ord (s[0]);
     i := 1;
     c := s[1];
     if c <> ' ' then exit;
     while c = ' ' do
     begin
          inc (i);
          c := s[i];
     end;
     for j := 1 to l-i+1 do
     begin
          s[j] := s[j+i-1];
     end;
     s[0] := chr (l-i+1);
end;

procedure addconstv (s : pchar; sn : word; a : word; isad : byte);
var
ps : pchar;
name : pchar;
pval : word;
i,j : word;
c : char;
pc : array [0..255] of char;
er : integer;
begin
     getmem (ps,256);
     copystrn (ps,s,sn);
     if isad=1 then
     begin
          list_additem (constnames,ps,a);
     end
     else
     begin
     i := 0;
     c := ps[i];
     if c = '=' then compileerror (4);
     fillchar (pc,256,0);
     while c <> '=' do
     begin
          c := ps[i];
          if c = #0 then compileerror (5);
          if c <> '=' then
          pc[i] := c;
          inc (i);
     end;
     pc[i] := #0;
     getmem (name,strlen(pc)+1);
     strcopy (name,pc);
     {}
     j := i;
     fillchar (pc,256,0);
     while c <> #0 do
     begin
          c := ps[i];
          pc[i-j] := c;
          inc (i);
     end;
     val (pc,pval,er);
     if er <> 0 then compileerror (6);
     list_additem (constnames,name,pval);
     end;
end;

function findproc (namm : pchar) : byte;
var
i : byte;
begin
     for i := 0 to maxproc-1 do
     begin
          if strcomp (procedures[i].name,namm) = 0 then break;
     end;
     findproc := i;
end;

procedure addval2code (v : longint; sv : byte; sp : word);
begin
     if sv = 1 then ocode^[sp] := byte (v)
     else
     if sv = 2 then
     begin
          ocode^[sp] := lo(word (v));
          ocode^[sp+1] := hi(word (v));
     end
     else
     if sv = 4 then
     begin
          ocode^[sp] := lo(word (v));
          ocode^[sp+1] := hi(word (v));
          ocode^[sp+2] := lo(v shr 16);
          ocode^[sp+3] := hi(v shr 16);
     end;
end;

function addstr2code (var s : pchar; st : word; sp : word; d : boolean) : word;
var
i,j : word;
c : char;
begin
     i := st+1;
     j := sp;
     c := #255;
     while (c <> '"') and (c <> #0) do
     begin
          c := s[i];
          if c = #0 then compileerror (13);
          if d then
          begin
          if c <> '"' then ocode^[sp] := ord (c)
          else
          ocode^[sp] := 0;
          end;
          inc (sp);
          inc (i);
     end;
     addstr2code := sp-j;
end;

function getprocsize (namm : pchar) : byte;
var
p : byte;
i : byte;
s : byte;
begin
     getprocsize := 1;
     p := findproc (namm);
     if procedures[p].args=0 then exit;
     s := 1;
     for i := 0 to procedures[p].args-1 do
     begin
          s := s+procedures[p].arg[i];
     end;
     getprocsize := s;
end;

procedure precompile;
var
ps : string;
pch : pchar;
ph : pchar;
pn : pchar;
fc : char;
isvar : boolean;
isscript : boolean;
csbpl : word;
pval : word;
er : integer;
begin
     writeln ('Przygotowuje kompilacje');
     scriptwordsize := 2;
     isvar := false;
     isscript := false;
     getmem (pch,256);
     getmem (ph,256);
     line := 1;
     while not eof (f) do
     begin
          readln (f,ps);
          if ps = '' then begin inc (line); continue; end;
          delspace (ps);
          fc := ps [1];
          if fc = opnames [variable] then
          begin
               if isvar = false then compileerror (1);
               csbpl := copystrbpl (pch,ps);
               getmem (pn,strlen (pch)+1);
               strcopy (pn,pch);
               list_additem (constnames,pn,scriptpos);
               if ps[csbpl+1] <> '"' then
               inc (scriptpos,scriptwordsize)
               else
               begin
                    strpcopy (pch,ps);
                    inc (scriptpos,addstr2code (pch,csbpl,scriptpos,false));
               end;
          end
          else
          if fc = opnames [jlabel] then
          begin
               if isscript = false then compileerror (2);
               copystrbpl (pch,ps);
               pch[strlen(pch)-1] := #0;
               getmem (pn,strlen (pch)+1);
               strcopy (pn,pch);
               list_additem (labelnames,pn,scriptpos);
          end
          else
          if fc = opnames [directive] then
          begin
               if ps [2] = '/' then begin inc (line); continue; end;
               csbpl := copystrbpl (pch,ps);
               if strcomp (pch,directives[script])=0 then
               if (isvar = false) and (isscript = false) then
               begin
                    isscript := true;
                    strpcopy (pch,ps);
                    copystrn (ph,pch,csbpl);
                    getmem (pn,strlen (ph)+1);
                    strcopy (pn,ph);
                    list_additem (labelnames,pn,scriptpos);
                    list_additem (scriptnames,pn,scriptpos);
               end
               else
               compileerror (7);
               {}
               if strcomp (pch,directives[endscript])=0 then if isscript then
               begin
                    isscript := false;
                    inc (scriptpos);
               end
               else
               compileerror (11);
               {}
               if strcomp (pch,directives[_var])=0 then
               if isscript = false then isvar := true else compileerror (8);
               if strcomp (pch,directives[endvar])=0 then if isvar then isvar := false
               else
               compileerror (12);
               {}
               if strcomp (pch,directives[_const])=0 then
               begin
                    strpcopy (ph,ps);
                    addconstv (ph,csbpl,scriptpos,0);
               end;
               if strcomp (pch,directives[wordsize])=0 then
               begin
                    strpcopy (pch,ps);
                    copystrn (ph,pch,csbpl);
                    val (ph,pval,er);
                    if er <> 0 then compileerror (6);
                    scriptwordsize := pval;
               end;
               if strcomp (pch,directives[code])=0 then inc (scriptpos);
          end
          else
          if fc = opnames [proc] then
          begin
               if isscript = false then compileerror (0);
               csbpl := copystrbpl (pch,ps);
               inc (scriptpos,getprocsize(pch));
          end
          else
          compileerror (3);
          inc (line);
     end;
     if isvar then compileerror (10);
     if isscript then compileerror (10);
end;

procedure compile;
var
isvar,isscript : boolean;
ps : string;
pch,ph : pchar;
fc : char;
csbpl : word;
pval : longint;
er : integer;
i,j : word;
pr : byte;
begin
     writeln ('Kompiluje');
     scriptpos := 0;
     reset (f);
     getmem (pch,256);
     getmem (ph,256);
     isscript := false;
     isvar := false;
     line := 1;
     scriptwordsize := 2;
     while not eof (f) do
     begin
          readln (f,ps);
          if ps = '' then begin inc (line); continue; end;
          delspace (ps);
          fc := ps [1];
          if fc = opnames [variable] then
          begin
               csbpl := copystrbpl (pch,ps);
               pval := scriptwordsize;
               if ps[csbpl] <> '=' then addval2code (0,scriptwordsize,scriptpos)
               else
               if ps[csbpl] = '=' then
               begin
                    if ps[csbpl+1] <> '"' then
                    begin
                         strpcopy (pch,ps);
                         copystrn (ph,pch,csbpl);
                         val (ph,pval,er);
                         if er <> 0 then compileerror (6);
                         addval2code (pval,scriptwordsize,scriptpos);
                         pval := scriptwordsize;
                    end
                    else
                    begin
                         strpcopy (pch,ps);
                         pval := addstr2code (pch,csbpl,scriptpos,true);
                    end;
               end
               else
               compileerror (5);
               inc (scriptpos,pval);
          end
          else
          {-----}
          if fc = opnames [directive] then
          begin
               if ps [2] = '/' then begin inc (line); continue; end;
               csbpl := copystrbpl (pch,ps);
               if strcomp (pch,directives[endscript])=0 then
               begin
                    isscript := false;
                    addval2code (exitpv,1,scriptpos);
                    inc (scriptpos);
               end;
               if strcomp (pch,directives[_var])=0 then
               if isscript = false then isvar := true else compileerror (8);
               if strcomp (pch,directives[endvar])=0 then isvar := false;
               if strcomp (pch,directives[wordsize])=0 then
               begin
                    strpcopy (pch,ps);
                    copystrn (ph,pch,csbpl);
                    val (ph,pval,er);
                    if er <> 0 then compileerror (6);
                    scriptwordsize := pval;
               end;
               if strcomp (pch,directives[code])=0 then
               begin
                    fillchar (ph^,256,0);
                    strpcopy (pch,ps);
                    copystrn (ph,pch,csbpl);
                    val (ph,pval,er);
                    if er <> 0 then compileerror (6);
                    ocode^[scriptpos] := pval;
                    inc (scriptpos);
               end;
          end
          {-----}
          else
          if fc = opnames [proc] then
          begin
               csbpl := copystrbpl (pch,ps);
               pr := findproc (pch);
               ocode^[scriptpos] := procedures[pr].code;
               inc (scriptpos);
               strpcopy (pch,ps);
               if procedures[pr].args > 0 then
               for j := 0 to procedures[pr].args-1 do
               begin
                    csbpl := copystrl (ph,pch,csbpl);
                    if ph[0] = '$' then
                    begin
                         copystrn (ph,ph,1);
                         list_getval (pval,constnames,ph);
                         if pval = pvnil then compileerror (9);
                    end
                    else
                    if ph[0] = '@' then
                    begin
                         copystrn (ph,ph,1);
                         list_getval (pval,labelnames,ph);
                         if pval = pvnil then compileerror (9);
                    end
                    else
                    begin
                         val (ph,pval,er);
                         if er <> 0 then compileerror (6);
                    end;
                    addval2code (pval,procedures[pr].arg[j],scriptpos);
                    inc (scriptpos,procedures[pr].arg[j]);
               end;
          end;
          inc (line);
     end;
     dec (line);
     writeln ('Lini: ',line);
     freemem (ph,256);
     freemem (pch,256);
end;

procedure savecfile;
const
_r = 'tsp';
ph : pchar = 'TRAP SCRIPT';
var
f : file;
newname : string[12];
r : string;
i,j : byte;
l : byte;
c : char;
pl : tslist;
pc : word;
begin
     r := _r;
     l := length (fname);
     for i := 1 to l do
     begin
          c := fname[i];
          if c = '.' then
          begin
               newname[i] := '.';
               for j := i+1 to l do
               begin
                    newname[j] := r[j-i];
               end;
               i := l;
          end
          else
          newname[i] := c;
     end;
     newname[0] := chr (l);
     {}
     writeln ('Zapisuje skrypt do pliku: ',newname);
     assign (f,newname);
     rewrite (f,1);
     {}
     blockwrite (f,ph^,strlen(ph)+1);
     pl := scriptnames;
     pc := 0;
     while pl <> nil do
     begin
          pl := pl^.next;
          inc (pc);
     end;
     blockwrite (f,pc,2);
     {}
     pl := scriptnames;
     while pl <> nil do
     begin
          blockwrite (f,pl^.name^,strlen(pl^.name)+1);
          blockwrite (f,pl^.val,2);
          pl := pl^.next;
     end;
     {}
     blockwrite (f,ocode^,scriptpos);
     close (f);
     writeln ('Kompilacja i zapisywanie zakoäczone bezbˆ©dnie');
end;

begin
     clrscr;
     writeln (progtitle);
     getparams;
     openfile;
     initall;
     precompile;
     compile;
     savecfile;
     exitprocess;
end.