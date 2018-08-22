  asm volatile ("\
0:\
  mov %10,%%eax;\
  cmp (%%esi),%%eax;\
  jae 1f;\
  mov %1,%%ebx;\
  mov %12,%%ecx;\
  mov %%eax,(%%esi);\
  sar %13,%%ebx;\
  and %11,%%ebx;\
  sal %%cl,%%ebx;\
  mov %0,%%ecx;\
  sal $2,%%ebx;\
  sar %13,%%ecx;\
  and %11,%%ecx;\
  lea (%%ebx,%%ecx,4),%%eax;\
  mov %5,%%ecx;\
  add %2,%%eax;\
  sar %13,%%ecx;\
  mov (%%eax),%%eax;\
  mov %%eax,%%ebx;\
  shr $16,%%eax;\
  and $0xff,%%eax;\
  mul %%cl;\
  and $0xffff,%%ebx;\
  shl $8,%%eax;\
  and $0xff0000,%%eax;\
  or %%ebx,%%eax;\
  xchg %%al,%%ah;\
  mul %%cl;\
  and $0xff,%%ebx;\
  imul %%ebx,%%ecx;\
  mov %%ch,%%al;\
  mov %%eax,(%%edi);\
1:\
  add $4,%%esi;\
  add $4,%%edi;\
  mov %6,%%eax;\
  add %%eax,%5;\
  mov %7,%%ebx;\
  add %%ebx,%0;\
  mov %8,%%ecx;\
  add %%ecx,%1;\
  decl %9;\
  jnz 0b; "::
  "m"(u)/*0*/,"m"(v)/*1*/,"m"(tex->peekbuf)/*2*/, //0-2
  "S"(zwsk),"D"(wsk),                                       //3-4
  "m"(c)/*5*/,"m"(dc)/*6*/,"m"(du)/*7*/,"m"(dv)/*8*/,"m"(i)/*9*/,
  "m"(z)/*10*/,"m"(tex->peekmask)/*11*/,"m"(tex->peekshift)/*12*/,"n"(FIXED_SHIFT)/*13*/
  :"%eax","%ecx","%ebx");
