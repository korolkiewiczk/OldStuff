asm volatile ("\
0:\
  cmp (%%esi),%%ebx;\
  jae 1f;\
  mov %5,%%eax;\
  sar %7,%%eax;\
  mov (%%edx,%%eax,4),%%eax;\
  mov %%eax,(%%edi);\
  mov %%ebx,(%%esi);\
1:\
  add $4,%%esi;\
  add $4,%%edi;\
  mov %6,%%eax;\
  add %%eax,%5;\
  dec %%ecx;\
  jnz 0b; "::"b"(z),"c"(i),"d"(pal),"S"(zwsk),"D"(wsk),"m"(c),"m"(dc),"n"(FIXED_SHIFT));
