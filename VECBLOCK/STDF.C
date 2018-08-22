#ifndef __STDF_C
#define __STDF_C

//Biblioteka standardowych funkcji i zmiennych dla vb_expr.c

//======================================

#define STDF_MAX 21+3+2+2+1         //liczba funkcji i zmiennych lacznie

//======================================

//standardowe funckje matematyczne
float stdf_sin        (float a)         {return sin(a);}
float stdf_cos        (float a)         {return cos(a);}
float stdf_tg         (float a)         {return tan(a);}
float stdf_ctg        (float a)         {return 1/tan(a);}
float stdf_asin       (float a)         {return asin(a);}
float stdf_acos       (float a)         {return acos(a);}
float stdf_atg        (float a)         {return atan(a);}
float stdf_actg       (float a)         {return atan(1/a);}
float stdf_sqrt       (float a)         {return sqrt(a);}
float stdf_cbrt       (float a)         {return cbrt(a);}
float stdf_log10      (float a)         {return log10(a);}
float stdf_ln         (float a)         {return log(a);}
float stdf_log2       (float a)         {return log2(a);}
float stdf_abs        (float a)         {return fabs(a);}
float stdf_floor      (float a)         {return floor(a);}
float stdf_ceil       (float a)         {return ceil(a);}
float stdf_hypot      (float x,float y) {return sqrt (x*x+y*y);}
float stdf_powi       (float a,int b)   {return powi(a,b);}
float stdf_sgn        (float a)         {return (a==0)? 0 : (a>0)? 1:-1;}
float stdf_rad        (float a)         {return (3.14159265f/180.0)*a;}
float stdf_deg        (float a)         {return a/(3.14159265f*180.0);}

//======================================

//funcje tekstowe
//pisze formatowany ciag i liczbe typu float na konsoli
int stdf_con_printff (char *str,float a)
{
 #ifndef STDPRINT
 Con_Printf (str,a);
 #else
 printf (str,a);
 #endif
 return 0;
}
//pisze formatowany ciag i liczbe typu int na konsoli
int stdf_con_printfl (char *str,int a)
{
 #ifndef STDPRINT
 Con_Printf (str,a);
 #else
 printf (str,a);
 #endif
 return 0;
}
//pisze ciag na konsoli
int stdf_con_echo (char *str1)
{
 #ifndef STDPRINT
 Con_Print (str1);
 #else
 printf ("%s\n",str1);
 #endif
 return 0;
}

//======================================

//funkcje graficzne
//ustawia dany tryb graficzny
int stdf_setvidmode (int mode)
{
 float w=vid_buf.sheight,w2;
 if (Vid_SetMode(mode)==0)
 {
  Con_Printf("Vid_SetMode(%d) failed!",mode);
  return 0;
 }
 Menu_VidApply();
 w2=(float)vid_buf.sheight/w;
 Con_SetnLines((float)con_lines*w2);
 Con_Printf("Current video mode: %dX%d",vid_buf.swidth,vid_buf.sheight);
 return mode;
}

int stdf_setbrightness(int b)
{
 Vid_SetBrightness(b);
 Con_Printf("Brightness: %d",b);
}

int stdf_dumpmem(unsigned int addr)
{
 int i,j;
 unsigned char *wsk=(unsigned char*)addr;
 char buf[256],buf2[6];
 Con_Printf("Dump at %X",addr);
 if (addr>__dpmi_get_segment_limit(_my_ds())) return 0;
 for (i=0;i<0x20;i++)
 {
  sprintf(buf,"%08X:",wsk);
  for (j=0;j<0x8;j++)
  {
   sprintf(buf2,"%02X ",*wsk);
   strcat(buf,buf2);
   wsk++;
  }
  wsk-=0x8;
  buf2[1]=0;
  for (j=0;j<0x8;j++)
  {
   buf2[0]=*wsk;
   if (buf2[0]==0) buf2[0]=0xff;
   if (buf2[0]==10) buf2[0]=0xff;
   if (buf2[0]==13) buf2[0]=0xff;
   strcat(buf,buf2);
   wsk++;
  }
  Con_Print(buf);
 }
}

int stdf_dumpmem0(unsigned int addr)
{
 int i,j;
 unsigned char *wsk=(unsigned char*)addr;
 char buf[256],buf2[6];
 Con_Printf("Dump at %X",addr);
 for (i=0;i<0x20;i++)
 {
  sprintf(buf,"%08X:",wsk);
  for (j=0;j<0x8;j++)
  {
   sprintf(buf2,"%02X ",__peek(wsk));
   strcat(buf,buf2);
   wsk++;
  }
  wsk-=0x8;
  buf2[1]=0;
  for (j=0;j<0x8;j++)
  {
   buf2[0]=__peek(wsk);
   if (buf2[0]==0) buf2[0]=0xff;
   if (buf2[0]==10) buf2[0]=0xff;
   if (buf2[0]==13) buf2[0]=0xff;
   strcat(buf,buf2);
   wsk++;
  }
  Con_Print(buf);
 }
}

//======================================
//biblioteka
TFVLib stdf[STDF_MAX]=
{
//standardowe funckje matematyczne
 {"sin",        {stdf_sin,1,1|2,0}},
 {"cos",        {stdf_cos,1,1|2,0}},
 {"tg",         {stdf_tg,1,1|2,0}},
 {"ctg",        {stdf_ctg,1,1|2,0}},
 {"asin",       {stdf_asin,1,1|2,0}},
 {"acos",       {stdf_acos,1,1|2,0}},
 {"atg",        {stdf_atg,1,1|2,0}},
 {"actg",       {stdf_actg,1,1|2,0}},
 {"sqrt",       {stdf_sqrt,1,1|2|4,0}},
 {"cbrt",       {stdf_sqrt,1,1|2,0}},
 {"log10",      {stdf_log10,1,1|2,0}},
 {"ln",         {stdf_ln,1,1|2,0}},
 {"log2",       {stdf_log2,1,1|2,0}},
 {"abs",        {stdf_abs,1,1|2,0}},
 {"floor",      {stdf_floor,1,1|2,0}},
 {"ceil",       {stdf_ceil,1,1|2,0}},
 {"hypot",      {stdf_hypot,2,1|2|4,0}},
 {"powi",       {stdf_powi,2,1|2,0}},
 {"sgn",        {stdf_sgn,1,1|2,0}},
 {"rad",        {stdf_rad,1,1|2,0}},
 {"deg",        {stdf_deg,1,1|2,0}},
 
//funcje tekstowe dla konsoli
 {"con_printff",{stdf_con_printff,2,4,0}},
 {"con_printfl",{stdf_con_printfl,2,0,0}},
 {"con_echo",   {stdf_con_echo,1,0,0}},

//funkcje video
 {"setvidmode",{stdf_setvidmode,1,0,0}},
 {"setbrightness",{stdf_setbrightness,1,0,0}},

//debug
 {"dumpmem",{stdf_dumpmem,1,0,0}},
 {"dumpmem0",{stdf_dumpmem0,1,0,0}},

//zmienne
 {"pi",{0,0,0,3.14159265f}}
};

#endif
