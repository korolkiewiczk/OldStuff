unit QREADMOD;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Menus, ExtCtrls,qreadabt,qreadpom,qreadtes, Buttons,qreadcfw;

type
  TForm1 = class(TForm)
    MainMenu1: TMainMenu;
    Plik1: TMenuItem;
    Otwrz1: TMenuItem;
    Zamknij1: TMenuItem;
    new: TMenuItem;
    N1: TMenuItem;
    Zapisz1: TMenuItem;
    N2: TMenuItem;
    Wycie1: TMenuItem;
    Memo1: TMemo;
    OpenDialog1: TOpenDialog;
    SaveDialog1: TSaveDialog;
    Zapiszjako1: TMenuItem;
    Pomoc1: TMenuItem;
    Oprogramie1: TMenuItem;
    Pomocprogramu1: TMenuItem;
    N3: TMenuItem;
    Opcje1: TMenuItem;
    Ustawieniaczcionki1: TMenuItem;
    N4: TMenuItem;
    Zawijaniewierszy1: TMenuItem;
    FontDialog1: TFontDialog;
    Szybkieczytanie1: TMenuItem;
    Uruchomtest1: TMenuItem;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    SpeedButton5: TSpeedButton;
    N5: TMenuItem;
    Lokalizacjaplikuconfiguracyjnego1: TMenuItem;
    procedure Wycie1Click(Sender: TObject);
    procedure resizeall(Sender: TObject);
    procedure newClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Zamknij1Click(Sender: TObject);
    procedure Otwrz1Click(Sender: TObject);
    procedure tosave(Sender: TObject);
    procedure Zapisz1Click(Sender: TObject);
    procedure Zapiszjako1Click(Sender: TObject);
    procedure openfile (namm : string);
    procedure savefile (namm : string);
    procedure Oprogramie1Click(Sender: TObject);
    procedure act(Sender: TObject);
    procedure Pomocprogramu1Click(Sender: TObject);
    procedure Ustawieniaczcionki1Click(Sender: TObject);
    procedure Zawijaniewierszy1Click(Sender: TObject);
    procedure Uruchomtest1Click(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
    procedure SpeedButton2Click(Sender: TObject);
    procedure SpeedButton3Click(Sender: TObject);
    procedure SpeedButton4Click(Sender: TObject);
    procedure SpeedButton5Click(Sender: TObject);
    procedure Lokalizacjaplikuconfiguracyjnego1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  filename : string;
  fileloaded : boolean;
  changedd : boolean;
  zawwier : boolean;

implementation

{$R *.DFM}

function saveit : boolean;
begin
  saveit := true;
  case application.messagebox ('Plik zosta³ zmieniony. Zapisaæ zmiany?','B³ad!',32+3) of
    idyes : form1.savefile (filename);
    idno : exit;
    idcancel : saveit := false;
  end;
end;

procedure setmenu;
begin
  if fileloaded then
  begin
    form1.mainmenu1.items[0].items[3].enabled := true;
    form1.mainmenu1.items[0].items[4].enabled := true;
    form1.mainmenu1.items[0].items[5].enabled := true;
    form1.speedbutton4.enabled := true;
    form1.speedbutton5.enabled := true;
  end
  else
  begin
    form1.mainmenu1.items[0].items[3].enabled := false;
    form1.mainmenu1.items[0].items[4].enabled := false;
    form1.mainmenu1.items[0].items[5].enabled := false;
    form1.speedbutton4.enabled := false;
    form1.speedbutton5.enabled := false;
  end;
end;

procedure tform1.openfile (namm : string);
begin
  if changedd then if not saveit then exit;
  try
  memo1.lines.loadfromfile (namm);
  except
    application.messagebox ('Nie mo¿na otworzyæ pliku!','B³ad!',16);
    exit;
  end;
  filename := namm;
  caption := 'Qread ['+namm+']';
  fileloaded := true;
  memo1.enabled := true;
  memo1.color := clwhite;
  if zawwier then
  memo1.scrollbars := ssvertical
  else
  memo1.scrollbars := ssboth;
  changedd := false;
  setmenu;
end;

procedure tform1.savefile (namm : string);
begin
  try
  memo1.lines.savetofile (namm);
  except
    application.messagebox ('Nie mo¿na zapisaæ pliku!','B³ad!',16);
    exit;
  end;
  filename := namm;
  caption := 'Qread ['+namm+']';
  changedd := false;
  setmenu;
end;

procedure TForm1.Wycie1Click(Sender: TObject);
begin
  if changedd then if not saveit then exit;
  Application.terminate;
end;

procedure TForm1.resizeall(Sender: TObject);
begin
  memo1.width := width-7;
  memo1.height := height-85;
end;

procedure TForm1.newClick(Sender: TObject);
begin
  if changedd then if not saveit then exit;
  memo1.lines.clear;
  filename := 'Noname.txt';
  caption := 'Qread ['+filename+']';
  fileloaded := true;
  memo1.enabled := true;
  memo1.color := clwhite;
  if zawwier then
  memo1.scrollbars := ssvertical
  else
  memo1.scrollbars := ssboth;
  changedd := false;
  setmenu;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  filename := '-';
  caption := 'Qread';
  fileloaded := false;
  memo1.enabled := false;
  memo1.color := clgray;
  memo1.scrollbars := ssnone;
  changedd := false;
  zawwier := true;
  setmenu;
end;

procedure TForm1.Zamknij1Click(Sender: TObject);
begin
  if changedd then if not saveit then exit;
  filename := '-';
  fileloaded := false;
  memo1.lines.clear;
  caption := 'Qread';
  memo1.enabled := false;
  memo1.color := clgray;
  memo1.scrollbars := ssnone;
  changedd := false;
  setmenu;
end;

procedure TForm1.Otwrz1Click(Sender: TObject);
begin
  if opendialog1.execute then openfile (opendialog1.filename);
end;

procedure TForm1.tosave(Sender: TObject);
begin
  changedd := true;
end;

procedure TForm1.Zapisz1Click(Sender: TObject);
begin
  savefile (filename);
end;

procedure TForm1.Zapiszjako1Click(Sender: TObject);
begin
  if savedialog1.execute then savefile (savedialog1.filename);
end;

procedure TForm1.Oprogramie1Click(Sender: TObject);
begin
  form2.show;
end;

procedure TForm1.act(Sender: TObject);
begin
  form2.Close;
  form3.close;
  form6.close;
end;

procedure TForm1.Pomocprogramu1Click(Sender: TObject);
begin
  form3.show;
end;

procedure TForm1.Ustawieniaczcionki1Click(Sender: TObject);
begin
  if fontdialog1.execute then memo1.font := fontdialog1.font;
end;

procedure TForm1.Zawijaniewierszy1Click(Sender: TObject);
begin
  if zawwier = false then
  begin
    zawwier := true;
    zawijaniewierszy1.checked := zawwier;
    if fileloaded then
    memo1.scrollbars := ssvertical;
  end
  else
  begin
    zawwier := false;
    zawijaniewierszy1.checked := zawwier;
    if fileloaded then
    memo1.scrollbars := ssboth;
  end;
end;

procedure TForm1.Uruchomtest1Click(Sender: TObject);
var
f : file;
begin
  if changedd then
  case application.messagebox ('Plik zosta³ zmieniony.'+
    'Aby podczas szybkiego czytania wyœwietlany by³ zmieniony tekst musz go zapisaæ.'+
    'Zapisaæ zmiany?','B³ad!',32+3) of
    idyes : form1.savefile (filename);
    idcancel : exit;
  end;
  filen := filename;
  assignfile (f,filename);
  {$I-}
  reset (f,1);
  {I+}
  if ioresult <> 0 then
  begin
       application.messagebox ('Nie mo¿na uruchomiæ testu. Nie mo¿na odnale¿æ pliku.','B³¹d!',16);
       exit;
  end;
  filess := filesize (f);
  if filess = 0 then form4.button2.enabled := false
  else
  form4.button2.enabled := true;
  getmem (form4.text,filess);
  blockread (f,form4.text^,filess);
  closefile (f);
  sc := katl+'QREAD.CFG';
  form4.show;
end;

procedure TForm1.SpeedButton1Click(Sender: TObject);
begin
  newClick(form1);
end;

procedure TForm1.SpeedButton2Click(Sender: TObject);
begin
  Otwrz1Click(form1);
end;

procedure TForm1.SpeedButton3Click(Sender: TObject);
begin
  Uruchomtest1Click(form1);
end;

procedure TForm1.SpeedButton4Click(Sender: TObject);
begin
  Zapiszjako1Click(form1);
end;

procedure TForm1.SpeedButton5Click(Sender: TObject);
begin
  Zamknij1Click(form1);
end;

procedure TForm1.Lokalizacjaplikuconfiguracyjnego1Click(Sender: TObject);
begin
  form6.show;
end;

end.
