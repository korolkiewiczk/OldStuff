asm volatile ("\
0:\
 mov %%eax,(%%edi);\
 add %7,%%esi;\
 cmp %6,%%esi;\
 jae 1f;\
 add %%edx,%%edi;\
 dec %%ecx;\
 jnz 0b;\
 jmp 2f;\
1:\
 sub %6,%%esi;\
 add %%ebx,%%edi;\
 dec %%ecx;\
 jnz 0b;\
2:\
 "::"a"(col),"b"(dwsk1*sizeof(int)),"c"(a+1),"d"(dwsk2*sizeof(int)),"S"(s),"D"(wsk),
    "m"(a),"m"(b));
