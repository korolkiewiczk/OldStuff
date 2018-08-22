//spaceship.h
#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "rect.h"
#include "pic.h"
#include "tex.h"

#include <list>
using namespace std;

#define VELATTEN 0.92

//informacje o strzale
class CShot
{
private:
 float x,y;
 float vel;
 CPic *pic;
public:
 CShot(float _x,float _y,float _vel,CPic *_pic) 
 {
  x=_x;
  y=_y;
  vel=_vel;
  pic=_pic;
 }
 void Update(float);
 float GetX() {return x;}
 float GetY() {return y;}
};

class CSpaceShip
{
private:
 float x,y;
 float vx,vy;
 float acc;
 int power;
 int boundx1,boundy1,boundx2,boundy2;
 CPic pic;
 CPic shotpic;
 float scale;
 
 float shotvel;
 float shottime;
 float shottimer;
 
 //lista strzalow
 list <CShot*> shot;
public:
 float timer;
 int dir;

 enum
 {
  Up=1,
  Down=2,
  Left=4,
  Right=8,
  Shot=16
 } Commands;
       
 void Init(int,int,int,int,int,int,int,float,float,float,float); //inicjuje klase
 void SetTex(int,int,int,int,CTex *);
 void SetShotTex(int,int,int,int,CTex *);
 void Draw();
 void Command(int);
 void Update();
 void CheckColWith(CSpaceShip&);
 int IsAlive() {return power>0;}
 TRect ShipRect();
 void DecPower() {if (power>0) power--;}
 int GetPower() {return power;}
 void GetPos(float &px,float &py) {px=x; py=y;}
 void SetPower(int a) {power=a;}
};

#endif
