typedef struct _ENV
{
 float (*CSys_GetTime)();
 FILE *(*CSys_FOpen)(char *fn,char *m);
 int *MultiExec;
 int *GraphState;
 int (*expr_conprint)(char *s);
} ENV;

enum
{
 FillMode=1,
 EnableTexture=2,
 EnableLight=4,
 Enable3DMode=8,
 EnableOpenGL=16
} GraphStateEnum;
