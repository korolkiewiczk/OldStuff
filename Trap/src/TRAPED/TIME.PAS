{
  TRAP (r) Copyright (c) Kamil Korolkiewicz 2003
  Darmowa biblioteka do obslugi czasu stworzona na potzeby gry TRAP (r)
  Wszelkie prawa autorskie zastrze¾one
}
{$S-,G+}
unit time;

interface

{deklaracje stalych}

const

timer_in_milli_seconds = 3600879;        {54.945054}
timer_in_seconds       = 3600;           {0.054945054}
timer_in_minutes       = 60;             {0.00091575}
timer_in_hours         = 1;              {0.000015262}

{deklaracje typ¢w}

type

stimer = record
               old_time,current_time : longint;
end;
ptimer = ^stimer;
ttimer = ptimer;

var

clock : longint absolute 0:1132;

procedure newtimer (var timer : ttimer);
procedure disposetimer (var timer : ttimer);
procedure starttimer (var timer : ttimer);
procedure stoptimer (var timer : ttimer; typeoftimer : longint; var result : longint);

implementation

procedure newtimer;
begin
     new (timer);
end;

procedure disposetimer;
begin
     dispose (timer);
end;

procedure starttimer;
begin
asm
   mov ax,0
   mov es,ax
   mov di,1132
   {mov eax,es:[di]}  db 66h; db 26h; db 8bh; db 05;
   les di,timer
   les di,es:[di]
   {mov es:[di],eax}  db 66h; db 26h; db 89h; db 05;
end;
{timer^.old_time := meml [0000:1132];}
end;

procedure stoptimer;
begin
asm
   mov ax,0
   mov es,ax
   mov di,1132
   {mov eax,es:[di]}      db 66h; db 26h; db 8bh; db 05;
   les di,timer
   les di,es:[di]
   {mov es:[di+4],eax}    db 66h; db 26h; db 89h; db 45h; db 4;
   {sub eax,es:[di]}      db 66h; db 26h; db 2bh; db 05h;
   {mov ebx,[bp+10]}      db 66h; db 08bh; db 5eh; db 10;
   {mul ebx}              db 66h; db 0f7h; db 0e3h;
   {shr eax,16}           db 66h; db 0c1h; db 0e8h; db 16;
   les di,result
   {mov es:[di],eax}      db 66h; db 26h; db 89h; db 05;
end;
{timer^.current_time := meml [0000:1132];}
{result := (timer^.current_time-timer^.old_time)*typeoftimer;}
end;

end.