//ctex.c

int CTex::Load(char *fn)
{
 if (CExpr::MultiExec) return 0;
 unsigned int *wsk,*pwsk;
 unsigned int *bf; //bufor pomocniczy
 
 if (!CSys::LoadImage(fn,&bf,&width,&height)) return 0;

 //znajdz rozmiary tekstury
 int sw=1,lw=0;
 while ((sw<width)&&(lw<MAXMIPMAPS))
 {
  sw<<=1;
  lw++;
 }
 
 int sh=1,lh=0;
 while ((sh<height)&&(lh<MAXMIPMAPS))
 {
  sh<<=1;
  lh++;
 }
 
 int s;
 if (sw>sh)
 {
  s=sw;
  shift=lw;
 }
 else
 {
  s=sh;
  shift=lh;
 }
 
 unsigned int memreq=(s*s*4/3+width*height)*sizeof(unsigned int);
 unsigned int memavail=CSys::MemAvail();
 unsigned int minmemavail=CSys::MemTotal()/16;
 if (memavail<memreq+minmemavail) return 0;
 
 //alokuj pamiec dla bufora i mipmap
 int j=s;
 for (int i=0; i<=shift; i++)
 {
  CSys::FreeMem((void*)b[i]);
  b[i]=(unsigned int*)CSys::AllocMem(j*j*sizeof(unsigned int));
  j>>=1;
 }
 
 //skaluj bufor
 if ((width!=s)||(height!=s))
 {
  wsk=b[0];
  float dx=(float)width/s;
  float dy=(float)height/s;
  float x,y=0;

  unsigned char r,g,b;
  for (int j=0; j<s-1; j++)
  {
   x=0;
   for (int i=0; i<s-1; i++)
   {
    unsigned char *d=(unsigned char*)((int)bf+(ifloor(x)+ifloor(y)*width)*sizeof(unsigned int));
    float x1=x-floor(x),y1=y-floor(y);
    float b1=(float)*d;
    float b2=(float)*(d+4);
    float b3=(float)*(d+width*4+4);
    float b4=(float)*(d+width*4);
    float g1=(float)*(d+1);
    float g2=(float)*(d+4+1);
    float g3=(float)*(d+width*4+4+1);
    float g4=(float)*(d+width*4+1);
    float r1=(float)*(d+2);
    float r2=(float)*(d+4+2);
    float r3=(float)*(d+width*4+4+2);
    float r4=(float)*(d+width*4+2);
    r=ifloor((r1*(1-x1)+r2*x1)*(1-y1)+(r3*x1+r4*(1-x1))*y1);
    g=ifloor((g1*(1-x1)+g2*x1)*(1-y1)+(g3*x1+g4*(1-x1))*y1);
    b=ifloor((b1*(1-x1)+b2*x1)*(1-y1)+(b3*x1+b4*(1-x1))*y1);
    *wsk=RGB32(r,g,b)|0xff000000;
    wsk++;
    x+=dx;
   }
   *wsk=*(unsigned int*)((int)bf+(ifloor(x)+ifloor(y)*width)*sizeof(unsigned int));
   wsk++;
   y+=dy;
  }
  x=0;
  for (int i=0; i<s; i++)
  {
   *wsk=*(unsigned int*)((int)bf+(ifloor(x)+ifloor(y)*width)*sizeof(unsigned int));
   wsk++;
   x+=dx;
  }
  SetParameters(x0,y0,1,dy/dx);
 }
 else
 {
  memcpy(b[0],bf,s*s*sizeof(unsigned int));
  SetParameters(x0,y0,1,1);
 }
 
 if (fn!=0) CSys::FreeMem((void*)bf);
 
 width=height=s;
 
 //odwroc wzgledem osi y
 if (((cubemaptex_side>0)&&(cubemaptex_side<=6))&&
 (!((cubemaptex_side>=3)&&(cubemaptex_side<=4))))
 {
  bf=(unsigned int*)CSys::AllocMem(width*sizeof(unsigned int));
  for (int j=0; j<height; j++)
  {
   wsk=b[0]+j*width;
   memcpy(bf,wsk,width*sizeof(int));
   pwsk=bf+width-1;
   for (int i=0; i<width; i++)
   {
    *wsk=*pwsk;
    wsk++;
    pwsk--;
   }
  }
  CSys::FreeMem(bf);
 }
 
 //obroc wzgledem osi x
 if ((cubemaptex_side>=3)&&(cubemaptex_side<=4))
 {
  wsk=b[0];
  bf=(unsigned int*)CSys::AllocMem(width*height*sizeof(unsigned int));
  memcpy(bf,wsk,width*height*sizeof(unsigned int));
  for (int j=0; j<height; j++)
  {
   memcpy(wsk+j*width,bf+(height-j-1)*width,width*sizeof(unsigned int));
  }
  CSys::FreeMem(bf);
 }
 
 
 //buduj mipmapy
 BuildMipMaps(b,width,shift);
 
#ifdef CG
 if (cubemaptex_side==0)
 {
  //tworz mape normalnych traktujac teksture jako mape wysokosci
  if (nmapheight==0) nmapheight=10.f;
  BuildNormalMap(nmapheight);
 }
#endif
 
#ifdef OPENGL
 texbinded=0;
#endif
 return 1;
}

void CTex::BuildMipMaps(unsigned int *tbuf[],int twidth,int tshift)
{
 unsigned int *wsk,*pwsk;
 int j;
 j=twidth>>1;
 for (int i=1; i<=tshift; i++)
 {
  wsk=tbuf[i];
  for (int y=0; y<j; y++)
  {
   pwsk=tbuf[i-1]+(j<<2)*y;
   for (int x=0; x<j; x++)
   {
    int c1=*pwsk;
    int c2=*(pwsk+1);
    int c3=*(pwsk+(j<<1));
    int c4=*(pwsk+(j<<1)+1);
    int sr=(((c1>>16)&255)+((c2>>16)&255)+((c3>>16)&255)+((c4>>16)&255))/4;
    int sg=(((c1>>8)&255)+((c2>>8)&255)+((c3>>8)&255)+((c4>>8)&255))/4;
    int sb=(((c1)&255)+((c2)&255)+((c3)&255)+((c4)&255))/4;
    int sa=(((c1>>24)&255)+((c2>>24)&255)+((c3>>24)&255)+((c4>>24)&255))/4;
    *wsk=(sa<<24)+(sr<<16)+(sg<<8)+sb;
    wsk++;
    pwsk+=2;
   }
  }
  j>>=1;
 }
}

#ifdef CG
void CTex::BuildNormalMap(float h)
{
 if (!IsLoaded()) return;
 for (int i=0; i<=ntexshift; i++)
 {
  CSys::FreeMem((void*)nmap[i]);
 }
 ntexwidth=width;
 ntexheight=height;
 ntexshift=shift;
 nmapheight=h;
 
 int j=ntexwidth;
 for (int i=0; i<=ntexshift; i++)
 {
  nmap[i]=(unsigned int*)CSys::AllocMem(j*j*sizeof(unsigned int));
  j>>=1;
 }
 
 unsigned int *wsk,*wskd;
 for (int y=0; y<height; y++)
 {
  wsk=b[0]+y*width;
  wskd=nmap[0]+y*width;
  for (int x=0; x<width; x++)
  {
   float norm[3];
   unsigned int c0,c1,c2,c3,c4;
   float z0,z1,z2,z3,z4;
   
   c0=*wsk;
   
   if (y>0)
   c1=*(wsk-width);
   else
   c1=*wsk;
   
   if (x<width-1)
   c2=*(wsk+1);
   else
   c2=*wsk;
   
   if (y<height-1)
   c3=*(wsk+width);
   else
   c3=*wsk;
   
   if (x>0)
   c4=*(wsk-1);
   else
   c4=*wsk;
   
   z0=(float)(_MEM(&c0,0,unsigned char)+
              _MEM(&c0,1,unsigned char)+
              _MEM(&c0,2,unsigned char))
              *(1.f/3.f);
   
   z1=(float)(_MEM(&c1,0,unsigned char)+
              _MEM(&c1,1,unsigned char)+
              _MEM(&c1,2,unsigned char));
   z2=(float)(_MEM(&c2,0,unsigned char)+
              _MEM(&c2,1,unsigned char)+
              _MEM(&c2,2,unsigned char));
   z3=(float)(_MEM(&c3,0,unsigned char)+
              _MEM(&c3,1,unsigned char)+
              _MEM(&c3,2,unsigned char));
   z4=(float)(_MEM(&c4,0,unsigned char)+
              _MEM(&c4,1,unsigned char)+
              _MEM(&c4,2,unsigned char));
   
   norm[0]=(z4-z2)*nmapheight*(1.f/255.f/3.f);
   norm[2]=(z1-z3)*nmapheight*(1.f/255.f/3.f);

   float d=1.f/sqrt(norm[0]*norm[0]+norm[2]*norm[2]+1);
   norm[0]*=d;
   norm[1]=d;
   norm[2]*=d;
   
   norm[0]=0.5f*(norm[0]+1);
   norm[1]=0.5f*(norm[1]+1);
   norm[2]=0.5f*(norm[2]+1);
   
   *wskd=((((unsigned int)(z0*255.f))<<24)+
         (((unsigned int)(norm[0]*255.f))<<16)+
         (((unsigned int)(norm[1]*255.f))<<8)+
         (((unsigned int)(norm[2]*255.f))));
   wsk++; wskd++;
  }
 }
 BuildMipMaps(nmap,ntexwidth,ntexshift);
 texbinded=0;
}

int CTex::SetNormalMap(char *fn)
{
 if (!IsLoaded()) return 0;
 CTex *ntex=new CTex;
 ntex->SetAsCubeMapTex(100,0);
 if (!ntex->Load(fn)) return 0;
 
 for (int i=0; i<=shift; i++)
 {
  CSys::FreeMem((void*)nmap[i]);
 }
 ntexwidth=ntex->width;
 ntexheight=ntex->height;
 ntexshift=ntex->shift;
 int j=ntexwidth;
 
 for (int i=0; i<=ntexshift; i++)
 {
  nmap[i]=(unsigned int*)CSys::AllocMem(j*j*sizeof(unsigned int));
  j>>=1;
 }
 
 unsigned int *wsk,*wskd;
 for (int y=0; y<ntex->height; y++)
 {
  wsk=ntex->b[0]+y*ntex->width;
  wskd=nmap[0]+y*ntex->width;
  for (int x=0; x<ntex->width; x++)
  {
   _MEM(wskd,0,unsigned char)=255-_MEM(wsk,1,unsigned char);
   _MEM(wskd,1,unsigned char)=_MEM(wsk,0,unsigned char);
   _MEM(wskd,2,unsigned char)=_MEM(wsk,2,unsigned char);
   wsk++; wskd++;
  }
 }
 BuildMipMaps(nmap,ntexwidth,ntexshift);
 texbinded=0;
 delete ntex;
 return 1;
}
#endif

void CTex::SetAlpha(float a)
{
 if (CExpr::MultiExec==CExpr::MultiExec_Multi) return;
#ifdef OPENGL
 texbinded=0;
#endif
 int i,j;
 unsigned char c=ifloor(a*255.f);
 for (i=0; i<=shift; i++)
 {
  unsigned char *wsk=(unsigned char*)b[i];
  for (j=0; j<((width>>i)*(height>>i)); j++)
  {
   *(wsk+3)=c;
   wsk+=4;
  }
 }
}

int CTex::TexFunc1(char *s,float x1,float y1,float x2,float y2)
{
 if (CExpr::MultiExec==CExpr::MultiExec_Multi) return 0;
 if ((width<=0)||(height<=0)) return 0;
 float dx=(x2-x1)/(float)width;
 float dy=(y2-y1)/(float)height;
 int i,j;
 static CExpr e=CExpr(CExpr::GetLib());
 if (e.Set(s)!=CExpr::NoErr) return 0;
 unsigned int *wsk;
 wsk=b[0];
 for (i=0; i<height; i++)
 {
  expr_x=x1;
  expr_y=y1;
  for (j=0; j<width; j++)
  {
   unsigned char c=ifloor(e.Do()*255.f);
   *wsk=0xff000000|((c<<16)+(c<<8)+c);
   expr_x+=dx;
   wsk++;
  }
  y1+=dy;
 }
 BuildMipMaps(b,width,shift);
#ifdef OPENGL
 texbinded=0;
#endif
 return 1;
}

int CTex::TexFunc3(char *sr,char *sg,char *sb,float x1,float y1,float x2,float y2)
{
 if (CExpr::MultiExec==CExpr::MultiExec_Multi) return 0;
 if ((width<=0)||(height<=0)) return 0;
 float dx=(x2-x1)/(float)width;
 float dy=(y2-y1)/(float)height;
 int i,j;
 static CExpr er=CExpr(CExpr::GetLib());
 static CExpr eg=CExpr(CExpr::GetLib());
 static CExpr eb=CExpr(CExpr::GetLib());
 if (er.Set(sr)!=CExpr::NoErr) return 0;
 if (eg.Set(sg)!=CExpr::NoErr) return 0;
 if (eb.Set(sb)!=CExpr::NoErr) return 0;
 unsigned int *wsk;
 wsk=b[0];
 for (i=0; i<height; i++)
 {
  expr_x=x1;
  expr_y=y1;
  for (j=0; j<width; j++)
  {
   unsigned char cr=ifloor(er.Do()*255.f);
   unsigned char cg=ifloor(eg.Do()*255.f);
   unsigned char cb=ifloor(eb.Do()*255.f);
   *wsk=0xff000000|((cr<<16)+(cg<<8)+cb);
   expr_x+=dx;
   wsk++;
  }
  y1+=dy;
 }
 BuildMipMaps(b,width,shift);
#ifdef OPENGL
 texbinded=0;
#endif
 return 1;
}

int CTex::TexFunc4(char *sr,char *sg,char *sb,char *sa,float x1,float y1,float x2,float y2)
{
 if (CExpr::MultiExec==CExpr::MultiExec_Multi) return 0;
 if ((width<=0)||(height<=0)) return 0;
 float dx=(x2-x1)/(float)width;
 float dy=(y2-y1)/(float)height;
 int i,j;
 static CExpr er=CExpr(CExpr::GetLib());
 static CExpr eg=CExpr(CExpr::GetLib());
 static CExpr eb=CExpr(CExpr::GetLib());
 static CExpr ea=CExpr(CExpr::GetLib());
 if (er.Set(sr)!=CExpr::NoErr) return 0;
 if (eg.Set(sg)!=CExpr::NoErr) return 0;
 if (eb.Set(sb)!=CExpr::NoErr) return 0;
 if (ea.Set(sa)!=CExpr::NoErr) return 0;
 unsigned int *wsk;
 wsk=b[0];
 for (i=0; i<height; i++)
 {
  expr_x=x1;
  expr_y=y1;
  for (j=0; j<width; j++)
  {
   unsigned char cr=ifloor(er.Do()*255.f);
   unsigned char cg=ifloor(eg.Do()*255.f);
   unsigned char cb=ifloor(eb.Do()*255.f);
   unsigned char ca=ifloor(ea.Do()*255.f);
   *wsk=(ca<<24)+(cr<<16)+(cg<<8)+cb;
   expr_x+=dx;
   wsk++;
  }
  y1+=dy;
 }
 BuildMipMaps(b,width,shift);
#ifdef OPENGL
 texbinded=0;
#endif
 return 1;
}

CTex::~CTex()
{
 if (IsLoaded())
 {
  for (int i=0; i<=shift; i++)
  {
   CSys::FreeMem((void*)b[i]);
  }
#ifdef CG
  for (int i=0; i<=ntexshift; i++)
  {
   CSys::FreeMem((void*)nmap[i]);
  }
#endif
 }
}

#ifdef OPENGL

//mode:
//0-wylacz
//1-wlacz
//2-traktuj jak texture 2d z offsetem texid o cubemaptex_side+1, bez normalmapy - wlacz
//3-traktuj jak texture 2d bez normalmapy - wylacz

#define SETGL_OFF 0
#define SETGL_ON  1
#define SETGL_TEX_CUBE_MAP_AS_2D_ON  2
#define SETGL_TEX_CUBE_MAP_AS_2D_OFF 3

void CTex::SetGL(int mode) 
{
 if ((cubemaptex_side>0)&&(mode<=SETGL_ON))
 {
#ifdef GL_TEXTURE_CUBE_MAP
  if (mode==SETGL_OFF)
  {
   glDisable(GL_TEXTURE_CUBE_MAP);
   return;
  }
  else
  {
   glEnable(GL_TEXTURE_CUBE_MAP);
  }
  glBindTexture(GL_TEXTURE_CUBE_MAP,texid);
  
  COpenGL::SetTexPar(GL_TEXTURE_CUBE_MAP);
  
  int w=0;
  glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X+cubemaptex_side-1,
  0,GL_TEXTURE_WIDTH,&w);
  if (!((texbinded==2)&&(w>0)))
  {
   for (int i=0; i<=shift; i++)
   {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+cubemaptex_side-1,
    i,GL_RGBA,width>>i,height>>i,0,
    GL_BGRA,GL_UNSIGNED_BYTE,b[i]);
   }
  }
  texbinded=2;
  return;
#else
  return;
#endif
 }

 
 if (mode==SETGL_OFF)
 {
  glDisable(GL_TEXTURE_2D);

#ifdef CG
  if (CGraph::GetGraph()->UseShaders())
  {
   glActiveTexture(GL_TEXTURE1);
   glDisable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
  }
#endif
  return;
 }
 
 if (mode==SETGL_TEX_CUBE_MAP_AS_2D_OFF)
 {
  glDisable(GL_TEXTURE_2D);
  return;
 }

 if (mode!=SETGL_TEX_CUBE_MAP_AS_2D_ON)
 {
  if (texid==0)
  {
   glGenTextures(1,(GLuint*)&texid);
  }
 }
 else
 texid+=cubemaptex_side+1;
 
 glBindTexture(GL_TEXTURE_2D,texid);
 
 int w=0;
 int sh=-1;
 glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&w);
 
 if ((texbinded)&&(w>0)) 
 {
  goto set;
 }
 
 int s;
 glGetIntegerv(GL_MAX_TEXTURE_SIZE,&s);
 
 while (s)
 {
  sh++;
  s>>=1;
 }
 int shift0;
 if (sh<shift) shift0=shift-sh;
 else shift0=0;

 for (int i=shift0; i<=shift; i++)
 {
  glTexImage2D(GL_TEXTURE_2D,i-shift0,GL_RGBA,width>>i,height>>i,0,
  GL_BGRA,GL_UNSIGNED_BYTE,b[i]);
 }

 texbinded=1;
#ifdef CG
 if (mode==SETGL_TEX_CUBE_MAP_AS_2D_ON) goto set;
 
 //normalmapa
 if (ntexid==0)
 {
  glGenTextures(1,(GLuint*)&ntexid);
 }

 glBindTexture(GL_TEXTURE_2D,ntexid);
 
 if (sh<ntexshift) shift0=ntexshift-sh;
 else shift0=0;
 
 for (int i=shift0; i<=ntexshift; i++)
 {
  glTexImage2D(GL_TEXTURE_2D,i-shift0,GL_RGBA,ntexwidth>>i,ntexheight>>i,0,
  GL_BGRA,GL_UNSIGNED_BYTE,nmap[i]);
 }

#endif
set:
#ifdef CG
 if (CGraph::GetGraph()->UseShaders())
 {
  glActiveTexture(GL_TEXTURE0);
 }
#endif
 glEnable(GL_TEXTURE_2D);
 COpenGL::SetTexPar();
 glBindTexture(GL_TEXTURE_2D,texid);
 
 if (mode==SETGL_TEX_CUBE_MAP_AS_2D_ON) 
 {
  texid-=cubemaptex_side+1;
  return;
 }

#ifdef CG
 if (CGraph::GetGraph()->UseShaders())
 {
  glActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_2D);
  COpenGL::SetTexPar();
  glBindTexture(GL_TEXTURE_2D,ntexid);
  glActiveTexture(GL_TEXTURE0);
 }
#endif
}
#endif
