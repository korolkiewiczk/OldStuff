//mathx86.h

//szybkie funkcje matematyczne

float inline round(float a)
{
 float register r;
 asm volatile("frndint": "=t"(r):"0"(a));
 return r;
}

#define floor(a) round((a)-0.4999f)
#define ceil(a)  round((a)+0.4999f)

int inline iround(float a)
{
 int register r;
 asm volatile("fistl %2":"=t"(a):"0"(a),"m"(r):"memory");
 return r;
 //return (int)round(a);
}

#define ifloor(a) iround((a)-0.4999f)
#define iceil(a)  iround((a)+0.4999f)

float inline sin(float a)
{
 float register r;
 asm volatile("fsin": "=t"(r):"0"(a));
 return r;
}

float inline cos(float a)
{
 float register r;
 asm volatile("fcos": "=t"(r):"0"(a));
 return r;
}

float inline tan(float a)
{
 float register r;
 asm volatile("fsincos; fxch; fdivp %%st(0),%%st(1)": "=t"(r):"0"(a));
 return r;
}

float inline sqrt(float a)
{
 float register r;
 asm volatile("fsqrt": "=t"(r):"0"(a));
 return r;
}

float inline fabs(float a)
{
 float register r;
 asm volatile("fabs": "=t"(r):"0"(a));
 return r;
}
