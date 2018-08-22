#ifndef __VB_KEY_C
#define __VB_KEY_C
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_KEY.C
 Opis: Obsluga klawiatury
------------------------------------------------
*/

//===========================================

#include "vb_key.h"

//===========================================

__dpmi_paddr __oldint09vec;
unsigned char key_key2chartab[KEY_K2CN][3]=
{
 { 2,'1','!'},{ 3,'2','@'},{ 4,'3','#'},{ 5,'4','$'},{ 6,'5','%'},{ 7,'6','^'},
 { 8,'7','&'},{ 9,'8','*'},{10,'9','('},{11,'0',')'},{12,'-','_'},{13,'=','+'},
 {14,  8,  8},{15,'\t','\t'},{16,'q','Q'},{17,'w','W'},{18,'e','E'},{19,'r','R'},
 {20,'t','T'},{21,'y','Y'},{22,'u','U'},{23,'i','I'},{24,'o','O'},{25,'p','P'},
 {26,'[','{'},{27,']','}'},{28,'\n','\n'},{30,'a','A'},{31,'s','S'},{32,'d','D'},
 {33,'f','F'},{34,'g','G'},{35,'h','H'},{36,'j','J'},{37,'k','K'},{38,'l','L'},
 {39,';',':'},{40,39,'"'},{43,'\\','|'},{44,'z','Z'},{45,'x','X'},{46,'c','C'},
 {47,'v','V'},{48,'b','B'},{49,'n','N'},{50,'m','M'},{51,',','<'},{52,'.','>'},
 {53,'/','?'},{55,'*','*'},{57,' ',' '},{74,'-','-'},{78,'+','+'},{83,127,127}
};

char key_names[128][8]=
{
 "esc","1","2","3","4","5","6","7","8","9","0","-","=","bcksp", //1..14
 "tab","q","w","e","r","t","y","u","i","o","p","[","]","enter","ctrl", //15..29
 "a","s","d","f","g","h","j","k","l",";","'","~", //30..41
 "lshift","\\","z","x","c","v","b","n","m",",",".","/", //42..53
 "rshift","*","alt","space","capslck", //54..58
 "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10", //59..68
 "numlck","scrllck", //69..70
 "home","up","pgup","-","left","5","right","+","end","down","pgdown","ins","del", //71..83
 "","","","F11","F12" //84..88
};

unsigned char key_scantokeytab_save[128];

/*
########################################
 InitKeySys
 Inicjuje system obslugi klawiatury.
 Jesli ok 1 : 0
########################################
*/

int InitKeySys()
{
 int i;
 for (i=0; i<128; i++)
 {
  key_keytab[i]=0;
  key_scantokeytab[i]=i;
 }
 __dpmi_paddr pa;
 __dpmi_get_protected_mode_interrupt_vector (0x9,&__oldint09vec);
 pa.offset32=(int)&__Int09_Keyboard_Handler;
 pa.selector=_go32_my_cs();
 if (__dpmi_set_protected_mode_interrupt_vector(0x9,&pa)==-1) return 0;
 return 1;
}

/*
########################################
 CloseKeySys
 Zamyka system obslugi klawiatury.
########################################
*/

void CloseKeySys()
{
 __dpmi_set_protected_mode_interrupt_vector (0x9,&__oldint09vec);
}

/*
########################################
 Key_IsP
 Makro sprawdza czy dany klawisz jest nacisniety
 Mozna tez uzyc key_keytab[a].
 Jesli tak 1 : 0
########################################
*/

#define Key_IsP(a) key_keytab[a]

/*
########################################
 Key_CGet
 Sprawdza czy dany klawisz jest nacisniety i 'puszcza' dany klawisz
 Jesli tak 1 : 0
########################################
*/

inline int Key_CGet(int a)
{
 register unsigned char w=key_keytab[a];
 key_keytab[a]=0;
 return w;
}

/*
########################################
 Key_Key2Char
 Zamienia kod klawisza na znak ASCII
 Zwraca kod ASCII znaku lub -1
########################################
*/

int Key_Key2Char(int k)
{
 int l=0,p=KEY_K2CN,s,c;
 while (l<=p)
 {
  s=(l+p)/2;
  c=key_key2chartab[s][0];
  if (c==k)
  {
   return key_key2chartab[s][key_isshiftp+1];
  }
  if (c<k) l=s+1; else p=s-1;
 }
 return -1;
}

/*
########################################
 Key_Bind
 Zmienia znaczenie danego klawisza
########################################
*/

inline void Key_Bind (int k,int nk)
{
 key_scantokeytab[nk]=k;
 key_scantokeytab[k]=nk;
}

/*
########################################
 Key_Defaults
 Przywraca domyslne ustawienie tablicy scantokeytab i zapamietuje poprzednie
 ustawienia. Funkcja uslugowa dla konsoli.
########################################
*/

void Key_Defaults()
{
 memcpy (&key_scantokeytab_save,&key_scantokeytab,128); //zapamietaj poprzednie ustawienia
 int i;
 for (i=0; i<128; i++)
 {
  key_keytab[i]=0;
  key_scantokeytab[i]=i;
 }
}

/*
########################################
 Key_Restore
 Przywraca poprzednie ustawienia tablicy scantokeytab
########################################
*/

void Key_Restore()
{
 memcpy (&key_scantokeytab,&key_scantokeytab_save,128); //przywroc poprzednia ustawienia
 int i;
 for (i=0; i<128; i++) key_keytab[i]=0;
}

#endif
