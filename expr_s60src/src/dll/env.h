typedef struct _ENV
{
 float (*CSys_GetTime)();
 FILE *(*CSys_FOpen)(char *fn,char *m);
 int *MultiExec;
} ENV;
