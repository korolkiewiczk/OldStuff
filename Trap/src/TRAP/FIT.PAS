{$G+}
uses crt;
{$I FLOATINT.PAS}

var

a,modu : floatint;
b,c : integer;

begin
     clrscr;
     floatint_add (a,256*55,256*33);
     floatint_rnd (b,a);
     writeln ('floatint_add: ',a,' ',b);
     floatint_sub (a,256*90,256*10);
     floatint_rnd (b,a);
     writeln ('floatint_sub: ',a,' ',b);
     floatint_mul (a,256*10,256*12);
     floatint_rnd (b,a);
     writeln ('floatint_mul: ',a,' ',b);
     floatint_div (a,modu,256*1,256*20);
     floatint_rnd (b,a);
     {floatint_rnd (modu,modu);}
     writeln ('floatint_div: ',a,' ',modu,' ',b);
     floatint_sqr (a,256*20);
     floatint_rnd (b,a);
     writeln ('floatint_sqr: ',a,' ',b);
     floatint_chs (a,256*20);
     floatint_rnd (b,a);
     writeln ('floatint_chs: ',a,' ',b);
     readln;
     {}
     clrscr;
     floatint_add (a,256*-55,256*33);
     floatint_rnd (b,a);
     writeln ('floatint_add: ',a,' ',b);
     floatint_sub (a,256*90,256*-10);
     floatint_rnd (b,a);
     writeln ('floatint_sub: ',a,' ',b);
     floatint_mul (a,256*-10,256*12);
     floatint_rnd (b,a);
     writeln ('floatint_mul: ',a,' ',b);
     floatint_mul (a,256*1230,256*2312);
     floatint_rnd (b,a);
     writeln ('floatint_mul: ',a,' ',b);
     floatint_div (a,modu,256*100,256*-20);
     floatint_rnd (b,a);
     floatint_rnd (c,modu);
     writeln ('floatint_div: ',a,' ',c,' ',b);
     floatint_sqr (a,256*-20);
     floatint_rnd (b,a);
     writeln ('floatint_sqr: ',a,' ',b);
     floatint_chs (a,256*-20);
     floatint_rnd (b,a);
     writeln ('floatint_chs: ',a,' ',b);
     readln;
end.