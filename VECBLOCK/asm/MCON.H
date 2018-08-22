//mcon.h mcon/stub32 dla DJGPP

extern char MCON_CUR_X,MCON_CUR_Y,MCON_TEXT_ATTR;

//prototypy funkcji

extern unsigned short MReadKey();
extern unsigned short MGetKey();
extern void MGotoXY(int,int);
extern void MPrint(int,int,char*);
extern void MCls();
