#include <cstring>
#include <string>
#include <windows.h>
#include <gl/gl.h>
#include "eval.cpp"
#include "func.h"
#include "common.h"

double CFunc::readval(char *str)
{
	CEval<double> e;
	e.Set(str);
	return e();
}

char *CFunc::getl(char *s,int &l)
{
	static char buf[1024]; int i=0;
	while ((s[l]!=0)&&(s[l]<32)) l++;
	while ((s[l]!=0)&&(s[l]>=32)) buf[i++]=s[l++];
	buf[i]=0;
	return buf;
}

void CFunc::parse(char *str)
{
	loaded=0;
	char *buf;

	//fgets(buf,256,f);
	int i=0;
	buf=getl(str,i);
	color=0;
	sscanf(buf,"%c %x",&type,&color);
	if (color==0) color=0xffffff;
	if (type>'Z') type-='a'-'A';

	//fgets(buf,256,f);
	buf=getl(str,i);
	if (type=='C')
	{
		char strt1[256],strt2[256],strnp[256];
		//sscanf(buf,"%f %f %d",&t1,&t2,&npoints);
		sscanf(buf,"%s %s %s",strt1,strt2,strnp);
		t1=readval(strt1);
		t2=readval(strt2);
		npoints=(int)readval(strnp);
	}
	else
	if (type=='S')
	{
		//sscanf(buf,"%f %f %f %f %d",&t1,&t2,&s1,&s2,&npoints);
		char strt1[256],strt2[256],strs1[256],strs2[256],strnp[256];
		sscanf(buf,"%s %s %s %s %s",strt1,strt2,strs1,strs2,strnp);
		t1=readval(strt1);
		t2=readval(strt2);
		s1=readval(strs1);
		s2=readval(strs2);
		npoints=(int)readval(strnp);
	}

	if (x_e) delete x_e;
	if (y_e) delete y_e;
	if (z_e) delete z_e;

	x_e=new CEval<double>;
	y_e=new CEval<double>;
	z_e=new CEval<double>;

	x_e->AddVar("t",0);
    x_t=x_e->GetVarPtr("t");
    x_e->AddVar("s",0);
    x_s=x_e->GetVarPtr("s");
    x_e->AddVar("time",0);
    x_time=x_e->GetVarPtr("time");

    y_e->AddVar("t",0);
    y_t=y_e->GetVarPtr("t");
    y_e->AddVar("s",0);
    y_s=y_e->GetVarPtr("s");
    y_e->AddVar("time",0);
    y_time=y_e->GetVarPtr("time");

    z_e->AddVar("t",0);
    z_t=z_e->GetVarPtr("t");
    z_e->AddVar("s",0);
    z_s=z_e->GetVarPtr("s");
    z_e->AddVar("time",0);
    z_time=z_e->GetVarPtr("time");


	buf=getl(str,i);
	z_e->Set(buf);

	buf=getl(str,i);
	x_e->Set(buf);

	buf=getl(str,i);
	y_e->Set(buf);


	if (type=='S')
	{
		//generate index tab
		if (indextab) delete[]indextab;
		indextab=new int[npoints*npoints*6];
		int *itab;
		itab=indextab;
		for (int j=0; j<npoints; j++)
		{
			for (int i=0; i<npoints; i++)
			{
				*(itab++)=i+j*(npoints+1);
				*(itab++)=i+1+j*(npoints+1);
				*(itab++)=i+(j+1)*(npoints+1);
				*(itab++)=i+(j+1)*(npoints+1);
				*(itab++)=i+1+j*(npoints+1);
				*(itab++)=i+1+(j+1)*(npoints+1);
			}
		}

		if (lindextab) delete[]lindextab;
		lindextab=new int[npoints*npoints*8];
		itab=lindextab;
		for (int j=0; j<npoints; j++)
		{
			for (int i=0; i<npoints; i++)
			{
				*(itab++)=i+j*(npoints+1);
				*(itab++)=i+1+j*(npoints+1);
				*(itab++)=i+1+j*(npoints+1);
				*(itab++)=i+1+(j+1)*(npoints+1);
				*(itab++)=i+1+(j+1)*(npoints+1);
				*(itab++)=i+(j+1)*(npoints+1);
				*(itab++)=i+(j+1)*(npoints+1);
				*(itab++)=i+j*(npoints+1);
			}
		}
	}

	if (vtab) delete []vtab; vtab=0;
	if (ntab) delete []ntab; ntab=0;

	//check 'time' used
	string stri;
	istime=0;
	int l;
	stri=x_e->expression;
	l=stri.find("time");
	if (l!=stri.npos) {istime=1; goto quit;}
	stri=y_e->expression;
	l=stri.find("time");
	if (l!=stri.npos) {istime=1; goto quit;}
	stri=z_e->expression;
	l=stri.find("time");
	if (l!=stri.npos) {istime=1; goto quit;}

quit:

	funcstr=str;

	loaded=1;
}

CFunc::CFunc(char *str)
{
	t1=-1;
	t2=1;
	npoints=10;
	x_e=0;
	y_e=0;
	z_e=0;
	polymode=Lines;
	vtab=0;
	ntab=0;
	indextab=0;
	lindextab=0;
	loaded=0;
	parse(str);
	shademodel=GL_SMOOTH;
}

CFunc::~CFunc()
{
	if (x_e) delete x_e;
	if (y_e) delete y_e;
	if (z_e) delete z_e;
}

void CFunc::cross(	double x1,double y1,double z1,
					double x2,double y2,double z2,
					double x3,double y3,double z3,
					double &x,double &y,double &z)
		{
			double ax=x2-x1;
			double ay=y2-y1;
			double az=z2-z1;
			double bx=x3-x1;
			double by=y3-y1;
			double bz=z3-z1;
			x=ay*bz-az*by;
			y=ax*bz-az*bx;
			z=ax*by-ay*bx;
		}

void CFunc::Draw()
{
	if (!loaded) return;
	unsigned char alpha=color>>24;
	if (alpha==0) alpha=255;
	glColor4ub((color>>16)&0xff,(color>>8)&0xff,color&0xff,alpha);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if ((polymode==Light)&&(type=='S'))
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);
		glEnable(GL_COLOR_MATERIAL);
		glShadeModel(shademodel);
	}

	double time=GetTime();

 	if (istime)
 	{
		*x_time=time;
		*y_time=time;
		*z_time=time;
 	}
	if (type=='C')
	{
		//generate vertex tab
		if ((istime!=0)||(vtab==0))
		{
			if (vtab) delete []vtab;
			vtab=new SVertex[npoints+1];

			double dt=(t2-t1)/npoints;
			double t=t1;

			*x_t=t;
			*y_t=t;
			*z_t=t;

			for (int i=0; i<=npoints; i++)
			{
				*x_t=t;
				*y_t=t;
				*z_t=t;

				vtab[i].x=(*x_e)();
				vtab[i].y=(*y_e)();
				vtab[i].z=(*z_e)();

				t+=dt;
			}
		}

		//render
		glEnableClientState(GL_VERTEX_ARRAY);

  		glVertexPointer(3,GL_FLOAT,0,(void*)vtab);

  		glDrawArrays(GL_LINE_STRIP,0,npoints+1);

  		glDisableClientState(GL_VERTEX_ARRAY);
	}
	else
	if (type=='S')
	{
		//generate vertex tab
		if ((istime!=0)||(vtab==0))
		{
			if (vtab) delete []vtab;
			vtab=new SVertex[npoints*npoints+3*npoints];
			SVertex *v;
			v=vtab;

			double dt=(t2-t1)/npoints;
			double ds=(s2-s1)/npoints;

			double s=s1;
			for (int j=0; j<=npoints; j++)
			{
				double t=t1;
				for (int i=0; i<=npoints; i++)
				{
					*x_t=t;
					*y_t=t;
					*z_t=t;
					*x_s=s;
					*y_s=s;
					*z_s=s;
					v->x=(*x_e)();
					v->y=(*y_e)();
					v->z=(*z_e)();
					v++;

					t+=dt;
				}
				s+=ds;
			}
		}

		if (polymode==Lines)
		{
			//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			//render
			glEnableClientState(GL_VERTEX_ARRAY);

			glVertexPointer(3,GL_FLOAT,0,(void*)vtab);
			glDrawElements(GL_LINES,npoints*npoints*8,GL_UNSIGNED_INT,(void*)lindextab);

			glDisableClientState(GL_VERTEX_ARRAY);
		}
		else
		if (polymode==Fill)
		{
			//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			//render
			glEnableClientState(GL_VERTEX_ARRAY);

			glVertexPointer(3,GL_FLOAT,0,(void*)vtab);
			glDrawElements(GL_TRIANGLES,npoints*npoints*6,GL_UNSIGNED_INT,(void*)indextab);

			glDisableClientState(GL_VERTEX_ARRAY);
		}
		else
		if (polymode==Light)
		{
			//generate normals
			if ((istime!=0)||(ntab==0))
			{
				if (ntab) delete []ntab;
				ntab=new SVertex[npoints*npoints+3*npoints];
				SVertex *n;
				n=ntab;
				int i=0,j=0;

				{
				double x0=vtab[((j)*(npoints+1)+i)].x;
				double y0=vtab[((j)*(npoints+1)+i)].y;
				double z0=vtab[((j)*(npoints+1)+i)].z;

				double x1=vtab[((npoints)*(npoints+1)+i)].x;
				double y1=vtab[((npoints)*(npoints+1)+i)].y;
				double z1=vtab[((npoints)*(npoints+1)+i)].z;

				double x2=vtab[((j)*(npoints+1)+i+1)].x;
				double y2=vtab[((j)*(npoints+1)+i+1)].y;
				double z2=vtab[((j)*(npoints+1)+i+1)].z;

				double x3=vtab[((j+1)*(npoints+1)+i)].x;
				double y3=vtab[((j+1)*(npoints+1)+i)].y;
				double z3=vtab[((j+1)*(npoints+1)+i)].z;

				double x4=vtab[((j)*(npoints+1)+(npoints))].x;
				double y4=vtab[((j)*(npoints+1)+(npoints))].y;
				double z4=vtab[((j)*(npoints+1)+(npoints))].z;

				double nx1,ny1,nz1;
				double nx2,ny2,nz2;
				double nx3,ny3,nz3;
				double nx4,ny4,nz4;
				cross(x0,y0,z0,x1,y1,z1,x2,y2,z2,nx1,ny1,nz1);
				cross(x0,y0,z0,x2,y2,z2,x3,y3,z3,nx2,ny2,nz2);
				cross(x0,y0,z0,x3,y3,z3,x4,y4,z4,nx3,ny3,nz3);
				cross(x0,y0,z0,x4,y4,z4,x1,y1,z1,nx4,ny4,nz4);

				ntab[((j)*(npoints+1)+i)].x=(nx1+nx2+nx3+nx4)*0.25;
				ntab[((j)*(npoints+1)+i)].y=(ny1+ny2+ny3+ny4)*0.25;
				ntab[((j)*(npoints+1)+i)].z=(nz1+nz2+nz3+nz4)*0.25;
				}

				for (i=1; i<npoints; i++)
				{
					double x0=vtab[((j)*(npoints+1)+i)].x;
					double y0=vtab[((j)*(npoints+1)+i)].y;
					double z0=vtab[((j)*(npoints+1)+i)].z;

					double x1=vtab[((npoints)*(npoints+1)+i)].x;
					double y1=vtab[((npoints)*(npoints+1)+i)].y;
					double z1=vtab[((npoints)*(npoints+1)+i)].z;

					double x2=vtab[((j)*(npoints+1)+i+1)].x;
					double y2=vtab[((j)*(npoints+1)+i+1)].y;
					double z2=vtab[((j)*(npoints+1)+i+1)].z;

					double x3=vtab[((j+1)*(npoints+1)+i)].x;
					double y3=vtab[((j+1)*(npoints+1)+i)].y;
					double z3=vtab[((j+1)*(npoints+1)+i)].z;

					double x4=vtab[((j)*(npoints+1)+i-1)].x;
					double y4=vtab[((j)*(npoints+1)+i-1)].y;
					double z4=vtab[((j)*(npoints+1)+i-1)].z;

					double nx1,ny1,nz1;
					double nx2,ny2,nz2;
					double nx3,ny3,nz3;
					double nx4,ny4,nz4;
					cross(x0,y0,z0,x1,y1,z1,x2,y2,z2,nx1,ny1,nz1);
					cross(x0,y0,z0,x2,y2,z2,x3,y3,z3,nx2,ny2,nz2);
					cross(x0,y0,z0,x3,y3,z3,x4,y4,z4,nx3,ny3,nz3);
					cross(x0,y0,z0,x4,y4,z4,x1,y1,z1,nx4,ny4,nz4);

					ntab[((j)*(npoints+1)+i)].x=(nx1+nx2+nx3+nx4)*0.25;
					ntab[((j)*(npoints+1)+i)].y=(ny1+ny2+ny3+ny4)*0.25;
					ntab[((j)*(npoints+1)+i)].z=(nz1+nz2+nz3+nz4)*0.25;
					//n++;

				}

				for (j=1; j<npoints; j++)
				{
					i=0;
					{
					double x0=vtab[((j)*(npoints+1)+i)].x;
					double y0=vtab[((j)*(npoints+1)+i)].y;
					double z0=vtab[((j)*(npoints+1)+i)].z;

					double x1=vtab[((j-1)*(npoints+1)+i)].x;
					double y1=vtab[((j-1)*(npoints+1)+i)].y;
					double z1=vtab[((j-1)*(npoints+1)+i)].z;

					double x2=vtab[((j)*(npoints+1)+i+1)].x;
					double y2=vtab[((j)*(npoints+1)+i+1)].y;
					double z2=vtab[((j)*(npoints+1)+i+1)].z;

					double x3=vtab[((j+1)*(npoints+1)+i)].x;
					double y3=vtab[((j+1)*(npoints+1)+i)].y;
					double z3=vtab[((j+1)*(npoints+1)+i)].z;

					double x4=vtab[((j)*(npoints+1)+(npoints))].x;
					double y4=vtab[((j)*(npoints+1)+(npoints))].y;
					double z4=vtab[((j)*(npoints+1)+(npoints))].z;

					double nx1,ny1,nz1;
					double nx2,ny2,nz2;
					double nx3,ny3,nz3;
					double nx4,ny4,nz4;
					cross(x0,y0,z0,x1,y1,z1,x2,y2,z2,nx1,ny1,nz1);
					cross(x0,y0,z0,x2,y2,z2,x3,y3,z3,nx2,ny2,nz2);
					cross(x0,y0,z0,x3,y3,z3,x4,y4,z4,nx3,ny3,nz3);
					cross(x0,y0,z0,x4,y4,z4,x1,y1,z1,nx4,ny4,nz4);

					ntab[((j)*(npoints+1)+i)].x=(nx1+nx2+nx3+nx4)*0.25;
					ntab[((j)*(npoints+1)+i)].y=(ny1+ny2+ny3+ny4)*0.25;
					ntab[((j)*(npoints+1)+i)].z=(nz1+nz2+nz3+nz4)*0.25;
					}

					for (i=1; i<npoints; i++)
					{
						double x0=vtab[((j)*(npoints+1)+i)].x;
						double y0=vtab[((j)*(npoints+1)+i)].y;
						double z0=vtab[((j)*(npoints+1)+i)].z;

						double x1=vtab[((j-1)*(npoints+1)+i)].x;
						double y1=vtab[((j-1)*(npoints+1)+i)].y;
						double z1=vtab[((j-1)*(npoints+1)+i)].z;

						double x2=vtab[((j)*(npoints+1)+i+1)].x;
						double y2=vtab[((j)*(npoints+1)+i+1)].y;
						double z2=vtab[((j)*(npoints+1)+i+1)].z;

						double x3=vtab[((j+1)*(npoints+1)+i)].x;
						double y3=vtab[((j+1)*(npoints+1)+i)].y;
						double z3=vtab[((j+1)*(npoints+1)+i)].z;

						double x4=vtab[((j)*(npoints+1)+i-1)].x;
						double y4=vtab[((j)*(npoints+1)+i-1)].y;
						double z4=vtab[((j)*(npoints+1)+i-1)].z;

						double nx1,ny1,nz1;
						double nx2,ny2,nz2;
						double nx3,ny3,nz3;
						double nx4,ny4,nz4;
						cross(x0,y0,z0,x1,y1,z1,x2,y2,z2,nx1,ny1,nz1);
						cross(x0,y0,z0,x2,y2,z2,x3,y3,z3,nx2,ny2,nz2);
						cross(x0,y0,z0,x3,y3,z3,x4,y4,z4,nx3,ny3,nz3);
						cross(x0,y0,z0,x4,y4,z4,x1,y1,z1,nx4,ny4,nz4);

						ntab[((j)*(npoints+1)+i)].x=(nx1+nx2+nx3+nx4)*0.25;
						ntab[((j)*(npoints+1)+i)].y=(ny1+ny2+ny3+ny4)*0.25;
						ntab[((j)*(npoints+1)+i)].z=(nz1+nz2+nz3+nz4)*0.25;
						//n++;

					}

					{
					double x0=vtab[((j)*(npoints+1)+i)].x;
					double y0=vtab[((j)*(npoints+1)+i)].y;
					double z0=vtab[((j)*(npoints+1)+i)].z;

					double x1=vtab[((j-1)*(npoints+1)+i)].x;
					double y1=vtab[((j-1)*(npoints+1)+i)].y;
					double z1=vtab[((j-1)*(npoints+1)+i)].z;

					double x2=vtab[((j)*(npoints+1)+0)].x;
					double y2=vtab[((j)*(npoints+1)+0)].y;
					double z2=vtab[((j)*(npoints+1)+0)].z;

					double x3=vtab[((j+1)*(npoints+1)+i)].x;
					double y3=vtab[((j+1)*(npoints+1)+i)].y;
					double z3=vtab[((j+1)*(npoints+1)+i)].z;

					double x4=vtab[((j)*(npoints+1)+i-1)].x;
					double y4=vtab[((j)*(npoints+1)+i-1)].y;
					double z4=vtab[((j)*(npoints+1)+i-1)].z;

					double nx1,ny1,nz1;
					double nx2,ny2,nz2;
					double nx3,ny3,nz3;
					double nx4,ny4,nz4;
					cross(x0,y0,z0,x1,y1,z1,x2,y2,z2,nx1,ny1,nz1);
					cross(x0,y0,z0,x2,y2,z2,x3,y3,z3,nx2,ny2,nz2);
					cross(x0,y0,z0,x3,y3,z3,x4,y4,z4,nx3,ny3,nz3);
					cross(x0,y0,z0,x4,y4,z4,x1,y1,z1,nx4,ny4,nz4);

					ntab[((j)*(npoints+1)+i)].x=(nx1+nx2+nx3+nx4)*0.25;
					ntab[((j)*(npoints+1)+i)].y=(ny1+ny2+ny3+ny4)*0.25;
					ntab[((j)*(npoints+1)+i)].z=(nz1+nz2+nz3+nz4)*0.25;
					}

				}
				for (i=1; i<npoints; i++)
				{
					double x0=vtab[((j)*(npoints+1)+i)].x;
					double y0=vtab[((j)*(npoints+1)+i)].y;
					double z0=vtab[((j)*(npoints+1)+i)].z;

					double x1=vtab[((j-1)*(npoints+1)+i)].x;
					double y1=vtab[((j-1)*(npoints+1)+i)].y;
					double z1=vtab[((j-1)*(npoints+1)+i)].z;

					double x2=vtab[((j)*(npoints+1)+i+1)].x;
					double y2=vtab[((j)*(npoints+1)+i+1)].y;
					double z2=vtab[((j)*(npoints+1)+i+1)].z;

					double x3=vtab[((0)*(npoints+1)+i)].x;
					double y3=vtab[((0)*(npoints+1)+i)].y;
					double z3=vtab[((0)*(npoints+1)+i)].z;

					double x4=vtab[((j)*(npoints+1)+i-1)].x;
					double y4=vtab[((j)*(npoints+1)+i-1)].y;
					double z4=vtab[((j)*(npoints+1)+i-1)].z;

					double nx1,ny1,nz1;
					double nx2,ny2,nz2;
					double nx3,ny3,nz3;
					double nx4,ny4,nz4;
					cross(x0,y0,z0,x1,y1,z1,x2,y2,z2,nx1,ny1,nz1);
					cross(x0,y0,z0,x2,y2,z2,x3,y3,z3,nx2,ny2,nz2);
					cross(x0,y0,z0,x3,y3,z3,x4,y4,z4,nx3,ny3,nz3);
					cross(x0,y0,z0,x4,y4,z4,x1,y1,z1,nx4,ny4,nz4);

					ntab[((j)*(npoints+1)+i)].x=(nx1+nx2+nx3+nx4)*0.25;
					ntab[((j)*(npoints+1)+i)].y=(ny1+ny2+ny3+ny4)*0.25;
					ntab[((j)*(npoints+1)+i)].z=(nz1+nz2+nz3+nz4)*0.25;
					//n++;

				}
				{
				double x0=vtab[((j)*(npoints+1)+i)].x;
				double y0=vtab[((j)*(npoints+1)+i)].y;
				double z0=vtab[((j)*(npoints+1)+i)].z;

				double x1=vtab[((j-1)*(npoints+1)+i)].x;
				double y1=vtab[((j-1)*(npoints+1)+i)].y;
				double z1=vtab[((j-1)*(npoints+1)+i)].z;

				double x2=vtab[((j)*(npoints+1)+0)].x;
				double y2=vtab[((j)*(npoints+1)+0)].y;
				double z2=vtab[((j)*(npoints+1)+0)].z;

				double x3=vtab[(0*(npoints+1)+i)].x;
				double y3=vtab[(0*(npoints+1)+i)].y;
				double z3=vtab[(0*(npoints+1)+i)].z;

				double x4=vtab[((j)*(npoints+1)+i-1)].x;
				double y4=vtab[((j)*(npoints+1)+i-1)].y;
				double z4=vtab[((j)*(npoints+1)+i-1)].z;

				double nx1,ny1,nz1;
				double nx2,ny2,nz2;
				double nx3,ny3,nz3;
				double nx4,ny4,nz4;
				cross(x0,y0,z0,x1,y1,z1,x2,y2,z2,nx1,ny1,nz1);
				cross(x0,y0,z0,x2,y2,z2,x3,y3,z3,nx2,ny2,nz2);
				cross(x0,y0,z0,x3,y3,z3,x4,y4,z4,nx3,ny3,nz3);
				cross(x0,y0,z0,x4,y4,z4,x1,y1,z1,nx4,ny4,nz4);

				ntab[((j)*(npoints+1)+i)].x=(nx1+nx2+nx3+nx4)*0.25;
				ntab[((j)*(npoints+1)+i)].y=(ny1+ny2+ny3+ny4)*0.25;
				ntab[((j)*(npoints+1)+i)].z=(nz1+nz2+nz3+nz4)*0.25;
				}
			}

			//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			//render
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(3,GL_FLOAT,0,(void*)vtab);
			glNormalPointer(GL_FLOAT,0,(void*)ntab);
			glDrawElements(GL_TRIANGLES,npoints*npoints*6,GL_UNSIGNED_INT,(void*)indextab);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
		}
	}
	glPopAttrib();
}
