#define TEXTCOLOR 0x00ffff
#define BARCOLOR 0xb0b0b0
#define MARKCOLOR 0xff0000
#define SHIFTMUL 20
#define SDELTA ((shift)? SHIFTMUL:1)
#define FILEBUFSIZE 65536
#define EDITBUFSIZE 64

class CView
{
      private:
      unsigned char *filebuf;
      int buf_pos; //pozycja wczytanego bufora
      
      int peekbuf(int pos); //pobierz z pliku bajt
      
      int pos; //pozycja w pliku
      
      int filesize; //rozmiar pliku
      
      char filename[1024];
      
      int noloadable; //czy mozna wczytywac do klasy pliki
      
      int viewno; //numer widoku
      
      int filetype; //typ pliku
      enum
      {
       File_Bin,
       File_Text
      };
      
      int vmode; //tryb podgladu pliku
      
      CWin *win;
      
      int shift; //czy shift
      
      int cursorpos[2];
      
      int VIEW_WIDTH;
      int VIEW_HEIGHT;
      int LINEWIDTH_BIN;
      int LINEWIDTH_HEX;
      int LINEWIDTH_DEC;
      int LINEWIDTH_OCT;

      int pagesize(int dir,int a=0);      
      void changepage(int dir);
      void changeline(int dir);
      
      
      char *conbuf;
      int conwidth,conheight;
      int conlen;
      int conposx,conposy;
      int conshift;
      
      void alloccon(int,int);
      void printf(const char *s,...);
      void putchar(char c);
      void presentcon();
      void cls() {conposx=-conshift; conposy=0; memset(conbuf,32,conlen);}
      
      int editboxon; //czy wlaczone jest pole edycji
      char editboxstr[EDITBUFSIZE];
      int editboxpos; //pozycja kursora
      int editboxtype; //typ wprowazanych danych
      
      char findseq[EDITBUFSIZE]; //ciag do znalezienia
      
      public:
             
      enum
      {
       Edit_Null,
       Edit_Pos,
       Edit_Find
      };
             
      enum
      {
       Text_Mode,
       Bin_Mode,
       Hex_Mode,
       Dec_Mode,
       Oct_Mode,
       Last_Mode
      };
      
      enum
      {
       Cmd_Null=0,
       Cmd_Quit,
       Cmd_PageUp,
       Cmd_PageDown,
       Cmd_LineUp,
       Cmd_LineDown,
       Cmd_Byte,
       Cmd_Txt,
       Cmd_Bin,
       Cmd_Hex,
       Cmd_Dec,
       Cmd_Oct,
       Cmd_NextMode,
       Cmd_ChangeFont,
       Cmd_ChangeFontSmall,
       Cmd_Left,
       Cmd_Right,
       Cmd_Home,
       Cmd_End,
       Cmd_PrintScreen
      };
      
      CView(int w,int h,void *buf);
      ~CView();
      int LoadFile(char *fn);
      void SetString(char *name,char *s);
      int SaveFile(char *fn);
      void SetVMode(int mode) {vmode=mode;}
      int GetVMode() {return vmode;}
      void Command(int c);
      void Show();
      int GetPos() {return pos;}
      int GetFileSize() {return filesize;}
      char *ModeString();
      void SetShift(int a) {shift=a;}
      void SetNoLoadable() {noloadable=1;}
      void SetNo(int a) {viewno=a;}
      char *GenStat();
      void Change(int w,int h,void *b);
      int IsEditBoxOn() {return editboxon;}
      void EnterEditBox(int type) {editboxtype=type; editboxon=1;}
      void EditBoxKey(int c);
      int EditBoxType() {return editboxtype;}
      void ExecuteBoxStr(int type);
      char *GetBoxText();
      void ChangeBuf(void *b) {win->Change(b);}
};
