/*
  X86 EMULATOR by Kamil Korolkiewicz 2007
  Plik:misc.c
  Opis:Funkcje pomocnicze roznego typu
*/

//======================================

//funkcje czasu

qword cpu_freq;                         //czestotliwosc procesora

/*
  InitTimer
  Inicjuje system czasu: Podaje czestotliwosc procesora w zmiennej cpu_freq.
*/
void InitTimer()
{
 LARGE_INTEGER frq,pc1,pc2;
 qword t1,t2;
 int i;
 QueryPerformanceFrequency(&frq);
 _rdtsc(&t1);
 QueryPerformanceCounter(&pc1);
 for (i=0; i<65535*1024; i++) ;        //nic nie rob
 QueryPerformanceCounter(&pc2);
 _rdtsc(&t2);
 cpu_freq=(t2-t1)*frq.QuadPart/(pc2.QuadPart-pc1.QuadPart);
}

/*
  GetTime
  Podaje czas w sekundach (wzgledny, zalezny od czsu dzialania komputera)
*/

double GetTime()
{
 return (double)_rdtsc(0)/cpu_freq;
}

//======================================

//obsluga pamieci

//alokuje n bajtow pamieci systemowej i czysci ja
ptr inline AllocMem(dword n)
{
 ptr register a= GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,n);
 return a;
}

//zwalnia pamiec
void inline FreeMem(ptr wsk)
{
 GlobalFree(wsk);
}

//obsluga pamieci heap

ptr _heap_addr;
dword _heap_size;
dword _heap_top;

dword CreateHeap(dword n)
{
 _heap_addr=GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,n+4);
 _heap_size=n;
 _heap_top=4;
 return _heap_addr!=0;
}

void FreeHeap()
{
 GlobalFree(_heap_addr);
}

//0 - blad
ptr AllocHeap(dword n)
{
 if (_heap_top+n>_heap_size) return 0;
 dword register a=_heap_top;
 _heap_top+=n;
 return (ptr)(a+_heap_addr);
}

//======================================

//obsluga przedzialow

//1-ok
dword AddRange(range_t **r,dword a,dword b)
{
 if (a>b) {dword pom=b; b=a; a=pom;}
 range_t *r2;
 if (*r==0) 
 {
  *r=AllocHeap(sizeof(range_t));
  if (*r==0) return 0;
  r2=*r;
 }
 else
 {
  (*r)->next=AllocHeap(sizeof(range_t));
  r2=(*r)->next;
  if (*r==0) return 0;
 }
 r2->a=a;
 r2->b=b;
 r2->next=0;
 return 1;
}
