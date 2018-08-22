#include "engine.h"
#include <cmath>
#include <cstdio>
#include <gl/gl.h>
#include <gl/glu.h>

CEngine::CEngine()
{
	Defaults();
}

void CEngine::Defaults()
{
	posx=posy=posz=0;
	ax=ay=az=0;
	isinfo=0;
	lightvec[0]=0;
	lightvec[1]=-1;
	lightvec[2]=0;
	lightvec[3]=0;
}

void CEngine::ChangePos(double m)
{
	posx-=m*(sin(ax)*cos(az));
	posz-=m*(cos(ax)*cos(az));
	posy-=m*(sin(az));
}

void CEngine::Translate(double dx,double dy,double dz)
{
	posx+=dx;
	posy+=dy;
	posz+=dz;
}

void CEngine::Rotate(double dax,double day,double daz)
{
	ax-=dax;
	ay+=day;
	az+=daz;
}

void CEngine::UpdateCamera()
{
	glLoadIdentity();
	double lposx,lposy,lposz;
	lposx=posx-sin(ax)*cos(az);
	lposz=posz-cos(ax)*cos(az);
	lposy=posy-sin(az);
	gluLookAt(posx,posy,posz,lposx,lposy,lposz,0,sin(az+3.1415/2),0);
	glRotated(90,0,1,0);
}

void CEngine::DrawInfo()
{
	if (!isinfo) return;
	char buf[128];
	glColor3d(1,1,1);
	sprintf(buf,"x=%f",posx);
	text.Draw(-1,0.95,0xffffff,buf);
	sprintf(buf,"y=%f",-posz);
	text.Draw(-1,0.9,0xffffff,buf);
	sprintf(buf,"z=%f",posy);
	text.Draw(-1,0.85,0xffffff,buf);

	//draw axis

	const double d=25;
	//OX
	double vox[]={0,0,0,0,0,d};
	glColor3d(1,0,0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3,GL_DOUBLE,0,&vox);

	glDrawArrays(GL_LINES,0,2);
	text.Draw(0,0,0xff0000,"X",1,d);

	//OY
	double voy[]={0,0,0,d,0,0};
	glColor3d(0,1,0);

	glVertexPointer(3,GL_DOUBLE,0,&voy);

	glDrawArrays(GL_LINES,0,2);
	text.Draw(d,0,0xff0000,"Y",1);

	//OZ
	double voz[]={0,0,0,0,d,0};
	glColor3d(0,0,1);

	glVertexPointer(3,GL_DOUBLE,0,&voz);

	glDrawArrays(GL_LINES,0,2);

	text.Draw(0,d,0xff0000,"Z",1);

	//Lightvec
	double lvec[]={0,0,0,lightvec[0],lightvec[1],lightvec[2]};
	glColor3d(1,1,0);
	glPushMatrix();
	glRotated(180,0,1,0);

	glVertexPointer(3,GL_DOUBLE,0,&lvec);

	glDrawArrays(GL_LINES,0,2);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CEngine::SetLight()
{
	glLightfv(GL_LIGHT0,GL_POSITION,(GLfloat *)&lightvec);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
	float ambient[4]={0.05,0.05,0.05,1};
	float diffuse[4]={1,1,1,1};
	//float specular[4]={0.25,0.25,0.25,1};
	float specular[4]={0,0,0,1};
	//glLightfv(GL_LIGHT0,GL_AMBIENT,(GLfloat *)&ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,(GLfloat *)&diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,(GLfloat *)&specular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,(GLfloat *)&ambient);
	//float matspec[4]={0.2,0.2,0.2,0};
	float matspec[4]={0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(GLfloat *)&matspec);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,(GLfloat *)&gl_light_emission);
	//glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,16);
}

void CEngine::UpdateLightVec()
{
    lightvec[0]=-cos(ax)*cos(az);
    lightvec[2]=sin(ax)*cos(az);
    lightvec[1]=-sin(az);
}

void CEngine::ToogleBlend()
{
	blend=1-blend;
	if (blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	glDisable(GL_BLEND);
}
