#ifndef __TIME_H
#define __TIME_H

//time.h dla time.inc

#define TIME_FLAGS_ABSOLUTE_TIME        0
#define TIME_FLAGS_MILLISEC_TIME        1
#define TIME_FLAGS_SEC_TIME             2
#define TIME_FLAGS_MIN_TIME             3
#define TIME_FLAGS_HOUR_TIME            4
#define TIME_FLAGS_CLOCK_ONLY           8

typedef struct _TTime
{
 unsigned long long currenttime;
 int flags;
} TTime;

typedef double time_f;

extern unsigned long long __cpu__tsc__freq;

extern int _InitTime();
extern time_f (*Time_Get)(TTime *);
extern time_f __gettime__clock__(TTime *);
extern time_f __gettime__rdtsc__(TTime *);

int InitTime()
{
 int w=_InitTime();
 if (w==0) Time_Get=&__gettime__clock__;
 else Time_Get=&__gettime__rdtsc__;
 return w;
}

void Time_InitStruct(TTime *t,int f)
{
 t->flags=f;
 Time_Get(t);
}

#endif
