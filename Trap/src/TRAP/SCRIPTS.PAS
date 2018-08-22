const
scriptprocedures = 37;
SC_E = 1;
SC_B = 2;
SC_A = 4;
maxscripttimers = 32;
type
stab = array [0..65534] of byte;
ptab = ^stab;
var
scriptcodeip : word;
scriptbcode : byte;
scriptinforec : pinforecs;
scriptcode : ptrapscripts;
scriptmap : ptab;
scriptplayerisuse : boolean;
scriptconditionalflags : byte;
scriptcurrinforec : byte;
scriptownxyc : byte;
{}
scripttimers : array [0..maxscripttimers-1] of stimer;

{procedury pomocnicze do pobierania parametr¢w}

procedure s_param_getxyc (var x,y : word; var c : byte);
begin
     if scriptownxyc = 0 then
     begin
          x := (word (scriptcode^[scriptcodeip+1])+word (scriptcode^[scriptcodeip+2])*256);
          y := (word (scriptcode^[scriptcodeip+3])+word (scriptcode^[scriptcodeip+4])*256);
          c := scriptcode^[scriptcodeip+5];
     end
     else
     if scriptownxyc = 1 then
     begin
          x := inforecxyctab[scriptcurrinforec].x;
          y := inforecxyctab[scriptcurrinforec].y;
          c := inforecxyctab[scriptcurrinforec].c;
     end
     else
     if scriptownxyc = 2 then
     begin
          x := inforecxyctab[scriptcurrinforec].x;
          y := inforecxyctab[scriptcurrinforec].y;
          c := inforecxyctab[scriptcurrinforec].c+1;
     end;
end;

procedure s_param_getadas (var ad,as : word);
begin
     ad := (word (scriptcode^[scriptcodeip+1])+word (scriptcode^[scriptcodeip+2])*256);
     as := (word (scriptcode^[scriptcodeip+3])+word (scriptcode^[scriptcodeip+4])*256);
end;

{procedury obsˆugi skrypt¢w}

procedure s_setcell;
var
x,y : word;
c : byte;
a : word;
begin
     s_param_getxyc (x,y,c);
     {if c > 5 then syserror ('s_setcell()','Invalid "c" parameter');}
     a := (word (scriptcode^[scriptcodeip+6])+word (scriptcode^[scriptcodeip+7])*256);
     scriptmap^[(y*trapcurrmap.trapmapsizex+x)*6+c] := scriptcode^[a];
     inc (scriptcodeip,7);
end;

procedure s_getcell;
var
x,y : word;
c : byte;
a : word;
begin
     s_param_getxyc (x,y,c);
     a := (word (scriptcode^[scriptcodeip+6])+word (scriptcode^[scriptcodeip+7])*256);
     scriptcode^[a] := scriptmap^[(y*trapcurrmap.trapmapsizex+x)*6+c];
     inc (scriptcodeip,7);
end;

procedure s_setcellinfoxy;
var
x,y : word;
c : byte;
ax,ay : word;
pc : byte;
begin
     s_param_getxyc (x,y,c);
     pc := scriptmap^[(y*trapcurrmap.trapmapsizex+x)*6+c+4];
     if (pc > 0) and (pc <= maxinforecs) then
     begin
          ax := (word (scriptcode^[scriptcodeip+6])+word (scriptcode^[scriptcodeip+7])*256);
          ay := (word (scriptcode^[scriptcodeip+8])+word (scriptcode^[scriptcodeip+9])*256);
          scriptinforec^[pc-1].objectpozx := scriptcode^[ax];
          scriptinforec^[pc-1].objectpozy := scriptcode^[ay];
     end;
     inc (scriptcodeip,9);
end;

procedure s_getcellinfoxy;
var
x,y : word;
c : byte;
ax,ay : word;
pc : byte;
begin
     s_param_getxyc (x,y,c);
     pc := scriptmap^[(y*trapcurrmap.trapmapsizex+x)*6+c+4];
     if (pc > 0) and (pc <= maxinforecs) then
     begin
          ax := (word (scriptcode^[scriptcodeip+6])+word (scriptcode^[scriptcodeip+7])*256);
          ay := (word (scriptcode^[scriptcodeip+8])+word (scriptcode^[scriptcodeip+9])*256);
          scriptcode^[ax] := scriptinforec^[pc-1].objectpozx;
          scriptcode^[ay] := scriptinforec^[pc-1].objectpozy;
     end;
     inc (scriptcodeip,9);
end;

procedure s_setcellinfostate;
var
x,y : word;
c : byte;
pc : byte;
s : byte;
avisible,afaded,atransp,afade : word;
begin
     s_param_getxyc (x,y,c);
     pc := scriptmap^[(y*trapcurrmap.trapmapsizex+x)*6+c+4];
     if (pc > 0) and (pc <= maxinforecs) then
     begin
          avisible := (word (scriptcode^[scriptcodeip+6])+word (scriptcode^[scriptcodeip+7])*256);
          afaded   := (word (scriptcode^[scriptcodeip+8])+word (scriptcode^[scriptcodeip+9])*256);
          atransp  := (word (scriptcode^[scriptcodeip+10])+word (scriptcode^[scriptcodeip+11])*256);
          afade    := (word (scriptcode^[scriptcodeip+12])+word (scriptcode^[scriptcodeip+13])*256);
          s := scriptcode^[avisible] or (scriptcode^[afaded] shl 1) or (scriptcode^[atransp] shl 2)
          or (scriptcode^[afade] shl 3);
          scriptinforec^[pc-1].objectstate := s;
     end;
     inc (scriptcodeip,13);
end;

procedure s_getcellinfostate;
var
x,y : word;
c : byte;
pc : byte;
s : byte;
avisible,afaded,atransp,afade : word;
begin
     s_param_getxyc (x,y,c);
     pc := scriptmap^[(y*trapcurrmap.trapmapsizex+x)*6+c+4];
     if (pc > 0) and (pc <= maxinforecs) then
     begin
          s := scriptinforec^[pc-1].objectstate;
          avisible := (word (scriptcode^[scriptcodeip+6])+word (scriptcode^[scriptcodeip+7])*256);
          afaded   := (word (scriptcode^[scriptcodeip+8])+word (scriptcode^[scriptcodeip+9])*256);
          atransp  := (word (scriptcode^[scriptcodeip+10])+word (scriptcode^[scriptcodeip+11])*256);
          afade    := (word (scriptcode^[scriptcodeip+12])+word (scriptcode^[scriptcodeip+13])*256);
          scriptcode^[avisible] := s and 1;
          scriptcode^[afaded]   := (s shr 1) and 1;
          scriptcode^[atransp]  := (s shr 2) and 1;
          scriptcode^[afade]    := (s shr 3) and 31;
     end;
     inc (scriptcodeip,13);
end;

procedure s_setcellinfoextra;
var
x,y : word;
c : byte;
pc : byte;
aextra : word;
begin
     s_param_getxyc (x,y,c);
     pc := scriptmap^[(y*trapcurrmap.trapmapsizex+x)*6+c+4];
     if (pc > 0) and (pc <= maxinforecs) then
     begin
          aextra := (word (scriptcode^[scriptcodeip+6])+word (scriptcode^[scriptcodeip+7])*256);
          scriptinforec^[pc-1].objectextra := scriptcode^[aextra];
     end;
     inc (scriptcodeip,7);
end;

procedure s_getcellinfoextra;
var
x,y : word;
c : byte;
pc : byte;
aextra : word;
begin
     s_param_getxyc (x,y,c);
     pc := scriptmap^[(y*trapcurrmap.trapmapsizex+x)*6+c+4];
     if (pc > 0) and (pc <= maxinforecs) then
     begin
          aextra := (word (scriptcode^[scriptcodeip+6])+word (scriptcode^[scriptcodeip+7])*256);
          scriptcode^[aextra] := scriptinforec^[pc-1].objectextra;
     end;
     inc (scriptcodeip,7);
end;

procedure s_setinfo;
var
i : byte;
ax,ay,as,ast,aex : word;
begin
     i := scriptcode^[scriptcodeip+1];
     ax := (word (scriptcode^[scriptcodeip+2])+word (scriptcode^[scriptcodeip+3])*256);
     ay := (word (scriptcode^[scriptcodeip+4])+word (scriptcode^[scriptcodeip+5])*256);
     as := (word (scriptcode^[scriptcodeip+6])+word (scriptcode^[scriptcodeip+7])*256);
     ast:= (word (scriptcode^[scriptcodeip+8])+word (scriptcode^[scriptcodeip+9])*256);
     aex:= (word (scriptcode^[scriptcodeip+10])+word (scriptcode^[scriptcodeip+11])*256);
     scriptinforec^[i].objectpozx   := scriptcode^[ax];
     scriptinforec^[i].objectpozy   := scriptcode^[ay];
     scriptinforec^[i].objectscript := scriptcode^[as];
     scriptinforec^[i].objectstate  := scriptcode^[ast];
     scriptinforec^[i].objectextra  := scriptcode^[aex];
     inc (scriptcodeip,11);
end;

procedure s_getinfo;
var
i : byte;
ax,ay,as,ast,aex : word;
begin
     i := scriptcode^[scriptcodeip+1];
     ax := (word (scriptcode^[scriptcodeip+2])+word (scriptcode^[scriptcodeip+3])*256);
     ay := (word (scriptcode^[scriptcodeip+4])+word (scriptcode^[scriptcodeip+5])*256);
     as := (word (scriptcode^[scriptcodeip+6])+word (scriptcode^[scriptcodeip+7])*256);
     ast:= (word (scriptcode^[scriptcodeip+8])+word (scriptcode^[scriptcodeip+9])*256);
     aex:= (word (scriptcode^[scriptcodeip+10])+word (scriptcode^[scriptcodeip+11])*256);
     scriptcode^[ax] := scriptinforec^[i].objectpozx;
     scriptcode^[ay] := scriptinforec^[i].objectpozy;
     scriptcode^[as] := scriptinforec^[i].objectscript;
     scriptcode^[ast]:= scriptinforec^[i].objectstate;
     scriptcode^[aex]:= scriptinforec^[i].objectextra;
     inc (scriptcodeip,11);
end;

procedure s_bmoveval;
var
ad,as : word;
begin
     s_param_getadas (ad,as);
     scriptcode^[ad] := scriptcode^[as];
     inc (scriptcodeip,4);
end;

procedure s_wmoveval;
var
ad,as : word;
begin
     s_param_getadas (ad,as);
     scriptcode^[ad] := scriptcode^[as];
     scriptcode^[ad+1] := scriptcode^[as+1];
     inc (scriptcodeip,4);
end;

procedure s_dmoveval;
var
ad,as : word;
begin
     s_param_getadas (ad,as);
     scriptcode^[ad] := scriptcode^[as];
     scriptcode^[ad+1] := scriptcode^[as+1];
     scriptcode^[ad+2] := scriptcode^[as+2];
     scriptcode^[ad+3] := scriptcode^[as+3];
     inc (scriptcodeip,4);
end;

procedure s_bsetval;
var
ad : word;
begin
     ad := (word (scriptcode^[scriptcodeip+1])+word (scriptcode^[scriptcodeip+2])*256);
     scriptcode^[ad] := scriptcode^[scriptcodeip+3];
     inc (scriptcodeip,3);
end;

procedure s_wsetval;
var
ad : word;
begin
     ad := (word (scriptcode^[scriptcodeip+1])+word (scriptcode^[scriptcodeip+2])*256);
     scriptcode^[ad] := scriptcode^[scriptcodeip+3];
     scriptcode^[ad+1] := scriptcode^[scriptcodeip+4];
     inc (scriptcodeip,4);
end;

procedure s_dsetval;
var
ad : word;
begin
     ad := (word (scriptcode^[scriptcodeip+1])+word (scriptcode^[scriptcodeip+2])*256);
     scriptcode^[ad] := scriptcode^[scriptcodeip+3];
     scriptcode^[ad+1] := scriptcode^[scriptcodeip+4];
     scriptcode^[ad+2] := scriptcode^[scriptcodeip+5];
     scriptcode^[ad+3] := scriptcode^[scriptcodeip+6];
     inc (scriptcodeip,6);
end;

procedure s_baddval;
var
ad,as : word;
begin
     s_param_getadas (ad,as);
     inc (scriptcode^[ad],scriptcode^[as]);
     inc (scriptcodeip,4);
end;

procedure s_waddval;
var
ad,as : word;
pw,pw2 : word;
begin
     s_param_getadas (ad,as);
     pw := (word (scriptcode^[ad])+word (scriptcode^[ad+1])*256);
     pw2:= (word (scriptcode^[as])+word (scriptcode^[as+1])*256);
     inc (pw,pw2);
     scriptcode^[ad] := pw;
     scriptcode^[ad+1] := pw shr 8;
     inc (scriptcodeip,4);
end;

procedure s_daddval;
var
ad,as : word;
pw,pw2 : word;
begin
     s_param_getadas (ad,as);
     pw := (longint (scriptcode^[ad])+longint (scriptcode^[ad+1])*256+
     longint (scriptcode^[ad+2])*65536+longint (scriptcode^[ad+3])*16777216);
     pw := (longint (scriptcode^[as])+longint (scriptcode^[as+1])*256+
     longint (scriptcode^[as+2])*65536+longint (scriptcode^[as+3])*16777216);
     inc (pw,pw2);
     scriptcode^[ad] := pw;
     scriptcode^[ad+1] := pw shr 8;
     scriptcode^[ad+2] := pw shr 16;
     scriptcode^[ad+3] := pw shr 24;
     inc (scriptcodeip,4);
end;

procedure s_bsubval;
var
ad,as : word;
begin
     s_param_getadas (ad,as);
     dec (scriptcode^[ad],scriptcode^[as]);
     inc (scriptcodeip,4);
end;

procedure s_wsubval;
var
ad,as : word;
pw,pw2 : word;
begin
     s_param_getadas (ad,as);
     pw := (word (scriptcode^[ad])+word (scriptcode^[ad+1])*256);
     pw2:= (word (scriptcode^[as])+word (scriptcode^[as+1])*256);
     dec (pw,pw2);
     scriptcode^[ad] := pw;
     scriptcode^[ad+1] := pw shr 8;
     inc (scriptcodeip,4);
end;

procedure s_dsubval;
var
ad,as : word;
pw,pw2 : word;
begin
     s_param_getadas (ad,as);
     pw := (longint (scriptcode^[ad])+longint (scriptcode^[ad+1])*256+
     longint (scriptcode^[ad+2])*65536+longint (scriptcode^[ad+3])*16777216);
     pw := (longint (scriptcode^[as])+longint (scriptcode^[as+1])*256+
     longint (scriptcode^[as+2])*65536+longint (scriptcode^[as+3])*16777216);
     dec (pw,pw2);
     scriptcode^[ad] := pw;
     scriptcode^[ad+1] := pw shr 8;
     scriptcode^[ad+2] := pw shr 16;
     scriptcode^[ad+3] := pw shr 24;
     inc (scriptcodeip,4);
end;

procedure s_bcompval;
var
ad,as : word;
d,s : byte;
begin
     s_param_getadas (ad,as);
     d := scriptcode^[ad];
     s := scriptcode^[as];
     scriptconditionalflags := 0;
     if d = s then scriptconditionalflags := scriptconditionalflags or SC_E;
     if d < s then scriptconditionalflags := scriptconditionalflags or SC_B;
     if d > s then scriptconditionalflags := scriptconditionalflags or SC_A;
     inc (scriptcodeip,4);
end;

procedure s_wcompval;
var
ad,as : word;
d,s : word;
begin
     s_param_getadas (ad,as);
     d := (word (scriptcode^[ad])+word (scriptcode^[ad+1])*256);
     s := (word (scriptcode^[as])+word (scriptcode^[as+1])*256);
     scriptconditionalflags := 0;
     if d = s then scriptconditionalflags := scriptconditionalflags or SC_E;
     if d < s then scriptconditionalflags := scriptconditionalflags or SC_B;
     if d > s then scriptconditionalflags := scriptconditionalflags or SC_A;
     inc (scriptcodeip,4);
end;

procedure s_dcompval;
var
ad,as : word;
d,s : word;
begin
     s_param_getadas (ad,as);
     d := (longint (scriptcode^[ad])+longint (scriptcode^[ad+1])*256+
     longint (scriptcode^[ad+2])*65536+longint (scriptcode^[ad+3])*16777216);
     s := (longint (scriptcode^[as])+longint (scriptcode^[as+1])*256+
     longint (scriptcode^[as+2])*65536+longint (scriptcode^[as+3])*16777216);
     scriptconditionalflags := 0;
     if d = s then scriptconditionalflags := scriptconditionalflags or SC_E;
     if d < s then scriptconditionalflags := scriptconditionalflags or SC_B;
     if d > s then scriptconditionalflags := scriptconditionalflags or SC_A;
     inc (scriptcodeip,4);
end;

procedure s_jump;
var
l : word;
begin
     l := (word (scriptcode^[scriptcodeip+1])+word (scriptcode^[scriptcodeip+2])*256);
     scriptcodeip := l-1;
end;

procedure s_jumpe;
var
l : word;
begin
     if scriptconditionalflags and SC_E = SC_E then
     begin
          l := (word (scriptcode^[scriptcodeip+1])+word (scriptcode^[scriptcodeip+2])*256);
          scriptcodeip := l-1;
          exit;
     end;
     inc (scriptcodeip,2);
end;

procedure s_jumpb;
var
l : word;
begin
     if scriptconditionalflags and SC_B = SC_B then
     begin
          l := (word (scriptcode^[scriptcodeip+1])+word (scriptcode^[scriptcodeip+2])*256);
          scriptcodeip := l-1;
          exit;
     end;
     inc (scriptcodeip,2);
end;

procedure s_jumpa;
var
l : word;
begin
     if scriptconditionalflags and SC_A = SC_A then
     begin
          l := (word (scriptcode^[scriptcodeip+1])+word (scriptcode^[scriptcodeip+2])*256);
          scriptcodeip := l-1;
          exit;
     end;
     inc (scriptcodeip,2);
end;

procedure s_jumpbe;
var
l : word;
begin
     if (scriptconditionalflags and SC_E = SC_E) or (scriptconditionalflags and SC_B = SC_B) then
     begin
          l := (word (scriptcode^[scriptcodeip+1])+word (scriptcode^[scriptcodeip+2])*256);
          scriptcodeip := l-1;
          exit;
     end;
     inc (scriptcodeip,2);
end;

procedure s_jumpae;
var
l : word;
begin
     if (scriptconditionalflags and SC_E = SC_E) or (scriptconditionalflags and SC_A = SC_A) then
     begin
          l := (word (scriptcode^[scriptcodeip+1])+word (scriptcode^[scriptcodeip+2])*256);
          scriptcodeip := l-1;
          exit;
     end;
     inc (scriptcodeip,2);
end;

procedure s_useownxyc;
begin
     scriptownxyc := scriptcode^[scriptcodeip+1];
     inc (scriptcodeip,1);
end;

procedure s_starttimer;
begin
     startstimer (scripttimers [scriptcode^[scriptcodeip+1]]);
     inc (scriptcodeip,1);
end;

procedure s_stoptimer;
var
pv : longint;
at : word;
begin
     stopstimer (scripttimers [scriptcode^[scriptcodeip+1]],timer_in_milli_seconds,pv);
     at := (word (scriptcode^[scriptcodeip+2])+word (scriptcode^[scriptcodeip+3])*256);
     scriptcode^[at] := pv;
     scriptcode^[at+1] := pv shr 8;
     scriptcode^[at+2] := pv shr 16;
     scriptcode^[at+3] := pv shr 24;
     inc (scriptcodeip,3);
end;

procedure s_random;
var
z : word;
av : word;
rv : word;
begin
     z := (word (scriptcode^[scriptcodeip+1])+word (scriptcode^[scriptcodeip+2])*256);
     av := (word (scriptcode^[scriptcodeip+3])+word (scriptcode^[scriptcodeip+4])*256);
     rv := random (z);
     scriptcode^[av] := rv;
     scriptcode^[av+1] := rv shr 8;
     inc (scriptcodeip,4);
end;

const

scriptprocs : array [0..scriptprocedures-1] of pointer = (
nil,                                        {0}
nil,                                        {1}
@s_setcell,                                 {2}
@s_getcell,                                 {3}
@s_setcellinfoxy,                           {4}
@s_getcellinfoxy,                           {5}
@s_setcellinfostate,                        {6}
@s_getcellinfostate,                        {7}
@s_setcellinfoextra,                        {8}
@s_getcellinfoextra,                        {9}
@s_setinfo,                                 {10}
@s_getinfo,                                 {11}
@s_bmoveval,                                {12}
@s_wmoveval,                                {13}
@s_dmoveval,                                {14}
@s_bsetval,                                 {15}
@s_wsetval,                                 {16}
@s_dsetval,                                 {17}
@s_baddval,                                 {18}
@s_waddval,                                 {19}
@s_daddval,                                 {20}
@s_bsubval,                                 {21}
@s_wsubval,                                 {22}
@s_dsubval,                                 {23}
@s_bcompval,                                {24}
@s_wcompval,                                {25}
@s_dcompval,                                {26}
@s_jump,                                    {27}
@s_jumpe,                                   {28}
@s_jumpb,                                   {29}
@s_jumpa,                                   {30}
@s_jumpbe,                                  {31}
@s_jumpae,                                  {32}
@s_useownxyc,                               {33}
@s_starttimer,                              {34}
@s_stoptimer,                               {35}
@s_random);                                 {36}