#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SCRWIDTH 240
#define SCRHEIGHT 320
#define BPP 32

#define NBLOCKS 7
#define BLOCKSIZE 12
#define WSIZEX 10
#define WSIZEY 24
#define WPOSX 90
#define WPOSY 4
#define WBORDERCOLOR2 0xffffffff
#define WBORDERCOLOR1 0x0
#define FASTFALLSPEED 25
#define MAXCOLORS 4

#define INFOCOLOR 0xffffffff
#define INFOCOLORF 0x70707070
#define BKCOLOR 0x303030

#define KEYUP 16
#define KEYDOWN 17
#define KEYLEFT 14
#define KEYRIGHT 15
#define KEYEXIT 165
#define KEYRESET 164
#define KEYENTER 167
#define KEYPAUSE 196
#define KEYACCTIMER 48

#define ALTKEYUP 50
#define ALTKEYDOWN 56
#define ALTKEYLEFT 52
#define ALTKEYRIGHT 54
#define ALTKEYENTER 53

#include "font3X5.h"

char well[WSIZEY][WSIZEX];

#define POINTABMAXTIME 4000
int pointtab[WSIZEY][2]; //[][0]-punkty,[][1]-timer

//def-dla pozycji 0 rozmiar. dla pozycji>0: index,pozycja x,y srodka

char block1[2][2]=
{
  {1,1},
  {1,1}
};

char block1def[5][3]=
{
 {0,2,2},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
};

char block2[4][3][3]=
{
 {
  {0,0,0},
  {0,1,0},
  {1,1,1}
 },
 {
  {1,0,0},
  {1,1,0},
  {1,0,0}
 },
 {
  {1,1,1},
  {0,1,0},
  {0,0,0}
 },
 {
  {0,0,1},
  {0,1,1},
  {0,0,1}
 }
};

char block2def[5][3]=
{
 {0,3,3},{0,1,2},{1,0,1},{2,1,0},{3,2,1}
};

char block3[2][4][4]=
{
 {
  {0,0,1,0},
  {0,0,1,0},
  {0,0,1,0},
  {0,0,1,0}
 },
 {
  {0,0,0,0},
  {0,0,0,0},
  {1,1,1,1},
  {0,0,0,0}
 }
};

char block3def[5][3]=
{
 {0,4,4},{0,2,2},{1,2,2},{0,2,2},{1,2,2}
};

char block4[2][3][3]=
{
 {
  {0,0,0},
  {0,1,1},
  {1,1,0}
 },
 {
  {1,0,0},
  {1,1,0},
  {0,1,0}
 }
};

char block4def[5][3]=
{
 {0,3,3},{0,1,1},{1,1,1},{0,1,1},{1,1,1}
};

char block5[2][3][3]=
{
 {
  {0,0,0},
  {1,1,0},
  {0,1,1}
 },
 {
  {0,1,0},
  {1,1,0},
  {1,0,0}
 }
};

char block5def[5][3]=
{
 {0,3,3},{0,1,1},{1,1,1},{0,1,1},{1,1,1}
};

char block6[4][3][3]=
{
 {
  {0,1,1},
  {0,0,1},
  {0,0,1}
 },
 {
  {0,0,0},
  {0,0,1},
  {1,1,1}
 },
 {
  {1,0,0},
  {1,0,0},
  {1,1,0}
 },
 {
  {1,1,1},
  {1,0,0},
  {0,0,0}
 }
};

char block6def[5][3]=
{
 {0,3,3},{0,1,1},{1,1,1},{2,1,1},{3,1,1}
};

char block7[4][3][3]=
{
 {
  {1,1,0},
  {1,0,0},
  {1,0,0}
 },
 {
  {1,1,1},
  {0,0,1},
  {0,0,0}
 },
 {
  {0,0,1},
  {0,0,1},
  {0,1,1}
 },
 {
  {0,0,0},
  {1,0,0},
  {1,1,1}
 }
};

char block7def[5][3]=
{
 {0,3,3},{0,1,1},{1,1,1},{2,1,1},{3,1,1}
};



char *blocks[NBLOCKS][2]=
{
 {(char*)block1,(char*)block1def},
 {(char*)block2,(char*)block2def},
 {(char*)block3,(char*)block3def},
 {(char*)block4,(char*)block4def},
 {(char*)block5,(char*)block5def},
 {(char*)block6,(char*)block6def},
 {(char*)block7,(char*)block7def}
};

int blockcolors[MAXCOLORS]=
{
 0xff0000,0x00ff00,0x0000ff,0xffff00
};

void inline memset32(void *p,int a,int b) 
{
 if (b<=0) return;
 while (b--)
 {
  *(int*)p=a;
  p=(void*)((int)p+4);
 }
}

void bar(int x1,int y1,int x2,int y2,int color)
{
 if (x1<0) x1=0;
 if (x2<0) x2=0;
 if (x1>=SCRWIDTH) x1=SCRWIDTH-1;
 if (x2>=SCRWIDTH) x2=SCRWIDTH-1;
 if (y1<0) y1=0;
 if (y2<0) y2=0;
 if (y1>=SCRHEIGHT) y1=SCRHEIGHT-1;
 if (y2>=SCRHEIGHT) y2=SCRHEIGHT-1;
 int i;
 for (i=y1; i<=y2; i++)
 {
  memset32((void*)((int)buf+(i*SCRWIDTH+x1)*4),color,x2-x1+1);
 }
}

void ClearBuf(int c)
{
 memset32(buf,c,SCRWIDTH*SCRHEIGHT);
}

void getblockinfo(int no,int dir,int &bsx,int &bsy,int &bposx,int &bposy,char **bimg)
{
 dir&=3;
 dir++;
 char (*defptr)[3]=(char(*)[3])blocks[no%NBLOCKS][1];
 bsx=defptr[0][1];
 bsy=defptr[0][2];
 bposx=defptr[dir][1];
 bposy=defptr[dir][2];
 *bimg=(char*)((int)(blocks[no%NBLOCKS][0])+bsx*bsy*(defptr[dir][0]));
}

//m-czy nie sprawdzac czy w studni
void drawblock(int x,int y,int no,int dir,int color,int m=0)
{
 int bsx;
 int bsy;
 int bposx;
 int bposy;
 char *bimg;
 getblockinfo(no,dir,bsx,bsy,bposx,bposy,&bimg);
 //DEBUG_Printf("%d %d %d %d %d %d %x\n",sx,sy,bsx,bsy,bposx,bposy,bimg);
 for (int i=0; i<bsy; i++)
 {
 for (int j=0; j<bsx; j++) 
 if (*(bimg+bsx*i+j))
 {
  //DEBUG_Printf("*");
  int bposx0=j-bposx+x;
  int bposy0=i-bposy+y;
  if ((m)||((!m)&&(bposx0>=0)&&(bposy0>=0)&&(bposx0<WSIZEX)&&(bposy0<WSIZEY)))
  {
   int posx=WPOSX+bposx0*BLOCKSIZE;
   int posy=WPOSY+bposy0*BLOCKSIZE;
   bar(posx+1,posy+1,posx+BLOCKSIZE-1,posy+BLOCKSIZE-1,color);
  }
 }
 //else
 //DEBUG_Printf("0");
 //DEBUG_Printf("\n");
 }
}

void hline(int x1,int y,int x2,int col)
{
 if ((y<0)||(y>=SCRHEIGHT)) return;
 if (x1<0) x1=0;
 if (x2>=SCRWIDTH) x2=SCRWIDTH-1;
 if (x2-x1+1<=0) return;
 memset32((void*)((int)buf+(x1+y*SCRWIDTH)*4),col,x2-x1+1);
}

void vline(int x,int y1,int y2,int col)
{
 if ((x<0)||(x>=SCRWIDTH)) return;
 if (y1<0) y1=0;
 if (y2>=SCRHEIGHT) y2=SCRHEIGHT-1;
 int i=y2-y1+1;
 if (i<=0) return;
 unsigned int *bf;
 bf=(unsigned int*)((x+y1*SCRWIDTH)*4+(int)buf);
 while (i--)
 {
  *bf=col;
  bf+=SCRWIDTH;
 }
}

int gamespeed=500; //ms
int currgamespeed;
int posx,posy,dir;
int block,blockcolor,nextblock,nextblockcolor;
int lastmove=0;
int mode;
float lasttime;
int fallmode=0;
int points;
int gamemode=0;
int paused=0;
float totaltime,prevtime;
int acctimer=0;

void drawwell()
{
 hline(WPOSX,WPOSY,WPOSX+BLOCKSIZE*WSIZEX,WBORDERCOLOR1);
 hline(WPOSX,WPOSY+BLOCKSIZE*WSIZEY,WPOSX+BLOCKSIZE*WSIZEX,WBORDERCOLOR2);
 vline(WPOSX,WPOSY,WPOSY+BLOCKSIZE*WSIZEY,WBORDERCOLOR1);
 vline(WPOSX+BLOCKSIZE*WSIZEX,WPOSY,WPOSY+BLOCKSIZE*WSIZEY,WBORDERCOLOR2);
 if (paused) return;
 int lines=1;
 for (int i=WSIZEY-1; i>=0; i--)
 {
  for (int j=0; j<WSIZEX; j++)
  {
   if (well[i][j]) 
   bar(WPOSX+j*BLOCKSIZE+1,
      WPOSY+i*BLOCKSIZE+1,
      WPOSX+j*BLOCKSIZE+BLOCKSIZE-1,
      WPOSY+i*BLOCKSIZE+BLOCKSIZE-1,
      blockcolors[well[i][j]-1]);
  }
  if (pointtab[i][0])
  {
   char bf[32];
   if (lines>1)
   {
    sprintf(bf,"+%d*%d",pointtab[i][0],lines);
   }
   else
   {
    sprintf(bf,"+%d",pointtab[i][0]);
   }
   DrawText3X5(WPOSX+BLOCKSIZE*WSIZEX+2,WPOSY+i*BLOCKSIZE+BLOCKSIZE/2-2,INFOCOLOR,bf,1);
   if (GetTime()*1000-pointtab[i][1]>POINTABMAXTIME)pointtab[i][0]=0;
   lines<<=1;
  }
 }
}

void newblock()
{
 posx=WSIZEX/2;
 posy=0;
 dir=0;
 block=nextblock;
 blockcolor=nextblockcolor;

 nextblock=rand()%NBLOCKS;
 nextblockcolor=rand()%MAXCOLORS;
 
 fallmode=0;
 GetKey(80);
}

void resetgame()
{
 memset(well,0,sizeof(well));
 nextblock=rand()%NBLOCKS;
 nextblockcolor=rand()%MAXCOLORS;
 newblock();
 points=0;
 nextblock=rand()%NBLOCKS;
 nextblockcolor=rand()%MAXCOLORS;
 paused=0;
 totaltime=0;
 prevtime=GetTime();
}

int checkcol0()
{
 int bsx,bsy,bposx,bposy;
 char *bimg;
 getblockinfo(block,dir,bsx,bsy,bposx,bposy,&bimg);
 int ret=0;
 for (int i=0; i<bsy; i++)
 for (int j=0; j<bsx; j++)
 {
  int bposx0=j-bposx+posx;
  int bposy0=i-bposy+posy;
  if ((bposx0>=0)&&(bposy0>=0)&&(bposx0<WSIZEX)&&(bposy0<WSIZEY)&&(*(bimg+bsx*i+j)))
  {
   if (well[bposy0][bposx0]) ret=2;
  }
  else
  if (*(bimg+bsx*i+j))
  {
   if ((bposx0<0)||(bposx0>=WSIZEX)) ret=1;
   if (bposy0>=WSIZEY) ret=2;
  }
 }
 return ret;
}

int checkcol(int m)
{
 int r=checkcol0();
 if (r==0) return 0;
 if (r==1) posx+=lastmove;
 else
 {
 if (m==1)
 {
  posx+=lastmove;
  return r;
 }
 if (m==2)
 {
  return r;
 }
 else
 {
  posy--;
  return 1;
 }
 }
 return 0;
}

void setblock()
{
 int bsx,bsy,bposx,bposy;
 char *bimg;
 getblockinfo(block,dir,bsx,bsy,bposx,bposy,&bimg);
 for (int i=0; i<bsy; i++)
 for (int j=0; j<bsx; j++)
 {
  int bposx0=j-bposx+posx;
  int bposy0=i-bposy+posy;
  if ((bposx0>=0)&&(bposy0>=0)&&(bposx0<WSIZEX)&&(bposy0<WSIZEY)&&(*(bimg+bsx*i+j)))
  {
   well[bposy0][bposx0]=*(bimg+bsx*i+j)*(blockcolor+1);
  }
 }
}

int cutline()
{
 memset(pointtab,0,sizeof(pointtab));
 int sum=0;
 int lines=1;
 for (int i=WSIZEY-1; i>=0; i--)
 {
  int ok=1;
  int partsum=0;
  for (int j=0; j<WSIZEX; j++)
  {
   if (well[i][j]==0) {ok=0; partsum=0; break;}
   partsum+=(4-well[i][j]+1);
  }
  if (ok)
  {
   sum+=partsum*lines;
   pointtab[i][0]=partsum;
   lines<<=1;
  }
 }
 for (int i=0; i<WSIZEY; i++)
 {
  if (pointtab[i][0])
  {
   for (int k=i-1; k>=0; k--)
   {
    memcpy(well[k+1],well[k],WSIZEX);
   }
   pointtab[i][1]=(int)(GetTime()*1000);
  }
 }
 return sum;
}

void updategame()
{
 if ((int)(1000*(GetTime()-lasttime))>currgamespeed)
 {
  posy+=1;
  if (fallmode==1) points+=1;
  if (fallmode==2) points+=2;
  lasttime=GetTime();
 }
 currgamespeed=gamespeed;
 if (fallmode==1)
 {
  currgamespeed=FASTFALLSPEED;
  fallmode=0;
 }
 if (fallmode==2)
 {
  currgamespeed=FASTFALLSPEED/2;
 }
 if (checkcol(0))
 {
  setblock();
  points+=cutline();
  newblock();
 }
 for (int i=0; i<WSIZEX; i++)
 {
  if (well[0][i]) {gamemode=4; break; }
 }
}

//if returns 0 program exits else goes to MainLoop()
int AppInit()
{
 srand((int)(GetTime()*1000));
 SetVidMode(SCRWIDTH,SCRHEIGHT,BPP,0);
 return 1;
}

//if returns 0 program goes to AtExit()
int AppMainLoop()
{
 if (gamemode==0) //reset
  {
   lasttime=GetTime();
   currgamespeed=gamespeed;
   resetgame();
   gamemode=1;
  }
  else
  if (gamemode==1)
  {
  
  ClearBuf(BKCOLOR);
  if (paused==0)
  {
   updategame();
   drawblock(posx,posy,block,dir,blockcolors[blockcolor]);
  }
  else
  {
   DrawText3X5((WPOSX+WSIZEX*BLOCKSIZE/2)-strlen("PAUSE")*4,(WPOSY+WSIZEY*BLOCKSIZE)/2-4,
   INFOCOLOR,"PAUSED");
  }
  
  drawwell();

  drawblock(-5,4,nextblock,0,blockcolors[nextblockcolor],1);
  
  
  char bf[64];
  /*sprintf(bf,"%d %d",posx,posy);
  DrawText3X5(0,0,INFOCOLOR,bf);*/
  if (paused==0)
  totaltime+=GetTime()-prevtime;
  prevtime=GetTime();
  float t=totaltime;
  sprintf(bf,"%02d:%02d",(int)(t/60),(int)(t)%60);
  DrawText3X5(2,2,INFOCOLOR,bf);
  if (acctimer)
  {
   int a=strlen(bf);
   sprintf(bf,"%02d",(int)(t*100)%100);
   DrawText3X5(2+a*8,2+5,INFOCOLOR,bf,1);
  }
  
  sprintf(bf,"SCORE:%d",points);
  DrawText3X5(2,SCRHEIGHT-36,INFOCOLOR,bf);
  DrawText3X5(2+1,SCRHEIGHT-36+1,INFOCOLORF,bf);
  
  DrawText3X5(2,SCRHEIGHT-12,INFOCOLOR,"RESET");
  DrawText3X5(SCRWIDTH-strlen("EXIT")*8-2,SCRHEIGHT-12,INFOCOLOR,"EXIT");
  
  if (paused==0)
  {
   if (GetKey(KEYLEFT)||GetKey(ALTKEYLEFT)) {posx--; lastmove=1; checkcol(1);}
   if (GetKey(KEYRIGHT)||GetKey(ALTKEYRIGHT)) {posx++; lastmove=-1; checkcol(1);}
   if (GetKey(KEYUP)||GetKey(ALTKEYUP)) {dir++; if (checkcol(2)==2) dir--;}
   if (KeyPressed(KEYDOWN)||KeyPressed(ALTKEYDOWN)) if (fallmode==0) fallmode=1;
  }
  if (GetKey(KEYEXIT)) gamemode=3;
  if (GetKey(KEYRESET)) gamemode=0;
  if (GetKey(KEYENTER)||GetKey(ALTKEYENTER)) if (fallmode==0) fallmode=2;
  if (GetKey(KEYPAUSE)) paused=1-paused;
  if (GetKey(KEYACCTIMER)) acctimer=1-acctimer;
  } //gamemode==1
  else
  if (gamemode==3) return 0;
  else
  //koniec gry
  if (gamemode==4)
  {
   float t=totaltime;
   ClearBuf(BKCOLOR);
   char bf[64];
   sprintf(bf,"POINTS: %d",points);
   DrawText3X5(SCRWIDTH/2-strlen(bf)*8/2,SCRHEIGHT/2-22,INFOCOLOR,bf);
   sprintf(bf,"Time: %d min. %d sec.",(int)(t)/60,(int)(t)%60);
   DrawText3X5(SCRWIDTH/2-strlen(bf)*8/2,SCRHEIGHT/2-10,INFOCOLOR,bf);
   sprintf(bf,"RATIO: %0.3f",(float)points/(t));
   DrawText3X5(SCRWIDTH/2-strlen(bf)*8/2,SCRHEIGHT/2+2,INFOCOLOR,bf);
   DrawText3X5(SCRWIDTH/2-strlen("PRESS ANY KEY...")*8/2,SCRHEIGHT/2+14,INFOCOLOR,"PRESS ANY KEY...");
   DrawText3X5(SCRWIDTH/2-strlen("http://kk.plenty.vbiz.pl")*4/2,SCRHEIGHT-6,INFOCOLOR,"http://kk.plenty.vbiz.pl",1);
   if (GetTime()-lasttime>0.2)
   {
    for (int i=0; i<256; i++) if (GetKey(i)) gamemode=0;
   }
  }
 return 1;
}

void AppExit()
{
}
