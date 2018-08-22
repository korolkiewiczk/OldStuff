asm volatile ("\
0:\
 mov %%eax,(%%edi);\
 add %%edx,%%edi;\
 dec %%ecx;\
 jnz 0b;\
 "::"a"(col),"c"(i),"d"(width*sizeof(int)),"D"(bf));
