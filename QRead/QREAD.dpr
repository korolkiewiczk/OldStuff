program QREAD;

uses
  Forms,
  QREADMOD in 'QREADMOD.pas' {Form1},
  qreadabt in 'qreadabt.pas' {Form2},
  qreadpom in 'qreadpom.pas' {Form3},
  qreadtes in 'qreadtes.pas' {Form4},
  qreadentr in 'qreadentr.pas' {Form5},
  Qreadcfw in 'Qreadcfw.pas' {Form6};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TForm2, Form2);
  Application.CreateForm(TForm3, Form3);
  Application.CreateForm(TForm4, Form4);
  Application.CreateForm(TForm5, Form5);
  Application.CreateForm(TForm6, Form6);
  Application.Run;
end.
