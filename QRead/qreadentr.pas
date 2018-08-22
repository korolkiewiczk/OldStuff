unit qreadentr;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls;

const
tabspik : array [0..18] of string [4] = ('I','A','ORAZ','LUB','NA','POD','W','Z','¯E','¿E','DO',
                                         'AND','OR','AT','FOR','THE','AN','IN','ON');

type
  tabch = array [0..65535] of char;
  TForm5 = class(TForm)
    Timer1: TTimer;
    procedure ext(Sender: TObject; var Key: Char);
    procedure Timer1Timer(Sender: TObject);
    procedure sdt;
    procedure mcli(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    pt : ^tabch;
    afnt,bfnt : tfont;
  end;

var
  Form5: TForm5;
  sp : word;
  t : byte;
  l : integer;
  s : integer;
  ps : string;
  st : string;

implementation

{$R *.DFM}

function newread : boolean;
begin
  newread := false;
  if form5.timer1.enabled then
  begin
  form5.canvas.fillrect (rect (0,0,form5.clientwidth,form5.clientheight));
  form5.timer1.enabled := false;
  case application.messagebox ('Program doszed³ do koñca tekstu. Czy zacz¹æ od nowa?','Qread - test',64+4) of
  idyes : newread := true;
  idno : newread := false;
  end;
  form5.timer1.enabled := true;
  end;
end;

function upcasestring (st : string) : string;
var
i : integer;
ast : string;
begin
  ast := '';
  for i := 1 to length (st) do ast := ast+(upcase (st[i]));
  upcasestring := ast;
end;

procedure render (var sa : string);
var
l2 : word;
ch,ch1 : char;
s1,s2 : string;
i : byte;
begin
  s1 := '';
  s2 := '';
  repeat
    if l > s then
    begin
      if newread then l := 0
      else
      begin
        form5.timer1.enabled := false;
        form5.close;
      end;
    end;
    ch1 := form5.pt^[l];
    if (ch1 <= #32) then inc (l)
    else
    break;
  until 1=2;
  if t = 0 then
  begin
    repeat
      if l > s then
      begin
        s1 := '';
        s2 := '';
        if newread then l := 0
        else
        begin
          form5.timer1.enabled := false;
          form5.close;
        end;
      end;
      ch := form5.pt^[l];
      if (ch > #32) then s1 := s1+ch;
      inc (l);
    until (ch <= #32) or (ch = '!') or (ch = '?');
    l2 := l;
    repeat
      if l > s then break;
      ch1 := form5.pt^[l];
      if (ch1 <= #32) then inc (l)
      else
      break;
    until 1=2;
    repeat
      if l > s then break;
      ch1 := form5.pt^[l];
      if (ch1 > #32) then s2 := s2+ch1;
      inc (l);
    until (ch1 <= #32) or (ch1 = '!') or (ch1 = '?');
    for i := 0 to 18 do if upcasestring (s1) = tabspik [i] then
    begin
      sa := s1+' '+s2;
      exit;
    end;
    for i := 1 to length (s2) do
    begin
      ch := s2[i];
      if ((ch >= #48) and (ch <= #57)) then continue
      else
      break;
    end;
    if i = length (s2)+1 then
    begin
      sa := s1+' '+s2;
      exit;
    end;
    sa := s1;
    l := l2;
  end
  else
  begin
    repeat
      if l > s then
      begin
        s1 := '';
        if newread then l := 0
        else
        begin
          form5.timer1.enabled := false;
          form5.close;
        end;
      end;
      ch := form5.pt^[l];
      if (ch >= #32) then s1 := s1+ch;
      inc (l);
    until (ch = '.') or (ch = '!') or (ch = '?') or (ch < #32);
    sa := s1;
  end;
end;

procedure tform5.sdt;
begin
  timer1.enabled := false;
  canvas.font := bfnt;
  font := bfnt;
  close;
end;

procedure TForm5.ext(Sender: TObject; var Key: Char);
var
pcolor : tcolor;
begin
  case key of
  #27:sdt;
  '+' : if sp <= 995 then inc (sp,5);
  '-' : if sp > 0 then dec (sp,5);
  's' : timer1.enabled := false;
  'S' : timer1.enabled := false;
  'r' : timer1.enabled := true;
  'R' : timer1.enabled := true;
  '0'..'9' : sp := (ord (key)-48)*100;
  'p' : if t = 1 then t := 0 else t := 1;
  'P' : if t = 1 then t := 0 else t := 1;
  else
  exit;
  end;
  pcolor := canvas.brush.color;
  canvas.brush.color := bfnt.color;
  canvas.font := afnt;
  case bfnt.color of
  clwhite  : canvas.font.color := clblack;
  clblack  : canvas.font.color := clwhite;
  clred    : canvas.font.color := clblack;
  clblue   : canvas.font.color := clyellow;
  clgreen  : canvas.font.color := clblue;
  clyellow : canvas.font.color := clblue;
  clsilver : canvas.font.color := clblack;
  clgray   : canvas.font.color := clwhite;
  end;
  canvas.textout (4,-canvas.font.height*14,'                           ');
  canvas.textout (4,-canvas.font.height*14,'szybkoœæ   '+inttostr (sp));
  canvas.brush.color := pcolor;
  canvas.font := bfnt;
  timer1.interval := 1001-sp;
end;

procedure TForm5.Timer1Timer(Sender: TObject);
var
pcolor : tcolor;
s2 : string;
i : integer;
begin
  canvas.fillrect (rect (0,clientheight div 10,clientwidth,clientheight));
  render (st);
  canvas.font := afnt;
  pcolor := canvas.brush.color;
  canvas.brush.color := bfnt.color;
  case bfnt.color of
  clwhite  : canvas.font.color := clblack;
  clblack  : canvas.font.color := clwhite;
  clred    : canvas.font.color := clblack;
  clblue   : canvas.font.color := clyellow;
  clgreen  : canvas.font.color := clblue;
  clyellow : canvas.font.color := clblue;
  clsilver : canvas.font.color := clblack;
  clgray   : canvas.font.color := clwhite;
  end;
  canvas.rectangle (0,0,clientwidth div 6,clientheight div 4);
  canvas.textout (4,1,'[ESC]   zakoñcz');
  canvas.textout (4,-canvas.font.height*2,'[+]   zwiêksz szybkoœæ');
  canvas.textout (4,-canvas.font.height*4,'[-]   zmiejsz szybkoœæ');
  canvas.textout (4,-canvas.font.height*6,'[S]   zatrzymaj');
  canvas.textout (4,-canvas.font.height*8,'[R]   dalej');
  canvas.textout (4,-canvas.font.height*10,'[P]   prze³¹cz miêdzy czytaniem');
  canvas.textout (4,-canvas.font.height*12,'      s³owami a zdaniami');
  canvas.textout (4,-canvas.font.height*14,'szybkoœæ   '+inttostr (sp));
  canvas.font := bfnt;
  canvas.brush.color := pcolor;
  if ((length (st)*(-font.height) div 5) < (clientwidth div 5)) or (t = 0) then
  canvas.textout (clientwidth div 2 - ((length (st)*(-font.height) div 4)),clientheight div 2 - (-font.height div 2),st)
  else
  for i := length (st) div 2 to length (st) do if st[i] <= #32 then
  begin
    s2 := copy (st,i,length (st)-i+1);
    delete (st,i,length (st)-i+1);
    canvas.textout (clientwidth div 2 - ((length (st)*(-font.height) div 5)),clientheight div 2 - (-font.height),st);
    canvas.textout (clientwidth div 2 - ((length (s2)*(-font.height) div 5)),clientheight div 2 + (-font.height div 3),s2);
    break;
  end;
end;


procedure TForm5.mcli(Sender: TObject);
begin
  sdt;
end;

procedure TForm5.FormCreate(Sender: TObject);
begin
  afnt := font;
  bfnt := font;
end;

end.
