/*
  X86 EMULATOR by Kamil Korolkiewicz 2007
  Plik:x86memory.c
  Opis:Obsluga pamieci wirtualnej maszyny
*/

//======================================

//funkcje pomocnicze do pobierania kodu z [eip]
//zrobic obsluge PM

byte inline mem_code_b()
{
 return *(byte*)(vmmemaddr+(cpusregs[CS]<<4)+((eip++)&0xffff));
}

word inline mem_code_w()
{
 word register a=*(word*)(vmmemaddr+(cpusregs[CS]<<4)+(eip&0xffff));
 eip+=2;
 return a;
}

dword inline mem_code_d()
{
 dword register a=*(dword*)(vmmemaddr+(cpusregs[CS]<<4)+(eip&0xffff));
 eip+=4;
 return a;
}

//======================================

//funkcje pobierania i zapisywania pamieci z uzyciem ADRESU FIZYCZNEGO

//zapisywanie

void _mem_write_b(dword ad,dword x)
{
 *(byte*)(vmmemaddr+ad)=x;
}
#define mem_write_b(a,b) _call2p (memcalltabwrite[*(byte*)(vmmemtypetab+((a)>>vmmemtypetabshf))>0][0],a,b)

void _mem_write_w(dword ad,dword x)
{
 *(word*)(vmmemaddr+ad)=x;
}
#define mem_write_w(a,b) _call2p (memcalltabwrite[*(byte*)(vmmemtypetab+((a)>>vmmemtypetabshf))>0][1],a,b)

void _mem_write_d(dword ad,dword x)
{
 *(dword*)(vmmemaddr+ad)=x;
}
#define mem_write_d(a,b) _call2p (memcalltabwrite[*(byte*)(vmmemtypetab+((a)>>vmmemtypetabshf))>0][2],a,b)

void _mem_write_q(dword ad,qword x)
{
 *(qword*)(vmmemaddr+ad)=x;
}
#define mem_write_q(a,b) _call2p (memcalltabwrite[*(byte*)(vmmemtypetab+((a)>>vmmemtypetabshf))>0][3],a,b)

//odczyt

dword _mem_read_b(dword ad)
{
 return *(byte*)(vmmemaddr+ad);
}
#define mem_read_b(a) _call1p (memcalltabread[*(byte*)(vmmemtypetab+((a)>>vmmemtypetabshf))>0][0],a)

dword _mem_read_w(dword ad)
{
 return *(word*)(vmmemaddr+ad);
}
#define mem_read_w(a) _call1p (memcalltabread[*(byte*)(vmmemtypetab+((a)>>vmmemtypetabshf))>0][1],a)

dword _mem_read_d(dword ad)
{
 return *(dword*)(vmmemaddr+ad);
}
#define mem_read_d(a) _call1p (memcalltabread[*(byte*)(vmmemtypetab+((a)>>vmmemtypetabshf))>0][2],a)

qword _mem_read_q(dword ad)
{
 return *(qword*)(vmmemaddr+ad);
}
#define mem_read_q(a) _call1p (memcalltabread[*(byte*)(vmmemtypetab+((a)>>vmmemtypetabshf))>0][3],a)

//======================================

//obsluga blokow pamieci

//zapis n bajtow o wartosci x
void mem_write_n_b(dword ad,dword x,dword n)
{
 //kopiuj blokami
 while (n>0)
 {
  //jesli pamiec urzadzenia to idz do funkcji urzadzenia
  if (*(byte*)(vmmemtypetab+(ad>>vmmemtypetabshf))>0) ;
  else
  {
   int register k=((ad>>vmmemtypetabshf)+1)<<vmmemtypetabshf;
   if (k-ad>n) k=n; else k=k-ad;
   _memsetb(vmmemaddr+ad,x,k);
   ad+=k;
   n-=k;
  }
 }
}

//zapis n slow o wartosci x
void mem_write_n_w(dword ad,dword x,dword n)
{
 //kopiuj blokami
 while (n>0)
 {
  //jesli pamiec urzadzenia to idz do funkcji urzadzenia
  if (*(byte*)(vmmemtypetab+(ad>>vmmemtypetabshf))>0) ;
  else
  {
   int register k=((ad>>vmmemtypetabshf)+1)<<vmmemtypetabshf;
   if (k-ad>n) k=n; else k=k-ad;
   _memsetw(vmmemaddr+ad,x,k);
   ad+=k;
   n-=k;
  }
 }
}


//======================================

//tablica funkcji instrukcji odczytu pamieci
//[0][]-pamiec, [1][]-urzadzenie
//[][x]-rozmiar byte,word,dword,qword
ptr memcalltabread[2][4]=
{
 //pamiec
 {
  //byte
  _mem_read_b,
  //word
  _mem_read_w,
  //dword
  _mem_read_d,
  //qword
  _mem_read_q
 },
 //urzadzenie
 {
  //byte
  dev_read_b,
  //word
  dev_read_w,
  //dword
  dev_read_d,
  //qword
  dev_read_q
 }
};

//tablica funkcji instrukcji zapisu do pamieci
//[0][]-pamiec, [1][]-urzadzenie
//[][x]-rozmiar byte,word,dword,qword
ptr memcalltabwrite[2][4]=
{
 //pamiec
 {
  //byte
  _mem_write_b,
  //word
  _mem_write_w,
  //dword
  _mem_write_d,
  //qword
  _mem_write_q
 },
 //urzadzenie
 {
  //byte
  dev_write_b,
  //word
  dev_write_w,
  //dword
  dev_write_d,
  //qword
  dev_write_q
 }
};
