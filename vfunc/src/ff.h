#ifndef _FF_H_
#define _FF_H_
#include "func.h"

struct CFuncList
{
	CFunc *f;
	CFuncList *next;
};

class CFF
{
private:
	CFuncList *fl;
	char *filename;
public:
	CFF();
	~CFF();
	CFF(char *fn);
	int Load(char *fn);
	void Draw();
	void Update();
	void NextPolyMode();
	void NextShadeModel();
};

#endif
