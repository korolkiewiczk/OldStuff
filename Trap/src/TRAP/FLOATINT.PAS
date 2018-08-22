const
floatintshift = 8;
f_i_m = 256;

type

floatint = longint;

procedure floatint_add (var out : floatint; arg1,arg2 : floatint);
inline ($66/$5B/$66/$58/$5F/$07/$66/$03/$C3/$66/$26/$89/$05);
{                               -----------
asm
   pop ebx
   pop eax
   pop di
   pop es
   add eax,ebx
   mov es:[di],eax
end;
begin
     out := arg1+arg2;
end;}
procedure floatint_sub (var out : floatint; arg1,arg2 : floatint);
inline ($66/$5B/$66/$58/$5F/$07/$66/$2B/$C3/$66/$26/$89/$05);
{                               -----------
asm
   pop ebx
   pop eax
   pop di
   pop es
   sub eax,ebx
   mov es:[di],eax
end;
begin
     out := arg1-arg2;
end;}
procedure floatint_mul (var out : floatint; arg1,arg2 : floatint);
inline ($66/$5B/$66/$58/$5F/$07/$66/$F7/$EB/$66/$0F/$AC/$D0/floatintshift/$66/$26/$89/$05);
{                               ----------- -----------------------------
asm
   pop ebx
   pop eax
   pop di
   pop es
   imul ebx
   shrd eax,edx,floatintshift
   mov es:[di],eax
end;
begin
     out := arg1*arg2 shr floatintshift;
end;}
procedure floatint_div (var out,outmod : floatint; arg1,arg2 : floatint);
inline ($66/$5B/$66/$58/$5F/$07/$66/$33/$D2/$66/$8B/$C8/$66/$C1/$E0/floatintshift/$66/$0F/$A4/$CA/floatintshift/$66/$F7/$FB/
$66/$26/$89/$15/$5F/$07/$66/$26/$89/$05);
{
asm
   pop ebx
   pop eax
   pop di
   pop es
   xor edx,edx
   mov ecx,eax
   shl eax,floatintshift
   shld edx,ecx,floatintshift
   idiv ebx
   mov es:[di],edx
   pop di
   pop es
   mov es:[di],eax
end;
begin
     out := (arg1 shl floatintshift) div arg2;
end;}
procedure floatint_sqr (var out : floatint; arg1 : floatint);
inline ($66/$58/$5F/$07/$66/$F7/$E8/$66/$0F/$AC/$D0/floatintshift/$66/$26/$89/$05);
{                               ----------- -----------------------------
asm
   pop eax
   pop di
   pop es
   imul eax
   shrd eax,edx,floatintshift
   mov es:[di],eax
end;
begin
     out := arg1*arg1 shr floatintshift;
end;}
procedure floatint_chs (var out : floatint; arg1 : floatint);
inline ($66/$58/$5F/$07/$66/$F7/$D8/$66/$26/$89/$05);
{                       ----------- -------
asm
   pop eax
   pop di
   pop es
   neg eax
   mov es:[di],eax
end;
begin
     out := -arg1;
end;}
procedure floatint_rnd (var out : integer; arg1 : floatint);
inline ($66/$58/$5F/$07/$66/$C1/$F8/floatintshift/$26/$89/$05);
{                       -------------------------
asm
   pop eax
   pop di
   pop es
   sar eax,floatintshift
   mov es:[di],ax
end;
begin
     out := arg1 shr floatintshift;
end;}