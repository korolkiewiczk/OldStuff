/*
  X86 EMULATOR by Kamil Korolkiewicz 2007
  Plik:x86device.c
  Opis:Podstawowa obsluga urzadzen
*/

//tablica urzadzen I/O
device_t *devicetab[DEV_MAX];
dword dev_n;                      //liczba urzadzen


//======================================

//funkcja obslugi urzadzen

/*
  AddDevice
  Dodaje urzadzenie do listy urzadzen. Uzupelnia przedzialy i/o i pamieci
  Ok 1:0
*/
dword AddDevice(device_t *dev)
{
 if (dev_n==DEV_MAX) return 0;
 devicetab[dev_n]=dev;
 
 range_t *r;
 
 //inicjuj tablice i/o
 r=dev->iorange;
 while (r)
 {
  //debug
   if (r->a>65535) r->a=65535;
   if (r->b>65535) r->b=65535;
  //
  _memsetb(iotab+r->a,dev_n,r->b-r->a+1);
  r=r->next;
 }
 
 //inicjuj tablice pamieci
 r=dev->memrange;
 while (r)
 {
  //debug
   if (r->a>vmmemsize) r->a=vmmemsize;
   if (r->b>vmmemsize) r->b=vmmemsize;
  //
  //bierze zawsze wiekszy przedzial (po uwzglednieniu dzielenia a i b)
  _memsetb(vmmemtypetab+(r->a>>vmmemtypetabshf),dev_n,((r->b-r->a)>>vmmemtypetabshf)+1);
  r=r->next;
 }
 
 dev_n++;
 return 1;
}


//funkcje odczytu z pamieci urzadzen

dword dev_read_b(dword addr)
{
}

dword dev_read_w(dword addr)
{
}

dword dev_read_d(dword addr)
{
}

dword dev_read_q(dword addr)
{
}

//funkcje zapisu do pamieci urzadzen

void dev_write_b(dword addr,dword x)
{
}

void dev_write_w(dword addr,dword x)
{
}

void dev_write_d(dword addr,dword x)
{
}

void dev_write_q(dword addr,qword x)
{
}
