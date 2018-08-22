type

keyname = record
                keyscan : byte;
                keyname : pchar;
end;

const

{(keyscan:42; keyname:'LSHIFT'),
(keyscan:54; keyname:'RSHIFT'),}
keysnamesn = 30;
keysnames : array [0..keysnamesn] of keyname = (
(keyscan:72; keyname:'UP ARROW'),
(keyscan:80; keyname:'DOWN ARROW'),
(keyscan:75; keyname:'LEFT ARROW'),
(keyscan:77; keyname:'RIGHT ARROW'),
(keyscan:15; keyname:'TAB'),
(keyscan:58; keyname:'CAPSLOCK'),
(keyscan:29; keyname:'CTRL'),
(keyscan:56; keyname:'ALT'),
(keyscan:14; keyname:'BACKSPACE'),
(keyscan:41; keyname:'~'),
(keyscan:82; keyname:'INSERT'),
(keyscan:71; keyname:'HOME'),
(keyscan:73; keyname:'PAGEUP'),
(keyscan:83; keyname:'DELETE'),
(keyscan:79; keyname:'END'),
(keyscan:81; keyname:'PAGEDOWN'),
(keyscan:28; keyname:'ENTER'),
(keyscan:57; keyname:'SPACE'),
(keyscan:69; keyname:'NUM LOCK'),
(keyscan:58; keyname:'CAPS LOCK'),
(keyscan:70; keyname:'SCROLL LOCK'),
(keyscan:12; keyname:'-'),
(keyscan:13; keyname:'='),
(keyscan:43; keyname:'\'),
(keyscan:26; keyname:'['),
(keyscan:27; keyname:']'),
(keyscan:39; keyname:';'),
(keyscan:40; keyname:''''),
(keyscan:51; keyname:','),
(keyscan:52; keyname:'.'),
(keyscan:53; keyname:'/'));

ptab1 : array [0..9] of char = ('Q','W','E','R','T','Y','U','I','O','P');
ptab2 : array [0..8] of char = ('A','S','D','F','G','H','J','K','L');
ptab3 : array [0..6] of char = ('Z','X','C','V','B','N','M');

procedure convertscan2pchar (scan : byte; var p : pchar);
var
c : char;
i : byte;
begin
     c := #0;
     p[0] := #0;
     if scan = 11 then c := '0' else
     if scan in [2..10] then c := chr ((scan-1)+$30) else
     if scan in [16..25] then c := ptab1[scan-16] else
     if scan in [30..38] then c := ptab2[scan-30] else
     if scan in [44..50] then c := ptab3[scan-44] else
     begin
          for i := 0 to keysnamesn do if scan = keysnames[i].keyscan then
          begin
               strcopy (p,keysnames[i].keyname);
               break;
          end;
     end;
     if c <> #0 then
     begin
          p[0] := c;
          p[1] := #0;
     end;
end;