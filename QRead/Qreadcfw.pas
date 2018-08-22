unit Qreadcfw;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TForm6 = class(TForm)
    Label1: TLabel;
    Edit1: TEdit;
    Button2: TButton;
    Button3: TButton;
    procedure FormCreate(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form6: TForm6;
  katl : string;

implementation

{$R *.DFM}

procedure TForm6.FormCreate(Sender: TObject);
begin
  getdir (0,katl);
  edit1.text := katl;
end;

procedure TForm6.Button2Click(Sender: TObject);
begin
  katl := edit1.text;
  close;
end;

procedure TForm6.Button3Click(Sender: TObject);
begin
  close;
end;

end.
