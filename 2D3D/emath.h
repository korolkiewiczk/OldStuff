//emath.h - funkcje matematyczne pomocnicze

#define INF 1000000000000.0
#define EPSILON 0.001
#define RAD(a) ((a)/180*3.14159265)
#define MAX(a,b) ((a)>(b)? (a):(b))
#define MIN(a,b) ((a)<(b)? (a):(b))

//liczy iloczyn skalarny wektorow
Float ScalarProduct(Float a1,Float a2,Float b1,Float b2)
{
 return a1*b1+a2*b2;
}

//rzutuje punkt 3D na ekran
//wsp. y stanowi plaszczyzne 'w glab'
//dp to odleglosc od plaszczyzny rzutowania
void ProjectPoint(Float x,Float y,Float z,Float dp,int swidth,int sheight,int *xp,int *yp)
{
 *xp=swidth/2+(int)round(x/y*dp);
 *yp=(int)round(z/y*dp);
 if (*yp==0) *yp=1;
}

//podaje wsp. kierunkowy a prostej y=ax+b na podstawie kata
Float Get_a_From_angle(Float angle)
{
 angle=RAD(angle);
 if (fabs(cos(angle))<EPSILON) return INF;
 else return sin(angle)/cos(angle);
}

//podaje wsp b prostej y=ax+b dla danych a i punktu
Float Get_b(Float a,Float x,Float y)
{
 return y-a*x;
}

//reszta ze specjalym uwzglednieniem liczb ujemnych
int intMod(int a,int m)
{
 if (a>=0) return a%m;
 else return ((abs(a)/m+1)*m+a)%m;
}

//transformuje punkt (x,y) wedlug punktu (x0,y0) o kat angle
void TransformPoint(Float x,Float y,Float x0,Float y0,Float angle,Float *xt,Float *yt)
{
 x=x-x0;
 y=y-y0;
 angle=RAD(angle);
 *xt=x*cos(angle)-y*sin(angle);
 *yt=x*sin(angle)+y*cos(angle);
}

//sprawdza czy dany punkt jest za(-1) czy przed(1) prosta oraz podaje punkt przeciecia
int CheckLinesCrossing(Float x1,Float y1,Float x2,Float y2,Float x,Float y,Float *xp,Float *yp)
{
 Float a1,a2;
 if (x1==x2) 
 a1=INF;
 else
 a1=(y1-y2)/(x1-x2);
 
 if (x==0) 
 a2=INF;
 else
 a2=y/x;
 
 if (a1==a2) {*xp=INF; *yp=INF; return 1;}
 
 *xp=(-a1*x1+y1)/(a2-a1);
 *yp=a2*(*xp);
 if (*xp*(*xp)+*yp*(*yp)<x*x+y*y) return -1; else return 1;
}

//sprawdza czy odcinki sie przecinaja (0-nie)
int CheckSegmentsCrossing(Float x1,Float y1,Float x2,Float y2,Float x3,Float y3,Float x4,Float y4)
{
 Float a1,a2;
 Float xp,yp;
 int w;
 if (fabs(x1-x2)>=fabs(y1-y2)) w=1; else w=2;
 if (fabs(x3-x4)<fabs(y3-y4)) w*=4;
 if (w==1)
 {
  a1=(y1-y2)/(x1-x2);
  a2=(y3-y4)/(x3-x4);
  
  if (a1==a2) return 0;
 
  xp=(y3-a2*x3-(y1-a1*x1))/(a1-a2);
  yp=a1*xp+y1-a1*x1;
 }
 else
 if (w==2)
 {
  a1=(x1-x2)/(y1-y2);
  a2=(y3-y4)/(x3-x4);
 
  xp=(a1*(-a2*x3+y3)+x1-a1*y1)/(1-a1*a2);
  yp=a2*xp+y3-a2*x3;
 }
 else
 if (w==4)
 {
  a1=(y1-y2)/(x1-x2);
  a2=(x3-x4)/(y3-y4);
 
  xp=(a2*(-a1*x1+y1)+x3-a2*y3)/(1-a1*a2);
  yp=a1*xp+y1-a1*x1;
 }
 else
 if (w==8)
 {
  a1=(x1-x2)/(y1-y2);
  a2=(x3-x4)/(y3-y4);
  
  if (a1==a2) return 0;
 
  yp=(-a2*y3+x3-(-a1*y1+x1))/(a1-a2);
  xp=a1*yp-a1*y1+x1;
 }
 
 int ok=0;
 if ((xp>=x1)&&(xp<=x2)) ok=1;
 else
 if ((xp<=x1)&&(xp>=x2)) ok=1;
 
 if (ok==1)
 {
  if ((yp>=y1)&&(yp<=y2)) ok=2;
  else
  if ((yp<=y1)&&(yp>=y2)) ok=2;
 }
 
 if (ok==2)
 {
  if ((xp>=x3)&&(xp<=x4)) ok=3;
  else
  if ((xp<=x3)&&(xp>=x4)) ok=3;
 
  if (ok==3)
  {
   if ((yp>=y3)&&(yp<=y4)) ok=4;
   else
   if ((yp<=y3)&&(yp>=y4)) ok=4;
  }
 }
 
 if (ok==4) return 1;
 return 0;
}

//przycina linie zgodnie z prosta y=c (zwraca wsp. x)
Float CutLine(Float x1,Float y1,Float x2,Float y2,Float c)
{
 if (x1==x2) return x1;
 Float a=(y1-y2)/(x1-x2);
 return (c+a*x1-y1)/a;
}
