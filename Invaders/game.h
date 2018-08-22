//game.h
#ifndef GAME_H
#define GAME_H

#define MAXINVAD 12
#define INVADDEFY 1700
#define INVADTIME 0.1
#define INVADACC 720.f
#define INVADSHOTVEL 1400.f
#define INVADSHOTTIME 0.7

#define BOUNDDIV 20

#define BONUSMINY -5000.f
#define BONUSVEL 700.f

#define MAXSTARS 400
#define STARVEL 520.f

#define GENPARTS 150

#define PLAYERPOWER 16
#define PLAYERACC 850.f
#define PLAYERSHOTVEL 1000.f
#define PLAYERSHOTTIME 0.5f

#include <stdlib.h>
#include <list>

#include "spaceship.h"
#include "part.h"
#include "tex.h"

class CGame
{
private:
 static CSpaceShip playership;
 static CSpaceShip invadship[MAXINVAD];
 static float frametime;
 static float prevt;
 static CPart stars[MAXSTARS];
 static int width,height;
 static float scale;
 static int mode;
 static int points,maxpoints;
 
 static float bonusx,bonusy;
 static CPic bonuspic;
 
 static list <CPart*> boomparts;
 
 static void genstar(int,int=0);
 static void updatestars(int,int);
public:
 static CTex GlobalTex; //globalna tekstura dla wszyskich obiektow
 static void Init(int,int);
 static int MainLoop();
 static void Close();
 static float GetTime();
 static float GetFrameTime() {return frametime*scale;}
 static float frand() {return (float)(rand()%RAND_MAX)/RAND_MAX;}
 static int GetScrHeight() {return height;}
 static void InitPlayer();
 static void InitInvad(int);
 static void NewBoom(float,float,float,float,float,int);
 static void UpdateBoomParts();
 static void DrawPlayerBar();
 static void CheckBonus();
 static void OutI(float,float,float,int);
};

#endif
