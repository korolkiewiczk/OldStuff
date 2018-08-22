/*
  DEBUG.C by Kamil Korolkiewcz 2007
*/

#include <stdarg.h>
#include <stdio.h>

FILE *(_DEBUG_file)=(FILE*)-1;

void DEBUG_Printf(char *s,...)
{
 if ((int)_DEBUG_file==-1) _DEBUG_file=fopen("debug.log","wt");
 else _DEBUG_file=fopen("debug.log","at");
 va_list args;
 char buf[2048];
 va_start(args,s);
 vsprintf(buf,s,args);
 va_end(args);
 fprintf(_DEBUG_file,buf);
 fclose(_DEBUG_file);
}
