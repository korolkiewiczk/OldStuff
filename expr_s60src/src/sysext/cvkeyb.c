//cvkeyb.c

unsigned char CVKeyb::vkeymap[3*3*VKEYB_K_NPART][2]
                       ={{'q','Q'},{'w','W'},{'e','E'},
                         {'a','A'},{'s','S'},{'d','D'},
                         {'z','Z'},{'x','X'},{'c','C'},
                        
                         {'r','R'},{'t','T'},{'y','Y'},
                         {'f','F'},{'g','G'},{'h','H'},
                         {'v','V'},{'b','B'},{'n','N'},
                        
                         {'u','U'},{'i','I'},{'o','O'},
                         {'j','J'},{'k','K'},{'l','L'},
                         {'m','M'},{',','<'},{'.','>'},
                        
                         {'p','P'},{'[','{'},{']','}'},
                         {';',':'},{'\'','"'},{'-','_'},
                         {'/','?'},{'\\','|'},{'=','+'},
                        
                         {192,201},{193,202},{194,203},
                         {195,204},{196,205},{197,206},
                         {198,207},{199,208},{200,209}};

unsigned char CVKeyb::extrakeymap[3*3*2][2]=
{
 {K_F4,192},{K_F5,193},{K_F6,194}
,{70,195},{K_HOME,196},{K_PGUP,197}
,{K_DEL,198},{K_END,199},{K_PGDOWN,200}
,{K_F1,201},{K_F8,202},{K_F9,203}
,{K_ESC,204},{K_UP,205},{K_SPACE,206}
,{K_LEFT,207},{K_DOWN,208},{K_RIGHT,209}
};
                         
char *CVKeyb::extranames[3*3*2]=
{
 "F4","F5","F6",
 "INS","HOME","PGUP",
 "DEL","END","PDWN",
 
 "F1","F8","F9",
 "ESC","UP","SPCE",
 "LEFT","DOWN","RGHT"
};

void CVKeyb::KeyFunc(int k)
{
 int shift=0;
 if (k>>8!=0) shift=1;
 k&=255;
 c=0;
 if ((k>=VKEYB_K_1)&&(k<VKEYB_K_1+9)) c=vkeymap[part*3*3+k-VKEYB_K_1][shift];
 if (k==K_F10) active=1-active;
 if (k==VKEYB_K_0) {c=-1; part=VKEYB_K_NPART-1;}
 if (k==VKEYB_K_ASTERISK) {c=-1; part--;}
 if (k==VKEYB_K_HASH) {c=-1; part++;}
 if (part<0) part=VKEYB_K_NPART-1;
 if (part>=VKEYB_K_NPART) part=0;
}

void CVKeyb::Draw(int shift)
{
 if (shift) shift=1;
 int i,j;
 char txt[2];
 txt[1]=0;
 Bar(VKEYB_POS,VKEYB_POS,VKEYB_POS+VKEYB_CELLSIZE*3,
 VKEYB_POS+VKEYB_CELLSIZE*3,CSys::Color[CSys::CPattern]);
 for(j=0; j<3; j++)
 {
 for(i=0; i<3; i++)
 {
  if (part==VKEYB_K_NPART-1)
  {
   DrawText3X5(VKEYB_POS+i*VKEYB_CELLSIZE+1,VKEYB_POS+j*VKEYB_CELLSIZE+1,
   CSys::Color[CSys::CTxt],extranames[shift*9+(j*3+i)]);
  }
  else
  {
   txt[0]=vkeymap[part*3*3+j*3+i][shift];
   DrawText(VKEYB_POS+i*VKEYB_CELLSIZE+2,VKEYB_POS+j*VKEYB_CELLSIZE+2,CSys::Color[CSys::CTxt],txt);
  }
  txt[0]=j*3+i+1+48;
  DrawText3X5(VKEYB_POS+i*VKEYB_CELLSIZE+12,VKEYB_POS+j*VKEYB_CELLSIZE+12,CSys::Color[CSys::CNum],txt);
 }
 HLine(VKEYB_POS,VKEYB_POS+j*VKEYB_CELLSIZE,VKEYB_POS+3*VKEYB_CELLSIZE,CSys::Color[CSys::CFaded]);
 }
 for(j=0; j<3; j++)
 VLine(VKEYB_POS+j*VKEYB_CELLSIZE,VKEYB_POS,VKEYB_POS+3*VKEYB_CELLSIZE,CSys::Color[CSys::CFaded]);
 HLine(VKEYB_POS,VKEYB_POS+3*VKEYB_CELLSIZE+1,VKEYB_POS+3*VKEYB_CELLSIZE,CSys::Color[CSys::CFaded]);
 VLine(VKEYB_POS+3*VKEYB_CELLSIZE+1,VKEYB_POS,VKEYB_POS+3*VKEYB_CELLSIZE+1,CSys::Color[CSys::CFaded]);
}

int CVKeyb::GetScan()
{
 if (c==0) return 0;
 if (c==-1) return 255;
 int r=0;
 if (part==VKEYB_K_NPART-1)
 for (int i=0; i<3*3*2; i++)
 {
  if (extrakeymap[i][1]==c) {r=extrakeymap[i][0]; break;}
 }
 else
 for (int i=0; i<KEYMAPLEN; i++)
 {
  if ((keymap[i][1]==c)||(keymap[i][2]==c)) {r=keymap[i][0]; break;}
 }
 c=0;
 return r;
}
