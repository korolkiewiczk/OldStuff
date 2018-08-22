/*
  X86 EMULATOR by Kamil Korolkiewicz 2007
  Plik:x86addr.c
  Opis:Modul obslugujacy adresowanie pamieci
*/


//funkcje inline zwracajace wartosc rejestru segmetowego<<4

byte addrtabs[65];

//ds lub prefix
dword inline addr_seg(dword seg)
{
 dword register a=((flags&SEG_MASK)>>5);
 return (dword)(cpusregs[seg*(a==0)+addrtabs[a]])<<4;
}

//======================================

//funkcje adresujace, zwracaja adres fizyczny lub wirtualny (zalezy od trybu)


//adresowanie w trybie 16bit

//adresowanie 16bit

//bx+si
dword addr_bx_si()
{
 return addr_seg(DS)+(cpuregs[EBX]+cpuregs[ESI])&0xffff;
}

//bx+di
dword addr_bx_di()
{
 return addr_seg(DS)+(cpuregs[EBX]+cpuregs[EDI])&0xffff;
}

//bp+si
dword addr_bp_si()
{
 return addr_seg(SS)+(cpuregs[EBP]+cpuregs[ESI])&0xffff;
}

//bp+di
dword addr_bp_di()
{
 return addr_seg(SS)+(cpuregs[EBP]+cpuregs[EDI])&0xffff;
}

//si
dword addr_si()
{
 return addr_seg(DS)+(cpuregs[ESI])&0xffff;
}

//di
dword addr_di()
{
 return addr_seg(DS)+(cpuregs[EDI])&0xffff;
}

//bx
dword addr_bx()
{
 return addr_seg(DS)+(cpuregs[EBX])&0xffff;
}

//r+imm8

//bx+si+imm8
dword addr_bx_si_imm8()
{
 return addr_seg(DS)+(cpuregs[EBX]+cpuregs[ESI]+mem_code_b())&0xffff;
}

//bx+di+imm8
dword addr_bx_di_imm8()
{
 return addr_seg(DS)+(cpuregs[EBX]+cpuregs[EDI]+mem_code_b())&0xffff;
}

//bp+si+imm8
dword addr_bp_si_imm8()
{
 return addr_seg(SS)+(cpuregs[EBP]+cpuregs[ESI]+mem_code_b())&0xffff;
}

//bp+di+imm8
dword addr_bp_di_imm8()
{
 return addr_seg(SS)+(cpuregs[EBP]+cpuregs[EDI]+mem_code_b())&0xffff;
}

//si+imm8
dword addr_si_imm8()
{
 return addr_seg(DS)+(cpuregs[ESI]+mem_code_b())&0xffff;
}

//di+imm8
dword addr_di_imm8()
{
 return addr_seg(DS)+(cpuregs[EDI]+mem_code_b())&0xffff;
}

//bp+imm8
dword addr_bp_imm8()
{
 return addr_seg(DS)+(cpuregs[EBP]+mem_code_b())&0xffff;
}

//bx+imm8
dword addr_bx_imm8()
{
 return addr_seg(DS)+(cpuregs[EBX]+mem_code_b())&0xffff;
}

//r+imm16

//bx+si+imm16
dword addr_bx_si_imm16()
{
 return addr_seg(DS)+(cpuregs[EBX]+cpuregs[ESI]+mem_code_w())&0xffff;
}

//bx+di+imm16
dword addr_bx_di_imm16()
{
 return addr_seg(DS)+(cpuregs[EBX]+cpuregs[EDI]+mem_code_w())&0xffff;
}

//bp+si+imm16
dword addr_bp_si_imm16()
{
 return addr_seg(SS)+(cpuregs[EBP]+cpuregs[ESI]+mem_code_w())&0xffff;
}

//bp+di+imm16
dword addr_bp_di_imm16()
{
 return addr_seg(SS)+(cpuregs[EBP]+cpuregs[EDI]+mem_code_w())&0xffff;
}

//si+imm16
dword addr_si_imm16()
{
 return addr_seg(DS)+(cpuregs[ESI]+mem_code_w())&0xffff;
}

//di+imm16
dword addr_di_imm16()
{
 return addr_seg(DS)+(cpuregs[EDI]+mem_code_w())&0xffff;
}

//bp+imm16
dword addr_bp_imm16()
{
 return addr_seg(DS)+(cpuregs[EBP]+mem_code_b())&0xffff;
}

//bx+imm16
dword addr_bx_imm16()
{
 return addr_seg(DS)+(cpuregs[EBX]+mem_code_w())&0xffff;
}


//imm16

dword addr_imm16()
{
 return addr_seg(DS)+mem_code_w()&0xffff;
}

//adresowanie pseudo 32bit

//eax
dword addr_eax16()
{
 return addr_seg(DS)+(cpuregs[EAX])&0xffff;
}

//ebx
dword addr_ebx16()
{
 return addr_seg(DS)+(cpuregs[EBX])&0xffff;
}

//ecx
dword addr_ecx16()
{
 return addr_seg(DS)+(cpuregs[ECX])&0xffff;
}

//edx
dword addr_edx16()
{
 return addr_seg(DS)+(cpuregs[EDX])&0xffff;
}

//esi
dword addr_esi16()
{
 return addr_seg(DS)+(cpuregs[ESI])&0xffff;
}

//edi
dword addr_edi16()
{
 return addr_seg(DS)+(cpuregs[EDI])&0xffff;
}

//r+imm8

//eax+imm8
dword addr_eax_imm816()
{
 return addr_seg(DS)+(cpuregs[EAX]+mem_code_b())&0xffff;
}

//ebx+imm8
dword addr_ebx_imm816()
{
 return addr_seg(DS)+(cpuregs[EBX]+mem_code_b())&0xffff;
}

//ecx+imm8
dword addr_ecx_imm816()
{
 return addr_seg(DS)+(cpuregs[ECX]+mem_code_b())&0xffff;
}

//edx+imm8
dword addr_edx_imm816()
{
 return addr_seg(DS)+(cpuregs[EDX]+mem_code_b())&0xffff;
}

//esi+imm8
dword addr_esi_imm816()
{
 return addr_seg(DS)+(cpuregs[ESI]+mem_code_b())&0xffff;
}

//edi+imm8
dword addr_edi_imm816()
{
 return addr_seg(DS)+(cpuregs[EDI]+mem_code_b())&0xffff;
}

//ebp+imm8
dword addr_ebp_imm816()
{
 return addr_seg(DS)+(cpuregs[EBP]+mem_code_b())&0xffff;
}


//r+imm32

//eax+imm32
dword addr_eax_imm3216()
{
 return addr_seg(DS)+(cpuregs[EAX]+mem_code_d())&0xffff;
}

//ebx+imm32
dword addr_ebx_imm3216()
{
 return addr_seg(DS)+(cpuregs[EBX]+mem_code_d())&0xffff;
}

//ecx+imm32
dword addr_ecx_imm3216()
{
 return addr_seg(DS)+(cpuregs[ECX]+mem_code_d())&0xffff;
}

//edx+imm32
dword addr_edx_imm3216()
{
 return addr_seg(DS)+(cpuregs[EDX]+mem_code_d())&0xffff;
}

//esi+imm32
dword addr_esi_imm3216()
{
 return addr_seg(DS)+(cpuregs[ESI]+mem_code_d())&0xffff;
}

//edi+imm32
dword addr_edi_imm3216()
{
 return addr_seg(DS)+(cpuregs[EDI]+mem_code_d())&0xffff;
}

//ebp+imm32
dword addr_ebp_imm3216()
{
 return addr_seg(DS)+(cpuregs[EBP]+mem_code_d())&0xffff;
}

//imm32
dword addr_imm3216()
{
 return addr_seg(DS)+mem_code_d()&0xffff;
}

//sib
dword addr_sib16()
{
 byte register sib=mem_code_b();
 if ((sib&7)==5)
 return addr_seg(DS)+(mem_code_d()+cpuregs[(sib>>3)&7]*(((sib>>3)&7)!=4)*(1<<(sib>>6)))&0xffff;
 else
 return addr_seg(((sib&7)==4)? SS:DS)+
 (cpuregs[sib&7]+cpuregs[(sib>>3)&7]*(((sib>>3)&7)!=4)*(1<<(sib>>6)))&0xffff;
}

//sib+imm8
dword addr_sib_imm816()
{
 byte register sib=mem_code_b();
 return addr_seg((((sib&7)>=4)&((sib&7)<=5))? SS:DS)+
 (cpuregs[sib&7]+cpuregs[(sib>>3)&7]*(((sib>>3)&7)!=4)*(1<<(sib>>6))+mem_code_b())&0xffff;
}

//sib+imm32
dword addr_sib_imm3216()
{
 byte register sib=mem_code_b();
 return addr_seg((((sib&7)>=4)&((sib&7)<=5))? SS:DS)+
 (cpuregs[sib&7]+cpuregs[(sib>>3)&7]*(((sib>>3)&7)!=4)*(1<<(sib>>6))+mem_code_d())&0xffff;
}


//======================================

//adresowanie w trybie 32bit

//eax
dword addr_eax()
{
 return cpuregs[EAX];
}

//ebx
dword addr_ebx()
{
 return cpuregs[EBX];
}

//ecx
dword addr_ecx()
{
 return cpuregs[ECX];
}

//edx
dword addr_edx()
{
 return cpuregs[EDX];
}

//esi
dword addr_esi()
{
 return cpuregs[ESI];
}

//edi
dword addr_edi()
{
 return cpuregs[EDI];
}

//r+imm8

//eax+imm8
dword addr_eax_imm8()
{
 return cpuregs[EAX]+mem_code_b();//+doit
}

//ebx+imm8
dword addr_ebx_imm8()
{
 return cpuregs[EBX]+mem_code_b();//+doit
}

//ecx+imm8
dword addr_ecx_imm8()
{
 return cpuregs[ECX]+mem_code_b();//+doit
}

//edx+imm8
dword addr_edx_imm8()
{
 return cpuregs[EDX]+mem_code_b();//+doit
}

//esi+imm8
dword addr_esi_imm8()
{
 return cpuregs[ESI]+mem_code_b();//+doit
}

//edi+imm8
dword addr_edi_imm8()
{
 return cpuregs[EDI]+mem_code_b();//+doit
}

//ebp+imm8
dword addr_ebp_imm8()
{
 return cpuregs[EBP]+mem_code_b();//+doit
}


//r+imm32

//eax+imm32
dword addr_eax_imm32()
{
 return cpuregs[EAX]+mem_code_d();//+doit
}

//ebx+imm32
dword addr_ebx_imm32()
{
 return cpuregs[EBX]+mem_code_d();//+doit
}

//ecx+imm32
dword addr_ecx_imm32()
{
 return cpuregs[ECX]+mem_code_d();//+doit
}

//edx+imm32
dword addr_edx_imm32()
{
 return cpuregs[EDX]+mem_code_d();//+doit
}

//esi+imm32
dword addr_esi_imm32()
{
 return cpuregs[ESI]+mem_code_d();//+doit
}

//edi+imm32
dword addr_edi_imm32()
{
 return cpuregs[EDI]+mem_code_d();//+doit
}

//ebp+imm32
dword addr_ebp_imm32()
{
 return cpuregs[EBP]+mem_code_d();//+doit
}

//doit--

//imm32
dword addr_imm32()
{
 return mem_code_d();
}

//sib
dword addr_sib()
{
}

//sib+imm8
dword addr_sib_imm8()
{
}

//sib+imm32
dword addr_sib_imm32()
{
}

//--

//======================================

//tablica wywolan
//[0][][]-tryb 16bit,[1][][]-adresowanie 32bit w trybie 16bit,[2][][]-tryb 32bit
//[][x][]-x oznacza czy przesuniecie

ptr addrcalltab[3][3][8]=
{
 //16bit
 {
  //10h
  {
   addr_bx_si,
   addr_bx_di,
   addr_bp_si,
   addr_bp_di,
   addr_si,
   addr_di,
   addr_imm16,
   addr_bx
  },
  //11h
  {
   addr_bx_si_imm8,
   addr_bx_di_imm8,
   addr_bp_si_imm8,
   addr_bp_di_imm8,
   addr_si_imm8,
   addr_di_imm8,
   addr_bp_imm8,
   addr_bx_imm8
  },
  //12h
  {
   addr_bx_si_imm16,
   addr_bx_di_imm16,
   addr_bp_si_imm16,
   addr_bp_di_imm16,
   addr_si_imm16,
   addr_di_imm16,
   addr_bp_imm16,
   addr_bx_imm16
  }
 },
 //16bit (67h)
 {
  //10h
  {
   addr_eax16,
   addr_ecx16,
   addr_edx16,
   addr_ebx16,
   addr_sib16,
   addr_imm3216,
   addr_esi16,
   addr_edi16
  },
  //11h
  {
   addr_eax_imm816,
   addr_ecx_imm816,
   addr_edx_imm816,
   addr_ebx_imm816,
   addr_sib_imm816,
   addr_ebp_imm816,
   addr_esi_imm816,
   addr_edi_imm816
  },
  //12h
  {
   addr_eax_imm3216,
   addr_ecx_imm3216,
   addr_edx_imm3216,
   addr_ebx_imm3216,
   addr_sib_imm3216,
   addr_ebp_imm3216,
   addr_esi_imm3216,
   addr_edi_imm3216
  }
 },
 //32bit
 {
  //10h
  {
   addr_eax,
   addr_ecx,
   addr_edx,
   addr_ebx,
   addr_sib,
   addr_imm32,
   addr_esi,
   addr_edi
  },
  //11h
  {
   addr_eax_imm8,
   addr_ecx_imm8,
   addr_edx_imm8,
   addr_ebx_imm8,
   addr_sib_imm8,
   addr_ebp_imm8,
   addr_esi_imm8,
   addr_edi_imm8
  },
  //12h
  {
   addr_eax_imm32,
   addr_ecx_imm32,
   addr_edx_imm32,
   addr_ebx_imm32,
   addr_sib_imm32,
   addr_ebp_imm32,
   addr_esi_imm32,
   addr_edi_imm32
  }
 },
};
