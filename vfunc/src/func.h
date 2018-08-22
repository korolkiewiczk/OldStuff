#ifndef _FUNC_H_
#define _FUNC_H_

#include <gl/gl.h>
#include "eval.h"

struct SVertex
{
	float x,y,z;
};

class CFunc
{
private:
	char *funcstr;
	/*
	Function description:
	  C/S [hexcolor]
	  t1 t2 [s1 s2] npoints
	  x_expr
	  y_expr
	  z_expr
	*/
	CEval<double> *x_e,*y_e,*z_e;
	double *x_t,*y_t,*z_t;
	double *x_s,*y_s,*z_s;
	double *x_time,*y_time,*z_time;
	unsigned int color;

	SVertex *vtab;
	SVertex *ntab;
	int *indextab;
	int *lindextab;
	int istime;

	int polymode;
	enum
	{
		Lines,
		Fill,
		Light,
		PolyModeMax
	};
	int shademodel;
	char type; //C-curve, S-surface
	double readval(char *str);
	char *getl(char *s,int &l);
	void parse(char *str);
	void inline
		cross(	double x1,double y1,double z1,
				double x2,double y2,double z2,
				double x3,double y3,double z3,
				double &x,double &y,double &z);
	int loaded;
	float t1,t2,s1,s2;
	int npoints;

public:
	CFunc(char *str);
	~CFunc();
	void Update(char *str){parse(str);}
	void Draw();
	void NextPolyMode() {polymode++; if (polymode>=PolyModeMax) polymode=0;}
	void NextShadeModel() {if (shademodel==GL_FLAT) shademodel=GL_SMOOTH; else shademodel=GL_FLAT;}
};

#endif
