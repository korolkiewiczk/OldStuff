#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "text.h"

class CEngine
{
private:
	double posx,posy,posz;
	double ax,ay,az;
	CText text;
	int isinfo;
	float lightvec[4];
	int blend;
public:
	void Defaults();
	CEngine();

	void ChangePos(double m);
	void Translate(double dx,double dy,double dz);
	void Rotate(double dax,double day,double daz);
	void UpdateCamera();
	void DrawInfo();
	void ShowInfo() {isinfo=1-isinfo;}
	void SetLight();
	void UpdateLightVec();
	void ToogleBlend();
};

#endif
