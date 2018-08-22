unit qreadtes;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Spin,qreadentr, ExtCtrls;

const

maxcol = 8;

type
  TForm4 = class(TForm)
    Label1: TLabel;
    SpinEdit1: TSpinEdit;
    Label2: TLabel;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    Label3: TLabel;
    ListBox1: TListBox;
    Label4: TLabel;
    ListBox2: TListBox;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Memo1: TMemo;
    FontDialog1: TFontDialog;
    Button5: TButton;
    Panel1: TPanel;
    Panel2: TPanel;
    procedure Button1Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure fm(Sender: TObject; var Action: TCloseAction);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure zm1(Sender: TObject);
    procedure zm2(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    text : ^tabch;
  end;


var
  Form4: TForm4;
  filen : string;
  filess : longint;
  speed : integer;
  colb,colt : integer;
  fnt : tfont;
  typ : byte;
  tl : boolean;
  sc : string;

implementation

{$R *.DFM}


procedure lcfg;
var
f : file;
begin
  assignfile (f,sc);
  {$I-}
  reset (f,1);
  {$I+}
  if ioresult <> 0 then exit;
  try
  blockread (f,speed,2);
  blockread (f,colb,2);
  blockread (f,colt,2);
  //----------------------------------
  case colb of
  0 : form4.panel1.color := clwhite;
  1 : form4.panel1.color := clblack;
  2 : form4.panel1.color := clred;
  3 : form4.panel1.color := clblue;
  4 : form4.panel1.color := clgreen;
  5 : form4.panel1.color := clyellow;
  6 : form4.panel1.color := clsilver;
  7 : form4.panel1.color := clgray;
  end;
  //----------------------------------
  case colt of
  0 : form4.panel2.color := clwhite;
  1 : form4.panel2.color := clblack;
  2 : form4.panel2.color := clred;
  3 : form4.panel2.color := clblue;
  4 : form4.panel2.color := clgreen;
  5 : form4.panel2.color := clyellow;
  6 : form4.panel2.color := clsilver;
  7 : form4.panel2.color := clgray;
  end;
  //----------------------------------
  form4.spinedit1.value := speed;
  except
  application.messagebox ('Wyj¹tek podczas odczytu pliku konfiguracyjnego!','Wyj¹tek!',16);
  end;
  //----------------------------------
  tl := true;
end;

procedure scfg;
var
f : file;
i : integer;
begin
  assignfile (f,sc);
  rewrite (f,1);
  speed := form4.spinedit1.value;
  for i := 0 to maxcol-1 do if form4.listbox1.selected [i] = true then colb := i;
  for i := 0 to maxcol-1 do if form4.listbox2.selected [i] = true then colt := i;
  blockwrite (f,speed,2);
  blockwrite (f,colb,2);
  blockwrite (f,colt,2);
end;

procedure TForm4.Button1Click(Sender: TObject);
begin
  if fontdialog1.execute then fnt := fontdialog1.font;
end;

procedure TForm4.Button3Click(Sender: TObject);
begin
  close;
end;

procedure TForm4.fm(Sender: TObject; var Action: TCloseAction);
begin
  freemem (text);
end;

procedure TForm4.Button4Click(Sender: TObject);
begin
  scfg;
end;

procedure TForm4.Button5Click(Sender: TObject);
begin
  lcfg;
end;

procedure TForm4.FormCreate(Sender: TObject);
begin
  speed := 0;
  colt := 1;
  colb := 0;
  fnt := font;
  typ := 0;
  panel1.color := clwhite;
  panel2.color := clblack;
  tl := false;
  getdir (0,sc);
  sc := sc+'\QREAD.CFG';
end;

procedure TForm4.Button2Click(Sender: TObject);
var
i : byte;
c : tcolor;
begin
  speed := spinedit1.value;
  sp := speed;
  if sp mod 5 <> 0 then
  begin
    sp := sp div 5;
    sp := sp*5;
    speed := sp;
    spinedit1.value := speed;
  end;
  if not tl then
  begin
    for i := 0 to maxcol-1 do if listbox1.selected [i] = true then colb := i;
    for i := 0 to maxcol-1 do if listbox2.selected [i] = true then colt := i;
  end;
  form5.font := fnt;
  case colb of
  0 : form5.color := clwhite;
  1 : form5.color := clblack;
  2 : form5.color := clred;
  3 : form5.color := clblue;
  4 : form5.color := clgreen;
  5 : form5.color := clyellow;
  6 : form5.color := clsilver;
  7 : form5.color := clgray;
  end;
  case colt of
  0 : form5.font.color := clwhite;
  1 : form5.font.color := clblack;
  2 : form5.font.color := clred;
  3 : form5.font.color := clblue;
  4 : form5.font.color := clgreen;
  5 : form5.font.color := clyellow;
  6 : form5.font.color := clsilver;
  7 : form5.font.color := clgray;
  end;
  //form5.col2 := colt;
  if radiobutton1.checked = true then typ := 0;
  if radiobutton2.checked = true then typ := 1;
  t := typ;
  form5.pt := addr (text^);
  form5.timer1.interval := 1001-sp;
  form5.timer1.enabled := true;
  l := 0;
  s := filess;
  form5.canvas.font := form5.font;
  form5.bfnt := form5.font;
  form5.afnt := font;
  form5.show;
end;

procedure TForm4.zm1(Sender: TObject);
var
i : byte;
begin
  for i := 0 to maxcol-1 do if listbox1.selected [i] = true then colb := i;
  case colb of
  0 : panel1.color := clwhite;
  1 : panel1.color := clblack;
  2 : panel1.color := clred;
  3 : panel1.color := clblue;
  4 : panel1.color := clgreen;
  5 : panel1.color := clyellow;
  6 : panel1.color := clsilver;
  7 : panel1.color := clgray;
  end;
  //button3.caption := inttostr (colb);
  tl := false;
end;

procedure TForm4.zm2(Sender: TObject);
var
i : byte;
begin
  for i := 0 to maxcol-1 do if listbox2.selected [i] = true then colt := i;
  case colt of
  0 : panel2.color := clwhite;
  1 : panel2.color := clblack;
  2 : panel2.color := clred;
  3 : panel2.color := clblue;
  4 : panel2.color := clgreen;
  5 : panel2.color := clyellow;
  6 : panel2.color := clsilver;
  7 : panel2.color := clgray;
  end;
  tl := false;
  //button4.caption := inttostr (colb);
end;

end.
