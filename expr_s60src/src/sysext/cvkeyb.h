//cvkeyb.h

#define VKEYB_POS 10
//do modyfikacji w s60
#define VKEYB_K_1 2
#define VKEYB_K_0 11
#define VKEYB_K_ASTERISK 224
#define VKEYB_K_HASH 225
#define VKEYB_K_NPART 5
#define VKEYB_CELLSIZE 17

class CVKeyb:public CWin
{
 private:
 static unsigned char vkeymap[3*3*VKEYB_K_NPART][2];
 static unsigned char extrakeymap[3*3*2][2];
 static char *extranames[3*3*2];
 int part;
 int active;
 int c;
 public:
 CVKeyb(int w,int h,void *b):CWin(w,h,b)
 {
  part=0;
  active=0;
 }
 void KeyFunc(int k);
 int GetScan();
 int IsActive() {return active;}
 void SetActive() {active=1;}
 void Draw(int shift);
};
