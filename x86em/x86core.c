/*
  X86 EMULATOR by Kamil Korolkiewicz 2007
  Plik:x86core.c
  Opis:Glowny modul wykonywania instrukcji procesora oraz funkcje deasemblujace
*/

//zmienne globalne

//adres do pamieci VM i jej rozmiar
ptr vmmemaddr;
dword vmmemsize;

//tablica typu pamieci i rozmiar pojedynczego obszaru uzyskiwany przy >>vmmemtypetabshf
byte *vmmemtypetab;              //0-pamiec systemowa, >0-pamiec urzadzenia
dword vmmemtypetabshf;

//tablica i/o - wartosc oznacza numer urzadzenia, 0 oznacza brak
byte *iotab;

//czy wykonywac kod VM
dword vmrun;

//czestotliwosc sprawdzania urzadzen i pomocniczy licznik
dword devicecontrolfreq;
dword lastdevicecontroltsc;

//czestotliwosc oddania watku systemowi (*devicefreq)
dword sysretfreq;
dword lastsysrettsc;

//rozmiar danych (0-byte,1-word,2-dword)
dword dsize;

//rozmiar adresowania
dword adsize;

//informacje o instrukcji
char *name;
dword flags;
dword params;
dword funcindex;
dword param[3];          //poszczegole parametry (operandy) rozkazu

//aktualna tablica op_t
op_t *curroptab;

//tryb pracy (0-16bit/1-32bit)
byte wordsize;

//wyswiatlane wartosci (10-dec,16-hex)
byte valtype=16;

//======================================

//makra pomocnicze

#define FLAG(f) ((flags&f)!=0)                   //czy ustawiona dana flaga
#define FLAGNSD(f) (((flags&f)!=0)&(!((flags&F_SD)!=0))) //czy ustawiona dana flaga i nie SD
#define FLAGSD(f) (((flags&f)!=0)&((flags&F_SD)!=0))       //czy ustawiona dana flaga i SD
//#define utoa(a,b,c) itoa(a,b,c)


//======================================

//funkcje pomocnicze

void inline utoa(int a,char *b,int c)
{
 itoa (a,b,c);
 if (c==16) strcat (b,"h");
}

dword inline _call(ptr func)
{
 dword register (*f)();
 f=func;
 return f();
}

dword inline _call1p(ptr func,dword a)
{
 dword register (*f)(dword);
 f=func;
 return f(a);
}

dword inline _call2p(ptr func,dword a,dword b)
{
 dword register (*f)(dword,dword);
 f=func;
 return f(a,b);
}


//======================================

//funkcje

/*
  Disasm
  Deasembluje kod w sposob ciagly zapisany w tablicy o rozmiarze count i
  zapisuje na standardowe wyjscie.
  code-tablica z kodem
  tabsize-ilosc bajtow w code
  wl-czy ma byc wypisana w lini pozycja w pliku (w hex)
*/

void Disasm(byte code[],dword tabsize,dword wl)
{
 dword i=0,di,j;
 dword pc;                           //ilosc operandow
 dword op;                           //kod instrukcji
 char s[16],s2[32];                  //ciagi pomocnicze
 char *sp;                           //wskaznik na ciag pomocniczy
 char *p[3];                         //nazwy operandow 1,2,3
 char formname[32];                  //formatowana nazwa
 dword immed1,immed2;                //wartosc bezposrednia
 dword pom;                          //wartosc pomocnicza
 dword prefixnum;                    //numer prefiksa
 while (i<tabsize)
 {
  name=0;
  params=0;
  curroptab=mainoptab;
  pc=0;
  di=i;
  
  //pobieranie kodu instrukcji
  while ((curroptab!=0)&&(i<tabsize))
  {
   op=code[i];
   name+=(dword)curroptab[op].name;
   flags|=curroptab[op].flags;
   params|=curroptab[op].params;
   curroptab=curroptab[op].next;
   i++;
  }
  
  //kod byl przedrostkiem
  if ((flags&PREFIX_MASK)!=0)
  {
   flags=(flags&PREFIX_MASK)>>(5+1);
   prefixnum=0;
   while (flags!=0) {flags>>=1; prefixnum++;}
   //prefiks typu rep instr
   if (prefixnum>5)
   {
    if (wl==1) printf("%08X:",di);
    printf("%s ",prefixnames[prefixnum]);
    wl=2;
   }
   flags=0;
  }
  
  //konkretna instrukcja
  else
  if (name!=0)
  {
   if (wl==1) printf("%08X:",di);
   if (wl==2) wl=1;
   dsize=FLAG(F_D)*(FLAG(F_D)+(2*wordsize-1)*(wordsize-FLAG(F_DP)));         //rozmiar danych
   adsize=wordsize^FLAG(F_MP);                                       //rozmiar adresowania pamieci
   
   //pobierz parametry z params
   if (FLAG(F_PAR))
   {
                   
    for (pc=0; pc<=FLAG(F_CP); pc++)
    {
     param[(pc+FLAG(F_DIR))&1]=params&0xffff;
     params>>=16;
    }
    
    //jesli adres jako wartosc bezposrednia to dodaj 1 parametr
    if (FLAG(F_MIMM))
    {
     switch (adsize)
     {
      case 0: param[(pc+FLAG(F_DIR))&1]=0x10+(0x6<<8); break;
      case 1: param[(pc+FLAG(F_DIR))&1]=0x10+(0x5<<8); break;
     }
     pc++;
    }
    
    //pobieranie paremetrow
    for (j=0; j<pc; j++)
    {
        
     //jesli operand to rejestr
     if (((param[j]&0xff)>>4)==OP_REG)
     {
      p[j]=(char*)(*(int*)((dword)(regnamestab[param[j]&0x0f])+4*3*(param[j]>>8)+dsize*4));
      //printf("%d,%s\n",j,p[j]);
      //strcpy(p[j],*nm);
     }
     
     //jesli operand to pamiec
     else
     if (((param[j]&0xff)>>4)==OP_MEM)
     {
                                      
      //czy wypisywac rozmiar (np. word ptr)
      if ((pc==1) || ((pc==2)&&(((param[(j+1)&1]&0xff)>>4)!=OP_REG)||((flags&ADSIZE_MASK)!=0))||(FLAG(F_SD)))
      {
       //specjalne flagi zmieniajace rozmiar operanda
       if ((flags&ADSIZE_MASK)!=0)
       {
        if (FLAGNSD(F_W)) strcpy(s2,adsizenames[1]);
        else
        if (FLAGSD(F_DWC)) strcpy(s2,adsizenames[2]);
        else
        if (FLAGNSD(F_SW)) strcpy(s2,adsizenames[3]);
        else
        if (FLAGNSD(F_QW)) strcpy(s2,adsizenames[4]);
        else
        if (FLAGSD(F_TWC)) strcpy(s2,adsizenames[5]);
        else
        if (FLAGSD(F_DQWC)) strcpy(s2,adsizenames[6]);
       }
       
       //byte ptr
       else
       if (FLAG(F_SD)) strcpy(s2,adsizenames[0]);
       
       //zwykla opcja
       else
       strcpy(s2,adsizenames[dsize]);
       //prefiks typu cs:[]
       if (prefixnum<=5) strcat(s2,prefixnames[prefixnum]);
       strcat(s2,"[");
      }

      //prefiks typu cs:[]
      else
      if (prefixnum<=5)
      {
       strcpy(s2,prefixnames[prefixnum]);
       strcat(s2,"[");
      }
      else
      strcpy(s2,"[");
      
      sp=adnames[param[j]>>8][adsize];
      
      //przypadek: czy jest to bp lub liczba
      if (sp==AD_IMMBP)
      {
       //wart. bezposr 16bit lub 32bit
       if (adsize==0)
       {
        if ((param[j]&0xf)==MEM_R)
        {
         immed1=*(word*)(code+i);
         i+=2;
         utoa(immed1,s,valtype);
         strcat(s2,s);
        }
        else
        {
         strcat(s2,"bp");
        }
       }
       else
       {
        if ((param[j]&0xf)==MEM_R)
        {
         immed1=*(dword*)(code+i); 
         i+=4;
         utoa(immed1,s,valtype);
         strcat(s2,s);
        }
        else
        {
         strcat(s2,"ebp");
        }
       }       
      } //koniec przypadku: bp lub liczba
      
      //przypadek: adresowanie sib [baza+index*skala]
      else
      if (sp==AD_SIB)
      {
       immed1=code[i++];
       
       //podaj baze
       //jesli baza=immed32
       if (((immed1&7)==5)&&((param[j]&0xf)==MEM_R))
       {
        pom=*(dword*)(code+i);
        i+=4;
        utoa (pom,s,valtype);
        strcat(s2,s);
       }
       else
       //baza=rejestr
       {
        strcat(s2,cpuregsnames[immed1&7][2]);
       }
       
       //podaj index
       pom=(immed1>>3)&7;
       //bez indeksu
       if (pom!=4)
       //z indeksem
       {
        strcat(s2,"+");
        strcat(s2,cpuregsnames[pom][2]);
        //podaj skale
        pom=immed1>>6;
        if (pom!=0)
        {
         pom=1<<pom;
         utoa(pom,s,10);       //zapis dziesietny
         strcat(s2,"*");
         strcat(s2,s);
        }
       }
      } //koniec przypadku: adresowanie sib [baza+index*skala]
      else
      strcat(s2,sp);
      
      //wart. immed8
      if ((param[j]&0xf)==MEM_RIMM8)
      {
       immed1=(byte)code[i]; 
       i+=1;
       utoa(immed1,s,valtype);
       strcat(s2,"+");
       strcat(s2,s);
      }
      
      //wart. immed16/32
      else
      if ((param[j]&0xf)==MEM_RIMM1632)
      {
       if (adsize==0)
       {
        immed1=*(word*)(code+i); 
        i+=2;
        utoa(immed1,s,valtype);
        strcat(s2,"+");
        strcat(s2,s);
       }
       else
       {
        immed1=*(dword*)(code+i);
        i+=4;
        utoa(immed1,s,valtype);
        strcat(s2,"+");
        strcat(s2,s);
       }
      }
      strcat(s2,"]");
      p[j]=s2;
     }
    }
   } //koniec pobierania parametrow
   
   //dodatkowy parametr cl (dla shl/rd)
   if (FLAG(F_XCL))
   {
    p[pc]="cl";
    pc++;
   }
   
   //parametr 2 jako bajt
   if (FLAG(F_PAR1D)&&FLAG(F_PAR2D))
   {
    if (((param[1]&0xff)>>4)==OP_REG)
    p[1]=(char*)(*(int*)((dword)(regnamestab[param[1]&0x0f])+4*3*(param[1]>>8)));
   }
   
   //parametr 1 jako slowo
   else
   if (FLAG(F_PAR1D))
   {
    if (((param[0]&0xff)>>4)==OP_REG)
    p[0]=(char*)(*(int*)((dword)(regnamestab[param[0]&0x0f])+4*3*(param[0]>>8)+1*4));
   }
   
   //parametr 2 jako slowo
   else
   if (FLAG(F_PAR2D))
   {
    if (((param[1]&0xff)>>4)==OP_REG)
    p[1]=(char*)(*(int*)((dword)(regnamestab[param[1]&0x0f])+4*3*(param[1]>>8)+1*4));
   }
   
   //wartosc bezposrednia jako argument
   if (FLAG(F_IMM))
   {
    //jesli immed ma wart 1
    if (FLAG(F_IMM1)) immed1=1;
    
    //w typowym przypadku
    else
    {
     //specjalne dla IMMB
     pom=dsize;
     dsize=(1-FLAG(F_IMMB))*dsize;
     switch (dsize)
     {
      case 0:immed1=(byte)code[i++]; break;
      case 1:immed1=*(word*)(code+i); i+=2; break;
      case 2:immed1=*(dword*)(code+i); i+=4; break;
     }
    }
     //jesli wartosc bezposrednia relatywna
    if (FLAG(F_REL))
    {
     switch (dsize)
     {
      case 0:immed1=(sbyte)immed1+i; break;
      case 1:immed1=(sword)immed1+i; break;
      case 2:immed1=(sdword)immed1+i; break;
     }
    }
    
    utoa(immed1,s,valtype);
    if (pc==1) p[pc]=s;
    else
    p[pc]=s;
    pc++;
    dsize=pom;
   }
   
   //wartosc bezposrednia jako adres seg:ofs
   else
   if (FLAG(F_PTR))
   {
    switch (dsize)
    {
     case 0:
     case 1:immed1=*(word*)(code+i); i+=2; break;
     case 2:immed1=*(dword*)(code+i); i+=4; break;
    }
    immed2=*(word*)(code+i); 
    i+=2;
    utoa(immed2,s2,valtype);
    utoa(immed1,s,valtype);
    strcat(s2,":");
    strcat(s2,s);
    p[pc]=s2;
    pc++;
   }
   
   //podwojna wartosc bezposrednia 16bit i 8bit
   else
   if (FLAG(F_DIMM))
   {
    immed1=*(word*)(code+i); 
    i+=2;
    immed2=code[i++];
    utoa(immed1,s,valtype);
    utoa(immed2,s2,valtype);
    p[pc++]=s;
    p[pc++]=s2;
   }
   
   //jesli w nazwie wystepuje formatowanie
   if (FLAG(F_FORMS))
   {
    j=0;
    while (name[j]!=0)
    {
     if ((name[j]=='*')||(name[j]=='#')||(name[j]=='|')||(name[j]=='$')) break;
     j++;
    }
    strcpy(formname,name);
    switch (name[j])
    {
     case '*':if (dsize==2) formname[j]='d'; else formname[j]='w'; break;
     case '#':if (dsize==2) formname[j]='d'; else formname[j]=0; break;
     case '|':if (dsize==2) strcpy (formname,name+j+1); else formname[j]=0; break;
     case '$':if (dsize==2) formname[j]='e'; else strcpy(formname+j,name+j+1);
    }
    name=formname;
   }
      
   //wypisz instrukcje na wyjscie
   if (pc==0)
   {
    printf(name);
   }
   else
   {
    //jesli znacznik far
    if (FLAG(F_FAR)) 
    printf ("%s far ",name);
    else
    printf ("%s ",name);
    
    //wypisz parametry
    printf(p[0]);
    if (pc>1) printf(",%s",p[1]);
    if (pc>2) printf(",%s",p[2]);
   }
   //printf("%d\n",pc);
   //printf(" %d\n",dsize);
   printf("\n");
   flags=0;
   prefixnum=-1;
  } //koniec name!=0  
  
  else
  //jesli byly 2 (lub wiecej) opcody bez znaczenia to wyzeruj flagi
  if (i-di>1) 
  flags=0;
  //printf("%x:",i);
 }
}

//funkcje laczace wirtualna maszyne z systemem


/*
  WinControl
  Oddaje cykle procesora systemowi
*/
void WinControl()
{
 lastsysrettsc=0;
}

/*
  DeviceControl
  Rozporzadza przerwaniami irq urzadzen oraz obsluguje dma. Pozwala tez na
  oddanie cykli procesora systemowi
*/
void DeviceControl()
{
 lastdevicecontroltsc=tsc;
 lastsysrettsc++;
 if (lastsysrettsc>sysretfreq) WinControl();
}

//funkcje wirtualnej maszyny w trybie normalnym

/*
  VMn_Init
  Inicjuje wirtualna maszyne w trybie normalnym na podstawie strukt. vm
  Jesli udalo sie utworzyc VM to zwraca 1:numer bledu
*/
int VMn_Init(VM_t vm)
{
 //inicjuj pamiec
 if (vm.memsize<1024*1024) return 0;
 vmmemsize=vm.memsize;
 vmmemaddr=AllocMem(vmmemsize);
 if (vmmemaddr==0) return INITERR_NOAVAILMEM;
 vmmemtypetabshf=vm.memtypetabshf;
 vmmemtypetab=AllocMem((vmmemsize>>vmmemtypetabshf)+1);
 if (vmmemtypetab==0) {FreeMem(vmmemaddr); return INITERR_NOAVAILMEM;}
 iotab=AllocMem(65536);
 if (iotab==0) return  {FreeMem(vmmemaddr); FreeMem(vmmemtypetab); INITERR_NOAVAILMEM;}
 
 //inicjuj rejestry
 cpusregs[CS]=CS_IN_START;
 eip=EIP_IN_START;
 tsc=0;
 
 //utworz tablice addrtabs
 int i=1,j=0;
 while (i<=64) {addrtabs[i]=j++; i<<=1;}
 
 wordsize=0;
 
 vmrun=1;
 
 devicecontrolfreq=vm.devicecontrolfreq;
 sysretfreq=vm.sysretfreq;
 lastdevicecontroltsc=0;
 lastsysrettsc=0;
 
 return 1;
}

/*
  VMn_Close
  Zamyka wirtualna maszyne w trybie normalnym
*/
void VMn_Close()
{
 FreeMem(vmmemaddr);
 vmrun=0;
}

/*
  VMn_Start
  Uruchamia wirtualny PC i wykonuje go az do zewnetrznego przerwania uzytkownika
*/

void VMn_Start()
{
 while (vmrun)
 {
  flags=0;
noinstr:  
  params=0;
  curroptab=mainoptab;
  funcindex=0;
  
  //pobieranie kodu instrukcji
  while (curroptab!=0)
  {
   register byte op=mem_code_b();
   flags|=curroptab[op].flags;
   params|=curroptab[op].params;
   funcindex|=curroptab[op].funcindex;
   curroptab=curroptab[op].next;
   eip++;
  }
  if (funcindex==0) goto noinstr;
  
  //pobierz parametry w odpowiedniej kolejnosci do tablicy param
  int register i;
  for (i=0; i<=FLAG(F_CP); i++)
  {
   param[(i+FLAG(F_DIR))&1]=params&0xffff;
   params>>=16;
  }
  
  dsize=FLAG(F_D)*(FLAG(F_D)+(2*wordsize-1)*(wordsize-FLAG(F_DP)));         //rozmiar danych
  //rozmiar adresowania pamieci 0-16bit,1-32bit w trybie 16bit,2-32bit
  adsize=2*wordsize+(FLAG(F_MP)&(wordsize-1));
  
  //wywolanie instrukcji procesora
  _call(x86calltab[funcindex][dsize]);
  
  //zwieksz licznik instrukcji tsc
  tsc++;
  
  //skontaktuj sie z wirtualnym sprzetem oraz z systemem operacyjnym
  if (tsc-lastdevicecontroltsc>devicecontrolfreq) DeviceControl();
 }
}
