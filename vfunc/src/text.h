#ifndef _TEXT_H_
#define _TEZT_H_

class CText
{
private:

public:
	CText();
	static void Init();
	void Draw(float x,float y,int color,char *s,int is3d=0,float z=0);
};

#endif
