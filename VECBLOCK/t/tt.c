#include <stdlib.h>
int f1;
float f2;
char t1[]="kamil";
char t2[]="kamil2";

int __(int _) {return _;}

void main()
{
 __(0x21436587);
 f1=1;
 f2=5.5;
 f2=f1;
 printf ("%f\n",f2);
 f1=1;
 f2=5.5;
 memcpy (&f2,&f1,4);
 printf ("%f\n",f2);
 f1=1;
 f2=5.5;
 f1=f2;
 printf ("%d\n",f1);
 f1=1;
 f2=5.5;
 memcpy (&f1,&f2,4);
 printf ("%d\n",f1);
 memcpy (&t1,&t2,5);
 printf ("%s\n",t1);
}
