#include "ff.h"

CFF::CFF()
{
	fl=0;
}

CFF::CFF(char *fn)
{
	fl=0;
	Load(fn);
}

int CFF::Load(char *fn)
{
	FILE *f;
	f=fopen(fn,"rt");
	if (!f) return 0;
	filename=fn;
	char mainbuf[4096];
	int ln=0;
	int lcnt=0;

	while (!feof(f))
	{
		char buf[1024];
		fgets(buf,1024,f);
		if (lcnt%5==0)
		{
			if (ln>0) //signature
			{
				CFuncList *flpom;
				flpom=new CFuncList;
				flpom->next=fl;

				fl=flpom;
				fl->f=new CFunc(mainbuf);
			}
			mainbuf[0]=0;
			ln++;
		}
		strcat(mainbuf,buf);
		lcnt++;
	}

	if (lcnt%5==0)
	{
		if (ln>0) //signature
		{
			CFuncList *flpom;
			flpom=new CFuncList;
			flpom->next=fl;

			fl=flpom;
			fl->f=new CFunc(mainbuf);
		}
		mainbuf[0]=0;
		ln++;
	}
	fclose(f);
	return 1;
}

CFF::~CFF()
{
	CFuncList *l=fl,*pom;
	while (l)
	{
		delete l->f;
		pom=l;
		l=l->next;
		delete pom;
	}
	fl=0;
}

void CFF::Draw()
{
	CFuncList *l=fl;

	while (l)
	{
		l->f->Draw();
		l=l->next;
	}
}

void CFF::Update()
{
	this->~CFF();
	Load(filename);
}

void CFF::NextPolyMode()
{
	CFuncList *l=fl;
	while (l)
	{
		l->f->NextPolyMode();
		l=l->next;
	}
}
void CFF::NextShadeModel()
{
	CFuncList *l=fl;
	while (l)
	{
		l->f->NextShadeModel();
		l=l->next;
	}
}
