//rdtsc.asm
extern qword _rdtsc(qword *);

//bt.asm
extern void _memsetb(ptr,dword,dword);
extern void _memsetw(ptr,dword,dword);
extern void _memsetd(ptr,dword,dword);
extern void _memcpyb(ptr,ptr,dword);
extern void _memcpyw(ptr,ptr,dword);
extern void _memcpyd(ptr,ptr,dword);
